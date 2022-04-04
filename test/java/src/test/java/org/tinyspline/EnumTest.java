package org.tinyspline;

import org.junit.jupiter.api.Test;

import static org.assertj.core.api.Assertions.assertThat;
import static org.assertj.core.data.Offset.offset;
import static org.tinyspline.tinysplinejavaConstants.TS_KNOT_EPSILON;

public class EnumTest {

    @Test
    public void testOpened() {
        // Given
        BSpline spline = new BSpline(7, 2, 3, BSpline.Type.Opened);

        // When

        // Then
        assertThat(spline.getKnots().stream().mapToDouble(Double::doubleValue).toArray())
                .containsExactly(new double[] { 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0 },
                        offset(TS_KNOT_EPSILON));
    }
}
