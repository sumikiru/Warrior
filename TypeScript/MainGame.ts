// 注意：需要在这里导入每个ts文件！
import "./IncludedFilesPath"
import * as UE from "ue";
import {argv} from "puerts";

const GameInstance = argv.getByName("GameInstance") as UE.WarriorGameInstance;
GameInstance.Call.Bind((FunctionName, InObj) => {
    (InObj as any)[FunctionName]();
});