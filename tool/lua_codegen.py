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

class FunctionDesc(object):
    def __init__(self, retType, fields):
        self.retType = retType
        self.fields = fields
        self.signature = retType +"(*)("+ str.join(", ",  [x.type for x in fields]) + ")"

class Function(object):
    def __init__(self, name):
        self.name = name
        self.short_name = str.rsplit(name, "::", 1)[-1]
        self.descs = []
        self.overloaded = False

class Record(object):
    def __init__(self, name, fields, methods):
        self.name = name
        self.short_name = str.rsplit(name, "::", 1)[-1]
        self.fields = fields
        self.methods = methods
        self.bases = []

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
    def add_record(self, record, bases):
        self.records.append(record)
        self.name_to_record[record.name] = record
        for base in bases:
            if base in self.name_to_record:
                record.bases.append(self.name_to_record[base])
            else:
                print("base class {} of reflected class {} is not reflected!".format(record.name, base))



def GetInclude(path):
    return path.replace("\\", "/").rsplit("include/", 1)[-1]

def main():
    meta = json.load(open(os.path.join(BASE, "../build/meta.json")))
    db = Binding()
    for key, value in meta["records"].items():
        file = value["fileName"]
        fields = []
        for key2, value2 in value["fields"].items():
            attr = value2["attrs"]
            if not "script" in attr:
                continue
            field = Field(key2, value2["type"])
            fields.append(field)
        functions = parseFunctions(value["methods"])
        if fields or functions:
            if str.endswith(file, ".cpp"):
                print("unable to gen lua bind for records in cpp, name:%s" % key, file=sys.stderr)
                continue
            db.headers.add(GetInclude(file))
            db.add_record(Record(key, fields, functions), value["bases"])
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
        db.enums.append(Enum(key, enumerators))
        
    db.functions = parseFunctions(meta["functions"], headers=db.headers)
    
    template = os.path.join(BASE, "luaBind.cpp.mako")
    content = render(template, db = db)
    output = os.path.join(BASE, "luaBind.cpp")
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
    overloadDict = {}
    for value in dict:
        attr = value["attrs"]
        name = value["name"]
        overloadDict.setdefault(name, 0)
        overloadDict[name] = overloadDict[name]+1
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
        function.descs.append(FunctionDesc(value["retType"], fields))
    for k,v in overloadDict.items():
        if v > 1 and k in functionsDict:
            functionsDict[k].overloaded = True
    return [v for v in functionsDict.values()]
    

if __name__ == "__main__":
    main()