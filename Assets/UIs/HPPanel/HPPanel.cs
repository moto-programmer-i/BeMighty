using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.UIElements;

[UxmlElement]
public partial class HPPanel : VisualElement {
    const int DEFAULT_HP = 9;
    const string HP_LABEL = "HP";

    const string CLASS_NAME = "hp-panel";
    const string CLASS_NAME_IMAGE = "hp-image";
    const string CLASS_NAME_SIDE = "hp-panel-side";
    const string CLASS_NAME_LINE = "hp-line";
    const string CLASS_NAME_LABEL = "hp-label";
    const string CLASS_NAME_HP = "hp";
    public HPPanel()
    {
        // バージョン6になってもここに書かなければいけない。何とかならんのか
        

        // なぜかここでは、拡張メソッドはthis.を省略できない
        // Add(new VisualElement(), CLASS_NAME);
        var panel = this.Add(new VisualElement(), CLASS_NAME);
        panel.Add(new VisualElement(), CLASS_NAME_IMAGE);

        var side = panel.Add(new VisualElement(), CLASS_NAME_SIDE);
        var line = side.Add(new VisualElement(), CLASS_NAME_LINE);

        line.Add(new Label(), CLASS_NAME_LABEL)
            .text = HP_LABEL;
        line.Add(new Label(), CLASS_NAME_HP)
            .text = DEFAULT_HP.ToString();
    }
}
