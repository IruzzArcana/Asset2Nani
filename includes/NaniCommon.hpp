#ifndef NANICOMMON_HPP
#define NANICOMMON_HPP

#include <string>
#include <vector>

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

#endif