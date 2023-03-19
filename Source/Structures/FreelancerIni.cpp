#include "PCH.hpp"
#include "Structures/FreelancerIni.hpp"

int FreelancerIni::OnIniSection(const std::string& section, int lineNumber)
{
	if (CompareInsensitive(section, "Freelancer"))
	{
		currentHandler = HandleSection(ProcessFreelancerIniValue);
	}

	return true;
}

int FreelancerIni::ProcessFreelancerIniValue(Freelancer* output, const std::string& name, const std::string& value, int lineNumber)
{
	if (CompareInsensitive(name, "data path"))
	{
		output->subsection.dataPath = value;
	}
	else if (CompareInsensitive(name, "local_server"))
	{
		output->subsection.localServer = value;
	}
	else if (CompareInsensitive(name, "initial_world"))
	{
		output->subsection.initialWorld = value;
	}
	else if (CompareInsensitive(name, "appGuid"))
	{
		output->subsection.appGuid = value;
	}
	else
	{
		return false;
	}

	return true;
}
