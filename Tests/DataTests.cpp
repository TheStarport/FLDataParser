#include "pch.h"
#include "Structures/FreelancerIni.hpp"

TEST(DataTests, FreelancerIniCanBeProcessed)
{
	auto ini = FreelancerIni(R"(D:\Freelancer\EXE\freelancer.ini)");
	const auto fl = ini.Parse<Freelancer>();
	EXPECT_FALSE(fl->subsection.dataPath.empty());
}