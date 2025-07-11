import * as UE from 'ue';
import {blueprint} from "puerts";

const uclass = UE.Class.Load("/Game/Blueprints/PlayerCharacter/GameplayAbility/Attack/HeavyAttack/GA_Hero_HeavyAttack_Axe.GA_Hero_HeavyAttack_Axe_C");
const jsClass = blueprint.tojs<typeof UE.Game.Blueprints.PlayerCharacter.GameplayAbility.Attack.HeavyAttack.GA_Hero_HeavyAttack_Axe.GA_Hero_HeavyAttack_Axe_C>(uclass);

interface TS_GA_Hero_HeavyAttack_Axe extends UE.Game.Blueprints.PlayerCharacter.GameplayAbility.Attack.HeavyAttack.GA_Hero_HeavyAttack_Axe.GA_Hero_HeavyAttack_Axe_C {}
class TS_GA_Hero_HeavyAttack_Axe implements TS_GA_Hero_HeavyAttack_Axe {
    /*K2_ActivateAbility() : void {

    }*/
}

blueprint.mixin(jsClass, TS_GA_Hero_HeavyAttack_Axe);
