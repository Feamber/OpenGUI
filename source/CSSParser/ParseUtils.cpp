#include "ParseUtils.hpp"
#include "OpenGUI/Core/Utilities/any_move.hpp"
#include "OpenGUI/Core/ColorSpace.h"
#include "peglib.h"
#include <regex>
#include "OpenGUI/Core/Utilities/ipair.hpp"

namespace OGUI
{
    void sort(property_list_t& list)
	{
		struct prop
		{
			int order;
			std::string_view name;
			std::string_view value;
		};
		std::vector<prop> ordered;
		ordered.reserve(list.size());
		for (auto& pair : list)
			ordered.push_back({PropertyNameToOrder(pair.first), pair.first, pair.second});
		auto firstAnim = std::stable_partition(ordered.begin(), ordered.end(), [](const prop& p) { return p.order < (int)StylePropertyId::NumStyle; });
		std::sort(ordered.begin(), firstAnim, [](const prop& p1, const prop& p2) { return p1.order < p2.order; });
		for (auto&& [i, p] : ipair(ordered))
			list[i] = {p.name, p.value};
	}

    bool FromString(std::string_view str, std::string& value)
	{
		value = {str.begin(), str.end()};
		return true;
	}

	bool FromString(std::string_view str, float& value)
	{
		try
		{
			value = std::stof(std::string{str.begin(), str.end()});
			return true;
		}
		catch (std::exception)
		{
			return false;
		}
	}

	bool FromString(std::string_view str, int& value)
	{
		try
		{
			value = std::stoi(std::string{str.begin(), str.end()});
			return true;
		}
		catch (std::exception)
		{
			return false;
		}
	}

    bool FromString(std::string_view str, StyleKeyword& value)
	{
		if (str == "inherit")
			value = StyleKeyword::Inherit;
		else if (str == "unset")
			value = StyleKeyword::Unset;
		else if (str == "initial")
			value = StyleKeyword::Initial;
		else return false;
		return true;
	}

	bool FromString(std::string_view str, YGValue& value)
	{
		if (str == "auto")
		{
			value = YGValueAuto;
			return true;
		}
		if (str == "none")
		{
			value = YGValueUndefined;
			return true;
		}
		std::regex zero("0(\\.0+)?");
		if (std::regex_match(str.begin(), str.end(), zero))
		{
			value = YGValueZero;
			return true;
		}
		float comp;
		std::string numstr;
		YGUnit unit;
		if (ends_with(str, "%"))
		{
			numstr = str.substr(0, str.length() - 1);
			unit = YGUnitPercent;
		}
		else if (ends_with(str, "px"))
		{
			numstr = str.substr(0, str.length() - 2);
			unit = YGUnitPoint;
		}
		else
			return false;

		if (FromString(numstr, comp))
		{
			value = YGValue{comp, unit};
			return true;
		}
		else
			return false;
	}

	bool FromString(std::string_view str, AnimTimingFunction& TimingFunction)
	{
		//TODO
		TimingFunction = AnimLinearFuncion;
		return true;
	}

