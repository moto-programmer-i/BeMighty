using UnityEngine;

/// <summary>
/// 直線移動
/// </summary>
public class LinearMotion : MonoBehaviour
{
    [SerializeField]
    private Vector3 linearVelocity;

    /// <summary>
    /// trueの場合はオブジェクトの方向に応じて移動する
    /// (linearVelocityを回転させる)
    /// </summary>
    [SerializeField]
    private bool goForward;
    
    void Start()
    {
        var rigidbody = GetComponent<Rigidbody>();
        rigidbody.linearVelocity = goForward ? 
            Quaternion.FromToRotation(linearVelocity, transform.forward) * linearVelocity:
            linearVelocity;
    } 
}
