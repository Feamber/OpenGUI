import json
import os.path
import re
import sys

BASE = os.path.dirname(os.path.realpath(__file__).replace("\\", "/"))
ROOT = os.path.join(BASE, "../../../..")

sys.path.insert(0, ROOT)
from tool.style_codegen import PHYSICAL_CORNERS, PHYSICAL_SIDES, PHYSICAL_SIZES, StyleStruct, renderer, DEFAULT_HEADER_TEMPLATE_PATH, DEFAULT_SOURCE_TEMPLATE_PATH
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

def render_struct(struct, shorthand_path = None):
    if shorthand_path:
        shorthand_template = os.path.join(BASE, shorthand_path)
        shorthand_file = render(shorthand_template, struct = struct)
        write(struct.shorthand_path, shorthand_file)
    header_file = render(DEFAULT_HEADER_TEMPLATE_PATH, struct = struct)
    source_file = render(DEFAULT_SOURCE_TEMPLATE_PATH, struct = struct)
    write(struct.header_path, header_file)
    write(struct.source_path, source_file)

def gen_position():
    struct = make_struct("position", False)
    def add_longhand(*args, **kwargs):
        struct.add_longhand(*args, **kwargs)
    add_longhand("transform",       "TransformFunction","{}"      ,vector=True    ,restyle_damage="Transform")
    #add_longhand("transform",       "ComputedTransform","ComputedTransform::ident()")
    add_longhand("flex-grow",       "float",            "0.f"   ,restyle_damage="Layout")
    add_longhand("flex-shrink",     "float",            "1.f"   ,restyle_damage="Layout")
    add_longhand("flex-basis",      "YGValue",          "YGValueAuto"   ,restyle_damage="Layout")
    for side in PHYSICAL_SIDES:
        add_longhand(side, "YGValue", "YGValueAuto"   ,restyle_damage="Layout")
    for side in PHYSICAL_SIDES:
        add_longhand("margin-{0}".format(side), "YGValue", "YGValueZero"   ,restyle_damage="Layout")
    for side in PHYSICAL_SIDES:
        add_longhand("padding-{0}".format(side), "YGValue", "YGValueZero"   ,restyle_damage="Layout")
    add_longhand("width",			"YGValue",			"YGValueAuto"   ,restyle_damage="Layout")		
    add_longhand("height",			"YGValue",			"YGValueAuto"   ,restyle_damage="Layout")		
    add_longhand("position",		"YGPositionType",	"YGPositionTypeRelative"   ,restyle_damage="Layout")
    add_longhand("overflow",		"StyleOverflow",	"StyleOverflow::Visible"   ,restyle_damage="Layout")
    add_longhand("align-self",		"YGAlign",			"YGAlignAuto"   ,restyle_damage="Layout")		
    for size in PHYSICAL_SIZES:
        add_longhand("max-{0}".format(size), "YGValue", "YGValueUndefined"   ,restyle_damage="Layout")	
    for size in PHYSICAL_SIZES:
        add_longhand("min-{0}".format(size), "YGValue", "YGValueAuto"   ,restyle_damage="Layout")	
    add_longhand("flex-direction",	"YGFlexDirection",	"YGFlexDirectionRow"   ,restyle_damage="Layout")
    add_longhand("align-content",	"YGAlign",			"YGAlignFlexStart"   ,restyle_damage="Layout")	
    add_longhand("align-items",		"YGAlign",			"YGAlignStretch"   ,restyle_damage="Layout")	
    add_longhand("justify-content",	"YGJustify",		"YGJustifyFlexStart"   ,restyle_damage="Layout")
    add_longhand("flex-wrap",		"YGWrap",			"YGWrapNoWrap"   ,restyle_damage="Layout")		
    add_longhand("flex-display",	"YGDisplay",		"YGDisplayFlex"   ,restyle_damage="Layout")		
    add_longhand("vertical-align",	"EInlineAlign",		"EInlineAlign::Middle"   ,restyle_damage="Layout")	

    struct.headers.append("yoga/Yoga.h")
    struct.headers.append("OpenGUI/Core/OMath.h")
    struct.headers.append("OpenGUI/Style2/Transform.h")
    struct.headers.append("OpenGUI/Style2/Parse/MathParse.h")
    struct.headers.append("OpenGUI/Style2/Parse/YogaParse.h")
    struct.headers.append("OpenGUI/Style2/Lerp/MathLerp.h")
    struct.headers.append("OpenGUI/Style2/Lerp/YogaLerp.h")
    render_struct(struct, "shorthands/position.h.mako")

