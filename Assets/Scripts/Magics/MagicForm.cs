using UnityEngine;

/// <summary>
/// 魔法陣。魔法を出力する。
/// </summary>
public class MagicForm : MonoBehaviour
{
    [SerializeField]
    private float delaySeconds;

    /// <summary>
    /// 魔法
    /// </summary>
    [SerializeField]
    private GameObject prefab;


    void Start()
    {
        // 魔法陣生成
        AsyncUtils.Delay(delaySeconds, () =>
        {
            // ディレイして魔法生成、魔法陣削除
            InstantiateAsync(prefab, transform.position, transform.rotation);
            prefab = null;
            Destroy(this.gameObject);
        });
    }
}
