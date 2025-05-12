using UnityEngine;

public class Magic : MonoBehaviour
{
    public const string TAG_NAME = "Magic";

    public const int DEFAULT_DAMAGE = 1;
    
    [field:SerializeField]
    public MagicType Type {get; private set;}

    public bool IsOpposite(Magic target) {
        return Type.IsOpposite(target.Type);
    }

    void OnCollisionEnter(Collision target)
    {
        // 相反する属性とぶつかったら相殺
        if (target.gameObject.TryGetComponent<Magic>(out var targetMagic)
            && IsOpposite(targetMagic)
        ) {
            Destroy(gameObject);
            Destroy(target.gameObject);
            return;
        }

        // キャラクターとぶつかったら
         if (target.gameObject.TryGetComponent<Character>(out var character)) {
            OnCharacterHit(character);
            return;
         }
    }

    public void OnCharacterHit(Character target)
    {
        // ダメージ処理
        target.Damage(DEFAULT_DAMAGE);

        // 魔法は消す
        Destroy(gameObject);
    }
}
