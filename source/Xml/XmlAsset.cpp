
#define DLL_IMPLEMENTATION
#include <fstream>
#include <iostream>
#include "OpenGUI/Text/TextElement.h"
#include "OpenGUI/Xml/XmlFactoryRegistry.h"
#include "OpenGUI/Xml/XmlFactoryTool.h"
#include "OpenGUI/Xml/XmlAsset.h"
#include "tinyxml2/tinyxml2.h"

namespace XmlTool
{
    using namespace std;
    void split(const string_view& s, vector<string_view>& tokens, const string_view& delimiters = " ")
    {
        string::size_type lastPos = s.find_first_not_of(delimiters, 0);
        string::size_type pos = s.find_first_of(delimiters, lastPos);
        while (string::npos != pos || string::npos != lastPos)
        {
            auto substr = s.substr(lastPos, pos - lastPos);
            tokens.push_back(substr);//use emplace_back after C++11
            lastPos = s.find_first_not_of(delimiters, pos);
            pos = s.find_first_of(delimiters, lastPos);
        }
    }
}

namespace OGUI {
    using namespace tinyxml2;

    bool SplitXmlName(ostr::string xml_name, Name& out_prefix, Name& out_name) {
        ostr::string_view token1, token2;
        xml_name.split(u":"_o, &token1, &token2);
        if (token2.is_empty())
        {
            out_prefix = token2;
            out_name = token1;
        }
        else
        {
            out_prefix = token1;
            out_name = token2;
        }
        return true;

        /* orginal: */
        /*std::vector<ostr::string_view> tokens;
        xml_name.split(u":"_o, tokens);
        if (tokens.size() == 2) {
            out_prefix = tokens[0];
            out_name = tokens[1];
            return true;
        }
        else if (tokens.size() == 1) {
            out_prefix = "";
            out_name = tokens[0];
            return true;
        }
        return false;*/
    }

    bool SplitXmlElementName(const XMLElement *xml_element, std::string_view file_path, Name& out_prefix,
                             Name& out_name) {
        if (!SplitXmlName(xml_element->Name(), out_prefix, out_name)) {
            olog::Error(u"XML解析错误：元素名称格式不正常 {} ({})"_o, file_path, xml_element->GetLineNum());
            return false;
        }
        return true;
    }

    bool SplitXmlAttrName(const XMLAttribute *xml_attr, std::string_view file_path, Name& out_prefix, Name& out_name) {
        
        if (!SplitXmlName(xml_attr->Name(), out_prefix, out_name)) {
            olog::Error(u"XML解析错误：属性名称格式不正常 {} ({})"_o, file_path, xml_attr->GetLineNum());
            return false;
        }
        return true;
    }

    void RegisterNamespace(XmlAsset &asset, XMLElement &xml_element) {
        using namespace literal;

        auto xml_attr = xml_element.FirstAttribute();
        while (xml_attr) {
            bool is_delete = false;
            Name out_prefix;
            Name out_name;
            if (SplitXmlAttrName(xml_attr, asset.file_path, out_prefix, out_name)) {
                if (out_prefix == u"xmlns"_name) {
                    is_delete = true;
                    if (!out_name.IsNone())
                        asset.all_namespace.emplace(out_name, xml_attr->Value());
                }
            }

            if(is_delete) //后面不需要这个信息了所以先清理掉
            {
                auto next = xml_attr->Next();
                xml_element.DeleteAttribute(xml_attr->Name());
                xml_attr = next;
            }
            else
                xml_attr = xml_attr->Next();
        }
    }

    Name FindNamespace(std::map<Name, Name> &namespace_map, Name key) {
        auto find = namespace_map.find(key);
        if (find != namespace_map.end()) {
            return find->second;
        }
        return SpecialName::None;
    }

