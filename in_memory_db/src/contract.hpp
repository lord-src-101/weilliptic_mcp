#ifndef IN_MEMORY_DB_CONTRACT_HPP
#define IN_MEMORY_DB_CONTRACT_HPP

#include <string>
#include <tuple>
#include <optional>
#include <vector>
#include <algorithm> // std::remove, std::find
#include <utility>   // std::pair, std::tuple

// Necessary includes for the implementation logic
#include "weilsdk/runtime.h"
#include "weilsdk/collections/map.hpp"
#include "weilsdk/collections/vector.hpp"
#include "external/nlohmann.hpp"

// Define Option as an alias for std::optional
template <typename T>
using Option = std::optional<T>;

// Serialization for Option types
template <typename T>
void to_json(nlohmann::ordered_json& j, const Option<T>& opt) {
    if (opt.has_value()) {
        j = opt.value();
    } else {
        j = nullptr;
    }
}

template <typename T>
void from_json(const nlohmann::ordered_json& j, Option<T>& opt) {
    if (j.is_null()) {
        opt = std::nullopt;
    } else {
        opt = j.get<T>();
    }
}


class in_memory_db_ContractState {
    private:
    /*
     * -------------------------------------------------------------------------
     * O(1) DATA STRUCTURES
     * -------------------------------------------------------------------------
     */
    
    // 1. Metadata: Master list of table names
    collections::WeilMap<std::string, std::vector<std::string>> metadata_registry =
        collections::WeilMap<std::string, std::vector<std::string>>(static_cast<uint8_t>(1));

    // 2. Data Store: key = "table|record_key" -> JSON string
    collections::WeilMap<std::string, std::string> store =
        collections::WeilMap<std::string, std::string>(static_cast<uint8_t>(2));

    // 3. Counts: key = "table" -> count (uint64_t)
    collections::WeilMap<std::string, uint64_t> table_counts =
        collections::WeilMap<std::string, uint64_t>(static_cast<uint8_t>(3));

    // 4. Index-to-Key: key = "table|index" -> "record_key"
    collections::WeilMap<std::string, std::string> index_to_key =
        collections::WeilMap<std::string, std::string>(static_cast<uint8_t>(4));

    // 5. Key-to-Index: key = "table|record_key" -> index (uint64_t)
    collections::WeilMap<std::string, uint64_t> key_to_index =
        collections::WeilMap<std::string, uint64_t>(static_cast<uint8_t>(5));

    // --- Helpers ---
    
    std::vector<std::string> get_tables_list_internal() {
        if (metadata_registry.contains(std::string("__list__"))) {
            return metadata_registry.get(std::string("__list__"));
        }
        return std::vector<std::string>{};
    }

    void set_tables_list_internal(const std::vector<std::string>& names) {
        metadata_registry.insert(std::string("__list__"), names);
    }

    bool table_exists_persisted(const std::string& table_name) {
        std::vector<std::string> list = get_tables_list_internal();
        return std::find(list.begin(), list.end(), table_name) != list.end();
    }

    static std::string make_record_key(const std::string& table, const std::string& key) {
        return table + "|" + key;
    }

    static std::string make_index_key(const std::string& table, uint64_t index) {
        return table + "|" + std::to_string(index);
    }
    
    static bool is_safe(const std::string& s) {
        return s.find('|') == std::string::npos;
    }

    public:
    in_memory_db_ContractState() = default;

    // Mutate
    int32_t create_table(const std::string &table_name) {
        if (!is_safe(table_name)) return 400;
        
        std::vector<std::string> master = get_tables_list_internal();
        if (std::find(master.begin(), master.end(), table_name) != master.end()) {
            return 409; // Already exists
        }
        master.push_back(table_name);
        set_tables_list_internal(master);
        table_counts.insert(table_name, 0);
        return 200;
    }

    // Mutate
// Mutate - O(N) - Reclaims Storage
    int32_t drop_table(const std::string &table_name) {
        if (!table_exists_persisted(table_name)) return 404;

        uint64_t count = 0;
        if (table_counts.contains(table_name)) {
            count = table_counts.get(table_name);
        }

        // --- DANGER ZONE: GAS LIMIT ---
        // If 'count' is huge (e.g. > 2000), this loop might cause the transaction 
        // to fail (Out of Gas). If that happens, you must delete records manually 
        // using remove_record() before calling drop_table().
        for (uint64_t i = 0; i < count; ++i) {
            std::string idx_key = make_index_key(table_name, i);
            
            // We need the user_key to find the other map entries
            if (index_to_key.contains(idx_key)) {
                std::string user_key = index_to_key.get(idx_key);
                std::string composite = make_record_key(table_name, user_key);

                // 1. Delete the Data Record
                store.remove(composite);

                // 2. Delete the Lookup (Key -> Index)
                key_to_index.remove(composite);
            }

            // 3. Delete the Index (Index -> Key)
            index_to_key.remove(idx_key);
        }
        // ------------------------------

        // Final Metadata Cleanup
        std::vector<std::string> master = get_tables_list_internal();
        auto it = std::remove(master.begin(), master.end(), table_name);
        if (it != master.end()) {
            master.erase(it, master.end());
            set_tables_list_internal(master);
        }
        
        // Remove the count
        table_counts.remove(table_name);

        return 200;
    }