	bool FromColorName(std::string_view str, Color4f& value)
	{
		using namespace std::string_view_literals;
		static const std::unordered_map<std::string_view, Color4f> NamedColor({
			// pre-defined
			{"aliceblue"sv,Color4f(240.f / 255,248.f / 255,255.f / 255, 1.f)},
			{"antiquewhite"sv,Color4f(250.f / 255,235.f / 255,215.f / 255, 1.f)},
			{"aqua"sv,Color4f(0.f / 255,255.f / 255,255.f / 255, 1.f)},
			{"aquamarine"sv,Color4f(127.f / 255,255.f / 255,212.f / 255, 1.f)},
			{"azure"sv,Color4f(240.f / 255,255.f / 255,255.f / 255, 1.f)},
			{"beige"sv,Color4f(245.f / 255,245.f / 255,220.f / 255, 1.f)},
			{"bisque"sv,Color4f(255.f / 255,228.f / 255,196.f / 255, 1.f)},
			{"black"sv,Color4f(0.f / 255,0.f / 255,0.f / 255, 1.f)},
			{"blanchedalmond"sv,Color4f(255.f / 255,235.f / 255,205.f / 255, 1.f)},
			{"blue"sv,Color4f(0.f / 255,0.f / 255,255.f / 255, 1.f)},
			{"blueviolet"sv,Color4f(138.f / 255,43.f / 255,226.f / 255, 1.f)},
			{"brown"sv,Color4f(165.f / 255,42.f / 255,42.f / 255, 1.f)},
			{"burlywood"sv,Color4f(222.f / 255,184.f / 255,135.f / 255, 1.f)},
			{"cadetblue"sv,Color4f(95.f / 255,158.f / 255,160.f / 255, 1.f)},
			{"chartreuse"sv,Color4f(127.f / 255,255.f / 255,0.f / 255, 1.f)},
			{"chocolate"sv,Color4f(210.f / 255,105.f / 255,30.f / 255, 1.f)},
			{"coral"sv,Color4f(255.f / 255,127.f / 255,80.f / 255, 1.f)},
			{"cornflowerblue"sv,Color4f(100.f / 255,149.f / 255,237.f / 255, 1.f)},
			{"cornsilk"sv,Color4f(255.f / 255,248.f / 255,220.f / 255, 1.f)},
			{"crimson"sv,Color4f(220.f / 255,20.f / 255,60.f / 255, 1.f)},
			{"cyan"sv,Color4f(0.f / 255,255.f / 255,255.f / 255, 1.f)},
			{"darkblue"sv,Color4f(0.f / 255,0.f / 255,139.f / 255, 1.f)},
			{"darkcyan"sv,Color4f(0.f / 255,139.f / 255,139.f / 255, 1.f)},
			{"darkgoldenrod"sv,Color4f(184.f / 255,134.f / 255,11.f / 255, 1.f)},
			{"darkgray"sv,Color4f(169.f / 255,169.f / 255,169.f / 255, 1.f)},
			{"darkgrey"sv,Color4f(169.f / 255,169.f / 255,169.f / 255, 1.f)},
			{"darkgreen"sv,Color4f(0.f / 255,100.f / 255,0.f / 255, 1.f)},
			{"darkkhaki"sv,Color4f(189.f / 255,183.f / 255,107.f / 255, 1.f)},
			{"darkmagenta"sv,Color4f(139.f / 255,0.f / 255,139.f / 255, 1.f)},
			{"darkolivegreen"sv,Color4f(85.f / 255,107.f / 255,47.f / 255, 1.f)},
			{"darkorange"sv,Color4f(255.f / 255,140.f / 255,0.f / 255, 1.f)},
			{"darkorchid"sv,Color4f(153.f / 255,50.f / 255,204.f / 255, 1.f)},
			{"darkred"sv,Color4f(139.f / 255,0.f / 255,0.f / 255, 1.f)},
			{"darksalmon"sv,Color4f(233.f / 255,150.f / 255,122.f / 255, 1.f)},
			{"darkseagreen"sv,Color4f(143.f / 255,188.f / 255,143.f / 255, 1.f)},
			{"darkslateblue"sv,Color4f(72.f / 255,61.f / 255,139.f / 255, 1.f)},
			{"darkslategray"sv,Color4f(47.f / 255,79.f / 255,79.f / 255, 1.f)},
			{"darkslategrey"sv,Color4f(47.f / 255,79.f / 255,79.f / 255, 1.f)},
			{"darkturquoise"sv,Color4f(0.f / 255,206.f / 255,209.f / 255, 1.f)},
			{"darkviolet"sv,Color4f(148.f / 255,0.f / 255,211.f / 255, 1.f)},
			{"deeppink"sv,Color4f(255.f / 255,20.f / 255,147.f / 255, 1.f)},
			{"deepskyblue"sv,Color4f(0.f / 255,191.f / 255,255.f / 255, 1.f)},
			{"dimgray"sv,Color4f(105.f / 255,105.f / 255,105.f / 255, 1.f)},
			{"dimgrey"sv,Color4f(105.f / 255,105.f / 255,105.f / 255, 1.f)},
			{"dodgerblue"sv,Color4f(30.f / 255,144.f / 255,255.f / 255, 1.f)},
			{"firebrick"sv,Color4f(178.f / 255,34.f / 255,34.f / 255, 1.f)},
			{"floralwhite"sv,Color4f(255.f / 255,250.f / 255,240.f / 255, 1.f)},
			{"forestgreen"sv,Color4f(34.f / 255,139.f / 255,34.f / 255, 1.f)},
			{"fuchsia"sv,Color4f(255.f / 255,0.f / 255,255.f / 255, 1.f)},
			{"gainsboro"sv,Color4f(220.f / 255,220.f / 255,220.f / 255, 1.f)},
			{"ghostwhite"sv,Color4f(248.f / 255,248.f / 255,255.f / 255, 1.f)},
			{"gold"sv,Color4f(255.f / 255,215.f / 255,0.f / 255, 1.f)},
			{"goldenrod"sv,Color4f(218.f / 255,165.f / 255,32.f / 255, 1.f)},
			{"gray"sv,Color4f(128.f / 255,128.f / 255,128.f / 255, 1.f)},
			{"grey"sv,Color4f(128.f / 255,128.f / 255,128.f / 255, 1.f)},
			{"green"sv,Color4f(0.f / 255,128.f / 255,0.f / 255, 1.f)},
			{"greenyellow"sv,Color4f(173.f / 255,255.f / 255,47.f / 255, 1.f)},
			{"honeydew"sv,Color4f(240.f / 255,255.f / 255,240.f / 255, 1.f)},
			{"hotpink"sv,Color4f(255.f / 255,105.f / 255,180.f / 255, 1.f)},
			{"indianred"sv,Color4f(205.f / 255,92.f / 255,92.f / 255, 1.f)},
			{"indigo"sv,Color4f(75.f / 255,0.f / 255,130.f / 255, 1.f)},
			{"ivory"sv,Color4f(255.f / 255,255.f / 255,240.f / 255, 1.f)},
			{"khaki"sv,Color4f(240.f / 255,230.f / 255,140.f / 255, 1.f)},
			{"lavender"sv,Color4f(230.f / 255,230.f / 255,250.f / 255, 1.f)},
			{"lavenderblush"sv,Color4f(255.f / 255,240.f / 255,245.f / 255, 1.f)},
			{"lawngreen"sv,Color4f(124.f / 255,252.f / 255,0.f / 255, 1.f)},
			{"lemonchiffon"sv,Color4f(255.f / 255,250.f / 255,205.f / 255, 1.f)},
			{"lightblue"sv,Color4f(173.f / 255,216.f / 255,230.f / 255, 1.f)},
			{"lightcoral"sv,Color4f(240.f / 255,128.f / 255,128.f / 255, 1.f)},
			{"lightcyan"sv,Color4f(224.f / 255,255.f / 255,255.f / 255, 1.f)},
			{"lightgoldenrodyellow"sv,Color4f(250.f / 255,250.f / 255,210.f / 255, 1.f)},
			{"lightgray"sv,Color4f(211.f / 255,211.f / 255,211.f / 255, 1.f)},
			{"lightgrey"sv,Color4f(211.f / 255,211.f / 255,211.f / 255, 1.f)},
			{"lightgreen"sv,Color4f(144.f / 255,238.f / 255,144.f / 255, 1.f)},
			{"lightpink"sv,Color4f(255.f / 255,182.f / 255,193.f / 255, 1.f)},
			{"lightsalmon"sv,Color4f(255.f / 255,160.f / 255,122.f / 255, 1.f)},
			{"lightseagreen"sv,Color4f(32.f / 255,178.f / 255,170.f / 255, 1.f)},
			{"lightskyblue"sv,Color4f(135.f / 255,206.f / 255,250.f / 255, 1.f)},
			{"lightslategray"sv,Color4f(119.f / 255,136.f / 255,153.f / 255, 1.f)},
			{"lightslategrey"sv,Color4f(119.f / 255,136.f / 255,153.f / 255, 1.f)},
			{"lightsteelblue"sv,Color4f(176.f / 255,196.f / 255,222.f / 255, 1.f)},
			{"lightyellow"sv,Color4f(255.f / 255,255.f / 255,224.f / 255, 1.f)},
			{"lime"sv,Color4f(0.f / 255,255.f / 255,0.f / 255, 1.f)},
			{"limegreen"sv,Color4f(50.f / 255,205.f / 255,50.f / 255, 1.f)},
			{"linen"sv,Color4f(250.f / 255,240.f / 255,230.f / 255, 1.f)},
			{"magenta"sv,Color4f(255.f / 255,0.f / 255,255.f / 255, 1.f)},
			{"maroon"sv,Color4f(128.f / 255,0.f / 255,0.f / 255, 1.f)},
			{"mediumaquamarine"sv,Color4f(102.f / 255,205.f / 255,170.f / 255, 1.f)},
			{"mediumblue"sv,Color4f(0.f / 255,0.f / 255,205.f / 255, 1.f)},
			{"mediumorchid"sv,Color4f(186.f / 255,85.f / 255,211.f / 255, 1.f)},
			{"mediumpurple"sv,Color4f(147.f / 255,112.f / 255,219.f / 255, 1.f)},
			{"mediumseagreen"sv,Color4f(60.f / 255,179.f / 255,113.f / 255, 1.f)},
			{"mediumslateblue"sv,Color4f(123.f / 255,104.f / 255,238.f / 255, 1.f)},
			{"mediumspringgreen"sv,Color4f(0.f / 255,250.f / 255,154.f / 255, 1.f)},
			{"mediumturquoise"sv,Color4f(72.f / 255,209.f / 255,204.f / 255, 1.f)},
			{"mediumvioletred"sv,Color4f(199.f / 255,21.f / 255,133.f / 255, 1.f)},
			{"midnightblue"sv,Color4f(25.f / 255,25.f / 255,112.f / 255, 1.f)},
			{"mintcream"sv,Color4f(245.f / 255,255.f / 255,250.f / 255, 1.f)},
			{"mistyrose"sv,Color4f(255.f / 255,228.f / 255,225.f / 255, 1.f)},
			{"moccasin"sv,Color4f(255.f / 255,228.f / 255,181.f / 255, 1.f)},
			{"navajowhite"sv,Color4f(255.f / 255,222.f / 255,173.f / 255, 1.f)},
			{"navy"sv,Color4f(0.f / 255,0.f / 255,128.f / 255, 1.f)},
			{"oldlace"sv,Color4f(253.f / 255,245.f / 255,230.f / 255, 1.f)},
			{"olive"sv,Color4f(128.f / 255,128.f / 255,0.f / 255, 1.f)},
			{"olivedrab"sv,Color4f(107.f / 255,142.f / 255,35.f / 255, 1.f)},
			{"orange"sv,Color4f(255.f / 255,165.f / 255,0.f / 255, 1.f)},
			{"orangered"sv,Color4f(255.f / 255,69.f / 255,0.f / 255, 1.f)},
			{"orchid"sv,Color4f(218.f / 255,112.f / 255,214.f / 255, 1.f)},
			{"palegoldenrod"sv,Color4f(238.f / 255,232.f / 255,170.f / 255, 1.f)},
			{"palegreen"sv,Color4f(152.f / 255,251.f / 255,152.f / 255, 1.f)},
			{"paleturquoise"sv,Color4f(175.f / 255,238.f / 255,238.f / 255, 1.f)},
			{"palevioletred"sv,Color4f(219.f / 255,112.f / 255,147.f / 255, 1.f)},
			{"papayawhip"sv,Color4f(255.f / 255,239.f / 255,213.f / 255, 1.f)},
			{"peachpuff"sv,Color4f(255.f / 255,218.f / 255,185.f / 255, 1.f)},
			{"peru"sv,Color4f(205.f / 255,133.f / 255,63.f / 255, 1.f)},
			{"pink"sv,Color4f(255.f / 255,192.f / 255,203.f / 255, 1.f)},
			{"plum"sv,Color4f(221.f / 255,160.f / 255,221.f / 255, 1.f)},
			{"powderblue"sv,Color4f(176.f / 255,224.f / 255,230.f / 255, 1.f)},
			{"purple"sv,Color4f(128.f / 255,0.f / 255,128.f / 255, 1.f)},
			{"rebeccapurple"sv,Color4f(102.f / 255,51.f / 255,153.f / 255, 1.f)},
			{"red"sv,Color4f(255.f / 255,0.f / 255,0.f / 255, 1.f)},
			{"rosybrown"sv,Color4f(188.f / 255,143.f / 255,143.f / 255, 1.f)},
			{"royalblue"sv,Color4f(65.f / 255,105.f / 255,225.f / 255, 1.f)},
			{"saddlebrown"sv,Color4f(139.f / 255,69.f / 255,19.f / 255, 1.f)},
			{"salmon"sv,Color4f(250.f / 255,128.f / 255,114.f / 255, 1.f)},
			{"sandybrown"sv,Color4f(244.f / 255,164.f / 255,96.f / 255, 1.f)},
			{"seagreen"sv,Color4f(46.f / 255,139.f / 255,87.f / 255, 1.f)},
			{"seashell"sv,Color4f(255.f / 255,245.f / 255,238.f / 255, 1.f)},
			{"sienna"sv,Color4f(160.f / 255,82.f / 255,45.f / 255, 1.f)},
			{"silver"sv,Color4f(192.f / 255,192.f / 255,192.f / 255, 1.f)},
			{"skyblue"sv,Color4f(135.f / 255,206.f / 255,235.f / 255, 1.f)},
			{"slateblue"sv,Color4f(106.f / 255,90.f / 255,205.f / 255, 1.f)},
			{"slategray"sv,Color4f(112.f / 255,128.f / 255,144.f / 255, 1.f)},
			{"slategrey"sv,Color4f(112.f / 255,128.f / 255,144.f / 255, 1.f)},
			{"snow"sv,Color4f(255.f / 255,250.f / 255,250.f / 255, 1.f)},
			{"springgreen"sv,Color4f(0.f / 255,255.f / 255,127.f / 255, 1.f)},
			{"steelblue"sv,Color4f(70.f / 255,130.f / 255,180.f / 255, 1.f)},
			{"tan"sv,Color4f(210.f / 255,180.f / 255,140.f / 255, 1.f)},
			{"teal"sv,Color4f(0.f / 255,128.f / 255,128.f / 255, 1.f)},
			{"thistle"sv,Color4f(216.f / 255,191.f / 255,216.f / 255, 1.f)},
			{"tomato"sv,Color4f(255.f / 255,99.f / 255,71.f / 255, 1.f)},
			{"turquoise"sv,Color4f(64.f / 255,224.f / 255,208.f / 255, 1.f)},
			{"violet"sv,Color4f(238.f / 255,130.f / 255,238.f / 255, 1.f)},
			{"wheat"sv,Color4f(245.f / 255,222.f / 255,179.f / 255, 1.f)},
			{"white"sv,Color4f(255.f / 255,255.f / 255,255.f / 255, 1.f)},
			{"whitesmoke"sv,Color4f(245.f / 255,245.f / 255,245.f / 255, 1.f)},
			{"yellow"sv,Color4f(255.f / 255,255.f / 255,0.f / 255, 1.f)},
			{"yellowgreen"sv,Color4f(154.f / 255,205.f / 255,50.f / 255, 1.f)},
			// special
			{ "transparent"sv, Color4f(0.f, 0.f, 0.f, 0.f) }
			// customize (if required)
			});

		std::string lower(str);
		std::transform(str.cbegin(), str.cend(), lower.begin(),
			[](unsigned char c) { return std::tolower(c); });
		auto it = NamedColor.find(lower);
		if (it != NamedColor.cend())
		{
			value = it->second;
			return true;
		}
		return false;
	}

