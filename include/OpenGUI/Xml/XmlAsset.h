#pragma once
#include "OpenGUI/Configure.h"
#include <string>
#include <string_view>
#include <filesystem>
#include "OpenGUI/Core/Containers/vector.hpp"
#include <map>
#include <set>
#include "OpenGUI/Core/Name.h"

namespace OGUI
{
    using XmlAssetID = size_t;

    struct OGUI_API XmlAttribute
    {
        Name name;
        std::string value;

        XmlAttribute(const Name& name, const std::string &value) : name(name), value(value) {}

        bool operator<(const XmlAttribute &rhs) const
        {
            return name.Compare_Literal(rhs.name) < 0;
        }
    };

    struct OGUI_API XmlElement
    {
        Name name = "";
        Name full_name = "";
        Name prefix = "";
        Name namespace_url = "";
        std::string text = "";
        std::vector<XmlAttribute> attributes {};
        std::vector<XmlElement> children {};

        class XmlAsset* xml_asset = nullptr;
        int file_line = 0;

        void PrintError(ostr::string_view message) const;
        void SetAttribute(const Name &name, const std::string &value);
        const XmlAttribute* FindAttribute(const Name &name) const;
    };

	class OGUI_API XmlAsset : public std::enable_shared_from_this<XmlAsset>
	{
	public:
	    // TODO 以后有资源系统后这里应该要改
		static std::shared_ptr<XmlAsset> LoadXmlFile(const std::string& file_path);
        static class TemplateContainer* ParseTemplate(XmlElement& xml_root, struct CreationContext& context);
        static class VisualElement* ParseElement(XmlElement& xml_element, struct CreationContext& context);

        XmlAsset(const std::string& file_path) : file_path(file_path) {};
        class VisualElement* Instantiate();

        std::string file_path;
        std::map<Name, Name> all_namespace;
        XmlElement root;

        std::list<std::shared_ptr<XmlAsset>> all_child_asset;
	};
}