#define PARENT_CLASS(type)
#define CHILD(name, elementName, elementNamespace) \
XmlChildElementDescription name = XmlChildElementDescription(elementName, elementNamespace);

CHILDREN
#undef PARENT_CLASS
#undef CHILD

void GetAllChild(std::vector<XmlChildElementDescription*>& result)
{
#define CHILD(...)
#define PARENT_CLASS(type) \
	type::GetAllChild(result);
	CHILDREN
#undef CHILD
#undef PARENT_CLASS

#define PARENT_CLASS(type)
#define CHILD(name, ...) \
	result.push_back(&name);
	CHILDREN
#undef CHILD
#undef PARENT_CLASS
}

#undef CHILDREN