    // Query
    std::vector<std::string> list_tables() {
        return get_tables_list_internal();
    }

    // Query
    int32_t table_size(const std::string &table_name) {
        if (table_counts.contains(table_name)) {
            return static_cast<int32_t>(table_counts.get(table_name));
        }
        return 0;
    }

    // Mutate
    int32_t insert(const std::string &table, const std::string &key, const std::string &field, const std::string &value) {
        if (!table_exists_persisted(table)) return 404;
        if (!is_safe(key)) return 400;

        std::string composite = make_record_key(table, key);

        // O(1) Indexing logic
        if (!store.contains(composite)) {
            uint64_t count = 0;
            if (table_counts.contains(table)) count = table_counts.get(table);

            index_to_key.insert(make_index_key(table, count), key);
            key_to_index.insert(composite, count);
            table_counts.insert(table, count + 1);
        }

        nlohmann::ordered_json j;
        if (store.contains(composite)) {
            try { j = nlohmann::ordered_json::parse(store.get(composite)); } catch(...) { j = nlohmann::ordered_json::object(); }
        } else {
            j = nlohmann::ordered_json::object();
        }

        j[field] = value;
        store.insert(composite, j.dump());
        return 200;
    }

    // Mutate
    int32_t update(const std::string &table, const std::string &key, const std::string &field, const std::string &value) {
        if (!table_exists_persisted(table)) return 404;
        std::string composite = make_record_key(table, key);
        
        if (!store.contains(composite)) return 404; // Should return 404 if record doesn't exist

        std::string raw = store.get(composite);
        nlohmann::ordered_json j;
        try { j = nlohmann::ordered_json::parse(raw); } catch(...) { return 500; }

        j[field] = value;
        store.insert(composite, j.dump());
        return 200;
    }

    // Query
    std::optional<std::string> get_value(const std::string &table, const std::string &key, const std::string &field) {
        if (!table_exists_persisted(table)) return std::nullopt;
        std::string composite = make_record_key(table, key);
        if (!store.contains(composite)) return std::nullopt;

        try {
            nlohmann::ordered_json j = nlohmann::ordered_json::parse(store.get(composite));
            if (j.contains(field)) {
                auto v = j[field];
                return v.is_string() ? std::optional<std::string>(v.get<std::string>()) : std::optional<std::string>(v.dump());
            }
        } catch (...) {}
        return std::nullopt;
    }

    // Mutate
    int32_t remove_field(const std::string &table, const std::string &key, const std::string &field) {
        if (!table_exists_persisted(table)) return 404;
        std::string composite = make_record_key(table, key);
        if (!store.contains(composite)) return 404;

        nlohmann::ordered_json j;
        try { j = nlohmann::ordered_json::parse(store.get(composite)); } catch(...) { return 500; }

        if (j.contains(field)) {
            j.erase(field);
            if (j.empty()) {
                return remove_record(table, key);
            }
            store.insert(composite, j.dump());
        }
        return 200;
    }

    // Mutate - O(1) via Swap-and-Pop
    int32_t remove_record(const std::string &table, const std::string &key) {
        if (!table_exists_persisted(table)) return 404;
        std::string composite = make_record_key(table, key);
        if (!store.contains(composite)) return 404;

        // 1. Remove Data
        store.remove(composite);

        // 2. Fix Index
        uint64_t count = table_counts.get(table);
        uint64_t index_to_remove = key_to_index.get(composite);
        uint64_t last_index = count - 1;

        if (index_to_remove != last_index) {
            std::string last_key_idx_str = make_index_key(table, last_index);
            std::string last_key = index_to_key.get(last_key_idx_str);
            std::string last_key_composite = make_record_key(table, last_key);

            // Move last key to the empty slot
            index_to_key.insert(make_index_key(table, index_to_remove), last_key);
            key_to_index.insert(last_key_composite, index_to_remove);
        }

        // Cleanup tail
        index_to_key.remove(make_index_key(table, last_index));
        key_to_index.remove(composite);
        table_counts.insert(table, last_index); // count - 1

        return 200;
    }

