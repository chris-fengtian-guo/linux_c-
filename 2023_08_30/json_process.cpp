#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include "nlohmann/json.hpp"

struct require_struct {
    std::string field1;
    std::string field2;
    std::string field3;
    std::string field4;
    std::string field5;
    std::string field6;
    std::string field7;
    std::string field8;
    std::string field9;
    std::string field10;
};

// 为nlohmann::json定义序列化和反序列化函数
namespace nlohmann {
    template<>
    struct adl_serializer<require_struct> {
        static void to_json(json& j, const require_struct& req) {
            j = json{
                {"field1", req.field1},
                {"field2", req.field2},
                {"field3", req.field3},
                {"field4", req.field4},
                {"field5", req.field5},
                {"field6", req.field6},
                {"field7", req.field7},
                {"field8", req.field8},
                {"field9", req.field9},
                {"field10", req.field10},
            };
        }

        static void from_json(const json& j, require_struct& req) {
            j.at("field1").get_to(req.field1);
            j.at("field2").get_to(req.field2);
            j.at("field3").get_to(req.field3);
            j.at("field4").get_to(req.field4);
            j.at("field5").get_to(req.field5);
            j.at("field6").get_to(req.field6);
            j.at("field7").get_to(req.field7);
            j.at("field8").get_to(req.field8);
            j.at("field9").get_to(req.field9);
            j.at("field10").get_to(req.field10);
        }
    };
}

nlohmann::json default_require_json = {
    {"field1", ""},
    {"field2", ""},
    {"field3", ""},
    {"field4", ""},
    {"field5", ""},
    {"field6", ""},
    {"field7", ""},
    {"field8", ""},
    {"field9", ""},
    {"field10", ""}
};

template<typename T>
T parse_with_default(const nlohmann::json& input, const nlohmann::json& default_json) {
    nlohmann::json combined = input;

    std::cout << "Initial input JSON:\n" << combined.dump(4) << std::endl;
    
    for (auto it = default_json.begin(); it != default_json.end(); ++it) {
        if (!combined.contains(it.key())) {
            std::cout << "Key missing: " << it.key() << ", using default value: " << it.value().dump() << std::endl;
            combined[it.key()] = it.value();
        }

        // 检查是否有非法值并进行处理
        if (combined[it.key()].is_null() || combined[it.key()] == "nullptr") {
            std::cout << "Detected null or nullptr value for key: " << it.key() << ". Replacing with empty string." << std::endl;
            combined[it.key()] = "";
        }
    }

    std::cout << "Final combined JSON:\n" << combined.dump(4) << std::endl;

    return combined.get<T>();
}
nlohmann::json received_json = {
    {"field1", "data1"},
    {"field3", "data3"},
    {"field5", nullptr},
    {"field7", "data7"},
    {"field8", "nullptr"}
};

int main() {
	require_struct msg = parse_with_default<require_struct>(received_json, default_require_json);
}
