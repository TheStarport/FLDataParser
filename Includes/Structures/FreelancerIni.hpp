#pragma once

#include "IniReader.hpp"

struct Freelancer
{
	struct FreelancerSubsection
	{
		std::string dataPath;
		std::string localServer;
		std::string initialWorld;
		std::string appGuid;
	};

	FreelancerSubsection subsection;
};

class FLDataImportExport FreelancerIni final : public IniReader
{
	static int ProcessFreelancerIniValue(Freelancer* output, const std::string& name, const std::string& value, int lineNumber);

protected:
	int OnIniSection(const std::string& section, int lineNumber) override;

public:
	explicit FreelancerIni(const std::string& fileName) : IniReader(fileName)
	{
	}
	~FreelancerIni() override = default;
};