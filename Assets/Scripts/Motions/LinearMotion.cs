using UnityEngine;

public class LinearMotion : MonoBehaviour
{
    [SerializeField]
    private Vector3 linearVelocity;
    
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        var rigidbody = GetComponent<Rigidbody>();
        rigidbody.linearVelocity = linearVelocity;
    }

    
}
