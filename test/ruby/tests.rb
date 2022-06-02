require "test/unit"
require "tinyspline"

puts "Running tests on: #{RUBY_VERSION}"



def assert_knots_in_delta(exp, act)
  if exp.length() != act.length()
    flunk "length does not match"
  end
  for i in 0..exp.length()
    assert_in_delta(exp[i], act[i], TinySpline::TS_KNOT_EPSILON)
  end
end

def assert_pts1d_in_delta(exp, act)
  if exp.length() != act.length()
    flunk "length does not match"
  end
  for i in 0..exp.length()
    assert_in_delta(exp[i], act[i], TinySpline::TS_POINT_EPSILON)
  end
end



class TestEnums < Test::Unit::TestCase

  def test_opened()
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

  def test_vectors()
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



class TestInterpolation < Test::Unit::TestCase

  def assert_vec_equal(vec, x, y)
    dist = vec.distance(TinySpline::Vec2.new(x, y))
    assert_in_delta(0, dist, TinySpline::TS_POINT_EPSILON)
  end

  def test_cubic_natural()
    # Given
    points = [
      1.0, -1.0, # P1
      -1.0, 2.0, # P2
      1.0, 4.0,  # P3
      4.0, 3.0,  # P4
      7.0, 5.0   # P5
    ]

    # When
    spline = TinySpline::BSpline::interpolate_cubic_natural(points, 2)

    # Then
    assert_equal(16, spline.num_control_points)

    # First bezier segment.
    assert_vec_equal(spline.control_point_vec2_at(0), 1.0, -1.0)
    assert_vec_equal(spline.control_point_vec2_at(1), 0.0, 0.0)
    assert_vec_equal(spline.control_point_vec2_at(2), -1.0, 1.0)
    assert_vec_equal(spline.control_point_vec2_at(3), -1.0, 2.0)

    # Second bezier segment.
    assert_vec_equal(spline.control_point_vec2_at(4), -1.0, 2.0)
    assert_vec_equal(spline.control_point_vec2_at(5), -1.0, 3.0)
    assert_vec_equal(spline.control_point_vec2_at(6), 0.0, 4.0)
    assert_vec_equal(spline.control_point_vec2_at(7), 1.0, 4.0)

    # Third bezier segment.
    assert_vec_equal(spline.control_point_vec2_at(8), 1.0, 4.0)
    assert_vec_equal(spline.control_point_vec2_at(9), 2.0, 4.0)
    assert_vec_equal(spline.control_point_vec2_at(10), 3.0, 3.0)
    assert_vec_equal(spline.control_point_vec2_at(11), 4.0, 3.0)

    # Forth bezier segment.
    assert_vec_equal(spline.control_point_vec2_at(12), 4.0, 3.0)
    assert_vec_equal(spline.control_point_vec2_at(13), 5.0, 3.0)
    assert_vec_equal(spline.control_point_vec2_at(14), 6.0, 4.0)
    assert_vec_equal(spline.control_point_vec2_at(15), 7.0, 5.0)
  end

end



class TestVec < Test::Unit::TestCase

  def test_values_vec2()
    # Given
    vec = TinySpline::Vec2.new(1, 2)

    # When
    values = vec.values

    # Then
    assert_pts1d_in_delta([1, 2], values)
  end

  def test_values_vec3()
    # Given
    vec = TinySpline::Vec3.new(3, 2, 1)

    # When
    values = vec.values

    # Then
    assert_pts1d_in_delta([3, 2, 1], values)
  end

  def test_values_vec4()
    # Given
    vec = TinySpline::Vec4.new(1, 3, 2, 4)

    # When
    values = vec.values

    # Then
    assert_pts1d_in_delta([1, 3, 2, 4], values)
  end

end
