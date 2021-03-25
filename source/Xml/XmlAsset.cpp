#include "OpenGUI/Xml/XmlAsset.h"
#include <fstream>
#include <iostream>
#include <OpenGUI/Xml/XmlFactoryRegistry.h>
#include "OpenGUI/Xml/XmlFactoryTool.h"
#include "xercesc/util/PlatformUtils.hpp"
#include "xercesc/parsers/XercesDOMParser.hpp"
#include "xercesc/sax/SAXException.hpp"
#include "xercesc/sax/HandlerBase.hpp"
#include "xercesc/framework/MemBufInputSource.hpp"
#include "xercesc/util/OutOfMemoryException.hpp"
#include "xercesc/util/XMemory.hpp"
#include "xercesc/dom/DOMNodeIterator.hpp"
#include "xercesc/dom/DOMNodeFilter.hpp"

namespace OGUI
{
	class StrX
	{
	public:
		StrX(const XMLCh* const toTranscode)
		{
			// Call the private transcoding method
			fLocalForm = XMLString::transcode(toTranscode);
		}

		~StrX()
		{
			XMLString::release(&fLocalForm);
		}

		const char* localForm() const
		{
			return fLocalForm;
		}

	private:
		char* fLocalForm;
	};

	inline std::ostream& operator<<(std::ostream& target, const StrX& toDump)
	{
		target << toDump.localForm();
		return target;
	}

	class DOMTreeErrorReporter : public ErrorHandler
	{
	public:
		bool is_error = false;

		void warning(const SAXParseException& toCatch) override
		{
			std::cerr << "Warning at file \"" << StrX(toCatch.getSystemId())
				<< "\", line " << toCatch.getLineNumber()
				<< ", column " << toCatch.getColumnNumber()
				<< "\n   Message: " << StrX(toCatch.getMessage()) << std::endl;
		}

		void error(const SAXParseException& toCatch) override
		{
			is_error = true;
			std::cerr << "Error at file \"" << StrX(toCatch.getSystemId())
				<< "\", line " << toCatch.getLineNumber()
				<< ", column " << toCatch.getColumnNumber()
				<< "\n   Message: " << StrX(toCatch.getMessage()) << std::endl;
		}

		void fatalError(const SAXParseException& toCatch) override
		{
			is_error = true;
			std::cerr << "Fatal Error at file \"" << StrX(toCatch.getSystemId())
				<< "\", line " << toCatch.getLineNumber()
				<< ", column " << toCatch.getColumnNumber()
				<< "\n   Message: " << StrX(toCatch.getMessage()) << std::endl;
		}

		void resetErrors() override
		{
			is_error = false;
		}
	};

    std::map<XmlAssetID, XmlAsset*> XmlAsset::all_xml_asset = {};

