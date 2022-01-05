import json
import os
import sys
import re
from mako import exceptions
from mako.template import Template
from pathlib import Path

BASE = os.path.dirname(os.path.realpath(__file__).replace("\\", "/"))

class Field(object):
    def __init__(self, name, type, offset, comment):
        self.name = name
        self.type = type
        self.getter = None
        self.setter = None
        self.offset = offset
        self.comment = comment

class FunctionDesc(object):
    def __init__(self, retType, fields, isConst, comment):
        self.retType = retType
        self.fields = fields
        self.isConst = isConst
        self.comment = comment
    def getCall(self):
        return ", ".join(["args["+str(i)+"].Convert<"+field.type+">()" for i, field in enumerate(self.fields)])
    def getSignature(self, record):
        return self.retType +"("+ (record.name + "::" if record else "") + "*)("+ str.join(", ",  [x.type for x in self.fields])  + ")" + ("const" if self.isConst else "")

class Function(object):
    def __init__(self, name):
        self.name = name
        self.short_name = str.rsplit(name, "::", 1)[-1]
        self.descs = []

class Record(object):
    def __init__(self, name, fields, methods, bases, comment):
        self.name = name
        self.luaName = name.replace("::", ".")
        self.short_name = str.rsplit(name, "::", 1)[-1]
        self.fields = fields
        self.methods = methods
        self.bases = bases
        self.comment = comment
        self.hashable = False
    def allFields(self):
        result = []
        result.extend(self.fields)
        for base in self.bases:
            result.extend(base.allFields())
        return result
    def allMethods(self):
        result = dict(self.methods)
        for base in self.bases:
            for k, v in base.allMethods().items():
                function = result.setdefault(k, Function(k))
                function.descs.extend(v.descs)
        return result

class Enumerator(object):
    def __init__(self, name, value, comment):
        self.name = name
        self.short_name = str.rsplit(name, "::", 1)[-1]
        self.value = value
        self.comment = comment

class Enum(object):
    def __init__(self, name, enumerators, comment):
        self.name = name
        self.short_name = str.rsplit(name, "::", 1)[-1]
        self.enumerators = enumerators
        self.comment = comment

class Binding(object):
    def __init__(self):
        self.records = []
        self.enums = []
        self.name_to_record = {}
        self.headers = set()
    def add_record(self, record):
        self.records.append(record)
        self.name_to_record[record.name] = record



def GetInclude(path):
    return path.replace("\\", "/").rsplit("include/", 1)[-1]

def main():
    meta = json.load(open(os.path.join(BASE, "../build/meta.json")))
    db = Binding()
    for key, value in meta["records"].items():
        if not "rtti" in value["attrs"]:
            continue
        file = value["fileName"]
        fields = []
        for key2, value2 in value["fields"].items():
            attr = value2["attrs"]
            if not "rtti" in attr:
                continue
            field = Field(key2, value2["type"], value["offset"], value2["comment"])
            fields.append(field)
        functions = parseFunctions(value["methods"])
        bases = []
        for value3 in value["bases"]:
            if value3 in db.name_to_record:
                bases.append(db.name_to_record[value3])
        if str.endswith(file, ".cpp"):
            print("unable to gen rtti for records in cpp, name:%s" % key, file=sys.stderr)
            continue
        db.headers.add(GetInclude(file))
        db.add_record(Record(key, fields, functions, bases, value["comment"]))
    for key, value in meta["enums"].items():
        attr = value["attrs"]
        if not "rtti" in attr:
            continue
        file = value["fileName"]
        if str.endswith(file, ".cpp"):
            print("unable to gen rtti for enums in cpp, name:%s" % value["name"], file=sys.stderr)
            continue
        db.headers.add(GetInclude(file))
        enumerators = []
        for key2, value2 in value["values"].items():
            enumerators.append(Enumerator(key2, value2["value"], value2["comment"]))
        db.enums.append(Enum(key, enumerators, value["comment"]))
    
    template = os.path.join(BASE, "rtti.cpp.mako")
    content = render(template, db = db)
    output = os.path.join(BASE, "../source/rtti.generated.cpp")
    write(output, content)
    template = os.path.join(BASE, "rtti.h.mako")
    content = render(template, db = db)
    output = os.path.join(BASE, "../include/OpenGUI/rtti.generated.h")
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

def parseFunctions(dict):
    functionsDict = {}
    for value in dict:
        attr = value["attrs"]
        name = value["name"]
        if not "rtti" in attr:
            continue
        function = functionsDict.setdefault(name, Function(name))
        fields = []
        for key2, value2 in value["parameters"].items():
            field = Field(key2, value2["type"], value2["offset"], value2["comment"])
            fields.append(field)
        f = FunctionDesc(value["retType"], fields, value["isConst"], value["comment"])
        function.descs.append(f)
    return functionsDict

if __name__ == "__main__":
    main()