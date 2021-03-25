#include "OpenGUI/Window/VisualWindow.h"

OGUI::VisualWindow::VisualWindow()
{
    _pseudoMask |= (uint32_t)PseudoStates::Root;
}