#include "OpenGUI/VisualWindow.h"

OGUI::VisualWindow::VisualWindow()
{
    _pseudoMask |= (uint32_t)PseudoStates::Root;
}