
#define DLL_IMPLEMENTATION
#include "peglib.h"
#include "OpenGUI/Style2/Transform.h"
#include "OpenGUI/Style2/Parse/Math.h"
#include "OpenGUI/Style2/Parse.h"
#include <string>
#include "OpenGUI/Core/Utilities/string_hash.hpp"
#include "OpenGUI/Core/Utilities/any_move.hpp"
#include "OpenGUI/Core/open_string.h"
#include "OpenGUI/Core/ColorSpace.h"
#include "OpenGUI/Style2/generated/position.h"
#include <regex>
using namespace ostr::literal;


bool OGUI::ParseValue(std::string_view str, float& value)
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
bool OGUI::ParseValue(std::string_view str, int& value)
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
bool OGUI::ParseColorName(std::string_view str, Color4f& value)
{
    std::string lower(str);
    switchstr(lower)
    {
        casestr("aliceblue") value = Color4f(240.f / 255,248.f / 255,255.f / 255, 1.f); return true;
        casestr("antiquewhite") value = Color4f(250.f / 255,235.f / 255,215.f / 255, 1.f); return true;
        casestr("aqua") value = Color4f(0.f / 255,255.f / 255,255.f / 255, 1.f); return true;
        casestr("aquamarine") value = Color4f(127.f / 255,255.f / 255,212.f / 255, 1.f); return true;
        casestr("azure") value = Color4f(240.f / 255,255.f / 255,255.f / 255, 1.f); return true;
        casestr("beige") value = Color4f(245.f / 255,245.f / 255,220.f / 255, 1.f); return true;
        casestr("bisque") value = Color4f(255.f / 255,228.f / 255,196.f / 255, 1.f); return true;
        casestr("black") value = Color4f(0.f / 255,0.f / 255,0.f / 255, 1.f); return true;
        casestr("blanchedalmond") value = Color4f(255.f / 255,235.f / 255,205.f / 255, 1.f); return true;
        casestr("blue") value = Color4f(0.f / 255,0.f / 255,255.f / 255, 1.f); return true;
        casestr("blueviolet") value = Color4f(138.f / 255,43.f / 255,226.f / 255, 1.f); return true;
        casestr("brown") value = Color4f(165.f / 255,42.f / 255,42.f / 255, 1.f); return true;
        casestr("burlywood") value = Color4f(222.f / 255,184.f / 255,135.f / 255, 1.f); return true;
        casestr("cadetblue") value = Color4f(95.f / 255,158.f / 255,160.f / 255, 1.f); return true;
        casestr("chartreuse") value = Color4f(127.f / 255,255.f / 255,0.f / 255, 1.f); return true;
        casestr("chocolate") value = Color4f(210.f / 255,105.f / 255,30.f / 255, 1.f); return true;
        casestr("coral") value = Color4f(255.f / 255,127.f / 255,80.f / 255, 1.f); return true;
        casestr("cornflowerblue") value = Color4f(100.f / 255,149.f / 255,237.f / 255, 1.f); return true;
        casestr("cornsilk") value = Color4f(255.f / 255,248.f / 255,220.f / 255, 1.f); return true;
        casestr("crimson") value = Color4f(220.f / 255,20.f / 255,60.f / 255, 1.f); return true;
        casestr("cyan") value = Color4f(0.f / 255,255.f / 255,255.f / 255, 1.f); return true;
        casestr("darkblue") value = Color4f(0.f / 255,0.f / 255,139.f / 255, 1.f); return true;
        casestr("darkcyan") value = Color4f(0.f / 255,139.f / 255,139.f / 255, 1.f); return true;
        casestr("darkgoldenrod") value = Color4f(184.f / 255,134.f / 255,11.f / 255, 1.f); return true;
        casestr("darkgray") value = Color4f(169.f / 255,169.f / 255,169.f / 255, 1.f); return true;
        casestr("darkgrey") value = Color4f(169.f / 255,169.f / 255,169.f / 255, 1.f); return true;
        casestr("darkgreen") value = Color4f(0.f / 255,100.f / 255,0.f / 255, 1.f); return true;
        casestr("darkkhaki") value = Color4f(189.f / 255,183.f / 255,107.f / 255, 1.f); return true;
        casestr("darkmagenta") value = Color4f(139.f / 255,0.f / 255,139.f / 255, 1.f); return true;
        casestr("darkolivegreen") value = Color4f(85.f / 255,107.f / 255,47.f / 255, 1.f); return true;
        casestr("darkorange") value = Color4f(255.f / 255,140.f / 255,0.f / 255, 1.f); return true;
        casestr("darkorchid") value = Color4f(153.f / 255,50.f / 255,204.f / 255, 1.f); return true;
        casestr("darkred") value = Color4f(139.f / 255,0.f / 255,0.f / 255, 1.f); return true;
        casestr("darksalmon") value = Color4f(233.f / 255,150.f / 255,122.f / 255, 1.f); return true;
        casestr("darkseagreen") value = Color4f(143.f / 255,188.f / 255,143.f / 255, 1.f); return true;
        casestr("darkslateblue") value = Color4f(72.f / 255,61.f / 255,139.f / 255, 1.f); return true;
        casestr("darkslategray") value = Color4f(47.f / 255,79.f / 255,79.f / 255, 1.f); return true;
        casestr("darkslategrey") value = Color4f(47.f / 255,79.f / 255,79.f / 255, 1.f); return true;
        casestr("darkturquoise") value = Color4f(0.f / 255,206.f / 255,209.f / 255, 1.f); return true;
        casestr("darkviolet") value = Color4f(148.f / 255,0.f / 255,211.f / 255, 1.f); return true;
        casestr("deeppink") value = Color4f(255.f / 255,20.f / 255,147.f / 255, 1.f); return true;
        casestr("deepskyblue") value = Color4f(0.f / 255,191.f / 255,255.f / 255, 1.f); return true;
        casestr("dimgray") value = Color4f(105.f / 255,105.f / 255,105.f / 255, 1.f); return true;
        casestr("dimgrey") value = Color4f(105.f / 255,105.f / 255,105.f / 255, 1.f); return true;
        casestr("dodgerblue") value = Color4f(30.f / 255,144.f / 255,255.f / 255, 1.f); return true;
        casestr("firebrick") value = Color4f(178.f / 255,34.f / 255,34.f / 255, 1.f); return true;
        casestr("floralwhite") value = Color4f(255.f / 255,250.f / 255,240.f / 255, 1.f); return true;
        casestr("forestgreen") value = Color4f(34.f / 255,139.f / 255,34.f / 255, 1.f); return true;
        casestr("fuchsia") value = Color4f(255.f / 255,0.f / 255,255.f / 255, 1.f); return true;
        casestr("gainsboro") value = Color4f(220.f / 255,220.f / 255,220.f / 255, 1.f); return true;
        casestr("ghostwhite") value = Color4f(248.f / 255,248.f / 255,255.f / 255, 1.f); return true;
        casestr("gold") value = Color4f(255.f / 255,215.f / 255,0.f / 255, 1.f); return true;
        casestr("goldenrod") value = Color4f(218.f / 255,165.f / 255,32.f / 255, 1.f); return true;
        casestr("gray") value = Color4f(128.f / 255,128.f / 255,128.f / 255, 1.f); return true;
        casestr("grey") value = Color4f(128.f / 255,128.f / 255,128.f / 255, 1.f); return true;
        casestr("green") value = Color4f(0.f / 255,128.f / 255,0.f / 255, 1.f); return true;
        casestr("greenyellow") value = Color4f(173.f / 255,255.f / 255,47.f / 255, 1.f); return true;
        casestr("honeydew") value = Color4f(240.f / 255,255.f / 255,240.f / 255, 1.f); return true;
        casestr("hotpink") value = Color4f(255.f / 255,105.f / 255,180.f / 255, 1.f); return true;
        casestr("indianred") value = Color4f(205.f / 255,92.f / 255,92.f / 255, 1.f); return true;
        casestr("indigo") value = Color4f(75.f / 255,0.f / 255,130.f / 255, 1.f); return true;
        casestr("ivory") value = Color4f(255.f / 255,255.f / 255,240.f / 255, 1.f); return true;
        casestr("khaki") value = Color4f(240.f / 255,230.f / 255,140.f / 255, 1.f); return true;
        casestr("lavender") value = Color4f(230.f / 255,230.f / 255,250.f / 255, 1.f); return true;
        casestr("lavenderblush") value = Color4f(255.f / 255,240.f / 255,245.f / 255, 1.f); return true;
        casestr("lawngreen") value = Color4f(124.f / 255,252.f / 255,0.f / 255, 1.f); return true;
        casestr("lemonchiffon") value = Color4f(255.f / 255,250.f / 255,205.f / 255, 1.f); return true;
        casestr("lightblue") value = Color4f(173.f / 255,216.f / 255,230.f / 255, 1.f); return true;
        casestr("lightcoral") value = Color4f(240.f / 255,128.f / 255,128.f / 255, 1.f); return true;
        casestr("lightcyan") value = Color4f(224.f / 255,255.f / 255,255.f / 255, 1.f); return true;
        casestr("lightgoldenrodyellow") value = Color4f(250.f / 255,250.f / 255,210.f / 255, 1.f); return true;
        casestr("lightgray") value = Color4f(211.f / 255,211.f / 255,211.f / 255, 1.f); return true;
        casestr("lightgrey") value = Color4f(211.f / 255,211.f / 255,211.f / 255, 1.f); return true;
        casestr("lightgreen") value = Color4f(144.f / 255,238.f / 255,144.f / 255, 1.f); return true;
        casestr("lightpink") value = Color4f(255.f / 255,182.f / 255,193.f / 255, 1.f); return true;
        casestr("lightsalmon") value = Color4f(255.f / 255,160.f / 255,122.f / 255, 1.f); return true;
        casestr("lightseagreen") value = Color4f(32.f / 255,178.f / 255,170.f / 255, 1.f); return true;
        casestr("lightskyblue") value = Color4f(135.f / 255,206.f / 255,250.f / 255, 1.f); return true;
        casestr("lightslategray") value = Color4f(119.f / 255,136.f / 255,153.f / 255, 1.f); return true;
        casestr("lightslategrey") value = Color4f(119.f / 255,136.f / 255,153.f / 255, 1.f); return true;
        casestr("lightsteelblue") value = Color4f(176.f / 255,196.f / 255,222.f / 255, 1.f); return true;
        casestr("lightyellow") value = Color4f(255.f / 255,255.f / 255,224.f / 255, 1.f); return true;
        casestr("lime") value = Color4f(0.f / 255,255.f / 255,0.f / 255, 1.f); return true;
        casestr("limegreen") value = Color4f(50.f / 255,205.f / 255,50.f / 255, 1.f); return true;
        casestr("linen") value = Color4f(250.f / 255,240.f / 255,230.f / 255, 1.f); return true;
        casestr("magenta") value = Color4f(255.f / 255,0.f / 255,255.f / 255, 1.f); return true;
        casestr("maroon") value = Color4f(128.f / 255,0.f / 255,0.f / 255, 1.f); return true;
        casestr("mediumaquamarine") value = Color4f(102.f / 255,205.f / 255,170.f / 255, 1.f); return true;
        casestr("mediumblue") value = Color4f(0.f / 255,0.f / 255,205.f / 255, 1.f); return true;
        casestr("mediumorchid") value = Color4f(186.f / 255,85.f / 255,211.f / 255, 1.f); return true;
        casestr("mediumpurple") value = Color4f(147.f / 255,112.f / 255,219.f / 255, 1.f); return true;
        casestr("mediumseagreen") value = Color4f(60.f / 255,179.f / 255,113.f / 255, 1.f); return true;
        casestr("mediumslateblue") value = Color4f(123.f / 255,104.f / 255,238.f / 255, 1.f); return true;
        casestr("mediumspringgreen") value = Color4f(0.f / 255,250.f / 255,154.f / 255, 1.f); return true;
        casestr("mediumturquoise") value = Color4f(72.f / 255,209.f / 255,204.f / 255, 1.f); return true;
        casestr("mediumvioletred") value = Color4f(199.f / 255,21.f / 255,133.f / 255, 1.f); return true;
        casestr("midnightblue") value = Color4f(25.f / 255,25.f / 255,112.f / 255, 1.f); return true;
        casestr("mintcream") value = Color4f(245.f / 255,255.f / 255,250.f / 255, 1.f); return true;
        casestr("mistyrose") value = Color4f(255.f / 255,228.f / 255,225.f / 255, 1.f); return true;
        casestr("moccasin") value = Color4f(255.f / 255,228.f / 255,181.f / 255, 1.f); return true;
        casestr("navajowhite") value = Color4f(255.f / 255,222.f / 255,173.f / 255, 1.f); return true;
        casestr("navy") value = Color4f(0.f / 255,0.f / 255,128.f / 255, 1.f); return true;
        casestr("oldlace") value = Color4f(253.f / 255,245.f / 255,230.f / 255, 1.f); return true;
        casestr("olive") value = Color4f(128.f / 255,128.f / 255,0.f / 255, 1.f); return true;
        casestr("olivedrab") value = Color4f(107.f / 255,142.f / 255,35.f / 255, 1.f); return true;
        casestr("orange") value = Color4f(255.f / 255,165.f / 255,0.f / 255, 1.f); return true;
        casestr("orangered") value = Color4f(255.f / 255,69.f / 255,0.f / 255, 1.f); return true;
        casestr("orchid") value = Color4f(218.f / 255,112.f / 255,214.f / 255, 1.f); return true;
        casestr("palegoldenrod") value = Color4f(238.f / 255,232.f / 255,170.f / 255, 1.f); return true;
        casestr("palegreen") value = Color4f(152.f / 255,251.f / 255,152.f / 255, 1.f); return true;
        casestr("paleturquoise") value = Color4f(175.f / 255,238.f / 255,238.f / 255, 1.f); return true;
        casestr("palevioletred") value = Color4f(219.f / 255,112.f / 255,147.f / 255, 1.f); return true;
        casestr("papayawhip") value = Color4f(255.f / 255,239.f / 255,213.f / 255, 1.f); return true;
        casestr("peachpuff") value = Color4f(255.f / 255,218.f / 255,185.f / 255, 1.f); return true;
        casestr("peru") value = Color4f(205.f / 255,133.f / 255,63.f / 255, 1.f); return true;
        casestr("pink") value = Color4f(255.f / 255,192.f / 255,203.f / 255, 1.f); return true;
        casestr("plum") value = Color4f(221.f / 255,160.f / 255,221.f / 255, 1.f); return true;
        casestr("powderblue") value = Color4f(176.f / 255,224.f / 255,230.f / 255, 1.f); return true;
        casestr("purple") value = Color4f(128.f / 255,0.f / 255,128.f / 255, 1.f); return true;
        casestr("rebeccapurple") value = Color4f(102.f / 255,51.f / 255,153.f / 255, 1.f); return true;
        casestr("red") value = Color4f(255.f / 255,0.f / 255,0.f / 255, 1.f); return true;
        casestr("rosybrown") value = Color4f(188.f / 255,143.f / 255,143.f / 255, 1.f); return true;
        casestr("royalblue") value = Color4f(65.f / 255,105.f / 255,225.f / 255, 1.f); return true;
        casestr("saddlebrown") value = Color4f(139.f / 255,69.f / 255,19.f / 255, 1.f); return true;
        casestr("salmon") value = Color4f(250.f / 255,128.f / 255,114.f / 255, 1.f); return true;
        casestr("sandybrown") value = Color4f(244.f / 255,164.f / 255,96.f / 255, 1.f); return true;
        casestr("seagreen") value = Color4f(46.f / 255,139.f / 255,87.f / 255, 1.f); return true;
        casestr("seashell") value = Color4f(255.f / 255,245.f / 255,238.f / 255, 1.f); return true;
        casestr("sienna") value = Color4f(160.f / 255,82.f / 255,45.f / 255, 1.f); return true;
        casestr("silver") value = Color4f(192.f / 255,192.f / 255,192.f / 255, 1.f); return true;
        casestr("skyblue") value = Color4f(135.f / 255,206.f / 255,235.f / 255, 1.f); return true;
        casestr("slateblue") value = Color4f(106.f / 255,90.f / 255,205.f / 255, 1.f); return true;
        casestr("slategray") value = Color4f(112.f / 255,128.f / 255,144.f / 255, 1.f); return true;
        casestr("slategrey") value = Color4f(112.f / 255,128.f / 255,144.f / 255, 1.f); return true;
        casestr("snow") value = Color4f(255.f / 255,250.f / 255,250.f / 255, 1.f); return true;
        casestr("springgreen") value = Color4f(0.f / 255,255.f / 255,127.f / 255, 1.f); return true;
        casestr("steelblue") value = Color4f(70.f / 255,130.f / 255,180.f / 255, 1.f); return true;
        casestr("tan") value = Color4f(210.f / 255,180.f / 255,140.f / 255, 1.f); return true;
        casestr("teal") value = Color4f(0.f / 255,128.f / 255,128.f / 255, 1.f); return true;
        casestr("thistle") value = Color4f(216.f / 255,191.f / 255,216.f / 255, 1.f); return true;
        casestr("tomato") value = Color4f(255.f / 255,99.f / 255,71.f / 255, 1.f); return true;
        casestr("turquoise") value = Color4f(64.f / 255,224.f / 255,208.f / 255, 1.f); return true;
        casestr("violet") value = Color4f(238.f / 255,130.f / 255,238.f / 255, 1.f); return true;
        casestr("wheat") value = Color4f(245.f / 255,222.f / 255,179.f / 255, 1.f); return true;
        casestr("white") value = Color4f(255.f / 255,255.f / 255,255.f / 255, 1.f); return true;
        casestr("whitesmoke") value = Color4f(245.f / 255,245.f / 255,245.f / 255, 1.f); return true;
        casestr("yellow") value = Color4f(255.f / 255,255.f / 255,0.f / 255, 1.f); return true;
        casestr("yellowgreen") value = Color4f(154.f / 255,205.f / 255,50.f / 255, 1.f); return true;
        // special
        casestr("transparent") value = Color4f(0.f, 0.f, 0.f, 0.f); return true;
        default:
            break;
    }
    return false;
}
bool ParseAngle(std::string_view str, float& value)
{
    using namespace std;
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
        return false; //hue can not be percent value
    }
    else
    {
        anglestr = str.substr(0, str.length() - 3);
        angleType = 0;
    }
    if (!OGUI::ParseValue(anglestr, value))
        return false;
    if (angleType == 1)
        value = value / OGUI::math::PI_;// rad2deg(value);
    else if (angleType == 2)
        value = value * 0.9;// grad2deg(value);
    else if (angleType == 3)
        value = value * 360;// turn2deg(value);
    return true;
}
bool ParseTranslationComponent(std::string_view str, float& value)
{
    if (ends_with(str, "%"))
        return false;
    if (ends_with(str, "px"))
        str = str.substr(0, str.length() - 2);
    return OGUI::ParseValue(str, value);
}
bool OGUI::ParseTranslate(std::string_view str, Vector2f& t)
{
    std::vector<std::string_view> tokens;
    std::split(str, tokens, ",");
    return ParseTranslationComponent(tokens[0], t.X) && ParseTranslationComponent(tokens[1], t.Y);
}
bool OGUI::ParseValue(std::string_view str, Vector2f& t)
{
    std::vector<std::string_view> tokens;
    std::split(str, tokens, ",");
    return ParseValue(tokens[0], t.X) && ParseValue(tokens[1], t.Y);
}
bool OGUI::ParseValue(std::string_view str, Color4f& value)
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
							if (!ParseValue(valuestr, value))
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
                        {
							 if(!ParseAngle(first, color.X))
                                throw parse_error("invalid hue angle.");
                        }
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
						if (!ParseColorName(vs.token(), res))
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

		parser.enable_packrat_parsing();
		return parser.parse(str, value);
}

