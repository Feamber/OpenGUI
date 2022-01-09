import glob
import sys
from os import path
BASE = path.dirname(path.realpath(__file__).replace("\\", "/"))

if __name__ == "__main__":
    headers = glob.glob("../include/OpenGUI/**/*.h", recursive=True)
    dummy = str.join("\n", ["#include \""+header.replace("../include/", "")+"\"" for header in headers])
    open(path.join(BASE, "../source/Dummy/Dummy.cpp"), "wb").write(dummy.encode("utf-8"))