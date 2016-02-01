"""Unit-tests for swig generated tinyspline module."""
import unittest

import tinyspline


class TestTsBSpline(unittest.TestCase):

    def MakeValid2DBSpline(self):
        return tinyspline.TsBSpline(2, 2, 3, tinyspline.TS_CLAMPED)

    def MakeValid2DBSplineAlongAStraightLine(self):
        spline = self.MakeValid2DBSpline()
        spline.setCtrlp([1, 1,
                         2, 1,
                         3, 1])
        return spline

    def MakeValid2DBSplineAlong5SkewedPoints(self):
        spline = tinyspline.TsBSpline(2, 2, 5, tinyspline.TS_CLAMPED)
        spline.setCtrlp([0, 0,
                         0.5, 5,
                         0.75, -2.5,
                         1.0, 0,
                         2.0, 2])
        return spline

    def testMakeWithTooFewControlPointsRaisesRuntimeError(self):
        # TODO(JacobSHuffman): Make this into a more specific error
        # and make it more verbose
        with self.assertRaises(RuntimeError):
            tinyspline.TsBSpline(4, 2, 3, tinyspline.TS_CLAMPED)

    def testMakeValid2DBSpline(self):
        spline = self.MakeValid2DBSpline()
        self.assertEqual(spline.deg, 2)
        self.assertEqual(spline.order, 3)
        self.assertEqual(spline.dim, 2)
        self.assertEqual(spline.nCtrlp, 3)
        self.assertEqual(spline.nKnots, 6)

    def testSetCtrlpWithNoControlPointsRaisesRuntimeError(self):
        spline = self.MakeValid2DBSpline()
        # TODO(JacobSHuffman): Make this into a more specific error
        # and make it more verbose
        with self.assertRaises(RuntimeError):
            spline.setCtrlp([])

    def testSetCtrlpWithTooFewControlPointsRaisesRuntimeError(self):
        spline = self.MakeValid2DBSpline()
        with self.assertRaises(RuntimeError):
            spline.setCtrlp([0, 1, 2])

    def testSetKnotsWithNoKnotsRaisesRuntimeError(self):
        spline = self.MakeValid2DBSpline()
        # TODO(JacobSHuffman): Make this into a more specific error
        # and make it more verbose
        with self.assertRaises(RuntimeError):
            spline.setKnots([])

    def testSetKnotsWithTooFewKnotsRaisesRuntimeError(self):
        spline = self.MakeValid2DBSpline()
        with self.assertRaises(RuntimeError):
            spline.setKnots([0, 1, 2])

    def testSetCtrlpAlongAStraightLine(self):
        spline = self.MakeValid2DBSplineAlongAStraightLine()
        self.assertListEqual(
            spline.ctrlp, [1, 1, 2, 1, 3, 1])

    def testGetKnots(self):
        spline = self.MakeValid2DBSpline()
        self.assertListEqual(
            spline.knots, [0, 0, 0, 1, 1, 1])

    def testEvaulateAlongAStraightLine(self):
        spline = self.MakeValid2DBSplineAlongAStraightLine()
        
        self.assertEqual(spline.evaluate(0).result, [1, 1])
        self.assertEqual(spline.evaluate(1).result, [3, 1])
        self.assertEqual(spline.evaluate(0.25).result, [1.5, 1])

    def testSetDegChangesSplineShape(self):
        spline = self.MakeValid2DBSplineAlong5SkewedPoints()
        point_on_order_2_spline = spline.evaluate(0.7).result
        spline.deg = 3
        point_on_order_3_spline = spline.evaluate(0.7).result
        self.assertNotEqual(
            point_on_order_2_spline, point_on_order_3_spline)


if __name__ == '__main__':
    unittest.main()
