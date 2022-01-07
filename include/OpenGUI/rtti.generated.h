//DO NOT MODIFY THIS FILE
//generated from rtti.h.mako
#pragma once
#include "OpenGUI/Configure.h"
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/Style2/Selector.h"
#include "OpenGUI/Bind/Bind.h"
#include "OpenGUI/Event/FocusEvent.h"
#include "OpenGUI/Core/Types.h"
#include "OpenGUI/Event/EventBase.h"
#include "OpenGUI/Core/value.h"

namespace OGUI::Meta
{
    template<>
    struct OGUI_API TypeOf<OGUI::Bindable> { static const Type* Get(); };
    template<class Serializer>
    void Serialize(const OGUI::Bindable& value, Serializer& s)
    {
        s.BeginObject((const RecordType*)TypeOf<OGUI::Bindable>::Get());
        s.EndObject();
    }
}
namespace OGUI::Meta
{
    template<>
    struct OGUI_API TypeOf<OGUI::VisualElement> { static const Type* Get(); };
    template<class Serializer>
    void Serialize(const OGUI::VisualElement& value, Serializer& s)
    {
        s.BeginObject((const RecordType*)TypeOf<OGUI::VisualElement>::Get());
        s.EndObject();
    }
}
namespace OGUI::Meta
{
    template<>
    struct OGUI_API TypeOf<OGUI::ENavMode> { static const Type* Get(); };
}
namespace OGUI::Meta
{
    template<>
    struct OGUI_API TypeOf<OGUI::ENavCycleMode> { static const Type* Get(); };
}
namespace OGUI::Meta
{
    template<>
    struct OGUI_API TypeOf<OGUI::ENavDirection> { static const Type* Get(); };
}
namespace OGUI::Meta
{
    template<>
    struct OGUI_API TypeOf<OGUI::EKeyCode> { static const Type* Get(); };
}
namespace OGUI::Meta
{
    template<>
    struct OGUI_API TypeOf<OGUI::EInlineAlign> { static const Type* Get(); };
}
namespace OGUI::Meta
{
    template<>
    struct OGUI_API TypeOf<OGUI::EMouseKey> { static const Type* Get(); };
}
namespace OGUI::Meta
{
    template<>
    struct OGUI_API TypeOf<OGUI::EControllerAxis> { static const Type* Get(); };
}
namespace OGUI::Meta
{
    template<>
    struct OGUI_API TypeOf<OGUI::EControllerButton> { static const Type* Get(); };
}
namespace OGUI::Meta
{
    template<>
    struct OGUI_API TypeOf<OGUI::EControllerVibrationMotor> { static const Type* Get(); };
}
namespace OGUI::Meta
{
    template<>
    struct OGUI_API TypeOf<OGUI::EMouseCursor> { static const Type* Get(); };
}
namespace OGUI::Meta
{
    template<>
    struct OGUI_API TypeOf<OGUI::EGestureEvent> { static const Type* Get(); };
}
namespace OGUI::Meta
{
    template<>
    struct OGUI_API TypeOf<OGUI::PseudoStates> { static const Type* Get(); };
}
namespace OGUI::Meta
{
    template<>
    struct OGUI_API TypeOf<OGUI::EventRoutePhase> { static const Type* Get(); };
}
namespace OGUI::Meta
{
    template<>
    struct OGUI_API TypeOf<OGUI::FocusChangeCause> { static const Type* Get(); };
}
