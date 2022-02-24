#include "OpenGUI/Core/Math/Vector.h"
#include "OpenGUI/Style2/peglib.h"
#include "OpenGUI/Core/Types.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Style2/Properties.h"
#include "yoga/Yoga.h"
#include "OpenGUI/Core/ColorSpace.h"
#include "OpenGUI/Style2/Transform.h"
#include <any>
#include <string_view>


namespace OGUI
{
    struct MathValueParser
    {
        static std::string_view GetGrammar();
        static void SetupAction(peg::parser& parser);
    };
    namespace CSSParser
    {
        void SetupMathValueParser()
        {
            RegisterGrammar(MathValueParser::GetGrammar(), &MathValueParser::SetupAction);
        }
    }
}

namespace OGUI
{
Color4f ParseColorName(std::string_view str)
{
    ostr::string lower = ostr::string::decode_from_utf8(str);
    switchstr(lower)
    {
        casestr("aliceblue") return Color4f(240.f / 255,248.f / 255,255.f / 255, 1.f); 
        casestr("antiquewhite") return Color4f(250.f / 255,235.f / 255,215.f / 255, 1.f); 
        casestr("aqua") return Color4f(0.f / 255,255.f / 255,255.f / 255, 1.f); 
        casestr("aquamarine") return Color4f(127.f / 255,255.f / 255,212.f / 255, 1.f); 
        casestr("azure") return Color4f(240.f / 255,255.f / 255,255.f / 255, 1.f); 
        casestr("beige") return Color4f(245.f / 255,245.f / 255,220.f / 255, 1.f); 
        casestr("bisque") return Color4f(255.f / 255,228.f / 255,196.f / 255, 1.f); 
        casestr("black") return Color4f(0.f / 255,0.f / 255,0.f / 255, 1.f); 
        casestr("blanchedalmond") return Color4f(255.f / 255,235.f / 255,205.f / 255, 1.f); 
        casestr("blue") return Color4f(0.f / 255,0.f / 255,255.f / 255, 1.f); 
        casestr("blueviolet") return Color4f(138.f / 255,43.f / 255,226.f / 255, 1.f); 
        casestr("brown") return Color4f(165.f / 255,42.f / 255,42.f / 255, 1.f); 
        casestr("burlywood") return Color4f(222.f / 255,184.f / 255,135.f / 255, 1.f); 
        casestr("cadetblue") return Color4f(95.f / 255,158.f / 255,160.f / 255, 1.f); 
        casestr("chartreuse") return Color4f(127.f / 255,255.f / 255,0.f / 255, 1.f); 
        casestr("chocolate") return Color4f(210.f / 255,105.f / 255,30.f / 255, 1.f); 
        casestr("coral") return Color4f(255.f / 255,127.f / 255,80.f / 255, 1.f); 
        casestr("cornflowerblue") return Color4f(100.f / 255,149.f / 255,237.f / 255, 1.f); 
        casestr("cornsilk") return Color4f(255.f / 255,248.f / 255,220.f / 255, 1.f); 
        casestr("crimson") return Color4f(220.f / 255,20.f / 255,60.f / 255, 1.f); 
        casestr("cyan") return Color4f(0.f / 255,255.f / 255,255.f / 255, 1.f); 
        casestr("darkblue") return Color4f(0.f / 255,0.f / 255,139.f / 255, 1.f); 
        casestr("darkcyan") return Color4f(0.f / 255,139.f / 255,139.f / 255, 1.f); 
        casestr("darkgoldenrod") return Color4f(184.f / 255,134.f / 255,11.f / 255, 1.f); 
        casestr("darkgray") return Color4f(169.f / 255,169.f / 255,169.f / 255, 1.f); 
        casestr("darkgrey") return Color4f(169.f / 255,169.f / 255,169.f / 255, 1.f); 
        casestr("darkgreen") return Color4f(0.f / 255,100.f / 255,0.f / 255, 1.f); 
        casestr("darkkhaki") return Color4f(189.f / 255,183.f / 255,107.f / 255, 1.f); 
        casestr("darkmagenta") return Color4f(139.f / 255,0.f / 255,139.f / 255, 1.f); 
        casestr("darkolivegreen") return Color4f(85.f / 255,107.f / 255,47.f / 255, 1.f); 
        casestr("darkorange") return Color4f(255.f / 255,140.f / 255,0.f / 255, 1.f); 
        casestr("darkorchid") return Color4f(153.f / 255,50.f / 255,204.f / 255, 1.f); 
        casestr("darkred") return Color4f(139.f / 255,0.f / 255,0.f / 255, 1.f); 
        casestr("darksalmon") return Color4f(233.f / 255,150.f / 255,122.f / 255, 1.f); 
        casestr("darkseagreen") return Color4f(143.f / 255,188.f / 255,143.f / 255, 1.f); 
        casestr("darkslateblue") return Color4f(72.f / 255,61.f / 255,139.f / 255, 1.f); 
        casestr("darkslategray") return Color4f(47.f / 255,79.f / 255,79.f / 255, 1.f); 
        casestr("darkslategrey") return Color4f(47.f / 255,79.f / 255,79.f / 255, 1.f); 
        casestr("darkturquoise") return Color4f(0.f / 255,206.f / 255,209.f / 255, 1.f); 
        casestr("darkviolet") return Color4f(148.f / 255,0.f / 255,211.f / 255, 1.f); 
        casestr("deeppink") return Color4f(255.f / 255,20.f / 255,147.f / 255, 1.f); 
        casestr("deepskyblue") return Color4f(0.f / 255,191.f / 255,255.f / 255, 1.f); 
        casestr("dimgray") return Color4f(105.f / 255,105.f / 255,105.f / 255, 1.f); 
        casestr("dimgrey") return Color4f(105.f / 255,105.f / 255,105.f / 255, 1.f); 
        casestr("dodgerblue") return Color4f(30.f / 255,144.f / 255,255.f / 255, 1.f); 
        casestr("firebrick") return Color4f(178.f / 255,34.f / 255,34.f / 255, 1.f); 
        casestr("floralwhite") return Color4f(255.f / 255,250.f / 255,240.f / 255, 1.f); 
        casestr("forestgreen") return Color4f(34.f / 255,139.f / 255,34.f / 255, 1.f); 
        casestr("fuchsia") return Color4f(255.f / 255,0.f / 255,255.f / 255, 1.f); 
        casestr("gainsboro") return Color4f(220.f / 255,220.f / 255,220.f / 255, 1.f); 
        casestr("ghostwhite") return Color4f(248.f / 255,248.f / 255,255.f / 255, 1.f); 
        casestr("gold") return Color4f(255.f / 255,215.f / 255,0.f / 255, 1.f); 
        casestr("goldenrod") return Color4f(218.f / 255,165.f / 255,32.f / 255, 1.f); 
        casestr("gray") return Color4f(128.f / 255,128.f / 255,128.f / 255, 1.f); 
        casestr("grey") return Color4f(128.f / 255,128.f / 255,128.f / 255, 1.f); 
        casestr("green") return Color4f(0.f / 255,128.f / 255,0.f / 255, 1.f); 
        casestr("greenyellow") return Color4f(173.f / 255,255.f / 255,47.f / 255, 1.f); 
        casestr("honeydew") return Color4f(240.f / 255,255.f / 255,240.f / 255, 1.f); 
        casestr("hotpink") return Color4f(255.f / 255,105.f / 255,180.f / 255, 1.f); 
        casestr("indianred") return Color4f(205.f / 255,92.f / 255,92.f / 255, 1.f); 
        casestr("indigo") return Color4f(75.f / 255,0.f / 255,130.f / 255, 1.f); 
        casestr("ivory") return Color4f(255.f / 255,255.f / 255,240.f / 255, 1.f); 
        casestr("khaki") return Color4f(240.f / 255,230.f / 255,140.f / 255, 1.f); 
        casestr("lavender") return Color4f(230.f / 255,230.f / 255,250.f / 255, 1.f); 
        casestr("lavenderblush") return Color4f(255.f / 255,240.f / 255,245.f / 255, 1.f); 
        casestr("lawngreen") return Color4f(124.f / 255,252.f / 255,0.f / 255, 1.f); 
        casestr("lemonchiffon") return Color4f(255.f / 255,250.f / 255,205.f / 255, 1.f); 
        casestr("lightblue") return Color4f(173.f / 255,216.f / 255,230.f / 255, 1.f); 
        casestr("lightcoral") return Color4f(240.f / 255,128.f / 255,128.f / 255, 1.f); 
        casestr("lightcyan") return Color4f(224.f / 255,255.f / 255,255.f / 255, 1.f); 
        casestr("lightgoldenrodyellow") return Color4f(250.f / 255,250.f / 255,210.f / 255, 1.f); 
        casestr("lightgray") return Color4f(211.f / 255,211.f / 255,211.f / 255, 1.f); 
        casestr("lightgrey") return Color4f(211.f / 255,211.f / 255,211.f / 255, 1.f); 
        casestr("lightgreen") return Color4f(144.f / 255,238.f / 255,144.f / 255, 1.f); 
        casestr("lightpink") return Color4f(255.f / 255,182.f / 255,193.f / 255, 1.f); 
        casestr("lightsalmon") return Color4f(255.f / 255,160.f / 255,122.f / 255, 1.f); 
        casestr("lightseagreen") return Color4f(32.f / 255,178.f / 255,170.f / 255, 1.f); 
        casestr("lightskyblue") return Color4f(135.f / 255,206.f / 255,250.f / 255, 1.f); 
        casestr("lightslategray") return Color4f(119.f / 255,136.f / 255,153.f / 255, 1.f); 
        casestr("lightslategrey") return Color4f(119.f / 255,136.f / 255,153.f / 255, 1.f); 
        casestr("lightsteelblue") return Color4f(176.f / 255,196.f / 255,222.f / 255, 1.f); 
        casestr("lightyellow") return Color4f(255.f / 255,255.f / 255,224.f / 255, 1.f); 
        casestr("lime") return Color4f(0.f / 255,255.f / 255,0.f / 255, 1.f); 
        casestr("limegreen") return Color4f(50.f / 255,205.f / 255,50.f / 255, 1.f); 
        casestr("linen") return Color4f(250.f / 255,240.f / 255,230.f / 255, 1.f); 
        casestr("magenta") return Color4f(255.f / 255,0.f / 255,255.f / 255, 1.f); 
        casestr("maroon") return Color4f(128.f / 255,0.f / 255,0.f / 255, 1.f); 
        casestr("mediumaquamarine") return Color4f(102.f / 255,205.f / 255,170.f / 255, 1.f); 
        casestr("mediumblue") return Color4f(0.f / 255,0.f / 255,205.f / 255, 1.f); 
        casestr("mediumorchid") return Color4f(186.f / 255,85.f / 255,211.f / 255, 1.f); 
        casestr("mediumpurple") return Color4f(147.f / 255,112.f / 255,219.f / 255, 1.f); 
        casestr("mediumseagreen") return Color4f(60.f / 255,179.f / 255,113.f / 255, 1.f); 
        casestr("mediumslateblue") return Color4f(123.f / 255,104.f / 255,238.f / 255, 1.f); 
        casestr("mediumspringgreen") return Color4f(0.f / 255,250.f / 255,154.f / 255, 1.f); 
        casestr("mediumturquoise") return Color4f(72.f / 255,209.f / 255,204.f / 255, 1.f); 
        casestr("mediumvioletred") return Color4f(199.f / 255,21.f / 255,133.f / 255, 1.f); 
        casestr("midnightblue") return Color4f(25.f / 255,25.f / 255,112.f / 255, 1.f); 
        casestr("mintcream") return Color4f(245.f / 255,255.f / 255,250.f / 255, 1.f); 
        casestr("mistyrose") return Color4f(255.f / 255,228.f / 255,225.f / 255, 1.f); 
        casestr("moccasin") return Color4f(255.f / 255,228.f / 255,181.f / 255, 1.f); 
        casestr("navajowhite") return Color4f(255.f / 255,222.f / 255,173.f / 255, 1.f); 
        casestr("navy") return Color4f(0.f / 255,0.f / 255,128.f / 255, 1.f); 
        casestr("oldlace") return Color4f(253.f / 255,245.f / 255,230.f / 255, 1.f); 
        casestr("olive") return Color4f(128.f / 255,128.f / 255,0.f / 255, 1.f); 
        casestr("olivedrab") return Color4f(107.f / 255,142.f / 255,35.f / 255, 1.f); 
        casestr("orange") return Color4f(255.f / 255,165.f / 255,0.f / 255, 1.f); 
        casestr("orangered") return Color4f(255.f / 255,69.f / 255,0.f / 255, 1.f); 
        casestr("orchid") return Color4f(218.f / 255,112.f / 255,214.f / 255, 1.f); 
        casestr("palegoldenrod") return Color4f(238.f / 255,232.f / 255,170.f / 255, 1.f); 
        casestr("palegreen") return Color4f(152.f / 255,251.f / 255,152.f / 255, 1.f); 
        casestr("paleturquoise") return Color4f(175.f / 255,238.f / 255,238.f / 255, 1.f); 
        casestr("palevioletred") return Color4f(219.f / 255,112.f / 255,147.f / 255, 1.f); 
        casestr("papayawhip") return Color4f(255.f / 255,239.f / 255,213.f / 255, 1.f); 
        casestr("peachpuff") return Color4f(255.f / 255,218.f / 255,185.f / 255, 1.f); 
        casestr("peru") return Color4f(205.f / 255,133.f / 255,63.f / 255, 1.f); 
        casestr("pink") return Color4f(255.f / 255,192.f / 255,203.f / 255, 1.f); 
        casestr("plum") return Color4f(221.f / 255,160.f / 255,221.f / 255, 1.f); 
        casestr("powderblue") return Color4f(176.f / 255,224.f / 255,230.f / 255, 1.f); 
        casestr("purple") return Color4f(128.f / 255,0.f / 255,128.f / 255, 1.f); 
        casestr("rebeccapurple") return Color4f(102.f / 255,51.f / 255,153.f / 255, 1.f); 
        casestr("red") return Color4f(255.f / 255,0.f / 255,0.f / 255, 1.f); 
        casestr("rosybrown") return Color4f(188.f / 255,143.f / 255,143.f / 255, 1.f); 
        casestr("royalblue") return Color4f(65.f / 255,105.f / 255,225.f / 255, 1.f); 
        casestr("saddlebrown") return Color4f(139.f / 255,69.f / 255,19.f / 255, 1.f); 
        casestr("salmon") return Color4f(250.f / 255,128.f / 255,114.f / 255, 1.f); 
        casestr("sandybrown") return Color4f(244.f / 255,164.f / 255,96.f / 255, 1.f); 
        casestr("seagreen") return Color4f(46.f / 255,139.f / 255,87.f / 255, 1.f); 
        casestr("seashell") return Color4f(255.f / 255,245.f / 255,238.f / 255, 1.f); 
        casestr("sienna") return Color4f(160.f / 255,82.f / 255,45.f / 255, 1.f); 
        casestr("silver") return Color4f(192.f / 255,192.f / 255,192.f / 255, 1.f); 
        casestr("skyblue") return Color4f(135.f / 255,206.f / 255,235.f / 255, 1.f); 
        casestr("slateblue") return Color4f(106.f / 255,90.f / 255,205.f / 255, 1.f); 
        casestr("slategray") return Color4f(112.f / 255,128.f / 255,144.f / 255, 1.f); 
        casestr("slategrey") return Color4f(112.f / 255,128.f / 255,144.f / 255, 1.f); 
        casestr("snow") return Color4f(255.f / 255,250.f / 255,250.f / 255, 1.f); 
        casestr("springgreen") return Color4f(0.f / 255,255.f / 255,127.f / 255, 1.f); 
        casestr("steelblue") return Color4f(70.f / 255,130.f / 255,180.f / 255, 1.f); 
        casestr("tan") return Color4f(210.f / 255,180.f / 255,140.f / 255, 1.f); 
        casestr("teal") return Color4f(0.f / 255,128.f / 255,128.f / 255, 1.f); 
        casestr("thistle") return Color4f(216.f / 255,191.f / 255,216.f / 255, 1.f); 
        casestr("tomato") return Color4f(255.f / 255,99.f / 255,71.f / 255, 1.f); 
        casestr("turquoise") return Color4f(64.f / 255,224.f / 255,208.f / 255, 1.f); 
        casestr("violet") return Color4f(238.f / 255,130.f / 255,238.f / 255, 1.f); 
        casestr("wheat") return Color4f(245.f / 255,222.f / 255,179.f / 255, 1.f); 
        casestr("white") return Color4f(255.f / 255,255.f / 255,255.f / 255, 1.f); 
        casestr("whitesmoke") return Color4f(245.f / 255,245.f / 255,245.f / 255, 1.f); 
        casestr("yellow") return Color4f(255.f / 255,255.f / 255,0.f / 255, 1.f); 
        casestr("yellowgreen") return Color4f(154.f / 255,205.f / 255,50.f / 255, 1.f); 
        // special
        casestr("transparent") return Color4f(0.f, 0.f, 0.f, 0.f); 
        default:
            break;
    }
    throw peg::parse_error("invalid color name!");
}
}

