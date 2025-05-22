using System;
using UnityEngine;
using UnityEngine.UIElements;

public class Talk : MonoBehaviour
{
    const int SCREEN_MIN = 0;
    public const string MAIN_BUBBLE_NAME = "main-bubble";
    public const string ORIGIN_NAME = "origin";
    [SerializeField]
    private UIDocument talk;

    /// <summary>
    /// 主人公
    /// </summary>
    public Bubble Main {private set; get;}

    [SerializeField]
    private string bubbleName;

    [SerializeField,TextArea]
    private string bubbleText;

    [SerializeField]
    private Camera mainCamera;


    [SerializeField]
    private Transform mainSpeaker;

    [SerializeField]
    private float stickPeriod = 0.5f;

    private VisualElement origin;

    void Start()
    {
        origin = talk.rootVisualElement.Q<VisualElement>(ORIGIN_NAME);
        Main = talk.rootVisualElement.Q<Bubble>(MAIN_BUBBLE_NAME);
        Main.SetName(bubbleName);
        Main.SetText(bubbleText);

        StickBubble(mainSpeaker, mainCamera, stickPeriod);

        // Debug.Log($"min:{MathF.Min(float.NaN, 0)}, max:{MathF.Max(float.NaN, Screen.height)}");
    }

    public void SetBubblePosition(Vector3 position, Camera camera)
    {
        // 2Dの座標系とUIが違うらしく、WorldToScreenPointではうまくいかない
        // var mainSpakerPosition = camera.WorldToScreenPoint(position);
        // RuntimePanelUtils.CameraTransformWorldToPanelだとうまくいく
        var mainSpakerPosition = RuntimePanelUtils.CameraTransformWorldToPanel(talk.rootVisualElement.panel, position, mainCamera);
        origin.SetLeftTop(mainSpakerPosition);






        // Debug.Log("対象x "+ mainSpakerPosition.x);
        // // talk.rootVisualElement.style.left = mainSpakerPosition.x;
        // // talk.rootVisualElement.style.top = Screen.height - mainSpakerPosition.y;

        // origin.style.left = Length.Percent(50);
        // origin.style.top = Length.Percent(50);
        // // Debug.Log($"吹き出しサイズ {talk.rootVisualElement.resolvedStyle.width}: {talk.rootVisualElement.resolvedStyle.height}");

        // origin.SetLeftByPercent(mainSpakerPosition.x / Screen.width);
        // origin.SetTopByPercent((Screen.height - mainSpakerPosition.y) / Screen.width);

        // Debug.Log($"対象 {origin.style.left}:{origin.style.top}");
        // return;


        // 吹き出しがはみださないように収める
        // origin.SetLeftByPercent(
        //     Mathf.Clamp(mainSpakerPosition.x,
        //         SCREEN_MIN, Screen.width - origin.resolvedStyle.width)
        //     / Screen.width
        //     );

        // origin.SetTopByPercent(
        //     Mathf.Clamp(
        //         // y座標は下が0だが、topは上が0
        //         Screen.height - mainSpakerPosition.y,

        //         SCREEN_MIN, Screen.height - origin.resolvedStyle.height)
        //     / Screen.height
        //     );
    }

    public void StickBubble(Transform target, Camera camera, float stickPeriod)
    {
        this.Loop(stickPeriod, () => SetBubblePosition(target.position, camera));
    }
}