    std::shared_ptr<XmlAsset> XmlAsset::LoadXmlFile(const std::string &file_path) {
        using namespace literal;

        XMLDocument doc;
        doc.LoadFile(file_path.data());
        if (doc.Error()) {
            doc.PrintError();
            return std::shared_ptr<XmlAsset>();
        }

        // 找到<Root>元素
        auto xml_root = doc.FirstChildElement();
        while (xml_root) {
            Name out_prefix;
            Name out_name;
            if (!SplitXmlElementName(xml_root, file_path, out_prefix, out_name))
                return std::shared_ptr<XmlAsset>();
            if (out_name == u"Root"_name)
                break;
            xml_root = xml_root->NextSiblingElement();
        }

        if (!xml_root) {
            olog::Error(u"XML解析错误：没有找到<Root>元素{}"_o, file_path);
        }

        auto shared_asset = std::make_shared<XmlAsset>(file_path);
        XmlAsset *asset = shared_asset.get();
        asset->all_xml_file.push_back(file_path);
        std::vector<std::pair<XMLNode *, XmlElement *>> current_layer = {{xml_root, &asset->root}};
        std::vector<std::pair<XMLNode *, XmlElement *>> next_layer;
        while (!current_layer.empty()) {
            for (auto &pair : current_layer) {
                XMLNode &xml_node = *pair.first;
                XmlElement &asset_element = *pair.second;

                if(xml_node.ToText()) //字符节点是特殊的
                {
                    XMLText &xml_text = *xml_node.ToText();
                    asset_element.name = u"TextValue"_name;
                    asset_element.prefix = u"OGUI"_name;
                    asset_element.namespace_url = u"OGUI"_name;
                    asset_element.xml_asset = asset;
                    asset_element.file_line = xml_node.GetLineNum();
                    asset_element.full_name = u"OGUI.TextValue"_name;
                    auto t = xml_text.Value();;
                    asset_element.text = xml_text.Value();
                }
                else if(xml_node.ToElement())
                {
                    XMLElement &xml_element = *xml_node.ToElement();
                    //先扫描属性注册命名空间
                    RegisterNamespace(*asset, xml_element);
                    //初始化 asset_element
                    Name out_prefix;
                    Name out_name;
                    if (!SplitXmlElementName(&xml_element, file_path, out_prefix, out_name)) std::weak_ptr<XmlAsset>();
                    asset_element.name = out_name;
                    asset_element.prefix = out_prefix;
                    asset_element.namespace_url = FindNamespace(asset->all_namespace, out_prefix);
                    asset_element.xml_asset = asset;
                    asset_element.file_line = xml_element.GetLineNum();
                    if (asset_element.namespace_url != "")
                        asset_element.full_name = u"{}.{}"_o.format(asset_element.namespace_url.ToStringView(), asset_element.name.ToStringView());
                    else
                        asset_element.full_name = asset_element.name;
                    //初始化元素属性
                    auto xml_attr = xml_element.FirstAttribute();
                    while (xml_attr) {
                        Name out_prefix2;
                        Name out_name2;
                        if (SplitXmlAttrName(xml_attr, file_path, out_prefix2, out_name2))
                        {
                            if(FindNamespace(asset->all_namespace, out_prefix2) != "http://www.w3.org/2001/XMLSchema-instance")
                            {
                                if (!out_prefix2.IsNone())
                                {
                                    ostr::string err_msg = u"暂不支持有前缀的属性! attrName:{}"_o.format(xml_attr->Name());
                                    asset_element.PrintError(err_msg);
                                    return std::shared_ptr<XmlAsset>();
                                }
                                asset_element.SetAttribute(out_name2, xml_attr->Value());
                            }
                        } else
                            return std::shared_ptr<XmlAsset>();
                        xml_attr = xml_attr->Next();
                    }
                }

                //添加子元素
                auto xml_child = xml_node.FirstChild();
                int childCount = 0;
                while (xml_child) {
                    if(xml_child && (xml_child->ToText() || xml_child->ToElement()))
                        childCount++;
                    xml_child = xml_child->NextSibling();
                }
                asset_element.children.reserve(childCount);
                xml_child = xml_node.FirstChild();
                while (xml_child) {
                    if(xml_child->ToText() || xml_child->ToElement())
                    {
                        XmlElement &asset_child = asset_element.children.emplace_back();
                        next_layer.emplace_back(std::pair<XMLNode *, XmlElement *>(xml_child, &asset_child));
                    }
                    xml_child = xml_child->NextSibling();
                }
            }
            current_layer.clear();
            std::swap(current_layer, next_layer);
        }
        return shared_asset;
    }

    VisualElement* XmlAsset::Instantiate()
    {
        all_css_file.clear();
        CreationContext new_context{};
        new_context.main_asset = this;
        auto new_template = ParseTemplate(root, new_context);
        if (new_template) 
            return new_template;

        for(auto element : new_context.all)
            delete element;
        return nullptr;
    }

