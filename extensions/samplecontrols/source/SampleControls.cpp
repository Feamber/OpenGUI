
#include "SampleControls.h"
#include "generated/sample.h"

void SampleControls::Install()
{
    RegisterStyleStruct<StyleSample>();
    RegisterXmlParser<SliderXmlFactory>();
    RegisterXmlParser<ButtonXmlFactory>();
    RegisterXmlParser<CheckBoxXmlFactory>();
    RegisterXmlParser<ScrollViewXmlFactory>();
}