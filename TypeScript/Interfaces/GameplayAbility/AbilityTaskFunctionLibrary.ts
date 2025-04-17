import * as UE from "ue";
import {$InRef, $Nullable, $unref} from "puerts";

export abstract class TS_AbilityTaskFunctionLibrary extends UE.GameplayAbility {
    // Play Montage And Wait, 但是Completed/BlendOut/Interrupted/Cancelled以后立即执行K2_EndAbility()
    TS_Lib_PlayMontageAndWait_EndAbility(MontageToPlay: $InRef<$Nullable<UE.AnimMontage>>): void {
        let PlayMontageAbilityTask: UE.AbilityTask_PlayMontageAndWait = UE.AbilityTask_PlayMontageAndWait.CreatePlayMontageAndWaitProxy(
            this,
            "None",
            $unref(MontageToPlay));
        PlayMontageAbilityTask.OnCompleted.Add(() => {
            UE.KismetSystemLibrary.PrintString(this, "Completed", false, true);
            this.K2_EndAbility();
        });
        PlayMontageAbilityTask.OnBlendOut.Add(() => {
            UE.KismetSystemLibrary.PrintString(this, "BlendOut", false, true);
            this.K2_EndAbility();
        });
        PlayMontageAbilityTask.OnInterrupted.Add(() => {
            UE.KismetSystemLibrary.PrintString(this, "Interrupted", false, true);
            this.K2_EndAbility();
        });
        PlayMontageAbilityTask.OnCancelled.Add(() => {
            UE.KismetSystemLibrary.PrintString(this, "Cancelled", false, true);
            this.K2_EndAbility();
        });
        // Called to trigger the actual task once the delegates have been set up
        PlayMontageAbilityTask.ReadyForActivation();
    }

    // Play Montage And Wait, 但是Completed/BlendOut/Interrupted/Cancelled以后执行同一事件
    TS_Lib_PlayMontageAndWait_AllSameEvents<T extends () => void>(MontageToPlay: $InRef<$Nullable<UE.AnimMontage>>, NextEvent?: T): void {
        let PlayMontageAbilityTask: UE.AbilityTask_PlayMontageAndWait = UE.AbilityTask_PlayMontageAndWait.CreatePlayMontageAndWaitProxy(
            this,
            "None",
            $unref(MontageToPlay));
        if (NextEvent) {
            PlayMontageAbilityTask.OnCompleted.Add(NextEvent);
            PlayMontageAbilityTask.OnBlendOut.Add(NextEvent);
            PlayMontageAbilityTask.OnInterrupted.Add(NextEvent);
            PlayMontageAbilityTask.OnCancelled.Add(NextEvent);
        }
        // Called to trigger the actual task once the delegates have been set up
        PlayMontageAbilityTask.ReadyForActivation();
    }

    TS_Lib_WaitGameplayEvent<T extends (Payload: UE.GameplayEventData) => void>(EventTag: $InRef<UE.GameplayTag>, EventReceivedFunction?: T): void {
        let WaitGameplayEventTask: UE.AbilityTask_WaitGameplayEvent = UE.AbilityTask_WaitGameplayEvent.WaitGameplayEvent(
            this,
            $unref(EventTag));
        // EventReceivedFunction == null时，不应该绑定事件
        if (EventReceivedFunction) {
            WaitGameplayEventTask.EventReceived.Add(EventReceivedFunction);
        }
        // 所有AbilityTask的子类在绑定Delegate后都需要手动准备好激活
        WaitGameplayEventTask.ReadyForActivation();
    }

    TS_Lib_PrintDebugString(DebugString: string) : void {
        UE.KismetSystemLibrary.PrintString(this, DebugString, true, true, UE.LinearColor.Blue, 200.0);
    }

    TS_Lib_PrintDebugStringToLog(DebugString: string) : void {
        UE.KismetSystemLibrary.PrintString(this, DebugString, false, true);
    }
}
