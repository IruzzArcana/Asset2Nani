#include <functional>
#include <string>
#include "json.hpp"
#include <fstream>

struct ParseContext
{
    const nlohmann::json &refId;
    const std::map<std::string, std::string> &textMap;
    const int &idx;
    bool is_inline = false;
};

enum ArgType
{
    Int,
    Bool,
    String,
    StringList,
    Float,
    FloatList,
    Kvp,
    Text
};

struct CommandArg
{
    ArgType type;
    std::string entryname;
    std::string cmdname = "";
};

struct Command
{
    std::string name;
    std::vector<CommandArg> args;
};

using LineHandler = std::function<std::string(const ParseContext &)>;

class NaniCmd
{
public:
    static std::string Dispatch(const std::string &type, const ParseContext &ctx);

private:
    static std::string ParseLabelScript(const ParseContext &ctx);
    static std::string ParseCommandScriptLine(const ParseContext &ctx);
    static std::string ParseGenericTextScript(const ParseContext &ctx);
    static std::string ParseEmptyScriptLine(const ParseContext &ctx);
    static std::string ParseCommentScriptLine(const ParseContext &ctx);
    static std::string ParsePrintText(const ParseContext &ctx);
    static std::string ParseCmd(const ParseContext &ctx);

    static const std::map<std::string, LineHandler> s_handler;
    static const std::map<std::string, Command> m_cmds;
};