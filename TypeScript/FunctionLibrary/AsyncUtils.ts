import * as UE from 'ue'
import {argv} from "puerts";

export function WaitLatentActionState(state: UE.LatentActionState) : Promise<void> {
    return new Promise<void>((resolve, reject) => {
        // 结合LatentActionState.cpp，该单播委托绑定的该匿名函数将在duration结束后执行(因为LatentActionCallback.ExecuteIfBound();)
        state.LatentActionCallback.Bind(() : void => {
            state.LatentActionCallback.Unbind();  // 解绑避免重复触发
            resolve(); // 标记Promise完成，外部await由此结束等待
        });
        // reject处理失败状态，这里没有处理
    });
}

/**
 * 加上async关键字的函数将会变成异步函数。
 * 参考https://blog.csdn.net/qq_40258073/article/details/108887291
 */
export async function DelayNode(Duration : number, PrintToLog ?: boolean) : Promise<void> {
    if (PrintToLog) {
        console.warn("Begin to delay for " + Duration.toString() + " seconds...");
    }
    let world : UE.World = (argv.getByName("GameInstance") as UE.GameInstance).GetWorld();
    let LatentActionState : UE.LatentActionState = new UE.LatentActionState();

    UE.KismetSystemLibrary.Delay(
        world, // 注意是world
        Duration,
        LatentActionState.GetLatentActionInfo()
    );

    // WaitLatentActionState的Promise状态变为“已完成”时，外部 await 由此结束等待，之后才继续后面的代码
    await WaitLatentActionState(LatentActionState);
    if (PrintToLog) {
        console.warn("Delay Finished");
    }
}

// CompletedEvent指的是Delay结束后要执行的事件
export function TS_Lib_Delay<T extends () => void>(Duration : number, PrintToLog ?: boolean, CompletedEvent?: T) : void {
    // 该函数不是异步函数，所以不能直接使用await DelayNode(Duration);
    DelayNode(Duration, PrintToLog)
        .then(() : void => {
            if (CompletedEvent) {
                CompletedEvent();
                if (PrintToLog) {
                    console.warn("Try CompletedEvent");
                }
            }
        })
        .catch((reason) : void => { console.error(reason); });
}

