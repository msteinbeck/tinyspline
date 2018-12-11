library(methods)
library(Rcpp)

dyn.load(paste("tinyspliner", .Platform$dynlib.ext, sep=""))
source("tinyspline.R")
cacheMetaData(1)

# Create a cubic spline with 7 control points in 2D using
# a clamped knot vector. This call is equivalent to:
# spline <- BSpline(7, 2, 3, BSplineType.CLAMPED)
spline <- BSpline(7)

# Setup control points.
ctrlp <- BSpline_controlPoints_get(spline)
ctrlp[1]  = -1.75 # x0
ctrlp[2]  = -1.0  # y0
ctrlp[3]  = -1.5  # x1
ctrlp[4]  = -0.5  # y1
ctrlp[5]  = -1.5  # x2
ctrlp[6]  =  0.0  # y2
ctrlp[7]  = -1.25 # x3
ctrlp[8]  =  0.5  # y3
ctrlp[9]  = -0.75 # x4
ctrlp[10] =  0.75 # y4
ctrlp[11] =  0.0  # x5
ctrlp[12] =  0.5  # y5
ctrlp[13] =  0.5  # x6
ctrlp[14] =  0.0  # y6
invisible(BSpline_controlPoints_set(spline, ctrlp))

# Evaluate `spline` at u = 0.4.
result = BSpline_eval(spline, 0.4)
result = DeBoorNet_result_get(.Last.value)
cat("x = ", result[1], ", y = ", result[2], "\n")

# Derive `spline` and subdivide it into a sequence of Bezier curves.
beziers = BSpline_derive(spline)
beziers = BSpline_toBeziers(.Last.value)

# Evaluate `beziers` at u = 0.3.
result = BSpline_eval(beziers, 0.3)
result = DeBoorNet_result_get(.Last.value)
cat("x = ", result[1], ", y = ", result[2], "\n")
