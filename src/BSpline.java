package com.github.tinyspline;

import static com.github.tinyspline.Binding.BSplineBinding;
import static com.github.tinyspline.Binding.BSplineTypeBinding;
import static com.github.tinyspline.Binding.TSLibrary;
import static com.github.tinyspline.Binding.size_t;

public class BSpline {
    public enum Type {
        OPENED {
            @Override
            public int getValue() {
                return BSplineTypeBinding.OPENED;
            }
        },

        CLAMPED {
            @Override
            public int getValue() {
                return BSplineTypeBinding.CLAMPED;
            }
        };

        public abstract int getValue();
    }

    private static final TSLibrary LIB = Binding.getInstance();
    private final BSplineBinding bspline = new BSplineBinding();

    public BSpline() {
        LIB.ts_bspline_default(bspline);
    }

    public BSpline(final int deg, final int dim, final int nCtrlp, final BSpline.Type type) {
        final int val = LIB.ts_bspline_new(
            new size_t(deg),
            new size_t(dim),
            new size_t(nCtrlp),
            type.getValue(),
            bspline
        );

        if (val == -1) {
            throw new OutOfMemoryError();
        } else if (val < -1) {
            throw new IllegalArgumentException();
        }
    }

    public BSpline(final BSpline other) {
        final int val = LIB.ts_bspline_copy(other.bspline, this.bspline);

        if (val == -1) {
            throw new OutOfMemoryError();
        } else if (val < -1) {
            throw new IllegalArgumentException();
        }
    }


    public BSpline buckle(final float b) {
        final BSpline buckled = new BSpline();
        final int val = LIB.ts_bspline_buckle(this.bspline, b, buckled.bspline);

        if (val == - 1) {
            throw new OutOfMemoryError();
        } else if (val < -1) {
            throw new IllegalArgumentException();
        }

        return buckled;
    }


    @Override
    protected void finalize() throws Throwable {
        LIB.ts_bspline_free(bspline);
        super.finalize();
    }
}
