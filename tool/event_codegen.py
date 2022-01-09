import json
import os
import sys
import re
from mako import exceptions
from mako.template import Template
from pathlib import Path

BASE = os.path.dirname(os.path.realpath(__file__).replace("\\", "/"))

class Field(object):
    def __init__(self, name, type):
        self.name = name
        self.type = type

class Record(object):
    def __init__(self, name, fields, bases, isEvent):
        self.name = name
        self.short_name = str.rsplit(name, "::", 1)[-1]
        self.event_name = re.sub("([A-Z]+)", lambda m: "-" + m.group(1).lower(), self.short_name.replace("Event", "")).replace("_", "-").strip("-")
        self.fields = fields
        self.bases = bases
        self.isEvent = isEvent

class Binding(object):
    def __init__(self):
        self.records = []
        self.events = []
        self.headers = set()

def GetInclude(path):
    return path.replace("\\", "/").rsplit("include/", 1)[-1]

def main():
    meta = json.load(open(os.path.join(BASE, "../build/meta.json")))
    db = Binding()
    for key, value in meta["records"].items():
        attr = value["attrs"]
        if not "event" in attr and not "event-data" in attr:
            continue
        file = value["fileName"]
        fields = []
        for key2, value2 in value["fields"].items():
            field = Field(key2, value2["type"])
            fields.append(field)
        db.headers.add(GetInclude(file))
        record = Record(key, fields, value["bases"], "event" in attr)
        if record.isEvent:
            db.events.append(record)
        db.records.append(record)
    template = os.path.join(BASE, "EventArgWrapper.cpp.mako")
    content = render(template, db = db)
    output = os.path.join(BASE, "../source/EventArgWrapper.cpp")
    write(output, content)
    template = os.path.join(BASE, "EventArgWrapper.h.mako")
    content = render(template, db = db)
    output = os.path.join(BASE, "../include/OpenGUI/Bind/EventArgWrapper.h")
    write(output, content)
    template = os.path.join(BASE, "EventName.cpp.mako")
    content = render(template, db = db)
    output = os.path.join(BASE, "../source/EventName.cpp")
    write(output, content)
    template = os.path.join(BASE, "EventName.h.mako")
    content = render(template, db = db)
    output = os.path.join(BASE, "../include/OpenGUI/Event/EventName.h")
    write(output, content)

def render(filename, **context):
    try:
        template = Template(
            open(filename, "rb").read(),
            filename=filename,
            input_encoding="utf8",
            strict_undefined=True,
        )
        return template.render(**context)
    except Exception:
        # Uncomment to see a traceback in generated Python code:
        # raise
        abort(exceptions.text_error_template().render())

def write(path, content):
    RE_PYTHON_ADDR = re.compile(r"<.+? object at 0x[0-9a-fA-F]+>")
    directory = os.path.dirname(path)
    if not os.path.exists(directory):
        os.makedirs(directory)
    open(path, "wb").write(content.encode("utf-8"))

    python_addr = RE_PYTHON_ADDR.search(content)
    if python_addr:
        abort('Found "{}" in {} ({})'.format(python_addr.group(0), os.path.basename(path), path))

def abort(message):
        print(message, file=sys.stderr)
        sys.exit(1)
    
if __name__ == "__main__":
    main()