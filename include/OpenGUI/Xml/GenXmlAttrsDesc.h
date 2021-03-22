#define PARENT_CLASS(type)
#define ATTR(type, name, default_value, use) \
type name {#name, default_value, use};

ATTRS
#undef PARENT_CLASS
#undef ATTR

void GetAllAttrDesc(std::vector<XmlAttributeDescription*>& result)
{
#define ATTR(...)
#define PARENT_CLASS(type) \
	type::GetAllAttr(result);
	ATTRS
#undef ATTR
#undef PARENT_CLASS

#define PARENT_CLASS(type)
#define ATTR(type, name, ...) \
	result.push_back(&name);
	ATTRS
#undef ATTR
#undef PARENT_CLASS
}

#undef ATTRS