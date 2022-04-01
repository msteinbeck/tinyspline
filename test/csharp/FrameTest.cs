using NUnit.Framework;
using System.Collections.Generic;
using TinySpline;

namespace csharp;

public class FrameTest
{
    private void AssertOrthogonal(Vec3 v1, Vec3 v2)
    {
        double angle = v1.Angle(v2);
        Assert.That(angle, Is.EqualTo(90)
              .Within(tinysplinecsharp.TS_POINT_EPSILON));
    }

    [Test]
    public void TestVectors()
    {
        // Given
        BSpline spline = new BSpline(7, 2)
        {
            ControlPoints = new List<double> {
                120.0, 100.0, // P1
                270.0, 40.0,  // P2
                370.0, 490.0, // P3
                590.0, 40.0,  // P4
                570.0, 490.0, // P5
                420.0, 480.0, // P6
                220.0, 500.0  // P7
            }
        };
        IList<double> knots = spline.UniformKnotSeq(100);

        // When
        FrameSeq frames = spline.ComputeRMF(knots);

        // Then
        for (int i = 0; i < knots.Count; i++)
        {
            Frame frame = frames.At((uint) i);

            double pos = spline.Eval(knots[i])
                               .ResultVec3()
                               .Distance(frame.Position);
            Assert.That(pos, Is.EqualTo(0)
                  .Within(tinysplinecsharp.TS_POINT_EPSILON));

            double tan = spline.Derive()
                               .Eval(knots[i])
                               .ResultVec3()
                               .Normalize()
                               .Distance(frame.Tangent);
            Assert.That(tan, Is.EqualTo(0)
                  .Within(tinysplinecsharp.TS_POINT_EPSILON));

            AssertOrthogonal(frame.Normal, frame.Tangent);
            AssertOrthogonal(frame.Normal, frame.Binormal);
            AssertOrthogonal(frame.Binormal, frame.Tangent);
        }
    }
}
