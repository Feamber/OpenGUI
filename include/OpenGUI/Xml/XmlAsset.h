#pragma once
#include <string>
#include <string_view>
#include <filesystem>
#include <vector>
#include <map>
#include <set>

namespace OGUI
{
    using XmlAssetID = size_t;

    struct XmlAttribute
    {
        std::string_view name;
        std::string value;

        XmlAttribute(const std::string_view& name, const std::string &value) : name(name), value(value) {}

        bool operator<(const XmlAttribute &rhs) const
        {
            return name < rhs.name;
        }
    };

    struct XmlElement
    {
        std::string_view name = "";
        std::string_view full_name = "";
        std::string_view prefix = "";
        std::string_view namespace_url = "";
        std::string text = "";
        std::vector<XmlAttribute> attributes {};
        std::vector<XmlElement> children {};

        class XmlAsset* xml_asset = nullptr;
        int file_line = 0;

        void PrintError(std::string_view message) const;
        void SetAttribute(const std::string_view &name, const std::string &value);
        const XmlAttribute* FindAttribute(const std::string_view &name) const;
    };

	class XmlAsset : public std::enable_shared_from_this<XmlAsset>
	{
	public:
	    // TODO 以后有资源系统后这里应该要改
		static std::shared_ptr<XmlAsset> LoadXmlFile(const std::string& file_path);
        static class TemplateContainer* ParseTemplate(XmlElement& xml_root, struct CreationContext& context);
        static class VisualElement* ParseElement(XmlElement& xml_element, struct CreationContext& context);

        XmlAsset(const std::string& file_path) : file_path(file_path) {};
        class VisualElement* Instantiate();

        std::string file_path;
        std::map<std::string_view, std::string> all_namespace;
        std::set<std::string> all_string;
        XmlElement root;

        std::list<std::shared_ptr<XmlAsset>> all_child_asset;
	};
}