#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

struct require_struct {
    std::string field1;
    std::string field2;

    static nlohmann::json default_json() {
        return {
            {"field1", ""},
            {"field2", ""}
        };
    }
};

template<typename T>
T parse_with_default(const nlohmann::json& input) {
    std::cout << "Inside parse_with_default:\nOriginal Input:\n" << input.dump(4) << std::endl;

    nlohmann::json combined = T::default_json();
    std::cout << "Default JSON:\n" << combined.dump(4) << std::endl;

    for (auto it = input.begin(); it != input.end(); ++it) {
        combined[it.key()] = it.value();
    }

    std::cout << "Combined JSON:\n" << combined.dump(4) << std::endl;

    // Directly creating the object from the combined JSON
    T result;
    from_json(combined, result);
    return result;
}

namespace nlohmann {
    template <>
    struct adl_serializer<require_struct> {
        static void from_json(const json &j, require_struct &t) {
            // 这里可以直接调用 parse_with_default 方法，如之前的例子所示
            t = parse_with_default<require_struct>(j);
        }

        static void to_json(json &j, const require_struct &t) {
            j = json{{"field1", t.field1}, {"field2", t.field2}};
        }
    };
}


void from_json(const nlohmann::json& j, require_struct& r) {
    j.at("field1").get_to(r.field1);
    j.at("field2").get_to(r.field2);
}


int main() {
    nlohmann::json j = {{"field1", "data1"}};
    std::cout << "Received JSON:\n" << j.dump(4) << std::endl;

    require_struct req = j.get<require_struct>();

    std::cout << "Parsed Data:\nfield1: " << req.field1 << "\nfield2: " << req.field2 << std::endl;
}