    XmlAsset* XmlAsset::LoadXmlFile(std::filesystem::path file_path)
	{
        XmlAssetID asset_id = GenerateID(file_path.generic_string());
        auto result = all_xml_asset.find(asset_id);
        if(result != all_xml_asset.end()) return result->second;

		struct Initial
		{
			bool initial_fail = false;
			XercesDOMParser* parser = nullptr;
			DOMTreeErrorReporter* err_reporter = nullptr;

			Initial()
			{
				try
				{
					XMLPlatformUtils::Initialize();
				}
				catch (const XMLException& toCatch)
				{
					initial_fail = true;
					std::cerr << XMLString::transcode(toCatch.getMessage());
				}
				if (!initial_fail)
				{
					parser = new XercesDOMParser();
					parser->setValidationScheme(XercesDOMParser::Val_Auto);
					parser->setDoNamespaces(true);
					parser->setDoSchema(false);
					parser->setHandleMultipleImports(true);
					parser->setValidationSchemaFullChecking(true);
					parser->setCreateEntityReferenceNodes(false);

					err_reporter = new DOMTreeErrorReporter();
					parser->setErrorHandler(err_reporter);
				}
			}

			~Initial()
			{
				XMLPlatformUtils::Terminate();
			}
		};
		static Initial init;
		if (init.initial_fail) return false;

		std::ifstream infile;
		infile.open(file_path);
		if (infile.fail())
		{
			std::cerr << "Cannot open the list file: " << file_path << std::endl;
			return false;
		}
		
		std::string xml_str{ std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>() };

		// 解析
		{
			XercesDOMParser& parser = *init.parser;
			DOMTreeErrorReporter& err_reporter = *init.err_reporter;

			parser.resetErrors();

			static const char* xml_buff_id = "OGUI XmlAsset";
			MemBufInputSource xml_buff = MemBufInputSource((const XMLByte*)xml_str.data(), xml_str.size(), xml_buff_id);

			bool is_error = false;
			try
			{
				parser.parse(xml_buff);
			}
			catch (const OutOfMemoryException&)
			{
				std::cerr << "OutOfMemoryException" << std::endl;
				is_error = true;
			}
			catch (const XMLException& e)
			{
				std::cerr << "An error occurred during parsing\n   Message: "
					<< StrX(e.getMessage()) << std::endl;
				is_error = true;
			}
			catch (const DOMException& e)
			{
				const unsigned int maxChars = 2047;
				XMLCh errText[maxChars + 1];

				std::cerr << "\nDOM Error during parsing: '" << file_path << "'\n"
					<< "DOMException code is:  " << e.code << std::endl;

				if (DOMImplementation::loadDOMExceptionMsg(e.code, errText, maxChars))
					std::cerr << "Message is: " << StrX(errText) << std::endl;

				is_error = true;
			}
			catch (...)
			{
				std::cerr << "An error occurred during parsing\n " << std::endl;
				is_error = true;
			}
            infile.close();

			// 判断是否解析成功
			if (!is_error && !err_reporter.is_error)
			{
				DOMDocument* doc = parser.adoptDocument();
				DOMElement* root = doc->getDocumentElement();
				// 只在最顶层找 RootElement
                while(!XMLString::compareString(XMLString::transcode(root->getTagName()), XmlRootElementFactory::element_name.data()))
                {
                    root = root->getNextElementSibling();
                    if(root == nullptr) break;
                }
                if(root != nullptr) //找到 RootElement 了
                {
                    XmlAsset* new_asset = new XmlAsset(asset_id, *root);
                    all_xml_asset[new_asset->id] = new_asset;
                    return new_asset;
                }

				// 失败后要释放
				doc->release();
			}
		}
		return nullptr;
	}

    XmlAssetID XmlAsset::GenerateID(const std::string& absolute_path)
    {
        return std::hash<std::string>{}(absolute_path);
    }

    XmlAsset::~XmlAsset()
    {
        root.release();
    }

    std::shared_ptr<VisualElement> XmlAsset::Instantiate(XmlAssetID asset_id)
    {
        auto result = all_xml_asset.find(asset_id);
        if(result == all_xml_asset.end()) return nullptr;

        CreationContext new_context {};
        auto new_template = ParseTemplate(result->second->root, new_context);
        if(new_template) return new_template->shared_from_this();
        return nullptr;
    }

