using System;
using System.Linq;
using UnityEditor;
using UnityEngine;
using UnityEngine.UIElements;

public class Talk : MonoBehaviour
{
    public const string MAIN_BUBBLE_NAME = "main-bubble";
    public const string BUBBLE_NAME = "bubble";
    public const string ORIGIN_NAME = "origin";
    [SerializeField]
    private UIDocument talk;

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
    private VisualElement bubble;

    void Start()
    {
        // origin = talk.rootVisualElement.Q<VisualElement>(ORIGIN_NAME);
        // bubble = talk.rootVisualElement.Q<VisualElement>(BUBBLE_NAME);
        Main = talk.rootVisualElement.Q<Bubble>(MAIN_BUBBLE_NAME);
        Main.SetName(bubbleName);
        Main.SetText(bubbleText);

        StickBubble(mainSpeaker, mainCamera, stickPeriod);
    }

    public void SetBubblePosition(Transform target, Camera camera)
    {
        
        Main.SetBottom(target, talk, camera);
        // Debug.Log($"吹き出し位置情報 {Main.worldBound}");

        // 画面外にいったら非表示
        if (Main.IsOffScreen())
        {
            enabled = false;
        }
    }

    public void StickBubble(Transform target, Camera camera, float stickPeriod)
    {
        this.Loop(stickPeriod, () => SetBubblePosition(target, camera));
    }
}
