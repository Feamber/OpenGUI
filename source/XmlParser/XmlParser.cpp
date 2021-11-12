
#define DLL_IMPLEMENTATION
#include "OpenGUI/XmlParser/XmlParser.h"
#include <map>
#include <vector>
#include <list>
#include <fstream>
#include <string>
#include <unordered_map>
#include <memory>
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/XmlParser/TemplateContainer.h"
#include "OpenGUI/Text/TextElement.h"
#include "OpenGUI/Core/ostring/ostr.h"
#include "OpenGUI/Core/ostring/osv.h"
#include "tinyxml2/tinyxml2.h"
#include "OpenGUI/VisualElement.h"

namespace OGUI
{
    using namespace XmlParserHelper;

    namespace XmlBase
    {
        OGUI_API ostr::string DefaultNamespace      = u"OGUI"_o;
        OGUI_API Name RootName                      = u"OGUI:Root"_o;
        OGUI_API Name VisualElementName             = u"OGUI:VisualElement"_o;
        OGUI_API Name StyleName                     = u"OGUI:Style"_o;
        OGUI_API Name TemplateName                  = u"OGUI:Template"_o;
        OGUI_API Name InstanceName                  = u"OGUI:Instance"_o;
        OGUI_API Name AttributeOverridesName        = u"OGUI:AttributeOverrides"_o;
        OGUI_API Name TextName                      = u"OGUI:Text"_o;

        OGUI_API Name Attr_Name                     = u"name"_o;
        OGUI_API Name Attr_Path                     = u"path"_o;
        OGUI_API Name Attr_Template                 = u"template"_o;
        OGUI_API Name Attr_ElementName              = u"element-name"_o;
        OGUI_API Name Attr_Class                    = u"class"_o;
        OGUI_API Name Attr_Style                    = u"style"_o;
        OGUI_API Name Attr_InsertSlot               = u"insert-slot"_o;
        OGUI_API Name Attr_Slot                     = u"slot"_o;
        OGUI_API Name Attr_FocusScope               = u"focus-scope"_o;
        OGUI_API Name Attr_KeepFocused              = u"keep-focused"_o;
        OGUI_API Name Attr_NavCycleMode             = u"nav-cycle-mode"_o;
        OGUI_API Name Attr_Focusable                = u"focusable"_o;
        OGUI_API Name Attr_NavMode                  = u"nav-mode"_o;
        OGUI_API Name Attr_NavUp                    = u"nav-up"_o;
        OGUI_API Name Attr_NavDown                  = u"nav-down"_o;
        OGUI_API Name Attr_NavLeft                  = u"nav-left"_o;
        OGUI_API Name Attr_NavRight                 = u"nav-right"_o;
    }

    namespace XmlHelper
    {
        struct XmlParser : public std::enable_shared_from_this<XmlParser>
        {
            Name fullName;
            OnParseXmlElement               parseXmlElement;
            OnParseXmlElementSiblingPost    parseXmlElementSiblingPost;
            OnParseXmlElementChildPost      parseXmlElementChildPost;
            OnCreateElement                 createElement;
            OnInitElement                   initElement;
            OnInitElementChildPost          initElementChildPost;
        };

        static std::unordered_map<Name, std::shared_ptr<XmlParser>> AllXmlParser;

        bool ParseNamespace(tinyxml2::XMLElement* xe, ParseXmlState& state)
        {
            const tinyxml2::XMLAttribute* attr = xe->FirstAttribute();
            while (attr) 
            {
                ostr::string fullName = attr->Name();
                ostr::string_view prefix;
                ostr::string_view namespaceAlias;
                if(fullName.split(u":"_o, &prefix, &namespaceAlias) && prefix == u"xmlns"_o)
                {
                    ostr::string attrValue = attr->Value();
                    if(attrValue.is_empty())
                    {
                        olog::Error(u"XML解析错误：xmlns:{}=空"_o, namespaceAlias);
                        return false;
                    }

                    state.namespaceSet[namespaceAlias] = attrValue;
                }
                attr = attr->Next();
            }
            return true;
        };

