using UnityEngine;

public class Character : MonoBehaviour
{
    public const int DEFAULT_HP = 3;

    [field:SerializeField]
    public string Name {private set; get;}

    [field:SerializeField]
    public int HP {set; get;} = DEFAULT_HP;
    public HPPanel HPPanel {set; get;}

    [SerializeField]
    private bool destroyByHP = true;

    public Character(string name)
    {
        Name = name;
    }

    /// <summary>
    /// 表示と同期
    /// </summary>
    public void Sync()
    {
        HPPanel.SetHP(HP);
    }

    /// <summary>
    /// ダメージ処理
    /// </summary>
    /// <param name="damage"></param>
    public void Damage(int damage)
    {
        HP -= damage;
        if(HP <= 0) {
            if(destroyByHP) {
                Destroy(gameObject);
            }
            HP = 0;
        }
        HPPanel?.SetHP(HP);
    }

    // PlayerControllerでしか動かない、しかも地面に当たる度に呼ばれるので使用するべきでない
    // public void OnControllerColliderHit(ControllerColliderHit target)
    // {        
    //     // ぶつかった相手が魔法でなければ何もしない
    //     if(!target.gameObject.CompareTag(Magic.TAG_NAME))
    //     {
    //         return;
    //     }

    //     // 2回以上接触しないよう、レイヤーを変える
    //     // 参考 http://kan-kikuchi.hatenablog.com/entry/ChangeLayer
    //     target.gameObject.layer = Layers.DESTROY;
    //     Destroy(target.gameObject);
        

    //     // 魔法とぶつかった場合はHPを減らして、魔法は削除
    //     --HP;
    //     if(HP < 0) {
    //         HP = 0;
    //     }
    //     HPPanel.SetHP(HP);
    // }
}
