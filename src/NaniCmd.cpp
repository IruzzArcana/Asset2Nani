#include "NaniCmd.hpp"
#include <format>
#include <iostream>

const std::map<std::string, LineHandler> NaniCmd::s_handler = {
    {"LabelScriptLine", NaniCmd::ParseLabelScript},
    {"CommandScriptLine", NaniCmd::ParseCommandScriptLine},
    {"GenericTextScriptLine", NaniCmd::ParseGenericTextScript},
    {"EmptyScriptLine", NaniCmd::ParseEmptyScriptLine},
    {"CommentScriptLine", NaniCmd::ParseCommentScriptLine},
};

const std::map<std::string, Command> NaniCmd::m_cmds = {
    {
        "SetControlPanelState",
        {
            "SetControlPanelState",
            {
                {ArgType::Int, "history", "history"},
                {ArgType::Int, "setting", "setting"},
            },
        },
    },
    {
        "SetCustomVariable",
        {
            "set",
            {
                {ArgType::String, "Expression"},
            },
        },
    },
    {
        "ModifyTextPrinter",
        {
            "printer",
            {
                {ArgType::Kvp, "IdAndAppearance"},
                {ArgType::Bool, "MakeDefault", "default"},
                {ArgType::Bool, "HideOther", "hideOther"},
            },
        },
    },
    {
        "ModifyCharacter",
        {
            "char",
            {
                {ArgType::Bool, "Wait", "wait"},
                {ArgType::FloatList, "Scale", "scale"},
                {ArgType::Float, "Duration", "time"},
                {ArgType::FloatList, "ScenePosition", "pos"},
                {ArgType::FloatList, "Position", "position"},
                {ArgType::Kvp, "IdAndAppearance"},
            },
        },
    },
    {
        "ModifyBackground",
        {
            "back",
            {
                {ArgType::Bool, "Wait", "wait"},
                {ArgType::FloatList, "Scale", "scale"},
                {ArgType::Float, "Duration", "time"},
                {ArgType::FloatList, "ScenePosition", "pos"},
                {ArgType::Kvp, "AppearanceAndTransition"},
            },
        },
    },
    {
        "CustomFlashOnce",
        {
            "flashOnce",
            {
                {ArgType::Float, "startValue", "startValue"},
                {ArgType::Float, "endValue", "endValue"},
                {ArgType::Float, "duration", "duration"},
                {ArgType::Bool, "Wait", "wait"},

            },
        },
    },
    {
        "CustomExposure",
        {
            "customExposure",
            {
                {ArgType::Float, "startValue", "startValue"},
                {ArgType::Float, "endValue", "endValue"},
                {ArgType::Float, "duration", "duration"},
                {ArgType::Bool, "Wait", "wait"},
            },
        },
    },
    {
        "HideActors",
        {
            "hide",
            {
                {ArgType::Bool, "Wait", "wait"},
                {ArgType::StringList, "ActorIds"},
                {ArgType::Float, "Duration", "time"},
                {ArgType::Bool, "Remove", "lazy"},
            },
        },
    },
    {
        "HideAllActors",
        {
            "hideAll",
            {
                {ArgType::Float, "Duration", "time"},
                {ArgType::Bool, "Remove", "lazy"},
                {ArgType::Bool, "Wait", "wait"},
            },
        },
    },
    {
        "WaitForInput",
        {"wait i", {{ArgType::Int, ""}}},
    },
    {
        "ModifyCamera",
        {
            "camera",
            {{ArgType::FloatList, "Offset", "offset"},
             {ArgType::FloatList, "Rotation", "rotation"},
             {ArgType::Float, "Zoom", "zoom"},
             {ArgType::Float, "Duration", "duration"},
             {ArgType::Bool, "Wait", "wait"},
             {ArgType::String, "EasingTypeName", "easing"}},
        },
    },
    {
        "HidePrinter",
        {
            "hidePrinter",
            {
                {ArgType::Float, "Duration", "time"},
                {ArgType::Bool, "Wait", "wait"},
            },
        },
    },
    {
        "PrintText",
        {
            "print",
            {
                {ArgType::Bool, "Wait", "wait"},
                {ArgType::Text, "Text", ""},
                {ArgType::String, "AuthorId", "author"},
                {ArgType::Float, "RevealSpeed", "speed"},
                {ArgType::Bool, "WaitForInput", "waitInput"},
            },
        },
    },
    {
        "ResetText",
        {
            "resetText",
            {
                {ArgType::Int, "ResetAuthor"},
            },
        },
    },
    {
        "AddChoice",
        {
            "choice",
            {
                {ArgType::Text, "ChoiceSummary"},
                {ArgType::Kvp, "GotoPath", "goto"},
                {ArgType::Bool, "AutoPlay", "play"},
                {ArgType::Bool, "ShowHandler", "show"},
            },

        },
    },
    {
        "PlaySfx",
        {
            "sfx",
            {
                {ArgType::String, "SfxPath"},
                {ArgType::Float, "Volume", "volume"},
                {ArgType::Bool, "Loop", "loop"},
                {ArgType::Float, "FadeInDuration", "fade"},
            },
        },
    },
    {
        "PlayBgm",
        {
            "bgm",
            {
                {ArgType::Bool, "Wait", "wait"},
                {ArgType::String, "BgmPath"},
                {ArgType::Float, "Volume", "volume"},
                {ArgType::Bool, "Loop", "loop"},
                {ArgType::Float, "FadeInDuration", "fade"},
            },
        },
    },
    {
        "StopBgm",
        {
            "stopBgm",
            {
                {ArgType::Bool, "Wait", "wait"},
                {ArgType::String, "BgmPath"},
                {ArgType::Float, "FadeOutDuration", "fade"},
            },
        },
    },
    {
        "Spawn",
        {
            "spawn",
            {
                {ArgType::Bool, "Wait", "wait"},
                {ArgType::String, "Path"},
                {ArgType::StringList, "Params", "params"},
            },
        },
    },
    {
        "SpawnShake",
        {
            "shake",
            {
                {ArgType::Bool, "Wait", "wait"},
                {ArgType::String, "ActorId"},
                {ArgType::Int, "ShakeCount", "count"},
                {ArgType::Float, "ShakeDuration", "time"},
            },
        },
    },
    {
        "Unlock",
        {
            "unlock",
            {
                {ArgType::String, "Id"},
            },
        },
    },
    {
        "Goto",
        {
            "goto",
            {
                {ArgType::Kvp, "Path"},
            },
        },
    },
    {
        "Wait",
        {
            "wait",
            {
                {ArgType::String, "WaitMode"},
            },
        },
    },
    {
        "Stop",
        {"stop"},
    },
};

