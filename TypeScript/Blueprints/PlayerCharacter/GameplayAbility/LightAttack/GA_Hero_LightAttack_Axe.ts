import * as UE from 'ue';
import {blueprint} from "puerts";

const uclass = UE.Class.Load("/Game/Blueprints/PlayerCharacter/GameplayAbility/LightAttack/GA_Hero_LightAttack_Axe.GA_Hero_LightAttack_Axe_C");
const jsClass = blueprint.tojs<typeof UE.Game.Blueprints.PlayerCharacter.GameplayAbility.LightAttack.GA_Hero_LightAttack_Axe.GA_Hero_LightAttack_Axe_C>(uclass);

interface TS_GA_Hero_LightAttack_Axe extends UE.Game.Blueprints.PlayerCharacter.GameplayAbility.LightAttack.GA_Hero_LightAttack_Axe.GA_Hero_LightAttack_Axe_C {}
class TS_GA_Hero_LightAttack_Axe implements TS_GA_Hero_LightAttack_Axe {
    /*K2_ActivateAbility() : void {
        // super.K2_ActivateAbility();
    }*/
}

blueprint.mixin(jsClass, TS_GA_Hero_LightAttack_Axe);
