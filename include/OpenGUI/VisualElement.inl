
namespace OGUI
{
	template<class F>
	void VisualElement::Traverse(F&& f, int depth)
	{
		auto count = _children.size();
		int i = 0;
		while (i < count)
		{
			auto child = _children[i].get();
			f(child, depth + 1);
			if (child->_physical_parent.lock().get() != this)
				continue;
			i++;
		}
	}
}