    // Mutate
    int32_t insert_record(const std::string &table, const std::string &key, const std::vector<std::tuple<std::string, std::string>> &fields) {
        std::vector<std::tuple<std::string, std::vector<std::tuple<std::string, std::string>>>> batch;
        batch.push_back({key, fields});
        return insert_records(table, batch) > 0 ? 200 : 500;
    }

    // Mutate
    int32_t insert_records(const std::string &table, const std::vector<std::tuple<std::string, std::vector<std::tuple<std::string, std::string>>>> &records) {
        if (!table_exists_persisted(table)) return 0;
        int32_t success = 0;

        for (const auto& rec : records) {
            std::string key = std::get<0>(rec);
            if (!is_safe(key)) continue;

            std::string composite = make_record_key(table, key);
            
            // Register Index if new
            if (!store.contains(composite)) {
                uint64_t count = 0;
                if (table_counts.contains(table)) count = table_counts.get(table);
                
                index_to_key.insert(make_index_key(table, count), key);
                key_to_index.insert(composite, count);
                table_counts.insert(table, count + 1);
            }

            // Build JSON
            nlohmann::ordered_json j;
            if (store.contains(composite)) {
                 try { j = nlohmann::ordered_json::parse(store.get(composite)); } catch(...) {}
            } else { j = nlohmann::ordered_json::object(); }

            for (const auto& f : std::get<1>(rec)) {
                j[std::get<0>(f)] = std::get<1>(f);
            }

            store.insert(composite, j.dump());
            success++;
        }
        return success;
    }

    // Query
    std::vector<std::tuple<std::string, std::string>> get_fields(const std::string &table, const std::string &key, const std::vector<std::string> &fields) {
        std::vector<std::tuple<std::string, std::string>> out;
        if (!table_exists_persisted(table)) return out;

        std::string composite = make_record_key(table, key);
        if (!store.contains(composite)) return out;

        nlohmann::ordered_json j;
        try { j = nlohmann::ordered_json::parse(store.get(composite)); } catch(...) { return out; }

        for (const auto& f : fields) {
            if (j.contains(f)) {
                auto val = j[f];
                out.emplace_back(f, val.is_string() ? val.get<std::string>() : val.dump());
            }
        }
        return out;
    }

    // Query
    std::vector<std::tuple<std::string, std::string>> get_all_fields(const std::string &table, const std::string &key) {
        std::vector<std::tuple<std::string, std::string>> out;
        if (!table_exists_persisted(table)) return out;

        std::string composite = make_record_key(table, key);
        if (!store.contains(composite)) return out;

        try {
            nlohmann::ordered_json j = nlohmann::ordered_json::parse(store.get(composite));
            for (auto& el : j.items()) {
                out.emplace_back(el.key(), el.value().is_string() ? el.value().get<std::string>() : el.value().dump());
            }
        } catch (...) {}
        return out;
    }


