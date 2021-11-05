#include <algorithm>
#include <typeindex>
#include <vector>
#define DLL_IMPLEMENTATION
#include "OpenGUI/XmlParser/AttributeBind.h"

namespace OGUI
{
    namespace AttributeBind 
    {
         static std::unordered_map<Name, AttrSource*> AllAttrSource;
         static std::unordered_map<Name, std::vector<AttrBind*>> AllAttrBind;
         static std::unordered_map<std::type_index, std::unordered_map<std::type_index, AttrConverterFun>> AllConverter;
    }
    using namespace AttributeBind;

    AttrSource::AttrSource(Name fullName, std::type_index type, void* data)
        : fullName(fullName), type(type), data(data)
    {
        auto result = AllAttrSource.try_emplace(fullName, this);
        isValid = result.second;
    }

    AttrSource::~AttrSource()
    {
        if(!isValid)
            return;
        
        AllAttrSource.erase(fullName);
    }

    void AttrSource::DataChange() const
    {
        if(!isValid)
            return;

        auto find = AllAttrBind.find(fullName);
        if(find != AllAttrBind.end())
        {
            //TODO void* data
            auto& attrBindList = find->second;
            for(auto bind : attrBindList)
            {
                bind->changeFun(*this);
            }
        }
    }

    AttrBind::AttrBind(Name fullName, OnChange changeFun)
        : fullName(fullName), changeFun(changeFun), changePostFun(), type(typeid(nullptr)), data(nullptr)
    {
        auto result = AllAttrBind.try_emplace(fullName).first;
        auto& attrBindList = result->second;
        attrBindList.push_back(this);
    }

    AttrBind::AttrBind(Name fullName, std::type_index type, void* data, OnChangePost changePostFun)
        : fullName(fullName), changeFun(), changePostFun(changePostFun), type(type), data(data)
    {
        auto result = AllAttrBind.try_emplace(fullName).first;
        auto& attrBindList = result->second;
        attrBindList.push_back(this);
    }

    AttrBind::~AttrBind()
    {
        auto find = AllAttrBind.find(fullName);
        if(find != AllAttrBind.end())
        {
            auto& attrBindList = find->second;
            attrBindList.erase(std::find(attrBindList.begin(), attrBindList.end(), this));
        }
    }

    bool RegisterAttrConverter(std::type_index source, std::type_index target, AttrConverterFun converter)
    {
        auto result = AllConverter.try_emplace(source);
        auto& targetMap = result.first->second;
        auto result2 = targetMap.try_emplace(target, converter);
        return result2.second;
    }

    bool AttrConverter(std::type_index sourceType, void* source, std::type_index targetType, void* target)
    {
        auto findSource = AllConverter.find(sourceType);
        if(findSource == AllConverter.end())
            return false;
        
        auto& targetMap = findSource->second;
        auto findTarget = targetMap.find(targetType);
        if(findTarget == targetMap.end())
            return false;
        
        return findTarget->second(source, target);
    }

    void RegisterBaseAttrConverter()
    {
        RegisterAttrConverter<int, int32>([](void* source, void* out)
            {
                *((int32*)out) = *((int*)source);
                return true;
            });
        RegisterAttrConverter<int, float>([](void* source, void* out)
            {
                *((float*)out) = *((int*)source);
                return true;
            });
        
        RegisterAttrConverter<int32, int>([](void* source, void* out)
            {
                *((int*)out) = *((int32*)source);
                return true;
            });
        RegisterAttrConverter<int32, float>([](void* source, void* out)
            {
                *((float*)out) = *((int32*)source);
                return true;
            });

        RegisterAttrConverter<float, int32>([](void* source, void* out)
            {
                *((int32*)out) = *((float*)source);
                return true;
            });
        RegisterAttrConverter<float, int>([](void* source, void* out)
            {
                *((int*)out) = *((float*)source);
                return true;
            });
        
        RegisterAttrConverter<ostr::string, std::string>([](void* source, void* out)
            {
                auto strv = (*(ostr::string*)source).to_sv();
                *((std::string*)out) = {strv.begin(), strv.end()};
                return true;
            });

        RegisterAttrConverter<std::string, ostr::string>([](void* source, void* out)
            {
                *((ostr::string*)out) = *((std::string*)source);
                return true;
            });
    }
}