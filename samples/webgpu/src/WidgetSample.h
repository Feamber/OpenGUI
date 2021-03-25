#pragma once
#include <memory>
#include <vector>
namespace OGUI
{
    class VisualElement;
}
struct WidgetSample
{
    std::shared_ptr<OGUI::VisualElement> tree;
    std::vector<std::shared_ptr<OGUI::VisualElement>> nodes;
    void Initialize();
};