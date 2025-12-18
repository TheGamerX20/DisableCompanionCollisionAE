#include <pch.h>

// Patches
#include <DisableCompanionCollision.h>

namespace Main
{
    // Config Options
    static REX::INI::Bool iDisableCompanionCollisionPatch{ "Patches"sv, "EnableDisableCompanionCollisionPatch"sv, true };

    F4SE_PLUGIN_PRELOAD(const F4SE::LoadInterface* a_f4se)
    {
        // Init
        F4SE::Init(a_f4se);
        REX::INFO("Disable Companion Collision AE Initializing...");

        // Load the Config
        const auto config = REX::INI::SettingStore::GetSingleton();
        config->Init("Data/F4SE/Plugins/DisableCompanionCollisionAE.ini", "Data/F4SE/Plugins/DisableCompanionCollisionAECustom.ini");
        config->Load();

        // Install Disable Companion Collision
        if (iDisableCompanionCollisionPatch.GetValue() == true)
        {
            if (DisableCompanionCollision::Install())
            {
                REX::INFO("Disable Companion Collision AE Initialized!");
            }
        }

        // Finished
        return true;
    }
}