	float FromAngle(std::string_view str)
	{
		using namespace std;
		float value;
		int angleType;
		string_view anglestr;
		if (ends_with(str, "deg"))
		{
			anglestr = str.substr(0, str.length() - 3);
			angleType = 0;
		}
		else if (ends_with(str, "rad"))
		{
			anglestr = str.substr(0, str.length() - 3);
			angleType = 1;
		}
		else if (ends_with(str, "grad"))
		{
			anglestr = str.substr(0, str.length() - 4);
			angleType = 2;
		}
		else if (ends_with(str, "turn"))
		{
			anglestr = str.substr(0, str.length() - 4);
			angleType = 3;
		}
		else if (ends_with(str, "%"))
		{
			throw peg::parse_error("hue can not be percent value.");
		}
		else
		{
			anglestr = str.substr(0, str.length() - 3);
			angleType = 0;
		}
		if (!FromString(anglestr, value))
			throw peg::parse_error("invalid number.");
		if (angleType == 1)
			value = value / math::PI;// rad2deg(value);
		else if (angleType == 2)
			value = value * 0.9;// grad2deg(value);
		else if (angleType == 3)
			value = value * 360;// turn2deg(value);
		return value;
	}

	float FromTranslationComponent(std::string_view str)
	{
		if (ends_with(str, "%"))
			throw peg::parse_error("percentage in translation is not supported.");
		if (ends_with(str, "px"))
			str = str.substr(0, str.length() - 2);
		std::regex zero("0(\\.0+)?");
		if (std::regex_match(str.begin(), str.end(), zero))
			return 0.f;
		float value;
		if (!FromString(str, value))
			throw peg::parse_error("invalid number.");
		return value;
	}

