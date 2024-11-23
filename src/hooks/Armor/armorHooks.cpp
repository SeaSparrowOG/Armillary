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

		REL::Relocation<std::uintptr_t> advanceSkillTarget{ REL::ID(40488), 0x25 };
		REL::Relocation<std::uintptr_t> incrementSkillTarget{ REL::ID(40489), 0x13 };

		if (!REL::make_pattern<"E8">().match(advanceSkillTarget.address()) ||
			!REL::make_pattern<"E9">().match(incrementSkillTarget.address())) {

			util::report_and_fail("Armor: Failed to validate pattern, aborting launch.");
		}

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
		_handleSkillIncrease = trampoline.write_call<5>(advanceSkillTarget.address(), HandleSkillIncrease);
		_handleSkillIncrement = trampoline.write_branch<5>(incrementSkillTarget.address(), HandleSkillIncrement);
	}

	float Manager::VisitArmorRating(RE::Character* a_target)
	{
		constexpr float armorFactor = 0.001f;
		constexpr float diminishedFactor = 0.0005f;
		constexpr float softCap = 700.0f;
		constexpr float hardCap = 1000.0f;

		const auto armorRating = a_target ? a_target->GetActorValue(RE::ActorValue::kDamageResist) : 0.0f;
		const auto unbreakablePerk = Data::ModObject<RE::BGSPerk>("ARM_Armor_PRK_100_Unbreakable");
		assert(unbreakablePerk);

		auto response = 0.0f;
		logger::debug("Armor: {}", armorRating);
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
				response = armorRating * armorFactor;
			}
		}
		return response;
	}

	void Manager::HandleSkillIncrease(RE::PlayerCharacter* a_char, RE::ActorValue a_skill, float a_ammount, RE::TESForm* a_advanceObject,
		uint32_t a_advanceAction, bool a_useModifiers, bool a_hideNotification)
	{
		if (a_skill == RE::ActorValue::kLightArmor) {
			_handleSkillIncrease(a_char, RE::ActorValue::kHeavyArmor, a_ammount, a_advanceObject, a_advanceAction,
				a_useModifiers, a_hideNotification);
		}
		else {
			_handleSkillIncrease(a_char, a_skill, a_ammount, a_advanceObject, a_advanceAction,
				a_useModifiers, a_hideNotification);
		}
	}

	void Manager::HandleSkillIncrement(RE::PlayerCharacter* a_player, RE::ActorValue a_skill, unsigned int a_amount)
	{
		if (a_skill == RE::ActorValue::kLightArmor) {
			_handleSkillIncrement(a_player, RE::ActorValue::kHeavyArmor, a_amount);
		}
		else {
			_handleSkillIncrement(a_player, a_skill, a_amount);
		}
	}
}