        bool ParseElement(
            tinyxml2::XMLElement* xe, 
            ParseXmlState& state, 
            ostr::string& outFullName, 
            ostr::string_view& outName, 
            ostr::string_view& outNamespaceName)
        {
            outFullName = xe->Name();
            if(!outFullName.split(u":"_o, &outNamespaceName, &outName))
            {
                outFullName = XmlBase::DefaultNamespace + u":"_o + outFullName;
                return true;
            }

            auto search = state.namespaceSet.find(outNamespaceName);
            if(search != state.namespaceSet.end())
            {
                outFullName.replace_origin(0, outNamespaceName.length(), search->second);
                return true;
            }

            olog::Error(u"XML解析错误：没有找到<{}>元素"_o, outFullName);
            return false;
        };

        std::shared_ptr<XmlElement> ParseXmlText(ParseXmlState& state, tinyxml2::XMLNode* node)
        {
            if(tinyxml2::XMLText* xml_Text = node->ToText())
            {
                ostr::string v;
                v.decode_from_utf8(xml_Text->Value());
                return std::make_shared<XmlElement>(true, v);
            }
            return nullptr;
        };

        std::shared_ptr<XmlElement> ParseXmlElement(ParseXmlState& state, tinyxml2::XMLNode* node)
        {
            if(tinyxml2::XMLElement* xml_element = node->ToElement())
            {
                if(!ParseNamespace(xml_element, state))
                    return nullptr;

                // 解析元素名
                ostr::string fullName;
                ostr::string xmlName = xml_element->Name();
                ostr::string_view prefix;
                ostr::string_view name;
                if(xmlName.split(u":"_o, &prefix, &name))
                {
                    auto search = state.namespaceSet.find(prefix);
                    if(search != state.namespaceSet.end())
                        fullName = search->second + u":"_o + name;
                    else
                    {
                        olog::Error(u"XML解析错误：找不到命名空间<{}>"_o, xmlName);
                        return nullptr;
                    }
                }
                else 
                    fullName = XmlBase::DefaultNamespace + u":"_o + xmlName;

                auto xmlElement = std::make_shared<XmlElement>(false, fullName);
                const tinyxml2::XMLAttribute* attr = xml_element->FirstAttribute();
                while (attr) 
                {
                    auto result = xmlElement->attributes.try_emplace(attr->Name(), attr->Value());
                    if(!result.second)
                    {
                        olog::Error(u"XML解析错误：<{}> （{}行）有重复的属性，属性名：{}"_o, xmlName, xml_element->GetLineNum(), attr->Name());
                        return nullptr;
                    }
                    attr = attr->Next();
                }
                return xmlElement;
            }
            return nullptr;
        };

        bool ParseXmlNode(ParseXmlState& state, XmlElement& parent, tinyxml2::XMLNode* node)
        {
            if(!node)
                return true;

            std::shared_ptr<XmlElement> newXmlElement = ParseXmlElement(state, node);
            if(!newXmlElement)
                newXmlElement = ParseXmlText(state, node);
            if(!newXmlElement) //无法识别的节点，可能是注释之类的
                return true;

            newXmlElement->parent = parent.weak_from_this();
            parent.children.push_back(newXmlElement);

            if(newXmlElement->isString)
            {
                // 遍历 XmlNode
                if(!ParseXmlNode(state, parent, node->NextSibling()))
                    return false;
            }
            else 
            {
                auto search = AllXmlParser.find(newXmlElement->_fullName);
                if(search == AllXmlParser.end())
                {
                    olog::Error(u"XML解析错误：没有对应的Xml元素解析：{}  行：{}  解析后的全名：{}"_o, node->Value(), node->GetLineNum(), newXmlElement->fullName);
                    return false;
                }
                XmlParser& xmlParser = *search->second.get();
                XmlElement& newXmlElementRef = *newXmlElement.get();

                if(!xmlParser.parseXmlElement(state, newXmlElementRef))
                {
                    olog::Error(u"XML解析错误：OnParseXmlElement失败 位置：{}  行：{}"_o, node->Value(), node->GetLineNum());
                    return false;
                }

                // 遍历 XmlNode
                if(!ParseXmlNode(state, parent, node->NextSibling()))
                    return false;
                if(!xmlParser.parseXmlElementSiblingPost(state, newXmlElementRef))
                {
                    olog::Error(u"XML解析错误：OnParseXmlElementSiblingPost失败 位置：{}  行：{}"_o, node->Value(), node->GetLineNum());
                    return false;
                }

                if(!ParseXmlNode(state, newXmlElementRef, node->FirstChild()))
                    return false;
                if(!xmlParser.parseXmlElementChildPost(state, newXmlElementRef))
                {
                    olog::Error(u"XML解析错误：OnParseXmlElementChildPost失败 位置：{}  行：{}"_o, node->Value(), node->GetLineNum());
                    return false;
                }
                return true;
                }
        };

