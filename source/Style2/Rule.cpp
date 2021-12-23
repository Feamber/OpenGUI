
#include "OpenGUI/Style2/Rule.h"
#include "OpenGUI/Core/olog.h"
#include "OpenGUI/Managers/RenderTextureManager.h"
#include "OpenGUI/Style2/generated/background.h"
#include "OpenGUI/Context.h"


void OGUI::StyleSheet::Initialize()
{
    int i=0;
	for (auto& complexSel : styleSelectors)
	{
		complexSel.priority = i;
		auto& lastSel = complexSel.selectors.back();
		SelectorMap* mapPtr = nullptr;
		std::string_view key;
		if (lastSel.parts.size() == 0)
		{
			key = "*";
			mapPtr = &typeSelectors;
		}
		else
		{
			key = lastSel.parts.back().value;
			switch (lastSel.parts.back().type)
			{
				case StyleSelector::Class:
					mapPtr = &classSelectors;
					break;
				case StyleSelector::Name:
					mapPtr = &nameSelectors;
					break;
				case StyleSelector::Type:
				case StyleSelector::Wildcard:
					if (lastSel.parts.back().value.empty())
						key = "*";
					mapPtr = &typeSelectors;
					break;
				default:
					OUNREACHABLE
					break;
			}
		}
		
		mapPtr->insert(std::make_pair(key, i));
        ++i;
	}
    i=0;
	for (auto& keyframes : styleKeyframes)
		namedKeyframes.emplace(keyframes.name, i++);

	i=0;	
	for (auto& font : styleFonts)
		namedStyleFamilies.emplace(font.fontFamily, i++);\
	
	auto&& ctx = Context::Get();
	for(auto& rule : styleRules)
		for(auto& prop : rule.properties)
		{
			if(prop.id == StyleBackground::Ids::backgroundImage && !prop.keyword)
				preloaded.push_back(ctx.textureManager->RequireFromFileSystem(storage.Get<std::string>(prop.value)));
		}
}