#pragma once

#include "SimpleIni.h"

namespace DisableCompanionCollision::Settings
{
    extern bool Enabled;
    extern bool WeaponDrawn;
    extern bool InCombat;
}

namespace DisableCompanionCollision::Events
{
    class MenuOpenCloseEventSink : public RE::BSTEventSink<RE::MenuOpenCloseEvent>
    {
    private:
        virtual RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent& a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_source) override;
        void ReadINI();
    
    public:
        static MenuOpenCloseEventSink* GetSingleton()
        {
            static MenuOpenCloseEventSink instance;
            return &instance;
        }
        void InitRead()
        {
            ReadINI();
        }
    };
}
