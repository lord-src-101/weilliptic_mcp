
use serde::{Deserialize, Serialize};
use weil_macros::{constructor, mutate, query, secured, smart_contract, WeilType};
use weil_rs::collections::{streaming::ByteStream, plottable::Plottable};
use weil_rs::config::Secrets;
use weil_rs::webserver::WebServer;


trait credit_score {
    fn new() -> Result<Self, String>
    where
        Self: Sized;
    async fn get_score(&self, account_age_months: u32, monthly_income_avg: f64, income_frequency: String, monthly_rent: f64, monthly_utilities: f64, missed_payments_count: u32) -> f64;
    fn tools(&self) -> String;
    fn prompts(&self) -> String;
}

#[derive(Serialize, Deserialize, WeilType)]
pub struct credit_scoreContractState {
    // define your contract state here!
}

#[smart_contract]
impl credit_score for credit_scoreContractState {
    #[constructor]
    fn new() -> Result<Self, String>
    where
        Self: Sized,
    {
        unimplemented!();
    }


    #[query]
    async fn get_score(&self, account_age_months: u32, monthly_income_avg: f64, income_frequency: String, monthly_rent: f64, monthly_utilities: f64, missed_payments_count: u32) -> f64 {
        unimplemented!();
    }


    #[query]
    fn tools(&self) -> String {
        r#"[
  {
    "type": "function",
    "function": {
      "name": "get_score",
      "description": "compute a credit score based on financial and payment behavior (returns score as f64, higher is better)\n",
      "parameters": {
        "type": "object",
        "properties": {
          "account_age_months": {
            "type": "integer",
            "description": "age of the account in months\n"
          },
          "monthly_income_avg": {
            "type": "number",
            "description": "average monthly income\n"
          },
          "income_frequency": {
            "type": "string",
            "description": "income frequency (only acceptable parameter is - \"High – Consistent\", \"Medium – Variable\", \"Low-Unpredictable\")\n"
          },
          "monthly_rent": {
            "type": "number",
            "description": "average monthly rent expense\n"
          },
          "monthly_utilities": {
            "type": "number",
            "description": "average monthly utilities expense\n"
          },
          "missed_payments_count": {
            "type": "integer",
            "description": "total number of missed payments\n"
          }
        },
        "required": [
          "account_age_months",
          "monthly_income_avg",
          "income_frequency",
          "monthly_rent",
          "monthly_utilities",
          "missed_payments_count"
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

