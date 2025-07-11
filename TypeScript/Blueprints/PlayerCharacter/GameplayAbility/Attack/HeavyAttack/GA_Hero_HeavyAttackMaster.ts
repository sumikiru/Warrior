import * as UE from 'ue';
import {blueprint} from "puerts";

// 前提是设置为“每个Actor实例化”
const uclass = UE.Class.Load("/Game/Blueprints/PlayerCharacter/GameplayAbility/Attack/HeavyAttack/GA_Hero_HeavyAttackMaster.GA_Hero_HeavyAttackMaster_C");
const jsClass = blueprint.tojs<typeof UE.Game.Blueprints.PlayerCharacter.GameplayAbility.Attack.HeavyAttack.GA_Hero_HeavyAttackMaster.GA_Hero_HeavyAttackMaster_C>(uclass);

interface TS_GA_Hero_HeavyAttackMaster extends UE.Game.Blueprints.PlayerCharacter.GameplayAbility.Attack.HeavyAttack.GA_Hero_HeavyAttackMaster.GA_Hero_HeavyAttackMaster_C {}
class TS_GA_Hero_HeavyAttackMaster implements TS_GA_Hero_HeavyAttackMaster {
    /*K2_ActivateAbility() : void {

    }*/
}

blueprint.mixin(jsClass, TS_GA_Hero_HeavyAttackMaster);
