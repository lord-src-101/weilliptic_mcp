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
extern "C" void get_score() __attribute__((export_name("get_score")));
extern "C" void tools() __attribute__((export_name("tools")));

// Global contract state instance
credit_score_ContractState credit_score_instance;

// Argument structs for contract methods

struct get_score_args {
    uint32_t account_age_months;
    double monthly_income_avg;
    std::string income_frequency;
    double monthly_rent;
    double monthly_utilities;
    uint32_t missed_payments_count;

    
    friend void to_json(nlohmann::ordered_json &j, const get_score_args &obj) {
        j = nlohmann::ordered_json::object();

            j["account_age_months"] = obj.account_age_months;

            j["monthly_income_avg"] = obj.monthly_income_avg;

            j["income_frequency"] = obj.income_frequency;

            j["monthly_rent"] = obj.monthly_rent;

            j["monthly_utilities"] = obj.monthly_utilities;

            j["missed_payments_count"] = obj.missed_payments_count;
    }
    
    friend void from_json(const nlohmann::ordered_json &j, get_score_args &obj) {
        try {

            if (!j.contains("account_age_months")) {
                throw std::runtime_error("Missing required field 'account_age_months'");
            }
            j.at("account_age_months").get_to(obj.account_age_months);

            if (!j.contains("monthly_income_avg")) {
                throw std::runtime_error("Missing required field 'monthly_income_avg'");
            }
            j.at("monthly_income_avg").get_to(obj.monthly_income_avg);

            if (!j.contains("income_frequency")) {
                throw std::runtime_error("Missing required field 'income_frequency'");
            }
            j.at("income_frequency").get_to(obj.income_frequency);

            if (!j.contains("monthly_rent")) {
                throw std::runtime_error("Missing required field 'monthly_rent'");
            }
            j.at("monthly_rent").get_to(obj.monthly_rent);

            if (!j.contains("monthly_utilities")) {
                throw std::runtime_error("Missing required field 'monthly_utilities'");
            }
            j.at("monthly_utilities").get_to(obj.monthly_utilities);

            if (!j.contains("missed_payments_count")) {
                throw std::runtime_error("Missing required field 'missed_payments_count'");
            }
            j.at("missed_payments_count").get_to(obj.missed_payments_count);
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
        credit_score_ContractState new_instance;
        // TODO: Initialize contract state as needed
    
        nlohmann::ordered_json j = new_instance;
        weilsdk::WeilValue wv;
        wv.new_with_state_and_ok_value(j.dump(), "null");
        weilsdk::Runtime::setStateAndResult(std::variant<weilsdk::WeilValue,std::string> {wv});
    }
    
    
    // Method kind data collection
    void method_kind_data() {
        std::map<std::string, std::string> method_kind_mapping;
        
        
        method_kind_mapping["get_score"] = "query";
        method_kind_mapping["tools"] = "query";
        nlohmann::ordered_json json_object = method_kind_mapping;
        std::string serializ
        ed_string = json_object.dump();
        weilsdk::Runtime::setResult(serialized_string, 0);
    }
    
    
    void get_score() {
            std::pair<std::string, std::string> p = weilsdk::Runtime::stateAndArgs();
            std::string raw_args = p.second;
            nlohmann::ordered_json j = nlohmann::ordered_json::parse(raw_args);
        
            if (j.is_discarded() || !j.contains("account_age_months") || !j.contains("monthly_income_avg") || !j.contains("income_frequency") || !j.contains("monthly_rent") || !j.contains("monthly_utilities") || !j.contains("missed_payments_count")) {
            weilsdk::MethodError me = weilsdk::MethodError("get_score", "invalid_args");
            weilsdk::Runtime::setResult(weilsdk::WeilError::MethodArgumentDeserializationError(me), 1);
            return;
        }
        
        get_score_args args;
        args = j.get<get_score_args>();
        
        std::string stateString = p.first;
    
        nlohmann::ordered_json j1 = nlohmann::ordered_json::parse(stateString);
        from_json(j1, credit_score_instance);
        
        double result = credit_score_instance.get_score(args.account_age_months, args.monthly_income_avg, args.income_frequency, args.monthly_rent, args.monthly_utilities, args.missed_payments_count);
        nlohmann::ordered_json j_result = result;
        weilsdk::Runtime::setResult(j_result.dump(), 0);
    }

    void tools() {
    // 1. Recover state
    std::string stateString = weilsdk::Runtime::state();
    nlohmann::ordered_json j = nlohmann::ordered_json::parse(stateString);
    from_json(j, credit_score_instance);

    // 2. Get tools JSON string
    std::string tools_json_raw = credit_score_instance.tools();

    // 3. Wrap as a JSON string to satisfy the host's type expectation
    nlohmann::ordered_json result_wrapper = tools_json_raw;
    weilsdk::Runtime::setResult(result_wrapper.dump(), 0);
}

} // extern "C"
