// Fill out your copyright notice in the Description page of Project Settings.


#include "WarriorGameplayTags.h"

namespace WarriorGameplayTags
{
	/** =======================================Input Tags=============================================== */
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, "InputTag.Look");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_EquipAxe, "InputTag.EquipAxe");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_UnequipAxe, "InputTag.UnequipAxe");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_LightAttack_Axe, "InputTag.LightAttack.Axe");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_HeavyAttack_Axe, "InputTag.HeavyAttack.Axe");

	/** =======================================Player Tags============================================== */
	// Ability Tags
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Equip_Axe, "Player.Ability.Equip.Axe");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Unequip_Axe, "Player.Ability.Unequip.Axe");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Light_Axe, "Player.Ability.Attack.Light.Axe");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Heavy_Axe, "Player.Ability.Attack.Heavy.Axe");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_HitPause, "Player.Ability.HitPause");

	// Weapon Tags
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Axe, "Player.Weapon.Axe");

	// Play Event Tags
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Equip_Axe, "Player.Event.Equip.Axe");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Unequip_Axe, "Player.Event.Unequip.Axe");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_HitPause, "Player.Event.HitPause");

	// Status Tags
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_JumpToFinisher, "Player.Status.JumpToFinisher");

	// Combo Tags
	UE_DEFINE_GAMEPLAY_TAG(Player_Combo_ResetComboTimer, "Player.Combo.ResetComboTimer");
	UE_DEFINE_GAMEPLAY_TAG(Player_Combo_LightAttack_Axe_1, "Player.Combo.LightAttack.Axe.1");
	UE_DEFINE_GAMEPLAY_TAG(Player_Combo_LightAttack_Axe_2, "Player.Combo.LightAttack.Axe.2");
	UE_DEFINE_GAMEPLAY_TAG(Player_Combo_LightAttack_Axe_3, "Player.Combo.LightAttack.Axe.3");
	UE_DEFINE_GAMEPLAY_TAG(Player_Combo_LightAttack_Axe_4, "Player.Combo.LightAttack.Axe.4");
	UE_DEFINE_GAMEPLAY_TAG(Player_Combo_HeavyAttack_Axe_1, "Player.Combo.HeavyAttack.Axe.1");
	UE_DEFINE_GAMEPLAY_TAG(Player_Combo_HeavyAttack_Axe_2, "Player.Combo.HeavyAttack.Axe.2");

	// Set By Caller
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_AttackType_Light, "Player.SetByCaller.AttackType.Light");
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_AttackType_Heavy, "Player.SetByCaller.AttackType.Heavy");

	/** =======================================Enemy Tags=============================================== */
	// Ability Tags
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Melee, "Enemy.Ability.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Ranged, "Enemy.Ability.Ranged");
	
	// Weapon Tags
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Weapon, "Enemy.Weapon");

	/** =======================================Shared Tags=============================================== */
	// Ability Tags
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_HitReact, "Shared.Ability.HitReact");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_Death, "Shared.Ability.Death");
	
	// Event Tags
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_MeleeHit, "Shared.Event.MeleeHit");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_HitReact, "Shared.Event.HitReact");

	// Set By Caller
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_BaseDamage, "Shared.SetByCaller.BaseDamage");

	// Status Tags
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Dead, "Shared.Status.Dead");
}

