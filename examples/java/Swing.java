import org.tinyspline.BSpline;
import org.tinyspline.Utils;

import java.awt.Color;
import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.geom.Path2D;
import java.awt.geom.Point2D;
import java.awt.geom.Rectangle2D;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;

public class Swing extends JPanel implements
		MouseMotionListener, MouseListener {

	private final List<Point2D> points = new ArrayList<>();
	private Point2D dragged = null;

	private Swing() {
		super(true);
		setPreferredSize(new Dimension(800, 600));
		addMouseMotionListener(this);
		addMouseListener(this);

		final JFrame frame = new JFrame();
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.getContentPane().add(this);
		frame.pack();
		frame.setVisible(true);

		points.add(new Point2D.Double(300, 500));
		points.add(new Point2D.Double(100, 300));
		points.add(new Point2D.Double(300, 100));
		points.add(new Point2D.Double(500, 200));
		points.add(new Point2D.Double(700, 50));
	}

	@Override
	public void paint(final Graphics g) {
		super.paintComponents(g);
		final Graphics2D g2d = (Graphics2D) g;

		// enable antialiasing
		final RenderingHints hints = new RenderingHints(
				RenderingHints.KEY_ANTIALIASING,
				RenderingHints.VALUE_ANTIALIAS_ON
		);
		g2d.setRenderingHints(hints);

		// clear background
		g2d.setColor(Color.WHITE);
		g2d.fillRect(0, 0, getWidth(), getHeight());

		// interpolate a sequence of bezier curves
		final List<Double> toInterpolate = new ArrayList<>();
		points.forEach(p -> {
			toInterpolate.add(p.getX());
			toInterpolate.add(p.getY());
		});
		final BSpline spline = Utils.interpolateCubic(
				toInterpolate, 2).toBeziers();
		if (spline.getControlPoints().size() % 4 != 0) { // just to be sure
			throw new IllegalStateException(
					"Unexpected number of control points.");
		}

		// draw sequence curve by curve
		List<Double> ctrlp = spline.getControlPoints();
		final int order = (int) spline.getOrder();
		final int dim = (int) spline.getDimension();
		final int nBeziers = (int) (ctrlp.size() / dim) / order;
		final Path2D.Double path = new Path2D.Double();
		for (int i = 0; i < nBeziers; i++) {
			path.moveTo(ctrlp.get(i*dim*order), ctrlp.get(i*dim*order + 1));
			path.curveTo(
					ctrlp.get(i*dim*order + 2), ctrlp.get(i*dim*order + 3),
					ctrlp.get(i*dim*order + 4), ctrlp.get(i*dim*order + 5),
					ctrlp.get(i*dim*order + 6), ctrlp.get(i*dim*order + 7));
		}
		g2d.setColor(Color.BLACK);
		g2d.draw(path);

		// draw original control points
		g2d.setColor(Color.RED);
		points.stream()
				.map(p -> new Rectangle2D.Double(
						p.getX() - 2, p.getY() - 2, 4, 4))
				.forEach(g2d::fill);
	}

	@Override
	public void mouseDragged(final MouseEvent mouseEvent) {
		if (dragged != null) {
			dragged.setLocation(mouseEvent.getPoint());
			repaint();
		}
	}

	@Override
	public void mouseMoved(final MouseEvent mouseEvent) {
		final Optional<Point2D> any = points.stream()
				.filter(p -> p.distance(mouseEvent.getPoint()) < 10)
				.findAny();
		if (any.isPresent()) {
			setCursor(new Cursor(Cursor.HAND_CURSOR));
			dragged = any.get();
		} else {
			setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
			dragged = null;
		}
		repaint();
	}

	@Override
	public void mouseClicked(final MouseEvent mouseEvent) {}

	@Override
	public void mousePressed(final MouseEvent mouseEvent) {}

	@Override
	public void mouseReleased(final MouseEvent mouseEvent) {
		if (mouseEvent.getButton() == MouseEvent.BUTTON3) {
			points.add(mouseEvent.getPoint());
			repaint();
		}
	}

	@Override
	public void mouseEntered(final MouseEvent mouseEvent) {}

	@Override
	public void mouseExited(final MouseEvent mouseEvent) {}

	public static void main(final String[] args) {
		SwingUtilities.invokeLater(Swing::new);
	}
}
