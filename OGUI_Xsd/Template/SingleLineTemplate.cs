using System;
using System.Collections.Generic;
using System.Text;

namespace OGUI_Xsd.Template
{
    public static class SingleLineTemplate
    {
        public static String AttributeDefinition        = "        {CppType} {AttrName};";
        public static String OptionalAttrDefinition     = "        std::optional<{CppType}> {AttrName};";

        public static String AttributeInit              = "        {AttrName} = {Value};";
        public static String OptionalAttributeInit      = "        {AttrName}.reset();";

        public static String SwitchInitAttribute        = "            casestr(\"{AttrName}\") {AttrName} = {XmlValueToCppValue}; return true;";
        public static String OptionalSwitchInitAttr     = "            casestr(\"{AttrName}\") {AttrName}.emplace({XmlValueToCppValue}); return true;";
    }
}
