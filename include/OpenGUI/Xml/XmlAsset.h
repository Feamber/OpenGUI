#pragma once
#include <string>
#include <string_view>
#include <filesystem>
#include <map>
#include "xercesc/dom/DOM.hpp"
#include "XmlFactoryTool.h"

namespace OGUI
{
    using XmlAssetID = size_t;
    using namespace XERCES_CPP_NAMESPACE;

	class XmlAsset
	{
	public:
	    static std::map<XmlAssetID, XmlAsset*> all_xml_asset;

		// TODO 以后有资源系统后这里应该要改
		static XmlAsset* LoadXmlFile(std::filesystem::path file_path);

		static XmlAssetID GenerateID(const std::string& absolute_path);

		static class VisualElement* Instantiate(XmlAssetID asset_id);

        XmlAssetID id;

        XmlAsset(XmlAssetID id, DOMElement& root) :
                id(id),
                root(root)
        {};

        ~XmlAsset();

        static class TemplateContainer* ParseTemplate(DOMElement& xml_root, CreationContext& context);
        static class VisualElement* ParseElement(DOMElement& xml_element, CreationContext& context);

        DOMElement& root;
	};
}