        std::string tools() const {
        return R"JSON(        [
  {
    "type": "function",
    "function": {
      "name": "create_table",
      "description": "creates a table (returns 200 success, 409 already exists, 400 invalid name)\n",
      "parameters": {
        "type": "object",
        "properties": {
          "table_name": {
            "type": "string",
            "description": "name of the table to be created\n"
          }
        },
        "required": [
          "table_name"
        ]
      }
    }
  },
  {
    "type": "function",
    "function": {
      "name": "drop_table",
      "description": "drops/deletes a table (returns 200 success, 404 table not found)\n",
      "parameters": {
        "type": "object",
        "properties": {
          "table_name": {
            "type": "string",
            "description": "name of the table to be deleted\n"
          }
        },
        "required": [
          "table_name"
        ]
      }
    }
  },
  {
    "type": "function",
    "function": {
      "name": "list_tables",
      "description": "list all the tables present in db now (returns list of table names, empty if none)\n",
      "parameters": {
        "type": "object",
        "properties": {},
        "required": []
      }
    }
  },
  {
    "type": "function",
    "function": {
      "name": "table_size",
      "description": "gives size of any table (returns record count, 404 table not found)\n",
      "parameters": {
        "type": "object",
        "properties": {
          "table_name": {
            "type": "string",
            "description": "name of the table\n"
          }
        },
        "required": [
          "table_name"
        ]
      }
    }
  },
  {
    "type": "function",
    "function": {
      "name": "insert",
      "description": "inserts field-value pair for a given key (returns 200 success, 404 table missing, 400 invalid input)\n",
      "parameters": {
        "type": "object",
        "properties": {
          "table": {
            "type": "string",
            "description": "name of the table\n"
          },
          "key": {
            "type": "string",
            "description": "the key / primary key of the record\n"
          },
          "field": {
            "type": "string",
            "description": "the field name inside the record\n"
          },
          "value": {
            "type": "string",
            "description": "the value associated with the field\n"
          }
        },
        "required": [
          "table",
          "key",
          "field",
          "value"
        ]
      }
    }
  },
  {
    "type": "function",
    "function": {
      "name": "update",
      "description": "updates a field value for a given key (returns 200 success, 404 table or record missing)\n",
      "parameters": {
        "type": "object",
        "properties": {
          "table": {
            "type": "string",
            "description": "name of the table\n"
          },
          "key": {
            "type": "string",
            "description": "the key / primary key of the record\n"
          },
          "field": {
            "type": "string",
            "description": "the field name to update\n"
          },
          "value": {
            "type": "string",
            "description": "the new value of the field\n"
          }
        },
        "required": [
          "table",
          "key",
          "field",
          "value"
        ]
      }
    }
  },
  {
    "type": "function",
    "function": {
      "name": "get_value",
      "description": "get a single field value (returns Some(value) if found, None otherwise)\n",
      "parameters": {
        "type": "object",
        "properties": {
          "table": {
            "type": "string",
            "description": "name of the table\n"
          },
          "key": {
            "type": "string",
            "description": "the key / primary key of the record\n"
          },
          "field": {
            "type": "string",
            "description": "the field name whose value is requested\n"
          }
        },
        "required": [
          "table",
          "key",
          "field"
        ]
      }
    }
  },
  {
    "type": "function",
    "function": {
      "name": "remove_field",
      "description": "removes a field from a record (returns 200 success, 404 table or record missing)\n",
      "parameters": {
        "type": "object",
        "properties": {
          "table": {
            "type": "string",
            "description": "name of the table\n"
          },
          "key": {
            "type": "string",
            "description": "the key / primary key of the record\n"
          },
          "field": {
            "type": "string",
            "description": "the field name to be removed\n"
          }
        },
        "required": [
          "table",
          "key",
          "field"
        ]
      }
    }
  },
  {
    "type": "function",
    "function": {
      "name": "remove_record",
      "description": "removes a complete record (returns 200 success, 404 table missing)\n",
      "parameters": {
        "type": "object",
        "properties": {
          "table": {
            "type": "string",
            "description": "name of the table\n"
          },
          "key": {
            "type": "string",
            "description": "the key / primary key of the record\n"
          }
        },
        "required": [
          "table",
          "key"
        ]
      }
    }
  },
  {
    "type": "function",
    "function": {
      "name": "get_fields",
      "description": "retrieve multiple fields from a record (returns list of existing field-value pairs)\n",
      "parameters": {
        "type": "object",
        "properties": {
          "table": {
            "type": "string",
            "description": "name of the table\n"
          },
          "key": {
            "type": "string",
            "description": "the key / primary key of the record\n"
          },
          "fields": {
            "type": "array",
            "description": "list of field names to retrieve\n"
          }
        },
        "required": [
          "table",
          "key",
          "fields"
        ]
      }
    }
  },
  {
    "type": "function",
    "function": {
      "name": "get_all_fields",
      "description": "retrieve the full record (returns all field-value pairs, empty if record missing)\n",
      "parameters": {
        "type": "object",
        "properties": {
          "table": {
            "type": "string",
            "description": "name of the table\n"
          },
          "key": {
            "type": "string",
            "description": "the key / primary key of the record\n"
          }
        },
        "required": [
          "table",
          "key"
        ]
      }
    }
  }
])JSON";
    }

    // JSON serialization/deserialization
    friend void to_json(nlohmann::ordered_json &j, const in_memory_db_ContractState &obj);
    friend void from_json(const nlohmann::ordered_json &j, in_memory_db_ContractState &obj);
};

inline void to_json(nlohmann::ordered_json &j, const in_memory_db_ContractState &obj) {
    j = nlohmann::ordered_json::object();
    // We only expose the list of tables to avoid massive dumps.
    // NOTE: 'obj' is const, so we can't call non-const helper get_tables_list_internal().
    // We access the map directly or cast away const (safer to access map directly if public, but map is private).
    // However, since we are inside a friend function, we can access private members.
    if (obj.metadata_registry.contains(std::string("__list__"))) {
        j["tables"] = obj.metadata_registry.get(std::string("__list__"));
    }
}

inline void from_json(const nlohmann::ordered_json &j, in_memory_db_ContractState &obj) {
    if (j.contains("tables") && j["tables"].is_array()) {
        obj.metadata_registry.insert(std::string("__list__"), j["tables"].get<std::vector<std::string>>());
    }
}


#endif // IN_MEMORY_DB_CONTRACT_HPP