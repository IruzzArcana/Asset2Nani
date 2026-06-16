#include <functional>
#include <string>
#include "json.hpp"
#include <fstream>

struct ParseContext
{
    const nlohmann::json& refId;
    const std::map<std::string, std::string>& textMap;
    const int& idx;
    bool is_inline = false;
};

using LineHandler = std::function<std::string(const ParseContext&)>;

class NaniCmd
{
public:
    static std::string Dispatch(const std::string& type, const ParseContext& ctx);

private:
    static std::string ParseLabelScript(const ParseContext& ctx);
    static std::string ParseCommandScriptLine(const ParseContext& ctx);
    static std::string ParseGenericTextScript(const ParseContext& ctx);
    static std::string ParseEmptyScriptLine(const ParseContext& ctx);
    static std::string ParseCommentScriptLine(const ParseContext& ctx);
    
    static std::string ParseSetControlPanelState(const ParseContext &ctx);
    static std::string ParseModifyTextPrinter(const ParseContext &ctx);
    static std::string ParseHideAllActors(const ParseContext &ctx);
    static std::string ParseWaitForInput(const ParseContext &ctx);
    static std::string ParseModifyCamera(const ParseContext &ctx);
    static std::string ParsePrintText(const ParseContext &ctx);
    static std::string ParsePrintText2(const ParseContext &ctx);
    static std::string ParseResetText(const ParseContext &ctx);
    static std::string ParsePlayBgm(const ParseContext &ctx);
    static std::string ParseStopBgm(const ParseContext &ctx);
    static std::string ParseGoto(const ParseContext &ctx);
    static std::string ParseWait(const ParseContext &ctx);

    static const std::map<std::string, LineHandler> s_handler;
};