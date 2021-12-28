
#include "OpenGUI/XmlParser/BaseXmlFactory.h"
#include "OpenGUI/Context.h"
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Text/TextElement.h"

namespace OGUI
{
    void OGUI_API RegisterOGUIXmlParser()
    {
        static bool isRegister = false;
        if(isRegister)
            return;
        isRegister = true;

        RegisterXmlParser<VisualElementXmlFactory>();
        RegisterXmlParser<StyleXmlFactory>();
        RegisterXmlParser<InstanceXmlFactory>();
        RegisterXmlParser<AttributeOverridesXmlFactory>();
        RegisterXmlParser<TextXmlFactory>();
    }

    const Name& VisualElementXmlFactory::GetFullName()
    {
        static Name VisualElementName = "OGUI:VisualElement";
        return VisualElementName;
    }

    const Name& VisualElementXmlFactory::Attr_Name()        { static Name name = "name"; return name;}
    const Name& VisualElementXmlFactory::Attr_Class()       { static Name name = "class"; return name;}
    const Name& VisualElementXmlFactory::Attr_Style()       { static Name name = "style"; return name;}
    const Name& VisualElementXmlFactory::Attr_InsertSlot()  { static Name name = "insert-slot"; return name;}
    const Name& VisualElementXmlFactory::Attr_Slot()        { static Name name = "slot"; return name;}
    const Name& VisualElementXmlFactory::Attr_FocusScope()  { static Name name = "focus-scope"; return name;}
    const Name& VisualElementXmlFactory::Attr_KeepFocused() { static Name name = "keep-focused"; return name;}
    const Name& VisualElementXmlFactory::Attr_NavCycleMode(){ static Name name = "nav-cycle-mode"; return name;}
    const Name& VisualElementXmlFactory::Attr_Focusable()   { static Name name = "focusable"; return name;}
    const Name& VisualElementXmlFactory::Attr_NavMode()     { static Name name = "nav-mode"; return name;}
    const Name& VisualElementXmlFactory::Attr_NavUp()       { static Name name = "nav-up"; return name;}
    const Name& VisualElementXmlFactory::Attr_NavDown()     { static Name name = "nav-down"; return name;}
    const Name& VisualElementXmlFactory::Attr_NavLeft()     { static Name name = "nav-left"; return name;}
    const Name& VisualElementXmlFactory::Attr_NavRight()    { static Name name = "nav-right"; return name;}
    const Name& VisualElementXmlFactory::Attr_Filters()    { static Name name = "filters"; return name;}

    bool VisualElementXmlFactory::OnCreateElement(InstantiateXmlState &, XmlElement &, VisualElement *&outNewElement, VisualElement *parent)
    {
        outNewElement = new VisualElement();
        return true;
    }

