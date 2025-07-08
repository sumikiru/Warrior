// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

namespace WarriorGameplayTags
{
	/** Input Tags */
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_EquipAxe);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_UnequipAxe);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LightAttack_Axe);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_HeavyAttack_Axe);

	/** Player Tags */
	// Ability Tags
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Equip_Axe);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Unequip_Axe);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Light_Axe);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Heavy_Axe);

	// Weapon Tags
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Weapon_Axe);

	// Play Event Tags
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Equip_Axe);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Unequip_Axe);

	// Status Tags
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_JumpToFinisher);

	// Combo Tags
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Combo_LightAttack_Axe_1);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Combo_LightAttack_Axe_2);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Combo_LightAttack_Axe_3);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Combo_LightAttack_Axe_4);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Combo_HeavyAttack_Axe_1);
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Combo_HeavyAttack_Axe_2);

	/** Enemy Tags */
	// Weapon Tags
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Weapon);

	/** Shared Tags */
	// Event Tags
	WARRIOR_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_MeleeHit);
}
