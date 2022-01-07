#include "OpenGUI/rtti.h"
#include "OpenGUI/Core/Math/Vector.h"

namespace OGUI::Meta
{
    size_t Hash(const OGUI::Vector2f& value, size_t base)
    {
        base = Hash(value.x, base);
        base = Hash(value.y, base);
        return base;
    }

    const Type* TypeOf<Vector2f>::Get()
    {
        static std::aligned_storage_t<sizeof(Vector2f), alignof(Vector2f)> storage;
        static bool entry = false;
        if(entry)
            return std::launder(reinterpret_cast<const Type*>(&storage));
        entry = true;
        size_t size = sizeof(Vector2f);
        size_t align = alignof(Vector2f);
        ostr::string_view name = u"OGUI::Vector<float, 2>";
        auto base = (const RecordType*)nullptr;
        ObjectMethodTable nativeMethods {
            nullptr, //dtor
            nullptr, //ctor
            GetCopyCtor<Vector2f>(),
            GetMoveCtor<Vector2f>(),
            +[](const void* self, size_t base) { return Hash(*(const Vector2f*)self, base); }, //hash
        };
        static Field fields[] = 
        {
            { u"x", TypeOf<float>::Get(), offsetof(Vector2f, x) },
            { u"y", TypeOf<float>::Get(), offsetof(Vector2f, y) },
        };
        static gsl::span<Method> methods;
        new(&storage) RecordType(size, align, name, base, nativeMethods, fields, methods);
        return std::launder(reinterpret_cast<const Type*>(&storage));
    }
}