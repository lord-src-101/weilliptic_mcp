#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <optional>
#include <variant>
#include "weilsdk/error.h"
#include "weilsdk/utils.h"
#include "weilsdk/runtime.h"
#include "weilsdk/ledger.h"
#include "contract.hpp"

// Function declarations
extern "C" int __new(size_t len, unsigned char _id) __attribute__((export_name("__new")));
extern "C" void __free(size_t ptr, size_t len) __attribute__((export_name("__free")));
extern "C" void init() __attribute__((export_name("init")));
extern "C" void method_kind_data() __attribute__((export_name("method_kind_data")));
extern "C" void create_table() __attribute__((export_name("create_table")));
extern "C" void drop_table() __attribute__((export_name("drop_table")));
extern "C" void list_tables() __attribute__((export_name("list_tables")));
extern "C" void table_size() __attribute__((export_name("table_size")));
extern "C" void insert() __attribute__((export_name("insert")));
extern "C" void update() __attribute__((export_name("update")));
extern "C" void get_value() __attribute__((export_name("get_value")));
extern "C" void remove_field() __attribute__((export_name("remove_field")));
extern "C" void remove_record() __attribute__((export_name("remove_record")));
extern "C" void insert_record() __attribute__((export_name("insert_record")));
extern "C" void insert_records() __attribute__((export_name("insert_records")));
extern "C" void get_fields() __attribute__((export_name("get_fields")));
extern "C" void get_all_fields() __attribute__((export_name("get_all_fields")));
extern "C" void tools() __attribute__((export_name("tools")));

// Global contract state instance
in_memory_db_ContractState in_memory_db_instance;

// Argument structs for contract methods

struct create_table_args {
    std::string table_name;

    
    friend void to_json(nlohmann::ordered_json &j, const create_table_args &obj) {
        j = nlohmann::ordered_json::object();

            j["table_name"] = obj.table_name;
    }
    
    friend void from_json(const nlohmann::ordered_json &j, create_table_args &obj) {
        try {

            if (!j.contains("table_name")) {
                throw std::runtime_error("Missing required field 'table_name'");
            }
            j.at("table_name").get_to(obj.table_name);
        } catch (const std::exception& e) {
            throw std::runtime_error("Invalid argument format");
        }
    }
    
};
struct drop_table_args {
    std::string table_name;

    
    friend void to_json(nlohmann::ordered_json &j, const drop_table_args &obj) {
        j = nlohmann::ordered_json::object();

            j["table_name"] = obj.table_name;
    }
    
    friend void from_json(const nlohmann::ordered_json &j, drop_table_args &obj) {
        try {

            if (!j.contains("table_name")) {
                throw std::runtime_error("Missing required field 'table_name'");
            }
            j.at("table_name").get_to(obj.table_name);
        } catch (const std::exception& e) {
            throw std::runtime_error("Invalid argument format");
        }
    }
    
};
struct table_size_args {
    std::string table_name;

    
    friend void to_json(nlohmann::ordered_json &j, const table_size_args &obj) {
        j = nlohmann::ordered_json::object();

            j["table_name"] = obj.table_name;
    }
    
    friend void from_json(const nlohmann::ordered_json &j, table_size_args &obj) {
        try {

            if (!j.contains("table_name")) {
                throw std::runtime_error("Missing required field 'table_name'");
            }
            j.at("table_name").get_to(obj.table_name);
        } catch (const std::exception& e) {
            throw std::runtime_error("Invalid argument format");
        }
    }
    
};
struct insert_args {
    std::string table;
    std::string key;
    std::string field;
    std::string value;

    
    friend void to_json(nlohmann::ordered_json &j, const insert_args &obj) {
        j = nlohmann::ordered_json::object();

            j["table"] = obj.table;

            j["key"] = obj.key;

            j["field"] = obj.field;

            j["value"] = obj.value;
    }
    
