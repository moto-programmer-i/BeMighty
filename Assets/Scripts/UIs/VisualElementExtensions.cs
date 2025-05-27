using UnityEngine;
using UnityEngine.UIElements;

public static class VisualElementExtensions
{
    const float MAX_PERCENT = 100;
    const float HALF_PERCENT = 50;

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

    /// <summary>
    /// instance.style.bottomを設定
    /// 座標は上からだが、bottomは下からなので注意
    /// </summary>
    /// <param name="instance"></param>
    /// <param name="position"></param>
    public static void SetLeftBottom(this VisualElement instance, Vector2 position)
    {
        instance.style.left = position.x;
        instance.style.bottom = position.y;
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
    /// translate x を -50%に設定
    /// </summary>
    /// <param name="instance"></param>
    public static void TranslateXToMid(this VisualElement instance)
    {
        instance.style.translate = new Translate(Length.Percent(-HALF_PERCENT), Length.Percent(HALF_PERCENT));
    }




    /// <summary>
    /// 画面外か
    /// </summary>
    /// <param name="instance"></param>
    /// <returns></returns>
    public static bool IsOffScreen(this VisualElement instance)
    {
        float left = instance.worldBound.position.x;
        float top = instance.worldBound.position.y;

        // 右と下は左上 + 大きさ
        return IsOffScreen(left, top, left + instance.worldBound.width, top + instance.worldBound.height);
    }

    /// <summary>
    /// 画面外か
    /// 引数は各座標
    /// </summary>
    /// <param name="left"></param>
    /// <param name="top"></param>
    /// <param name="right"></param>
    /// <param name="bottom"></param>
    /// <returns></returns>
    public static bool IsOffScreen(float left, float top, float right, float bottom)
    {
        var resolution = UIManager.GetResolution();

        return left >= resolution.X
            || top >= resolution.Y
            || right <= UIManager.SCREEN_MIN
            || bottom <= UIManager.SCREEN_MIN
        ;
    }
}

