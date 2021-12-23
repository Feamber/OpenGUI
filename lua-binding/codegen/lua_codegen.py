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
        self.getter = None
        self.setter = None

class FunctionDesc(object):
    def __init__(self, retType, fields, isConst):
        self.retType = retType
        self.fields = fields
        self.isConst = isConst
    def getSignature(self, record):
        return self.retType +"("+ (record.name + "::" if record else "") + "*)("+ str.join(", ",  [x.type for x in self.fields])  + ")" + ("const" if self.isConst else "")

class Function(object):
    def __init__(self, name):
        self.name = name
        self.short_name = str.rsplit(name, "::", 1)[-1]
        self.descs = []

class Record(object):
    def __init__(self, name, fields, methods, bases):
        self.name = name
        self.short_name = str.rsplit(name, "::", 1)[-1]
        self.fields = fields
        self.methods = methods
        self.bases = bases
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
    def __init__(self, name, value):
        self.name = name
        self.short_name = str.rsplit(name, "::", 1)[-1]
        self.value = value

class Enum(object):
    def __init__(self, name, enumerators):
        self.name = name
        self.short_name = str.rsplit(name, "::", 1)[-1]
        self.enumerators = enumerators

class Binding(object):
    def __init__(self):
        self.records = []
        self.name_to_record = {}
        self.functions = []
        self.enums = []
        self.headers = set()
        self.event_arg_types = {}
    def add_record(self, record):
        self.records.append(record)
        self.name_to_record[record.name] = record



def GetInclude(path):
    return path.replace("\\", "/").rsplit("include/", 1)[-1]

def main():
    meta = json.load(open(os.path.join(BASE, "../../build/meta.json")))
    db = Binding()
    for v in [  
                "float", "double", 
                "bool", "int", "int32_t", "uint32_t", "size_t", "int64_t", "uint64_t",
                "char", "OGUI::Name", "std::basic_string_view<char>", "std::basic_string<char>", "ostr::string"
            ]:
        db.event_arg_types[v] = True
    for key, value in meta["records"].items():
        file = value["fileName"]
        isEvent = "event" in value["attrs"] or "event-data" in value["attrs"]
        fields = []
        for key2, value2 in value["fields"].items():
            if isEvent:
                db.event_arg_types[value2["type"]] = True
            attr = value2["attrs"]
            if not "script" in attr:
                continue
            field = Field(key2, value2["type"])
            fields.append(field)
        functions = parseFunctions(value["methods"])
        bases = []
        for value3 in value["bases"]:
            if value3 in db.name_to_record:
                bases.append(db.name_to_record[value3])
        if fields or functions or bases:
            if str.endswith(file, ".cpp"):
                print("unable to gen lua bind for records in cpp, name:%s" % key, file=sys.stderr)
                continue
            db.event_arg_types[key+"*"]=True
            db.headers.add(GetInclude(file))
            db.add_record(Record(key, fields, functions, bases))
    for key, value in meta["enums"].items():
        attr = value["attrs"]
        if not "script" in attr:
            continue
        file = value["fileName"]
        if str.endswith(file, ".cpp"):
            print("unable to gen lua bind for records in cpp, name:%s" % value["name"], file=sys.stderr)
            continue
        db.headers.add(GetInclude(file))
        enumerators = []
        for key2, value2 in value["values"].items():
            enumerators.append(Enumerator(key2, value2["value"]))
        db.event_arg_types[key]=True
        db.enums.append(Enum(key, enumerators))
        
    db.functions = parseFunctions(meta["functions"], headers=db.headers)
    
    template = os.path.join(BASE, "luaBind.cpp.mako")
    content = render(template, db = db)
    output = os.path.join(BASE, "../source/luaBind.generated.cpp")
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

def parseFunctions(dict, headers = None):
    functionsDict = {}
    for value in dict:
        attr = value["attrs"]
        name = value["name"]
        if not "script" in attr:
            continue
        if not (headers is None):
            file = value["fileName"]
            if str.endswith(file, ".cpp"):
                print("unable to gen lua bind for records in cpp, name:%s" % name, file=sys.stderr)
                continue
            headers.add(GetInclude(file))
        function = functionsDict.setdefault(name, Function(name))
        fields = []
        for key2, value2 in value["parameters"].items():
            field = Field(key2, value2["type"])
            fields.append(field)
        function.descs.append(FunctionDesc(value["retType"], fields, value["isConst"]))
    return functionsDict

if __name__ == "__main__":
    main()