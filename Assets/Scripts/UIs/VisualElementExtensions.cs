using UnityEngine.UIElements;

public static class VisualElementExtensions
{
    /// <summary>
    /// VisualElementに子を追加する
    /// (コンストラクタ内から呼び出す場合、なぜか this.Add のように、thisをつけないとエラーになる)
    /// </summary>
    /// <param name="instance"></param>
    /// <param name="child">子</param>
    /// <param name="className">USSクラス名</param>
    /// <returns>子</returns>
    public static T Add<T>(this VisualElement instance, T child, string className) where T: VisualElement
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
}
