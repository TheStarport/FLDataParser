#pragma once

#include "Reader.hpp"

namespace Ini
{
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

	class FreelancerIni final : public IniData
	{
	public:
		void ParseIniData(const std::vector<IniSection>& sections) override
		{
		}
	};
}