#pragma once

#include <functional>

#ifndef FLDataImportExport
#ifdef FLDataParser
#define FLDataImportExport __declspec(dllexport)
#else
#define FLDataImportExport __declspec(dllimport)
#endif
#endif

template <std::size_t Ind, typename R, typename ... Args>
std::tuple_element_t<Ind, std::tuple<std::remove_reference_t<Args>...>>
ParamHelper(R(Args...));

template <auto X, std::size_t Ind>
struct ParamType
{
	using Type = decltype(ParamHelper<Ind>(X));
};

#define HandleSection(x) [this](void* ptr, std::string name, std::string value, int valLineNum) { return x(static_cast<ParamType<x, 0u>::Type>(ptr), name, value, valLineNum); }

class FLDataImportExport IniReader
{
	static IniReader* i;

	std::string fileName;
	std::vector<std::string> errors;
	static int Handler(void* type, const char* section, const char* name, const char* value, int lineNumber);
	int OnIniValue(void* output, const std::string& name, const std::string& value, int lineNumber) const;
	static int ParseRaw(void* output, const char* fileName);

protected:
	virtual int OnIniSection(const std::string& section, int lineNumber) = 0;

	using CurrentHandler = std::function<int(void* ptr, const std::string& name, const std::string& value, int lineNumber)>;
	CurrentHandler currentHandler = nullptr;

public:
	explicit IniReader(std::string fileName);
	virtual ~IniReader() = default;

	template<typename T>
	std::shared_ptr<T> Parse()
	{
		i = this;
		std::shared_ptr<T> output = std::make_shared<T>();
		ParseRaw(output.get(), fileName.c_str());

		return output;
	}

	const std::vector<std::string>& GetErrors();
};