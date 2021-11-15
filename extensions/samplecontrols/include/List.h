#pragma once
#include "OpenGUI/VisualElement.h"

namespace SampleControls
{
    using namespace OGUI;
    class List : public VisualElement
    {
    public:
        std::string_view GetTypeName() override { return "List"; }
        std::string_view GetFullTypeName() override { return "SampleControls::List"; };
        List();
        static void RegisterXml();
    };
}