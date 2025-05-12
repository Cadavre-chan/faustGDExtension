#!/bin/bash
set -e
# faust  -o foo.cpp foo.dsp

faust -a godot_arch.cpp -cn mydsp -i -o foo.cpp foo.dsp


# awk '
#   BEGIN { inserted = 0 }
#   /^#include </ && inserted == 0 {
#     print "#include \"faust/architecture/faust/gui/CInterface.h\""
#     inserted = 1
#   }
#   { print }
# ' foo.cpp > foo_patched.cpp

# mv foo_patched.cpp foo.cpp

# awk '
#   BEGIN { inserted = 0 }
#   /^#include </ && inserted == 0 {
#     print "#include \"faust/dsp/dsp.h\""
#     inserted = 1
#   }
#   { print }
# ' foo.cpp > foo_patched.cpp

# mv foo_patched.cpp foo.cpp

g++ -std=c++17 -shared -fPIC -o libfoo.so foo.cpp
