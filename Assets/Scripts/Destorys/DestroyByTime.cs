using UnityEngine;

public class DestroyByTime : MonoBehaviour
{
    /// <summary>
    /// 削除されるまでの時間
    /// </summary>
    [SerializeField]
    private float seconds = 5;

    void Start()
    {
        // seconds秒後に削除される
        Destroy(this.gameObject, seconds);
    }
}
