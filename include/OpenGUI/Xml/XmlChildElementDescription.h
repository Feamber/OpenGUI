#pragma once
#include <string>
#include "../nameof/nameof.hpp"

namespace OGUI
{
    template<class T>
    std::string GetTypeNamespace()
    {
        constexpr auto name = NAMEOF_TYPE(T);
        std::string name2 = { name.begin(), name.end() };
        if(name2.find("::") == std::string::npos) return "";

        int star_index = 0;
        if(name2.find(' ') != std::string::npos) star_index = name2.find(' ') + 1;
        int end_index = name2.rfind("::");

        name2 = name2.substr(star_index, end_index - star_index);

        const std::string from("::");
        const std::string to(".");
        size_t start_pos = 0;
        while((start_pos = name2.find(from, start_pos)) != std::string::npos) {
            name2.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }

        return name2;
    }

	struct XmlChildElementDescription
	{
		std::string elementName;
		std::string elementNamespace;

        XmlChildElementDescription(const std::string &elementName, const std::string &elementNamespace) :
            elementName(elementName),
            elementNamespace(elementNamespace)
        {
        }

        template<class T = VisualElement>
		static XmlChildElementDescription Build()
		{
            return XmlChildElementDescription(NAMEOF_SHORT_TYPE(T), GetTypeNamespace<T>())
		}
	};
}