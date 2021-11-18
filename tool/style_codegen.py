
import re
import os.path
import sys

from mako import exceptions
from mako.lookup import TemplateLookup
from mako.template import Template

PHYSICAL_SIDES = ["left", "top", "right", "bottom"]
LOGICAL_SIDES = ["block-start", "block-end", "inline-start", "inline-end"]
PHYSICAL_SIZES = ["width", "height"]
LOGICAL_SIZES = ["block-size", "inline-size"]
PHYSICAL_CORNERS = ["top-left", "top-right", "bottom-right", "bottom-left"]
LOGICAL_CORNERS = ["start-start", "start-end", "end-start", "end-end"]
PHYSICAL_AXES = ["x", "y"]
LOGICAL_AXES = ["inline", "block"]

# bool is True when logical
ALL_SIDES = [(side, False) for side in PHYSICAL_SIDES] + [
    (side, True) for side in LOGICAL_SIDES
]
ALL_SIZES = [(size, False) for size in PHYSICAL_SIZES] + [
    (size, True) for size in LOGICAL_SIZES
]
ALL_CORNERS = [(corner, False) for corner in PHYSICAL_CORNERS] + [
    (corner, True) for corner in LOGICAL_CORNERS
]
ALL_AXES = [(axis, False) for axis in PHYSICAL_AXES] + [
    (axis, True) for axis in LOGICAL_AXES
]

SYSTEM_FONT_LONGHANDS = """font_family font_size font_style
                           font_variant_caps font_stretch font_kerning
                           font_variant_position font_weight
                           font_size_adjust font_variant_alternates
                           font_variant_ligatures font_variant_east_asian
                           font_variant_numeric font_language_override
                           font_feature_settings font_variation_settings
                           font_optical_sizing""".split()

# Bitfield values for all rule types which can have property declarations.
STYLE_RULE = 1 << 0
PAGE_RULE = 1 << 1
KEYFRAME_RULE = 1 << 2

ALL_RULES = STYLE_RULE | PAGE_RULE | KEYFRAME_RULE
DEFAULT_RULES = STYLE_RULE | KEYFRAME_RULE
DEFAULT_RULES_AND_PAGE = DEFAULT_RULES | PAGE_RULE
DEFAULT_RULES_EXCEPT_KEYFRAME = STYLE_RULE


# Rule name to value dict
RULE_VALUES = {
    "Style": STYLE_RULE,
    "Page": PAGE_RULE,
    "Keyframe": KEYFRAME_RULE,
}

def rule_values_from_arg(that):
    if isinstance(that, int):
        return that
    mask = 0
    for rule in that.split():
        mask |= RULE_VALUES[rule]
    return mask

def to_snake_case(ident):
    return re.sub("([A-Z]+)", lambda m: "_" + m.group(1).lower(), ident).strip("_")


def to_small_camel_case(ident):
    first = True
    def cast(m):
        nonlocal first
        if first:
            first = False
            return m.group(2)
        return m.group(2).upper()
    return re.sub(
        "(^|_|-)([a-z0-9])", cast, ident.strip("_").strip("-")
    )

def to_camel_case(ident):
    return re.sub(
        "(^|_|-)([a-z0-9])", lambda m: m.group(2).upper(), ident.strip("_").strip("-")
    )

def parse_property_aliases(alias_list):
    result = []
    if alias_list:
        for alias in alias_list.split():
            (name, _, pref) = alias.partition(":")
            result.append((name, pref))
    return result

class Keyword(object):
    def __init__(
        self,
        name,
        values
    ):
        self.name = name
        self.values = values.split()

class Property(object):
    def __init__(
        self,
        name,
        spec,
        rule_types_allowed,
        aliases,
        flags
    ):
        self.name = name
        self.spec = spec
        self.ident = to_small_camel_case(name)
        self.rule_types_allowed = rule_values_from_arg(rule_types_allowed)
        self.aliases = parse_property_aliases(aliases)
        self.flags = flags.split() if flags else []


