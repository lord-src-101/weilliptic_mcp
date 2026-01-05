
use serde::{Deserialize, Serialize};
use weil_macros::{constructor, mutate, query, secured, smart_contract, WeilType};
use weil_rs::collections::{streaming::ByteStream, plottable::Plottable};
use weil_rs::config::Secrets;
use weil_rs::webserver::WebServer;


trait in_memory_db {
    fn new() -> Result<Self, String>
    where
        Self: Sized;
    async fn create_table(&mut self, table_name: String) -> i32;
    async fn drop_table(&mut self, table_name: String) -> i32;
    async fn list_tables(&self) -> Vec<String>;
    async fn table_size(&self, table_name: String) -> i32;
    async fn insert(&mut self, table: String, key: String, field: String, value: String) -> i32;
    async fn update(&mut self, table: String, key: String, field: String, value: String) -> i32;
    async fn get_value(&self, table: String, key: String, field: String) -> Option<String>;
    async fn remove_field(&mut self, table: String, key: String, field: String) -> i32;
    async fn remove_record(&mut self, table: String, key: String) -> i32;
    async fn get_fields(&self, table: String, key: String, fields: Vec<String>) -> Vec<(String, String)>;
    async fn get_all_fields(&self, table: String, key: String) -> Vec<(String, String)>;
    fn tools(&self) -> String;
    fn prompts(&self) -> String;
}

#[derive(Serialize, Deserialize, WeilType)]
pub struct in_memory_dbContractState {
    // define your contract state here!
}

#[smart_contract]
impl in_memory_db for in_memory_dbContractState {
    #[constructor]
    fn new() -> Result<Self, String>
    where
        Self: Sized,
    {
        unimplemented!();
    }


    #[mutate]
    async fn create_table(&mut self, table_name: String) -> i32 {
        unimplemented!();
    }

    #[mutate]
    async fn drop_table(&mut self, table_name: String) -> i32 {
        unimplemented!();
    }

    #[query]
    async fn list_tables(&self) -> Vec<String> {
        unimplemented!();
    }

    #[query]
    async fn table_size(&self, table_name: String) -> i32 {
        unimplemented!();
    }

    #[mutate]
    async fn insert(&mut self, table: String, key: String, field: String, value: String) -> i32 {
        unimplemented!();
    }

    #[mutate]
    async fn update(&mut self, table: String, key: String, field: String, value: String) -> i32 {
        unimplemented!();
    }

    #[query]
    async fn get_value(&self, table: String, key: String, field: String) -> Option<String> {
        unimplemented!();
    }

    #[mutate]
    async fn remove_field(&mut self, table: String, key: String, field: String) -> i32 {
        unimplemented!();
    }

    #[mutate]
    async fn remove_record(&mut self, table: String, key: String) -> i32 {
        unimplemented!();
    }

    #[query]
    async fn get_fields(&self, table: String, key: String, fields: Vec<String>) -> Vec<(String, String)> {
        unimplemented!();
    }

    #[query]
    async fn get_all_fields(&self, table: String, key: String) -> Vec<(String, String)> {
        unimplemented!();
    }


    #[query]
    fn tools(&self) -> String {
        r#"[
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
]"#.to_string()
    }


    #[query]
    fn prompts(&self) -> String {
        r#"{
  "prompts": []
}"#.to_string()
    }
}

