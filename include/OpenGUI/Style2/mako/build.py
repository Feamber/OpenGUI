
from mako import exceptions
from mako.lookup import TemplateLookup
from mako.template import Template

import json
import os.path
import re
import sys

from data import PHYSICAL_CORNERS, PHYSICAL_SIDES, PHYSICAL_SIZES, Longhand, Shorthand, StyleStruct
import data


BASE = os.path.dirname(__file__.replace("\\", "/"))
ROOT = os.path.join(BASE, "../../../..")
sys.path.insert(0, os.path.join(BASE, "Mako-1.1.2-py2.py3-none-any.whl"))
sys.path.insert(0, BASE)  # For importing `data.py`
HEADER_OUT_DIR = os.path.join(ROOT, "include/OpenGUI/Style2/generated")
SOURCE_OUT_DIR = os.path.join(ROOT, "source/Style2/generated")

def gen_position():
    struct = StyleStruct("position", False)
    def add_longhand(*args, **kwargs):
        struct.add_longhand(*args, **kwargs)
    add_longhand("transform",       "ComputedTransform",            "ComputedTransform::ident()"),
    add_longhand("flex-grow",       "float",            "0.f")
    add_longhand("flex-shrink",     "float",            "1.f")
    add_longhand("flex-basis",      "YGValue",          "YGValueAuto")
    for side in PHYSICAL_SIDES:
        add_longhand(side, "YGValue", "YGValueAuto")
    for side in PHYSICAL_SIDES:
        add_longhand("margin-{0}".format(side), "YGValue", "YGValueZero")
    for side in PHYSICAL_SIDES:
        add_longhand("padding-{0}".format(side), "YGValue", "YGValueZero")
    add_longhand("width",			"YGValue",			"YGValueAuto")		
    add_longhand("height",			"YGValue",			"YGValueAuto")		
    add_longhand("position",		"YGPositionType",	"YGPositionTypeRelative")
    add_longhand("overflow",		"YGOverflow",		"YGOverflowVisible")
    add_longhand("align-self",		"YGAlign",			"YGAlignAuto")		
    for size in PHYSICAL_SIZES:
        add_longhand("max-{0}".format(size), "YGValue", "YGValueUndefined")	
    for size in PHYSICAL_SIZES:
        add_longhand("min-{0}".format(size), "YGValue", "YGValueAuto")	
    add_longhand("flex-direction",	"YGFlexDirection",	"YGFlexDirectionRow")
    add_longhand("align-content",	"YGAlign",			"YGAlignFlexStart")	
    add_longhand("align-items",		"YGAlign",			"YGAlignStretch")	
    add_longhand("justify-content",	"YGJustify",		"YGJustifyFlexStart")
    add_longhand("flex-wrap",		"YGWrap",			"YGWrapNoWrap")		
    add_longhand("flex-display",	"YGDisplay",		"YGDisplayFlex")	
    shorthand_template = os.path.join(BASE, "shorthands/position.mako.h")
    shorthand_file = render(shorthand_template, struct = struct,  data = data)

    struct.headers.append("yoga/Yoga.h")
    struct.headers.append("OpenGUI/Core/Math.h")
    struct.headers.append("OpenGUI/Style2/Parse/Math.h")
    struct.headers.append("OpenGUI/Style2/Parse/Yoga.h")
    struct.headers.append("OpenGUI/Style2/Lerp/Math.h")
    struct.headers.append("OpenGUI/Style2/Lerp/Yoga.h")
    header_template = os.path.join(BASE, "Struct.mako.h")
    header_file = render(header_template, struct = struct,  data = data)
    source_template = os.path.join(BASE, "Struct.mako.cpp")
    source_file = render(source_template, struct = struct,  data = data)
    write(HEADER_OUT_DIR, "position.h", header_file)
    write(SOURCE_OUT_DIR, "position_shorthands.h", shorthand_file)
    write(SOURCE_OUT_DIR, "position.cpp", source_file)

