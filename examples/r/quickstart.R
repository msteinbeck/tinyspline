library(methods)
library(Rcpp)

dyn.load(paste("tinyspliner", .Platform$dynlib.ext, sep=""))
source("tinyspline.R")
cacheMetaData(1)

spline <- BSpline(7)

ctrlp <- BSpline_ctrlp_get(spline)
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
invisible(BSpline_ctrlp_set(spline, ctrlp))

result = BSpline_evaluate(spline, 0.4)
result = DeBoorNet_result_get(.Last.value)
cat("x = ", result[1], "y = ", result[2], "\n")

beziers = BSpline_derive(spline)
beziers = BSpline_toBeziers(.Last.value)

result = BSpline_evaluate(beziers, 0.3)
result = DeBoorNet_result_get(.Last.value)
cat("x = ", result[1], "y = ", result[2], "\n")
