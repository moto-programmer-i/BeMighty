using UnityEngine;

/// <summary>
/// MagicTypeの拡張メソッド
/// </summary>
public static class MagicTypeExtensions
{
    /// <summary>
    /// 相反する属性ならばtrue
    /// 炎 ⇔ 水
    /// 雷 ⇔ 地
    /// </summary>
    /// <param name="self"></param>
    /// <param name="target">相手の属性</param>
    /// <returns></returns>
    public static bool IsOpposite(this MagicType self, MagicType? target)
    {
        return self switch
        {
            MagicType.FIRE => MagicType.WATER.Equals(target),
            MagicType.WATER => MagicType.FIRE.Equals(target),
            MagicType.THUNDER => MagicType.EARTH.Equals(target),
            MagicType.EARTH => MagicType.THUNDER.Equals(target),
            // 不明な場合はfalse
            _ => false,
        };
    }
}