def gen_border():
    struct = make_struct("border", False)
    def add_longhand(*args, **kwargs):
        struct.add_longhand(*args, **kwargs)
    for side in PHYSICAL_SIDES:
        add_longhand("border-{0}-width".format(side), "float", "0.f"   ,restyle_damage="Layout")
    for corner in PHYSICAL_CORNERS:
        add_longhand("border-{0}-radius".format(corner), "YGValue", "YGValueZero")
    struct.headers.append("OpenGUI/Style2/Parse/MathParse.h")
    struct.headers.append("OpenGUI/Style2/Parse/YogaParse.h")
    struct.headers.append("OpenGUI/Style2/Lerp/MathLerp.h")
    struct.headers.append("OpenGUI/Style2/Lerp/YogaLerp.h")
    struct.headers.append("yoga/Yoga.h")
    render_struct(struct, "shorthands/border.h.mako")
    
def gen_text():
    struct = make_struct("text", True)
    def add_longhand(*args, **kwargs):
        struct.add_longhand(*args, **kwargs)
    add_longhand("font-size", "float", "20.f", restyle_damage="TextLayout")
    add_longhand("color", "Color4f", "Color4f(0,0,0,1)")
    add_longhand("font-family", "std::string", "{}", restyle_damage="TextLayout|Font")
    add_longhand("font-style", "TextStyle", "TextStyle::Normal", restyle_damage="TextLayout|Font")
    add_longhand("font-weight", "int", "400", parser = "ParseTextWeight", restyle_damage="TextLayout|Font")
    add_longhand("line-height", "float", "1.f", parser = "ParseLineHeight", restyle_damage="TextLayout")
    add_longhand("text-align", "TextAlign", "TextAlign::Start", restyle_damage="TextLayout")
    struct.headers.append("OpenGUI/Style2/Parse/MathParse.h")
    struct.headers.append("OpenGUI/Style2/Parse/TextParse.h")
    struct.headers.append("OpenGUI/Style2/Lerp/MathLerp.h")
    render_struct(struct, "shorthands/text.h.mako")

def gen_background():
    struct = make_struct("background", False)
    def add_longhand(*args, **kwargs):
        struct.add_longhand(*args, **kwargs)
    add_longhand("background-color",	"Color4f",	    "Color4f(1.f,1.f,1.f,1.f)")	
    add_longhand("background-image",	"std::string",	"{}", parser = "ParseUrl")
    struct.headers.append("OpenGUI/Core/OMath.h")
    struct.headers.append("OpenGUI/Style2/Parse/MathParse.h")
    struct.headers.append("OpenGUI/Style2/Lerp/MathLerp.h")
    render_struct(struct)

def gen_animation():
    struct = make_struct("animation", False)
    def add_longhand(*args, **kwargs):
        struct.add_longhand(*args, **kwargs)
    add_longhand("animation-name", "std::string", "{}")
    add_longhand("animation-duration", "float", "1.f", parser = "ParseTime")
    add_longhand("animation-delay", "float", "0.f", parser = "ParseTime")
    add_longhand("animation-direction", "EAnimDirection", "EAnimDirection::Normal")
    add_longhand("animation-iteration-count", "float", "1.f", parser = "ParseIterationCount")
    add_longhand("animation-play-state", "EAnimPlayState", "{}")
    add_longhand("animation-timing-function", "AnimTimingFunction", "{}")
    add_longhand("animation-fill-mode", "EAnimFillMode", "EAnimFillMode::Forwards")
    add_longhand("animation-yield-mode", "EAnimYieldMode", "EAnimYieldMode::GoBack")
    add_longhand("animation-resume-mode", "EAnimResumeMode", "EAnimResumeMode::Resume")
    struct.headers.append("OpenGUI/Style2/Parse/MathParse.h")
    struct.headers.append("OpenGUI/Style2/Lerp/MathLerp.h")
    struct.headers.append("OpenGUI/Style2/AnimTypes.h")
    header_template = os.path.join(BASE, "AnimStruct.h.mako")
    header_file = render(header_template, struct = struct)
    source_template = os.path.join(BASE, "AnimStruct.cpp.mako")
    source_file = render(source_template, struct = struct)
    write(struct.header_path, header_file)
    write(struct.source_path, source_file)

def main():
    gen_position()
    gen_border()
    gen_text()
    gen_background()
    gen_animation()

if __name__ == "__main__":
    main()