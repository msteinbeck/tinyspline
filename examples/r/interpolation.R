library(grid)
library(methods)
library(Rcpp)

dyn.load(paste("tinyspliner", .Platform$dynlib.ext, sep=""))
source("tinyspline.R")
cacheMetaData(1)

points <- c(
	0.3, 0.1, 0,
	0.1, 0.4, 0,
	0.3, 0.6, 0,
	0.6, 0.5, 0,
	0.9, 0.9, 0)
spline <- Utils_interpolateCubic(points, 3)
dim    <- BSpline_dimension_get(spline)
order  <- BSpline_order_get(spline)
ctrlp  <- BSpline_controlPoints_get(spline)

X11()
grid.newpage()
xs <- ctrlp[seq(1, length(ctrlp), dim)]
ys <- ctrlp[seq(2, length(ctrlp), dim)]
grid.bezier(xs, ys, id=rep(1:order, each=order))
while (!is.null(dev.list())) Sys.sleep(0.5)
