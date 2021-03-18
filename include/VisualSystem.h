#pragma once
class VisualElement;
struct VisualSystemBase
{
	virtual void Update(VisualElement* Tree) = 0;
};