    friend void from_json(const nlohmann::ordered_json &j, insert_args &obj) {
        try {

            if (!j.contains("table")) {
                throw std::runtime_error("Missing required field 'table'");
            }
            j.at("table").get_to(obj.table);

            if (!j.contains("key")) {
                throw std::runtime_error("Missing required field 'key'");
            }
            j.at("key").get_to(obj.key);

            if (!j.contains("field")) {
                throw std::runtime_error("Missing required field 'field'");
            }
            j.at("field").get_to(obj.field);

            if (!j.contains("value")) {
                throw std::runtime_error("Missing required field 'value'");
            }
            j.at("value").get_to(obj.value);
        } catch (const std::exception& e) {
            throw std::runtime_error("Invalid argument format");
        }
    }
    
};
struct update_args {
    std::string table;
    std::string key;
    std::string field;
    std::string value;

    
    friend void to_json(nlohmann::ordered_json &j, const update_args &obj) {
        j = nlohmann::ordered_json::object();

            j["table"] = obj.table;

            j["key"] = obj.key;

            j["field"] = obj.field;

            j["value"] = obj.value;
    }
    
    friend void from_json(const nlohmann::ordered_json &j, update_args &obj) {
        try {

            if (!j.contains("table")) {
                throw std::runtime_error("Missing required field 'table'");
            }
            j.at("table").get_to(obj.table);

            if (!j.contains("key")) {
                throw std::runtime_error("Missing required field 'key'");
            }
            j.at("key").get_to(obj.key);

            if (!j.contains("field")) {
                throw std::runtime_error("Missing required field 'field'");
            }
            j.at("field").get_to(obj.field);

            if (!j.contains("value")) {
                throw std::runtime_error("Missing required field 'value'");
            }
            j.at("value").get_to(obj.value);
        } catch (const std::exception& e) {
            throw std::runtime_error("Invalid argument format");
        }
    }
    
};
struct get_value_args {
    std::string table;
    std::string key;
    std::string field;

    
    friend void to_json(nlohmann::ordered_json &j, const get_value_args &obj) {
        j = nlohmann::ordered_json::object();

            j["table"] = obj.table;

            j["key"] = obj.key;

            j["field"] = obj.field;
    }
    
    friend void from_json(const nlohmann::ordered_json &j, get_value_args &obj) {
        try {

            if (!j.contains("table")) {
                throw std::runtime_error("Missing required field 'table'");
            }
            j.at("table").get_to(obj.table);

            if (!j.contains("key")) {
                throw std::runtime_error("Missing required field 'key'");
            }
            j.at("key").get_to(obj.key);

            if (!j.contains("field")) {
                throw std::runtime_error("Missing required field 'field'");
            }
            j.at("field").get_to(obj.field);
        } catch (const std::exception& e) {
            throw std::runtime_error("Invalid argument format");
        }
    }
    
};
struct remove_field_args {
    std::string table;
    std::string key;
    std::string field;

    
    friend void to_json(nlohmann::ordered_json &j, const remove_field_args &obj) {
        j = nlohmann::ordered_json::object();

            j["table"] = obj.table;

            j["key"] = obj.key;

            j["field"] = obj.field;
    }
    
    friend void from_json(const nlohmann::ordered_json &j, remove_field_args &obj) {
        try {

            if (!j.contains("table")) {
                throw std::runtime_error("Missing required field 'table'");
            }
            j.at("table").get_to(obj.table);

            if (!j.contains("key")) {
                throw std::runtime_error("Missing required field 'key'");
            }
            j.at("key").get_to(obj.key);

            if (!j.contains("field")) {
                throw std::runtime_error("Missing required field 'field'");
            }
            j.at("field").get_to(obj.field);
        } catch (const std::exception& e) {
            throw std::runtime_error("Invalid argument format");
        }
    }
    
};
struct remove_record_args {
    std::string table;
    std::string key;

    
    friend void to_json(nlohmann::ordered_json &j, const remove_record_args &obj) {
        j = nlohmann::ordered_json::object();

            j["table"] = obj.table;

            j["key"] = obj.key;
    }
    
