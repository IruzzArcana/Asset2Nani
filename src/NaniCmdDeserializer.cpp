#include "NaniCmdDeserializer.hpp"
#include <fstream>
#include <exception>
#include <iostream>

NaniCmdDeserializer::NaniCmdDeserializer(std::filesystem::path path, std::map<std::string, Command> &output) : m_path(path), m_cmds(output) 
{
    if (!LoadJson())
        throw std::runtime_error("CmdMap not found");
}

void NaniCmdDeserializer::Deserialize()
{
    for (auto& [key, value] : m_data.items())
    {
        std::string name = value["name"];
        std::vector<CommandArg> args;
        for (auto& arg : value["args"])
        {
            auto it = m_argMap.find(arg["type"]);
            if (it == m_argMap.end())
                throw std::runtime_error(std::format("Unknown type {}", arg["type"].get<std::string>()));

            std::string cmdname = arg.contains("cmdname") ? arg["cmdname"] : "";
            args.push_back({it->second, arg["entryname"], cmdname});
        }
        m_cmds.insert({key, {name, args}});
    }
}

bool NaniCmdDeserializer::LoadJson()
{
    std::fstream f(m_path);

    if (!f.is_open())
        return false;

    m_data = nlohmann::json::parse(f);

    for (auto& [key, value] : m_data.items()) 
    {
        JSONValidator validator(value, m_cmdSchema, key);
        validator.Validate();
    }

    return true;
}