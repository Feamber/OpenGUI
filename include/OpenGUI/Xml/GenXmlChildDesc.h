#define PARENT_CLASS(type)
#define CHILD(name, element_type) \
XmlChildElementDescription name = XmlChildElementDescription::Build<element_type>();

CHILDREN
#undef PARENT_CLASS
#undef CHILD

void GetAllChild(std::vector<XmlAttributeDescription*>& result)
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