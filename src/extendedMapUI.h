#ifndef EXTENDED_MAP_UI_H
#define EXTENDED_MAP_UI_H

#include <faust/gui/MapUI.h>
#include <unordered_map>
#include <string>


class ExtendedMapUI : public MapUI {
    public:
        struct ParamMetadata {
            float min;
            float max;
            float init;
        };
        std::map<std::string, ParamMetadata> paramMeta;

        void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) override;

        float getParamMin(const std::string &path) const;

        float getParamMax(const std::string &path) const;

        // You can override other methods similarly if needed.
};

#endif