std::string_view OGUI::MathValueParser::GetGrammar()
{
    static auto grammar = R"(
        NumberPercentage <- Percentage / Number
        ColorComponent <- Percentage / Number
        Angle	     <- Number ('deg' / 'rad' / 'grad' / 'turn')
        Hue          <-  Angle / Number
        Color        <- ColorRGBA / ColorHSLA / ColorHex / ColorName
        ColorName    <- < IDENT >
        ColorRGBA    <- 'rgb' 'a'? '(' ((w ColorComponent w ',' w ColorComponent w ',' w ColorComponent w (',' w NumberPercentage w)?) / (w ColorComponent w ColorComponent w ColorComponent w ('/' w NumberPercentage)))')'
        ColorHSLA    <- 'hsl' 'a'? '(' ((w Hue w ',' w Percentage w ',' w Percentage w (',' w NumberPercentage w)?) / (w Angle w Percentage w Percentage w ('/' w NumberPercentage)))')'
        ColorHex     <- '#' <[0-9a-fA-F]+>

        
        TransformFunction		<- TranslateFunction / TranslateXFunction / TranslateYFunction / ScaleFunction / ScaleXFunction / ScaleYFunction / SkewFunction / SkewXFunction / SkewYFunction / RotateFunction / MatrixFunction
        TranslateFunction		<- 'translate(' w Length w ',' w Length w ')'
        TranslateXFunction		<- 'translateX(' w Length w ')'
        TranslateYFunction		<- 'translateY(' w Length w ')'
        ScaleFunction			<- 'scale(' w Number w ',' w Number w ')'
        ScaleXFunction			<- 'scaleX(' w Number w ')'
        ScaleYFunction			<- 'scaleY(' w Number w ')'
        RotateFunction			<- 'rotate(' w Angle w ')'
        SkewFunction			<- 'skew(' w Angle w',' w Angle w  ')'
        SkewXFunction			<- 'skewX(' w Angle w  ')'
        SkewYFunction			<- 'skewY(' w Angle w ')'
        MatrixFunction			<- 'matrix(' w Number w ',' w Number w ',' w Number w ',' w Number w ',' w Number w ',' w Number w ')'
    )";
    return grammar;
}

