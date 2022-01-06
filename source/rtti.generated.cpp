//DO NOT MODIFY THIS FILE
//generated from rtti.cpp.mako
#include "OpenGUI/rtti.generated.h"


namespace OGUI::Meta
{

    const Type* TypeOf<OGUI::Bindable>::Get()
    {
        static std::aligned_storage_t<sizeof(OGUI::Bindable), alignof(OGUI::Bindable)> storage;
        static bool entry = false;
        if(entry)
            return std::launder(reinterpret_cast<const Type*>(&storage));
        entry = true;
        size_t size = sizeof(OGUI::Bindable);
        size_t align = alignof(OGUI::Bindable);
        ostr::string_view name = u"OGUI::Bindable";
        auto base = (const RecordType*)nullptr;
        ObjectMethodTable nativeMethods {
            +[](void* self) { ((OGUI::Bindable*)self)->~Bindable(); }, //dtor
            +[](void* self, struct Value* param, size_t nparam) { /*TODO*/ }, //ctor
            GetCopyCtor<OGUI::Bindable>(),
            GetMoveCtor<OGUI::Bindable>(),
            nullptr, //hash
        };
        static gsl::span<Field> fields;
        static gsl::span<Method> methods;
        new(&storage) RecordType(size, align, name, base, nativeMethods, fields, methods);
        return std::launder(reinterpret_cast<const Type*>(&storage));
    }
}
namespace OGUI::Meta
{

    const Type* TypeOf<OGUI::VisualElement>::Get()
    {
        static std::aligned_storage_t<sizeof(OGUI::VisualElement), alignof(OGUI::VisualElement)> storage;
        static bool entry = false;
        if(entry)
            return std::launder(reinterpret_cast<const Type*>(&storage));
        entry = true;
        size_t size = sizeof(OGUI::VisualElement);
        size_t align = alignof(OGUI::VisualElement);
        ostr::string_view name = u"OGUI::VisualElement";
        auto base = (const RecordType*)TypeOf<OGUI::Bindable>::Get();
        ObjectMethodTable nativeMethods {
            +[](void* self) { ((OGUI::VisualElement*)self)->~VisualElement(); }, //dtor
            +[](void* self, struct Value* param, size_t nparam) { /*TODO*/ }, //ctor
            GetCopyCtor<OGUI::VisualElement>(),
            GetMoveCtor<OGUI::VisualElement>(),
            nullptr, //hash
        };
        static gsl::span<Field> fields;
        static gsl::span<Method> methods;
        new(&storage) RecordType(size, align, name, base, nativeMethods, fields, methods);
        return std::launder(reinterpret_cast<const Type*>(&storage));
    }
}

