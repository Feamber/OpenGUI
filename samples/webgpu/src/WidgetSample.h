#pragma once
#include <memory>
#include <vector>
namespace OGUI
{
    class VisualElement;
}
struct WidgetSample
{
    std::unique_ptr<OGUI::VisualElement> tree;
    std::vector<std::unique_ptr<OGUI::VisualElement>> nodes;
    void Initialize();
};