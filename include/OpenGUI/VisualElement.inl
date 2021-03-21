
namespace OGUI
{
	template<class F>
	void VisualElement::Traverse(F&& f, int depth)
	{
		auto count = _children.size();
		int i = 0;
		while (i < count)
		{
			auto child = _children[i];
			// !![HANG FOR BUILD]!!
			//f(function, depth + 1);
			if (child->_physical_parent != this)
				continue;
			i++;
		}
	}
}