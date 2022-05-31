require "test/unit"
require "tinyspline"



def assert_knots_in_delta(exp, act)
  if exp.length() != act.length()
    flunk "length does not match"
  end
  for i in 0..exp.length()
    assert_in_delta(exp[i], act[i], TinySpline::TS_KNOT_EPSILON)
  end
end



class TestEnums < Test::Unit::TestCase

  def test_opened
    # Given
    spline = TinySpline::BSpline.new(
      7, 2, 3, TinySpline::BSpline::Opened)

    # When

    # Then
    expected = [ 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0 ]
    assert_knots_in_delta(expected, spline.knots)
  end

end



class TestFrames < Test::Unit::TestCase

  def assert_orthogonal(v1, v2)
    angle = v1.angle(v2)
    assert_in_delta(90, angle, TinySpline::TS_KNOT_EPSILON)
  end

  def test_vectors
    # Given
    spline = TinySpline::BSpline.new(7, 2)
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
    knots.each_with_index {|knot, idx|
      frame = frames.at(idx)

      pos = spline.eval(knot)
              .result_vec3()
              .distance(frame.position)
      assert_in_delta(0, pos, TinySpline::TS_POINT_EPSILON)

      tan = spline.derive().eval(knot)
              .result_vec3()
              .normalize()
              .distance(frame.tangent)
      assert_in_delta(0, tan, TinySpline::TS_POINT_EPSILON)

      assert_orthogonal(frame.normal, frame.tangent)
      assert_orthogonal(frame.normal, frame.binormal)
      assert_orthogonal(frame.binormal, frame.tangent)
    }
  end

end
