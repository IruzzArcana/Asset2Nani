#include <string>
#include "json.hpp"
#include <map>
#include <vector>
#include <filesystem>
#include "JSONValidator.hpp"
#include "NaniCommon.hpp"

class Nani
{
public:
    Nani(std::string path);
    void ConvertToNani();

private:

    const std::vector<RequiredField> m_schema =
        {
            {"m_Name"},
            {"lines",
             {
                 {"Array",
                  {{"rid"}},
                  true},
             }},
            {"textMap",
             {{"idToText",
               {{"keys",
                 {{"Array"}}},
                {"values",
                 {{"Array"}}}}}}},
            {"references",
             {{"RefIds",
               {{"rid"}, {"data"}},
               true}}}};

    using json = nlohmann::json;
    json m_data;
    std::filesystem::path m_path;
    std::map<std::string, std::string> m_textMap;
    std::map<std::string, Command> m_cmds;

    bool LoadJson();
    void LoadTextMap();
};