	bool FromTranslation(std::string_view str, Vector2f& t)
	{
		std::vector<std::string_view> tokens;
		std::split(str, tokens, ",");
		try
		{
			t.X = FromTranslationComponent(tokens[0]);
			t.Y = FromTranslationComponent(tokens[1]);
			return true;
		}
		catch (peg::parse_error error)
		{
			std::cerr << error.what() << "\n";
			return false;
		}
	}

	bool FromString(std::string_view str, Vector2f& t)
	{
		std::vector<std::string_view> tokens;
		std::split(str, tokens, ",");
		return FromString(tokens[0], t.X) && FromString(tokens[1], t.Y);
	}

	bool FromString(std::string_view str, Color4f& value)
	{
		static auto grammar = R"(
			Color	<- CALLS / CALL / HEX / IDENT
			CALL	<- FUNC '(' _ CNUM _ ',' _ CNUM _ ',' _ CNUM _ (',' _ CNUM _)? ')'
			CALLS	<- FUNC '(' _ CNUM __ CNUM __ CNUM _ ('/' _ CNUM _)? ')'
			FUNC	<- <'rgba' / 'rgb' / 'hsl' / 'hsla'>
			NUM		<- < ('+' / '-')? (([0-9]*"."([0-9]+ 'e')?[0-9]+) / ([0-9]+)) >
			CNUM	<- < NUM ('deg' / 'rad' / 'grad' / 'turn' / '%')? >
			HEX		<- '#' < [0-9a-fA-F] >
			IDENT	<- < [a-zA-Z] [a-zA-Z0-9-]* >
			~_		<- [ ]*
			~__		<- [ ]+
		)";

