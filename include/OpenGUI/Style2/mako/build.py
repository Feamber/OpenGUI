
import json
import os.path
import re
import sys
import json

BASE = os.path.dirname(os.path.realpath(__file__).replace("\\", "/"))
ROOT = os.path.join(BASE, "../../../..")

sys.path.insert(0, ROOT)
from tool.style_codegen import PHYSICAL_CORNERS, PHYSICAL_SIDES, PHYSICAL_SIZES, StyleStruct, renderer, DEFAULT_HEADER_TEMPLATE_PATH, DEFAULT_SOURCE_TEMPLATE_PATH, to_small_camel_case
HEADER_OUT_DIR = os.path.join(ROOT, "include/OpenGUI/Style2/generated")
SOURCE_OUT_DIR = os.path.join(ROOT, "source/Style2/generated")
INCLUDE_DIR = os.path.join(ROOT, "include")
RENDERER = renderer(BASE)


def render(filename, **context):
    return RENDERER.render(filename, **context)


def write(path, content):
    RENDERER.write(path, content)


def make_struct(name, inherited):
    return StyleStruct(name, inherited, HEADER_OUT_DIR, SOURCE_OUT_DIR, INCLUDE_DIR)


def render_struct(struct, shorthand_path=None):
    struct.headers.append("OpenGUI/Configure.h")
    if shorthand_path:
        shorthand_template = os.path.join(BASE, shorthand_path)
        shorthand_file = render(shorthand_template, struct=struct)
        write(struct.shorthand_path, shorthand_file)
    header_file = render(DEFAULT_HEADER_TEMPLATE_PATH, struct=struct, linkage="OGUI_API")
    source_file = render(DEFAULT_SOURCE_TEMPLATE_PATH, struct=struct)
    write(struct.header_path, header_file)
    write(struct.source_path, source_file)


def gen_position():
    struct = make_struct("position", False)

    def add_longhand(*args, **kwargs):
        struct.add_longhand(*args, **kwargs)
    add_longhand("transform",       "TransformFunction", "{}",
                 "TransformFunction",    vector=True, restyle_damage="Transform")
    #add_longhand("transform",       "ComputedTransform","ComputedTransform::ident()")
    add_longhand("flex-grow",       "float",            "0.f",
                 "Number", restyle_damage="Layout")
    add_longhand("flex-shrink",     "float",            "1.f",
                 "Number", restyle_damage="Layout")
    add_longhand("flex-basis",      "YGValue",
                 "YGValueAuto",  "Width", restyle_damage="Layout")
    for side in PHYSICAL_SIDES:
        add_longhand(side, "YGValue", "YGValueAuto",
                     "LengthPercentage", restyle_damage="Layout")
    for side in PHYSICAL_SIDES:
        add_longhand("margin-{0}".format(side), "YGValue",
                     "YGValueZero",  "LengthPercentage", restyle_damage="Layout")
    for side in PHYSICAL_SIDES:
        add_longhand("padding-{0}".format(side), "YGValue",
                     "YGValueZero",  "LengthPercentage", restyle_damage="Layout")
    add_longhand("width",			"YGValue",			"YGValueAuto",
                 "Width", restyle_damage="Layout")
    add_longhand("height",			"YGValue",			"YGValueAuto",
                 "Width", restyle_damage="Layout")
    add_longhand("position",		"YGPositionType",	"YGPositionTypeRelative",
                 "FlexPosition", restyle_damage="Layout")
    add_longhand("overflow",		"EFlexOverflow",
                 "EFlexOverflow::Visible", "FlexOverflow", restyle_damage="Layout")
    add_longhand("align-self",		"YGAlign",			"YGAlignAuto",
                 "FlexAlign", restyle_damage="Layout")
    for size in PHYSICAL_SIZES:
        add_longhand("max-{0}".format(size), "YGValue", "YGValueUndefined",
                     "LengthPercentage", restyle_damage="Layout")
    for size in PHYSICAL_SIZES:
        add_longhand("min-{0}".format(size), "YGValue",
                     "YGValueAuto",  "Width", restyle_damage="Layout")
    add_longhand("flex-direction",	"YGFlexDirection",
                 "YGFlexDirectionRow", "FlexDirection", restyle_damage="Layout")
    add_longhand("align-content",	"YGAlign",
                 "YGAlignFlexStart", "FlexAlign", restyle_damage="Layout")
    add_longhand("align-items",		"YGAlign",
                 "YGAlignStretch", "FlexAlign", restyle_damage="Layout")
    add_longhand("justify-content",	"YGJustify",
                 "YGJustifyFlexStart", "FlexJustify", restyle_damage="Layout")
    add_longhand("flex-wrap",		"YGWrap",			"YGWrapNoWrap", "FlexWrap",
                 restyle_damage="Layout")
    add_longhand("flex-display",	"YGDisplay",
                 "YGDisplayFlex", "FlexDisplay", restyle_damage="Layout")
    add_longhand("vertical-align",	"EInlineAlign",
                 "EInlineAlign::Middle", "InlineAlign", restyle_damage="Layout")
    add_longhand("aspect-ratio",      "float",           "YGUndefined",
                 "AspectRatio",  restyle_damage="Layout")
    add_longhand("z-order-bias", "int", "0", "Integer")

    struct.headers.append("yoga/Yoga.h")
    struct.headers.append("OpenGUI/Core/Types.h")
    struct.headers.append("OpenGUI/Core/OMath.h")
    struct.headers.append("OpenGUI/Style2/Transform.h")
    struct.headers.append("OpenGUI/Style2/Lerp/MathLerp.h")
    struct.headers.append("OpenGUI/Style2/Lerp/YogaLerp.h")
    render_struct(struct, "shorthands/position.h.mako")


