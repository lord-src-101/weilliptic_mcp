
#ifndef CREDIT_SCORE_CONTRACT_HPP
#define CREDIT_SCORE_CONTRACT_HPP
    
    
#include <string>
#include <tuple>
#include <optional>
    
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
    
    
class credit_score_ContractState {
    private:
    // Private members go here
    
    public:
    credit_score_ContractState() = default;
    
        // Query
    double get_score(const uint32_t &account_age_months, const double &monthly_income_avg, const std::string &income_frequency, const double &monthly_rent, const double &monthly_utilities, const uint32_t &missed_payments_count)const{
            // --- 1. DATA EXTRACTION ---
    double monthly_income = monthly_income_avg;
    
    // Sum 'Survival' obligations only (Rent + Utilities)
    double essential_expenses = monthly_rent+monthly_utilities;
    

    // --- 2. CASH FLOW COVERAGE (Weight: 50%) ---
    if (monthly_income <= 0.1) return 0.0; // Fail safe

    double free_cash_flow = monthly_income - essential_expenses;
    double coverage_ratio = free_cash_flow / monthly_income; 

    // Math: (Ratio / 0.40) * 50.0
    // If they have 40% free cash, they get full 50 points.
    double score_cash = (std::min(coverage_ratio, 0.40) / 0.40) * 50.0;
    score_cash = std::max(0.0, score_cash);

    // --- 3. STABILITY (Weight: 30%) ---
    // Split: 25 pts for Consistency, 5 pts for Account Age
    
    // Part A: Income Consistency (Max 25)
    double score_consistency = 0.0;
    std::string consistency = income_frequency;

    if (consistency == "High – Consistent") {
        score_consistency = 25.0; 
    } 
    else if (consistency == "Medium – Variable") {
        score_consistency = 15.0; 
    } 
    else {
        score_consistency = 5.0;  
    }

    // Part B: Account Age Bonus (Max 5)
    // Logic: 0.5 points per month. Need 10 months history for full bonus.
    int age_months = account_age_months;
    double age_bonus = std::min((double)age_months * 0.5, 5.0);

    // Total Stability
    double score_stability = score_consistency + age_bonus;

    // --- 4. RELIABILITY (Weight: 20%) ---
    // Logic: Start with 20, lose 10 per miss.
    int missed = missed_payments_count;
    double score_reliability = std::max(0.0, 20.0 - (missed * 10.0));

    // --- 5. FINAL RAW SCORE ---
    double final_raw_score = score_cash + score_stability + score_reliability;

    return floor(final_raw_score);
    }


        std::string tools() const {
        return R"JSON(        [
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
])JSON";
    }

    // JSON serialization/deserialization
    friend void to_json(nlohmann::ordered_json &j, const credit_score_ContractState &obj);
    friend void from_json(const nlohmann::ordered_json &j, credit_score_ContractState &obj);
    
    
};
inline void to_json(nlohmann::ordered_json &j, const credit_score_ContractState &obj) {
    j = nlohmann::ordered_json{
        // TODO: Add serialization for class members
    };
}

inline void from_json(const nlohmann::ordered_json &j, credit_score_ContractState &obj) {
    // TODO: Add deserialization for class members
}
    
    
#endif // CREDIT_SCORE_CONTRACT_HPP
    