std::string NaniCmd::Dispatch(const std::string &type, const ParseContext &ctx)
{
    auto it = s_handler.find(type);

    if (it == s_handler.end())
        // throw std::runtime_error(std::format("Unknown line type: {}", type));
        return type + " !!TODO!!";

    return it->second(ctx);
}

std::string NaniCmd::ParseCmd(const ParseContext &ctx)
{
    std::string type = ctx.refId["type"]["class"].get<std::string>();

    auto it = m_cmds.find(type);
    if (it == m_cmds.end())
        return type + " !!TODO!!";

    const Command &cmdDef = it->second;
    std::string cmd = cmdDef.name;
    /*
        if (cmdDef.args.size() == 0)
            return cmd;
    */
    const auto &data = ctx.refId["data"];

    for (auto &[key, entry] : data.items())
    {
        if (!entry.contains("hasValue") || entry["hasValue"].get<int>() == 0)
            continue;

        bool found = false;
        for (const auto &arg : cmdDef.args)
        {
            if (key != arg.entryname)
                continue;

            found = true;

            cmd += " ";

            if (!arg.cmdname.empty())
            {
                cmd += arg.cmdname + ":";
            }

            switch (arg.type)
            {
            case ArgType::Int:
                cmd += std::to_string(entry["value"].get<int>());
                break;

            case ArgType::Bool:
                cmd += entry["value"].get<int>() ? "true" : "false";
                break;

            case ArgType::String:
                cmd += entry["value"].get<std::string>();
                break;

            case ArgType::StringList:
            {
                const auto &params = entry["value"]["Array"];
                for (size_t i = 0; i < params.size(); ++i)
                {
                    if (i > 0)
                        cmd += ",";
                    cmd += params[i]["value"].get<std::string>();
                }
                break;
            }

            case ArgType::Float:
                cmd += std::format("{:g}", entry["value"].get<float>());
                break;

            case ArgType::FloatList:
            {
                const auto &coords = entry["value"]["Array"];
                for (size_t i = 0; i < coords.size(); ++i)
                {
                    if (coords[i]["hasValue"].get<int>() == 0)
                        continue;

                    if (i > 0)
                        cmd += ",";
                    cmd += std::format("{:g}", coords[i]["value"].get<float>());
                }
                break;
            }

            case ArgType::Kvp:
            {
                std::string name = entry["value"]["name"]["value"].get<std::string>();
                std::string val = entry["value"]["value"]["value"].get<std::string>();
                cmd += val.empty() ? name : name + "." + val;
                break;
            }
            case ArgType::Text:
            {
                if (entry["value"]["parts"]["Array"].size() > 1)
                    std::cout << "[WARN] Text>Value>Parts>Array returned more than one" << std::endl;

                std::string tkey = entry["value"]["parts"]["Array"][0]["id"].get<std::string>();
                cmd += std::format("{}|#{}|", ctx.textMap.at(tkey), tkey);
            }
            }
            break;
        }
        if (!found)
        {
            cmd += std::format(" UNKNOWN_ARGUMENT_{} ", key);
        }
    }

    return cmd;
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
            return std::format("{}{}", !ctx.is_inline ? "@" : "", ParseCmd(ParseContext{ref, ctx.textMap, ctx.idx, true}));
    }
    throw std::runtime_error("rid not found");
}

