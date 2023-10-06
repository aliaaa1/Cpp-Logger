#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string_view>
#include <fstream>
#include <source_location>
#include <filesystem>


inline std::string GetRelativePath(std::filesystem::path p)
{
	return p.filename().string();
}

inline std::string GetDateAndTime()
{
	const auto time = std::chrono::current_zone()
		->to_local(std::chrono::system_clock::now());
	return std::format("{:%Y-%m-%d %EX}", time);
}

inline std::string BuildLogMessage(std::string_view message,
	std::string_view verbosity, std::string_view source_name, size_t line)
{
	return "{" + GetDateAndTime() + "} " + "[" + std::string(verbosity) + "] : " +
		"\"" + std::string(message) + "\" " + "(" + GetRelativePath(source_name) + ") "
		+ "(" + std::to_string(line) + ")";
}


enum class Verbosity{ Info, Warning, Error };

// This class writes the log message to file.
class FileLogger {
public:
	FileLogger() = default;
	
	void do_write(std::string_view all_message)
	{
		ofs << all_message;
	}
private:
	std::ofstream ofs{ "log_file.txt" };
};

// This class writes the log message to console.
class ConsoleLogger {
public:
	ConsoleLogger() = default;

	void do_write(std::string_view all_message)
	{
		std::cout << all_message;
	}
private:
};


// It is the manager class that creates specific(file, console, etc.) loggers
// and run "write functions" of member logger classes.
class Logger {
public:
	static Logger& get()
	{
		static Logger s_logger;
		return s_logger;
	}

	void write(std::string_view message, std::string_view verbosity, 
		const std::source_location& location)
	{
		std::string all_message = BuildLogMessage(message, verbosity,
			location.file_name(), static_cast<size_t>(location.line()));

		m_file.do_write(all_message);
		m_console.do_write(all_message); // for the console
	}
private:
	FileLogger m_file;
	
	// It can be used if you want to print a log message to the console.
	// Then the log message is written to both the file and the console.
	ConsoleLogger m_console;
};


inline void log(std::string_view message, 
	Verbosity verbosity = Verbosity::Info, const std::source_location location = std::source_location::current())
{
	if(verbosity == Verbosity::Info)
		Logger::get().write(message, "INFO", location);

	if (verbosity == Verbosity::Warning)
		Logger::get().write(message, "WARNING", location);
	
	if (verbosity == Verbosity::Error)
		Logger::get().write(message, "ERROR", location);
}
#endif // !LOGGER_H

