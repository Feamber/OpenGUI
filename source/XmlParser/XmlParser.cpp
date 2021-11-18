
#define DLL_IMPLEMENTATION
#include "OpenGUI/Bind/AttributeBind.h"
#include "OpenGUI/XmlParser/XmlParser.h"
#include "OpenGUI/XmlParser/XmlElementFactory.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Text/TextElement.h"
#include "OpenGUI/Core/ostring/ostr.h"
#include "OpenGUI/Core/ostring/osv.h"
#include "tinyxml2/tinyxml2.h"
#include "OpenGUI/VisualElement.h"
#include <map>
#include <vector>
#include <list>
#include <fstream>
#include <string>
#include <unordered_map>
#include <memory>

namespace OGUI
{
    using namespace XmlParserHelper;
    namespace XmlHelper
    {
        ostr::string DefaultNamespace = u"OGUI"_o;
        static std::unordered_map<Name, XmlElementFactory*> AllXmlParser;

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
                outFullName = DefaultNamespace + u":"_o + outFullName;
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
                    fullName = DefaultNamespace + u":"_o + xmlName;

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
                XmlElementFactory& xmlParser = *search->second;
                XmlElement& newXmlElementRef = *newXmlElement.get();

                if(!xmlParser.OnParseXmlElement(state, newXmlElementRef))
                {
                    olog::Error(u"XML解析错误：OnParseXmlElement失败 位置：{}  行：{}"_o, node->Value(), node->GetLineNum());
                    return false;
                }

                // 遍历 XmlNode
                if(!ParseXmlNode(state, parent, node->NextSibling()))
                    return false;
                if(!xmlParser.OnParseXmlElementSiblingPost(state, newXmlElementRef))
                {
                    olog::Error(u"XML解析错误：OnParseXmlElementSiblingPost失败 位置：{}  行：{}"_o, node->Value(), node->GetLineNum());
                    return false;
                }

                if(!ParseXmlNode(state, newXmlElementRef, node->FirstChild()))
                    return false;
                if(!xmlParser.OnParseXmlElementChildPost(state, newXmlElementRef))
                {
                    olog::Error(u"XML解析错误：OnParseXmlElementChildPost失败 位置：{}  行：{}"_o, node->Value(), node->GetLineNum());
                    return false;
                }
                return true;
            }
            return true;
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

            // 找到根元素
            tinyxml2::XMLElement* xml_root = doc.FirstChildElement();
            if (!xml_root)
            {
                olog::Error(u"XML解析错误：没有找到根元素"_o);
                return nullptr;
            }

            auto root = ParseXmlElement(state, xml_root);
            if(!root)
            {
                olog::Error(u"XML解析错误：无法解析第一个元素 <{}>"_o, xml_root->Name());
                return nullptr;
            }
            root->isXmlRoot = true;

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
            XmlElementFactory& xmlParser = *search->second;
            VisualElement* newElement = nullptr;
            auto prev = state.validCreateElementStack.size() > 0 ? state.validCreateElementStack.front() : InstantiateXmlStack();
            if(!xmlParser.OnCreateElement(state, xmlElement, newElement, prev.element))
            {
                olog::Error(u"XML实例化错误，OnCreateElement失败：<{}>"_o, xmlElement.fullName);
                return false;
            }
            if(newElement)
            {
                xmlElement.tempElement = newElement;
                xmlElement.tempFactory = prev.factory;
                if(!prev.element)
                    state.root = newElement;
                else
                {
                    bool isAutoPushToParent = true;
                    if(!xmlParser.OnInitElementHierarchy(state, xmlElement, newElement, prev.element, isAutoPushToParent))
                    {
                        olog::Error(u"XML实例化错误，OnInitElementHierarchy失败：<{}>"_o, xmlElement.fullName);
                        return false;
                    }

                    if(isAutoPushToParent && !newElement->GetHierachyParent() && prev.element)
                    {
                        if(!prev.factory->PushChild(state, *prev.xmlElement, prev.element, xmlElement, newElement))
                            return false;
                    }
                }
                newElement->_isXmlRoot = xmlElement.isXmlRoot;
                state.validCreateElementStack.push_front({&xmlParser, &xmlElement, newElement});
                state.all.push_front(newElement);
            }
            else if(!state.root)
            {
                olog::Error(u"XML实例化错误，根元素 OnCreateElement 没有创建VisualElement"_o, xmlElement.fullName);
                return false;
            }

            if(!xmlParser.OnInitElement(state, xmlElement, newElement, prev.element))
            {
                olog::Error(u"XML实例化错误，OnInitElement失败：<{}>"_o, xmlElement.fullName);
                return false;
            }
            

