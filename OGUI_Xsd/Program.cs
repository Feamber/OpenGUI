using System;
using System.Xml;
using System.Xml.Schema;
using System.IO;
using CommandLine;
using System.Collections.Generic;
using System.Linq;
using System.Diagnostics;
using System.Collections;
using OGUI_Xsd.Template;
using OGUI_Xsd.Properties;

namespace OGUI_Xsd
{
    class Program
    {
        [Verb("XsdToCpp", HelpText = "根据Xsd文件生成 cpp XmlFactory 工厂类")]
        public class XsdToCpp
        {
            [Option('x', "xds", Required = true, HelpText = "主xsd文件路径")]
            public string XsdMainFile { get; set; }

            [Option('h', "outH", Required = true, HelpText = "输出.h文件的目录")]
            public string OutputH { get; set; }

            [Option('c', "outCpp", Required = true, HelpText = "输出.cpp文件的目录")]
            public string OutputCpp { get; set; }
        }

        static int Main(string[] args)
        {
            var exitCode = Parser.Default.ParseArguments<XsdToCpp>(args)
                .MapResult(
                    (XsdToCpp o) => RunXsdToCpp(o),
                    error => {
                        HelpTextExtensions.Output(error);
                        return 1;
                    });
            return exitCode;
        }

        static int RunXsdToCpp(XsdToCpp configure)
        {
           

            // 通过主xsd文件加载所有xsd
            XmlSchemaSet schemaSet = new XmlSchemaSet
            {
                // Enable resolving of external schemas.
                XmlResolver = new XmlUrlResolver()
            };
            schemaSet.ValidationEventHandler += ValidationCallback;
            var fullPath = Path.GetFullPath(configure.XsdMainFile);
            XmlTextReader reader = new XmlTextReader(fullPath);
            XmlSchema xsdMain = XmlSchema.Read(reader, ValidationCallback);
            schemaSet.Add(xsdMain);
            schemaSet.Compile();

            // 给所有元素生成 cpp工厂类
            // 一个命名空间一个头文件
            var visualElementTypeName =  new XmlQualifiedName("VisualElementType", "OGUI");
            XmlSchemaType? visualElementType = null;
            foreach (DictionaryEntry entry in schemaSet.GlobalTypes)
            {
                if ((XmlQualifiedName)entry.Key == visualElementTypeName)
                {
                    visualElementType = (XmlSchemaType)entry.Value;
                    break;
                }
            }
            if(visualElementType == null)
            {
                Debug.Fail("ERROR: Can't find VisualElementType");
                return 1;
            }

            // 收集已定义的xml属性解析器
            var allXmlTypeMap = new Dictionary<string, XmlAttribute>();
            var allXmlType = typeof(XmlAttribute).Assembly.GetTypes().Where(type => type.IsSubclassOf(typeof(XmlAttribute)));
            foreach (Type xmlType in allXmlType)
            {
                XmlAttribute obj = (XmlAttribute)Activator.CreateInstance(xmlType);
                if (allXmlTypeMap.ContainsKey(obj.XmlType()))
                {
                    Debug.Fail("ERROR: XmlAttribute::XmlType() repeat! XmlType: ", obj.XmlType());
                    return 1;
                }
                allXmlTypeMap.Add(obj.XmlType(), obj);
            }

            // 将不同命名空间的元素区分
            var NamespaceMap = new Dictionary<string, List<XmlSchemaElement>>();
            foreach (DictionaryEntry entry in schemaSet.GlobalElements)
            {
                Debug.WriteLine(entry.Key);

                XmlSchemaElement element = (XmlSchemaElement)entry.Value;
                var namespaceName = element.QualifiedName.Namespace;
                if (!NamespaceMap.ContainsKey(namespaceName)) NamespaceMap.Add(namespaceName, new List<XmlSchemaElement>());
                NamespaceMap[namespaceName].Add(element);
            }

            foreach(var pair in NamespaceMap)
            {
                String OutH;
                String OutCpp;
                GenNamespaceXmlFactory(out OutH, out OutCpp, pair.Key, pair.Value, allXmlTypeMap, schemaSet);
                if(OutH.Length == 0) continue;

                Directory.CreateDirectory(configure.OutputH);
                String hFilePath = Path.Combine(configure.OutputH, pair.Key + ".h");
                File.Create(hFilePath).Close();
                if (File.ReadAllText(hFilePath) != OutH)
                {
                    File.WriteAllText(hFilePath, OutH);
                    Debug.WriteLine("生成.h文件：" + Path.GetFullPath(hFilePath));
                }

                Directory.CreateDirectory(configure.OutputCpp);
                String cppFilePath = Path.Combine(configure.OutputCpp, pair.Key + ".cpp");
                File.Create(cppFilePath).Close();
                if (File.ReadAllText(cppFilePath) != OutCpp)
                {
                    File.WriteAllText(cppFilePath, OutCpp);
                    Debug.WriteLine("生成.cpp文件：" + Path.GetFullPath(cppFilePath));
                }
            }

            return 0;
        }

