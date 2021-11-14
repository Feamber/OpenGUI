import json
import os.path
import re
import sys

BASE = os.path.dirname(__file__.replace("\\", "/"))
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
    header_file = render(DEFAULT_HEADER_TEMPLATE_PATH, struct = struct)
    source_file = render(DEFAULT_SOURCE_TEMPLATE_PATH, struct = struct)
    write(struct.header_path, header_file)
    write(struct.source_path, source_file)
    if shorthand_path:
        shorthand_template = os.path.join(BASE, shorthand_path)
        shorthand_file = render(shorthand_template, struct = struct)
        write(struct.shorthand_path, shorthand_file)

def gen_position():
    struct = make_struct("position", False)
    def add_longhand(*args, **kwargs):
        struct.add_longhand(*args, **kwargs)
    add_longhand("transform",       "TransformFunction","{}"      ,vector=True    ,restyle_damage="Transform")
    #add_longhand("transform",       "ComputedTransform","ComputedTransform::ident()")
    add_longhand("flex-grow",       "float",            "0.f"   ,restyle_damage="Yoga")
    add_longhand("flex-shrink",     "float",            "1.f"   ,restyle_damage="Yoga")
    add_longhand("flex-basis",      "YGValue",          "YGValueAuto"   ,restyle_damage="Yoga")
    for side in PHYSICAL_SIDES:
        add_longhand(side, "YGValue", "YGValueAuto"   ,restyle_damage="Yoga")
    for side in PHYSICAL_SIDES:
        add_longhand("margin-{0}".format(side), "YGValue", "YGValueZero"   ,restyle_damage="Yoga")
    for side in PHYSICAL_SIDES:
        add_longhand("padding-{0}".format(side), "YGValue", "YGValueZero"   ,restyle_damage="Yoga")
    add_longhand("width",			"YGValue",			"YGValueAuto"   ,restyle_damage="Yoga")		
    add_longhand("height",			"YGValue",			"YGValueAuto"   ,restyle_damage="Yoga")		
    add_longhand("position",		"YGPositionType",	"YGPositionTypeRelative"   ,restyle_damage="Yoga")
    add_longhand("overflow",		"YGOverflow",		"YGOverflowVisible"   ,restyle_damage="Yoga")
    add_longhand("align-self",		"YGAlign",			"YGAlignAuto"   ,restyle_damage="Yoga")		
    for size in PHYSICAL_SIZES:
        add_longhand("max-{0}".format(size), "YGValue", "YGValueUndefined"   ,restyle_damage="Yoga")	
    for size in PHYSICAL_SIZES:
        add_longhand("min-{0}".format(size), "YGValue", "YGValueAuto"   ,restyle_damage="Yoga")	
    add_longhand("flex-direction",	"YGFlexDirection",	"YGFlexDirectionRow"   ,restyle_damage="Yoga")
    add_longhand("align-content",	"YGAlign",			"YGAlignFlexStart"   ,restyle_damage="Yoga")	
    add_longhand("align-items",		"YGAlign",			"YGAlignStretch"   ,restyle_damage="Yoga")	
    add_longhand("justify-content",	"YGJustify",		"YGJustifyFlexStart"   ,restyle_damage="Yoga")
    add_longhand("flex-wrap",		"YGWrap",			"YGWrapNoWrap"   ,restyle_damage="Yoga")		
    add_longhand("flex-display",	"YGDisplay",		"YGDisplayFlex"   ,restyle_damage="Yoga")	

    struct.headers.append("yoga/Yoga.h")
    struct.headers.append("OpenGUI/Core/Math.h")
    struct.headers.append("OpenGUI/Style2/Transform.h")
    struct.headers.append("OpenGUI/Style2/Parse/Math.h")
    struct.headers.append("OpenGUI/Style2/Parse/Yoga.h")
    struct.headers.append("OpenGUI/Style2/Lerp/Math.h")
    struct.headers.append("OpenGUI/Style2/Lerp/Yoga.h")
    render_struct(struct, "shorthands/position.mako.h")

def gen_border():
    struct = make_struct("border", False)
    def add_longhand(*args, **kwargs):
        struct.add_longhand(*args, **kwargs)
    for side in PHYSICAL_SIDES:
        add_longhand("border-{0}-width".format(side), "float", "0.f"   ,restyle_damage="Yoga")
    for corner in PHYSICAL_CORNERS:
        add_longhand("border-{0}-radius".format(corner), "YGValue", "YGValueZero")
    struct.headers.append("OpenGUI/Style2/Parse/Math.h")
    struct.headers.append("OpenGUI/Style2/Parse/Yoga.h")
    struct.headers.append("OpenGUI/Style2/Lerp/Math.h")
    struct.headers.append("OpenGUI/Style2/Lerp/Yoga.h")
    struct.headers.append("yoga/Yoga.h")
    render_struct(struct, "shorthands/border.mako.h")
    
def gen_text():
    struct = make_struct("text", True)
    def add_longhand(*args, **kwargs):
        struct.add_longhand(*args, **kwargs)
    add_longhand("font-size", "float", "20.f")
    add_longhand("color", "Color4f", "Color4f(0,0,0,1)")
    struct.headers.append("OpenGUI/Style2/Parse/Math.h")
    struct.headers.append("OpenGUI/Style2/Lerp/Math.h")
    header_file = render(DEFAULT_HEADER_TEMPLATE_PATH, struct = struct)
    source_file = render(DEFAULT_SOURCE_TEMPLATE_PATH, struct = struct)
    write(struct.header_path, header_file)
    write(struct.source_path, source_file)
    render_struct(struct)


def gen_background():
    struct = make_struct("background", False)
    def add_longhand(*args, **kwargs):
        struct.add_longhand(*args, **kwargs)
    add_longhand("background-color",	"Color4f",	    "Color4f(1.f,1.f,1.f,1.f)")	
    add_longhand("background-image",	"std::string",	"{}", parser = "ParseUrl")
    struct.headers.append("OpenGUI/Core/Math.h")
    struct.headers.append("OpenGUI/Style2/Parse/Math.h")
    struct.headers.append("OpenGUI/Style2/Lerp/Math.h")
    header_file = render(DEFAULT_HEADER_TEMPLATE_PATH, struct = struct)
    source_file = render(DEFAULT_SOURCE_TEMPLATE_PATH, struct = struct)
    write(struct.header_path, header_file)
    write(struct.source_path, source_file)
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
    struct.headers.append("OpenGUI/Style2/Parse/Math.h")
    struct.headers.append("OpenGUI/Style2/Lerp/Math.h")
    struct.headers.append("OpenGUI/Style2/AnimTypes.h")
    header_template = os.path.join(BASE, "AnimStruct.mako.h")
    header_file = render(header_template, struct = struct)
    source_template = os.path.join(BASE, "AnimStruct.mako.cpp")
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