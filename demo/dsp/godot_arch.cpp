#include <faust/dsp/dsp.h>

#include <faust/gui/MapUI.h>

#include <faust/gui/meta.h>

<<includeIntrinsic>>
<<includeclass>>

extern "C" dsp* newmydsp() {
    return new mydsp();
}