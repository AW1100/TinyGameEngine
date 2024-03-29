#pragma once
#include <string>
#include <sstream>

#define CONF_PATH "tge_conf.txt"

class Log
{
public:
	static Log& GetInstance()
	{
		static Log instance;
		return instance;
	}

	template<typename... Args>
	std::string format(const std::string& fmt, Args&&... args) {
		std::ostringstream oss;
		// Custom implementation or simplified version using `<<` operators
		// This is just a placeholder for your actual formatting logic
		oss << fmt; // Simplified example; real implementation would format args
		return oss.str();
	}

	template<typename... Args>
	void AddLog(const std::string& fmt, Args&&... args) {
		auto formatted = format(fmt, std::forward<Args>(args)...);
		logBuffer.append(formatted);
		logBuffer.append("\n");
		AppendStringToFile();
	}

	std::string ReadFileContent(const std::string& filePath);

	void AppendStringToFile();

	void RenderLogWindow();

	void Clear();

private:
	Log() = default;
	~Log() = default;

private:
	std::string logBuffer;
};

#define LOG(args) Log::GetInstance().AddLog(args);