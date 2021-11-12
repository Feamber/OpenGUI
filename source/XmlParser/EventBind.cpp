#include <algorithm>
#define DLL_IMPLEMENTATION
#include "OpenGUI/XmlParser/EventBind.h"

namespace OGUI
{
    namespace EventBind
    {
        OGUI_API std::map<Name, std::vector<Handler*>> _AllEventHandler;

        Handler::~Handler()
        {
            if(!eventName.IsNone())
            {
                auto find = _AllEventHandler.find(eventName);
                if(find != _AllEventHandler.end())
                {
                    auto& list = find->second;
                    list.erase(std::find(list.begin(), list.end(), this));
                }
            }
        }
    }
}