bool OGUI::ParseValue(std::string_view str, std::vector<TransformFunction>& value)
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
			Skew			<- 'skew' _ '(' _ ANGLE _',' _ ANGLE _  ')'
			SkewX			<- 'skewX' _ '(' _ ANGLE _  ')'
			SkewY			<- 'skewY' _ '(' _ ANGLE _ ')'
			Matrix			<- 'matrix' _ '(' _ NUM _ ',' _ NUM _ ',' _ NUM _ ',' _ NUM _ ',' _ NUM _ ',' _ NUM _ ')'
			NUM				<- ('+' / '-')? (([0-9]*"."([0-9]+ 'e')?[0-9]+) / ([0-9]+))
			LENGTH			<- < ( NUM ('px' / '%') ) > / '0'
			ANGLE			<- < NUM ('deg' / 'rad' / 'grad' / 'turn')? >
			~_				<- [ ]*
			~w				<- [ ]+
		)";


		using namespace peg;
		using namespace std;

		struct ParserInitializer
		{
			parser parser;
			bool ok;
			ParserInitializer()
			{
				ok = parser.load_grammar(grammar);
				if (ok)
				{
					parser.log = [](size_t line, size_t col, const string& msg)
					{
						cerr << line << ":" << col << ": " << msg << "\n";
					};
					parser["NUM"] = [](SemanticValues& vs)
					{
						float value = 0;
						if (!ParseValue(vs.token(), value))
							throw peg::parse_error("invalid number.");
						return value;
					};

					parser["LENGTH"] = [](SemanticValues& vs)
					{
                        float value;
                        if(!ParseTranslationComponent(vs.token(), value))
                            throw parse_error("invalid length.");
						return value;
					};

					parser["ANGLE"] = [](SemanticValues& vs)
					{
                        float value;
                        if(!ParseAngle(vs.token(), value))
                            throw parse_error("invalid angle.");
						return value / 180 * math::PI_;
					};
#define ARG(n) any_move<float>(vs[n])
					parser["Rotate"] = [](SemanticValues& vs) { return TransformFunction::rotate(ARG(0)); };
					parser["ScaleX"] = [](SemanticValues& vs) { return TransformFunction::scale({ARG(0), 1}); };
					parser["ScaleY"] = [](SemanticValues& vs) { return TransformFunction::scale({1, ARG(0)}); };
					parser["Scale"] =   [](SemanticValues& vs)  { return TransformFunction::scale({ARG(0), ARG(1)}); };
					parser["TranslateX"] = [](SemanticValues& vs) { return TransformFunction::translate({ARG(0), 0}); };
					parser["TranslateY"] = [](SemanticValues& vs) { return TransformFunction::translate({0, ARG(0)}); };
					parser["Translate"] = [](SemanticValues& vs) { return TransformFunction::translate({ARG(0), ARG(1)}); };
					parser["SkewX"] = [](SemanticValues& vs) { return TransformFunction::skew({ARG(0), 0}); };
					parser["SkewY"] = [](SemanticValues& vs) { return TransformFunction::skew({0, ARG(0)}); };
					parser["Skew"] = [](SemanticValues& vs) { return TransformFunction::skew({ARG(0), ARG(1)}); };
					parser["Matrix"] = [](SemanticValues& vs) { return TransformFunction::matrix(ARG(0), ARG(1), ARG(2), ARG(3), ARG(4), ARG(5)); };
#undef ARG
					parser["Transform"] = [](SemanticValues& vs) { return std::move(vs[0]); };
					parser["TransformList"] = [](SemanticValues& vs)
					{
						std::vector<TransformFunction> list;
                        list.reserve(vs.size());
						for (int i = 0; i < vs.size(); ++i)
							list.push_back(any_move<TransformFunction>(vs[i]));
						return list;
					};
				}
			}
		};
		static ParserInitializer parserInitializer; //do once
		auto& parser = parserInitializer.parser;

		if (!parserInitializer.ok)
			return false;
		parser.enable_packrat_parsing();
		if (!parser.parse(str, value))
			return false;
		return true;
}