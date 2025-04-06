import * as UE from 'ue';
import {$Ref, $ref, $set, $unref, blueprint} from "puerts";

const uclass = UE.Class.Load("/Game/Blueprints/HotReloadTest/BP_Cube.BP_Cube_C");   // 注意_C后缀
const jsClass = blueprint.tojs<typeof UE.Game.Blueprints.HotReloadTest.BP_Cube.BP_Cube_C>(uclass);

interface TS_Cube extends UE.Game.Blueprints.HotReloadTest.BP_Cube.BP_Cube_C {}
class TS_Cube implements TS_Cube {
    /*ReceiveBeginPlay(): void {
        console.log("这条消息来自BP_Cube");
    }*/

    ReceiveTick(DeltaSeconds: number) {
        this.K2_AddActorLocalRotation(
            new UE.Rotator(0, 0, DeltaSeconds * 100),
            false,
            $ref<UE.HitResult>(),
            false);
        this.BP_PrintHelloWorld();
    }

    TS_Bigger(){
        this.SetActorScale3D(UE.Vector.OneVector.op_Multiply(2));
    }

    TS_PrintHelloWorld(InputStr: $Ref<string>, OutputStr: $Ref<string>) {
        $set(OutputStr, "Hello World from TS_Cube " + $unref(InputStr));
    }
}

blueprint.mixin(jsClass, TS_Cube);