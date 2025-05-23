using System;
using NUnit.Framework.Internal;
using UnityEngine;
using UnityEngine.Rendering;
using UnityEngine.UIElements;

public class UIManager
{
    public const int SCREEN_MIN = 0;

    private PanelSettings panelSettings;

    private static UIManager current;

    /// <summary>
    /// panelSettings.referenceResolutionが構造体のためクラスを用意
    /// </summary>
    private Vector2IntData referenceResolution;


    public UIManager(PanelSettings panelSettings)
    {
        current = this;
        this.panelSettings = panelSettings;
        referenceResolution = new (current.panelSettings.referenceResolution);
    }

    public static Vector2IntData GetResolution()
    {
        return current.referenceResolution;
    }
}
