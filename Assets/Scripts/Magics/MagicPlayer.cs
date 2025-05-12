using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using UnityEngine.InputSystem;

public class MagicPlayer : MonoBehaviour
{
    [SerializeField]
    private float cooltime = 1.0f;

    
    /// <summary>
    /// 魔法陣の入力
    /// </summary>
    [SerializeField]
    private InputAction cast;

    [SerializeField]
    private GameObject magicForm;

    /// <summary>
    /// Actionの有効を自分で管理するクラス
    /// </summary>
    private readonly List<InputAction> actions = new ();

    // 最後に発動した時間
    private float lastCastTime = float.MinValue;

    void Start()
    {
        // 魔法陣入力設定
        cast.performed += context => {
            // 固定クールタイム
            if(Time.time - lastCastTime < cooltime) {
                return;
            }
            lastCastTime = Time.time;

            Instantiate(magicForm, transform.position, transform.rotation);
        };
    }

    // InputActionはOnEnableとOnDisableを実装しなければならない
    // https://docs.unity3d.com/Packages/com.unity.inputsystem@1.14/manual/Actions.html#enabling-actions
    void OnEnable()
    {
        // アクションが登録されていなければ登録する
        if (!actions.Any()) {
            if (cast != null) {
                actions.Add(cast);
            }
            
        }
        
        actions.ForEach(action => action.Enable());
    }

    void OnDisable()
    {
        actions.ForEach(action => action.Disable());
    }

}
