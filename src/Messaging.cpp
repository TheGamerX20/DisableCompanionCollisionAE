#include "Messaging.h"
#include "Events.h"

namespace DisableCompanionCollision::Messaging
{
    void Register()
    {
        // Listen for Messages
        auto MessagingInterface = F4SE::GetMessagingInterface();
        MessagingInterface->RegisterListener(F4SEMessageListener);
        REX::INFO("Started Listening for F4SE Message Callbacks.");
    }

    void F4SEMessageListener(F4SE::MessagingInterface::Message* a_msg)
    {
        switch (a_msg->type)
        {
        case F4SE::MessagingInterface::kGameLoaded:
        {
            REX::INFO("Received kGameLoaded");

            // Only Register Once
            static bool Registered = false;
			if (Registered)
            {
                return;
            }

            // Register to Menu Handler
            RE::UI::GetSingleton()->RegisterSink(Events::MenuOpenCloseEventSink::GetSingleton());
			REX::INFO("Menu Handler Registered");

            // Read Config
            Events::MenuOpenCloseEventSink::GetSingleton()->InitRead();

            // Registered
            Registered = true;

            break;
        }
        default:
            break;
        }
    }
}
