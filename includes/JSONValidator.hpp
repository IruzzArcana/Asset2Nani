#ifndef JSONVALIDATOR_HPP
#define JSONVALIDATOR_HPP

#include "json.hpp"
#include <format>

struct RequiredField
{
    std::string name;
    std::vector<RequiredField> children;
    bool isArray = false;
};

class JSONValidator
{
public:
    JSONValidator(const nlohmann::json &data, const std::vector<RequiredField> &fields, const std::string path = "") : m_data(data), m_fields(fields), m_path(path) {};
    void Validate();

private:
    const nlohmann::json &m_data;
    const std::vector<RequiredField> &m_fields;
    const std::string m_path;
};
#endif