    TemplateContainer *XmlAsset::ParseTemplate(XmlElement &xml_root, CreationContext &context) {
        using namespace literal;

        auto new_template = context.New<TemplateContainer>();
        context.stack_template.emplace_front(&xml_root, new_template);
        if(context.stack.size() == 0)
            context.stack.push_front(new_template);

        // 覆盖属性
        {
            auto &front_template = context.stack_template.front();
            // 解析覆盖属性，并继承父模板的覆盖属性
            if (context.stack_template.size() > 1) // 因为需要从上一级的模板中解析覆盖属性
            {
                for (const XmlElement &xml_child : context.instance_asset->children) {
                    if (xml_child.name == u"AttributeOverrides"_name) {
                        Name element_name;
                        std::set<XmlAttribute> attribute_overrides;
                        for (const XmlAttribute &attr : xml_child.attributes) {
                            if (attr.name == u"name"_name) continue;
                            if (attr.name == u"element_name"_name)
                                element_name = attr.value;
                            else
                                attribute_overrides.emplace(attr.name, attr.value);
                        }

                        if (element_name.IsNone()) {
                            context.is_error = true;
                            xml_child.PrintError(u"<AttributeOverrides element_name=?> 找不到element_name"_o);
                            return nullptr;
                        }
                        // 合并父模板中的属性覆盖
                        auto it = ++context.stack_template.begin();
                        while (it != context.stack_template.end()) {
                            CreationContext::Template &inherited_template = *it;
                            auto &parent_overrides = inherited_template.attribute_overrides[element_name];
                            attribute_overrides.insert(parent_overrides.begin(), parent_overrides.end());
                            ++it;
                        }
                        // 可能有多个一样的 <AttributeOverrides element_name=相同>
                        front_template.attribute_overrides[element_name].insert(attribute_overrides.begin(),
                                                                                attribute_overrides.end());
                    }
                }
            }

            // 应用覆盖属性
            std::vector<XmlElement *> current_layer = {&xml_root};
            std::vector<XmlElement *> next_layer;
            while (!current_layer.empty() && front_template.attribute_overrides.size() > 0) {
                for (XmlElement *xml_element : current_layer) {
                    auto name_attr = xml_element->FindAttribute(u"name"_name);
                    if(name_attr)
                    {
                        auto find = front_template.attribute_overrides.find(name_attr->value);
                        if (find != front_template.attribute_overrides.end()) {
                            const auto &overrides = find->second;
                            for (const XmlAttribute &attribute : overrides) {
                                xml_element->SetAttribute(attribute.name, attribute.value);
                            }
                        }
                    }
                    for (auto &child: xml_element->children)
                        next_layer.emplace_back(&child);
                }
                current_layer.clear();
                std::swap(current_layer, next_layer);
            }
        }

        for(auto& child : xml_root.children)
        {
            if (auto new_element = ParseElement(child, context))
                new_template->PushChild(new_element);
            if (context.is_error)
                break;
        }
        context.stack_template.pop_front();

        if (context.is_error) {
            return nullptr;
        }
        return new_template;
    }

    VisualElement *XmlAsset::ParseElement(XmlElement &xml_element, CreationContext &context)
    {
        IXmlFactory *factory = XmlFactoryRegistry::FindFactory(xml_element.full_name);
        if (factory == nullptr) {
            context.is_error = true;
            olog::Error(u"找不到对应的元素类型 xml_qualified_name: {}"_o, xml_element.full_name.ToStringView());
            return nullptr;
        }

        auto new_element = factory->Create(xml_element, context);
        if (new_element != nullptr) {
            OGUI::TextElement* new_text_element = nullptr;
            if(new_element->IsA("OGUI::TextElement"))
                new_text_element = static_cast<OGUI::TextElement*>(new_element);
            context.stack.push_front(new_element);
            for(auto& child : xml_element.children)
            {
                auto new_child_element = ParseElement(child, context);
                if (new_child_element != nullptr && new_child_element->GetHierachyParent() == nullptr)
                {
                    if(new_text_element)
                    {
                        if(new_text_element->IsA("OGUI::TextElement"))
                            new_text_element->AddInlineText(static_cast<OGUI::TextElement*>(new_child_element));
                        else
                            new_text_element->AddInlineElement(new_child_element);
                    }
                    else
                        new_element->PushChild(new_child_element);
                }
                if (context.is_error) break;
            }
            context.stack.pop_front();
        }
        return new_element;
    }

    void XmlElement::PrintError(ostr::string_view message) const {
        olog::Error(u"{} [{}] <{}> Error: {}"_o, xml_asset->file_path, file_line, full_name.ToStringView(), message);
    }

    void XmlElement::SetAttribute(const Name &attr_name, const std::string &attr_value) {
        for (auto &attr : attributes) {
            if (attr.name == attr_name) {
                attr.value = attr_value;
                return;
            }
        }
        attributes.emplace_back(attr_name, attr_value);
    }

    const XmlAttribute *XmlElement::FindAttribute(const Name &name_) const
    {
        for (auto &attr : attributes) {
            if (attr.name == name_) return &attr;
        }
        return nullptr;
    }
}