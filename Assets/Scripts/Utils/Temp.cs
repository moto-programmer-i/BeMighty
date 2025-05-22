using UnityEngine;
using UnityEngine.UIElements;

/// <summary>
/// テスト用。最終的には削除すること
/// </summary>
public class Temp : MonoBehaviour
{
    [SerializeField]
    private Transform target;
    [SerializeField]
    private Camera mainCamera;
    [SerializeField]
    private UIDocument ui;

    void Start()
    {
        // Debug.Log($"{Screen.width}:{Screen.height}");
        // yの座標が逆なはずだが、CameraTransformWorldToPanelを使うとピタリと合う
        var position = RuntimePanelUtils.CameraTransformWorldToPanel(ui.rootVisualElement.panel, target.position, mainCamera);
        Debug.Log($"UIでの位置{position}");
        // UIでの位置(323.94, 53.74)

        // UI toolkitでWorldToScreenPointは全くうまくいかない
        var screenPosition = mainCamera.WorldToScreenPoint(target.position);
        Debug.Log($"2Dでの位置{screenPosition}");
        // 2Dでの位置(518.30, 994.02, 10.63)
        var origin = ui.rootVisualElement.Q<VisualElement>(Talk.ORIGIN_NAME);
        origin.SetLeftTop(position);
    }

}
