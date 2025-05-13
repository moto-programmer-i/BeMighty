using UnityEngine;
using UnityEngine.AI;

/// <summary>
/// Navmeshで追いかけるクラス
/// NavMeshAgent必須。
/// </summary>
public class MoveTo : MonoBehaviour
{
    public const float DEFAULT_RECALCULATE_SECONDS = 1;

    [SerializeField]
    private Transform goal;

    /// <summary>
    /// 追跡を再計算する期間
    /// </summary>
    [SerializeField]
    private float recalculateSeconds = DEFAULT_RECALCULATE_SECONDS;

    private NavMeshAgent agent;

    void Start () {
        agent = GetComponent<NavMeshAgent>();

        // 対象の位置を計算し続ける
        this.Loop(recalculateSeconds, () => {
            agent.destination = goal.position;
        });
    }
}
