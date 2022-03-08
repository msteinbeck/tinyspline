package org.tinyspline;

import org.junit.jupiter.api.Test;

import java.util.List;
import java.util.function.Predicate;

import static org.assertj.core.api.Assertions.assertThat;
import static org.tinyspline.tinysplinejavaConstants.TS_POINT_EPSILON;

public class InterpolationTest {

    private String str = "";

    private Predicate<? super Vec2> dist(double x, double y) {
        Vec2 vec2 = new Vec2(x, y);
        str = "distance => " + vec2;
        return vec -> vec.distance(vec2) <= TS_POINT_EPSILON;
    }

    @Test
    public void testCubicNatural() {
        // Given
        List<Double> points = List.of(
                1.0, -1.0, // P1
                -1.0, 2.0, // P2
                1.0, 4.0,  // P3
                4.0, 3.0,  // P4
                7.0, 5.0   // P5
        );

        // When
        BSpline spline = BSpline.interpolateCubicNatural(points, 2);

        // Then
        assertThat(spline.getNumControlPoints()).isEqualTo(16);

        // First bezier segment
        assertThat(spline.controlPointVec2At(0)).matches(dist(1, -1), str);
        assertThat(spline.controlPointVec2At(1)).matches(dist(0, 0), str);
        assertThat(spline.controlPointVec2At(2)).matches(dist(-1, 1), str);
        assertThat(spline.controlPointVec2At(3)).matches(dist(-1, 2), str);

        // Second bezier segment
        assertThat(spline.controlPointVec2At(4)).matches(dist(-1, 2), str);
        assertThat(spline.controlPointVec2At(5)).matches(dist(-1, 3), str);
        assertThat(spline.controlPointVec2At(6)).matches(dist(0, 4), str);
        assertThat(spline.controlPointVec2At(7)).matches(dist(1, 4), str);

        // Third bezier segment
        assertThat(spline.controlPointVec2At(8)).matches(dist(1, 4), str);
        assertThat(spline.controlPointVec2At(9)).matches(dist(2, 4), str);
        assertThat(spline.controlPointVec2At(10)).matches(dist(3, 3), str);
        assertThat(spline.controlPointVec2At(11)).matches(dist(4, 3), str);

        // Forth bezier segment
        assertThat(spline.controlPointVec2At(12)).matches(dist(4, 3), str);
        assertThat(spline.controlPointVec2At(13)).matches(dist(5, 3), str);
        assertThat(spline.controlPointVec2At(14)).matches(dist(6, 4), str);
        assertThat(spline.controlPointVec2At(15)).matches(dist(7, 5), str);
    }
}
