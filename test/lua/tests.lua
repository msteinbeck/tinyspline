local lu = require('luaunit')
local ts = require("tinyspline")



TestEnums = {}

function TestEnums:testOpened()
   -- Given
   spline = ts.BSpline(7, 2, 3, ts.BSpline.Opened)

   -- When

   -- Then
   expected = { 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0 }
   lu.assertAlmostEquals(spline.knots, expected, ts.TS_KNOT_EPSILON)
end

-- end TestEnums



TestFrames = {}

local function assertOrthogonal(v1, v2)
   angle = v1:angle(v2)
   lu.assertAlmostEquals(angle, 90, ts.TS_POINT_EPSILON)
end

function TestFrames:testVectors()
   -- Given
   spline = ts.BSpline(7, 2)
   spline.control_points = {
      120, 100, -- P1
      270, 40,  -- P2
      370, 490, -- P3
      590, 40,  -- P4
      570, 490, -- P5
      420, 480, -- P6
      220, 500  -- P7
   }
   knots = spline:uniform_knot_seq(100)

   -- When
   frames = spline:compute_rmf(knots)

   -- Then
   for idx, knot in ipairs(knots) do
      frame = frames:at(idx - 1)

      pos = spline(knot):result_vec3():distance(frame.position)
      lu.assertAlmostEquals(pos, 0, ts.TS_POINT_EPSILON)

      tan = spline:derive()(knot):result_vec3():norm()
         :distance(frame.tangent)
      lu.assertAlmostEquals(tan, 0, ts.TS_POINT_EPSILON)

      assertOrthogonal(frame.normal, frame.tangent)
      assertOrthogonal(frame.normal, frame.binormal)
      assertOrthogonal(frame.binormal, frame.tangent)
   end
end

-- end TestFrames



TestInterpolation = {}

local function assertEquals(vec, x, y)
   dist = vec:distance(ts.Vec2(x, y))
   lu.assertAlmostEquals(dist, 0, ts.TS_POINT_EPSILON)
end

function TestInterpolation:testCubicNatural()
   -- Given
   points = {
      1.0, -1.0, -- P1
      -1.0, 2.0, -- P2
      1.0, 4.0,  -- P3
      4.0, 3.0,  -- P4
      7.0, 5.0   -- P5
   }

   -- When
   spline = ts.BSpline_interpolate_cubic_natural(points, 2)

   -- Then
   lu.assertEquals(spline.num_control_points, 16)

   -- First bezier segment
   assertEquals(spline:control_point_vec_2at(0), 1.0, -1.0)
   assertEquals(spline:control_point_vec_2at(1), 0.0, 0.0)
   assertEquals(spline:control_point_vec_2at(2), -1.0, 1.0)
   assertEquals(spline:control_point_vec_2at(3), -1.0, 2.0)

   -- Second bezier segment
   assertEquals(spline:control_point_vec_2at(4), -1.0, 2.0)
   assertEquals(spline:control_point_vec_2at(5), -1.0, 3.0)
   assertEquals(spline:control_point_vec_2at(6), 0.0, 4.0)
   assertEquals(spline:control_point_vec_2at(7), 1.0, 4.0)

   -- Third bezier segment
   assertEquals(spline:control_point_vec_2at(8), 1.0, 4.0)
   assertEquals(spline:control_point_vec_2at(9), 2.0, 4.0)
   assertEquals(spline:control_point_vec_2at(10), 3.0, 3.0)
   assertEquals(spline:control_point_vec_2at(11), 4.0, 3.0)

   -- Forth bezier segment
   assertEquals(spline:control_point_vec_2at(12), 4.0, 3.0)
   assertEquals(spline:control_point_vec_2at(13), 5.0, 3.0)
   assertEquals(spline:control_point_vec_2at(14), 6.0, 4.0)
   assertEquals(spline:control_point_vec_2at(15), 7.0, 5.0)
end

-- end TestInterpolation



os.exit( lu.run() )
