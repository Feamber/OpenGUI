namespace OGUI
{
	template<class F>
	void VisualElement::Traverse(F&& f)
	{
		std::vector<VisualElement*> children;
		GetChildren(children);
		for(auto child : children)
			f(child);
	}
}