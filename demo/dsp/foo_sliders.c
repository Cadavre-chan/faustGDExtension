#ifndef FOO_SLIDERS_C
#define FOO_SLIDERS_C

#include <stdlib.h>

// Forward declare the DSP struct
typedef struct mydsp {
    float fHslider0;  // drive slider
    float fHslider1;  // offset slider
    int fSampleRate;  // (can be omitted if unused here)
} mydsp;

// Gain access helpers
float* getDrivePointer(mydsp* dsp) {
    return &dsp->fHslider0;
}

float* getOffsetPointer(mydsp *dsp) {
    return &dsp->fHslider1;
}

void setDriveValue(mydsp* dsp, float value) {
    dsp->fHslider0 = value;
}

void setOffsetValue(mydsp* dsp, float value) {
    dsp->fHslider1 = value;
}

const float getDriveValue(mydsp* dsp) {
    return dsp->fHslider0;
}

const float getOffsetValue(mydsp *dsp) {
    return dsp->fHslider1;
}

#endif
