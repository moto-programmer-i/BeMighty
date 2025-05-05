

using System;
using UnityEngine;

public static class AsyncUtils
{
    public static async void Delay(float seconds, Action action)
    {
        await Awaitable.WaitForSecondsAsync(seconds);
        action();
    }
}
