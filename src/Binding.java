package com.github.tinyspline;

import com.sun.jna.IntegerType;
import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.Structure;

import java.util.Arrays;
import java.util.List;

class Binding {
    /********************************************************
    *                                                       *
    * Singleton setup                                       *
    *                                                       *
    ********************************************************/
    private static final TSLibrary INSTANCE = (TSLibrary) Native.loadLibrary("tinyspline", TSLibrary.class);

    public static TSLibrary getInstance() {
        return INSTANCE;
    }

    private Binding() {}


    /********************************************************
    *                                                       *
    * Utility                                               *
    *                                                       *
    ********************************************************/
    public static class size_t extends IntegerType {
        public size_t() {
            this(0);
        }

        public size_t(long value) {
            super(Native.SIZE_T_SIZE, value);
        }
    }


    /********************************************************
    *                                                       *
    * Bindings & Library                                    *
    *                                                       *
    ********************************************************/
    public static class ErrorBinding {
	    public static final int SUCCESS = 0;
	    public static final int MALLOC = -1;
	    public static final int DIM_ZERO = -2;
	    public static final int DEG_GE_NCTRLP = -3;
	    public static final int U_UNDEFINED = -4;
	    public static final int MULTIPLICITY = -5;
    }

    public static class BSplineTypeBinding {
	    public static final int OPENED = 0;
	    public static final int CLAMPED = 1;
    }

    public static class DeBoorNetBinding extends Structure {
        public int k;
        public int s;
        public int h;
        public size_t deg;
        public size_t dim;
        public size_t n_affected;
        public size_t n_points;
        public size_t last_idx;
        public Pointer points;

        @Override
        protected List getFieldOrder() {
            return Arrays.asList("k", "s", "h", "deg", "dim", "n_affected", "n_points", "last_idx", "points");
        }
    }

    public static class BSplineBinding extends Structure {
	    public size_t deg;
	    public size_t order;
	    public size_t dim;
	    public size_t n_ctrlp;
	    public size_t n_knots;
	    public Pointer ctrlp;
	    public Pointer knots;

        @Override
        protected List getFieldOrder() {
            return Arrays.asList("deg", "order", "dim", "n_ctrlp", "n_knots", "ctrlp", "knots");
        }
    }

    public static class BSplineSequenceBinding extends Structure {
        public size_t n_bsplines;
        public Pointer bsplines;

        @Override
        protected List getFieldOrder() {
            return Arrays.asList("n_bsplines", "bsplines");
        }
    }

    public static interface TSLibrary extends Library {
        void ts_deboornet_default(DeBoorNetBinding deBoorNet);
        void ts_deboornet_free(DeBoorNetBinding deBoorNet);
        void ts_bspline_default(BSplineBinding bspline);
        void ts_bspline_free(BSplineBinding bspline);
        void ts_bsplinesequence_default(BSplineSequenceBinding sequence);
        void ts_bsplinesequence_free(BSplineSequenceBinding sequence);

        int ts_bspline_new(
            final size_t deg, final size_t dim, final size_t n_ctrlp, final int type,
            BSplineBinding bspline
        );

        int ts_bspline_copy(
            final BSplineBinding original,
            BSplineBinding copy
        );

        int ts_bspline_evaluate(
            final BSplineBinding bspline, final float u,
            DeBoorNetBinding deBoorNet
        );

        int ts_bspline_split(
            final BSplineBinding bspline, final float u,
            BSplineSequenceBinding split
        );

        int ts_bspline_buckle(
            final BSplineBinding original, final float b,
            BSplineBinding buckled
        );

        int ts_bsplinesequence_new(
            final size_t n,
            BSplineSequenceBinding sequence
        );
    }
}
