#pragma once
#include "Configure.h"
#include "Button.h"
#include "OpenGUI/XmlParser/BaseXmlFactory.h"

namespace SampleControls
{
    using namespace OGUI;
    class CheckBox : public Button
    {
    public:
        ostr::string_view GetTypeName() override { return u"CheckBox"; }
        ostr::string_view GetFullTypeName() override { return u"SampleControls::CheckBox"; };
        CheckBox();
        void OnClicked() override;
        bool _checked;
    };

    class SAMPLECONTROLS_API CheckBoxXmlFactory : public VisualElementXmlFactory
    {
    public:
        static const OGUI::Name& GetFullName();
        virtual bool OnCreateElement(InstantiateXmlState&, XmlElement&, VisualElement*& outNewElement, VisualElement* parent) override;
    };
}