		using namespace peg;
		using namespace std;

		struct ParserInitializer
		{
			parser parser;
			bool ok;
			ParserInitializer()
			{
				parser.log = [](size_t line, size_t col, const string& msg)
				{
					cerr << line << ":" << col << ": " << msg << "\n";
				};
				ok = parser.load_grammar(grammar);
				if (ok)
				{
					auto token = [](SemanticValues& vs)
					{
						return vs.token(0);
					};

					parser["CNUM"] = token;

					parser["FUNC"] = [](SemanticValues& vs)
					{
						return vs.choice() / 2;
					};

					auto CALL = [](SemanticValues& vs)
					{
						size_t type = any_move<size_t>(vs[0]);
						Color4f color = Color4f::vector_one();
						auto ParseCNUM = [](string_view str, bool mustBePercent, bool normalize = true)
						{
							bool isPercent = false;
							auto valuestr = str;
							if (ends_with(str, "%"))
							{
								isPercent = true;
								valuestr = str.substr(0, str.length() - 1);
							}
							else if (mustBePercent)
								throw parse_error("invalid number, except percent.");
							float value;
							if (!FromString(valuestr, value))
								throw parse_error("invalid number.");
							if (isPercent && !mustBePercent)
								value = value / 100;
							else if (normalize)
								value = roundf(value) / 255;
							return value;
						};
						int size = vs.size();
						if (size < 4)
							throw parse_error("not enough parameter for color function.");
						if (size > 5)
							throw parse_error("too many parameter for color function.");
						auto first = any_move<string_view>(vs[1]);
						if (type == 1)
							color.X = FromAngle(first);
						else
							color.X = ParseCNUM(first, false);
						auto second = any_move<string_view>(vs[2]);
						color.Y = ParseCNUM(second, type == 1);
						auto third = any_move<string_view>(vs[3]);
						color.Z = ParseCNUM(third, type == 1);
						if (vs.size() == 5)
						{
							auto forth = any_move<string_view>(vs[4]);
							color.W = ParseCNUM(forth, false, false);
						}
						if (type == 1)
						{
							Color4f hsv = color;
							HSVtoRGB(hsv.X, hsv.Y, hsv.Z, color.X, color.Y, color.Z);
						}
						return color;
					};
					parser["CALLS"] = CALL;
					parser["CALL"] = CALL;

					parser["HEX"] = [](SemanticValues& vs)
					{
						Color4f color = Color4f::vector_one();
						auto str = vs.token();
						auto S2H = [](string_view hex)
						{
							string hexstr = {hex.begin(), hex.end()};
							try
							{
								return stoi(hexstr, nullptr, 16);
							}
							catch (std::exception)
							{
								throw parse_error("invalid hex value");
							}
						};
						auto length = str.length();
						string fullstr;
						if (length == 3 || length == 4)
						{
							fullstr.push_back(str[0]); fullstr.push_back(str[0]);
							fullstr.push_back(str[1]); fullstr.push_back(str[1]);
							fullstr.push_back(str[2]); fullstr.push_back(str[2]);
							if (length == 4)
							{
								fullstr.push_back(str[3]); fullstr.push_back(str[3]);
							}
							length = length * 2;
							str = fullstr;
						}
						if (length == 6 || length == 8)
						{
							color.X = S2H(str.substr(0, 2)) / 255.0;
							color.Y = S2H(str.substr(2, 2)) / 255.0;
							color.Z = S2H(str.substr(4, 2)) / 255.0;
							if (length == 8)
							{
								color.W = S2H(str.substr(6, 2)) / 255.0;
							}
						}
						else
						{
							throw parse_error("invalid hex value");
						}
						return color;
					};

					parser["IDENT"] = [](SemanticValues& vs)
					{
						Color4f res;
						if (!FromColorName(vs.token(), res))
							throw parse_error("invalid color name");
						return res;
					};
					auto forward = [](SemanticValues& vs)
					{
						return vs[0];
					};
					parser["Color"] = forward;
				}
			}
		};
		static ParserInitializer parserInitializer; //do once
		auto& parser = parserInitializer.parser;

