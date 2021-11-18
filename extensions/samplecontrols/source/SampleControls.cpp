#define DLL_IMPLEMENTATION
#include "SampleControls.h"
#include "generated/sample.h"

void SampleControls::Install()
{
    RegisterStyleStruct(GetDesc<StyleSample>());
    RegisterXmlParser<SliderXmlFactory>();
    RegisterXmlParser<ButtonXmlFactory>();
    RegisterXmlParser<CheckBoxXmlFactory>();
    RegisterXmlParser<ScrollViewXmlFactory>();
}