std::string NaniCmd::ParseGenericTextScript(const ParseContext &ctx)
{
    std::string authorPrefix;
    std::string body;
    bool authorExtracted = false;

    const auto &inlined = ctx.refId[ctx.idx]["data"]["inlinedCommands"]["Array"];

    for (const auto &cmdRef : inlined)
    {
        int rid = cmdRef["rid"];
        for (const auto &ref : ctx.refId)
        {
            if (ref["rid"] != rid)
                continue;

            if (ref["type"]["class"].get<std::string>() == "PrintText")
            {
                auto &data = ref["data"];
                if (!authorExtracted &&
                    data.contains("AuthorId") &&
                    data["AuthorId"].contains("hasValue") &&
                    data["AuthorId"]["hasValue"].get<int>() == 1)
                {
                    authorPrefix = std::format("{}: ", data["AuthorId"]["value"].get<std::string>());
                    authorExtracted = true;
                }
                std::string tkey = data["Text"]["value"]["parts"]["Array"][0]["id"].get<std::string>();
                body += std::format("{}|#{}|", ctx.textMap.at(tkey), tkey);
            }
            else
            {
                body += std::format("[{}]", ParseCmd(ParseContext{ref, ctx.textMap, ctx.idx, true}));
            }
            break;
        }
    }

    return authorPrefix + body;
}

std::string NaniCmd::ParseEmptyScriptLine(const ParseContext &ctx)
{
    return "";
}

std::string NaniCmd::ParsePrintText(const ParseContext &ctx)
{
    std::string cmd = "";
    auto &data = ctx.refId["data"];

    if (data.contains("AuthorId") && data["AuthorId"].contains("hasValue") && data["AuthorId"]["hasValue"].get<int>() == 1)
        cmd += std::format("{}: ", data["AuthorId"]["value"].get<std::string>());

    std::string tkey = data["Text"]["value"]["parts"]["Array"][0]["id"].get<std::string>();
    cmd += std::format("{}|#{}|", ctx.textMap.at(tkey), tkey);

    return cmd;
}
