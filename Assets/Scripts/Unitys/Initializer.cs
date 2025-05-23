using UnityEngine;
using UnityEngine.UIElements;

/// <summary>
/// 各Managerクラスを初期化
/// </summary>
public class Initializer : MonoBehaviour
{
    [SerializeField]
    private UIDocument talk;

    void Awake()
    {
        // 他クラスの設定
        new UIManager(talk.panelSettings);

        // 終わったら不要
        Destroy(this);
    }
}
