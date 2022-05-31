using NUnit.Framework;
using System.Collections.Generic;
using TinySpline;

namespace csharp;

public class VecTest
{
    [Test]
    public void TestValuesVec2()
    {
        // Given
        Vec2 vec = new Vec2(1, 2);

        // When
        IList<double> values = vec.Values;

        // Then
        Assert.That(values, Is.EqualTo(new List<double> { 1, 2 })
              .Within(tinysplinecsharp.TS_POINT_EPSILON));
    }

    [Test]
    public void TestValuesVec3()
    {
        // Given
        Vec3 vec = new Vec3(3, 2, 1);

        // When
        IList<double> values = vec.Values;

        // Then
        Assert.That(values, Is.EqualTo(new List<double> { 3, 2, 1 })
              .Within(tinysplinecsharp.TS_POINT_EPSILON));
    }

    [Test]
    public void TestValuesVec4()
    {
        // Given
        Vec4 vec = new Vec4(1, 3, 2, 4);

        // When
        IList<double> values = vec.Values;

        // Then
        Assert.That(values, Is.EqualTo(new List<double> { 1, 3, 2, 4 })
              .Within(tinysplinecsharp.TS_POINT_EPSILON));
    }
}
