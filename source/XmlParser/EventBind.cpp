#define DLL_IMPLEMENTATION
#include <algorithm>
#include "OpenGUI/Bind/EventBind.h"

namespace OGUI
{
    namespace EventBind
    {
        std::unordered_map<Name, std::vector<struct Handler*>>& GetAllEventHandler()
        {
            static std::unordered_map<Name, std::vector<Handler*>> _AllEventHandler;
            return _AllEventHandler;
        }

        Handler::~Handler()
        {
            if(!eventName.IsNone())
            {
                auto find = GetAllEventHandler().find(eventName);
                if(find != GetAllEventHandler().end())
                {
                    auto& list = find->second;
                    list.erase(std::find(list.begin(), list.end(), this));
                }
            }
        }
    }
}