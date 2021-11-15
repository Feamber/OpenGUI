#pragma once
#include "Button.h"

namespace SampleControls
{
    using namespace OGUI;
    class CheckBox : public Button
    {
    public:
        std::string_view GetTypeName() override { return "CheckBox"; }
        std::string_view GetFullTypeName() override { return "SampleControls::CheckBox"; };
        CheckBox();
        static void RegisterXml();
        void OnClicked() override;
        bool _checked;
    };
}