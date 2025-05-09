using UnityEngine;
using UnityEngine.UIElements;

public class HPArea : MonoBehaviour
{
    const string MAIN_NAME = "main";
    [SerializeField]
    private UIDocument hpArea;

    /// <summary>
    /// 主人公
    /// </summary>
    public HPPanel Main {private set; get;}
    
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Awake()
    {
        Main = hpArea.rootVisualElement.Q<HPPanel>(MAIN_NAME);
    }
}
