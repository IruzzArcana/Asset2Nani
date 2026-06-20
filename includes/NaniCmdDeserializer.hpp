#ifndef NANICMDDESERIALIZER_HPP
#define NANICMDDESERIALIZER_HPP

#include "NaniCommon.hpp"
#include "JSONValidator.hpp"
#include "json.hpp"
#include <map>
#include <vector>

class NaniCmdDeserializer
{
public:
    NaniCmdDeserializer(std::string path, std::map<std::string, Command> &output);
    void Deserialize();

private:
    const std::map<std::string, ArgType> m_argMap = {
        {"Int", ArgType::Int},
        {"Bool", ArgType::Bool},
        {"String", ArgType::String},
        {"StringList", ArgType::StringList},
        {"Float", ArgType::Float},
        {"FloatList", ArgType::FloatList},
        {"Kvp", ArgType::Kvp},
        {"Text", ArgType::Text},
    };

    std::vector<RequiredField> m_cmdSchema = {
        {"name"},
        {"args", {{"type"}, {"entryname"}}, true}};

    std::string m_path;
    std::map<std::string, Command> &m_cmds;
    nlohmann::json m_data;
    bool LoadJson();
};
#endif