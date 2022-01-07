#pragma once
#include "OpenGUI/Core/Math/Vector.h"
#include "OpenGUI/rtti.generated.h"

namespace OGUI::Meta
{
    OGUI_API size_t Hash(const OGUI::Vector2f& value, size_t base = FNV_offset_basis);
    template<>
    struct OGUI_API TypeOf<OGUI::Vector2f> { static const Type* Get(); };
    template<class Serializer>
    void Serialize(const OGUI::Vector2f& value, Serializer& s)
    {
        s.BeginObject((const RecordType*)TypeOf<OGUI::Vector2f>::Get());
        SerializeField(OGUI::Vector2f, x)
        SerializeField(OGUI::Vector2f, y)
        s.EndObject();
    }
}
namespace std
{
    template<>
    class hash<OGUI::Vector2f>
    {
        size_t operator()(const OGUI::Vector2f& value)
        {
            return OGUI::Meta::Hash(value);
        }
    };
}