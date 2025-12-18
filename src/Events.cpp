#include "Events.h"

namespace DisableCompanionCollision::Settings
{
	bool Enabled = true;
	bool WeaponDrawn = false;
	bool InCombat = false;
}

namespace DisableCompanionCollision::Events
{
    RE::BSEventNotifyControl MenuOpenCloseEventSink::ProcessEvent(const RE::MenuOpenCloseEvent& a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_source)
    {
        if (a_event.menuName == "PauseMenu" && !a_event.opening)
        {
            ReadINI();
        }

		return RE::BSEventNotifyControl::kContinue;
    }

    void MenuOpenCloseEventSink::ReadINI()
    {
		CSimpleIniA INI;

		if (!std::filesystem::exists(std::filesystem::path("Data/MCM/Settings/DisableCompanionCollision.ini")))
		{
			REX::INFO("Config File is Missing!");
			return;
		}

		SI_Error rc = INI.LoadFile("Data/MCM/Settings/DisableCompanionCollision.ini");
		if (rc > 0)
		{
			REX::INFO("Failed to Read the Config, Code = {}", rc);
		}

		Settings::Enabled = INI.GetBoolValue("Settings", "bEnabled", Settings::Enabled);
		Settings::WeaponDrawn = INI.GetBoolValue("Settings", "bIsActiveOnlyIfPlayerWeaponDrawn", Settings::WeaponDrawn);
		Settings::InCombat = INI.GetBoolValue("Settings", "bIsActiveOnlyIfPlayerInCombat", Settings::InCombat);
    }
}