		if (!parserInitializer.ok)
			return false;

		return parser.parse(str, value);
	}

	bool FromString(std::string_view str, YGValue& left, YGValue& top, YGValue& right, YGValue& bottom)
	{
		std::vector<std::string_view> tokens;
		std::split(str, tokens, ",");
		if (tokens.size() == 1)
		{
			if (!FromString(tokens[0], left))
				return false;
			top = right = bottom = left;
			return true;
		}
		else if (tokens.size() == 2)
		{
			if (!FromString(tokens[0], top))
				return false;
			bottom = top;
			if (!FromString(tokens[1], left))
				return false;
			right = left;
			return true;
		}
		else if (tokens.size() == 3)
		{
			if (!FromString(tokens[0], top))
				return false;
			if (!FromString(tokens[1], left))
				return false;
			right = left;
			if (!FromString(tokens[2], bottom))
				return false;
			return true;
		}
		else if (tokens.size() == 4)
		{
			if (!FromString(tokens[0], top))
				return false;
			if (!FromString(tokens[1], left))
				return false;
			if (!FromString(tokens[2], right))
				return false;
			if (!FromString(tokens[3], bottom))
				return false;
			return true;
		}
		else
			return false;
	}

	bool FromTime(std::string_view str, float& value)
	{
		std::string_view valuestr;
		float scale = 1;
		if (std::ends_with(str, "s"))
		{
			scale = 1;
			valuestr = str.substr(0, str.length() - 1);
		}
		else if (std::ends_with(str, "ms"))
		{
			scale = 1000;
			valuestr = str.substr(0, str.length() - 2);
		}
		else
			return false;
		if (!FromString(valuestr, value))
			return false;
		value /= scale;
		return true;
	}

	bool FromTime(std::string_view str, std::vector<float>& values)
	{
		std::vector<std::string_view> tokens;
		std::split(str, tokens, ", ");
		for (auto& token : tokens)
		{
			float value;
			if (!FromString(token, value))
				return false;
			values.push_back(std::move(value));
		}
		return true;
	}

	bool FromString(std::string_view str, YGPositionType& value)
	{
		if (str == "static")
		{
			value = YGPositionTypeStatic;
			return true;
		}
		else if (str == "relative")
		{
			value = YGPositionTypeRelative;
			return true;
		}
		else if (str == "absolute")
		{
			value = YGPositionTypeAbsolute;
			return true;
		}
		return false;
	}

	bool FromString(std::string_view str, YGOverflow& value)
	{
		if (str == "visible")
		{
			value = YGOverflowVisible;
			return true;
		}
		else if (str == "hidden")
		{
			value = YGOverflowHidden;
			return true;
		}
		else if (str == "scroll")
		{
			value = YGOverflowScroll;
			return true;
		}
		return false;
	}

	bool FromString(std::string_view str, YGAlign& value)
	{
		if (str == "auto")
		{
			value = YGAlignAuto;
			return true;
		}
		else if (str == "flex-start")
		{
			value = YGAlignFlexStart;
			return true;
		}
		else if (str == "center")
		{
			value = YGAlignCenter;
			return true;
		}
		else if (str == "flex-end")
		{
			value = YGAlignFlexEnd;
			return true;
		}
		else if (str == "stretch")
		{
			value = YGAlignStretch;
			return true;
		}
		else if (str == "baseline")
		{
			value = YGAlignBaseline;
			return true;
		}
		else if (str == "space-between")
		{
			value = YGAlignSpaceBetween;
			return true;
		}
		else if (str == "space-around")
		{
			value = YGAlignSpaceAround;
			return true;
		}
		return false;
	}

	bool FromString(std::string_view str, YGFlexDirection& value)
	{
		if (str == "column")
		{
			value = YGFlexDirectionColumn;
			return true;
		}
		else if (str == "column-reverse")
		{
			value = YGFlexDirectionColumnReverse;
			return true;
		}
		else if (str == "row")
		{
			value = YGFlexDirectionRow;
			return true;
		}
		else if (str == "row-reverse")
		{
			value = YGFlexDirectionRowReverse;
			return true;
		}
		return false;
	}

	bool FromString(std::string_view str, YGJustify& value)
	{
		if (str == "flex-start")
		{
			value = YGJustifyFlexStart;
			return true;
		}
		else if (str == "center")
		{
			value = YGJustifyCenter;
			return true;
		}
		else if (str == "flex-end")
		{
			value = YGJustifyFlexEnd;
			return true;
		}
		else if (str == "space-between")
		{
			value = YGJustifySpaceBetween;
			return true;
		}
		else if (str == "space-around")
		{
			value = YGJustifySpaceAround;
			return true;
		}
		else if (str == "space-evenly")
		{
			value = YGJustifySpaceEvenly;
			return true;
		}
		return false;
	}

	bool FromString(std::string_view str, YGWrap& value)
	{
		if (str == "no-wrap")
		{
			value = YGWrapNoWrap;
			return true;
		}
		else if (str == "wrap")
		{
			value = YGWrapWrap;
			return true;
		}
		else if (str == "wrap-reverse")
		{
			value = YGWrapWrapReverse;
			return true;
		}
		return false;
	}

	bool FromString(std::string_view str, YGDisplay& value)
	{
		if (str == "flex")
		{
			value = YGDisplayFlex;
			return true;
		}
		else if (str == "none")
		{
			value = YGDisplayNone;
			return true;
		}
		return false;
	}

	bool FromString(std::string_view str, EAnimDirection& value)
	{
		if (str == "normal")
		{
			value = EAnimDirection::Normal;
			return true;
		}
		else if (str == "reverse")
		{
			value = EAnimDirection::Reverse;
			return true;
		}
		else if (str == "alternate")
		{
			value = EAnimDirection::Alternate;
			return true;
		}
		else if (str == "alternate-reverse")
		{
			value = EAnimDirection::AlternateReverse;
			return true;
		}

		return false;
	}

	bool FromString(std::string_view str, EAnimResumeMode& value)
	{
		if (str == "reset")
		{
			value = EAnimResumeMode::Reset;
			return true;
		}
		else if (str == "resume")
		{
			value = EAnimResumeMode::Resume;
			return true;
		}

		return false;
	}

	bool FromString(std::string_view str, EAnimYieldMode& value)
	{
		if (str == "stop")
		{
			value = EAnimYieldMode::Stop;
			return true;
		}
		else if (str == "goback")
		{
			value = EAnimYieldMode::Goback;
			return true;
		}
		else if (str == "keep")
		{
			value = EAnimYieldMode::Keep;
			return true;
		}

		return false;
	}

	bool FromString(std::string_view str, EAnimFillMode& value)
	{
		if (str == "forwards")
		{
			value = EAnimFillMode::Forwards;
			return true;
		}
		else if (str == "backwards")
		{
			value = EAnimFillMode::Backwards;
			return true;
		}
		else if (str == "none")
		{
			value = EAnimFillMode::None;
			return true;
		}
		else if (str == "both")
		{
			value = EAnimFillMode::Both;
			return true;
		}

		return false;
	}

	bool FromUrl(std::string_view str, std::string& value)
	{
		if (std::starts_with(str, "url(") && std::ends_with(str, ")"))
		{
			auto valuestr = str.substr(4, str.length() - 5);
			value = {valuestr.begin(), valuestr.end()};
			return true;
		}
		return false;
	}

	bool FromIterationCount(std::string_view str, float& value)
	{
		if (str == "infinite")
		{
			value = -1;
			return true;
		}
		return FromString(str, value);
	}
    
	bool FromTransform(std::string_view str, StyleSheetStorage& sheet, StyleRule& rule)
	{
		static auto grammar = R"(
			TransformList	<- Transform (w Transform)*
			Transform		<- Translate / TranslateX / TranslateY / Scale / ScaleX / ScaleY / Rotate
			Translate		<- 'translate' _ '(' _ LENGTH _ ',' _ LENGTH _ ')'
			TranslateX		<- 'translateX' _ '(' _ LENGTH _ ')'
			TranslateY		<- 'translateY' _ '(' _ LENGTH _ ')'
			Scale			<- 'scale' _ '(' _ NUM _ ',' _ NUM _ ')'
			ScaleX			<- 'scaleX' _ '(' _ NUM _ ')'
			ScaleY			<- 'scaleY' _ '(' _ NUM _ ')'
			Rotate			<- 'rotate' _ '(' _ ANGLE _ ')'
			NUM				<- ('+' / '-')? (([0-9]*"."([0-9]+ 'e')?[0-9]+) / ([0-9]+))
			LENGTH			<- < ( NUM ('px' / '%') ) > / '0'
			ANGLE			<- < NUM ('deg' / 'rad' / 'grad' / 'turn')? >
			~_				<- [ ]*
			~w				<- [ ]+
		)";


		using namespace peg;
		using namespace std;

		struct optionalTransform
		{
			std::optional<Vector2f> t;
			std::optional<float> r;
			std::optional<Vector2f> s;
		};
		using tt = optionalTransform;
		struct ParserInitializer
		{
			parser parser;
			bool ok;
			ParserInitializer()
			{
				parser.log = [](size_t line, size_t col, const string& msg)
				{
					cerr << line << ":" << col << ": " << msg << "\n";
				};
				ok = parser.load_grammar(grammar);
				if (ok)
				{


					parser["NUM"] = [](SemanticValues& vs)
					{
						float value = 0;
						if (!FromString(vs.token(), value))
							throw peg::parse_error("invalid number.");
						return value;
					};

					parser["LENGTH"] = [](SemanticValues& vs)
					{
						return FromTranslationComponent(vs.token());
					};

					parser["ANGLE"] = [](SemanticValues& vs)
					{
						return FromAngle(vs.token()) / 180 * math::PI;
					};
#define ARG(n) any_move<float>(vs[n])
					parser["Rotate"] = [](SemanticValues& vs) -> tt { return {{},						ARG(0),	{}}; };
					parser["ScaleX"] = [](SemanticValues& vs) -> tt { return {{},						{},		Vector2f{ARG(0), 1}}; };
					parser["ScaleY"] = [](SemanticValues& vs) -> tt { return {{},						{},		Vector2f{1, ARG(0)}}; };
					parser["Scale"] = [](SemanticValues& vs) -> tt { return {{},						{},		Vector2f{ARG(0), ARG(1)}}; };
					parser["TranslateX"] = [](SemanticValues& vs) -> tt { return {Vector2f{ARG(0), 0},		{},		{}}; };
					parser["TranslateY"] = [](SemanticValues& vs) -> tt { return {Vector2f{0, ARG(0)},		{},		{}}; };
					parser["Translate"] = [](SemanticValues& vs) -> tt { return {Vector2f{ARG(0), ARG(1)}, {},		{}}; };
#undef ARG
					parser["Transform"] = [](SemanticValues& vs) { return std::move(vs[0]); };
					static auto merge = [](auto& a, const auto& b)
					{
						if (!b.has_value()) return;
						if (a.has_value())
							a = a.value() + b.value();
						else a = b.value();
					};
					parser["TransformList"] = [](SemanticValues& vs)
					{
						tt finalM = any_move<tt>(vs[0]);
						for (int i = 1; i < vs.size(); ++i)
						{
							tt M = any_move<tt>(vs[i]);
							merge(finalM.t, M.t);
							merge(finalM.r, M.r);
							merge(finalM.s, M.s);
						}
						return finalM;
					};
				}
			}
		};
		static ParserInitializer parserInitializer; //do once
		auto& parser = parserInitializer.parser;

		if (!parserInitializer.ok)
			return false;
		tt value;
		if (!parser.parse(str, value))
			return false;
		if (value.t.has_value())
			rule.properties.push_back(StyleProperty{StylePropertyId::translation, sheet.Push<Vector2f>(value.t.value())});
		if (value.r.has_value())
			rule.properties.push_back(StyleProperty{StylePropertyId::rotation, sheet.Push<float>(value.r.value())});
		if (value.s.has_value())
			rule.properties.push_back(StyleProperty{StylePropertyId::scale, sheet.Push<Vector2f>(value.s.value())});
		return true;
	}
}