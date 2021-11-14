#define DLL_IMPLEMENTATION
#include "SampleControls.h"
#include "generated/sample.h"

void SampleControls::Install()
{
    RegisterStyleStruct(GetDesc<StyleSample>());
    Slider::RegisterXml();
}