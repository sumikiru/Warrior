import * as UE from 'ue';
import {blueprint} from "puerts";

const uclass = UE.Class.Load("/Game/Blueprints/PlayerCharacter/GameplayAbility/GA_Hero_EquipAxe.GA_Hero_EquipAxe_C");   // 注意_C后缀
const jsClass = blueprint.tojs<typeof UE.Game.Blueprints.PlayerCharacter.GameplayAbility.GA_Hero_EquipAxe.GA_Hero_EquipAxe_C>(uclass);

interface TS_GA_Hero_EquipAxe extends UE.Game.Blueprints.PlayerCharacter.GameplayAbility.GA_Hero_EquipAxe.GA_Hero_EquipAxe_C {}
class TS_GA_Hero_EquipAxe implements TS_GA_Hero_EquipAxe {
    K2_ActivateAbility() {
        UE.KismetSystemLibrary.PrintString(this, "激活装备Axe");

        this.K2_EndAbility();
    }
}

blueprint.mixin(jsClass, TS_GA_Hero_EquipAxe);