def gen_border():
    struct = make_struct("border", False)

    def add_longhand(*args, **kwargs):
        struct.add_longhand(*args, **kwargs)
    for side in PHYSICAL_SIDES:
        add_longhand("border-{0}-width".format(side), "float",
                     "0.f", "Length", restyle_damage="Layout")
    for corner in PHYSICAL_CORNERS:
        add_longhand("border-{0}-radius".format(corner),
                     "YGValue", "YGValueZero", "LengthPercentage")
    struct.headers.append("yoga/Yoga.h")
    struct.headers.append("OpenGUI/Style2/Lerp/MathLerp.h")
    struct.headers.append("OpenGUI/Style2/Lerp/YogaLerp.h")
    render_struct(struct, "shorthands/border.h.mako")


def gen_text():
    struct = make_struct("text", True)

    def add_longhand(*args, **kwargs):
        struct.add_longhand(*args, **kwargs)
    add_longhand("font-size", "float", "20.f",
                 "Number", restyle_damage="TextLayout")
    add_longhand("color", "Color4f", "Color4f(0,0,0,1)", "Color")
    add_longhand("font-family", "ostr::string", "{}",
                 "FontFamily", string = True, vector = True, restyle_damage="TextLayout|Font")
    add_longhand("font-style", "ETextStyle", "ETextStyle::Normal",
                 "TextStyle", restyle_damage="TextLayout|Font")
    add_longhand("font-weight", "int", "400", "TextWeight",
                 restyle_damage="TextLayout|Font")
    add_longhand("line-height", "YGValue", "YGValueAuto",
                 "Width", restyle_damage="TextLayout")
    add_longhand("text-align", "ETextAlign", "ETextAlign::Start", "TextAlign")
    add_longhand("text-shadow", "TextShadow", "{}", "TextShadow", vector=True)
    struct.headers.append("yoga/Yoga.h")
    struct.headers.append("OpenGUI/Style2/Shadow.h")
    struct.headers.append("OpenGUI/Text/TextTypes.h")
    struct.headers.append("OpenGUI/Style2/Lerp/MathLerp.h")
    struct.headers.append("OpenGUI/Style2/Lerp/TextLerp.h")
    struct.headers.append("OpenGUI/Style2/Lerp/YogaLerp.h")
    render_struct(struct, "shorthands/text.h.mako")


def gen_effects():
    struct = make_struct("effects", False)

    def add_longhand(*args, **kwargs):
        struct.add_longhand(*args, **kwargs)
    add_longhand("opacity",	"float", "1", "Number")
    struct.headers.append("OpenGUI/Style2/Lerp/MathLerp.h")
    render_struct(struct)


