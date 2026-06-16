#include "NaniCmd.hpp"
#include <format>
#include <iostream>

const std::map<std::string, LineHandler> NaniCmd::s_handler = {
    {"LabelScriptLine", NaniCmd::ParseLabelScript},
    {"CommandScriptLine", NaniCmd::ParseCommandScriptLine},
    {"GenericTextScriptLine", NaniCmd::ParseGenericTextScript},
    {"EmptyScriptLine", NaniCmd::ParseEmptyScriptLine},
    {"CommentScriptLine", NaniCmd::ParseCommentScriptLine},

    {"SetControlPanelState", NaniCmd::ParseSetControlPanelState},
    {"ModifyTextPrinter", NaniCmd::ParseModifyTextPrinter},
    {"HideAllActors", NaniCmd::ParseHideAllActors},
    {"WaitForInput", NaniCmd::ParseWaitForInput},
    {"ModifyCamera", NaniCmd::ParseModifyCamera},
    {"PrintText", NaniCmd::ParsePrintText},
    {"ResetText", NaniCmd::ParseResetText},
    {"PlayBgm", NaniCmd::ParsePlayBgm},
    {"StopBgm", NaniCmd::ParseStopBgm},
    {"Goto", NaniCmd::ParseGoto},
    {"Wait", NaniCmd::ParseWait}};

std::string NaniCmd::Dispatch(const std::string &type, const ParseContext &ctx)
{
    auto it = s_handler.find(type);

    if (it == s_handler.end())
        // throw std::runtime_error(std::format("Unknown line type: {}", type));
        return "@ " + type + " ; TODO";

    return it->second(ctx);
}

std::string NaniCmd::ParseLabelScript(const ParseContext &ctx)
{
    return std::format("# {}", ctx.refId[ctx.idx]["data"]["labelText"].get<std::string>());
}

std::string NaniCmd::ParseCommentScriptLine(const ParseContext &ctx)
{
    return std::format("; {}", ctx.refId[ctx.idx]["data"]["commentText"].get<std::string>());
}

std::string NaniCmd::ParseCommandScriptLine(const ParseContext &ctx)
{
    int rid = ctx.refId[ctx.idx]["data"]["command"]["rid"];

    for (auto &ref : ctx.refId)
    {
        if (ref["rid"] == rid)
            return Dispatch(ref["type"]["class"].get<std::string>(), ParseContext{ref, ctx.textMap, ctx.idx});
    }
    throw std::runtime_error("rid not found");
}

std::string NaniCmd::ParseGenericTextScript(const ParseContext &ctx)
{
    std::string result;
    const auto &inlined = ctx.refId[ctx.idx]["data"]["inlinedCommands"]["Array"];

    bool first = true;
    for (const auto &cmdRef : inlined)
    {
        int rid = cmdRef["rid"];
        for (const auto &ref : ctx.refId)
        {
            if (ref["rid"] == rid)
            {
                if (ref["type"]["class"].get<std::string>() == "PrintText")
                    result += ParsePrintText2({ref, ctx.textMap, ctx.idx});
                else
                    result += std::format("[{}]", Dispatch(ref["type"]["class"].get<std::string>(), ParseContext{ref, ctx.textMap, ctx.idx}));
                break;
            }
        }
        result += " ";
    }

    return result;
}

std::string NaniCmd::ParseEmptyScriptLine(const ParseContext &ctx)
{
    return "";
}

std::string NaniCmd::ParseSetControlPanelState(const ParseContext &ctx)
{
    return std::format("@SetControlPanelState history:{} setting:{}", ctx.refId["data"]["history"]["value"].get<int>(), ctx.refId["data"]["setting"]["value"].get<int>());
}

std::string NaniCmd::ParseModifyTextPrinter(const ParseContext &ctx)
{
    std::string cmd = "@printer";
    auto &data = ctx.refId["data"];

    for (auto &[key, entry] : data.items())
    {
        if (!entry.contains("hasValue") || entry["hasValue"].get<int>() == 0)
            continue;

        cmd += " ";

        if (key == "IdAndAppearance")
        {
            std::string name = entry["value"]["name"]["value"].get<std::string>();
            std::string val = entry["value"]["value"]["value"].get<std::string>();
            cmd += val.empty() ? name : name + "." + val;
        }
        else if (key == "MakeDefault")
        {
            cmd += "default:";
            cmd += entry["value"].get<int>() ? "true" : "false";
        }
        else if (key == "HideOther")
        {
            cmd += "hideOther:";
            cmd += entry["value"].get<int>() ? "true" : "false";
        }
    }
    return cmd;
}

std::string NaniCmd::ParsePlayBgm(const ParseContext &ctx)
{
    std::string cmd = "@bgm";
    auto &data = ctx.refId["data"];

    for (auto &[key, entry] : data.items())
    {
        if (!entry.contains("hasValue") || entry["hasValue"].get<int>() == 0)
            continue;

        cmd += " ";

        if (key == "Wait")
        {
            cmd += "wait:";
            cmd += entry["value"].get<int>() ? "true" : "false";
        }
        else if (key == "BgmPath")
        {
            cmd += entry["value"].get<std::string>();
        }
        else if (key == "Volume")
        {
            cmd += "volume:";
            cmd += std::format("{:g}", entry["value"].get<float>());
        }
        else if (key == "Loop")
        {
            cmd += "loop:";
            cmd += entry["value"].get<int>() ? "true" : "false";
        }
        else if (key == "FadeInDuration")
        {
            cmd += "fade:";
            cmd += std::format("{:g}", entry["value"].get<float>());
        }
    }
    return cmd;
}

