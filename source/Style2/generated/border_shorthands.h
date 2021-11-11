#pragma once
#include "OpenGUI/Style2/Rule.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Style2/generated/border.h"



    
    
    
    
namespace OGUI
{
    namespace Parse
    {
        bool ParseBorderRadius(StyleSheetStorage& sheet, std::string_view name, std::string_view value, StyleRule& rule, std::string& errorMsg)
        {
            YGValue values[4];
            if(!ParseFourSides(value, values[0], values[1], values[2], values[3]))
            {
                errorMsg = "failed to parse border-radius value!";
                return false;
            }
            rule.properties.push_back(StyleProperty{StyleBorder::Id::borderTopLeftRadius, sheet.Push(values[0])});
            rule.properties.push_back(StyleProperty{StyleBorder::Id::borderTopRightRadius, sheet.Push(values[1])});
            rule.properties.push_back(StyleProperty{StyleBorder::Id::borderBottomRightRadius, sheet.Push(values[2])});
            rule.properties.push_back(StyleProperty{StyleBorder::Id::borderBottomLeftRadius, sheet.Push(values[3])});
            return true;
        }
    }
}
    

