#include "ParseUtils.hpp"
#include "peglib.h"
#include "OpenGUI/Core/Utilities/any_move.hpp"
#include "OpenGUI/Core/Utilities/ipair.hpp"

namespace OGUI
{
	template<class T>
	std::enable_if_t<!std::is_enum_v<T>, VariantHandle>
		AddPropertyImpl(StyleSheetStorage& sheet, StylePropertyId id, const T& value)
	{
		return sheet.Push<T>(value);
	}

	template<class T>
	std::enable_if_t<std::is_enum_v<T>, VariantHandle>
		AddPropertyImpl(StyleSheetStorage& sheet, StylePropertyId id, const T& value)
	{
		return sheet.Push<int>((int)value);
	}
}


bool OGUI::ParseProperty(StyleSheetStorage& sheet, std::string_view name, std::string_view str, StyleRule& rule, const char*& errorMsg, ParseErrorType& errorType, int animIndex, int animEndIndex)
{
	StyleKeyword keyword = StyleKeyword::None;
	auto id = PropertyNameToId(name);
	//keywords
	FromString(str, keyword);

	//shorthands
#define SHORTHAND(mm, mm2, smm) \
	if (name == smm) \
	{ \
		props = {StylePropertyId::mm##Left##mm2, StylePropertyId::mm##Top##mm2, StylePropertyId::mm##Right##mm2, StylePropertyId::mm##Bottom##mm2}; \
		goto parse_edges; \
	}

	std::array<StylePropertyId, 4> props;
	SHORTHAND(margin, , "margin")
	else SHORTHAND(border, Width, "border-width")
	else SHORTHAND(padding, , "padding")
	else if (name == "border-radius")
	{
		props = {StylePropertyId::borderTopLeftRadius, StylePropertyId::borderTopRightRadius, StylePropertyId::borderBottomRightRadius, StylePropertyId::borderBottomLeftRadius};
		parse_edges:
		if(keyword != StyleKeyword::None) 
		{
			for (int i = 0; i < 4; ++i)
				rule.properties.push_back(StyleProperty{props[i], (int)keyword});
			return true; 
		} 
		YGValue values[4]; 
		if(!FromString(str, values[0], values[1], values[2], values[3]))
			goto fail;
		for (int i=0;i<4;++i)
			rule.properties.push_back(StyleProperty{props[i], sheet.Push(values[i])});
		return true; 
	}
#undef SHORTHAND

	if (name == "transform")
	{
		if (keyword != StyleKeyword::None)
		{
			rule.properties.push_back(StyleProperty{StylePropertyId::translation, (int)keyword});
			rule.properties.push_back(StyleProperty{StylePropertyId::rotation, (int)keyword});
			rule.properties.push_back(StyleProperty{StylePropertyId::scale, (int)keyword});
		}
		else
		{
			if(!FromTransform(str, sheet, rule))
				goto fail;
		}
		return true;
	}

	if ((int)id >= (int)StylePropertyId::_End)
	{
		errorMsg = "custom style property is not support yet!";
		errorType = ParseErrorType::InvalidProperty;
		return false;
	}
	if (animIndex < 0 && (int)id >= (int)StylePropertyId::NumStyle && id != StylePropertyId::animTimingFunction)
	{
		errorMsg = "canimation properties in keyframe blocks is not allowed!";
		errorType = ParseErrorType::InvalidProperty;
		return false;
	}

#define INTERNALPROP(idd) \
	if(id == StylePropertyId::idd) \
	{ \
		errorMsg = "this property is internal use only!"; \
		errorType = ParseErrorType::InvalidProperty; \
		return false; \
	} 

	//disable property like this
	//INTERNALPROP(color);

	if (keyword != StyleKeyword::None)
	{
		rule.properties.push_back(StyleProperty{id, (int)keyword});
		return true;
	}

#define PARSEANIMPROP(idd, type, ParseValue) \
	if (id == StylePropertyId::idd) \
	{ \
		std::vector<std::string_view> tokens; \
		std::split(str, tokens, ", "); \
		if(tokens.size() > animEndIndex - animIndex) \
			goto animFail; \
		for (auto&& [i, token] : ipair(tokens)) \
		{ \
			if(FromString(token, keyword)) \
				rule.properties.push_back(StyleProperty{id, (int)keyword, animIndex + i}); \
			else \
			{ \
				type value; \
				if (ParseValue(token, value)) \
					rule.properties.push_back({id, AddPropertyImpl(sheet, id, value), animIndex + i}); \
				else return false; \
			} \
		} \
		return true; \
	}

#define PARSEPROP(idd, type, ParseValue) \
	if(id == StylePropertyId::idd) \
	{ \
		type value; \
		if (ParseValue(str, value)) \
		{ \
			rule.properties.push_back({id, AddPropertyImpl(sheet, id, value)}); \
			return true; \
		} \
	}
	if (animIndex >= 0 && id >= StylePropertyId::NumStyle)
	{
		PARSEANIMPROP(animDuration, float, FromTime);
		PARSEANIMPROP(animDelay, float, FromTime);
		PARSEANIMPROP(animIterCount, float, FromIterationCount);
#define GEN(idd, type, ...) PARSEANIMPROP(idd, type, FromString);
		ANIMPROP(GEN);
#undef GEN
	}
	else
	{
		PARSEPROP(animTimingFunction, AnimTimingFunction, FromString);
	}

	PARSEPROP(backgroundImage, std::string, FromUrl);
	PARSEPROP(translation, Vector2f, FromTranslation);

#define	GEN(idd, type, ...) PARSEPROP(idd, type, FromString)
	STYLEPROP(GEN)
#undef  GEN
fail:
	errorMsg = "failed to parse style property value!"; 
	errorType = ParseErrorType::InvalidValue; 
	return false;
animFail:
	errorMsg = "animation property count doesnt match!";
	errorType = ParseErrorType::InvalidValue;
	return false;
}
