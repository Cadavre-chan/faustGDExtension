#include "extendedMapUI.h"

void ExtendedMapUI::addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {
    std::string path = buildPath(label);
    MapUI::addVerticalSlider(label, zone, init, min, max, step);
    paramMeta[path] = { min, max, init };
}

float ExtendedMapUI::getParamMin(const std::string &path) const {
    auto min = paramMeta.find(path);
    return min != paramMeta.end() ? min->second.min : 0.0f;
}

float ExtendedMapUI::getParamMax(const std::string &path) const {
    auto max = paramMeta.find(path);
    return max != paramMeta.end() ? max->second.max : 1.0f;
}