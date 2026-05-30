#include <string>
#include "json.hpp"
#include <map>
#include <vector>
#include <filesystem>

class Nani
{
public:
    Nani(std::string path);
    void ConvertToNani();

private:
    struct RequiredField
    {
        std::string name;
        std::vector<RequiredField> children;
        bool isArray = false;
    };

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

    bool LoadJson();
    void LoadTextMap();
    void Validate(const json &data, const std::vector<RequiredField> &fields,
                  const std::string &path);
};