def gen_background():
    struct = make_struct("background", False)

    def add_longhand(*args, **kwargs):
        struct.add_longhand(*args, **kwargs)
    add_longhand("background-color",	"Color4f",
                 "Color4f(1.f,1.f,1.f,1.f)", "Color")
    add_longhand("background-image",	"ostr::string",	"{}", "URL", string = True)
    add_longhand("background-material", "ostr::string", "{}", "URL", string = True)
    struct.headers.append("OpenGUI/Core/OMath.h")
    struct.headers.append("OpenGUI/Style2/Lerp/MathLerp.h")
    render_struct(struct)


def gen_animation():
    struct = make_struct("animation", False)

    def add_longhand(*args, **kwargs):
        struct.add_longhand(*args, **kwargs)
    add_longhand("animation-name", "ostr::string", "{}", "Name", string = True)
    add_longhand("animation-duration", "float", "1.f", "Time")
    add_longhand("animation-delay", "float", "0.f", "Time")
    add_longhand("animation-direction", "EAnimDirection",
                 "EAnimDirection::Normal", "AnimDirection")
    add_longhand("animation-iteration-count", "float",
                 "1.f", "AnimIterationCount")
    add_longhand("animation-play-state",
                 "EAnimPlayState", "{}", "AnimPlayState")
    add_longhand("animation-timing-function",
                 "AnimTimingFunction", "{}", "AnimTimingFunction")
    add_longhand("animation-fill-mode", "EAnimFillMode",
                 "EAnimFillMode::Forwards", "AnimFillMode")
    add_longhand("animation-yield-mode", "EAnimYieldMode",
                 "EAnimYieldMode::GoBack", "AnimYieldMode")
    add_longhand("animation-resume-mode", "EAnimResumeMode",
                 "EAnimResumeMode::Resume", "AnimResumeMode")
    struct.headers.append("OpenGUI/Style2/AnimTypes.h")
    header_template = os.path.join(BASE, "AnimStruct.h.mako")
    header_file = render(header_template, struct=struct)
    source_template = os.path.join(BASE, "AnimStruct.cpp.mako")
    source_file = render(source_template, struct=struct)
    write(struct.header_path, header_file)
    write(struct.source_path, source_file)


def to_syntax(ident):
    return re.sub("([A-Z]+)", lambda m: "-" + m.group(1).lower(), ident).strip("-")


class Enumerator(object):
    def __init__(self, name, value):
        self.name = name
        self.short_name = str.rsplit(name, "::", 1)[-1]
        self.syntax = to_syntax(self.short_name)
        self.value = value


class Enum(object):
    def __init__(self, name, enumerators):
        self.name = name
        self.short_name = str.rsplit(name, "::", 1)[-1]
        self.raw_name = self.short_name[1:]
        self.enumerators = sorted(enumerators, key=lambda e:e.short_name, reverse=True)


class DB(object):
    def __init__(self):
        self.enums = []
        self.headers = set()


def GetInclude(path):
    return path.replace("\\", "/").rsplit("include/", 1)[-1]


def gen_enum_parser():
    meta = json.load(open(os.path.join(ROOT, "build/meta.json")))
    db = DB()
    for key, value in meta["enums"].items():
        attr = value["attrs"]
        if not "style-enum" in attr:
            continue
        file = value["fileName"]
        if str.endswith(file, ".cpp"):
            print("unable to gen lua bind for records in cpp, name:%s" %
                  value["name"], file=sys.stderr)
            continue
        db.headers.add(GetInclude(file))
        enumerators = []
        for key2, value2 in value["values"].items():
            enumerators.append(Enumerator(key2, value2["value"]))
        db.enums.append(Enum(key, enumerators))
    template = os.path.join(BASE, "EnumParser.cpp.mako")
    content = render(template, db=db)
    output = os.path.join(SOURCE_OUT_DIR, "EnumParser.cpp")
    write(output, content)


def main():
    gen_position()
    gen_border()
    gen_text()
    gen_background()
    gen_animation()
    gen_effects()
    gen_enum_parser()


if __name__ == "__main__":
    main()