        std::shared_ptr<XmlAsset> ParseXml(const char* str, ParseXmlState& state)
        {
            state.xmlDocumentStack.push_back(std::make_shared<tinyxml2::XMLDocument>());
            tinyxml2::XMLDocument& doc = *state.xmlDocumentStack.back().get();
            doc.Parse(str);
            if (doc.Error())
            {
                olog::Error(doc.ErrorStr());
                return nullptr;
            }

            // 找到<Root>元素
            tinyxml2::XMLElement* xml_root = doc.FirstChildElement();
            while (xml_root) 
            {
                if(!ParseNamespace(xml_root, state))
                    return nullptr;

                ostr::string fullName;
                ostr::string_view name;
                ostr::string_view namespaceName;
                if(!ParseElement(xml_root, state, fullName, name, namespaceName))
                    return nullptr;

                if (fullName == XmlBase::RootName.ToStringView())
                    break;
                xml_root = xml_root->NextSiblingElement();
            }
            if (!xml_root)
            {
                olog::Error(u"XML解析错误：没有找到<{}>"_o, XmlBase::RootName.ToStringView());
                return nullptr;
            }

            auto root = ParseXmlElement(state, xml_root);
            if(!root)
                return nullptr;

            auto newAsset = std::make_shared<XmlAsset>();
            newAsset->root = root;
            if(ParseXmlNode(state, *root.get(), xml_root->FirstChild()))
                return newAsset;
            return nullptr;
        };

        bool InstantiateXml(InstantiateXmlState& state, XmlElement& xmlElement)
        {
            if(xmlElement.isString)
                return true;
                
            auto search = AllXmlParser.find(xmlElement._fullName);
            if(search == AllXmlParser.end())
            {
                olog::Error(u"XML实例化错误，没有对应的Xml元素解析器：<{}>"_o, xmlElement.fullName);
                return false;
            }
            XmlParser& xmlParser = *search->second.get();
            VisualElement* newElement = nullptr;
            VisualElement* prevElement = state.stack.size() > 0 ? state.stack.front() : nullptr;
            if(!xmlParser.createElement(state, xmlElement, newElement, prevElement))
            {
                 olog::Error(u"XML实例化错误，OnCreateElement失败：<{}>"_o, xmlElement.fullName);
                return false;
            }
            if(newElement)
            {
                if(VisualElement* parent = prevElement)
                    parent->PushChild(newElement);
                else
                    state.root = newElement;
                state.stack.push_front(newElement);
                state.all.push_front(newElement);
            }

            if(!xmlParser.initElement(state, xmlElement, newElement, prevElement))
            {
                 olog::Error(u"XML实例化错误，OnInitElement失败：<{}>"_o, xmlElement.fullName);
                return false;
            }
            xmlElement.tempElement = newElement;

            for(auto child  : xmlElement.children)
            {
                if(!InstantiateXml(state, *child.get()))
                    return false;
            }
            if(!xmlParser.initElementChildPost(state, xmlElement, newElement, prevElement))
            {
                 olog::Error(u"XML实例化错误，OnInitElementChildPost失败：<{}>"_o, xmlElement.fullName);
                return false;
            }

            if(newElement)
                state.stack.pop_front();
            return true;
        };

        void AttributeOverrides(XmlElement& xe, const ostr::string& target, std::map<Name, ostr::string>& override)
        {
            auto find = xe.attributes.find(XmlBase::Attr_Name);
            if(find != xe.attributes.end() && find->second == target)
                for(auto newAttr : override)
                    xe.attributes[newAttr.first] = newAttr.second;
            for(auto& child : xe.children)
            {
                if(child->_fullName == XmlBase::AttributeOverridesName)
                {
                    for(auto attr : child->attributes)
                    {
                        auto find = override.find(attr.first);
                        if(find != override.end())
                        {
                            override.erase(find);
                            if(override.size() == 0)
                                return;
                        }
                    }
                }
            }
            for(auto& child : xe.children)
                if(child->_fullName != XmlBase::AttributeOverridesName)
                    AttributeOverrides(*child.get(), target, override);
        }

