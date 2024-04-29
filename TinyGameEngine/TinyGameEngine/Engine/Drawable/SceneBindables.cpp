#include "SceneBindables.h"
#include "..\Bindable\Bindable.h"

std::shared_ptr<Bindable> SceneBindables::GetBindable(std::string&& key, std::function<std::shared_ptr<Bindable>()> callback)
{
    auto target = resources.find(key);
    if (target == resources.end())
    {
        auto ptr = callback();
        resources.insert({ key,ptr });
        return ptr;
    }
    return target->second;
}
