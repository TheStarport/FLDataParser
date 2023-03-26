#pragma once
#include <exception>
#include <string>

class IniParsingException final : std::exception
{
public:
	std::string message;

	explicit IniParsingException(std::string file, std::string message, int lineNumber)
		: message(std::format("Error parsing INI file. Message: {}\nLine Number: {}\nFile: {}", message, lineNumber, file))
	{
	}

	[[nodiscard]] char const* what() const override
	{
		return message.c_str();
	}
};