        void DebugPrint(XmlElement& xe, int indent = 0)
        {
            ostr::string indentStr;
            for(int i = indent; i > 0; --i)
                indentStr += u' ';
            if(!xe.isString)
            {
                ostr::string attrStr;
                for (auto& attr : xe.attributes) 
                {
                    attrStr += u' ';
                    attrStr += attr.first.ToStringView();
                    attrStr += u'=';
                    attrStr += attr.second;
                }
                if(xe.children.empty())
                {
                    olog::Debug(u"{}<{}{}/>"_o, indentStr, xe.fullName, attrStr);
                }
                else 
                {
                    olog::Debug(u"{}<{}{}>"_o, indentStr, xe.fullName, attrStr);
                    for(auto child : xe.children)
                        DebugPrint(*child.get(), indent + 2);
                    olog::Debug(u"{}</{}>"_o, indentStr, xe.fullName);
                }
            }
            else 
            {
                 olog::Debug(u"{}{}"_o, indentStr, xe.strValue);
            }
        }
    }

    std::shared_ptr<XmlAsset> LoadXmlFile(const char* filePath, ParseXmlState& state, bool useCache)
    {
        static std::unordered_map<std::string, std::string> cache;
        auto find = cache.find(filePath);
        if(find == cache.end() || !useCache)
        {
            std::ifstream ifs(filePath);
		    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
            cache[filePath] = std::move(content);
        }
        
        state.allXmlFile.push_back(filePath);
        return ParseXml(cache[filePath].c_str(), state);
    };

	std::shared_ptr<XmlAsset> ParseXml(const char* str, ParseXmlState& state)
    {
       return XmlHelper::ParseXml(str, state);
    };

    bool RegisterXmlParser
    (
        const Name& fullName,
        OnParseXmlElement               parseXmlElement,
        OnParseXmlElementSiblingPost    parseXmlElementSiblingPost,
        OnParseXmlElementChildPost      parseXmlElementChildPost,
        OnCreateElement                 createElement,
        OnInitElement                   initElement,
        OnInitElementChildPost          initElementChildPost
    )
    {
        auto find = XmlHelper::AllXmlParser.find(fullName);
        if(find != XmlHelper::AllXmlParser.end())
        {
            olog::Error(u"重复RegisterXmlParser，fullName：{}"_o, fullName.ToStringView());
            return false;
        }
        auto newXmlParser = std::make_shared<XmlHelper::XmlParser>();
        newXmlParser->fullName = fullName;
        newXmlParser->parseXmlElement = parseXmlElement;
        newXmlParser->parseXmlElementSiblingPost = parseXmlElementSiblingPost;
        newXmlParser->parseXmlElementChildPost = parseXmlElementChildPost;
        newXmlParser->createElement = createElement;
        newXmlParser->initElement = initElement;
        newXmlParser->initElementChildPost = initElementChildPost;

        XmlHelper::AllXmlParser[fullName] = newXmlParser;
        return true;
    };

    void XmlElement::DebugPrint()
    {
        XmlHelper::DebugPrint(*this);
    };

    XmlElement::XmlElement(bool isString, ostr::string value) : isString(isString)
    {
        if(isString)
        {
            strValue = value;
        }
        else 
        {
            _fullName = value;
            (ostr::string_view&)fullName = _fullName.ToStringView();
            assert(fullName.split(u":"_o, (ostr::string_view*)&namespaceName, (ostr::string_view*)&name));
        }
    };

    VisualElement* XmlAsset::Instantiate(InstantiateXmlState& state)
    {
        if(!XmlHelper::InstantiateXml(state, *root.get()))
        {
            state.all.reverse();
            for(auto e : state.all)
                delete e;
            return nullptr;
        }
        return state.root;
    };

    XmlParserHelper::FindResult XmlParserHelper::FindAttribute(XmlElement& e, Name name, ostr::string& out)
    {
        auto search = e.attributes.find(name);
        if(search != e.attributes.end())
        {
            if(IsDataBind(search->second))
                return FindResult::DataBind;
            out = search->second;
            return FindResult::OK;
        }
        return FindResult::NotExist;
    };

    XmlParserHelper::FindResult XmlParserHelper::FindAttribute(XmlElement& e, Name name, std::string& out)
    {
        auto search = e.attributes.find(name);
        if(search != e.attributes.end())
        {
            if(IsDataBind(search->second))
                return FindResult::DataBind;
            auto sv = search->second.to_sv();
            out = {sv.begin(), sv.end()};
            return FindResult::OK;
        }
        return FindResult::NotExist;
    };

