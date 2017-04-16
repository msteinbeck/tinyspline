import org.tinyspline.BSpline;
import org.tinyspline.BSplineType;

import java.util.List;

public class QuickStart {
    public static void main(final String[] args) {
        System.loadLibrary("tinysplinejava");
        
        // Create a clamped spline of degree 3 in 2D consisting of 7 control points.
        final BSpline spline = new BSpline(3, 2, 7, BSplineType.CLAMPED);

        // Setup the control points.
        final List<Float> ctrlp = spline.getCtrlp();
        ctrlp.set(0, -1.75f); // x0
        ctrlp.set(1, -1.0f);  // y0
        ctrlp.set(2, -1.5f);  // x1
        ctrlp.set(3, -0.5f);  // y1
        ctrlp.set(4, -1.5f);  // x2
        ctrlp.set(5, 0.0f);   // y2
        ctrlp.set(6, -1.25f); // x3
        ctrlp.set(7, 0.5f);   // y3
        ctrlp.set(8, -0.75f); // x4
        ctrlp.set(9, 0.75f);  // y4
        ctrlp.set(10, 0.0f);  // x5
        ctrlp.set(11, 0.5f);  // y5
        ctrlp.set(12, 0.5f);  // x6
        ctrlp.set(13, 0.0f);  // y6
        spline.setCtrlp(ctrlp);

        // Evaluate `spline` at u = 0.4
        List<Float> result = spline.evaluate(0.4f).getResult();
        System.out.println(String.format("x = %f, y = %f", result.get(0), result.get(1)));

        // Derive `spline` and subdivide it into a sequence of Bezier curves.
        final BSpline beziers = spline.derive().toBeziers();

        // Evaluate `beziers` at u = 0.3
        result = beziers.evaluate(0.3f).getResult(); // Java does not support the operator '()'
        System.out.println(String.format("x = %f, y = %f", result.get(0), result.get(1)));
    }
}
