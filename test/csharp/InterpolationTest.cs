using NUnit.Framework;
using System.Collections.Generic;
using TinySpline;

namespace csharp;

public class InterpolationTest
{
    class Vec2EqualityComparer : IEqualityComparer<Vec2>
    {
        public bool Equals(Vec2? v1, Vec2? v2)
        {
            if (v1 == null && v2 == null) return true;
            if (v1 == null || v2 == null) return false;
            return v1.Distance(v2) <= tinysplinecsharp.TS_POINT_EPSILON;
        }

        public int GetHashCode(Vec2 vec)
        {
            double hCode = vec.X * vec.Y;
            return hCode.GetHashCode();
        }
    }

    [Test]
    public void TestCubicNatural()
    {
        // Given
        List<double> points = new List<double> {
            1.0, -1.0, // P1
            -1.0, 2.0, // P2
            1.0, 4.0,  // P3
            4.0, 3.0,  // P4
            7.0, 5.0   // P5
        };
        Vec2EqualityComparer comparer = new Vec2EqualityComparer();

        // When
        BSpline spline = BSpline.InterpolateCubicNatural(points, 2);

        // Then
        Assert.That(spline.NumControlPoints, Is.EqualTo(16));

        // First bezier segment
        Assert.That(spline.ControlPointVec2At(0),
                    Is.EqualTo(new Vec2(1, -1))
                      .Using(comparer));
        Assert.That(spline.ControlPointVec2At(1),
                    Is.EqualTo(new Vec2(0, 0))
                      .Using(comparer));
        Assert.That(spline.ControlPointVec2At(2),
                    Is.EqualTo(new Vec2(-1, 1))
                      .Using(comparer));
        Assert.That(spline.ControlPointVec2At(3),
                    Is.EqualTo(new Vec2(-1, 2))
                      .Using(comparer));

        // Second bezier segment
        Assert.That(spline.ControlPointVec2At(4),
                    Is.EqualTo(new Vec2(-1, 2))
                      .Using(comparer));
        Assert.That(spline.ControlPointVec2At(5),
                    Is.EqualTo(new Vec2(-1, 3))
                      .Using(comparer));
        Assert.That(spline.ControlPointVec2At(6),
                    Is.EqualTo(new Vec2(0, 4))
                      .Using(comparer));
        Assert.That(spline.ControlPointVec2At(7),
                    Is.EqualTo(new Vec2(1, 4))
                      .Using(comparer));

        // Third bezier segment
        Assert.That(spline.ControlPointVec2At(8),
                    Is.EqualTo(new Vec2(1, 4))
                      .Using(comparer));
        Assert.That(spline.ControlPointVec2At(9),
                    Is.EqualTo(new Vec2(2, 4))
                      .Using(comparer));
        Assert.That(spline.ControlPointVec2At(10),
                    Is.EqualTo(new Vec2(3, 3))
                      .Using(comparer));
        Assert.That(spline.ControlPointVec2At(11),
                    Is.EqualTo(new Vec2(4, 3))
                      .Using(comparer));

        // Forth bezier segment
        Assert.That(spline.ControlPointVec2At(12),
                    Is.EqualTo(new Vec2(4, 3))
                      .Using(comparer));
        Assert.That(spline.ControlPointVec2At(13),
                    Is.EqualTo(new Vec2(5, 3))
                      .Using(comparer));
        Assert.That(spline.ControlPointVec2At(14),
                    Is.EqualTo(new Vec2(6, 4))
                      .Using(comparer));
        Assert.That(spline.ControlPointVec2At(15),
                    Is.EqualTo(new Vec2(7, 5))
                      .Using(comparer));
    }
}