    friend void from_json(const nlohmann::ordered_json &j, remove_record_args &obj) {
        try {

            if (!j.contains("table")) {
                throw std::runtime_error("Missing required field 'table'");
            }
            j.at("table").get_to(obj.table);

            if (!j.contains("key")) {
                throw std::runtime_error("Missing required field 'key'");
            }
            j.at("key").get_to(obj.key);
        } catch (const std::exception& e) {
            throw std::runtime_error("Invalid argument format");
        }
    }
    
};
struct insert_record_args {
    std::string table;
    std::string key;
    std::vector<std::tuple<std::string, std::string>> fields;

    
    friend void to_json(nlohmann::ordered_json &j, const insert_record_args &obj) {
        j = nlohmann::ordered_json::object();

            j["table"] = obj.table;

            j["key"] = obj.key;

            j["fields"] = obj.fields;
    }
    
    friend void from_json(const nlohmann::ordered_json &j, insert_record_args &obj) {
        try {

            if (!j.contains("table")) {
                throw std::runtime_error("Missing required field 'table'");
            }
            j.at("table").get_to(obj.table);

            if (!j.contains("key")) {
                throw std::runtime_error("Missing required field 'key'");
            }
            j.at("key").get_to(obj.key);

            if (!j.contains("fields")) {
                throw std::runtime_error("Missing required field 'fields'");
            }
            j.at("fields").get_to(obj.fields);
        } catch (const std::exception& e) {
            throw std::runtime_error("Invalid argument format");
        }
    }
    
};
struct insert_records_args {
    std::string table;
    std::vector<std::tuple<std::string, std::vector<std::tuple<std::string, std::string>>>> records;

    
    friend void to_json(nlohmann::ordered_json &j, const insert_records_args &obj) {
        j = nlohmann::ordered_json::object();

            j["table"] = obj.table;

            j["records"] = obj.records;
    }
    
    friend void from_json(const nlohmann::ordered_json &j, insert_records_args &obj) {
        try {

            if (!j.contains("table")) {
                throw std::runtime_error("Missing required field 'table'");
            }
            j.at("table").get_to(obj.table);

            if (!j.contains("records")) {
                throw std::runtime_error("Missing required field 'records'");
            }
            j.at("records").get_to(obj.records);
        } catch (const std::exception& e) {
            throw std::runtime_error("Invalid argument format");
        }
    }
    
};
struct get_fields_args {
    std::string table;
    std::string key;
    std::vector<std::string> fields;

    
    friend void to_json(nlohmann::ordered_json &j, const get_fields_args &obj) {
        j = nlohmann::ordered_json::object();

            j["table"] = obj.table;

            j["key"] = obj.key;

            j["fields"] = obj.fields;
    }
    
    friend void from_json(const nlohmann::ordered_json &j, get_fields_args &obj) {
        try {

            if (!j.contains("table")) {
                throw std::runtime_error("Missing required field 'table'");
            }
            j.at("table").get_to(obj.table);

            if (!j.contains("key")) {
                throw std::runtime_error("Missing required field 'key'");
            }
            j.at("key").get_to(obj.key);

            if (!j.contains("fields")) {
                throw std::runtime_error("Missing required field 'fields'");
            }
            j.at("fields").get_to(obj.fields);
        } catch (const std::exception& e) {
            throw std::runtime_error("Invalid argument format");
        }
    }
    
};
struct get_all_fields_args {
    std::string table;
    std::string key;

    
    friend void to_json(nlohmann::ordered_json &j, const get_all_fields_args &obj) {
        j = nlohmann::ordered_json::object();

            j["table"] = obj.table;

            j["key"] = obj.key;
    }
    
