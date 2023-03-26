#include "pch.h"
#include "Reader.hpp"
#include "Structures/FreelancerIni.hpp"

TEST(DataTests, FreelancerIniCanBeProcessed)
{
	auto ini = Ini::Reader();
	ini.Parse<Ini::FreelancerIni>(R"(C:\Users\laz\Downloads\shiparch.ini)");
}

TEST(DataTests, FreelancerIniCanBeProcessed2)
{
	auto ini = Ini::Reader();
	ini.Parse<Ini::FreelancerIni>(R"(D:\Freelancer\EXE\freelancer.ini)");
}