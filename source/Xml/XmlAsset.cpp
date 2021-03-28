#include <fstream>
#include <iostream>
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
    std::map<XmlAssetID, std::shared_ptr<XmlAsset>> XmlAsset::all_xml_asset = {};

    bool SplitXmlName(std::string_view xml_name, std::string_view &out_prefix, std::string_view &out_name) {
        std::vector<std::string_view> tokens;
        XmlTool::split(xml_name, tokens, ":");
        if (tokens.size() == 2) {
            out_prefix = tokens[0];
            out_name = tokens[1];
            return true;
        } else if (tokens.size() == 1) {
            out_prefix = "";
            out_name = tokens[0];
            return true;
        }


        return false;
    }

    bool SplitXmlElementName(const XMLElement *xml_element, std::string_view file_path, std::string_view &out_prefix,
                             std::string_view &out_name) {
        if (!SplitXmlName(xml_element->Name(), out_prefix, out_name)) {
            std::cerr << "XML解析错误：元素名称格式不正常 " << std::endl;
            std::cerr << file_path << "  (" << xml_element->GetLineNum() << ")" << std::endl;
            return false;
        }
        return true;
    }

    bool SplitXmlAttrName(const XMLAttribute *xml_attr, std::string_view file_path, std::string_view &out_prefix, std::string_view &out_name) {
        if (!SplitXmlName(xml_attr->Name(), out_prefix, out_name)) {
            std::cerr << "XML解析错误：属性名称格式不正常 " << std::endl;
            std::cerr << file_path << "  (" << xml_attr->GetLineNum() << ")" << std::endl;
            return false;
        }
        return true;
    }

    void RegisterNamespace(XmlAsset &asset, XMLElement &xml_element) {
        auto xml_attr = xml_element.FirstAttribute();
        while (xml_attr) {
            bool is_delete = false;
            std::string_view out_prefix;
            std::string_view out_name;
            if (SplitXmlAttrName(xml_attr, asset.file_path, out_prefix, out_name)) {
                if (out_prefix == "xmlns") {
                    is_delete = true;
                    if (!out_name.empty())
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

    std::string_view RegisterString(XmlAsset &asset, std::string_view string) {
        return asset.all_string.insert(
            std::string(string.begin(), string.end())).first->data();
    }

    std::string_view FindNamespace(std::map<std::string_view , std::string> &namespace_map, std::string_view key) {
        auto find = namespace_map.find(key);
        if (find != namespace_map.end()) {
            return find->second;
        }
        return "";
    }

    std::weak_ptr<XmlAsset> XmlAsset::LoadXmlFile(const std::string &file_path) {
        XmlAssetID asset_id = GenerateID(file_path);
        auto result = all_xml_asset.find(asset_id);
        if (result != all_xml_asset.end()) return result->second->weak_from_this();

        XMLDocument doc;
        doc.LoadFile(file_path.data());
        if (doc.Error()) {
            doc.PrintError();
            return std::weak_ptr<XmlAsset>();
        }

        // 找到<Root>元素
        auto xml_root = doc.FirstChildElement();
        while (xml_root) {
            std::string_view out_prefix;
            std::string_view out_name;
            if (!SplitXmlElementName(xml_root, file_path, out_prefix, out_name))
                return std::weak_ptr<XmlAsset>();
            if (out_name == XmlRootElementFactory::element_name)
                break;
            xml_root = xml_root->NextSiblingElement();
        }

        if (!xml_root) {
            std::cerr << "XML解析错误：没有找到<Root>元素" << std::endl;
            std::cerr << file_path << std::endl;
        }

        auto shared_asset = std::make_shared<XmlAsset>(asset_id, file_path);
        XmlAsset *asset = shared_asset.get();
        std::vector<std::pair<XMLElement *, XmlElement *>> current_layer = {{xml_root, &asset->root}};
        std::vector<std::pair<XMLElement *, XmlElement *>> next_layer;
        while (!current_layer.empty()) {
            for (auto &pair : current_layer) {
                XMLElement &xml_element = *pair.first;
                XmlElement &asset_element = *pair.second;
                //先扫描属性注册命名空间
                RegisterNamespace(*asset, xml_element);
                //初始化 asset_element
                std::string_view out_prefix;
                std::string_view out_name;
                if (!SplitXmlElementName(&xml_element, file_path, out_prefix, out_name)) std::weak_ptr<XmlAsset>();
                asset_element.name = RegisterString(*asset, out_name);
                asset_element.prefix = RegisterString(*asset, out_prefix);
                asset_element.namespace_url = FindNamespace(asset->all_namespace, out_prefix);
                asset_element.xml_asset = asset;
                asset_element.file_line = xml_element.GetLineNum();
                if (asset_element.namespace_url != "")
                    asset_element.full_name = RegisterString(*asset,
                                                             std::string({asset_element.namespace_url.begin(),asset_element.namespace_url.end()}) +
                                                             "." +
                                                             std::string({asset_element.name.begin(),asset_element.name.end()}));
                else
                    asset_element.full_name = asset_element.name;
                //初始化元素属性
                auto xml_attr = xml_element.FirstAttribute();
                while (xml_attr) {
                    std::string_view out_prefix2;
                    std::string_view out_name2;
                    if (SplitXmlAttrName(xml_attr, file_path, out_prefix2, out_name2)) {
                        if (out_prefix2 != "") {
                            asset_element.PrintError("暂不支持有前缀的属性! attrName:" + std::string(xml_attr->Name()));
                            return std::weak_ptr<XmlAsset>();
                        }
                        asset_element.SetAttribute(RegisterString(*asset, out_name2), xml_attr->Value());
                    } else
                        return std::weak_ptr<XmlAsset>();
                    xml_attr = xml_attr->Next();
                }
                //添加子元素
                auto xml_child = xml_element.FirstChildElement();
                int childCount = 0;
                while (xml_child) {
                    xml_child = xml_child->NextSiblingElement();
                    childCount++;
                }
                asset_element.children.reserve(childCount);
                xml_child = xml_element.FirstChildElement();
                while (xml_child) {
                    XmlElement &asset_child = asset_element.children.emplace_back();
                    next_layer.emplace_back(std::pair<XMLElement *, XmlElement *>(xml_child, &asset_child));
                    xml_child = xml_child->NextSiblingElement();
                }
            }
            current_layer.clear();
            std::swap(current_layer, next_layer);
        }
        XmlAsset::all_xml_asset[asset_id] = shared_asset;
        return shared_asset->weak_from_this();
    }

    XmlAssetID XmlAsset::GenerateID(const std::string &absolute_path) {
        return std::hash<std::string>{}(absolute_path);
    }

    std::shared_ptr<VisualElement> XmlAsset::Instantiate(XmlAssetID asset_id) {
        auto result = all_xml_asset.find(asset_id);
        if (result == all_xml_asset.end()) 
            return nullptr;

        CreationContext new_context{};
        auto new_template = ParseTemplate(result->second->root, new_context);
        if (new_template) 
            return new_template->shared_from_this();
        return nullptr;
    }

    TemplateContainer *XmlAsset::ParseTemplate(XmlElement &xml_root, CreationContext &context) {
        auto new_template = context.New<TemplateContainer>();
        context.stack_template.emplace_front(xml_root, *new_template);
        if(context.stack.size() == 0)
            context.stack.push_front(new_template);

        // 覆盖属性
        {
            auto &front_template = context.stack_template.front();
            // 解析覆盖属性，并继承父模板的覆盖属性
            if (context.stack_template.size() > 1) // 因为需要从上一级的模板中解析覆盖属性
            {
                for (const XmlElement &xml_child : context.instance_asset->children) {
                    if (xml_child.name == XmlAttributeOverridesElementFactory::element_name) {
                        std::string_view element_name;
                        std::set<XmlAttribute> attribute_overrides;
                        for (const XmlAttribute &attr : xml_child.attributes) {
                            if (attr.name == "name") continue;
                            if (attr.name == "element_name")
                                element_name = attr.value;
                            else
                                attribute_overrides.emplace(attr.name, attr.value);
                        }

                        if (element_name.empty()) {
                            context.is_error = true;
                            xml_child.PrintError("<AttributeOverrides element_name=?> 找不到element_name");
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
                    auto name_attr = xml_element->FindAttribute("name");
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
        IXmlFactory *factory = XmlFactoryRegistry::FindFactory({xml_element.full_name.begin(), xml_element.full_name.end()});
        if (factory == nullptr) {
            context.is_error = true;
            std::cerr << "找不到对应的元素类型 xml_qualified_name: " << xml_element.full_name << std::endl;
            return nullptr;
        }

        auto new_element = factory->Create(xml_element, context);
        if (new_element != nullptr) {
            context.stack.push_front(new_element);
            for(auto& child : xml_element.children)
            {
                auto new_child_element = ParseElement(child, context);
                if (new_child_element != nullptr && new_child_element->GetHierachyParent() == nullptr)
                    new_element->PushChild(new_child_element);

                if (context.is_error) break;
            }
            context.stack.pop_front();
        }
        return new_element;
    }

    void XmlElement::PrintError(std::string_view message) const {
        std::cerr << xml_asset->file_path + " [" + std::to_string(file_line) + "] <" << full_name << "> Error: "
                  << message << std::endl;
    }

    void XmlElement::SetAttribute(const std::string_view &attr_name, const std::string &attr_value) {
        for (auto &attr : attributes) {
            if (attr.name == attr_name) {
                attr.value = attr_value;
                return;
            }
        }
        attributes.emplace_back(attr_name, attr_value);
    }

    const XmlAttribute *XmlElement::FindAttribute(const std::string_view &name) const
    {
        for (auto &attr : attributes) {
            if (attr.name == name) return &attr;
        }
        return nullptr;
    }
}