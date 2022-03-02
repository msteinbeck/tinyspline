local lu = require('luaunit')
local ts = require("tinyspline")



local function assertOrthogonal(v1, v2)
    angle = v1:angle(v2)
    lu.assertAlmostEquals(angle, 90, ts.TS_POINT_EPSILON)
end



TestFrames = {}

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



os.exit( lu.run() )
