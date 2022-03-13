using NUnit.Framework;
using System.Collections.Generic;
using TinySpline;

namespace csharp;

public class EnumTest
{
    [Test]
    public void TestOpened()
    {
        // Given
        BSpline spline = new BSpline(7, 2, 3, BSpline.Type.Opened);

        // When

        // Then
        Assert.That(spline.Knots, Is.EqualTo(new List<double> {
                   0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0 })
              .Within(tinysplinecsharp.TS_KNOT_EPSILON));
    }
}
