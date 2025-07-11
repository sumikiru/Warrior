import * as UE from 'ue';
import {blueprint} from "puerts";

const uclass = UE.Class.Load("/Game/Blueprints/PlayerCharacter/GameplayAbility/Attack/LightAttack/GA_Hero_LightAttackMaster.GA_Hero_LightAttackMaster_C");
const jsClass = blueprint.tojs<typeof UE.Game.Blueprints.PlayerCharacter.GameplayAbility.Attack.LightAttack.GA_Hero_LightAttackMaster.GA_Hero_LightAttackMaster_C>(uclass);

interface TS_GA_Hero_LightAttackMaster extends UE.Game.Blueprints.PlayerCharacter.GameplayAbility.Attack.LightAttack.GA_Hero_LightAttackMaster.GA_Hero_LightAttackMaster_C {}
class TS_GA_Hero_LightAttackMaster implements TS_GA_Hero_LightAttackMaster {
    /*K2_ActivateAbility() : void {

    }*/
}

blueprint.mixin(jsClass, TS_GA_Hero_LightAttackMaster);


