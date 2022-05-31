import unittest
from tinyspline import *



def assertKnotsAlmostEqual(exp, act, tc):
    if len(exp) != len(act):
        tc.fail("length does not match")
    for i in range(len(exp)):
        tc.assertAlmostEqual(exp[i], act[i], delta=TS_KNOT_EPSILON)

def assertPts1dAlmostEqual(exp, act, tc):
    if len(exp) != len(act):
        tc.fail("length does not match")
    for i in range(len(exp)):
        tc.assertAlmostEqual(exp[i], act[i], delta=TS_POINT_EPSILON)



class TestEnums(unittest.TestCase):

    def testOpened(self):
        # Given
        spline = BSpline(7, 2, 3, BSpline.Opened)

        # When

        # Then
        expected = [ 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0 ]
        assertKnotsAlmostEqual(expected, spline.knots, self)



class TestFrames(unittest.TestCase):

    def assertOrthogonal(self, v1, v2):
        angle = v1.angle(v2)
        self.assertAlmostEqual(90, angle, delta=TS_POINT_EPSILON)

    def testVectors(self):
        # Given
        spline = BSpline(7, 2)
        spline.control_points = [
            120, 100, # P1
            270, 40,  # P2
            370, 490, # P3
            590, 40,  # P4
            570, 490, # P5
            420, 480, # P6
            220, 500  # P7
            ]
        knots = spline.uniform_knot_seq(100)

        # When
        frames = spline.compute_rmf(knots)

        # Then
        for i in range(len(knots)):
            frame = frames.at(i)
            knot = knots[i]

            pos = spline(knot).result_vec3().distance(frame.position)
            self.assertAlmostEqual(0, pos, delta=TS_POINT_EPSILON)

            tan = spline.derive()(knot).result_vec3().normalize().distance(frame.tangent)
            self.assertAlmostEqual(0, tan, delta=TS_POINT_EPSILON)

            self.assertOrthogonal(frame.normal, frame.tangent)
            self.assertOrthogonal(frame.normal, frame.binormal)
            self.assertOrthogonal(frame.binormal, frame.tangent)



class TestInterpolation(unittest.TestCase):

    def assertEquals(self, vec, x, y):
        dist = vec.distance(Vec2(x, y))
        self.assertAlmostEqual(0, dist, delta=TS_POINT_EPSILON)

    def testCubicNatural(self):
        # Given
        points = [
            1.0, -1.0, # P1
            -1.0, 2.0, # P2
            1.0, 4.0,  # P3
            4.0, 3.0,  # P4
            7.0, 5.0   # P5
            ]

        # When
        spline = BSpline.interpolate_cubic_natural(points, 2)

        # Then
        self.assertEqual(16, spline.num_control_points)

        # First bezier segment.
        self.assertEquals(spline.control_point_vec2_at(0), 1.0, -1.0)
        self.assertEquals(spline.control_point_vec2_at(1), 0.0, 0.0)
        self.assertEquals(spline.control_point_vec2_at(2), -1.0, 1.0)
        self.assertEquals(spline.control_point_vec2_at(3), -1.0, 2.0)

        # Second bezier segment.
        self.assertEquals(spline.control_point_vec2_at(4), -1.0, 2.0)
        self.assertEquals(spline.control_point_vec2_at(5), -1.0, 3.0)
        self.assertEquals(spline.control_point_vec2_at(6), 0.0, 4.0)
        self.assertEquals(spline.control_point_vec2_at(7), 1.0, 4.0)

        # Third bezier segment.
        self.assertEquals(spline.control_point_vec2_at(8), 1.0, 4.0)
        self.assertEquals(spline.control_point_vec2_at(9), 2.0, 4.0)
        self.assertEquals(spline.control_point_vec2_at(10), 3.0, 3.0)
        self.assertEquals(spline.control_point_vec2_at(11), 4.0, 3.0)

        # Forth bezier segment.
        self.assertEquals(spline.control_point_vec2_at(12), 4.0, 3.0)
        self.assertEquals(spline.control_point_vec2_at(13), 5.0, 3.0)
        self.assertEquals(spline.control_point_vec2_at(14), 6.0, 4.0)
        self.assertEquals(spline.control_point_vec2_at(15), 7.0, 5.0)



class TestVec(unittest.TestCase):

    def testValuesVec2(self):
        # Given
        vec = Vec2(1, 2)

        # When
        values = vec.values

        # Then
        assertPts1dAlmostEqual([1, 2], values, self)

    def testValuesVec3(self):
        # Given
        vec = Vec3(3, 2, 1)

        # When
        values = vec.values

        # Then
        assertPts1dAlmostEqual([3, 2, 1], values, self)

    def testValuesVec4(self):
        # Given
        vec = Vec4(1, 3, 2, 4)

        # When
        values = vec.values

        # Then
        assertPts1dAlmostEqual([1, 3, 2, 4], values, self)



if __name__ == '__main__':
    unittest.main()
