//DO NOT MODIFY THIS FILE
//generated from luaBind.cpp.mako
#include "OpenGUI/Bind/EventArg.h"
#include "OpenGUI/Core/Types.h"
#include "OpenGUI/Event/EventBase.h"
#include "OpenGUI/Event/FocusEvent.h"
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/Bind/Bind.h"
#include "OpenGUI/Context.h"
#include "OpenGUI/Style2/Selector.h"
#include "luaBind.hpp"
void BindLua_generated(lua_State* L)
{
    sol::state_view lua(L);
    {
        sol::usertype<OGUI::Context> type = lua.new_usertype<OGUI::Context>("Context");
        type["IsElementValid"] = (bool(OGUI::Context::*)(OGUI::VisualElement *)const)&OGUI::Context::IsElementValid;
        type["ActivateWindow"] = (bool(OGUI::Context::*)(OGUI::VisualWindow *))&OGUI::Context::ActivateWindow;
        type["SetFocus"] = (bool(OGUI::Context::*)(OGUI::VisualElement *, OGUI::FocusChangeCause, ostr::string))&OGUI::Context::SetFocus;
        type["SetXmlFilter_Global"] = (void(OGUI::Context::*)(const char *, const char *))&OGUI::Context::SetXmlFilter_Global;
        type["CleanXmlFilter_Global"] = (void(OGUI::Context::*)(const char *))&OGUI::Context::CleanXmlFilter_Global;
        type["UpdataXmlFilterCache_Global"] = (void(OGUI::Context::*)())&OGUI::Context::UpdataXmlFilterCache_Global;
        type["HasFilterTag_Global"] = (bool(OGUI::Context::*)(const char *)const)&OGUI::Context::HasFilterTag_Global;
    }
    {
        sol::usertype<OGUI::IEventArg> type = lua.new_usertype<OGUI::IEventArg>("IEventArg");
        type["TryGet"] = +[](OGUI::IEventArg* self, ostr::string _0) { return self->TryGet(_0); };
    }
    {
        sol::usertype<OGUI::Bindable> type = lua.new_usertype<OGUI::Bindable>("Bindable");
        type["Bind"] = (void(OGUI::Bindable::*)(OGUI::Bindable &))&OGUI::Bindable::Bind;
        type["Unbind"] = (void(OGUI::Bindable::*)(OGUI::Bindable &))&OGUI::Bindable::Unbind;
    }
    {
        sol::usertype<OGUI::VisualElement> type = 
            lua.new_usertype<OGUI::VisualElement>("VisualElement", sol::base_classes, sol::bases<OGUI::Bindable>());
        type["Visible"] = (bool(OGUI::VisualElement::*)()const)&OGUI::VisualElement::Visible;
        type["GetName"] = (const ostr::string &(OGUI::VisualElement::*)())&OGUI::VisualElement::GetName;
        type["SetName"] = +[](OGUI::VisualElement* self, ostr::string _0) { return self->SetName(_0); };
        type["SetVisibility"] = (void(OGUI::VisualElement::*)(bool))&OGUI::VisualElement::SetVisibility;
        type["IsClippingChildren"] = (bool(OGUI::VisualElement::*)())&OGUI::VisualElement::IsClippingChildren;
        type["PushChild"] = (void(OGUI::VisualElement::*)(OGUI::VisualElement *))&OGUI::VisualElement::PushChild;
        type["InsertChild"] = (void(OGUI::VisualElement::*)(OGUI::VisualElement *, int))&OGUI::VisualElement::InsertChild;
        type["RemoveChild"] = (void(OGUI::VisualElement::*)(OGUI::VisualElement *))&OGUI::VisualElement::RemoveChild;
        type["GetRoot"] = (OGUI::VisualElement *(OGUI::VisualElement::*)())&OGUI::VisualElement::GetRoot;
        type["GetLayoutRoot"] = (OGUI::VisualElement *(OGUI::VisualElement::*)())&OGUI::VisualElement::GetLayoutRoot;
        type["AddStyleClass"] = +[](OGUI::VisualElement* self, ostr::string _0) { return self->AddStyleClass(_0); };
        type["RemoveStyleClass"] = +[](OGUI::VisualElement* self, ostr::string _0) { return self->RemoveStyleClass(_0); };
        type["SetPseudoClass"] = (void(OGUI::VisualElement::*)(OGUI::PseudoStates, bool))&OGUI::VisualElement::SetPseudoClass;
        type["Bind"] = (void(OGUI::VisualElement::*)(OGUI::Bindable &))&OGUI::Bindable::Bind;
        type["Unbind"] = (void(OGUI::VisualElement::*)(OGUI::Bindable &))&OGUI::Bindable::Unbind;
    }
    lua["QueryFirst"] = +[](OGUI::VisualElement * _0, ostr::string _1) { return OGUI::QueryFirst(_0, _1); };
    lua["QueryAll"] = +[](OGUI::VisualElement * _0, ostr::string _1, std::vector<OGUI::VisualElement *> & _2) { return OGUI::QueryAll(_0, _1, _2); };
    lua["BindTree"] = (void(*)(OGUI::VisualElement *, OGUI::Bindable &))&OGUI::BindTree;
    lua.new_enum<int>("ENavMode", {
        {"None", 0},
        {"Horizontal", 1},
        {"Vertical", 2},
        {"Automatic", 3}
    });
    lua.new_enum<int>("ENavCycleMode", {
        {"None", 0},
        {"Horizontal", 1},
        {"Vertical", 2},
        {"Automatic", 3}
    });
    lua.new_enum<int>("ENavDirection", {
        {"Up", 0},
        {"Down", 1},
        {"Left", 2},
        {"Right", 3}
    });
    lua.new_enum<int>("EKeyCode", {
        {"Invalid", 0},
        {"Backspace", 8},
        {"Tab", 9},
        {"Clear", 12},
        {"Enter", 13},
        {"Shift", 16},
        {"Ctrl", 17},
        {"Alt", 18},
        {"Pause", 19},
        {"CapsLock", 20},
        {"Esc", 27},
        {"SpaceBar", 32},
        {"PageUp", 33},
        {"PageDown", 34},
        {"End", 35},
        {"Home", 36},
        {"Left", 37},
        {"Up", 38},
        {"Right", 39},
        {"Down", 40},
        {"Select", 41},
        {"Print", 42},
        {"Execute", 43},
        {"Print_screen", 44},
        {"Insert", 45},
        {"Del", 46},
        {"Help", 47},
        {"Zero", 48},
        {"One", 49},
        {"Two", 50},
        {"Three", 51},
        {"Four", 52},
        {"Five", 53},
        {"Six", 54},
        {"Seven", 55},
        {"Eight", 56},
        {"Nine", 57},
        {"A", 65},
        {"B", 66},
        {"C", 67},
        {"D", 68},
        {"E", 69},
        {"F", 70},
        {"G", 71},
        {"H", 72},
        {"I", 73},
        {"J", 74},
        {"K", 75},
        {"L", 76},
        {"M", 77},
        {"N", 78},
        {"O", 79},
        {"P", 80},
        {"Q", 81},
        {"R", 82},
        {"S", 83},
        {"T", 84},
        {"U", 85},
        {"V", 86},
        {"W", 87},
        {"X", 88},
        {"Y", 89},
        {"Z", 90},
        {"LSystem", 91},
        {"RSystem", 92},
        {"App", 93},
        {"Sleep", 95},
        {"Numpad0", 96},
        {"Numpad1", 97},
        {"Numpad2", 98},
        {"Numpad3", 99},
        {"Numpad4", 100},
        {"Numpad5", 101},
        {"Numpad6", 102},
        {"Numpad7", 103},
        {"Numpad8", 104},
        {"Numpad9", 105},
        {"Multiply", 106},
        {"Add", 107},
        {"Separator", 108},
        {"Subtract", 109},
        {"Decimal", 110},
        {"Divide", 111},
        {"F1", 112},
        {"F2", 113},
        {"F3", 114},
        {"F4", 115},
        {"F5", 116},
        {"F6", 117},
        {"F7", 118},
        {"F8", 119},
        {"F9", 120},
        {"F10", 121},
        {"F11", 122},
        {"F12", 123},
        {"F13", 124},
        {"F14", 125},
        {"F15", 126},
        {"F16", 127},
        {"F17", 128},
        {"F18", 129},
        {"F19", 130},
        {"F20", 131},
        {"F21", 132},
        {"F22", 133},
        {"F23", 134},
        {"F24", 135},
        {"Num_lock", 144},
        {"Scroll_lock", 145},
        {"LShift", 160},
        {"RShift", 161},
        {"LCtrl", 162},
        {"RCtrl", 163},
        {"LAlt", 164},
        {"RAlt", 165},
        {"Semicolon", 186},
        {"Plus", 187},
        {"Comma", 188},
        {"Minus", 189},
        {"Dot", 190},
        {"Slash", 191},
        {"Wave", 192},
        {"LBranket", 219},
        {"Backslash", 220},
        {"RBranket", 221},
        {"Quote", 222},
        {"Gamepad_Left2D", 223},
        {"Gamepad_LeftX", 224},
        {"Gamepad_LeftY", 225},
        {"Gamepad_Right2D", 226},
        {"Gamepad_RightX", 227},
        {"Gamepad_RightY", 228},
        {"Gamepad_LeftTriggerAxis", 229},
        {"Gamepad_RightTriggerAxis", 230},
        {"Gamepad_LeftThumbstick", 231},
        {"Gamepad_RightThumbstick", 232},
        {"Gamepad_Special_Left", 233},
        {"Gamepad_Special_Left_X", 234},
        {"Gamepad_Special_Left_Y", 235},
        {"Gamepad_Special_Right", 236},
        {"Gamepad_FaceButton_Bottom", 237},
        {"Gamepad_FaceButton_Right", 238},
        {"Gamepad_FaceButton_Left", 239},
        {"Gamepad_FaceButton_Top", 240},
        {"Gamepad_LeftShoulder", 241},
        {"Gamepad_RightShoulder", 242},
        {"Gamepad_LeftTrigger", 243},
        {"Gamepad_RightTrigger", 244},
        {"Gamepad_DPad_Up", 245},
        {"Gamepad_DPad_Down", 246},
        {"Gamepad_DPad_Right", 247},
        {"Gamepad_DPad_Left", 248},
        {"Gamepad_LeftStick_Up", 249},
        {"Gamepad_LeftStick_Down", 250},
        {"Gamepad_LeftStick_Right", 251},
        {"Gamepad_LeftStick_Left", 252},
        {"Gamepad_RightStick_Up", 253},
        {"Gamepad_RightStick_Down", 254},
        {"Gamepad_RightStick_Right", 255},
        {"Gamepad_RightStick_Left", 256}
    });
    lua.new_enum<int>("EInlineAlign", {
        {"Baseline", 0},
        {"Top", 1},
        {"Middle", 2}
    });
    lua.new_enum<int>("EMouseKey", {
        {"None", 0},
        {"LB", 1},
        {"RB", 2},
        {"MB", 4},
        {"X1B", 8},
        {"X2B", 16}
    });
    lua.new_enum<int>("EControllerAxis", {
        {"LX", 0},
        {"LY", 1},
        {"RX", 2},
        {"RY", 3},
        {"LT", 4},
        {"RT", 5}
    });
    lua.new_enum<int>("EControllerButton", {
        {"LThumb", 0},
        {"RThumb", 1},
        {"LUp", 2},
        {"LDown", 3},
        {"LLeft", 4},
        {"LRight", 5},
        {"RUp", 6},
        {"RDown", 7},
        {"RLeft", 8},
        {"RRight", 9},
        {"LB", 10},
        {"RB", 11},
        {"LT", 12},
        {"RT", 13},
        {"LSpecial", 14},
        {"RSpecial", 15}
    });
    lua.new_enum<int>("EControllerVibrationMotor", {
        {"Left", 1},
        {"Right", 2}
    });
    lua.new_enum<int>("EMouseCursor", {
        {"Hide", 0},
        {"Arrow", 1},
        {"TextInput", 2},
        {"ResizeAll", 3},
        {"ResizeEW", 4},
        {"ResizeNS", 5},
        {"ResizeNESW", 6},
        {"ResizeNWSE", 7},
        {"Hand", 8},
        {"NotAllowed", 9}
    });
    lua.new_enum<int>("EGestureEvent", {
        {"None", 0},
        {"Scroll", 1},
        {"Magnify", 2},
        {"Swipe", 3},
        {"Rotate", 4},
        {"LongPress", 5}
    });
    lua.new_enum<int>("PseudoStates", {
        {"None", 0},
        {"Active", 1},
        {"Hover", 2},
        {"Checked", 4},
        {"Disabled", 8},
        {"Focus", 16},
        {"KeyboardFocus", 32},
        {"Scroll", 64},
        {"Root", 128}
    });
    lua.new_enum<int>("EventRoutePhase", {
        {"None", 0},
        {"TrickleDown", 1},
        {"Reach", 2},
        {"Broadcast", 4},
        {"BubbleUp", 8},
        {"All", 15},
        {"NoBroadcast", 11}
    });
    lua.new_enum<int>("FocusChangeCause", {
        {"UserActions", 0},
        {"FocusNavigation", 1},
        {"ActivateWindow", 2}
    });
}