    XmlParserHelper::FindResult XmlParserHelper::FindAttribute(XmlElement& e, Name name, bool& out)
    {
        auto search = e.attributes.find(name);
        if(search != e.attributes.end())
        {
            if(IsDataBind(search->second))
                return FindResult::DataBind;
            if(!search->second.to_sv().is_bool())
            {
                olog::Warn(u"XmlParserHelper::FindAttribute bool ErrorType! XmlElementFullName:{} AttributeName:{} AttributeValue:{}"_o, e.fullName, name.ToStringView(), search->second);
                return FindResult::ErrorType;
            }
            out = search->second.to_sv().to_bool();
            return FindResult::OK;
        }
        return FindResult::NotExist;
    };

    XmlParserHelper::FindResult XmlParserHelper::FindAttribute(XmlElement& e, Name name, int& out)
    {
        auto search = e.attributes.find(name);
        if(search != e.attributes.end())
        {
            if(IsDataBind(search->second))
                return FindResult::DataBind;
            if(!search->second.to_sv().is_integer())
            {
                olog::Warn(u"XmlParserHelper::FindAttribute int ErrorType! XmlElementFullName:{} AttributeName:{} AttributeValue:{}"_o, e.fullName, name.ToStringView(), search->second);
                return FindResult::ErrorType;
            }
            out = search->second.to_sv().to_int();
            return FindResult::OK;
        }
        return FindResult::NotExist;
    };

    XmlParserHelper::FindResult XmlParserHelper::FindAttribute(XmlElement& e, Name name, float& out)
    {
        auto search = e.attributes.find(name);
        if(search != e.attributes.end())
        {
            if(IsDataBind(search->second))
                return FindResult::DataBind;
            if(!search->second.to_sv().is_number())
            {
                olog::Warn(u"XmlParserHelper::FindAttribute float ErrorType! XmlElementFullName:{} AttributeName:{} AttributeValue:{}"_o, e.fullName, name.ToStringView(), search->second);
                return FindResult::ErrorType;
            }
            out = search->second.to_sv().to_float();
            return FindResult::OK;
        }
        return FindResult::NotExist;
    };

    XmlParserHelper::FindResult XmlParserHelper::FindAttribute(XmlElement& e, Name name, ostr::string_view splitter, std::vector<ostr::string>& out)
    {
        auto search = e.attributes.find(name);
        if(search != e.attributes.end())
        {
            if(IsDataBind(search->second))
                return FindResult::DataBind;
            std::vector<ostr::string_view> array;
            if(search->second.split(splitter, array))
                for(const auto& str : array)
                    out.push_back(str);
            else
                out.push_back(search->second);
            return FindResult::OK;
        }
        return FindResult::NotExist;
    };

    XmlParserHelper::FindResult XmlParserHelper::FindAttribute(XmlElement& e, Name name, ostr::string_view splitter, std::vector<std::string>& out)
    {
        auto search = e.attributes.find(name);
        if(search != e.attributes.end())
        {
            if(IsDataBind(search->second))
                return FindResult::DataBind;
            std::vector<ostr::string_view> array;
            if(search->second.split(splitter, array))
                for(const auto& str : array)
                    out.push_back({str.begin(), str.end()});
            else
            {
                auto sv = search->second.to_sv();
                out.push_back({sv.begin(), sv.end()});
            }
            return FindResult::OK;
        }
        return FindResult::NotExist;
    };

    bool XmlParserHelper::IsDataBind(ostr::string_view value)
    {
        return value.length() > 1 && *value.begin() == u'$';
    }

