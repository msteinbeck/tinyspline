package org.tinyspline;

import java.util.List;

import org.junit.jupiter.api.Test;

import static org.assertj.core.api.Assertions.assertThat;
import static org.assertj.core.data.Offset.offset;
import static org.tinyspline.tinysplinejavaConstants.TS_POINT_EPSILON;

public class VecTest {

    @Test
    public void testValuesVec2() {
        // Given
        Vec2 vec = new Vec2(1, 2);

        // When
        List<Double> values = vec.getValues();

        // Then
        assertThat(values.stream().mapToDouble(Double::doubleValue).toArray())
                .containsExactly(new double[] { 1.0, 2.0 },
                        offset(TS_POINT_EPSILON));
    }

    @Test
    public void testValuesVec3() {
        // Given
        Vec3 vec = new Vec3(3, 2, 1);

        // When
        List<Double> values = vec.getValues();

        // Then
        assertThat(values.stream().mapToDouble(Double::doubleValue).toArray())
                .containsExactly(new double[] { 3.0, 2.0, 1.0 },
                        offset(TS_POINT_EPSILON));
    }

    @Test
    public void testValuesVec4() {
        // Given
        Vec4 vec = new Vec4(1, 3, 2, 4);

        // When
        List<Double> values = vec.getValues();

        // Then
        assertThat(values.stream().mapToDouble(Double::doubleValue).toArray())
                .containsExactly(new double[] { 1.0, 3.0, 2.0, 4.0 },
                        offset(TS_POINT_EPSILON));
    }
}
