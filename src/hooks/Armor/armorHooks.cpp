#include "armorHooks.h"

#include "data/modObjectManager.h"

#include <xbyak.h>

namespace Hooks::Armor
{
	void Install() {
		Manager::GetSingleton()->Install();
	}

	void Manager::Install()
	{
		REL::Relocation<std::uintptr_t> armorRating1(REL::ID(44014));

		struct ratingPatch : Xbyak::CodeGenerator
		{
			ratingPatch(std::uintptr_t returnAddress)
			{
				Xbyak::Label funcLabel;
				Xbyak::Label returnLabel;

				call(ptr[rip + funcLabel]);
				movss(xmm1, xmm0);
				jmp(ptr[rip + returnLabel]);

				L(funcLabel);
				dq(reinterpret_cast<std::uintptr_t>(VisitArmorRating));

				L(returnLabel);
				dq(returnAddress);
			}
		};

		ratingPatch code1{ armorRating1.address() + 0x108 };

		auto& trampoline = SKSE::GetTrampoline();
		trampoline.write_branch<6>(armorRating1.address() + 0xF9, trampoline.allocate(code1));
	}

	float Manager::VisitArmorRating(RE::Character* a_target)
	{
		constexpr float armorFactor = 0.001f;
		constexpr float diminishedFactor = 0.0005;
		constexpr float softCap = 700.0f;
		constexpr float hardCap = 1000.0f;

		const auto armorRating = a_target ? a_target->CalcArmorRating() : 0.0f;
		const auto unbreakablePerk = Data::ModObject<RE::BGSPerk>("ARM_Armor_PRK_100_Unbreakable");
		assert(unbreakablePerk);

		auto response = 0.0f;

		if (a_target) {
			if (armorRating > softCap) {
				if (unbreakablePerk && a_target->HasPerk(unbreakablePerk)) {
					if (armorRating > hardCap) {
						response = softCap * armorFactor + (hardCap - softCap) * diminishedFactor;
					}
					else {
						const auto remainder = hardCap - armorRating;
						response = remainder * diminishedFactor + softCap * armorFactor;
					}
				}
				else {
					response = softCap * armorFactor;
				}
			}
			else {
				response = softCap * armorFactor;
			}
		}
		return response;
	}
}