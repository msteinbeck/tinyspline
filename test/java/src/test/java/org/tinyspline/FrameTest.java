package org.tinyspline;

import org.junit.jupiter.api.Test;

import java.util.List;

import static org.assertj.core.api.Assertions.assertThat;
import static org.assertj.core.data.Offset.offset;
import static org.tinyspline.tinysplinejavaConstants.TS_POINT_EPSILON;

public class FrameTest {

    private void assertOrthogonal(Vec3 v1, Vec3 v2) {
        double angle = v1.angle(v2);
        assertThat(angle).isEqualTo(90, offset(TS_POINT_EPSILON));
    }

    @Test
    public void testVectors() {
        // Given
        BSpline spline = new BSpline(7, 2);
        spline.setControlPoints(List.of(
                120.0, 100.0, // P1
                270.0, 40.0,  // P2
                370.0, 490.0, // P3
                590.0, 40.0,  // P4
                570.0, 490.0, // P5
                420.0, 480.0, // P6
                220.0, 500.0  // P7
        ));
        List<Double> knots = spline.uniformKnotSeq(100);

        // When
        FrameSeq frames = spline.computeRMF(knots);

        // Then
        for (int i = 0; i < knots.size(); i++) {
            Frame frame = frames.at(i);

            double pos = spline.eval(knots.get(i))
                               .resultVec3()
                               .distance(frame.getPosition());
            assertThat(pos).isEqualTo(0, offset(TS_POINT_EPSILON));

            double tan = spline.derive()
                               .eval(knots.get(i))
                               .resultVec3()
                               .normalize()
                               .distance(frame.getTangent());
            assertThat(tan).isEqualTo(0, offset(TS_POINT_EPSILON));

            assertOrthogonal(frame.getNormal(), frame.getTangent());
            assertOrthogonal(frame.getNormal(), frame.getBinormal());
            assertOrthogonal(frame.getBinormal(), frame.getTangent());
        }
    }
}
