using UnityEngine;
using UnityEngine.UIElements;

public static class VisualElementExtensions
{
    const float MAX_PERCENT = 100;

    /// <summary>
    /// VisualElementに子を追加する
    /// (コンストラクタ内から呼び出す場合、なぜか this.Add のように、thisをつけないとエラーになる)
    /// </summary>
    /// <param name="instance"></param>
    /// <param name="child">子</param>
    /// <param name="className">USSクラス名</param>
    /// <returns>子</returns>
    public static T Add<T>(this VisualElement instance, T child, string className) where T : VisualElement
    {
        instance.Add(child);
        child.AddToClassList(className);

        return child;
    }

    /// <summary>
    /// VisualElement.style.displayを設定
    /// </summary>
    /// <param name="instance"></param>
    /// <param name="display">true: Flex, false:None</param>
    public static void SetDisplay(this VisualElement instance, bool display)
    {
        instance.style.display = display ? DisplayStyle.Flex : DisplayStyle.None;
    }

    /// <summary>
    /// instance.style.left = Length.Percent(
    /// </summary>
    /// <param name="instance"></param>
    /// <param name="rawValue">内部で* 100される。</param>
    public static void SetLeftByPercent(this VisualElement instance, float rawValue)
    {
        instance.style.left = Length.Percent(rawValue * MAX_PERCENT);
    }

    /// <summary>
    /// instance.style.top = Length.Percent(
    /// </summary>
    /// <param name="instance"></param>
    /// <param name="rawValue">内部で* 100される。</param>
    public static void SetTopByPercent(this VisualElement instance, float rawValue)
    {
        instance.style.top = Length.Percent(rawValue * MAX_PERCENT);
    }

    /// <summary>
    /// instance.style.left と topを設定
    /// </summary>
    /// <param name="instance"></param>
    /// <param name="position"></param>
    public static void SetLeftTop(this VisualElement instance, Vector2 position)
    {
        instance.style.left = position.x;
        instance.style.top = position.y;
    }

    public static float GetLeft(this VisualElement instance)
    {
        return instance.style.left.value.value;
    }

    public static float GetTop(this VisualElement instance)
    {
        return instance.style.top.value.value;
    }
    public static float GetRight(this VisualElement instance)
    {
        return instance.style.right.value.value;
    }

    public static float GetBottom(this VisualElement instance)
    {
        return instance.style.bottom.value.value;
    }




    /// <summary>
    /// 画面外か
    /// </summary>
    /// <param name="instance">absoluteでLeft Topが設定されていることが前提</param>
    /// <returns></returns>
    public static bool IsOffScreen(this VisualElement instance)
    {
        var resolution = UIManager.GetResolution();

        // absoluteでleft topが設定されていることが前提
        // ほかの場合もうまく対処できるように要検討
        float left = instance.GetLeft();
        float top = instance.GetTop();

        // Debug.Log($"pos {left}:{top}");

        // 右と下は左上 + 大きさ
        float right = instance.GetLeft() + instance.resolvedStyle.width;
        float bottom = instance.GetTop() + instance.resolvedStyle.height;


        return left >= resolution.X
            || top >= resolution.Y
            || right <= UIManager.SCREEN_MIN
            || bottom <= UIManager.SCREEN_MIN
        ;
    }
}

