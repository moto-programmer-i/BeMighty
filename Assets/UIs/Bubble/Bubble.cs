using System;
using UnityEngine.UIElements;

[UxmlElement]
public partial class Bubble : VisualElement {
    const string HP_LABEL = "HP";

    const string CLASS_NAME = "bubble";
    const string CLASS_NAME_NAME_WRAPPER = "name-wrapper";
    const string CLASS_NAME_NAME = "name";
    const string CLASS_NAME_TEXT_WRAPPER = "text-wrapper";
    const string CLASS_NAME_TEXT = "text";

    private VisualElement nameWrapper;
    private Label bubbleName;
    private Label text;

    public Bubble()
    {
        // バージョン6になってもここに書かなければいけない。何とかならんのか
        

        // なぜかここでは、拡張メソッドはthis.を省略できない
        // Add(new VisualElement(), CLASS_NAME);
        var panel = this.Add(new VisualElement(), CLASS_NAME);
        nameWrapper = panel.Add(new VisualElement(), CLASS_NAME_NAME_WRAPPER);
        bubbleName = nameWrapper.Add(new Label(), CLASS_NAME_NAME);
        text = panel.Add(new VisualElement(), CLASS_NAME_TEXT_WRAPPER)
            .Add(new Label(), CLASS_NAME_TEXT);
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="name">nullまたは空の場合は非表示にする</param>
    public void SetName(string name)
    {
        bubbleName.text = name;

        // 名前がない場合は表示しない
        nameWrapper.SetDisplay(!string.IsNullOrEmpty(name));
    }

    public void SetText(string text)
    {
        this.text.text = text;
    }
}
