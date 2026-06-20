#include "Nani.hpp"
#include "NaniCmd.hpp"
#include "NaniCmdDeserializer.hpp"
#include <fstream>
#include <exception>
#include <iostream>
#include <format>

Nani::Nani(std::string path) : m_path(path)
{
    if (!LoadJson())
        throw std::runtime_error("Failed to load JSON");
}

bool Nani::LoadJson()
{
    std::fstream f(m_path);

    if (!f.is_open())
        return false;

    m_data = json::parse(f);

    JSONValidator validator(m_data, m_schema);
    validator.Validate();

    return true;
}

void Nani::LoadTextMap()
{
    json keys = m_data["textMap"]["idToText"]["keys"]["Array"];
    json values = m_data["textMap"]["idToText"]["values"]["Array"];

    if (keys.size() != values.size())
        throw std::runtime_error("Failed to parse textMap");

    for (int i = 0; i < keys.size(); i++)
        m_textMap.insert({keys[i].get<std::string>(), values[i].get<std::string>()});
}

void Nani::ConvertToNani()
{
    std::map<std::string, Command> cmds;
    NaniCmdDeserializer deserializer("nanicmds.json", cmds);
    deserializer.Deserialize();

    json lines = m_data["lines"];
    json references = m_data["references"];
    json refIds = references["RefIds"];

    size_t line_count = lines["Array"].size();
    size_t ref_count = refIds.size();

    LoadTextMap();

    if (ref_count < line_count)
        throw std::runtime_error("RefIds less than line count");

    std::filesystem::path outpath = m_path.parent_path() / std::format("{}.nani", m_data["m_Name"].get<std::string>());

    std::ofstream outfile(outpath);

    for (int i = 0; i < line_count; i++)
        outfile << NaniCmd::Dispatch(refIds[i]["type"]["class"].get<std::string>(), ParseContext{refIds, m_textMap, i, cmds}) << std::endl;

    std::cout << "All done :3" << std::endl;
}
