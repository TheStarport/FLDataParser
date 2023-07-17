#pragma once

#include "IniSerializer.hpp"

class FreelancerIni final : Ini::IniFile
{
    public:
        struct FreelancerSubsection : Ini::Subsection
        {
                std::string dataPath;
                std::string localServer;
                std::string initialWorld;
                std::string appGuid;

                FreelancerSubsection() = default;
        };

        FreelancerSubsection freelancer;

        FreelancerIni() = default;
};

REFL_AUTO(type(FreelancerIni::FreelancerSubsection), Key(dataPath, "data path"), Key(localServer, "local_server"), Key(initialWorld, "initial_world"),
          Key(appGuid, "appguid"));

REFL_AUTO(type(FreelancerIni), Section(freelancer, "freelancer"));
