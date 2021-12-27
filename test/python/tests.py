import unittest
from tinyspline import *

class TestInterpolation(unittest.TestCase):

    def test_cubic_natural(self):
        spline = BSpline.interpolate_cubic_natural(
            [1.0, -1.0,
             -1.0, 2.0,
             1.0, 4.0,
             4.0, 3.0,
             7.0, 5.0], 2)
        ctrlp = spline.control_points

        def assert_equals(pts, idx, x, y):
            a = Vec3(pts[idx * 2], pts[idx * 2 + 1], 0)
            b = Vec3(x, y, 0)
            dist = (a - b).magnitude()
            self.assertAlmostEqual(0.0, dist, delta=0.0001)

        # First bezier segment.
        assert_equals(ctrlp, 0, 1.0, -1.0)
        assert_equals(ctrlp, 1, 0.0, 0.0)
        assert_equals(ctrlp, 2, -1.0, 1.0)
        assert_equals(ctrlp, 3, -1.0, 2.0)

        # Second bezier segment.
        assert_equals(ctrlp, 4, -1.0, 2.0)
        assert_equals(ctrlp, 5, -1.0, 3.0)
        assert_equals(ctrlp, 6, 0.0, 4.0)
        assert_equals(ctrlp, 7, 1.0, 4.0)

        # Third bezier segment.
        assert_equals(ctrlp, 8, 1.0, 4.0)
        assert_equals(ctrlp, 9, 2.0, 4.0)
        assert_equals(ctrlp, 10, 3.0, 3.0)
        assert_equals(ctrlp, 11, 4.0, 3.0)

        # Forth bezier segment.
        assert_equals(ctrlp, 12, 4.0, 3.0)
        assert_equals(ctrlp, 13, 5.0, 3.0)
        assert_equals(ctrlp, 14, 6.0, 4.0)
        assert_equals(ctrlp, 15, 7.0, 5.0)

if __name__ == '__main__':
    unittest.main()