def gen_border():
    struct = StyleStruct("border", False)
    def add_longhand(*args, **kwargs):
        struct.add_longhand(*args, **kwargs)
    for side in PHYSICAL_SIDES:
        add_longhand("border-{0}-width".format(side), "float", "0.f")
    for corner in PHYSICAL_CORNERS:
        add_longhand("border-{0}-radius".format(corner), "YGValue", "YGValueZero")
    shorthand_template = os.path.join(BASE, "shorthands/border.mako.h")
    shorthand_file = render(shorthand_template, struct = struct,  data = data)
    struct.headers.append("OpenGUI/Style2/Parse/Math.h")
    struct.headers.append("OpenGUI/Style2/Parse/Yoga.h")
    struct.headers.append("OpenGUI/Style2/Lerp/Math.h")
    struct.headers.append("OpenGUI/Style2/Lerp/Yoga.h")
    struct.headers.append("yoga/Yoga.h")
    header_template = os.path.join(BASE, "Struct.mako.h")
    header_file = render(header_template, struct = struct,  data = data)
    source_template = os.path.join(BASE, "Struct.mako.cpp")
    source_file = render(source_template, struct = struct,  data = data)
    write(HEADER_OUT_DIR, "border.h", header_file)
    write(SOURCE_OUT_DIR, "border_shorthands.h", shorthand_file)
    write(SOURCE_OUT_DIR, "border.cpp", source_file)
    
def gen_text():
    struct = StyleStruct("text", True)
    def add_longhand(*args, **kwargs):
        struct.longhands.append(Longhand(*args, **kwargs))
    add_longhand("font-size", "float", "20.f")
    struct.headers.append("OpenGUI/Style2/Parse/Math.h")
    struct.headers.append("OpenGUI/Style2/Lerp/Math.h")
    header_template = os.path.join(BASE, "Struct.mako.h")
    header_file = render(header_template, struct = struct,  data = data)
    source_template = os.path.join(BASE, "Struct.mako.cpp")
    source_file = render(source_template, struct = struct,  data = data)
    write(HEADER_OUT_DIR, "text.h", header_file)
    write(SOURCE_OUT_DIR, "text.cpp", source_file)


def gen_background():
    struct = StyleStruct("background", False)
    def add_longhand(*args, **kwargs):
        struct.longhands.append(Longhand(*args, **kwargs))
    add_longhand("background-color",	"Color4f",	    "Color4f(1.f,1.f,1.f,1.f)")	
    add_longhand("background-image",	"std::string",	"{}", parser = "ParseUrl")
    struct.headers.append("OpenGUI/Core/Math.h")
    struct.headers.append("OpenGUI/Style2/Parse/Math.h")
    struct.headers.append("OpenGUI/Style2/Lerp/Math.h")
    header_template = os.path.join(BASE, "Struct.mako.h")
    header_file = render(header_template, struct = struct,  data = data)
    source_template = os.path.join(BASE, "Struct.mako.cpp")
    source_file = render(source_template, struct = struct,  data = data)
    write(HEADER_OUT_DIR, "background.h", header_file)
    write(SOURCE_OUT_DIR, "background.cpp", source_file)

def gen_animation():
    struct = StyleStruct("animation", False)
    def add_longhand(*args, **kwargs):
        struct.longhands.append(Longhand(*args, **kwargs))
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
    header_file = render(header_template, struct = struct,  data = data)
    source_template = os.path.join(BASE, "AnimStruct.mako.cpp")
    source_file = render(source_template, struct = struct,  data = data)
    write(HEADER_OUT_DIR, "animation.h", header_file)
    write(SOURCE_OUT_DIR, "animation.cpp", source_file)



def main():
    gen_position()
    gen_border()
    gen_text()
    gen_background()
    gen_animation()


def abort(message):
    print(message, file=sys.stderr)
    sys.exit(1)
    
def render(filename, **context):
    try:
        lookup = TemplateLookup(
            directories=[BASE], input_encoding="utf8", strict_undefined=True
        )
        template = Template(
            open(filename, "rb").read(),
            filename=filename,
            input_encoding="utf8",
            lookup=lookup,
            strict_undefined=True,
        )
        # Uncomment to debug generated Python code:
        # write("/tmp", "mako_%s.py" % os.path.basename(filename), template.code)
        return template.render(**context)
    except Exception:
        # Uncomment to see a traceback in generated Python code:
        # raise
        abort(exceptions.text_error_template().render())


RE_PYTHON_ADDR = re.compile(r"<.+? object at 0x[0-9a-fA-F]+>")
        
def write(directory, filename, content):
    if not os.path.exists(directory):
        os.makedirs(directory)
    full_path = os.path.join(directory, filename)
    open(full_path, "w", encoding="utf-8").write(content)

    python_addr = RE_PYTHON_ADDR.search(content)
    if python_addr:
        abort('Found "{}" in {} ({})'.format(python_addr.group(0), filename, full_path))

if __name__ == "__main__":
    main()