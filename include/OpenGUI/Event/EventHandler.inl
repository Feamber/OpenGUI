namespace OGUI
{
    template<class T>
    bool EventHandler::Handle(T& event)
    {
        bool handled = false;
        for (auto& callback : _callbacks)
            if (callback.eventType == typeid(T))
                handled |= callback.trampoline(callback.payload, (void*)&event);
        return handled;
    }


    template<class E, auto f>
    std::enable_if_t<!std::is_member_function_pointer_v<decltype(f)>, void>
    EventHandler::Register()
    {
        Callback callback;
        callback.eventType = typeid(E);
        callback.payload = nullptr;
        callback.trampoline = +[](void* _, void* event) -> bool
        {
            return (*f)(*(E*)event);
        };
        _callbacks.push_back(std::move(callback));
    }

    template<class E, auto f, class T>
    std::enable_if_t<!std::is_member_function_pointer_v<decltype(f)>, void>
    EventHandler::Register(T& userData)
    {
        Callback callback;
        callback.eventType = typeid(E);
        callback.payload = &userData;
        callback.trampoline = +[](void* userdata, void* event) -> bool
        {
            return (*f)(*(E*)event, *(T*)userdata);
        };
        _callbacks.push_back(std::move(callback));
    }

    template<class E, auto f, class O>
    std::enable_if_t<std::is_member_function_pointer_v<decltype(f)>, void>
    EventHandler::Register(O* owner)
    {
        Callback callback;
        callback.eventType = typeid(E);
        callback.payload = owner;
        callback.trampoline = +[](void* owner, void* event) -> bool
        {
            return (((O*)owner)->*f)(*(E*)event);
        };
        _callbacks.push_back(std::move(callback));
    }
}