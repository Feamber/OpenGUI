
namespace OGUI
{
	template<class F>
	void VisualElement::Traverse(F&& f)
	{
		auto count = _children.size();
		int i = 0;
		while (i < count)
		{
			auto child = _children[i];
			f(child);
			if (child->_physical_parent != this)
				continue;
			i++;
		}
	}
}