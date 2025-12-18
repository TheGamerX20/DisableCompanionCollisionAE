#pragma once

#include "Messaging.h"
#include "Events.h"

namespace DisableCompanionCollision
{
	inline bool IsPlayerTeammate(RE::Actor* a_actor) 
	{
		if (!a_actor)
		{
			return false;
		}

		return (a_actor->niFlags.flags >> 26) & 1;
	}

	inline bool CanOverlap(RE::TESObjectREFR* a_refr)
	{
		if (!Settings::Enabled)
		{
			return false;
		}

		if (Settings::WeaponDrawn || Settings::InCombat)
		{
			RE::PlayerCharacter* Player = RE::PlayerCharacter::GetSingleton();
			
			if (!Player)
			{
				return false;
			}

			if (Settings::WeaponDrawn && !Player->GetWeaponMagicDrawn())
			{
				return false;
			}

			if (Settings::InCombat && !Player->IsInCombat())
			{
				return false;
			}
		}

		if (!a_refr)
		{
			return false;
		}

		RE::Actor* Actor = a_refr->As<RE::Actor>();
		if (!Actor)
		{
			return false;
		}

		if (!IsPlayerTeammate(Actor))
		{
			return false;
		}

		return true;
	}

	inline void InstallHook()
	{
		// Get the Trampoline and Allocate
		auto& trampoline = REL::GetTrampoline();
		trampoline.create(128);

		struct asm_code : Xbyak::CodeGenerator {
			asm_code(std::uintptr_t a_srcAddr) {
				Xbyak::Label funcLabel, defLabel, skipLabel;

				// Using pop/push screws up something so this is workaround.
				//push(rax); 
				mov(r10, rax);                    // Save original rax
				mov(rcx, ptr[r10 + 0x3E0]);       // rcx = (TESObjectREFR*) from saved rax
				call(ptr[rip + funcLabel]);       // call CanOverlap
				movzx(r8d, al);                   // preserve result from al (return of the CanOverlap)
				mov(rax, r10);                    // restore original rax
				test(r8b, r8b);                   // test result of CanOverlap
				//pop(rax);

				je("DEFAULT");
				jmp(ptr[rip + skipLabel]);        // if true, skip collision

				L("DEFAULT");
				mov(ecx, ptr[rax + 0x300]);       // original collision logic
				jmp(ptr[rip + defLabel]);

				L(funcLabel);
				dq((std::uintptr_t)CanOverlap);

				L(defLabel);
				dq(a_srcAddr + 0x06);             // resume at: 141799524                 shr     ecx, 1Bh
				L(skipLabel);
				dq(a_srcAddr + 0x0E);             // resume at: 14179952C                 mov     r10, [rsp+240h+var_200]
			}
		};

		REL::Relocation<std::uintptr_t> target{ REL::ID(2278536), REL::Offset(0x5DE) };
		asm_code codegen{ target.address() };
		void* codeBuf = REL::GetTrampoline().allocate(codegen);
		trampoline.write_jmp6(target.address(), reinterpret_cast<std::uintptr_t>(codeBuf));

		REX::INFO("Installed Hooks.");
	}

	inline bool Install()
	{
		// Install the Hooks
		InstallHook();

		// Register for the Messaging Interface
		Messaging::Register();

		return true;
	}
}
