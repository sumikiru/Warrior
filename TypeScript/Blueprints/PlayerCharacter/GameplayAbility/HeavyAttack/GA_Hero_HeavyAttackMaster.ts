import * as UE from 'ue';
import {$ref, blueprint} from "puerts";
import {TS_AbilityTaskFunctionLibrary} from "../../../../Interfaces/GameplayAbility/AbilityTaskFunctionLibrary";

// 前提是设置为“每个Actor实例化”
const uclass = UE.Class.Load("/Game/Blueprints/PlayerCharacter/GameplayAbility/HeavyAttack/GA_Hero_HeavyAttackMaster.GA_Hero_HeavyAttackMaster_C");
const jsClass = blueprint.tojs<typeof UE.Game.Blueprints.PlayerCharacter.GameplayAbility.HeavyAttack.GA_Hero_HeavyAttackMaster.GA_Hero_HeavyAttackMaster_C>(uclass);

interface TS_GA_Hero_HeavyAttackMaster extends UE.Game.Blueprints.PlayerCharacter.GameplayAbility.HeavyAttack.GA_Hero_HeavyAttackMaster.GA_Hero_HeavyAttackMaster_C {}
class TS_GA_Hero_HeavyAttackMaster extends TS_AbilityTaskFunctionLibrary implements TS_GA_Hero_HeavyAttackMaster {
    K2_ActivateAbility() : void {
        // Clear And Invalidate Timer By Handle
        UE.KismetSystemLibrary.K2_ClearAndInvalidateTimerHandle(this, $ref(this.ComboCountResetTimerHandle));
        // Check if should jump to finisher
        if (UE.WarriorFunctionLibrary.CPP_DoesActorHaveTag(this.GetHeroCharacterFromActorInfo(), this.JumpToFinisherTag)){
            this.CurrentHeavyAttackComboCount = this.AttackMontagesMap.Num();
        }
        super.TS_Lib_PrintDebugStringToLog("Heavy Combo " + this.CurrentHeavyAttackComboCount.toString());
        // Play Montage And Wait
        this.TS_PlayMontageAndWait();
        /* Sequence */
        // Then 0
        if (this.CurrentHeavyAttackComboCount === this.AttackMontagesMap.Num()) {
            this.BP_ResetHeavyAttackComboCount();
        }
        else {
            this.CurrentHeavyAttackComboCount++;
        }
    }

    TS_ResetHeavyAttackComboCount() : void {
        this.CurrentHeavyAttackComboCount = 1;
        super.TS_Lib_PrintDebugStringToLog("Reset Heavy Attack Combo");
        UE.WarriorFunctionLibrary.RemoveGameplayTagFromActorIfFound(
            this.GetHeroCharacterFromActorInfo(),
            this.JumpToFinisherTag);
    }

    BP_ResetHeavyAttackComboCount() {
        this.TS_ResetHeavyAttackComboCount();
        UE.KismetSystemLibrary.K2_ClearAndInvalidateTimerHandle(this, $ref(this.ComboCountResetTimerHandle));
    }

    TS_PlayMontageAndWait() : void {
        super.TS_Lib_PlayMontageAndWait_AllSameEvents(
            $ref(this.AttackMontagesMap.Get(this.CurrentHeavyAttackComboCount) ?? null),
            ()=>{
                this.K2_EndAbility();
                // Set Timer By Event
                this.BP_SetHeavyAttackTimerDelegate(3, false, this.ComboCountResetTimerHandle);
            }
        );
    }
}

blueprint.mixin(jsClass, TS_GA_Hero_HeavyAttackMaster);