    friend void from_json(const nlohmann::ordered_json &j, get_all_fields_args &obj) {
        try {

            if (!j.contains("table")) {
                throw std::runtime_error("Missing required field 'table'");
            }
            j.at("table").get_to(obj.table);

            if (!j.contains("key")) {
                throw std::runtime_error("Missing required field 'key'");
            }
            j.at("key").get_to(obj.key);
        } catch (const std::exception& e) {
            throw std::runtime_error("Invalid argument format");
        }
    }
    
};
extern "C" {

    int __new(size_t len, unsigned char _id) {
        void *ptr = weilsdk::Runtime::allocate(len);
        return reinterpret_cast<int>(ptr);
    }
    
    void __free(size_t ptr, size_t len) {
        weilsdk::Runtime::deallocate(ptr, len);
    }
    
    
    // Initialize contract state
    void init() {
        in_memory_db_ContractState new_instance;
        // TODO: Initialize contract state as needed
    
        nlohmann::ordered_json j = new_instance;
        weilsdk::WeilValue wv;
        wv.new_with_state_and_ok_value(j.dump(), "null");
        weilsdk::Runtime::setStateAndResult(std::variant<weilsdk::WeilValue,std::string> {wv});
    }
    
    
    // Method kind data collection
    void method_kind_data() {
        std::map<std::string, std::string> method_kind_mapping;
        
        
        method_kind_mapping["create_table"] = "mutate";    
        method_kind_mapping["drop_table"] = "mutate";    
        method_kind_mapping["list_tables"] = "query";    
        method_kind_mapping["table_size"] = "query";    
        method_kind_mapping["insert"] = "mutate";    
        method_kind_mapping["update"] = "mutate";    
        method_kind_mapping["get_value"] = "query";    
        method_kind_mapping["remove_field"] = "mutate";    
        method_kind_mapping["remove_record"] = "mutate";    
        method_kind_mapping["insert_record"] = "mutate";    
        method_kind_mapping["insert_records"] = "mutate";    
        method_kind_mapping["get_fields"] = "query";    
        method_kind_mapping["get_all_fields"] = "query";
        method_kind_mapping["tools"] = "query";
        nlohmann::ordered_json json_object = method_kind_mapping;
        std::string serialized_string = json_object.dump();
        weilsdk::Runtime::setResult(serialized_string, 0);
    }
    
    
    void create_table() {
        std::pair<std::string, std::string> p = weilsdk::Runtime::stateAndArgs();
        std::string raw_args = p.second;
        nlohmann::ordered_json j = nlohmann::ordered_json::parse(raw_args);
        
        if (j.is_discarded() || !j.contains("table_name")) {
            weilsdk::MethodError me = weilsdk::MethodError("create_table", "invalid_args");
            weilsdk::Runtime::setResult(weilsdk::WeilError::MethodArgumentDeserializationError(me), 1);
            return;
        }
        
        create_table_args args;
        args = j.get<create_table_args>();
        
        std::string stateString = p.first;
        nlohmann::ordered_json j1 = nlohmann::ordered_json::parse(stateString);
        
        from_json(j1, in_memory_db_instance);
        
        int32_t result = in_memory_db_instance.create_table(args.table_name);
        nlohmann::ordered_json j2 = in_memory_db_instance;
        weilsdk::WeilValue wv;
        nlohmann::ordered_json j_result = result;
        wv.new_with_state_and_ok_value(j2.dump(), j_result.dump());
        weilsdk::Runtime::setStateAndResult(std::variant<weilsdk::WeilValue,std::string> {wv});
    }


    void drop_table() {
        std::pair<std::string, std::string> p = weilsdk::Runtime::stateAndArgs();
        std::string raw_args = p.second;
        nlohmann::ordered_json j = nlohmann::ordered_json::parse(raw_args);
        
        if (j.is_discarded() || !j.contains("table_name")) {
            weilsdk::MethodError me = weilsdk::MethodError("drop_table", "invalid_args");
            weilsdk::Runtime::setResult(weilsdk::WeilError::MethodArgumentDeserializationError(me), 1);
            return;
        }
        
        drop_table_args args;
        args = j.get<drop_table_args>();
        
        std::string stateString = p.first;
        nlohmann::ordered_json j1 = nlohmann::ordered_json::parse(stateString);
        
        from_json(j1, in_memory_db_instance);
        
        int32_t result = in_memory_db_instance.drop_table(args.table_name);
        nlohmann::ordered_json j2 = in_memory_db_instance;
        weilsdk::WeilValue wv;
        nlohmann::ordered_json j_result = result;
        wv.new_with_state_and_ok_value(j2.dump(), j_result.dump());
        weilsdk::Runtime::setStateAndResult(std::variant<weilsdk::WeilValue,std::string> {wv});
    }


    void list_tables() {    std::string stateString = weilsdk::Runtime::state();
    
        nlohmann::ordered_json j1 = nlohmann::ordered_json::parse(stateString);
        from_json(j1, in_memory_db_instance);
        
        std::vector<std::string> result = in_memory_db_instance.list_tables();
        nlohmann::ordered_json j_result = result;
        weilsdk::Runtime::setResult(j_result.dump(), 0);
    }


    void table_size() {
            std::pair<std::string, std::string> p = weilsdk::Runtime::stateAndArgs();
            std::string raw_args = p.second;
            nlohmann::ordered_json j = nlohmann::ordered_json::parse(raw_args);
        
            if (j.is_discarded() || !j.contains("table_name")) {
            weilsdk::MethodError me = weilsdk::MethodError("table_size", "invalid_args");
            weilsdk::Runtime::setResult(weilsdk::WeilError::MethodArgumentDeserializationError(me), 1);
            return;
        }
        
        table_size_args args;
        args = j.get<table_size_args>();
        
        std::string stateString = p.first;
    
        nlohmann::ordered_json j1 = nlohmann::ordered_json::parse(stateString);
        from_json(j1, in_memory_db_instance);
        
        int32_t result = in_memory_db_instance.table_size(args.table_name);
        nlohmann::ordered_json j_result = result;
        weilsdk::Runtime::setResult(j_result.dump(), 0);
    }


    void insert() {
        std::pair<std::string, std::string> p = weilsdk::Runtime::stateAndArgs();
        std::string raw_args = p.second;
        nlohmann::ordered_json j = nlohmann::ordered_json::parse(raw_args);
        
        if (j.is_discarded() || !j.contains("table") || !j.contains("key") || !j.contains("field") || !j.contains("value")) {
            weilsdk::MethodError me = weilsdk::MethodError("insert", "invalid_args");
            weilsdk::Runtime::setResult(weilsdk::WeilError::MethodArgumentDeserializationError(me), 1);
            return;
        }
        
        insert_args args;
        args = j.get<insert_args>();
        
        std::string stateString = p.first;
        nlohmann::ordered_json j1 = nlohmann::ordered_json::parse(stateString);
        
        from_json(j1, in_memory_db_instance);
        
        int32_t result = in_memory_db_instance.insert(args.table, args.key, args.field, args.value);
        nlohmann::ordered_json j2 = in_memory_db_instance;
        weilsdk::WeilValue wv;
        nlohmann::ordered_json j_result = result;
        wv.new_with_state_and_ok_value(j2.dump(), j_result.dump());
        weilsdk::Runtime::setStateAndResult(std::variant<weilsdk::WeilValue,std::string> {wv});
    }


    void update() {
        std::pair<std::string, std::string> p = weilsdk::Runtime::stateAndArgs();
        std::string raw_args = p.second;
        nlohmann::ordered_json j = nlohmann::ordered_json::parse(raw_args);
        
        if (j.is_discarded() || !j.contains("table") || !j.contains("key") || !j.contains("field") || !j.contains("value")) {
            weilsdk::MethodError me = weilsdk::MethodError("update", "invalid_args");
            weilsdk::Runtime::setResult(weilsdk::WeilError::MethodArgumentDeserializationError(me), 1);
            return;
        }
        
        update_args args;
        args = j.get<update_args>();
        
        std::string stateString = p.first;
        nlohmann::ordered_json j1 = nlohmann::ordered_json::parse(stateString);
        
        from_json(j1, in_memory_db_instance);
        
        int32_t result = in_memory_db_instance.update(args.table, args.key, args.field, args.value);
        nlohmann::ordered_json j2 = in_memory_db_instance;
        weilsdk::WeilValue wv;
        nlohmann::ordered_json j_result = result;
        wv.new_with_state_and_ok_value(j2.dump(), j_result.dump());
        weilsdk::Runtime::setStateAndResult(std::variant<weilsdk::WeilValue,std::string> {wv});
    }


    void get_value() {
            std::pair<std::string, std::string> p = weilsdk::Runtime::stateAndArgs();
            std::string raw_args = p.second;
            nlohmann::ordered_json j = nlohmann::ordered_json::parse(raw_args);
        
            if (j.is_discarded() || !j.contains("table") || !j.contains("key") || !j.contains("field")) {
            weilsdk::MethodError me = weilsdk::MethodError("get_value", "invalid_args");
            weilsdk::Runtime::setResult(weilsdk::WeilError::MethodArgumentDeserializationError(me), 1);
            return;
        }
        
        get_value_args args;
        args = j.get<get_value_args>();
        
        std::string stateString = p.first;
    
        nlohmann::ordered_json j1 = nlohmann::ordered_json::parse(stateString);
        from_json(j1, in_memory_db_instance);
        
        std::optional<std::string> result = in_memory_db_instance.get_value(args.table, args.key, args.field);
        if (result.has_value()) {
            nlohmann::ordered_json j_result = result.value();
            weilsdk::Runtime::setResult(j_result.dump(), 0);
        } else {
            weilsdk::Runtime::setResult("null", 0);
        }
    }


    void remove_field() {
        std::pair<std::string, std::string> p = weilsdk::Runtime::stateAndArgs();
        std::string raw_args = p.second;
        nlohmann::ordered_json j = nlohmann::ordered_json::parse(raw_args);
        
        if (j.is_discarded() || !j.contains("table") || !j.contains("key") || !j.contains("field")) {
            weilsdk::MethodError me = weilsdk::MethodError("remove_field", "invalid_args");
            weilsdk::Runtime::setResult(weilsdk::WeilError::MethodArgumentDeserializationError(me), 1);
            return;
        }
        
        remove_field_args args;
        args = j.get<remove_field_args>();
        
        std::string stateString = p.first;
        nlohmann::ordered_json j1 = nlohmann::ordered_json::parse(stateString);
        
        from_json(j1, in_memory_db_instance);
        
        int32_t result = in_memory_db_instance.remove_field(args.table, args.key, args.field);
        nlohmann::ordered_json j2 = in_memory_db_instance;
        weilsdk::WeilValue wv;
        nlohmann::ordered_json j_result = result;
        wv.new_with_state_and_ok_value(j2.dump(), j_result.dump());
        weilsdk::Runtime::setStateAndResult(std::variant<weilsdk::WeilValue,std::string> {wv});
    }


    void remove_record() {
        std::pair<std::string, std::string> p = weilsdk::Runtime::stateAndArgs();
        std::string raw_args = p.second;
        nlohmann::ordered_json j = nlohmann::ordered_json::parse(raw_args);
        
        if (j.is_discarded() || !j.contains("table") || !j.contains("key")) {
            weilsdk::MethodError me = weilsdk::MethodError("remove_record", "invalid_args");
            weilsdk::Runtime::setResult(weilsdk::WeilError::MethodArgumentDeserializationError(me), 1);
            return;
        }
        
        remove_record_args args;
        args = j.get<remove_record_args>();
        
        std::string stateString = p.first;
        nlohmann::ordered_json j1 = nlohmann::ordered_json::parse(stateString);
        
        from_json(j1, in_memory_db_instance);
        
        int32_t result = in_memory_db_instance.remove_record(args.table, args.key);
        nlohmann::ordered_json j2 = in_memory_db_instance;
        weilsdk::WeilValue wv;
        nlohmann::ordered_json j_result = result;
        wv.new_with_state_and_ok_value(j2.dump(), j_result.dump());
        weilsdk::Runtime::setStateAndResult(std::variant<weilsdk::WeilValue,std::string> {wv});
    }


    void insert_record() {
        std::pair<std::string, std::string> p = weilsdk::Runtime::stateAndArgs();
        std::string raw_args = p.second;
        nlohmann::ordered_json j = nlohmann::ordered_json::parse(raw_args);
        
        if (j.is_discarded() || !j.contains("table") || !j.contains("key") || !j.contains("fields")) {
            weilsdk::MethodError me = weilsdk::MethodError("insert_record", "invalid_args");
            weilsdk::Runtime::setResult(weilsdk::WeilError::MethodArgumentDeserializationError(me), 1);
            return;
        }
        
        insert_record_args args;
        args = j.get<insert_record_args>();
        
        std::string stateString = p.first;
        nlohmann::ordered_json j1 = nlohmann::ordered_json::parse(stateString);
        
        from_json(j1, in_memory_db_instance);
        
        int32_t result = in_memory_db_instance.insert_record(args.table, args.key, args.fields);
        nlohmann::ordered_json j2 = in_memory_db_instance;
        weilsdk::WeilValue wv;
        nlohmann::ordered_json j_result = result;
        wv.new_with_state_and_ok_value(j2.dump(), j_result.dump());
        weilsdk::Runtime::setStateAndResult(std::variant<weilsdk::WeilValue,std::string> {wv});
    }


    void insert_records() {
        std::pair<std::string, std::string> p = weilsdk::Runtime::stateAndArgs();
        std::string raw_args = p.second;
        nlohmann::ordered_json j = nlohmann::ordered_json::parse(raw_args);
        
        if (j.is_discarded() || !j.contains("table") || !j.contains("records")) {
            weilsdk::MethodError me = weilsdk::MethodError("insert_records", "invalid_args");
            weilsdk::Runtime::setResult(weilsdk::WeilError::MethodArgumentDeserializationError(me), 1);
            return;
        }
        
        insert_records_args args;
        args = j.get<insert_records_args>();
        
        std::string stateString = p.first;
        nlohmann::ordered_json j1 = nlohmann::ordered_json::parse(stateString);
        
        from_json(j1, in_memory_db_instance);
        
        int32_t result = in_memory_db_instance.insert_records(args.table, args.records);
        nlohmann::ordered_json j2 = in_memory_db_instance;
        weilsdk::WeilValue wv;
        nlohmann::ordered_json j_result = result;
        wv.new_with_state_and_ok_value(j2.dump(), j_result.dump());
        weilsdk::Runtime::setStateAndResult(std::variant<weilsdk::WeilValue,std::string> {wv});
    }


    void get_fields() {
            std::pair<std::string, std::string> p = weilsdk::Runtime::stateAndArgs();
            std::string raw_args = p.second;
            nlohmann::ordered_json j = nlohmann::ordered_json::parse(raw_args);
        
            if (j.is_discarded() || !j.contains("table") || !j.contains("key") || !j.contains("fields")) {
            weilsdk::MethodError me = weilsdk::MethodError("get_fields", "invalid_args");
            weilsdk::Runtime::setResult(weilsdk::WeilError::MethodArgumentDeserializationError(me), 1);
            return;
        }
        
        get_fields_args args;
        args = j.get<get_fields_args>();
        
        std::string stateString = p.first;
    
        nlohmann::ordered_json j1 = nlohmann::ordered_json::parse(stateString);
        from_json(j1, in_memory_db_instance);
        
        std::vector<std::tuple<std::string, std::string>> result = in_memory_db_instance.get_fields(args.table, args.key, args.fields);
        nlohmann::ordered_json j_result = result;
        weilsdk::Runtime::setResult(j_result.dump(), 0);
    }


    void get_all_fields() {
            std::pair<std::string, std::string> p = weilsdk::Runtime::stateAndArgs();
            std::string raw_args = p.second;
            nlohmann::ordered_json j = nlohmann::ordered_json::parse(raw_args);
        
            if (j.is_discarded() || !j.contains("table") || !j.contains("key")) {
            weilsdk::MethodError me = weilsdk::MethodError("get_all_fields", "invalid_args");
            weilsdk::Runtime::setResult(weilsdk::WeilError::MethodArgumentDeserializationError(me), 1);
            return;
        }
        
        get_all_fields_args args;
        args = j.get<get_all_fields_args>();
        
        std::string stateString = p.first;
    
        nlohmann::ordered_json j1 = nlohmann::ordered_json::parse(stateString);
        from_json(j1, in_memory_db_instance);
        
        std::vector<std::tuple<std::string, std::string>> result = in_memory_db_instance.get_all_fields(args.table, args.key);
        nlohmann::ordered_json j_result = result;
        weilsdk::Runtime::setResult(j_result.dump(), 0);
    }

    void tools() {
    // 1. Recover state
    std::string stateString = weilsdk::Runtime::state();
    nlohmann::ordered_json j = nlohmann::ordered_json::parse(stateString);
    from_json(j, in_memory_db_instance);

    // 2. Get tools JSON string
    std::string tools_json_raw = in_memory_db_instance.tools();

    // 3. Wrap as a JSON string to satisfy the host's type expectation
    nlohmann::ordered_json result_wrapper = tools_json_raw;
    weilsdk::Runtime::setResult(result_wrapper.dump(), 0);
}

} // extern "C"
