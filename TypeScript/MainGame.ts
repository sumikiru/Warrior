import "./Blueprints/HotReloadTest/BP_Cube"
import * as UE from "ue";
import {argv} from "puerts";

const GameInstance = argv.getByName("GameInstance") as UE.WarriorGameInstance;
GameInstance.Call.Bind((FunctionName, InObj) => {
    (InObj as any)[FunctionName]();
});