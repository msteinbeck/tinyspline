import org.tinyspline.BSpline;
import org.tinyspline.BSplineType;

import java.util.List;

public class QuickStart {
	public static void main(final String[] args) {
		// Create a cubic spline with 7 control points in 2D using
		// a clamped knot vector. This call is equivalent to:
		// BSpline spline = new BSpline(7, 2, 3, BSplineType.CLAMPED);
		final BSpline spline = new BSpline(7);

		// Setup control points.
		final List<Double> ctrlp = spline.getControlPoints();
		ctrlp.set(0, -1.75); // x0
		ctrlp.set(1, -1.0);  // y0
		ctrlp.set(2, -1.5);  // x1
		ctrlp.set(3, -0.5);  // y1
		ctrlp.set(4, -1.5);  // x2
		ctrlp.set(5, 0.0);   // y2
		ctrlp.set(6, -1.25); // x3
		ctrlp.set(7, 0.5);   // y3
		ctrlp.set(8, -0.75); // x4
		ctrlp.set(9, 0.75);  // y4
		ctrlp.set(10, 0.0);  // x5
		ctrlp.set(11, 0.5);  // y5
		ctrlp.set(12, 0.5);  // x6
		ctrlp.set(13, 0.0);  // y6
		spline.setControlPoints(ctrlp);

		// Evaluate `spline` at u = 0.4.
		List<Double> result = spline.eval(0.4).getResult();
		System.out.printf("x = %f, y = %f%n",
				result.get(0), result.get(1));

		// Derive `spline` and subdivide it into a sequence of Bezier curves.
		final BSpline beziers = spline.derive().toBeziers();

		// Evaluate `beziers` at u = 0.3.
		result = beziers.eval(0.3).getResult();
		System.out.printf("x = %f, y = %f%n",
				result.get(0), result.get(1));
	}
}
