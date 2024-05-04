#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <functional>


class SceneBindables
{
public:
	static SceneBindables& GetInstance()
	{
		static SceneBindables instance;
		return instance;
	}

	std::shared_ptr<class Bindable> GetBindable(std::string&& key, std::function<std::shared_ptr<Bindable>()> callback);

private:
	SceneBindables() {}
	~SceneBindables() {}
	SceneBindables(const SceneBindables&) = delete;
	SceneBindables& operator=(const SceneBindables&) = delete;

private:
	std::unordered_map<std::string, std::shared_ptr<Bindable>> resources;
};

template<typename T>
std::string GenerateUID(const std::string& addition)
{
	return std::string(typeid(T).name()) + std::string("#") + addition;
}

template<typename T>
std::string GenerateUID()
{
	return std::string(typeid(T).name()) + std::string("#");
}
