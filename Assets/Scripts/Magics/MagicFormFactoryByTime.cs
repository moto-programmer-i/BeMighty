using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;
using UnityEngine;
using UnityEngine.InputSystem;

public class MagicFormFactoryByTime : MonoBehaviour
{
    [SerializeField]
    private float cooltime = 1.0f;

    [SerializeField]
    private GameObject magicForm;

    void Start()
    {
        this.Loop(cooltime, () => {
            Instantiate(magicForm, transform.position, transform.rotation);
        });
    }
}
