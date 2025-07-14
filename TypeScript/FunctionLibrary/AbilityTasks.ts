import * as UE from "ue";
import {$InRef, $Nullable, $unref} from "puerts";

export function TS_Lib_PrintDebugString(DebugString: string) : void {
    UE.KismetSystemLibrary.PrintString(null, DebugString, true, true, UE.LinearColor.MakeRandomColor(), 200.0);
}

export function TS_Lib_PrintDebugStringToLog(DebugString: string) : void {
    UE.KismetSystemLibrary.PrintString(null, DebugString, false, true);
}

// Play Montage And Wait, 但是Completed/BlendOut/Interrupted/Cancelled以后执行同一事件
export function TS_Lib_PlayMontageAndWait_AllSameEvents<T extends () => void>(OwningAbility : $Nullable<UE.GameplayAbility>, MontageToPlay: $InRef<$Nullable<UE.AnimMontage>>, NextEvent?: T): void {
    let PlayMontageAbilityTask: UE.AbilityTask_PlayMontageAndWait = UE.AbilityTask_PlayMontageAndWait.CreatePlayMontageAndWaitProxy(
        OwningAbility,
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

// Play Montage And Wait, 但是Completed/BlendOut/Interrupted/Cancelled以后立即执行K2_EndAbility()
export function TS_Lib_PlayMontageAndWait_EndAbility(OwningAbility : UE.GameplayAbility, MontageToPlay: $InRef<$Nullable<UE.AnimMontage>>): void {
    let PlayMontageAbilityTask: UE.AbilityTask_PlayMontageAndWait = UE.AbilityTask_PlayMontageAndWait.CreatePlayMontageAndWaitProxy(
        OwningAbility,
        "None",
        $unref(MontageToPlay));
    PlayMontageAbilityTask.OnCompleted.Add(() => {
        UE.KismetSystemLibrary.PrintString(OwningAbility, "Completed", false, true);
        OwningAbility.K2_EndAbility();
    });
    PlayMontageAbilityTask.OnBlendOut.Add(() => {
        UE.KismetSystemLibrary.PrintString(OwningAbility, "BlendOut", false, true);
        OwningAbility.K2_EndAbility();
    });
    PlayMontageAbilityTask.OnInterrupted.Add(() => {
        UE.KismetSystemLibrary.PrintString(OwningAbility, "Interrupted", false, true);
        OwningAbility.K2_EndAbility();
    });
    PlayMontageAbilityTask.OnCancelled.Add(() => {
        UE.KismetSystemLibrary.PrintString(OwningAbility, "Cancelled", false, true);
        OwningAbility.K2_EndAbility();
    });
    // Called to trigger the actual task once the delegates have been set up
    PlayMontageAbilityTask.ReadyForActivation();
}

export function TS_Lib_WaitGameplayEvent<T extends (Payload: UE.GameplayEventData) => void>(OwningAbility : $Nullable<UE.GameplayAbility>, EventTag: $InRef<UE.GameplayTag>, EventReceivedFunction?: T): void {
    let WaitGameplayEventTask: UE.AbilityTask_WaitGameplayEvent = UE.AbilityTask_WaitGameplayEvent.WaitGameplayEvent(
        OwningAbility,
        $unref(EventTag));
    // EventReceivedFunction == null时，不应该绑定事件
    if (EventReceivedFunction) {
        WaitGameplayEventTask.EventReceived.Add(EventReceivedFunction);
    }
    // 所有AbilityTask的子类在绑定Delegate后都需要手动准备好激活
    WaitGameplayEventTask.ReadyForActivation();
}
