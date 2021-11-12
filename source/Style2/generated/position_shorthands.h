#pragma once
#include "yoga/Yoga.h"
#include "OpenGUI/Style2/Rule.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Style2/generated/position.h"



    
    
    
    
namespace OGUI
{
    namespace Parse
    {
        bool ParseMargin(StyleSheetStorage& sheet, std::string_view name, std::string_view value, StyleRule& rule, std::string& errorMsg)
        {
            YGValue values[4];
            if(!ParseFourSides(value, values[0], values[1], values[2], values[3]))
            {
                errorMsg = "failed to parse margin value!";
                return false;
            }
            rule.properties.push_back(StyleProperty{StylePosition::Id::marginLeft, sheet.Push(values[0])});
            rule.properties.push_back(StyleProperty{StylePosition::Id::marginTop, sheet.Push(values[1])});
            rule.properties.push_back(StyleProperty{StylePosition::Id::marginRight, sheet.Push(values[2])});
            rule.properties.push_back(StyleProperty{StylePosition::Id::marginBottom, sheet.Push(values[3])});
            return true;
        }
    }
}
    

