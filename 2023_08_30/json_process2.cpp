
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include "nlohmann/json.hpp"

#define DEFINE_DEFAULT_JSON(Type, DefaultJson) \
    namespace nlohmann { \
        template <> \
        struct adl_serializer<Type> { \
            static Type from_json(const json &j) { \
                return parse_with_default<Type>(j); \
            } \
            static void to_json(json &j, const Type &t) { \
                j = json(t); \
            } \
        }; \
    }

template<typename T>
T parse_with_default(const nlohmann::json& input) {
    nlohmann::json combined = T::default_json();
    for (auto it = input.begin(); it != input.end(); ++it) {
        combined[it.key()] = it.value();
    }
    return combined.get<T>();
}

struct require_struct {
    std::string field1;
    std::string field2;
    // ... more fields ...

    static nlohmann::json default_json() {
        return {
            {"field1", ""},
            {"field2", ""}
            // ... more defaults ...
        };
    }
};

DEFINE_DEFAULT_JSON(require_struct, require_struct::default_json())

nlohmann::json received_json = {
    {"field1", "data1"},
};

int main() {
	nlohmann::json j = received_json;/* your json data */;
	require_struct req = j.get<require_struct>();
}

