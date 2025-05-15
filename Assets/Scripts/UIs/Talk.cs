using UnityEngine;
using UnityEngine.UIElements;

public class Talk : MonoBehaviour
{
    const string MAIN_BUBBLE_NAME = "main-bubble";
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

    void Start()
    {
        Main = talk.rootVisualElement.Q<Bubble>(MAIN_BUBBLE_NAME);
        Main.SetName(bubbleName);
        Main.SetText(bubbleText);
    }
}
