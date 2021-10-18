
from mako import exceptions
from mako.lookup import TemplateLookup
from mako.template import Template

import json
import os.path
import re
import sys

from include.OpenGUI.Style2.mako.data import Longhand, StyleStruct


BASE = os.path.dirname(__file__.replace("\\", "/"))
sys.path.insert(0, os.path.join(BASE, "Mako-1.1.2-py2.py3-none-any.whl"))
sys.path.insert(0, BASE)  # For importing `data.py`
OUT_DIR = os.path.join(BASE, "generated")

import data

STYLE_STRUCT_LIST = [
    StyleStruct("position", True, 
    [
        Longhand("transform", "Transform", "{}"),
        Longhand("rotate", "float", "{}"),
        Longhand("scale", "Vector2f", "{}"),
        Longhand("translate", "Vector2f", "{}"),
    ])
]

def main():
    files = {}
    for kind in ["longhands", "shorthands"]:
        files[kind] = {}
        for struct in STYLE_STRUCT_LIST:
            file_name = os.path.join(BASE, kind, "{}.mako.h".format(struct))
            if kind == "shorthands" and not os.path.exists(file_name):
                files[kind][struct] = ""
                continue
            files[kind][struct] = render(file_name, data=data.PropertiesData())
    for kind in ["longhands", "shorthands"]:
        for struct in files[kind]:
            write(
                os.path.join(OUT_DIR, kind),
                "{}.h".format(struct),
                files[kind][struct],
            )


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