std::string NaniCmd::ParsePrintText(const ParseContext &ctx)
{
    std::string cmd = "@print";
    auto &data = ctx.refId["data"];

    for (auto &[key, entry] : data.items())
    {
        if (!entry.contains("hasValue") || entry["hasValue"].get<int>() == 0)
            continue;

        cmd += " ";

        if (key == "Wait")
        {
            cmd += "wait:";
            cmd += entry["value"].get<int>() ? "true" : "false";
        }
        else if (key == "Text")
        {
            std::string tkey = entry["value"]["parts"]["Array"][0]["id"].get<std::string>();
            cmd += std::format("{}|#{}|", ctx.textMap.at(tkey), tkey);
        }
        else if (key == "AuthorId")
        {
            cmd += std::format("author:{}", entry["value"].get<std::string>());
        }
        else if (key == "RevealSpeed")
        {
            cmd += "speed:";
            cmd += std::format("{:g}", entry["value"].get<float>());
        }
        else if (key == "WaitForInput")
        {
            cmd += "waitInput:";
            cmd += entry["value"].get<int>() ? "true" : "false";
        }
    }
    return cmd;
}

std::string NaniCmd::ParsePrintText2(const ParseContext &ctx)
{
    std::string cmd = "";
    auto &data = ctx.refId["data"];

    if (data.contains("AuthorId") && data["AuthorId"].contains("hasValue") && data["AuthorId"]["hasValue"].get<int>() == 1)
        cmd += std::format("{}: ", data["AuthorId"]["value"].get<std::string>());

    std::string tkey = data["Text"]["value"]["parts"]["Array"][0]["id"].get<std::string>();
    cmd += std::format("{}|#{}|", ctx.textMap.at(tkey), tkey);

    return cmd;
}

std::string NaniCmd::ParseResetText(const ParseContext &ctx)
{
    return "@resetText";
}

std::string NaniCmd::ParseStopBgm(const ParseContext &ctx)
{
    std::string cmd = "@stopBgm";
    auto &data = ctx.refId["data"];

    for (auto &[key, entry] : data.items())
    {
        if (!entry.contains("hasValue") || entry["hasValue"].get<int>() == 0)
            continue;

        cmd += " ";

        if (key == "Wait")
        {
            cmd += "wait:";
            cmd += entry["value"].get<int>() ? "true" : "false";
        }
        else if (key == "BgmPath")
        {
            cmd += entry["value"].get<std::string>();
        }
        else if (key == "FadeOutDuration")
        {
            cmd += "fade:";
            cmd += std::format("{:g}", entry["value"].get<float>());
        }
    }
    return cmd;
}

std::string NaniCmd::ParseGoto(const ParseContext &ctx)
{
    std::string cmd = "@goto";
    auto &data = ctx.refId["data"];

    for (auto &[key, entry] : data.items())
    {
        if (!entry.contains("hasValue") || entry["hasValue"].get<int>() == 0)
            continue;

        cmd += " ";

        if (key == "Path")
        {
            std::string name = entry["value"]["name"]["value"].get<std::string>();
            std::string val = entry["value"]["value"]["value"].get<std::string>();
            cmd += val.empty() ? name : name + "." + val;
        }
    }
    return cmd;
}

std::string NaniCmd::ParseModifyCamera(const ParseContext &ctx)
{
    std::string cmd = "@camera";
    auto &data = ctx.refId["data"];

    for (auto &[key, entry] : data.items())
    {
        if (!entry.contains("hasValue") || entry["hasValue"].get<int>() == 0)
            continue;

        cmd += " ";

        if (key == "Offset")
        {
            cmd += "offset:";
            auto &coords = entry["value"]["Array"];
            for (size_t i = 0; i < coords.size(); ++i)
            {
                if (i > 0)
                    cmd += ",";
                cmd += std::format("{:g}", coords[i]["value"].get<float>());
            }
        }
        else if (key == "Rotation")
        {
            cmd += "rotation:";
            auto &coords = entry["value"]["Array"];
            for (size_t i = 0; i < coords.size(); ++i)
            {
                if (i > 0)
                    cmd += ",";
                cmd += std::format("{:g}", coords[i]["value"].get<float>());
            }
        }
        else if (key == "Zoom")
            cmd += std::format("zoom:{:g}", entry["value"].get<float>());
        else if (key == "Duration")
            cmd += std::format("duration:{:g}", entry["value"].get<float>());
    }
    return cmd;
}

std::string NaniCmd::ParseWait(const ParseContext &ctx)
{
    std::string cmd = "@wait";
    auto &data = ctx.refId["data"];

    for (auto &[key, entry] : data.items())
    {
        if (!entry.contains("hasValue") || entry["hasValue"].get<int>() == 0)
            continue;

        cmd += " ";

        if (key == "WaitMode")
            cmd += " " + entry["value"].get<std::string>();
    }
    return cmd;
}

std::string NaniCmd::ParseWaitForInput(const ParseContext &ctx)
{
    return "@wait i";
}

std::string NaniCmd::ParseHideAllActors(const ParseContext &ctx)
{
    std::string cmd = "@hideAll";
    auto &data = ctx.refId["data"];

    for (auto &[key, entry] : data.items())
    {
        if (!entry.contains("hasValue") || entry["hasValue"].get<int>() == 0)
            continue;

        cmd += " ";

        if (key == "Duration")
            cmd += std::format("time:{:g}", entry["value"].get<float>());

        else if (key == "Remove")
            cmd += std::format("lazy:{}", entry["value"].get<int>() ? "true" : "false");
    }
    return cmd;
}