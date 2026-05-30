#include "Nani.hpp"
#include "NaniCmd.hpp"
#include <fstream>
#include <exception>
#include <iostream>
#include <format>

Nani::Nani(std::string path) : m_path(path)
{
    if (!LoadJson())
        throw std::runtime_error("Faile to load JSON");
}

bool Nani::LoadJson()
{
    std::fstream f(m_path);

    if (!f.is_open())
    {
        return false;
    }

    m_data = json::parse(f);
    return true;
}

void Nani::Validate(const json &data, const std::vector<RequiredField> &fields,
                  const std::string &path = "")
{
    for (const auto& field : fields)
    {
        std::string currentPath =
            path.empty() ? field.name : path + "/" + field.name;

        if (!data.contains(field.name))
            throw std::runtime_error(
                std::format("Missing {}", currentPath));

        const json& value = data[field.name];

        if (field.isArray)
        {
            if (!value.is_array())
                throw std::runtime_error(
                    std::format("{} must be an array", currentPath));

            for (size_t i = 0; i < value.size(); ++i)
            {
                Validate(
                    value[i],
                    field.children,
                    std::format("{}/[{}]", currentPath, i));
            }
        }
        else if (!field.children.empty())
        {
            Validate(value, field.children, currentPath);
        }
    }
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
    Validate(m_data, m_schema);

    json lines = m_data["lines"];
    json references = m_data["references"];
    json refIds = references["RefIds"];

    int line_count = lines["Array"].size();
    int ref_count = refIds.size();

    LoadTextMap();

    if (ref_count < line_count)
        throw std::runtime_error("RefIds less than line count");

    std::filesystem::path outpath = m_path.parent_path() / std::format("{}.nani", m_data["m_Name"].get<std::string>());

    std::ofstream outfile(outpath);

    for (int i = 0; i < line_count; i++)
    {
        std::cout << refIds[i]["type"]["class"] << std::endl;
        outfile << NaniCmd::Dispatch(refIds[i]["type"]["class"].get<std::string>(), ParseContext {refIds, m_textMap, i}) << std::endl;
    }

}
