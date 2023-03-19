#include "PCH.hpp"

#include "IniReader.hpp"

IniReader* IniReader::i = nullptr;

int IniReader::Handler(void* type, const char* section, const char* name, const char* value, const int lineNumber)
{
	if (name == nullptr && value == nullptr)
	{
		return i->OnIniSection(std::string(section), lineNumber);
	}

	return i->OnIniValue(type, name, value, lineNumber);
}

int IniReader::OnIniValue(void* output, const std::string& name, const std::string& value, const int lineNumber) const
{
	if (!currentHandler)
		return 0;

	return currentHandler(output, name, value, lineNumber);
}

int IniReader::ParseRaw(void* output, const char* fileName)
{
	return ini_parse(fileName, Handler, output);
}

IniReader::IniReader(std::string fileName): fileName(std::move(fileName))
{
}

const std::vector<std::string>& IniReader::GetErrors()
{
	return errors;
}
