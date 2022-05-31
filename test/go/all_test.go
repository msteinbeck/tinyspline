package main

import (
	"github.com/stretchr/testify/assert"
	ts "github.com/tinyspline/go"
	"testing"
)

func TestEnums_TestOpened(t *testing.T) {
	// Given
	spline := ts.NewBSpline(7, 2, 3, ts.BSplineOpened)

	// When

	// Then
	expected := []float64{0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0}
	assert.InDeltaSlice(t, expected, spline.GetKnots(), ts.TS_KNOT_EPSILON)
}

func TestFrames_TestVectors(t *testing.T) {
	// Given
	ao := func(v1 ts.Vec3, v2 ts.Vec3) {
		angle := v1.Angle(v2)
		assert.InDelta(t, 90, angle, ts.TS_POINT_EPSILON)
	}

	spline := ts.NewBSpline(7, 2)
	spline.SetControlPoints([]float64{
		120, 100, // P1
		270, 40, // P2
		370, 490, // P3
		590, 40, // P4
		570, 490, // P5
		420, 480, // P6
		220, 500, // P7
	})
	knots := spline.UniformKnotSeq(100)

	// When
	frames := spline.ComputeRMF(knots)

	// Then
	for idx, knot := range knots {
		frame := frames.At(idx)

		pos := spline.Eval(knot).
			ResultVec3().
			Distance(frame.GetPosition())
		assert.InDelta(t, 0, pos, ts.TS_POINT_EPSILON)

		tan := spline.Derive().
			Eval(knot).
			ResultVec3().
			Normalize().
			Distance(frame.GetTangent())
		assert.InDelta(t, 0, tan, ts.TS_POINT_EPSILON)

		ao(frame.GetNormal(), frame.GetTangent())
		ao(frame.GetNormal(), frame.GetBinormal())
		ao(frame.GetBinormal(), frame.GetTangent())
	}
}

func TestInterpolation_TestCubicNatural(t *testing.T) {
	// Given
	ae := func(v1 ts.Vec2, x, y float64) {
		dist := v1.Distance(ts.NewVec2(x, y))
		assert.InDelta(t, 0, dist, ts.TS_POINT_EPSILON)
	}

	points := []float64{
		1.0, -1.0, // P1
		-1.0, 2.0, // P2
		1.0, 4.0, // P3
		4.0, 3.0, // P4
		7.0, 5.0, // P5
	}

	// When
	spline := ts.BSplineInterpolateCubicNatural(points, 2)

	// Then
	assert.Equal(t, 16, spline.GetNumControlPoints())

	// First bezier segment
	ae(spline.ControlPointVec2At(0), 1.0, -1.0)
	ae(spline.ControlPointVec2At(1), 0.0, 0.0)
	ae(spline.ControlPointVec2At(2), -1.0, 1.0)
	ae(spline.ControlPointVec2At(3), -1.0, 2.0)

	// Second bezier segment
	ae(spline.ControlPointVec2At(4), -1.0, 2.0)
	ae(spline.ControlPointVec2At(5), -1.0, 3.0)
	ae(spline.ControlPointVec2At(6), 0.0, 4.0)
	ae(spline.ControlPointVec2At(7), 1.0, 4.0)

	// Third bezier segment
	ae(spline.ControlPointVec2At(8), 1.0, 4.0)
	ae(spline.ControlPointVec2At(9), 2.0, 4.0)
	ae(spline.ControlPointVec2At(10), 3.0, 3.0)
	ae(spline.ControlPointVec2At(11), 4.0, 3.0)

	// Forth bezier segment
	ae(spline.ControlPointVec2At(12), 4.0, 3.0)
	ae(spline.ControlPointVec2At(13), 5.0, 3.0)
	ae(spline.ControlPointVec2At(14), 6.0, 4.0)
	ae(spline.ControlPointVec2At(15), 7.0, 5.0)
}

func TestVec_TestValuesVec2(t *testing.T) {
	// Given
	vec := ts.NewVec2(1.0, 2.0)

	// When
	values := vec.GetValues()

	// Then
	expected := []float64{1.0, 2.0}
	assert.InDeltaSlice(t, expected, values, ts.TS_POINT_EPSILON)
}

func TestVec_TestValuesVec3(t *testing.T) {
	// Given
	vec := ts.NewVec3(3.0, 2.0, 1.0)

	// When
	values := vec.GetValues()

	// Then
	expected := []float64{3.0, 2.0, 1.0}
	assert.InDeltaSlice(t, expected, values, ts.TS_POINT_EPSILON)
}

func TestVec_TestValuesVec4(t *testing.T) {
	// Given
	vec := ts.NewVec4(1.0, 3.0, 2.0, 4.0)

	// When
	values := vec.GetValues()

	// Then
	expected := []float64{1.0, 3.0, 2.0, 4.0}
	assert.InDeltaSlice(t, expected, values, ts.TS_POINT_EPSILON)
}
