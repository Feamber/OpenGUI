#include "peglib.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Core/Utilities/string_hash.hpp"
#include "OpenGUI/Core/open_string.h"
#include "OpenGUI/Context.h"
#include "../../Text/godot/font.h"
using namespace ostr::literal;

namespace OGUI
{

    StyleFont ParseFontFace(const PropertyList &props)
    {
        using namespace peg;
        StyleFont result;
        for(auto& pair : props)
        {
            switchstr(pair.first)
            {
                casestr("font-family")
                {
                    auto value = pair.second;
                    if(std::starts_with(value, "\"") && std::ends_with(value, "\""))
                        value = value.substr(1, value.size() - 2);
                    else if (value.find_first_of(' ') != std::string::npos)
                        throw parse_error("name cannot contain space.");
                    result.fontFamily = value;
                }
                casestr("src")
                {
                    std::vector<std::string_view> values;
                    std::split(pair.second, values, ",");
                    for(auto& value : values)
                    {
                        std::vector<std::string_view> parts;
                        std::split(value, parts);
                        if(parts.empty())
                            continue;
                        if(!(std::starts_with(parts[0], "url(") && std::ends_with(parts[0], ")")))
                            throw parse_error("url expected.");
                        auto url = parts[0].substr(4, parts[0].size() - 5);
                        if(parts.size() > 1)
                        {
                            if(!(std::starts_with(parts[1], "format(") && std::ends_with(parts[1], ")")) || parts.size() > 2)
                                throw parse_error("unknown function.");
                            auto format = parts[1].substr(7, parts[1].size() - 8);
                            //TODO: font format
                        }
                        std::string path = {url.begin(), url.end()};
                        auto& ctx = Context::Get().fileImpl;
                        auto f = ctx->Open(path.c_str());
                        if(!f)
                        {
                            olog::Warn(u"load font file {} failed."_o.format(path));
                            continue;
                        }
                        //TODO: cache? async?
                        auto length = ctx->Length(f);
                        godot::PackedByteArray buffer;
                        buffer.resize(length);
                        ctx->Read(buffer.data(), length, f);
                        ctx->Close(f);
                        auto& data = result.datas.emplace_back(new godot::FontData);
                        data->set_data(buffer);
                    }
                }
            }
        }
        return result;
    }
}