using System;
using System.Collections;
using Unity.VisualScripting;
using UnityEngine;

public static class MonoBehaivourExtensions
{
    /// <summary>
    /// 定期的にactionを実行する
    /// </summary>
    /// <param name="instance">呼び出し元</param>
    /// <param name="seconds">待ち時間</param>
    /// <param name="action"></param>
    public static void Loop(this MonoBehaviour instance, float seconds, Action action)
    {        
        StartLoop(instance, seconds, action);
    }

    private static async void StartLoop(MonoBehaviour instance, float seconds, Action action)
    {
        if (seconds <= 0) {
            throw new ArgumentException($"ループの秒数は0を超えている必要があります\nseconds: {seconds}");
        }

        try
        {
            while (instance != null && instance.isActiveAndEnabled && !instance.IsDestroyed())
            {
                action();
                await Awaitable.WaitForSecondsAsync(seconds, instance.destroyCancellationToken);
            }
        }
        // 終了時に例外で飛んでくるので無視
        catch (OperationCanceledException e)
        {
            _ = e;
        }
    }
}