    TemplateContainer* XmlAsset::ParseTemplate(DOMElement &xml_root, CreationContext &context)
    {
        auto new_template = context.New<TemplateContainer>();
        context.stack_template.emplace_front(xml_root, *new_template);

        // 覆盖属性
        {
            auto& front_template = context.stack_template.front();
            // 解析覆盖属性，并继承父模板的覆盖属性
            if(context.stack_template.size() > 1) // 因为需要从上一级的模板中解析覆盖属性
            {
                auto xml_child = context.instance_asset->getFirstElementChild();
                while (xml_child != nullptr)
                {
                    if(XMLString::transcode(xml_child->getLocalName()) == XmlAttributeOverridesElementFactory::element_name)
                    {
                        auto attributes = xml_child->getAttributes();
                        const int attributes_len = attributes->getLength();
                        std::string element_name;
                        std::set<CreationContext::AttributeOverrid> attribute_overrides;
                        for(int i = 0; i < attributes_len; ++i)
                        {
                            auto attr = attributes->item(i);
                            std::string attr_name = XMLString::transcode(attr->getLocalName());
                            std::string attr_value = XMLString::transcode(attr->getNodeValue());
                            if(element_name == "name") continue;
                            if(attr_name == "element_name")
                                element_name = attr_value;
                            else
                                attribute_overrides.emplace(attr_name, attr_value);
                        }
                        if(element_name.empty())
                        {
                            context.is_error = true;
                            std::cerr << "<AttributeOverrides element_name=?> 找不到element_name" << std::endl;
                            return nullptr;
                        }
                        // 合并父模板中的属性覆盖
                        auto it = ++context.stack_template.begin();
                        while (it != context.stack_template.end())
                        {
                            CreationContext::Template& inherited_template = *it;
                            auto& parent_overrides = inherited_template.attribute_overrides[element_name];
                            attribute_overrides.insert(parent_overrides.begin(), parent_overrides.end());
                            ++it;
                        }
                        // 可能有多个一样的 <AttributeOverrides element_name=相同>
                        front_template.attribute_overrides[element_name].insert(attribute_overrides.begin(), attribute_overrides.end());
                    }
                    xml_child = xml_child->getNextElementSibling();
                }
            }

            // 应用覆盖属性
            auto document = xml_root.getOwnerDocument();
            auto xml_element_iter = document->createNodeIterator(&xml_root, DOMNodeFilter::ShowTypeMasks::SHOW_ELEMENT,nullptr, false);
            auto xml_element = xml_element_iter->nextNode();
            const auto xml_element_attr_name = XMLString::transcode("name");
            while (xml_element != nullptr && front_template.attribute_overrides.size() > 0)
            {
                auto xml_attr = xml_element->getAttributes()->getNamedItem(xml_element_attr_name);
                if(xml_attr != nullptr)
                {
                    std::string element_name = XMLString::transcode(xml_attr->getNodeValue());
                    auto find = front_template.attribute_overrides.find(element_name);
                    if(find != front_template.attribute_overrides.end())
                    {
                        const auto& overrides = find->second;
                        for (const auto& attribute : overrides)
                        {
                            auto new_xml_attr = document->createAttribute(XMLString::transcode(attribute.name.data()));
                            new_xml_attr->setValue(XMLString::transcode(attribute.value.data()));
                            xml_element->getAttributes()->setNamedItem(new_xml_attr);
                        }
                    }
                }
                xml_element = xml_element_iter->nextNode();
            }
            xml_element_iter->release();
        }

        auto xml_element = xml_root.getFirstElementChild();
        while (xml_element != nullptr)
        {
            if(auto new_element = ParseElement(*xml_element, context))
                new_template->PushChild(new_element);

            if(context.is_error) break;
            xml_element = xml_element->getNextElementSibling();
        }
        context.stack_template.pop_front();

        if(context.is_error)
        {
            return nullptr;
        }
        return new_template;
    }

    VisualElement *XmlAsset::ParseElement(DOMElement &xml_element, CreationContext &context)
    {
        std::string local_name = XMLString::transcode(xml_element.getLocalName());
        std::string namespace_uri;
        std::string xml_qualified_name;

        auto uri = xml_element.getNamespaceURI();
        if(uri == nullptr)
        {
            namespace_uri = "";
            xml_qualified_name = local_name;
        }
        else
        {
            namespace_uri = XMLString::transcode(uri);
            xml_qualified_name = namespace_uri + "." + local_name;
        }

        IXmlFactory* factory = XmlFactoryRegistry::FindFactory(xml_qualified_name);
        if(factory == nullptr)
        {
            context.is_error = true;
            std::cerr << "找不到对应的元素类型 xml_qualified_name: " << xml_qualified_name << std::endl;
            return nullptr;
        }

        auto new_element = factory->Create(xml_element, context);
        if(new_element != nullptr)
        {
            context.stack.push_front(new_element);
            auto child = xml_element.getFirstElementChild();
            while (child != nullptr)
            {
                auto new_child_element = ParseElement(*child, context);
                if(new_child_element != nullptr && new_child_element->GetHierachyParent() == nullptr)
                    new_element->PushChild(new_child_element);

                if(context.is_error) break;
                child = child->getNextElementSibling();
            }
            context.stack.pop_front();
        }
        return new_element;
    }
}