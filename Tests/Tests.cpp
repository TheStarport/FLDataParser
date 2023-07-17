#include "pch.h"

#include "Reader.hpp"
#include "Structures/FreelancerIni.hpp"

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(DataTests, FreelancerIniCanBeProcessed)
{
    auto ini = Ini::Reader();
    // ini.Parse<Ini::FreelancerIni, "FreelancerIni">(R"(C:\Users\laz\Downloads\shiparch.ini)");
}

TEST(DataTests, FreelancerIniCanBeProcessed2)
{
    auto ini = Ini::Reader();
    const auto fl = ini.Parse<FreelancerIni>(R"(D:\Freelancer\EXE\freelancer.ini)");
    printf("");
}
