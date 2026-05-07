#pragma once
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class IReport
{
public:
    virtual ~IReport() = default;
    virtual std::string get_cube_state_JSON() const = 0;
};