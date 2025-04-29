using UnityEngine;

public class Magic : MonoBehaviour
{
    [field:SerializeField]
    public MagicType Type {get; private set;}

    void OnCollisionEnter(Collision target)
    {        
        // 相反する属性とぶつかったら相殺
        if (target.gameObject.TryGetComponent<Magic>(out var targetMagic)
            && IsOpposite(targetMagic)
        ) {
            Destroy(this);
            Destroy(target.gameObject);
        }
    }

    public bool IsOpposite(Magic target) {
        return Type.IsOpposite(target.Type);
    }
}
