#pragma one
#include <string>

class VisualElementAsset
{
public:
	bool GetAttributeValue(const std::string& name, std::string& out_value);
	bool GetValueString(std::string& out, const std::string& element) const;
};