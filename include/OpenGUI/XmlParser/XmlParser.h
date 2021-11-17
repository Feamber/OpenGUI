#pragma once
#include "OpenGUI/Configure.h"
#include "OpenGUI/Core/ostring/osv.h"
#include "OpenGUI/Core/ostring/ostr.h"
#include "OpenGUI/Core/Name.h"
#include "OpenGUI/Core/olog.h"
#include "OpenGUI/VisualElement.h"
#include <functional>
#include <memory>
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

    // 解析xml元素
    using OnParseXmlElement = std::function<bool(ParseXmlState&, XmlElement& /*element*/)>;
    // 解析完同级xml元素的时候
    using OnParseXmlElementSiblingPost = std::function<bool(ParseXmlState&, XmlElement& /*element*/)>;
    // 解析完子级xml元素的时候
    using OnParseXmlElementChildPost = std::function<bool(ParseXmlState&, XmlElement& /*element*/)>;

    // 创建OGUI元素
    using OnCreateElement = std::function<bool(InstantiateXmlState&, XmlElement& /*xmlElement*/, VisualElement*& /*out*/, VisualElement* /*parent*/)>;
    // 初始化OGUI对象
    using OnInitElement = std::function<bool(InstantiateXmlState&, XmlElement& /*xmlElement*/, VisualElement* /*element*/, VisualElement* /*parent*/)>;
    // 初始化完子级元素的时候
    using OnInitElementChildPost = std::function<bool(InstantiateXmlState&, XmlElement& /*xmlElement*/, VisualElement* /*element*/, VisualElement* /*parent*/)>;

    OGUI_API std::shared_ptr<XmlAsset> LoadXmlFile(const char* filePath, ParseXmlState& state);
    OGUI_API std::shared_ptr<XmlAsset> ParseXml(const char* str, ParseXmlState& state);
    OGUI_API bool RegisterXmlParser
    (
        const Name& fullName,
        OnParseXmlElement               parseXmlElement,
        OnParseXmlElementSiblingPost    parseXmlElementSiblingPost,
        OnParseXmlElementChildPost      parseXmlElementChildPost,
        OnCreateElement                 createElement,
        OnInitElement                   initElement,
        OnInitElementChildPost          initElementChildPost
    );

    struct OGUI_API ParseXmlState
    {
        std::map<ostr::string, ostr::string> namespaceSet;
        std::vector<std::shared_ptr<tinyxml2::XMLDocument>> xmlDocumentStack;
        std::list<std::map<ostr::string, std::string>> allTemplateStack;

        std::vector<std::string> allCssFile;
        std::vector<std::string> allXmlFile;

        bool useFileCache = true;
    };

    struct OGUI_API InstantiateXmlState
    {
        VisualElement* root;
        std::list<VisualElement*> stack;
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

        // 只在实例化过程中有用，对应OnCreateElement创建的元素，只应该在initElementChildPost中用
        VisualElement* tempElement = nullptr;

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

        OGUI_API bool IsDataBind(ostr::string_view value);

        OGUI_API bool OnParseXmlElement_Empty(ParseXmlState&, XmlElement&);
        OGUI_API bool OnInstantiateXmlElement_Empty(InstantiateXmlState&, XmlElement&, VisualElement*, VisualElement*);

        

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

    namespace XmlBase
    {
        extern OGUI_API ostr::string DefaultNamespace;
        extern OGUI_API Name RootName;
        extern OGUI_API Name VisualElementName;
        extern OGUI_API Name StyleName;
        extern OGUI_API Name TemplateName;
        extern OGUI_API Name InstanceName;
        extern OGUI_API Name AttributeOverridesName;
        extern OGUI_API Name TextName;

        extern OGUI_API Name Attr_Name;
        extern OGUI_API Name Attr_Path;
        extern OGUI_API Name Attr_Template;
        extern OGUI_API Name Attr_ElementName;
        extern OGUI_API Name Attr_Class;
        extern OGUI_API Name Attr_Style;
        extern OGUI_API Name Attr_InsertSlot;
        extern OGUI_API Name Attr_Slot;
        extern OGUI_API Name Attr_FocusScope;
        extern OGUI_API Name Attr_KeepFocused;
        extern OGUI_API Name Attr_NavCycleMode;
        extern OGUI_API Name Attr_Focusable;
        extern OGUI_API Name Attr_NavMode;
        extern OGUI_API Name Attr_NavUp;
        extern OGUI_API Name Attr_NavDown;
        extern OGUI_API Name Attr_NavLeft;
        extern OGUI_API Name Attr_NavRight;

        OGUI_API void RegisterOGUIXmlParser();
        OGUI_API bool OnInitElement_VisualElement(InstantiateXmlState&, XmlElement&, VisualElement* element, VisualElement* parent);
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