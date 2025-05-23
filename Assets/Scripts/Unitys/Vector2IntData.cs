using UnityEngine;

/// <summary>
/// Vector2Intが構造体のため使いづらく、クラス版
/// </summary>
public class Vector2IntData
{
    public int X { get; set; }
    public int Y { get; set; }

    public Vector2IntData()
    {

    }
    public Vector2IntData(int x, int y)
    {
        X = x;
        Y = y;
    }
    
    public Vector2IntData(Vector2Int v)
    {
        X = v.x;
        Y = v.y;
    }
}
