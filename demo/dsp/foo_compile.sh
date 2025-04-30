#!/bin/bash

faust -lang c -o foo.c foo.dsp

awk '
  BEGIN { inserted = 0 }
  /^#include </ && inserted == 0 {
    print "#include \"../../src/faust/architecture/faust/gui/CInterface.h\""
    inserted = 1
  }
  { print }
' foo.c > foo_patched.c

mv foo_patched.c foo.c

gcc -shared -fPIC -o libfoo.so foo.c foo_sliders.c
