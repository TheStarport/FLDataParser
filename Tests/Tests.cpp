#include "pch.h"
#include "Reader.hpp"
#include "Structures/FreelancerIni.hpp"
#include <cstdlib>

const char* flPath = std::getenv("FL_PATH");

TEST(DataTests, FreelancerIniCanBeProcessed)
{
	auto ini = Ini::Reader();
	ini.Parse<Ini::FreelancerIni>(std::format("{}/DATA/SHIPS/shiparch.ini",flPath));
}

TEST(DataTests, FreelancerIniCanBeProcessed2)
{
	auto ini = Ini::Reader();
	ini.Parse<Ini::FreelancerIni>(std::format("{}/EXE/Freelancer.ini",flPath));
}