using System;
using System.Collections.Generic;
using System.Text;

namespace OGUI_Xsd.Template
{
    public static class SingleLineTemplate
    {
        public static String AttributeDefinition        = "        {CppType} {AttrName};" + Environment.NewLine +
                                                          "        std::optional<std::string_view> {AttrName}_bind;";
        public static String OptionalAttrDefinition     = "        std::optional<{CppType}> {AttrName};" + Environment.NewLine +
                                                          "        std::optional<std::string_view> {AttrName}_bind;";

        public static String AttributeInit              = "        {AttrName} = {Value};";
        public static String OptionalAttributeInit      = "        {AttrName}.reset();" + Environment.NewLine +
                                                          "        {AttrName}_bind.reset();";

        public static String SwitchInitAttribute        = "            casestr(\"{AttrName}\") if(attr.value.size() > 0 && attr.value.front() == '$') {AttrName}_bind.emplace(attr.value); else {AttrName} = {XmlValueToCppValue}; return true;";
        public static String OptionalSwitchInitAttr     = "            casestr(\"{AttrName}\") if(attr.value.size() > 0 && attr.value.front() == '$') {AttrName}_bind.emplace(attr.value); else {AttrName}.emplace({XmlValueToCppValue}); return true;";
    }
}
