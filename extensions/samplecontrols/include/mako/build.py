import json
import os.path
import re
import sys

BASE = os.path.dirname(__file__.replace("\\", "/"))
ROOT = os.path.join(BASE, "../../../..")

sys.path.insert(0, ROOT)
from tool.style_codegen import PHYSICAL_CORNERS, PHYSICAL_SIDES, PHYSICAL_SIZES, StyleStruct, renderer, DEFAULT_HEADER_TEMPLATE_PATH, DEFAULT_SOURCE_TEMPLATE_PATH
HEADER_OUT_DIR = os.path.join(ROOT, "extensions/samplecontrols/include/generated")
SOURCE_OUT_DIR = os.path.join(ROOT, "extensions/samplecontrols/source/generated")
INCLUDE_DIR = os.path.join(ROOT, "extensions/samplecontrols/include")
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

def gen_sample():
    struct = make_struct("sample", False)
    def add_longhand(*args, **kwargs):
        struct.add_longhand(*args, **kwargs)
    add_longhand("some-value", "float", "0.5f")
    struct.headers.append("OpenGUI/Style2/Parse/Math.h")
    struct.headers.append("OpenGUI/Style2/Lerp/Math.h")
    render_struct(struct)

def main():
    gen_sample()

if __name__ == "__main__":
    main()