            for(auto child  : xmlElement.children)
            {
                if(!InstantiateXml(state, *child.get()))
                    return false;
            }
            if(!xmlParser.OnInitElementChildPost(state, xmlElement, newElement, prev.element))
            {
                 olog::Error(u"XML实例化错误，OnInitElementChildPost失败：<{}>"_o, xmlElement.fullName);
                return false;
            }

            if(newElement)
                state.validCreateElementStack.pop_front();
            return true;
        };

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

    std::shared_ptr<XmlAsset> LoadXmlFile(const char* filePath, ParseXmlState& state)
    {
        static std::unordered_map<std::string, std::string> cache;
        auto find = cache.find(filePath);
        if(find == cache.end() || !state.useFileCache)
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

    bool RegisterXmlParser(const Name& fullName, XmlElementFactory* factory)
    {
        if(!XmlHelper::AllXmlParser.try_emplace(fullName, factory).second)
        {
            olog::Error(u"重复RegisterXmlParser，fullName：{}"_o, fullName.ToStringView());
            return false;
        }
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

    XmlParserHelper::FindResult XmlParserHelper::FindAttribute(XmlElement& e, Name name, ostr::string& out, VisualElement* owner, AttrBind::OnChangePost changePostFun, bool bidirectional)
    {
        auto search = e.attributes.find(name);
        if(search != e.attributes.end())
        {
            if(IsDataBind(search->second))
                return AddBind(name, search->second.substring(1), out, owner, changePostFun, bidirectional);
            out = search->second;
            return FindResult::OK;
        }
        return FindResult::NotExist;
    };

    XmlParserHelper::FindResult XmlParserHelper::FindAttribute(XmlElement& e, Name name, std::string& out, VisualElement* owner, AttrBind::OnChangePost changePostFun, bool bidirectional)
    {
        auto search = e.attributes.find(name);
        if(search != e.attributes.end())
        {
            if(IsDataBind(search->second))
                return AddBind(name, search->second.substring(1), out, owner, changePostFun, bidirectional);
            auto sv = search->second.to_sv();
            out = {sv.begin(), sv.end()};
            return FindResult::OK;
        }
        return FindResult::NotExist;
    };

    XmlParserHelper::FindResult XmlParserHelper::FindAttribute(XmlElement& e, Name name, bool& out, VisualElement* owner, AttrBind::OnChangePost changePostFun, bool bidirectional)
    {
        auto search = e.attributes.find(name);
        if(search != e.attributes.end())
        {
            if(IsDataBind(search->second))
                return AddBind(name, search->second.substring(1), out, owner, changePostFun, bidirectional);
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

    XmlParserHelper::FindResult XmlParserHelper::FindAttribute(XmlElement& e, Name name, int& out, VisualElement* owner, AttrBind::OnChangePost changePostFun, bool bidirectional)
    {
        auto search = e.attributes.find(name);
        if(search != e.attributes.end())
        {
            if(IsDataBind(search->second))
                return AddBind(name, search->second.substring(1), out, owner, changePostFun, bidirectional);
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

    XmlParserHelper::FindResult XmlParserHelper::FindAttribute(XmlElement& e, Name name, float& out, VisualElement* owner, AttrBind::OnChangePost changePostFun, bool bidirectional)
    {
        auto search = e.attributes.find(name);
        if(search != e.attributes.end())
        {
            if(IsDataBind(search->second))
                return AddBind(name, search->second.substring(1), out, owner, changePostFun, bidirectional);
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

    XmlParserHelper::FindResult XmlParserHelper::FindAttribute(XmlElement& e, Name name, ostr::string_view splitter, std::vector<ostr::string>& out, VisualElement* owner, AttrBind::OnChangePost changePostFun, bool bidirectional)
    {
        auto search = e.attributes.find(name);
        if(search != e.attributes.end())
        {
            if(IsDataBind(search->second))
                return AddBind(name, search->second.substring(1), out, owner, changePostFun, bidirectional);
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

    XmlParserHelper::FindResult XmlParserHelper::FindAttribute(XmlElement& e, Name name, ostr::string_view splitter, std::vector<std::string>& out, VisualElement* owner, AttrBind::OnChangePost changePostFun, bool bidirectional)
    {
        auto search = e.attributes.find(name);
        if(search != e.attributes.end())
        {
            if(IsDataBind(search->second))
                return AddBind(name, search->second.substring(1), out, owner, changePostFun, bidirectional);
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
}