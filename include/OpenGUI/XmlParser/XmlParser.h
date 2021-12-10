#pragma once
#include "OpenGUI/Configure.h"
#include "OpenGUI/Core/ostring/osv.h"
#include "OpenGUI/Core/ostring/ostr.h"
#include "OpenGUI/Core/OMath.h"
#include "OpenGUI/Core/olog.h"
#include "OpenGUI/VisualElement.h"
#include <functional>
#include <memory>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace tinyxml2 
{
    class XMLDocument;
}

namespace OGUI
{
    class VisualElement;
    class XmlAsset;
    struct XmlElement;
    struct ParseXmlState;
    struct InstantiateXmlState;
    using namespace ostr::literal;

    OGUI_API std::shared_ptr<XmlAsset> LoadXmlFile(const char* filePath, ParseXmlState& state);
    OGUI_API std::shared_ptr<XmlAsset> ParseXml(const char* str, ParseXmlState& state);
    OGUI_API bool RegisterXmlParser(const Name& fullName, class XmlElementFactory* factory);
    template<typename Factory, typename = typename std::enable_if<std::is_constructible<XmlElementFactory, Factory>::value>::type>
    bool RegisterXmlParser()
    {
        auto newFactory = new Factory();
        if(!RegisterXmlParser(Factory::GetFullName(), newFactory))
        {
            delete newFactory;
            return false;
        }
        return true;
    }

    struct OGUI_API ParseXmlState
    {
        std::map<ostr::string, ostr::string> namespaceSet;
        std::vector<std::shared_ptr<tinyxml2::XMLDocument>> xmlDocumentStack;
        std::list<std::map<ostr::string, std::string>> allTemplateStack;

        std::vector<std::string> allCssFile;
        std::vector<std::string> allXmlFile;

        bool useFileCache = true;
    };

    struct OGUI_API InstantiateXmlStack
    {
        XmlElementFactory* factory = nullptr;
        XmlElement* xmlElement = nullptr;
        VisualElement* element = nullptr;
    };

    struct OGUI_API InstantiateXmlState
    {
        VisualElement* root;
        std::list<InstantiateXmlStack> validCreateElementStack;
        std::list<VisualElement*> all;
    };

    class OGUI_API XmlAsset : public std::enable_shared_from_this<XmlAsset>
    {
        public:
        VisualElement* Instantiate(InstantiateXmlState& state);
        std::shared_ptr<XmlElement> root;
    };

    struct OGUI_API XmlElement : public std::enable_shared_from_this<XmlElement>
    {
        Name _fullName;
        std::unordered_map<Name, ostr::string> attributes;
        const ostr::string_view fullName;       // OGUI.Tool:Text
        const ostr::string_view name;           // Text
        const ostr::string_view namespaceName;  // OGUI.Tool

        bool isString = false;
        ostr::string strValue;

        std::weak_ptr<XmlElement> parent;
        std::vector<std::shared_ptr<XmlElement>> children;

        bool isXmlRoot = false;
        // 只在实例化过程中有用，对应OnCreateElement创建的元素，只应该在initElementChildPost中用
        VisualElement* tempElement = nullptr;
        XmlElementFactory* tempFactory = nullptr;

        void DebugPrint();

        XmlElement(bool isString, ostr::string value);
        bool operator==(const XmlElement& element) { return _fullName == element._fullName; };
    };

    namespace XmlParserHelper
    {
        enum class FindResult
        {
            OK,
            ErrorType,
            NotExist,
            InvalidBind
        };

        template<typename Enum>
        FindResult FindAttributeEnum(XmlElement& e, Name name, std::map<ostr::string, Enum> allEnumValue, Enum& out, VisualElement* owner = nullptr, AttrBind::OnChangePost changePostFun = {}, bool bidirectional = false);
        OGUI_API FindResult FindAttribute(XmlElement& e, Name name, ostr::string& out, VisualElement* owner = nullptr, AttrBind::OnChangePost changePostFun = {}, bool bidirectional = false);
        OGUI_API FindResult FindAttribute(XmlElement& e, Name name, std::string& out, VisualElement* owner = nullptr, AttrBind::OnChangePost changePostFun = {}, bool bidirectional = false);
        OGUI_API FindResult FindAttribute(XmlElement& e, Name name, bool& out, VisualElement* owner = nullptr, AttrBind::OnChangePost changePostFun = {}, bool bidirectional = false);
        OGUI_API FindResult FindAttribute(XmlElement& e, Name name, int& out, VisualElement* owner = nullptr, AttrBind::OnChangePost changePostFun = {}, bool bidirectional = false);
        OGUI_API FindResult FindAttribute(XmlElement& e, Name name, float& out, VisualElement* owner = nullptr, AttrBind::OnChangePost changePostFun = {}, bool bidirectional = false);
        OGUI_API FindResult FindAttribute(XmlElement& e, Name name, ostr::string_view splitter, std::vector<ostr::string>& out, VisualElement* owner = nullptr, AttrBind::OnChangePost changePostFun = {}, bool bidirectional = false);
        OGUI_API FindResult FindAttribute(XmlElement& e, Name name, ostr::string_view splitter, std::vector<std::string>& out, VisualElement* owner = nullptr, AttrBind::OnChangePost changePostFun = {}, bool bidirectional = false);
        OGUI_API FindResult FindAttribute(XmlElement& e, Name name, ostr::string_view splitter, std::set<Name>& out, VisualElement* owner = nullptr, AttrBind::OnChangePost changePostFun = {}, bool bidirectional = false);
        
        OGUI_API bool IsDataBind(ostr::string_view value);

        OGUI_API bool OnParseXmlElement_Empty(ParseXmlState&, XmlElement&);
        OGUI_API bool OnInstantiateXmlElement_Empty(InstantiateXmlState&, XmlElement&, VisualElement*, VisualElement*);
        OGUI_API bool OnInitElementChildPost_PushChild(InstantiateXmlState&, XmlElement&, VisualElement*, VisualElement*);

        template<class T>
        FindResult AddBind(Name name, ostr::string_view bindName, T& out, VisualElement* owner, AttrBind::OnChangePost changePostFun, bool bidirectional)
        {
            if(!owner)
                return FindResult::InvalidBind;
            owner->AddBind({bindName, &out, std::move(changePostFun)});
            if(bidirectional)
            {
                owner->_bindBag.emplace(name, bindName);
                owner->AddSource({bindName, &out});
            }
            return FindResult::OK;
        }
    }

    template<typename Enum>
    XmlParserHelper::FindResult XmlParserHelper::FindAttributeEnum(XmlElement& e, Name name, std::map<ostr::string, Enum> allEnumValue, Enum& out, VisualElement* owner, AttrBind::OnChangePost changePostFun, bool bidirectional)
    {
        auto search = e.attributes.find(name);
        if(search != e.attributes.end())
        {
            if(IsDataBind(search->second))
                return AddBind(name, search->second.substring(1), out, owner, changePostFun, bidirectional);

            auto find = allEnumValue.find(search->second);
            if(find != allEnumValue.end())
            {
                out = find->second;
                return FindResult::OK;
            }
            else 
            {
                olog::Warn(u"XmlParserHelper::FindAttributeEnum ErrorType! XmlElementFullName:{} AttributeName:{} AttributeValue:{}"_o, e.fullName, name.ToStringView(), search->second);
                return FindResult::ErrorType;
            }
        }
        return FindResult::NotExist;
    };
}