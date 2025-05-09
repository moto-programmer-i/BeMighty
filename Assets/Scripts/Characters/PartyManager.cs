using Mono.Cecil.Cil;
using UnityEngine;

public class PartyManager : MonoBehaviour
{    
    /// <summary>
    /// 主人公
    /// </summary>
    [SerializeField]
    private Character main;

    [SerializeField]
    private HPArea hPArea;

    void Start()
    {
        main.HPPanel = hPArea.Main;
        main.Sync();
    }

    /// <summary>
    /// 主人公を取得
    /// </summary>
    /// <returns></returns>
    public Character GetMain()
    {
        return main;
    }
}
