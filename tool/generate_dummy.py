import glob

if __name__ == "__main__":
    headers = glob.glob("../include/OpenGUI/**/*.h", recursive=True)
    dummy = str.join("\n", ["#include \""+header.replace("../include/", "")+"\"" for header in headers])
    open("Dummy.cpp", "wb").write(dummy.encode("utf-8"))