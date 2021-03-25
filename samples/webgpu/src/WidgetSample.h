#pragma once
#include <memory>
#include <vector>
#include "OpenGUI/Style/StyleSheet.h"
namespace OGUI
{
    class VisualElement;
}
struct WidgetSample
{
    OGUI::StyleSheet styleSheet;
    std::shared_ptr<OGUI::VisualElement> tree;
    std::vector<std::shared_ptr<OGUI::VisualElement>> nodes;
    void Initialize();
};