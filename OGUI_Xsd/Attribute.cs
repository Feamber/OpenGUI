using System;
using System.Collections.Generic;
using System.Text;

namespace OGUI_Xsd
{
    public abstract class XmlAttribute
    {
        public abstract string CppType();
        public abstract string XmlType();
        public abstract string XmlValueFormatToCppValueFormat(string xmlValueString);
        public abstract string XmlValueStringToCppValueTemplate();
    }

    public class Enumeration_XmlAttribute : XmlAttribute
    {
        public override string CppType() { return "std::string_view"; }
        public override string XmlType() { return "Enumeration"; }
        public override string XmlValueFormatToCppValueFormat(string xmlValueString) { return '\"' + xmlValueString + '\"'; }
        public override string XmlValueStringToCppValueTemplate() { return "{XmlValueString}"; }
    }

    public class String_XmlAttribute : XmlAttribute
    {
        public override string CppType() { return "std::string_view"; }
        public override string XmlType() { return "http://www.w3.org/2001/XMLSchema:string"; }
        public override string XmlValueFormatToCppValueFormat(string xmlValueString) { return '\"' + xmlValueString + '\"'; }
        public override string XmlValueStringToCppValueTemplate() { return "{XmlValueString}"; }
    }

    public class Bool_XmlAttribute : XmlAttribute
    {
        public override string CppType() { return "bool"; }
        public override string XmlType() { return "http://www.w3.org/2001/XMLSchema:boolean"; }
        public override string XmlValueFormatToCppValueFormat(string xmlValueString) { return xmlValueString == null ? "false" : xmlValueString; }
        public override string XmlValueStringToCppValueTemplate() { return "{XmlValueString} == \"true\" || {XmlValueString} == \"1\" ? true : false"; }
    }

    public class Float_XmlAttribute : XmlAttribute
    {
        public override string CppType() { return "float"; }
        public override string XmlType() { return "http://www.w3.org/2001/XMLSchema:float"; }
        public override string XmlValueFormatToCppValueFormat(string xmlValueString) { return xmlValueString == null ? "0.0f" : xmlValueString; ; }
        public override string XmlValueStringToCppValueTemplate() { return "std::stof({XmlValueString})"; }
    }

    public class Double_XmlAttribute : XmlAttribute
    {
        public override string CppType() { return "double"; }
        public override string XmlType() { return "http://www.w3.org/2001/XMLSchema:double"; }
        public override string XmlValueFormatToCppValueFormat(string xmlValueString) { return xmlValueString == null ? "0.0" : xmlValueString; ; }
        public override string XmlValueStringToCppValueTemplate() { return "std::stod({XmlValueString})"; }
    }

    public class Int_XmlAttribute : XmlAttribute
    {
        public override string CppType() { return "int"; }
        public override string XmlType() { return "http://www.w3.org/2001/XMLSchema:int"; }
        public override string XmlValueFormatToCppValueFormat(string xmlValueString) { return xmlValueString == null ? "0" : xmlValueString; ; }
        public override string XmlValueStringToCppValueTemplate() { return "std::stoi({XmlValueString})"; }
    }

    public class Long_XmlAttribute : XmlAttribute
    {
        public override string CppType() { return "long"; }
        public override string XmlType() { return "http://www.w3.org/2001/XMLSchema:long"; }
        public override string XmlValueFormatToCppValueFormat(string xmlValueString) { return xmlValueString == null ? "0" : xmlValueString; ; }
        public override string XmlValueStringToCppValueTemplate() { return "std::stol({XmlValueString})"; }
    }
}
