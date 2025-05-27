using System;
using UnityEngine;
using UnityEngine.UIElements;

[UxmlElement]
public partial class Bubble : VisualElement
{
    public const string DEFAULT_NAME = "キャラクター名";
    public const string DEFAULT_TEXT = "デフォルト\nテキスト";
    const string HP_LABEL = "HP";

    const string CLASS_NAME = "bubble";
    const string CLASS_NAME_NAME_WRAPPER = "name-wrapper";
    const string CLASS_NAME_NAME = "name";
    const string CLASS_NAME_TEXT_WRAPPER = "text-wrapper";
    const string CLASS_NAME_TEXT = "text";
    private VisualElement nameWrapper;
    private Label bubbleName;
    private Label text;



    public Bubble() : this(DEFAULT_NAME, DEFAULT_TEXT)
    {
    }

    public Bubble(string name, string text)
    {
        // バージョン6になってもここに書かなければいけない。何とかならんのか


        // 左右中央
        style.position = Position.Absolute;
        this.TranslateXToMid();


        // なぜかここでは、拡張メソッドはthis.を省略できない
        // Add(new VisualElement(), CLASS_NAME);
        var panel = this.Add(new VisualElement(), CLASS_NAME);
        nameWrapper = panel.Add(new VisualElement(), CLASS_NAME_NAME_WRAPPER);
        bubbleName = nameWrapper.Add(new Label(), CLASS_NAME_NAME);
        this.text = panel.Add(new VisualElement(), CLASS_NAME_TEXT_WRAPPER)
            .Add(new Label(), CLASS_NAME_TEXT);

        SetName(name);
        SetText(text);
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

    /// <summary>
    /// 吹き出しの位置設定（下を基点）
    /// </summary>
    /// <param name="target"></param>
    /// <param name="document"></param>
    /// <param name="camera"></param>
    public void SetBottom(Transform target, UIDocument document, Camera camera)
    {
        // 2Dの座標系とUIが違うので、WorldToScreenPointではうまくいかない
        // var mainSpakerPosition = camera.WorldToScreenPoint(position);
        // RuntimePanelUtils.CameraTransformWorldToPanelだとうまくいく
        var speakerPosition = RuntimePanelUtils.CameraTransformWorldToPanel(document.rootVisualElement.panel, target.position, camera);

        // bottomは下から設定なので、yを変換
        speakerPosition.y = UIManager.ConvertToBottom(speakerPosition.y);

        // 位置を設定
        this.SetLeftBottom(speakerPosition);


    }
}