        static void GenNamespaceXmlFactory(out String OutH,out String OutCpp, string namespaceName, List<XmlSchemaElement> allElement, Dictionary<string, XmlAttribute> allXmlTypeMap, XmlSchemaSet schemaSet)
        {
            String AllIncludeH = "";
            String AllIncludeCpp = "";
            String AllXmlFactoryClassH = "";
            String AllXmlFactoryClassCpp = "";
            String XX_API = "";
            foreach (XmlSchemaElement element in allElement)
            {
                XmlSchema xmlSchema = (XmlSchema)element.Parent;
                var annotation = xmlSchema.Items.OfType<XmlSchemaAnnotation>().FirstOrDefault();
                var documentationArray = annotation.Items.OfType<XmlSchemaDocumentation>().ToArray();
                foreach(var doc in documentationArray)
                {
                    String insert = doc.Markup[0].Value;
                    if(insert.Contains("{h}"))
                    {
                        insert = insert.Replace("{h}", "");
                        if (!AllIncludeH.Contains(insert))
                            AllIncludeH += insert + Environment.NewLine;
                    }
                    else if (insert.Contains("{cpp}"))
                    {
                        insert = insert.Replace("{cpp}", "");
                        if (!AllIncludeCpp.Contains(insert))
                            AllIncludeCpp += insert + Environment.NewLine;
                    }
                    else if (insert.Contains("{xx_API}"))
                    {
                        insert = insert.Replace("{xx_API}", "");
                        XX_API = insert;
                    }

                }

                String xmlFactoryClassH = Resources.XmlFactoryClassH;
                String xmlFactoryClassCpp = Resources.XmlFactoryClassCpp;
                String AllAttributeDefinition = "";
                String AllInit = "";
                String AllSwitchInitAttribute = "";
                XmlSchemaComplexType elementType = element.ElementSchemaType as XmlSchemaComplexType;
                if(elementType == null)
                {
                    Debug.Fail("ERROR: 现在不应该有元素的类型不是 XmlSchemaComplexType！ element：" + element.QualifiedName);
                    OutH = "";
                    OutCpp = "";
                    return;
                }

                XmlSchemaType? baseType = null;
                if (elementType.Annotation != null) //这是用来开洞的，强行继承
                {
                    var documentationArray2 = elementType.Annotation.Items.OfType<XmlSchemaDocumentation>().ToArray();
                    foreach (var doc in documentationArray2)
                    {
                        String info = doc.Markup[0].Value;
                        if (info.Contains("{base}"))
                        {
                            info = info.Replace("{base}", "");
                            foreach (DictionaryEntry entry in schemaSet.GlobalTypes)
                            {
                                XmlSchemaType type = (XmlSchemaType)entry.Value;
                                if(type.QualifiedName.ToString() == info)
                                {
                                    baseType = type;
                                    break;
                                }
                            }
                        }

                    }
                }
                else
                    baseType = elementType.BaseXmlSchemaType;

                if(elementType.Annotation != null && baseType == null)
                {
                    Debug.Fail("ERROR: 类型标记了 <Annotation> 但是没匹配到正确的开洞 element：" + element.QualifiedName);
                    OutH = "";
                    OutCpp = "";
                    return;
                }

                XmlSchemaComplexType parentType = baseType as XmlSchemaComplexType;
                if (parentType == null)
                {
                    Debug.Fail("ERROR: 现在不应该有父类型类型不是 XmlSchemaComplexType！ element：" + element.QualifiedName);
                    OutH = "";
                    OutCpp = "";
                    return;
                }

                foreach (DictionaryEntry entry in elementType.AttributeUses)
                {
                    XmlSchemaAttribute attr = (XmlSchemaAttribute)entry.Value;
                    if (parentType.AttributeUses.Contains(attr.QualifiedName)) continue;

                    String attrTypeName = attr.SchemaTypeName.ToString();
                    XmlAttribute? Parser = null;
                    if (allXmlTypeMap.TryGetValue(attrTypeName,out Parser))
                    {
                        if(attr.Use == XmlSchemaUse.Optional && attr.DefaultValue == null)
                        {
                            string optionalAttrDefinition = SingleLineTemplate.OptionalAttrDefinition
                            .Replace("{CppType}", Parser.CppType())
                            .Replace("{AttrName}", attr.Name);
                            AllAttributeDefinition += optionalAttrDefinition + Environment.NewLine;

                            string optionalAttributeInit = SingleLineTemplate.OptionalAttributeInit
                            .Replace("{AttrName}", attr.Name);
                            AllInit += optionalAttributeInit + Environment.NewLine;

                            string optionalSwitchInitAttr = SingleLineTemplate.OptionalSwitchInitAttr
                            .Replace("{AttrName}", attr.Name)
                            .Replace("{XmlValueToCppValue}", Parser.XmlValueStringToCppValueTemplate()
                                .Replace("{XmlValueString}", "attr.value"));
                            AllSwitchInitAttribute += optionalSwitchInitAttr + Environment.NewLine;
                        }
                        else
                        {
                            string attributeDefinition = SingleLineTemplate.AttributeDefinition
                            .Replace("{CppType}", Parser.CppType())
                            .Replace("{AttrName}", attr.Name);
                            AllAttributeDefinition += attributeDefinition + Environment.NewLine;

                            string attributeInit = SingleLineTemplate.AttributeInit
                            .Replace("{AttrName}", attr.Name)
                            .Replace("{Value}", Parser.XmlValueFormatToCppValueFormat(attr.DefaultValue));
                            AllInit += attributeInit + Environment.NewLine;

                            string switchInitAttribute = SingleLineTemplate.SwitchInitAttribute
                            .Replace("{AttrName}", attr.Name)
                            .Replace("{XmlValueToCppValue}", Parser.XmlValueStringToCppValueTemplate()
                                .Replace("{XmlValueString}", "attr.value"));
                            AllSwitchInitAttribute += switchInitAttribute + Environment.NewLine;
                        }
                    }
                    else
                    {
                        Debug.Fail("ERROR: 找不到对应属性类型的XmlAttribute！ attrTypeName：" + attrTypeName);
                        OutH = "";
                        OutCpp = "";
                        return;
                    }
                }
                AllAttributeDefinition = AllAttributeDefinition.TrimEnd(Environment.NewLine.ToCharArray());
                AllInit = AllInit.TrimEnd(Environment.NewLine.ToCharArray());
                AllSwitchInitAttribute = AllSwitchInitAttribute.TrimEnd(Environment.NewLine.ToCharArray());

                String switchCode = "";
                if (AllSwitchInitAttribute.Length != 0)
                {
                    switchCode = Resources.SwitchTemplate.Replace("{AllSwitchInitAttribute}", AllSwitchInitAttribute);
                }

                String parentTypeName;
                if (parentType.QualifiedName.Namespace == "http://www.w3.org/2001/XMLSchema")
                    parentTypeName = "OGUI::IXmlFactory";
                else
                {
                    parentTypeName = parentType.QualifiedName.Namespace + "::" + "IXmlFactory_" + parentType.QualifiedName.Name.TrimEnd("Type".ToCharArray());
                }
                xmlFactoryClassH = xmlFactoryClassH
                    .Replace("{xx_API}", XX_API)
                    .Replace("{ElementName}", element.QualifiedName.Name)
                    .Replace("{Namespace}", element.QualifiedName.Namespace)
                    .Replace("{Qualified}", element.QualifiedName.Namespace + '.' + element.QualifiedName.Name)
                    .Replace("{ParentTypeName}", parentTypeName)
                    .Replace("{Mixed}", elementType.ContentType == XmlSchemaContentType.Mixed ? "mixed = true;" : "")
                    .Replace("{AllAttributeDefinition}", AllAttributeDefinition);

                xmlFactoryClassCpp = xmlFactoryClassCpp
                    .Replace("{ElementName}", element.QualifiedName.Name)
                    .Replace("{ParentTypeName}", parentTypeName)
                    .Replace("{AllInit}", AllInit)
                    .Replace("{Switch}", switchCode);

                AllXmlFactoryClassH += xmlFactoryClassH + Environment.NewLine;
                AllXmlFactoryClassCpp += xmlFactoryClassCpp + Environment.NewLine;
            }

            OutH = Resources.XmlFactoryHFile
                .Replace("{Include}", AllIncludeH)
                .Replace("{Namespace}", namespaceName)
                .Replace("{Content}", AllXmlFactoryClassH);

            OutCpp = Resources.XmlFactoryCppFile
                .Replace("{Include}", AllIncludeCpp)
                .Replace("{Namespace}", namespaceName)
                .Replace("{Content}", AllXmlFactoryClassCpp);
        }

        static void ValidationCallback(object sender, ValidationEventArgs args)
        {
            if (args.Severity == XmlSeverityType.Warning)
                Debug.Write("WARNING: ");
            else if (args.Severity == XmlSeverityType.Error)
                Debug.Fail("ERROR: ");

            Debug.WriteLine(args.Message);
        }
    }
}