namespace OGUI::Meta
{
    const Type* TypeOf<OGUI::ENavMode>::Get()
    {
        static EnumType::Enumerator enumerators[] = 
        {
            {u"None", 0},
            {u"Horizontal", 1},
            {u"Vertical", 2},
            {u"Automatic", 3},
        };
        static EnumType type{
            TypeOf<std::underlying_type_t<OGUI::ENavMode>>::Get(),
            u"OGUI::ENavMode", 
            +[](void* self, ostr::string_view str)
            {
                auto& This = *((OGUI::ENavMode*)self);
                switchstr(str)
                {
                    casestr("None") This = OGUI::ENavMode::None; break;
                    casestr("Horizontal") This = OGUI::ENavMode::Horizontal; break;
                    casestr("Vertical") This = OGUI::ENavMode::Vertical; break;
                    casestr("Automatic") This = OGUI::ENavMode::Automatic; break;
                    default:
                        olog::Error(u"fail to parse OGUI::ENavMode. input: {}"_o.format(str));
                        break;
                }
            },
            +[](const void* self)
            {
                auto& This = *((const OGUI::ENavMode*)self);
                switch(This)
                {
                    case OGUI::ENavMode::None: return ostr::string(u"None");
                    case OGUI::ENavMode::Horizontal: return ostr::string(u"Horizontal");
                    case OGUI::ENavMode::Vertical: return ostr::string(u"Vertical");
                    case OGUI::ENavMode::Automatic: return ostr::string(u"Automatic");
                }
                return ostr::string(u"OGUI::ENavMode::Unknown");
            },
            enumerators
        };
        return &type;
    }
}
namespace OGUI::Meta
{
    const Type* TypeOf<OGUI::ENavCycleMode>::Get()
    {
        static EnumType::Enumerator enumerators[] = 
        {
            {u"None", 0},
            {u"Horizontal", 1},
            {u"Vertical", 2},
            {u"Automatic", 3},
        };
        static EnumType type{
            TypeOf<std::underlying_type_t<OGUI::ENavCycleMode>>::Get(),
            u"OGUI::ENavCycleMode", 
            +[](void* self, ostr::string_view str)
            {
                auto& This = *((OGUI::ENavCycleMode*)self);
                switchstr(str)
                {
                    casestr("None") This = OGUI::ENavCycleMode::None; break;
                    casestr("Horizontal") This = OGUI::ENavCycleMode::Horizontal; break;
                    casestr("Vertical") This = OGUI::ENavCycleMode::Vertical; break;
                    casestr("Automatic") This = OGUI::ENavCycleMode::Automatic; break;
                    default:
                        olog::Error(u"fail to parse OGUI::ENavCycleMode. input: {}"_o.format(str));
                        break;
                }
            },
            +[](const void* self)
            {
                auto& This = *((const OGUI::ENavCycleMode*)self);
                switch(This)
                {
                    case OGUI::ENavCycleMode::None: return ostr::string(u"None");
                    case OGUI::ENavCycleMode::Horizontal: return ostr::string(u"Horizontal");
                    case OGUI::ENavCycleMode::Vertical: return ostr::string(u"Vertical");
                    case OGUI::ENavCycleMode::Automatic: return ostr::string(u"Automatic");
                }
                return ostr::string(u"OGUI::ENavCycleMode::Unknown");
            },
            enumerators
        };
        return &type;
    }
}
namespace OGUI::Meta
{
    const Type* TypeOf<OGUI::ENavDirection>::Get()
    {
        static EnumType::Enumerator enumerators[] = 
        {
            {u"Up", 0},
            {u"Down", 1},
            {u"Left", 2},
            {u"Right", 3},
        };
        static EnumType type{
            TypeOf<std::underlying_type_t<OGUI::ENavDirection>>::Get(),
            u"OGUI::ENavDirection", 
            +[](void* self, ostr::string_view str)
            {
                auto& This = *((OGUI::ENavDirection*)self);
                switchstr(str)
                {
                    casestr("Up") This = OGUI::ENavDirection::Up; break;
                    casestr("Down") This = OGUI::ENavDirection::Down; break;
                    casestr("Left") This = OGUI::ENavDirection::Left; break;
                    casestr("Right") This = OGUI::ENavDirection::Right; break;
                    default:
                        olog::Error(u"fail to parse OGUI::ENavDirection. input: {}"_o.format(str));
                        break;
                }
            },
            +[](const void* self)
            {
                auto& This = *((const OGUI::ENavDirection*)self);
                switch(This)
                {
                    case OGUI::ENavDirection::Up: return ostr::string(u"Up");
                    case OGUI::ENavDirection::Down: return ostr::string(u"Down");
                    case OGUI::ENavDirection::Left: return ostr::string(u"Left");
                    case OGUI::ENavDirection::Right: return ostr::string(u"Right");
                }
                return ostr::string(u"OGUI::ENavDirection::Unknown");
            },
            enumerators
        };
        return &type;
    }
}
namespace OGUI::Meta
{
    const Type* TypeOf<OGUI::EKeyCode>::Get()
    {
        static EnumType::Enumerator enumerators[] = 
        {
            {u"Invalid", 0},
            {u"Backspace", 8},
            {u"Tab", 9},
            {u"Clear", 12},
            {u"Enter", 13},
            {u"Shift", 16},
            {u"Ctrl", 17},
            {u"Alt", 18},
            {u"Pause", 19},
            {u"CapsLock", 20},
            {u"Esc", 27},
            {u"SpaceBar", 32},
            {u"PageUp", 33},
            {u"PageDown", 34},
            {u"End", 35},
            {u"Home", 36},
            {u"Left", 37},
            {u"Up", 38},
            {u"Right", 39},
            {u"Down", 40},
            {u"Select", 41},
            {u"Print", 42},
            {u"Execute", 43},
            {u"Print_screen", 44},
            {u"Insert", 45},
            {u"Del", 46},
            {u"Help", 47},
            {u"Zero", 48},
            {u"One", 49},
            {u"Two", 50},
            {u"Three", 51},
            {u"Four", 52},
            {u"Five", 53},
            {u"Six", 54},
            {u"Seven", 55},
            {u"Eight", 56},
            {u"Nine", 57},
            {u"A", 65},
            {u"B", 66},
            {u"C", 67},
            {u"D", 68},
            {u"E", 69},
            {u"F", 70},
            {u"G", 71},
            {u"H", 72},
            {u"I", 73},
            {u"J", 74},
            {u"K", 75},
            {u"L", 76},
            {u"M", 77},
            {u"N", 78},
            {u"O", 79},
            {u"P", 80},
            {u"Q", 81},
            {u"R", 82},
            {u"S", 83},
            {u"T", 84},
            {u"U", 85},
            {u"V", 86},
            {u"W", 87},
            {u"X", 88},
            {u"Y", 89},
            {u"Z", 90},
            {u"LSystem", 91},
            {u"RSystem", 92},
            {u"App", 93},
            {u"Sleep", 95},
            {u"Numpad0", 96},
            {u"Numpad1", 97},
            {u"Numpad2", 98},
            {u"Numpad3", 99},
            {u"Numpad4", 100},
            {u"Numpad5", 101},
            {u"Numpad6", 102},
            {u"Numpad7", 103},
            {u"Numpad8", 104},
            {u"Numpad9", 105},
            {u"Multiply", 106},
            {u"Add", 107},
            {u"Separator", 108},
            {u"Subtract", 109},
            {u"Decimal", 110},
            {u"Divide", 111},
            {u"F1", 112},
            {u"F2", 113},
            {u"F3", 114},
            {u"F4", 115},
            {u"F5", 116},
            {u"F6", 117},
            {u"F7", 118},
            {u"F8", 119},
            {u"F9", 120},
            {u"F10", 121},
            {u"F11", 122},
            {u"F12", 123},
            {u"F13", 124},
            {u"F14", 125},
            {u"F15", 126},
            {u"F16", 127},
            {u"F17", 128},
            {u"F18", 129},
            {u"F19", 130},
            {u"F20", 131},
            {u"F21", 132},
            {u"F22", 133},
            {u"F23", 134},
            {u"F24", 135},
            {u"Num_lock", 144},
            {u"Scroll_lock", 145},
            {u"LShift", 160},
            {u"RShift", 161},
            {u"LCtrl", 162},
            {u"RCtrl", 163},
            {u"LAlt", 164},
            {u"RAlt", 165},
            {u"Semicolon", 186},
            {u"Plus", 187},
            {u"Comma", 188},
            {u"Minus", 189},
            {u"Dot", 190},
            {u"Slash", 191},
            {u"Wave", 192},
            {u"LBranket", 219},
            {u"Backslash", 220},
            {u"RBranket", 221},
            {u"Quote", 222},
            {u"Gamepad_Left2D", 223},
            {u"Gamepad_LeftX", 224},
            {u"Gamepad_LeftY", 225},
            {u"Gamepad_Right2D", 226},
            {u"Gamepad_RightX", 227},
            {u"Gamepad_RightY", 228},
            {u"Gamepad_LeftTriggerAxis", 229},
            {u"Gamepad_RightTriggerAxis", 230},
            {u"Gamepad_LeftThumbstick", 231},
            {u"Gamepad_RightThumbstick", 232},
            {u"Gamepad_Special_Left", 233},
            {u"Gamepad_Special_Left_X", 234},
            {u"Gamepad_Special_Left_Y", 235},
            {u"Gamepad_Special_Right", 236},
            {u"Gamepad_FaceButton_Bottom", 237},
            {u"Gamepad_FaceButton_Right", 238},
            {u"Gamepad_FaceButton_Left", 239},
            {u"Gamepad_FaceButton_Top", 240},
            {u"Gamepad_LeftShoulder", 241},
            {u"Gamepad_RightShoulder", 242},
            {u"Gamepad_LeftTrigger", 243},
            {u"Gamepad_RightTrigger", 244},
            {u"Gamepad_DPad_Up", 245},
            {u"Gamepad_DPad_Down", 246},
            {u"Gamepad_DPad_Right", 247},
            {u"Gamepad_DPad_Left", 248},
            {u"Gamepad_LeftStick_Up", 249},
            {u"Gamepad_LeftStick_Down", 250},
            {u"Gamepad_LeftStick_Right", 251},
            {u"Gamepad_LeftStick_Left", 252},
            {u"Gamepad_RightStick_Up", 253},
            {u"Gamepad_RightStick_Down", 254},
            {u"Gamepad_RightStick_Right", 255},
            {u"Gamepad_RightStick_Left", 256},
        };
        static EnumType type{
            TypeOf<std::underlying_type_t<OGUI::EKeyCode>>::Get(),
            u"OGUI::EKeyCode", 
            +[](void* self, ostr::string_view str)
            {
                auto& This = *((OGUI::EKeyCode*)self);
                switchstr(str)
                {
                    casestr("Invalid") This = OGUI::EKeyCode::Invalid; break;
                    casestr("Backspace") This = OGUI::EKeyCode::Backspace; break;
                    casestr("Tab") This = OGUI::EKeyCode::Tab; break;
                    casestr("Clear") This = OGUI::EKeyCode::Clear; break;
                    casestr("Enter") This = OGUI::EKeyCode::Enter; break;
                    casestr("Shift") This = OGUI::EKeyCode::Shift; break;
                    casestr("Ctrl") This = OGUI::EKeyCode::Ctrl; break;
                    casestr("Alt") This = OGUI::EKeyCode::Alt; break;
                    casestr("Pause") This = OGUI::EKeyCode::Pause; break;
                    casestr("CapsLock") This = OGUI::EKeyCode::CapsLock; break;
                    casestr("Esc") This = OGUI::EKeyCode::Esc; break;
                    casestr("SpaceBar") This = OGUI::EKeyCode::SpaceBar; break;
                    casestr("PageUp") This = OGUI::EKeyCode::PageUp; break;
                    casestr("PageDown") This = OGUI::EKeyCode::PageDown; break;
                    casestr("End") This = OGUI::EKeyCode::End; break;
                    casestr("Home") This = OGUI::EKeyCode::Home; break;
                    casestr("Left") This = OGUI::EKeyCode::Left; break;
                    casestr("Up") This = OGUI::EKeyCode::Up; break;
                    casestr("Right") This = OGUI::EKeyCode::Right; break;
                    casestr("Down") This = OGUI::EKeyCode::Down; break;
                    casestr("Select") This = OGUI::EKeyCode::Select; break;
                    casestr("Print") This = OGUI::EKeyCode::Print; break;
                    casestr("Execute") This = OGUI::EKeyCode::Execute; break;
                    casestr("Print_screen") This = OGUI::EKeyCode::Print_screen; break;
                    casestr("Insert") This = OGUI::EKeyCode::Insert; break;
                    casestr("Del") This = OGUI::EKeyCode::Del; break;
                    casestr("Help") This = OGUI::EKeyCode::Help; break;
                    casestr("Zero") This = OGUI::EKeyCode::Zero; break;
                    casestr("One") This = OGUI::EKeyCode::One; break;
                    casestr("Two") This = OGUI::EKeyCode::Two; break;
                    casestr("Three") This = OGUI::EKeyCode::Three; break;
                    casestr("Four") This = OGUI::EKeyCode::Four; break;
                    casestr("Five") This = OGUI::EKeyCode::Five; break;
                    casestr("Six") This = OGUI::EKeyCode::Six; break;
                    casestr("Seven") This = OGUI::EKeyCode::Seven; break;
                    casestr("Eight") This = OGUI::EKeyCode::Eight; break;
                    casestr("Nine") This = OGUI::EKeyCode::Nine; break;
                    casestr("A") This = OGUI::EKeyCode::A; break;
                    casestr("B") This = OGUI::EKeyCode::B; break;
                    casestr("C") This = OGUI::EKeyCode::C; break;
                    casestr("D") This = OGUI::EKeyCode::D; break;
                    casestr("E") This = OGUI::EKeyCode::E; break;
                    casestr("F") This = OGUI::EKeyCode::F; break;
                    casestr("G") This = OGUI::EKeyCode::G; break;
                    casestr("H") This = OGUI::EKeyCode::H; break;
                    casestr("I") This = OGUI::EKeyCode::I; break;
                    casestr("J") This = OGUI::EKeyCode::J; break;
                    casestr("K") This = OGUI::EKeyCode::K; break;
                    casestr("L") This = OGUI::EKeyCode::L; break;
                    casestr("M") This = OGUI::EKeyCode::M; break;
                    casestr("N") This = OGUI::EKeyCode::N; break;
                    casestr("O") This = OGUI::EKeyCode::O; break;
                    casestr("P") This = OGUI::EKeyCode::P; break;
                    casestr("Q") This = OGUI::EKeyCode::Q; break;
                    casestr("R") This = OGUI::EKeyCode::R; break;
                    casestr("S") This = OGUI::EKeyCode::S; break;
                    casestr("T") This = OGUI::EKeyCode::T; break;
                    casestr("U") This = OGUI::EKeyCode::U; break;
                    casestr("V") This = OGUI::EKeyCode::V; break;
                    casestr("W") This = OGUI::EKeyCode::W; break;
                    casestr("X") This = OGUI::EKeyCode::X; break;
                    casestr("Y") This = OGUI::EKeyCode::Y; break;
                    casestr("Z") This = OGUI::EKeyCode::Z; break;
                    casestr("LSystem") This = OGUI::EKeyCode::LSystem; break;
                    casestr("RSystem") This = OGUI::EKeyCode::RSystem; break;
                    casestr("App") This = OGUI::EKeyCode::App; break;
                    casestr("Sleep") This = OGUI::EKeyCode::Sleep; break;
                    casestr("Numpad0") This = OGUI::EKeyCode::Numpad0; break;
                    casestr("Numpad1") This = OGUI::EKeyCode::Numpad1; break;
                    casestr("Numpad2") This = OGUI::EKeyCode::Numpad2; break;
                    casestr("Numpad3") This = OGUI::EKeyCode::Numpad3; break;
                    casestr("Numpad4") This = OGUI::EKeyCode::Numpad4; break;
                    casestr("Numpad5") This = OGUI::EKeyCode::Numpad5; break;
                    casestr("Numpad6") This = OGUI::EKeyCode::Numpad6; break;
                    casestr("Numpad7") This = OGUI::EKeyCode::Numpad7; break;
                    casestr("Numpad8") This = OGUI::EKeyCode::Numpad8; break;
                    casestr("Numpad9") This = OGUI::EKeyCode::Numpad9; break;
                    casestr("Multiply") This = OGUI::EKeyCode::Multiply; break;
                    casestr("Add") This = OGUI::EKeyCode::Add; break;
                    casestr("Separator") This = OGUI::EKeyCode::Separator; break;
                    casestr("Subtract") This = OGUI::EKeyCode::Subtract; break;
                    casestr("Decimal") This = OGUI::EKeyCode::Decimal; break;
                    casestr("Divide") This = OGUI::EKeyCode::Divide; break;
                    casestr("F1") This = OGUI::EKeyCode::F1; break;
                    casestr("F2") This = OGUI::EKeyCode::F2; break;
                    casestr("F3") This = OGUI::EKeyCode::F3; break;
                    casestr("F4") This = OGUI::EKeyCode::F4; break;
                    casestr("F5") This = OGUI::EKeyCode::F5; break;
                    casestr("F6") This = OGUI::EKeyCode::F6; break;
                    casestr("F7") This = OGUI::EKeyCode::F7; break;
                    casestr("F8") This = OGUI::EKeyCode::F8; break;
                    casestr("F9") This = OGUI::EKeyCode::F9; break;
                    casestr("F10") This = OGUI::EKeyCode::F10; break;
                    casestr("F11") This = OGUI::EKeyCode::F11; break;
                    casestr("F12") This = OGUI::EKeyCode::F12; break;
                    casestr("F13") This = OGUI::EKeyCode::F13; break;
                    casestr("F14") This = OGUI::EKeyCode::F14; break;
                    casestr("F15") This = OGUI::EKeyCode::F15; break;
                    casestr("F16") This = OGUI::EKeyCode::F16; break;
                    casestr("F17") This = OGUI::EKeyCode::F17; break;
                    casestr("F18") This = OGUI::EKeyCode::F18; break;
                    casestr("F19") This = OGUI::EKeyCode::F19; break;
                    casestr("F20") This = OGUI::EKeyCode::F20; break;
                    casestr("F21") This = OGUI::EKeyCode::F21; break;
                    casestr("F22") This = OGUI::EKeyCode::F22; break;
                    casestr("F23") This = OGUI::EKeyCode::F23; break;
                    casestr("F24") This = OGUI::EKeyCode::F24; break;
                    casestr("Num_lock") This = OGUI::EKeyCode::Num_lock; break;
                    casestr("Scroll_lock") This = OGUI::EKeyCode::Scroll_lock; break;
                    casestr("LShift") This = OGUI::EKeyCode::LShift; break;
                    casestr("RShift") This = OGUI::EKeyCode::RShift; break;
                    casestr("LCtrl") This = OGUI::EKeyCode::LCtrl; break;
                    casestr("RCtrl") This = OGUI::EKeyCode::RCtrl; break;
                    casestr("LAlt") This = OGUI::EKeyCode::LAlt; break;
                    casestr("RAlt") This = OGUI::EKeyCode::RAlt; break;
                    casestr("Semicolon") This = OGUI::EKeyCode::Semicolon; break;
                    casestr("Plus") This = OGUI::EKeyCode::Plus; break;
                    casestr("Comma") This = OGUI::EKeyCode::Comma; break;
                    casestr("Minus") This = OGUI::EKeyCode::Minus; break;
                    casestr("Dot") This = OGUI::EKeyCode::Dot; break;
                    casestr("Slash") This = OGUI::EKeyCode::Slash; break;
                    casestr("Wave") This = OGUI::EKeyCode::Wave; break;
                    casestr("LBranket") This = OGUI::EKeyCode::LBranket; break;
                    casestr("Backslash") This = OGUI::EKeyCode::Backslash; break;
                    casestr("RBranket") This = OGUI::EKeyCode::RBranket; break;
                    casestr("Quote") This = OGUI::EKeyCode::Quote; break;
                    casestr("Gamepad_Left2D") This = OGUI::EKeyCode::Gamepad_Left2D; break;
                    casestr("Gamepad_LeftX") This = OGUI::EKeyCode::Gamepad_LeftX; break;
                    casestr("Gamepad_LeftY") This = OGUI::EKeyCode::Gamepad_LeftY; break;
                    casestr("Gamepad_Right2D") This = OGUI::EKeyCode::Gamepad_Right2D; break;
                    casestr("Gamepad_RightX") This = OGUI::EKeyCode::Gamepad_RightX; break;
                    casestr("Gamepad_RightY") This = OGUI::EKeyCode::Gamepad_RightY; break;
                    casestr("Gamepad_LeftTriggerAxis") This = OGUI::EKeyCode::Gamepad_LeftTriggerAxis; break;
                    casestr("Gamepad_RightTriggerAxis") This = OGUI::EKeyCode::Gamepad_RightTriggerAxis; break;
                    casestr("Gamepad_LeftThumbstick") This = OGUI::EKeyCode::Gamepad_LeftThumbstick; break;
                    casestr("Gamepad_RightThumbstick") This = OGUI::EKeyCode::Gamepad_RightThumbstick; break;
                    casestr("Gamepad_Special_Left") This = OGUI::EKeyCode::Gamepad_Special_Left; break;
                    casestr("Gamepad_Special_Left_X") This = OGUI::EKeyCode::Gamepad_Special_Left_X; break;
                    casestr("Gamepad_Special_Left_Y") This = OGUI::EKeyCode::Gamepad_Special_Left_Y; break;
                    casestr("Gamepad_Special_Right") This = OGUI::EKeyCode::Gamepad_Special_Right; break;
                    casestr("Gamepad_FaceButton_Bottom") This = OGUI::EKeyCode::Gamepad_FaceButton_Bottom; break;
                    casestr("Gamepad_FaceButton_Right") This = OGUI::EKeyCode::Gamepad_FaceButton_Right; break;
                    casestr("Gamepad_FaceButton_Left") This = OGUI::EKeyCode::Gamepad_FaceButton_Left; break;
                    casestr("Gamepad_FaceButton_Top") This = OGUI::EKeyCode::Gamepad_FaceButton_Top; break;
                    casestr("Gamepad_LeftShoulder") This = OGUI::EKeyCode::Gamepad_LeftShoulder; break;
                    casestr("Gamepad_RightShoulder") This = OGUI::EKeyCode::Gamepad_RightShoulder; break;
                    casestr("Gamepad_LeftTrigger") This = OGUI::EKeyCode::Gamepad_LeftTrigger; break;
                    casestr("Gamepad_RightTrigger") This = OGUI::EKeyCode::Gamepad_RightTrigger; break;
                    casestr("Gamepad_DPad_Up") This = OGUI::EKeyCode::Gamepad_DPad_Up; break;
                    casestr("Gamepad_DPad_Down") This = OGUI::EKeyCode::Gamepad_DPad_Down; break;
                    casestr("Gamepad_DPad_Right") This = OGUI::EKeyCode::Gamepad_DPad_Right; break;
                    casestr("Gamepad_DPad_Left") This = OGUI::EKeyCode::Gamepad_DPad_Left; break;
                    casestr("Gamepad_LeftStick_Up") This = OGUI::EKeyCode::Gamepad_LeftStick_Up; break;
                    casestr("Gamepad_LeftStick_Down") This = OGUI::EKeyCode::Gamepad_LeftStick_Down; break;
                    casestr("Gamepad_LeftStick_Right") This = OGUI::EKeyCode::Gamepad_LeftStick_Right; break;
                    casestr("Gamepad_LeftStick_Left") This = OGUI::EKeyCode::Gamepad_LeftStick_Left; break;
                    casestr("Gamepad_RightStick_Up") This = OGUI::EKeyCode::Gamepad_RightStick_Up; break;
                    casestr("Gamepad_RightStick_Down") This = OGUI::EKeyCode::Gamepad_RightStick_Down; break;
                    casestr("Gamepad_RightStick_Right") This = OGUI::EKeyCode::Gamepad_RightStick_Right; break;
                    casestr("Gamepad_RightStick_Left") This = OGUI::EKeyCode::Gamepad_RightStick_Left; break;
                    default:
                        olog::Error(u"fail to parse OGUI::EKeyCode. input: {}"_o.format(str));
                        break;
                }
            },
            +[](const void* self)
            {
                auto& This = *((const OGUI::EKeyCode*)self);
                switch(This)
                {
                    case OGUI::EKeyCode::Invalid: return ostr::string(u"Invalid");
                    case OGUI::EKeyCode::Backspace: return ostr::string(u"Backspace");
                    case OGUI::EKeyCode::Tab: return ostr::string(u"Tab");
                    case OGUI::EKeyCode::Clear: return ostr::string(u"Clear");
                    case OGUI::EKeyCode::Enter: return ostr::string(u"Enter");
                    case OGUI::EKeyCode::Shift: return ostr::string(u"Shift");
                    case OGUI::EKeyCode::Ctrl: return ostr::string(u"Ctrl");
                    case OGUI::EKeyCode::Alt: return ostr::string(u"Alt");
                    case OGUI::EKeyCode::Pause: return ostr::string(u"Pause");
                    case OGUI::EKeyCode::CapsLock: return ostr::string(u"CapsLock");
                    case OGUI::EKeyCode::Esc: return ostr::string(u"Esc");
                    case OGUI::EKeyCode::SpaceBar: return ostr::string(u"SpaceBar");
                    case OGUI::EKeyCode::PageUp: return ostr::string(u"PageUp");
                    case OGUI::EKeyCode::PageDown: return ostr::string(u"PageDown");
                    case OGUI::EKeyCode::End: return ostr::string(u"End");
                    case OGUI::EKeyCode::Home: return ostr::string(u"Home");
                    case OGUI::EKeyCode::Left: return ostr::string(u"Left");
                    case OGUI::EKeyCode::Up: return ostr::string(u"Up");
                    case OGUI::EKeyCode::Right: return ostr::string(u"Right");
                    case OGUI::EKeyCode::Down: return ostr::string(u"Down");
                    case OGUI::EKeyCode::Select: return ostr::string(u"Select");
                    case OGUI::EKeyCode::Print: return ostr::string(u"Print");
                    case OGUI::EKeyCode::Execute: return ostr::string(u"Execute");
                    case OGUI::EKeyCode::Print_screen: return ostr::string(u"Print_screen");
                    case OGUI::EKeyCode::Insert: return ostr::string(u"Insert");
                    case OGUI::EKeyCode::Del: return ostr::string(u"Del");
                    case OGUI::EKeyCode::Help: return ostr::string(u"Help");
                    case OGUI::EKeyCode::Zero: return ostr::string(u"Zero");
                    case OGUI::EKeyCode::One: return ostr::string(u"One");
                    case OGUI::EKeyCode::Two: return ostr::string(u"Two");
                    case OGUI::EKeyCode::Three: return ostr::string(u"Three");
                    case OGUI::EKeyCode::Four: return ostr::string(u"Four");
                    case OGUI::EKeyCode::Five: return ostr::string(u"Five");
                    case OGUI::EKeyCode::Six: return ostr::string(u"Six");
                    case OGUI::EKeyCode::Seven: return ostr::string(u"Seven");
                    case OGUI::EKeyCode::Eight: return ostr::string(u"Eight");
                    case OGUI::EKeyCode::Nine: return ostr::string(u"Nine");
                    case OGUI::EKeyCode::A: return ostr::string(u"A");
                    case OGUI::EKeyCode::B: return ostr::string(u"B");
                    case OGUI::EKeyCode::C: return ostr::string(u"C");
                    case OGUI::EKeyCode::D: return ostr::string(u"D");
                    case OGUI::EKeyCode::E: return ostr::string(u"E");
                    case OGUI::EKeyCode::F: return ostr::string(u"F");
                    case OGUI::EKeyCode::G: return ostr::string(u"G");
                    case OGUI::EKeyCode::H: return ostr::string(u"H");
                    case OGUI::EKeyCode::I: return ostr::string(u"I");
                    case OGUI::EKeyCode::J: return ostr::string(u"J");
                    case OGUI::EKeyCode::K: return ostr::string(u"K");
                    case OGUI::EKeyCode::L: return ostr::string(u"L");
                    case OGUI::EKeyCode::M: return ostr::string(u"M");
                    case OGUI::EKeyCode::N: return ostr::string(u"N");
                    case OGUI::EKeyCode::O: return ostr::string(u"O");
                    case OGUI::EKeyCode::P: return ostr::string(u"P");
                    case OGUI::EKeyCode::Q: return ostr::string(u"Q");
                    case OGUI::EKeyCode::R: return ostr::string(u"R");
                    case OGUI::EKeyCode::S: return ostr::string(u"S");
                    case OGUI::EKeyCode::T: return ostr::string(u"T");
                    case OGUI::EKeyCode::U: return ostr::string(u"U");
                    case OGUI::EKeyCode::V: return ostr::string(u"V");
                    case OGUI::EKeyCode::W: return ostr::string(u"W");
                    case OGUI::EKeyCode::X: return ostr::string(u"X");
                    case OGUI::EKeyCode::Y: return ostr::string(u"Y");
                    case OGUI::EKeyCode::Z: return ostr::string(u"Z");
                    case OGUI::EKeyCode::LSystem: return ostr::string(u"LSystem");
                    case OGUI::EKeyCode::RSystem: return ostr::string(u"RSystem");
                    case OGUI::EKeyCode::App: return ostr::string(u"App");
                    case OGUI::EKeyCode::Sleep: return ostr::string(u"Sleep");
                    case OGUI::EKeyCode::Numpad0: return ostr::string(u"Numpad0");
                    case OGUI::EKeyCode::Numpad1: return ostr::string(u"Numpad1");
                    case OGUI::EKeyCode::Numpad2: return ostr::string(u"Numpad2");
                    case OGUI::EKeyCode::Numpad3: return ostr::string(u"Numpad3");
                    case OGUI::EKeyCode::Numpad4: return ostr::string(u"Numpad4");
                    case OGUI::EKeyCode::Numpad5: return ostr::string(u"Numpad5");
                    case OGUI::EKeyCode::Numpad6: return ostr::string(u"Numpad6");
                    case OGUI::EKeyCode::Numpad7: return ostr::string(u"Numpad7");
                    case OGUI::EKeyCode::Numpad8: return ostr::string(u"Numpad8");
                    case OGUI::EKeyCode::Numpad9: return ostr::string(u"Numpad9");
                    case OGUI::EKeyCode::Multiply: return ostr::string(u"Multiply");
                    case OGUI::EKeyCode::Add: return ostr::string(u"Add");
                    case OGUI::EKeyCode::Separator: return ostr::string(u"Separator");
                    case OGUI::EKeyCode::Subtract: return ostr::string(u"Subtract");
                    case OGUI::EKeyCode::Decimal: return ostr::string(u"Decimal");
                    case OGUI::EKeyCode::Divide: return ostr::string(u"Divide");
                    case OGUI::EKeyCode::F1: return ostr::string(u"F1");
                    case OGUI::EKeyCode::F2: return ostr::string(u"F2");
                    case OGUI::EKeyCode::F3: return ostr::string(u"F3");
                    case OGUI::EKeyCode::F4: return ostr::string(u"F4");
                    case OGUI::EKeyCode::F5: return ostr::string(u"F5");
                    case OGUI::EKeyCode::F6: return ostr::string(u"F6");
                    case OGUI::EKeyCode::F7: return ostr::string(u"F7");
                    case OGUI::EKeyCode::F8: return ostr::string(u"F8");
                    case OGUI::EKeyCode::F9: return ostr::string(u"F9");
                    case OGUI::EKeyCode::F10: return ostr::string(u"F10");
                    case OGUI::EKeyCode::F11: return ostr::string(u"F11");
                    case OGUI::EKeyCode::F12: return ostr::string(u"F12");
                    case OGUI::EKeyCode::F13: return ostr::string(u"F13");
                    case OGUI::EKeyCode::F14: return ostr::string(u"F14");
                    case OGUI::EKeyCode::F15: return ostr::string(u"F15");
                    case OGUI::EKeyCode::F16: return ostr::string(u"F16");
                    case OGUI::EKeyCode::F17: return ostr::string(u"F17");
                    case OGUI::EKeyCode::F18: return ostr::string(u"F18");
                    case OGUI::EKeyCode::F19: return ostr::string(u"F19");
                    case OGUI::EKeyCode::F20: return ostr::string(u"F20");
                    case OGUI::EKeyCode::F21: return ostr::string(u"F21");
                    case OGUI::EKeyCode::F22: return ostr::string(u"F22");
                    case OGUI::EKeyCode::F23: return ostr::string(u"F23");
                    case OGUI::EKeyCode::F24: return ostr::string(u"F24");
                    case OGUI::EKeyCode::Num_lock: return ostr::string(u"Num_lock");
                    case OGUI::EKeyCode::Scroll_lock: return ostr::string(u"Scroll_lock");
                    case OGUI::EKeyCode::LShift: return ostr::string(u"LShift");
                    case OGUI::EKeyCode::RShift: return ostr::string(u"RShift");
                    case OGUI::EKeyCode::LCtrl: return ostr::string(u"LCtrl");
                    case OGUI::EKeyCode::RCtrl: return ostr::string(u"RCtrl");
                    case OGUI::EKeyCode::LAlt: return ostr::string(u"LAlt");
                    case OGUI::EKeyCode::RAlt: return ostr::string(u"RAlt");
                    case OGUI::EKeyCode::Semicolon: return ostr::string(u"Semicolon");
                    case OGUI::EKeyCode::Plus: return ostr::string(u"Plus");
                    case OGUI::EKeyCode::Comma: return ostr::string(u"Comma");
                    case OGUI::EKeyCode::Minus: return ostr::string(u"Minus");
                    case OGUI::EKeyCode::Dot: return ostr::string(u"Dot");
                    case OGUI::EKeyCode::Slash: return ostr::string(u"Slash");
                    case OGUI::EKeyCode::Wave: return ostr::string(u"Wave");
                    case OGUI::EKeyCode::LBranket: return ostr::string(u"LBranket");
                    case OGUI::EKeyCode::Backslash: return ostr::string(u"Backslash");
                    case OGUI::EKeyCode::RBranket: return ostr::string(u"RBranket");
                    case OGUI::EKeyCode::Quote: return ostr::string(u"Quote");
                    case OGUI::EKeyCode::Gamepad_Left2D: return ostr::string(u"Gamepad_Left2D");
                    case OGUI::EKeyCode::Gamepad_LeftX: return ostr::string(u"Gamepad_LeftX");
                    case OGUI::EKeyCode::Gamepad_LeftY: return ostr::string(u"Gamepad_LeftY");
                    case OGUI::EKeyCode::Gamepad_Right2D: return ostr::string(u"Gamepad_Right2D");
                    case OGUI::EKeyCode::Gamepad_RightX: return ostr::string(u"Gamepad_RightX");
                    case OGUI::EKeyCode::Gamepad_RightY: return ostr::string(u"Gamepad_RightY");
                    case OGUI::EKeyCode::Gamepad_LeftTriggerAxis: return ostr::string(u"Gamepad_LeftTriggerAxis");
                    case OGUI::EKeyCode::Gamepad_RightTriggerAxis: return ostr::string(u"Gamepad_RightTriggerAxis");
                    case OGUI::EKeyCode::Gamepad_LeftThumbstick: return ostr::string(u"Gamepad_LeftThumbstick");
                    case OGUI::EKeyCode::Gamepad_RightThumbstick: return ostr::string(u"Gamepad_RightThumbstick");
                    case OGUI::EKeyCode::Gamepad_Special_Left: return ostr::string(u"Gamepad_Special_Left");
                    case OGUI::EKeyCode::Gamepad_Special_Left_X: return ostr::string(u"Gamepad_Special_Left_X");
                    case OGUI::EKeyCode::Gamepad_Special_Left_Y: return ostr::string(u"Gamepad_Special_Left_Y");
                    case OGUI::EKeyCode::Gamepad_Special_Right: return ostr::string(u"Gamepad_Special_Right");
                    case OGUI::EKeyCode::Gamepad_FaceButton_Bottom: return ostr::string(u"Gamepad_FaceButton_Bottom");
                    case OGUI::EKeyCode::Gamepad_FaceButton_Right: return ostr::string(u"Gamepad_FaceButton_Right");
                    case OGUI::EKeyCode::Gamepad_FaceButton_Left: return ostr::string(u"Gamepad_FaceButton_Left");
                    case OGUI::EKeyCode::Gamepad_FaceButton_Top: return ostr::string(u"Gamepad_FaceButton_Top");
                    case OGUI::EKeyCode::Gamepad_LeftShoulder: return ostr::string(u"Gamepad_LeftShoulder");
                    case OGUI::EKeyCode::Gamepad_RightShoulder: return ostr::string(u"Gamepad_RightShoulder");
                    case OGUI::EKeyCode::Gamepad_LeftTrigger: return ostr::string(u"Gamepad_LeftTrigger");
                    case OGUI::EKeyCode::Gamepad_RightTrigger: return ostr::string(u"Gamepad_RightTrigger");
                    case OGUI::EKeyCode::Gamepad_DPad_Up: return ostr::string(u"Gamepad_DPad_Up");
                    case OGUI::EKeyCode::Gamepad_DPad_Down: return ostr::string(u"Gamepad_DPad_Down");
                    case OGUI::EKeyCode::Gamepad_DPad_Right: return ostr::string(u"Gamepad_DPad_Right");
                    case OGUI::EKeyCode::Gamepad_DPad_Left: return ostr::string(u"Gamepad_DPad_Left");
                    case OGUI::EKeyCode::Gamepad_LeftStick_Up: return ostr::string(u"Gamepad_LeftStick_Up");
                    case OGUI::EKeyCode::Gamepad_LeftStick_Down: return ostr::string(u"Gamepad_LeftStick_Down");
                    case OGUI::EKeyCode::Gamepad_LeftStick_Right: return ostr::string(u"Gamepad_LeftStick_Right");
                    case OGUI::EKeyCode::Gamepad_LeftStick_Left: return ostr::string(u"Gamepad_LeftStick_Left");
                    case OGUI::EKeyCode::Gamepad_RightStick_Up: return ostr::string(u"Gamepad_RightStick_Up");
                    case OGUI::EKeyCode::Gamepad_RightStick_Down: return ostr::string(u"Gamepad_RightStick_Down");
                    case OGUI::EKeyCode::Gamepad_RightStick_Right: return ostr::string(u"Gamepad_RightStick_Right");
                    case OGUI::EKeyCode::Gamepad_RightStick_Left: return ostr::string(u"Gamepad_RightStick_Left");
                }
                return ostr::string(u"OGUI::EKeyCode::Unknown");
            },
            enumerators
        };
        return &type;
    }
}
namespace OGUI::Meta
{
    const Type* TypeOf<OGUI::EInlineAlign>::Get()
    {
        static EnumType::Enumerator enumerators[] = 
        {
            {u"Baseline", 0},
            {u"Top", 1},
            {u"Middle", 2},
        };
        static EnumType type{
            TypeOf<std::underlying_type_t<OGUI::EInlineAlign>>::Get(),
            u"OGUI::EInlineAlign", 
            +[](void* self, ostr::string_view str)
            {
                auto& This = *((OGUI::EInlineAlign*)self);
                switchstr(str)
                {
                    casestr("Baseline") This = OGUI::EInlineAlign::Baseline; break;
                    casestr("Top") This = OGUI::EInlineAlign::Top; break;
                    casestr("Middle") This = OGUI::EInlineAlign::Middle; break;
                    default:
                        olog::Error(u"fail to parse OGUI::EInlineAlign. input: {}"_o.format(str));
                        break;
                }
            },
            +[](const void* self)
            {
                auto& This = *((const OGUI::EInlineAlign*)self);
                switch(This)
                {
                    case OGUI::EInlineAlign::Baseline: return ostr::string(u"Baseline");
                    case OGUI::EInlineAlign::Top: return ostr::string(u"Top");
                    case OGUI::EInlineAlign::Middle: return ostr::string(u"Middle");
                }
                return ostr::string(u"OGUI::EInlineAlign::Unknown");
            },
            enumerators
        };
        return &type;
    }
}
namespace OGUI::Meta
{
    const Type* TypeOf<OGUI::EMouseKey>::Get()
    {
        static EnumType::Enumerator enumerators[] = 
        {
            {u"None", 0},
            {u"LB", 1},
            {u"RB", 2},
            {u"MB", 4},
            {u"X1B", 8},
            {u"X2B", 16},
        };
        static EnumType type{
            TypeOf<std::underlying_type_t<OGUI::EMouseKey>>::Get(),
            u"OGUI::EMouseKey", 
            +[](void* self, ostr::string_view str)
            {
                auto& This = *((OGUI::EMouseKey*)self);
                switchstr(str)
                {
                    casestr("None") This = OGUI::EMouseKey::None; break;
                    casestr("LB") This = OGUI::EMouseKey::LB; break;
                    casestr("RB") This = OGUI::EMouseKey::RB; break;
                    casestr("MB") This = OGUI::EMouseKey::MB; break;
                    casestr("X1B") This = OGUI::EMouseKey::X1B; break;
                    casestr("X2B") This = OGUI::EMouseKey::X2B; break;
                    default:
                        olog::Error(u"fail to parse OGUI::EMouseKey. input: {}"_o.format(str));
                        break;
                }
            },
            +[](const void* self)
            {
                auto& This = *((const OGUI::EMouseKey*)self);
                switch(This)
                {
                    case OGUI::EMouseKey::None: return ostr::string(u"None");
                    case OGUI::EMouseKey::LB: return ostr::string(u"LB");
                    case OGUI::EMouseKey::RB: return ostr::string(u"RB");
                    case OGUI::EMouseKey::MB: return ostr::string(u"MB");
                    case OGUI::EMouseKey::X1B: return ostr::string(u"X1B");
                    case OGUI::EMouseKey::X2B: return ostr::string(u"X2B");
                }
                return ostr::string(u"OGUI::EMouseKey::Unknown");
            },
            enumerators
        };
        return &type;
    }
}
namespace OGUI::Meta
{
    const Type* TypeOf<OGUI::EControllerAxis>::Get()
    {
        static EnumType::Enumerator enumerators[] = 
        {
            {u"LX", 0},
            {u"LY", 1},
            {u"RX", 2},
            {u"RY", 3},
            {u"LT", 4},
            {u"RT", 5},
        };
        static EnumType type{
            TypeOf<std::underlying_type_t<OGUI::EControllerAxis>>::Get(),
            u"OGUI::EControllerAxis", 
            +[](void* self, ostr::string_view str)
            {
                auto& This = *((OGUI::EControllerAxis*)self);
                switchstr(str)
                {
                    casestr("LX") This = OGUI::EControllerAxis::LX; break;
                    casestr("LY") This = OGUI::EControllerAxis::LY; break;
                    casestr("RX") This = OGUI::EControllerAxis::RX; break;
                    casestr("RY") This = OGUI::EControllerAxis::RY; break;
                    casestr("LT") This = OGUI::EControllerAxis::LT; break;
                    casestr("RT") This = OGUI::EControllerAxis::RT; break;
                    default:
                        olog::Error(u"fail to parse OGUI::EControllerAxis. input: {}"_o.format(str));
                        break;
                }
            },
            +[](const void* self)
            {
                auto& This = *((const OGUI::EControllerAxis*)self);
                switch(This)
                {
                    case OGUI::EControllerAxis::LX: return ostr::string(u"LX");
                    case OGUI::EControllerAxis::LY: return ostr::string(u"LY");
                    case OGUI::EControllerAxis::RX: return ostr::string(u"RX");
                    case OGUI::EControllerAxis::RY: return ostr::string(u"RY");
                    case OGUI::EControllerAxis::LT: return ostr::string(u"LT");
                    case OGUI::EControllerAxis::RT: return ostr::string(u"RT");
                }
                return ostr::string(u"OGUI::EControllerAxis::Unknown");
            },
            enumerators
        };
        return &type;
    }
}
namespace OGUI::Meta
{
    const Type* TypeOf<OGUI::EControllerButton>::Get()
    {
        static EnumType::Enumerator enumerators[] = 
        {
            {u"LThumb", 0},
            {u"RThumb", 1},
            {u"LUp", 2},
            {u"LDown", 3},
            {u"LLeft", 4},
            {u"LRight", 5},
            {u"RUp", 6},
            {u"RDown", 7},
            {u"RLeft", 8},
            {u"RRight", 9},
            {u"LB", 10},
            {u"RB", 11},
            {u"LT", 12},
            {u"RT", 13},
            {u"LSpecial", 14},
            {u"RSpecial", 15},
        };
        static EnumType type{
            TypeOf<std::underlying_type_t<OGUI::EControllerButton>>::Get(),
            u"OGUI::EControllerButton", 
            +[](void* self, ostr::string_view str)
            {
                auto& This = *((OGUI::EControllerButton*)self);
                switchstr(str)
                {
                    casestr("LThumb") This = OGUI::EControllerButton::LThumb; break;
                    casestr("RThumb") This = OGUI::EControllerButton::RThumb; break;
                    casestr("LUp") This = OGUI::EControllerButton::LUp; break;
                    casestr("LDown") This = OGUI::EControllerButton::LDown; break;
                    casestr("LLeft") This = OGUI::EControllerButton::LLeft; break;
                    casestr("LRight") This = OGUI::EControllerButton::LRight; break;
                    casestr("RUp") This = OGUI::EControllerButton::RUp; break;
                    casestr("RDown") This = OGUI::EControllerButton::RDown; break;
                    casestr("RLeft") This = OGUI::EControllerButton::RLeft; break;
                    casestr("RRight") This = OGUI::EControllerButton::RRight; break;
                    casestr("LB") This = OGUI::EControllerButton::LB; break;
                    casestr("RB") This = OGUI::EControllerButton::RB; break;
                    casestr("LT") This = OGUI::EControllerButton::LT; break;
                    casestr("RT") This = OGUI::EControllerButton::RT; break;
                    casestr("LSpecial") This = OGUI::EControllerButton::LSpecial; break;
                    casestr("RSpecial") This = OGUI::EControllerButton::RSpecial; break;
                    default:
                        olog::Error(u"fail to parse OGUI::EControllerButton. input: {}"_o.format(str));
                        break;
                }
            },
            +[](const void* self)
            {
                auto& This = *((const OGUI::EControllerButton*)self);
                switch(This)
                {
                    case OGUI::EControllerButton::LThumb: return ostr::string(u"LThumb");
                    case OGUI::EControllerButton::RThumb: return ostr::string(u"RThumb");
                    case OGUI::EControllerButton::LUp: return ostr::string(u"LUp");
                    case OGUI::EControllerButton::LDown: return ostr::string(u"LDown");
                    case OGUI::EControllerButton::LLeft: return ostr::string(u"LLeft");
                    case OGUI::EControllerButton::LRight: return ostr::string(u"LRight");
                    case OGUI::EControllerButton::RUp: return ostr::string(u"RUp");
                    case OGUI::EControllerButton::RDown: return ostr::string(u"RDown");
                    case OGUI::EControllerButton::RLeft: return ostr::string(u"RLeft");
                    case OGUI::EControllerButton::RRight: return ostr::string(u"RRight");
                    case OGUI::EControllerButton::LB: return ostr::string(u"LB");
                    case OGUI::EControllerButton::RB: return ostr::string(u"RB");
                    case OGUI::EControllerButton::LT: return ostr::string(u"LT");
                    case OGUI::EControllerButton::RT: return ostr::string(u"RT");
                    case OGUI::EControllerButton::LSpecial: return ostr::string(u"LSpecial");
                    case OGUI::EControllerButton::RSpecial: return ostr::string(u"RSpecial");
                }
                return ostr::string(u"OGUI::EControllerButton::Unknown");
            },
            enumerators
        };
        return &type;
    }
}
namespace OGUI::Meta
{
    const Type* TypeOf<OGUI::EControllerVibrationMotor>::Get()
    {
        static EnumType::Enumerator enumerators[] = 
        {
            {u"Left", 1},
            {u"Right", 2},
        };
        static EnumType type{
            TypeOf<std::underlying_type_t<OGUI::EControllerVibrationMotor>>::Get(),
            u"OGUI::EControllerVibrationMotor", 
            +[](void* self, ostr::string_view str)
            {
                auto& This = *((OGUI::EControllerVibrationMotor*)self);
                switchstr(str)
                {
                    casestr("Left") This = OGUI::EControllerVibrationMotor::Left; break;
                    casestr("Right") This = OGUI::EControllerVibrationMotor::Right; break;
                    default:
                        olog::Error(u"fail to parse OGUI::EControllerVibrationMotor. input: {}"_o.format(str));
                        break;
                }
            },
            +[](const void* self)
            {
                auto& This = *((const OGUI::EControllerVibrationMotor*)self);
                switch(This)
                {
                    case OGUI::EControllerVibrationMotor::Left: return ostr::string(u"Left");
                    case OGUI::EControllerVibrationMotor::Right: return ostr::string(u"Right");
                }
                return ostr::string(u"OGUI::EControllerVibrationMotor::Unknown");
            },
            enumerators
        };
        return &type;
    }
}
namespace OGUI::Meta
{
    const Type* TypeOf<OGUI::EMouseCursor>::Get()
    {
        static EnumType::Enumerator enumerators[] = 
        {
            {u"Hide", 0},
            {u"Arrow", 1},
            {u"TextInput", 2},
            {u"ResizeAll", 3},
            {u"ResizeEW", 4},
            {u"ResizeNS", 5},
            {u"ResizeNESW", 6},
            {u"ResizeNWSE", 7},
            {u"Hand", 8},
            {u"NotAllowed", 9},
        };
        static EnumType type{
            TypeOf<std::underlying_type_t<OGUI::EMouseCursor>>::Get(),
            u"OGUI::EMouseCursor", 
            +[](void* self, ostr::string_view str)
            {
                auto& This = *((OGUI::EMouseCursor*)self);
                switchstr(str)
                {
                    casestr("Hide") This = OGUI::EMouseCursor::Hide; break;
                    casestr("Arrow") This = OGUI::EMouseCursor::Arrow; break;
                    casestr("TextInput") This = OGUI::EMouseCursor::TextInput; break;
                    casestr("ResizeAll") This = OGUI::EMouseCursor::ResizeAll; break;
                    casestr("ResizeEW") This = OGUI::EMouseCursor::ResizeEW; break;
                    casestr("ResizeNS") This = OGUI::EMouseCursor::ResizeNS; break;
                    casestr("ResizeNESW") This = OGUI::EMouseCursor::ResizeNESW; break;
                    casestr("ResizeNWSE") This = OGUI::EMouseCursor::ResizeNWSE; break;
                    casestr("Hand") This = OGUI::EMouseCursor::Hand; break;
                    casestr("NotAllowed") This = OGUI::EMouseCursor::NotAllowed; break;
                    default:
                        olog::Error(u"fail to parse OGUI::EMouseCursor. input: {}"_o.format(str));
                        break;
                }
            },
            +[](const void* self)
            {
                auto& This = *((const OGUI::EMouseCursor*)self);
                switch(This)
                {
                    case OGUI::EMouseCursor::Hide: return ostr::string(u"Hide");
                    case OGUI::EMouseCursor::Arrow: return ostr::string(u"Arrow");
                    case OGUI::EMouseCursor::TextInput: return ostr::string(u"TextInput");
                    case OGUI::EMouseCursor::ResizeAll: return ostr::string(u"ResizeAll");
                    case OGUI::EMouseCursor::ResizeEW: return ostr::string(u"ResizeEW");
                    case OGUI::EMouseCursor::ResizeNS: return ostr::string(u"ResizeNS");
                    case OGUI::EMouseCursor::ResizeNESW: return ostr::string(u"ResizeNESW");
                    case OGUI::EMouseCursor::ResizeNWSE: return ostr::string(u"ResizeNWSE");
                    case OGUI::EMouseCursor::Hand: return ostr::string(u"Hand");
                    case OGUI::EMouseCursor::NotAllowed: return ostr::string(u"NotAllowed");
                }
                return ostr::string(u"OGUI::EMouseCursor::Unknown");
            },
            enumerators
        };
        return &type;
    }
}
namespace OGUI::Meta
{
    const Type* TypeOf<OGUI::EGestureEvent>::Get()
    {
        static EnumType::Enumerator enumerators[] = 
        {
            {u"None", 0},
            {u"Scroll", 1},
            {u"Magnify", 2},
            {u"Swipe", 3},
            {u"Rotate", 4},
            {u"LongPress", 5},
        };
        static EnumType type{
            TypeOf<std::underlying_type_t<OGUI::EGestureEvent>>::Get(),
            u"OGUI::EGestureEvent", 
            +[](void* self, ostr::string_view str)
            {
                auto& This = *((OGUI::EGestureEvent*)self);
                switchstr(str)
                {
                    casestr("None") This = OGUI::EGestureEvent::None; break;
                    casestr("Scroll") This = OGUI::EGestureEvent::Scroll; break;
                    casestr("Magnify") This = OGUI::EGestureEvent::Magnify; break;
                    casestr("Swipe") This = OGUI::EGestureEvent::Swipe; break;
                    casestr("Rotate") This = OGUI::EGestureEvent::Rotate; break;
                    casestr("LongPress") This = OGUI::EGestureEvent::LongPress; break;
                    default:
                        olog::Error(u"fail to parse OGUI::EGestureEvent. input: {}"_o.format(str));
                        break;
                }
            },
            +[](const void* self)
            {
                auto& This = *((const OGUI::EGestureEvent*)self);
                switch(This)
                {
                    case OGUI::EGestureEvent::None: return ostr::string(u"None");
                    case OGUI::EGestureEvent::Scroll: return ostr::string(u"Scroll");
                    case OGUI::EGestureEvent::Magnify: return ostr::string(u"Magnify");
                    case OGUI::EGestureEvent::Swipe: return ostr::string(u"Swipe");
                    case OGUI::EGestureEvent::Rotate: return ostr::string(u"Rotate");
                    case OGUI::EGestureEvent::LongPress: return ostr::string(u"LongPress");
                }
                return ostr::string(u"OGUI::EGestureEvent::Unknown");
            },
            enumerators
        };
        return &type;
    }
}
namespace OGUI::Meta
{
    const Type* TypeOf<OGUI::PseudoStates>::Get()
    {
        static EnumType::Enumerator enumerators[] = 
        {
            {u"None", 0},
            {u"Active", 1},
            {u"Hover", 2},
            {u"Checked", 4},
            {u"Disabled", 8},
            {u"Focus", 16},
            {u"KeyboardFocus", 32},
            {u"Scroll", 64},
            {u"Root", 128},
        };
        static EnumType type{
            TypeOf<std::underlying_type_t<OGUI::PseudoStates>>::Get(),
            u"OGUI::PseudoStates", 
            +[](void* self, ostr::string_view str)
            {
                auto& This = *((OGUI::PseudoStates*)self);
                switchstr(str)
                {
                    casestr("None") This = OGUI::PseudoStates::None; break;
                    casestr("Active") This = OGUI::PseudoStates::Active; break;
                    casestr("Hover") This = OGUI::PseudoStates::Hover; break;
                    casestr("Checked") This = OGUI::PseudoStates::Checked; break;
                    casestr("Disabled") This = OGUI::PseudoStates::Disabled; break;
                    casestr("Focus") This = OGUI::PseudoStates::Focus; break;
                    casestr("KeyboardFocus") This = OGUI::PseudoStates::KeyboardFocus; break;
                    casestr("Scroll") This = OGUI::PseudoStates::Scroll; break;
                    casestr("Root") This = OGUI::PseudoStates::Root; break;
                    default:
                        olog::Error(u"fail to parse OGUI::PseudoStates. input: {}"_o.format(str));
                        break;
                }
            },
            +[](const void* self)
            {
                auto& This = *((const OGUI::PseudoStates*)self);
                switch(This)
                {
                    case OGUI::PseudoStates::None: return ostr::string(u"None");
                    case OGUI::PseudoStates::Active: return ostr::string(u"Active");
                    case OGUI::PseudoStates::Hover: return ostr::string(u"Hover");
                    case OGUI::PseudoStates::Checked: return ostr::string(u"Checked");
                    case OGUI::PseudoStates::Disabled: return ostr::string(u"Disabled");
                    case OGUI::PseudoStates::Focus: return ostr::string(u"Focus");
                    case OGUI::PseudoStates::KeyboardFocus: return ostr::string(u"KeyboardFocus");
                    case OGUI::PseudoStates::Scroll: return ostr::string(u"Scroll");
                    case OGUI::PseudoStates::Root: return ostr::string(u"Root");
                }
                return ostr::string(u"OGUI::PseudoStates::Unknown");
            },
            enumerators
        };
        return &type;
    }
}
namespace OGUI::Meta
{
    const Type* TypeOf<OGUI::EventRoutePhase>::Get()
    {
        static EnumType::Enumerator enumerators[] = 
        {
            {u"None", 0},
            {u"TrickleDown", 1},
            {u"Reach", 2},
            {u"Broadcast", 4},
            {u"BubbleUp", 8},
            {u"All", 15},
            {u"NoBroadcast", 11},
        };
        static EnumType type{
            TypeOf<std::underlying_type_t<OGUI::EventRoutePhase>>::Get(),
            u"OGUI::EventRoutePhase", 
            +[](void* self, ostr::string_view str)
            {
                auto& This = *((OGUI::EventRoutePhase*)self);
                switchstr(str)
                {
                    casestr("None") This = OGUI::EventRoutePhase::None; break;
                    casestr("TrickleDown") This = OGUI::EventRoutePhase::TrickleDown; break;
                    casestr("Reach") This = OGUI::EventRoutePhase::Reach; break;
                    casestr("Broadcast") This = OGUI::EventRoutePhase::Broadcast; break;
                    casestr("BubbleUp") This = OGUI::EventRoutePhase::BubbleUp; break;
                    casestr("All") This = OGUI::EventRoutePhase::All; break;
                    casestr("NoBroadcast") This = OGUI::EventRoutePhase::NoBroadcast; break;
                    default:
                        olog::Error(u"fail to parse OGUI::EventRoutePhase. input: {}"_o.format(str));
                        break;
                }
            },
            +[](const void* self)
            {
                auto& This = *((const OGUI::EventRoutePhase*)self);
                switch(This)
                {
                    case OGUI::EventRoutePhase::None: return ostr::string(u"None");
                    case OGUI::EventRoutePhase::TrickleDown: return ostr::string(u"TrickleDown");
                    case OGUI::EventRoutePhase::Reach: return ostr::string(u"Reach");
                    case OGUI::EventRoutePhase::Broadcast: return ostr::string(u"Broadcast");
                    case OGUI::EventRoutePhase::BubbleUp: return ostr::string(u"BubbleUp");
                    case OGUI::EventRoutePhase::All: return ostr::string(u"All");
                    case OGUI::EventRoutePhase::NoBroadcast: return ostr::string(u"NoBroadcast");
                }
                return ostr::string(u"OGUI::EventRoutePhase::Unknown");
            },
            enumerators
        };
        return &type;
    }
}
namespace OGUI::Meta
{
    const Type* TypeOf<OGUI::FocusChangeCause>::Get()
    {
        static EnumType::Enumerator enumerators[] = 
        {
            {u"UserActions", 0},
            {u"FocusNavigation", 1},
            {u"ActivateWindow", 2},
        };
        static EnumType type{
            TypeOf<std::underlying_type_t<OGUI::FocusChangeCause>>::Get(),
            u"OGUI::FocusChangeCause", 
            +[](void* self, ostr::string_view str)
            {
                auto& This = *((OGUI::FocusChangeCause*)self);
                switchstr(str)
                {
                    casestr("UserActions") This = OGUI::FocusChangeCause::UserActions; break;
                    casestr("FocusNavigation") This = OGUI::FocusChangeCause::FocusNavigation; break;
                    casestr("ActivateWindow") This = OGUI::FocusChangeCause::ActivateWindow; break;
                    default:
                        olog::Error(u"fail to parse OGUI::FocusChangeCause. input: {}"_o.format(str));
                        break;
                }
            },
            +[](const void* self)
            {
                auto& This = *((const OGUI::FocusChangeCause*)self);
                switch(This)
                {
                    case OGUI::FocusChangeCause::UserActions: return ostr::string(u"UserActions");
                    case OGUI::FocusChangeCause::FocusNavigation: return ostr::string(u"FocusNavigation");
                    case OGUI::FocusChangeCause::ActivateWindow: return ostr::string(u"ActivateWindow");
                }
                return ostr::string(u"OGUI::FocusChangeCause::Unknown");
            },
            enumerators
        };
        return &type;
    }
}
