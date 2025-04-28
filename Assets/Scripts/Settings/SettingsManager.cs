using UnityEngine;

public class SettingsManager : MonoBehaviour
{
    [SerializeField]
    private int maxFrameRate;

    void Awake()
    {
        Application.targetFrameRate = maxFrameRate; 
    }
}
