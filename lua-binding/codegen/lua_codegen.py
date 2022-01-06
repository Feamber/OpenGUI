import json
import os
import sys
import re
from mako import exceptions
from mako.template import Template
from pathlib import Path

BASE = os.path.dirname(os.path.realpath(__file__).replace("\\", "/"))

def mysplit(string:str):
    result = []
    s = 0
    i = 0
    ln = len(string)
    c = 0
    while i < ln:
        if string[i]=="<":
            c=c+1
        elif string[i]==">" and c>0:
            c=c-1
        elif string[i]=="," and c==0:
            result.append(str(string[s : i]).strip())
            s = i+1
        i = i+1
    if s != i:
        result.append(string[s : i].strip())
    return result

    


def lua_type_name(name : str):
    name = str(name)
    if name == "float" or name == "double":
        return "number"
    if name == "int" or  name == "size_t" or name == "uint32_t" \
        or name == "int32_t" or name == "uint":
        return "integer"
    if name.startswith("std::string") or name == "ostr::string" or name == "char":
        return "string"
    name = name.replace("::", ".")
    i = name.find("<")
    if i!=-1 :
        return name[0 : i] + "<" + str.join(",", [lua_type_name(inner) for inner in mysplit(name[i+1 : len(name)-1])]) + ">"
    return name

class Field(object):
    def __init__(self, name, type, rawType, comment):
        self.name = name
        self.type = type
        self.luaType = lua_type_name(rawType)
        self.getter = None
        self.setter = None
        self.comment = comment

def converted(str):
    if str == "ostr::string_view":
        return "ostr::string"
    return str
class FunctionDesc(object):
    def __init__(self, retType, rawRetType, fields, isConst, comment):
        self.retType = retType
        self.luaRetType = lua_type_name(rawRetType)
        self.fields = fields
        self.isConst = isConst
        self.comment = comment
    def getReference(self, record, function):
        if "ostr::string_view" in [x.type for x in self.fields]:
            if record:
                return "+[]({}* self, {}) {{ return self->{}({}); }}".format(record.name, str.join(", ",  [converted(x.type) + " _" + str(i) for i, x in enumerate(self.fields)]), function.short_name, str.join(", ",  ["_" + str(i) for i, x in enumerate(self.fields)]))
            else:
                return "+[]({}) {{ return {}({}); }}".format(str.join(", ",  [converted(x.type) + " _" + str(i) for i, x in enumerate(self.fields)]), function.name, str.join(", ",  ["_" + str(i) for i, x in enumerate(self.fields)]))
        else:
            return "({}({}*)({}){})&{}".format(self.retType, record.name + "::" if record else "", str.join(", ",  [x.type for x in self.fields]), "const" if self.isConst else "", function.name)


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
        self.luaName = lua_type_name(name)
        self.short_name = str.rsplit(name, "::", 1)[-1]
        self.enumerators = enumerators
        self.comment = comment

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
    function_by_file = {}
    db = Binding()
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
            field = Field(key2, value2["type"], value2["rawType"], value2["comment"])
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
            db.add_record(Record(key, fields, functions, bases, value["comment"]))
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
            enumerators.append(Enumerator(key2, value2["value"], value2["comment"]))
        db.event_arg_types[key]=True
        db.enums.append(Enum(key, enumerators, value["comment"]))
        
    db.functions = parseFunctions(meta["functions"], headers=db.headers, function_by_file=function_by_file)
    template = os.path.join(BASE, "luaBind.cpp.mako")
    content = render(template, db = db)
    output = os.path.join(BASE, "../source/luaBind.generated.cpp")
    write(output, content)

    for record in db.records:
        template = os.path.join(BASE, "RecordIntelliSense.lua.mako")
        content = render(template, record = record)
        output = os.path.join(BASE, "../IntelliSense/%s.lua"%record.name.replace("::", "_"))
        write(output, content)
    for enum in db.enums:
        template = os.path.join(BASE, "EnumIntelliSense.lua.mako")
        content = render(template, enum = enum)
        output = os.path.join(BASE, "../IntelliSense/%s.lua"%enum.name.replace("::", "_"))
        write(output, content)
    for file, functions in function_by_file.items():
        template = os.path.join(BASE, "FunctionIntelliSense.lua.mako")
        content = render(template, functions = functions)
        output = os.path.join(BASE, "../IntelliSense/%s.lua"%(file.replace("\\", "_")))
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

def parseFunctions(dict, headers = None, function_by_file = None):
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
            field = Field(key2, value2["type"], value2["rawType"], value2["comment"])
            fields.append(field)
        f = FunctionDesc(value["retType"], value["rawRetType"], fields, value["isConst"], value["comment"])
        function.descs.append(f)
        if not (function_by_file is None):
            path = GetInclude(file)
            function_by_file.setdefault(path, set())
            function_by_file[path].add(function)
    return functionsDict

if __name__ == "__main__":
    main()