    void XmlBase::RegisterOGUIXmlParser()
    {
        RegisterXmlParser(
            XmlBase::RootName,
            OnParseXmlElement_Empty,
            OnParseXmlElement_Empty, 
            OnParseXmlElement_Empty, 
            [](InstantiateXmlState&, XmlElement&, VisualElement*& out, VisualElement*)
            {
                out = new TemplateContainer();
                return true;
            },
            OnInitElement_VisualElement,
            OnInstantiateXmlElement_Empty
        );

        RegisterXmlParser(
            XmlBase::VisualElementName,
            OnParseXmlElement_Empty,
            OnParseXmlElement_Empty, 
            OnParseXmlElement_Empty, 
            [](InstantiateXmlState&, XmlElement&, VisualElement*& out, VisualElement*)
            {
                out = new VisualElement();
                return true;
            },
            OnInitElement_VisualElement,
            OnInstantiateXmlElement_Empty
        );

        RegisterXmlParser(
            XmlBase::StyleName,
            [](ParseXmlState& state, XmlElement& xe)
            {
                ostr::string path;
                if(FindAttribute(xe, XmlBase::Attr_Path, path) == FindResult::OK)
                {
                    auto pathSv = path.to_sv();
                    state.allCssFile.push_back({pathSv.begin(), pathSv.end()});
                }
                return true;
            },
            OnParseXmlElement_Empty, 
            OnParseXmlElement_Empty, 
            [](InstantiateXmlState&, XmlElement& xe, VisualElement*, VisualElement* parent)
            {
                if(!parent)
                {
                    olog::Error(u"<OGUI:Style>必须放在<OGUI:VisualElement>下!"_o);
                    return false;
                }

                ostr::string path;
                if(FindAttribute(xe, XmlBase::Attr_Path, path) == FindResult::OK)
                {
                    auto pathSv = path.to_sv();

                    auto value = ParseCSSFile(std::string(pathSv.begin(), pathSv.end()));
                    if (!value)
                    {
                        olog::Error(u"<OGUI:Style path={}> ParseCSSFile失败!"_o, path);
                        return false;
                    }
                    auto sheet = new StyleSheet{value.value()};
                    sheet->Initialize();
                    parent->_styleSheets.push_back(sheet);
                    return true;
                }

                olog::Error(u"<OGUI:Style> 未定义path属性或path值错误!"_o);
                return false;
            },
            OnInstantiateXmlElement_Empty,
            OnInstantiateXmlElement_Empty
        );

        RegisterXmlParser(
            XmlBase::TemplateName,
            [](ParseXmlState& state, XmlElement& xe)
            {
                ostr::string name;
                if(FindAttribute(xe, XmlBase::Attr_Name, name) == FindResult::OK)
                {
                    ostr::string path;
                    if(FindAttribute(xe, XmlBase::Attr_Path, path) == FindResult::OK)
                    {
                        auto pathSv = path.to_sv();
                        if(state.allTemplateStack.empty())
                            state.allTemplateStack.push_front({});
                        state.allTemplateStack.front()[name] = {pathSv.begin(), pathSv.end()};
                    }
                    else 
                    {
                        olog::Error(u"<OGUI:Template> 未定义path属性!"_o);
                        return false;
                    }
                }
                else 
                {
                    olog::Error(u"<OGUI:Template> 未定义name属性!"_o);
                    return false;
                }
                return true;
            },
            OnParseXmlElement_Empty, 
            OnParseXmlElement_Empty,
            OnInstantiateXmlElement_Empty,
            OnInstantiateXmlElement_Empty,
            OnInstantiateXmlElement_Empty
        );

        RegisterXmlParser(
            XmlBase::InstanceName,
            OnParseXmlElement_Empty,
            OnParseXmlElement_Empty, 
            [](ParseXmlState& state, XmlElement& xe)
            {
                ostr::string templateName;
                if(FindAttribute(xe, XmlBase::Attr_Template, templateName) == FindResult::OK)
                {
                    if(state.allTemplateStack.empty())
                        state.allTemplateStack.push_front({});
                    auto& allTemplate = state.allTemplateStack.front();
                    auto find = allTemplate.find(templateName);
                    if(find != allTemplate.end())
                    {
                        state.allTemplateStack.push_front({});
                        auto newXmlAsset = LoadXmlFile(find->second.c_str(), state);
                        state.allTemplateStack.pop_front();
                        if(!newXmlAsset)
                        {
                             olog::Error(u"<OGUI:Instance template={}> 加载模板失败! filePath:{}"_o, templateName, find->second);
                            return false;
                        }
                        
                        auto root = newXmlAsset->root;
                        for(auto attr : xe.attributes)
                        {
                            if(attr.first != XmlBase::Attr_Template)
                                root->attributes[attr.first] = attr.second;
                        }
                        //覆盖属性
                        for(auto& child : xe.children)
                        {
                            if(child->_fullName == AttributeOverridesName)
                            {
                                std::map<Name, ostr::string> override;
                                ostr::string elementName;
                                if(FindAttribute(*child.get(), XmlBase::Attr_ElementName, elementName) == FindResult::OK)
                                {
                                    for(auto attr : child->attributes)
                                        if(attr.first != XmlBase::Attr_ElementName && attr.first != XmlBase::Attr_Name)
                                            override[attr.first] = attr.second;
                                    for(auto& child : root->children)
                                        if(child->_fullName != AttributeOverridesName)
                                            XmlHelper::AttributeOverrides(*child.get(), elementName, override);
                                }
                                else
                                {
                                    olog::Error(u"<OGUI:AttributeOverrides> 未定义element-name属性!"_o);
                                    return false;
                                }
                            }
                        }

                        root->children.insert(root->children.end(), xe.children.begin(), xe.children.end());
                        xe.children.clear();
                        xe.children.push_back(root);
                        root->parent = xe.weak_from_this();
                        return true;
                    }
                    olog::Error(u"<OGUI:Instance template={}> 没找到对应的<OGUI:Template>!"_o, templateName);
                    return false;
                }
                else
                {
                    olog::Error(u"<OGUI:Instance> 未定义template属性!"_o);
                    return false;
                }
            }, 
            OnInstantiateXmlElement_Empty,
            OnInstantiateXmlElement_Empty,
            OnInstantiateXmlElement_Empty
        );

        // 主要属性覆盖逻辑在 OGUI:Instance 里面
        RegisterXmlParser(
            XmlBase::AttributeOverridesName,
            [](ParseXmlState& state, XmlElement& xe)
            {
                if (!xe.parent.expired() && xe.parent.lock()->_fullName == InstanceName) 
                    return true;
                olog::Error(u"<OGUI:AttributeOverrides> 必须在 <OGUI:Instance> 下面!"_o);
                return false;
            },
            OnParseXmlElement_Empty, 
            OnParseXmlElement_Empty, 
            OnInstantiateXmlElement_Empty,
            OnInstantiateXmlElement_Empty,
            OnInstantiateXmlElement_Empty
        );

        RegisterXmlParser(
            XmlBase::TextName,
            OnParseXmlElement_Empty,
            OnParseXmlElement_Empty, 
            OnParseXmlElement_Empty, 
            [](InstantiateXmlState&, XmlElement&, VisualElement*& out, VisualElement*)
            {
                out = new TextElement();
                return true;
            },
            OnInitElement_VisualElement,
            [](InstantiateXmlState&, XmlElement& xe, VisualElement* e, VisualElement*)
            {
                auto textElement = (TextElement*)e;
                for(auto& child : xe.children)
                {
                    if(child->isString)
                    {
                        auto sv = child->strValue.to_sv();
                        ostr::string str;
                        for(auto i = sv.begin(); i != sv.end(); ++i)
                        {
                            // 玩家名：@{名称属性名} 等级：@{等级属性名} 。。。
                            if(*i == '@')
                            {
                                bool isAdd = false;
                                auto j = i;
                                if(*(++j) == '{')
                                {
                                    ostr::string BindName;
                                    while (++j != sv.end()) 
                                    {
                                        if(*j == '}')
                                        {
                                            if(BindName.is_empty())
                                                break;
                                            if(!str.is_empty())
                                            {
                                                textElement->AddText(str);
                                                str = "";
                                            }
                                            textElement->AddBindText(BindName);
                                            isAdd = true;
                                            i = j;
                                            break;
                                        }
                                        BindName += *j;
                                    }
                                }
                                if(isAdd)
                                    continue;
                            }

                            str += *i;
                        }
                        if(!str.is_empty())
                            textElement->AddText(str);
                    }
                    else if(child->_fullName == TextName && child->tempElement)
                        textElement->AddInlineText((TextElement*)child->tempElement);
                    else if(child->tempElement)
                        textElement->AddInlineElement(child->tempElement);
                }
                return true;
            }
        );
    };

