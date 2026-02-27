#pragma once
#include<nlohmann/json.hpp>
using json = nlohmann::json;

class IReport{
public:
    virtual std::string get_cube_state_JSON() const = 0;
};