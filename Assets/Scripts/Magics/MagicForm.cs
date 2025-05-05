using UnityEngine;

public class MagicForm : MonoBehaviour
{
    [SerializeField]
    private float delaySeconds;

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