    bool XmlParserHelper::OnParseXmlElement_Empty(ParseXmlState&, XmlElement&)
    {
        return true;
    }

    bool XmlParserHelper::OnInstantiateXmlElement_Empty(InstantiateXmlState&, XmlElement&, VisualElement*, VisualElement*)
    {
        return true;
    }

    bool XmlBase::OnInitElement_VisualElement(InstantiateXmlState&, XmlElement& xe, VisualElement* element, VisualElement* parent)
    {
        //------------------------------------------------------CSS
        // !元素名称（原则上唯一，实际可以重复）
        ostr::string name;
        FindAttribute(xe, XmlBase::Attr_Name, element->_name);
        // !元素类别（同类别元素类型不一定一样）
        FindAttribute(xe, XmlBase::Attr_Class, u","_o, element->_styleClasses);
        // !内联css样式
        std::string style;
        if(FindAttribute(xe, XmlBase::Attr_Style, style) == FindResult::OK)
            element->InitInlineStyle(style);

        //------------------------------------------------------XML
        // !插入槽位
        ostr::string insert_slot;
        if(FindAttribute(xe, XmlBase::Attr_InsertSlot, insert_slot) == FindResult::OK)
        {
            if(!xe.parent.expired() && xe.parent.lock()->_fullName == InstanceName)
            {
                auto template_container = (TemplateContainer*)parent;
                auto find_slots = template_container->slots.find(insert_slot);
                if(find_slots != template_container->slots.end())
                {
                    parent->RemoveChild(element);
                    find_slots->second->PushChild(element);
                }
                else
                {
                    olog::Error(u"insert_slot无效，找不到可插入的槽位! XmlElementFullName:{} insert_slot:{}"_o, xe.fullName, insert_slot);
                    return false;
                }
            }
            else
            {
                olog::Error(u"insert_slot只在<OGUI:Instance>下面的元素才有效! XmlElementFullName:{} insert_slot:{}"_o, xe.fullName, insert_slot);
                return false;
            }
        }
        // !定义一个槽位
        ostr::string slot;
        if(FindAttribute(xe, XmlBase::Attr_Slot, slot) == FindResult::OK)
        {
            VisualElement* find = parent;
            TemplateContainer* templateContainer = nullptr;
            while (find) 
            {
                if(find->IsA("OGUI::TemplateContainer"))
                {
                    templateContainer = (TemplateContainer*)find;
                    break;
                }
                find = find->GetHierachyParent();
            }

            if(templateContainer)
            {
                auto find_slots = templateContainer->slots.find(slot);
                if(find_slots != templateContainer->slots.end())
                    olog::Warn(u"slot重复，有相同的槽位! XmlElementFullName:{} slot:{}"_o, xe.fullName, slot);
                else
                    templateContainer->slots[slot] = element;
            }
            else
                olog::Fatal(u"找不到OGUI::TemplateContainer! XmlElementFullName:{} slot:{}"_o, xe.fullName, slot);
        }

        //------------------------------------------------------逻辑范围
        // !是否是FocusScope
        FindAttribute(xe, XmlBase::Attr_FocusScope, element->isFocusScope);
        // !焦点空间在失去焦点后保持当前空间内的焦点
        FindAttribute(xe, XmlBase::Attr_KeepFocused, element->isKeeyScopeFocused);
        // !导航循环模式
        static std::map<ostr::string, ENavCycleMode> NavCycleMode = 
        {
            {"None",        ENavCycleMode::None},
            {"Automatic",   ENavCycleMode::Automatic},
            {"Horizontal",  ENavCycleMode::Horizontal},
            {"Vertical",    ENavCycleMode::Vertical},
        };
        FindAttributeEnum(xe, XmlBase::Attr_NavCycleMode, NavCycleMode, element->navCycleMode);

        //------------------------------------------------------逻辑焦点
        // !是否接受焦点（启用了才能被导航到）
        FindAttribute(xe, XmlBase::Attr_Focusable, element->focusable);
        // !导航模式
        static std::map<ostr::string, ENavMode> NavMode = 
        {
            {"None",        ENavMode::None},
            {"Automatic",   ENavMode::Automatic},
            {"Horizontal",  ENavMode::Horizontal},
            {"Vertical",    ENavMode::Vertical},
        };
        FindAttributeEnum(xe, XmlBase::Attr_NavMode, NavMode, element->navMode);
        // !向上导航目标（使用css选择器语法）
        FindAttribute(xe, Attr_NavUp, element->navExplicitUp);
        // !向下导航目标（使用css选择器语法）
        FindAttribute(xe, Attr_NavDown, element->navExplicitDown);
        // !向左导航目标（使用css选择器语法）
        FindAttribute(xe, Attr_NavLeft, element->navExplicitLeft);
        // !向右导航目标（使用css选择器语法）
        FindAttribute(xe, Attr_NavRight, element->navExplicitRight);
    };
}