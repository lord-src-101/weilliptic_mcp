
#ifndef CREDIT_SCORE_CONTRACT_HPP
#define CREDIT_SCORE_CONTRACT_HPP
    
#include <cmath>
#include <algorithm>
#include <cstdint>
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
    
    // Logic: Quadratic Falloff. 
    // Small expenses (10-20%) are equally "good". High expenses (60-70%) drop score rapidly.
    double essential_expenses = monthly_rent + monthly_utilities;
    double score_affordability = 0.0;
    
    // Protect against division by zero
    if (monthly_income_avg > 1.0) {
        double expense_ratio = essential_expenses / monthly_income_avg;
        double threshold = 0.70; // If expenses > 70% of income, score is 0.
        
        if (expense_ratio < threshold) {
            // Formula: Max * (1 - (Ratio / Threshold)^2)
            score_affordability = 45.0 * (1.0 - std::pow(expense_ratio / threshold, 2.0));
        }
    }

    // --- 2. RELIABILITY (Weight: 35%) ---
    // Logic: Exponential Decay.
    // 0 misses = 35 pts. 1 miss = 21 pts. 2 misses = 12.6 pts.
    // This punishes early mistakes hard but never hits "hard zero".
    double score_reliability = 35.0 * std::pow(0.60, missed_payments_count);


    // --- 3. STABILITY (Weight: 20%) ---
    
    // Part A: Income Consistency (Max 10)
    double score_consistency = 2.0; // Default/Low
    if (income_frequency == "High – Consistent") {
        score_consistency = 10.0;
    } else if (income_frequency == "Medium – Variable") {
        score_consistency = 6.0;
    }

    // Part B: History (Max 10)
    // Logic: Soft Saturation (Diminishing Returns).
    // Uses k=12. Users get 50% of points (5 pts) at 12 months.
    // They get 80% of points (8 pts) at 48 months.
    double k_factor = 12.0;
    double score_history = 10.0 * ((double)account_age_months / (account_age_months + k_factor));

    double score_stability = score_consistency + score_history;

    // --- FINAL SCORE ---
    // Clamping to 100.0 to ensure floating point math doesn't exceed bounds slightly.
    return std::min(100.0, score_affordability + score_reliability + score_stability);
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
    