int hex_number(const char a)
{
    if(a >= '0' && a <= '9')
        return a-'0';
    else if(a >= 'a' && a <= 'f')
        return a-'a' + 10;
    else if(a >= 'A' && a <= 'F')
        return a-'A' + 10;
    else
        throw peg::parse_error("invalid hex number.");
};

void OGUI::MathValueParser::SetupAction(peg::parser &parser)
{
    using namespace peg;

    parser["NumberPercentage"] = [](SemanticValues& vs)
    {
        return std::any_cast<float>(vs[0]);
    };

    parser["ColorComponent"] = [](SemanticValues& vs)
    {
        auto value = std::any_cast<float>(vs[0]);
        return vs.choice() == 0 ? value : value / 255;
    };

    parser["Angle"] = [](SemanticValues& vs)
    {
        float value = std::any_cast<float>(vs[0]);
        if (vs.choice() == 0)
            value = value / 180 * OGUI::math::PI_;// degree2rad(value);
        else if (vs.choice() == 2)
            value = value / 180 * OGUI::math::PI_ * 0.9;// grad2rad(value);
        else if (vs.choice() == 3)
            value = value * 2 * OGUI::math::PI_;// turn2rad(value);
        return value;
    };

    parser["Hue"] = [](SemanticValues& vs)
    {
        if (vs.choice() == 0)
            return std::any_cast<float>(vs[0]);
        else
            return std::any_cast<float>(vs[1]);
    };

    parser["ColorName"] = [](SemanticValues& vs)
    {
        return ParseColorName(vs.token());
    };

    parser["ColorRGBA"] = [](SemanticValues& vs)
    {
        float r = std::any_cast<float>(vs[0]);
        float g = std::any_cast<float>(vs[1]);
        float b = std::any_cast<float>(vs[2]);
        float a = 1.f;
        if(vs.size() > 3)
            a = std::any_cast<float>(vs[3]);
        return Color4f(r, g, b, a);
    };

    parser["ColorHSLA"] = [](SemanticValues& vs)
    {
        float h = std::any_cast<float>(vs[0]);
        float s = std::any_cast<float>(vs[1]);
        float l = std::any_cast<float>(vs[2]);
        float a = 1.f;
        if(vs.size() > 3)
            a = std::any_cast<float>(vs[3]);
        Color4f result;
		HSVtoRGB(h, s, l, result.X, result.Y, result.Z);
        result.Y = a;
        return result;
    };

    parser["ColorHex"] = [](SemanticValues& vs)
    {
        Color4f color = Color4f::vector_one();
        auto str = vs.token();
        auto S2H = [](const char a, const char b)
        {
            return hex_number(a)*16 + hex_number(b);
        };
        auto length = str.length();
        if (length == 3 || length == 4)
        {
            color.X = S2H(str[0], str[0]) / 255.0;
            color.Y = S2H(str[1], str[1]) / 255.0;
            color.Z = S2H(str[2], str[2]) / 255.0;
            if (length == 4)
            {
                color.W = S2H(str[3], str[3]) / 255.0;
            }
        }
        else if (length == 6 || length == 8)
        {
            color.X = S2H(str[0], str[1]) / 255.0;
            color.Y = S2H(str[2], str[3]) / 255.0;
            color.Z = S2H(str[4], str[5]) / 255.0;
            if (length == 8)
            {
                color.W = S2H(str[6], str[7]) / 255.0;
            }
        }
        else
        {
            throw parse_error("invalid hex length.");
        }
        return color;
    };

    parser["Color"] = [](SemanticValues& vs)
    {
        return std::any_cast<Color4f>(vs[0]);
    };

#define ARG(n) any_move<float>(vs[n])
    parser["RotateFunction"] = [](SemanticValues& vs) { return TransformFunction::rotate(ARG(0)); };
    parser["ScaleXFunction"] = [](SemanticValues& vs) { return TransformFunction::scale({ARG(0), 1}); };
    parser["ScaleYFunction"] = [](SemanticValues& vs) { return TransformFunction::scale({1, ARG(0)}); };
    parser["ScaleFunction"] =   [](SemanticValues& vs)  { return TransformFunction::scale({ARG(0), ARG(1)}); };
    parser["TranslateXFunction"] = [](SemanticValues& vs) { return TransformFunction::translate({ARG(0), 0}); };
    parser["TranslateYFunction"] = [](SemanticValues& vs) { return TransformFunction::translate({0, ARG(0)}); };
    parser["TranslateFunction"] = [](SemanticValues& vs) { return TransformFunction::translate({ARG(0), ARG(1)}); };
    parser["SkewXFunction"] = [](SemanticValues& vs) { return TransformFunction::skew({ARG(0), 0}); };
    parser["SkewYFunction"] = [](SemanticValues& vs) { return TransformFunction::skew({0, ARG(0)}); };
    parser["SkewFunction"] = [](SemanticValues& vs) { return TransformFunction::skew({ARG(0), ARG(1)}); };
    parser["MatrixFunction"] = [](SemanticValues& vs) { return TransformFunction::matrix(ARG(0), ARG(1), ARG(2), ARG(3), ARG(4), ARG(5)); };
#undef ARG
    parser["TransformFunction"] = [](SemanticValues& vs) { return std::move(vs[0]); };
}