def arg_to_bool(arg):
    if isinstance(arg, bool):
        return arg
    assert arg in ["True", "False"], "Unexpected value for boolean arguement: " + repr(
        arg
    )
    return arg == "True"

import ctypes

class Longhand(Property):
    def __init__(
        self,
        name,
        type,
        initial_value,
        spec=None,
        keyword=None,
        rule_types_allowed=DEFAULT_RULES,
        logical=False,
        aliases=None,
        flags=None,
        vector=False,
        restyle_damage=None,
        parser = "ParseValue"
    ):
        Property.__init__(
            self,
            name=name,
            spec=spec,
            rule_types_allowed=rule_types_allowed,
            aliases=aliases,
            flags=flags
        )

        self.keyword = keyword
        self.type = type
        self.initial_value = initial_value
        self.logical = arg_to_bool(logical)
        self.is_vector = arg_to_bool(vector)
        if self.is_vector:
            self.view_type = "gsl::span<{}>".format(type)
            self.storage_type = "std::vector<{}>".format(type)
        else:
            self.view_type = type
            self.storage_type = type
        self.restyle_damage = restyle_damage
        self.parser = parser

class Shorthand(Property):
    def __init__(
        self,
        name,
        sub_properties,
        spec=None,
        rule_types_allowed=DEFAULT_RULES,
        aliases=None,
        flags=None
    ):
        Property.__init__(
            self,
            name=name,
            spec=spec,
            rule_types_allowed=rule_types_allowed,
            aliases=aliases,
            flags = flags
        )
        self.sub_properties = sub_properties


class StyleStruct(object):
    def __init__(self, name, inherited, header_dir, source_dir, include_dir):
        self.name = name
        self.name_lower = to_snake_case(name)
        self.ident = to_camel_case(self.name_lower)
        self.longhands = []
        self.shorthands = []
        self.headers = []
        self.inherited = inherited
        self.name_to_longhand = {}
        self.header_path = os.path.join(header_dir, self.name + ".h")
        self.source_path = os.path.join(source_dir, self.name + ".cpp")
        self.shorthand_path = os.path.join(source_dir, self.name + "_shorthands.h")
        self.include_path = os.path.relpath(self.header_path, include_dir).replace(os.path.sep, '/')

    def add_longhand(self, *args, **kwargs):
        longhand = Longhand(*args, **kwargs)
        self.longhands.append(longhand)
        self.name_to_longhand[longhand.name] = longhand

    def add_shorthand(self, name, sub_properties, *args, **kwargs):
        sub_properties = [self.name_to_longhand[s] for s in sub_properties]
        shorthand = Shorthand(name, sub_properties, *args, **kwargs)
        self.shorthands.append(shorthand)

        
BASE = os.path.dirname(os.path.realpath(__file__).replace(os.path.sep, "/"))
ROOT = os.path.join(BASE, "../")
DEFAULT_HEADER_TEMPLATE_PATH = os.path.join(ROOT, "include/OpenGUI/Style2/mako/Struct.h.mako")
DEFAULT_SOURCE_TEMPLATE_PATH = os.path.join(ROOT, "include/OpenGUI/Style2/mako/Struct.cpp.mako")
class renderer(object):
    def __init__(self, base):
        self.base = base

    def abort(self, message):
        print(message, file=sys.stderr)
        sys.exit(1)
        
    def render(self, filename, **context):
        try:
            lookup = TemplateLookup(
                directories=[self.base], input_encoding="utf8", strict_undefined=True
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
            self.abort(exceptions.text_error_template().render())

            
    def write(self, path, content):
        RE_PYTHON_ADDR = re.compile(r"<.+? object at 0x[0-9a-fA-F]+>")
        directory = os.path.dirname(path)
        if not os.path.exists(directory):
            os.makedirs(directory)
        open(path, "wb").write(content.encode("utf-8"))

        python_addr = RE_PYTHON_ADDR.search(content)
        if python_addr:
            self.abort('Found "{}" in {} ({})'.format(python_addr.group(0), os.path.basename(path), path))