    bool VisualElementXmlFactory::OnInitElementHierarchy(InstantiateXmlState& state, XmlElement& xe, VisualElement* element, VisualElement* parent, bool& isAutoPushToParent)
    {
        using namespace XmlParserHelper;

        // !插入槽位
        ostr::string insert_slot;
        if(FindAttribute(xe, Attr_InsertSlot(), insert_slot) == FindResult::OK)
        {
            if(!xe.parent.expired() && xe.parent.lock()->_fullName == InstanceXmlFactory::GetFullName())
            {
                auto find_slots = parent->_slots.find(insert_slot);
                if(find_slots != parent->_slots.end())
                {
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
        return true;
    }

    bool VisualElementXmlFactory::OnInitElement(InstantiateXmlState & state, XmlElement& xe, VisualElement *element, VisualElement *parent)
    {
        using namespace XmlParserHelper;
        //------------------------------------------------------CSS
        // !元素名称（原则上唯一，实际可以重复）
        ostr::string name;
        FindAttribute(xe, Attr_Name(), element->_name);
        // !元素类别（同类别元素类型不一定一样）
        FindAttribute(xe, Attr_Class(), u","_o, element->_styleClasses);
        // !内联css样式
        std::string style;
        if(FindAttribute(xe, Attr_Style(), style) == FindResult::OK)
            element->InitInlineStyle(style);

        //------------------------------------------------------XML
        for(auto& attr : xe.attributes)
        {
            using namespace ostr::literal;
            if(attr.first.ToStringView().start_with(u"on:"_o))
                element->_eventBag.try_emplace(attr.first.ToStringView().substring(3), attr.second);
        }

        // !定义一个槽位
        ostr::string slot;
        if(FindAttribute(xe, Attr_Slot(), slot) == FindResult::OK)
        {
            auto it = ++(state.validCreateElementStack.begin());
            VisualElement* root = nullptr;
            while (it != state.validCreateElementStack.end()) 
            {
                if((*it).element->_isXmlRoot)
                {
                    root = (*it).element;
                    break;
                }
                ++it;
            }

            if(root)
            {
                if(!root->_slots.try_emplace(slot, element).second)
                {
                    olog::Warn(u"slot重复，有相同的槽位! XmlElementFullName:{} slot:{}"_o, xe.fullName, slot);
                }
            }
            else
                olog::Fatal(u"找不到XmlRoot! XmlElementFullName:{} slot:{}"_o, xe.fullName, slot);
        }

        //------------------------------------------------------逻辑范围
        // !是否是FocusScope
        FindAttribute(xe, Attr_FocusScope(), element->isFocusScope);
        // !焦点空间在失去焦点后保持当前空间内的焦点
        FindAttribute(xe, Attr_KeepFocused(), element->isKeeyScopeFocused);
        // !导航循环模式
        static const std::map<ostr::string, ENavCycleMode> NavCycleMode = 
        {
            {"none",        ENavCycleMode::None},
            {"automatic",   ENavCycleMode::Automatic},
            {"horizontal",  ENavCycleMode::Horizontal},
            {"vertical",    ENavCycleMode::Vertical},
        };
        FindAttributeEnum(xe, Attr_NavCycleMode(), NavCycleMode, element->navCycleMode);

        //------------------------------------------------------逻辑焦点
        // !是否接受焦点（启用了才能被导航到）
        FindAttribute(xe, Attr_Focusable(), element->focusable);
        // !导航模式
        static const std::map<ostr::string, ENavMode> NavMode = 
        {
            {"none",        ENavMode::None},
            {"automatic",   ENavMode::Automatic},
            {"horizontal",  ENavMode::Horizontal},
            {"vertical",    ENavMode::Vertical},
        };
        FindAttributeEnum(xe, Attr_NavMode(), NavMode, element->navMode);
        // !向上导航目标（使用css选择器语法）
        FindAttribute(xe, Attr_NavUp(), element->navExplicitUp);
        // !向下导航目标（使用css选择器语法）
        FindAttribute(xe, Attr_NavDown(), element->navExplicitDown);
        // !向左导航目标（使用css选择器语法）
        FindAttribute(xe, Attr_NavLeft(), element->navExplicitLeft);
        // !向右导航目标（使用css选择器语法）
        FindAttribute(xe, Attr_NavRight(), element->navExplicitRight);

        // !Xml筛选，只有符合所有FilterTag才显示，否则隐藏
        FindAttribute(xe, Attr_Filters(), u","_o, element->_xmlFilters);
        return true;
    }

    const Name& StyleXmlFactory::GetFullName()
    {
        static Name name = "OGUI:Style";
        return name;
    }

    const Name& StyleXmlFactory::Attr_Path(){ static Name name = "path"; return name;}

    bool StyleXmlFactory::OnParseXmlElement(ParseXmlState & state, XmlElement & xe)
    {
        using namespace XmlParserHelper;
        ostr::string path;
        if(FindAttribute(xe, Attr_Path(), path) == FindResult::OK)
        {
            auto pathSv = path.to_sv();
            state.allCssFile.push_back({pathSv.begin(), pathSv.end()});
        }
        return true;
    }

    bool StyleXmlFactory::OnCreateElement(InstantiateXmlState &, XmlElement & xe, VisualElement *&, VisualElement *parent)
    {
        using namespace XmlParserHelper;
        if(!parent)
        {
            olog::Error(u"<OGUI:Style>必须放在任意VisualElement下!"_o);
            return false;
        }

        ostr::string path;
        if(FindAttribute(xe, Attr_Path(), path) == FindResult::OK)
        {
            auto pathSv = path.to_sv();

            auto sheet = CSSParser::ParseFile(std::string(pathSv.begin(), pathSv.end()));
            if (!sheet)
            {
                olog::Error(u"<OGUI:Style path={}> ParseCSSFile失败!"_o, path);
                return false;
            }
            
            sheet->Initialize();
            parent->_styleSheets.push_back(sheet);
            return true;
        }

        olog::Error(u"<OGUI:Style> 未定义path属性或path值错误!"_o);
        return false;
    }

    const Name& InstanceXmlFactory::GetFullName()
    {
        static Name name = "OGUI:Instance";
        return name;
    }

    const Name& InstanceXmlFactory::Attr_XmlPath() { static Name name = "xml-path"; return name;}

    bool InstanceXmlFactory::OnParseXmlElementChildPost(ParseXmlState & state, XmlElement & xe)
    {
        using namespace XmlParserHelper;
        ostr::string path;
        if(FindAttribute(xe, Attr_XmlPath(), path) == FindResult::OK)
        {
            auto pathSv = path.to_sv();
            std::string stdStr = {pathSv.begin(), pathSv.end()};
            auto newXmlAsset = LoadXmlFile(stdStr.c_str(), state);
            if(!newXmlAsset)
            {
                 olog::Error(u"<OGUI:Instance path={}> 加载Xml失败!"_o, path);
                return false;
            }
            
            auto root = newXmlAsset->root;
            for(auto attr : xe.attributes)
            {
                if(attr.first != Attr_XmlPath())
                    root->attributes[attr.first] = attr.second;
            }
            //覆盖属性
            for(auto& child : xe.children)
            {
                if(child->_fullName == AttributeOverridesXmlFactory::GetFullName())
                {
                    std::map<Name, ostr::string> override;
                    ostr::string elementName;
                    if(FindAttribute(*child.get(), AttributeOverridesXmlFactory::Attr_ElementName(), elementName) == FindResult::OK)
                    {
                        for(auto attr : child->attributes)
                            if(attr.first != AttributeOverridesXmlFactory::Attr_ElementName() && attr.first != VisualElementXmlFactory::Attr_Name())
                                override[attr.first] = attr.second;
                        for(auto& child2 : root->children)
                            if(child2->_fullName != AttributeOverridesXmlFactory::Attr_ElementName())
                                AttributeOverrides(*child2.get(), elementName, override);
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
        else 
        {
            olog::Error(u"<OGUI:Instance> 未定义xml-path属性!"_o);
            return false;
        }
    }

    void InstanceXmlFactory::AttributeOverrides(XmlElement& xe, const ostr::string& target, std::map<Name, ostr::string>& override)
    {
        auto find = xe.attributes.find(VisualElementXmlFactory::Attr_Name());
        if(find != xe.attributes.end() && find->second == target)
            for(auto newAttr : override)
                xe.attributes[newAttr.first] = newAttr.second;
        for(auto& child : xe.children)
        {
            if(child->_fullName == AttributeOverridesXmlFactory::GetFullName())
            {
                for(auto attr : child->attributes)
                {
                    auto find2 = override.find(attr.first);
                    if(find2 != override.end())
                    {
                        override.erase(find2);
                        if(override.size() == 0)
                            return;
                    }
                }
            }
        }
        for(auto& child : xe.children)
            if(child->_fullName != AttributeOverridesXmlFactory::GetFullName())
                AttributeOverrides(*child.get(), target, override);
    }

    const Name& AttributeOverridesXmlFactory::GetFullName()
    {
        static Name name = "OGUI:AttributeOverrides";
        return name;
    }

    const Name& AttributeOverridesXmlFactory::Attr_ElementName() { static Name name = "element-name"; return name;}

    bool AttributeOverridesXmlFactory::OnParseXmlElement(ParseXmlState &, XmlElement & xe)
    {
        if (!xe.parent.expired() && xe.parent.lock()->_fullName == InstanceXmlFactory::GetFullName()) 
            return true;
        olog::Error(u"<OGUI:AttributeOverrides> 必须在 <OGUI:Instance> 下面!"_o);
        return false;
    }

    const Name& TextXmlFactory::GetFullName()
    {
        static Name name = "OGUI:Text";
        return name;
    }

    bool TextXmlFactory::OnCreateElement(InstantiateXmlState &, XmlElement &, VisualElement *&outNewElement, VisualElement *)
    {
        outNewElement = new TextElement();
        return true;
    }

    bool TextXmlFactory::PushChild(InstantiateXmlState&, XmlElement& , VisualElement* , XmlElement& , VisualElement* )
    {
        return true;
    }

    bool TextXmlFactory::OnInitElementChildPost(InstantiateXmlState & state, XmlElement & xe, VisualElement *e, VisualElement *parent)
    {
        auto textElement = (TextElement*)e;
        for(auto& childXe : xe.children)
            TextPushChild(*childXe, *textElement);
        return true;
    }

    void TextXmlFactory::TextPushChild(XmlElement& child, TextElement& textElement)
    {
        if(child.isString)
        {
            auto sv = child.strValue.to_sv();
            ostr::string str;
            for(auto i = sv.begin(); i != sv.end(); ++i)
            {
                // 玩家名：@{名称属性名} 等级：@{等级属性名} 。。。
                if(*i == '$')
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
                                    textElement.AddText(str);
                                    str = "";
                                }
                                textElement.AddBindText(BindName);
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
                textElement.AddText(str);
            return;
        }
        else if(child._fullName == GetFullName() && child.tempElement)
        {
            textElement.AddInlineText((TextElement*)child.tempElement);
            return;
        }
        else if(child.tempElement)
        {
            textElement.AddInlineElement(child.tempElement);
            return;
        }

        for(auto& childXe : child.children)
            TextPushChild(*childXe, textElement);
    }
}