## C++ Interface

The following listing gives an example of TinySpline's C++ API. Depending on
its configuration, _tinyspline::real_ reprsents either a _float_ or a _double_:

```c++
#include <iostream>
#include "tinysplinecpp.h"

int main(int argc, char **argv)
{
        // Create a cubic spline with 7 control points in 2D using
        // a clamped knot vector. This call is equivalent to:
        // tinyspline::BSpline spline(7, 2, 3, TS_CLAMPED);
        tinyspline::BSpline spline(7);

        // Setup control points.
        std::vector<tinyspline::real> ctrlp = spline.ctrlp();
        ctrlp[0]  = -1.75; // x0
        ctrlp[1]  = -1.0;  // y0
        ctrlp[2]  = -1.5;  // x1
        ctrlp[3]  = -0.5;  // y1
        ctrlp[4]  = -1.5;  // x2
        ctrlp[5]  =  0.0;  // y2
        ctrlp[6]  = -1.25; // x3
        ctrlp[7]  =  0.5;  // y3
        ctrlp[8]  = -0.75; // x4
        ctrlp[9]  =  0.75; // y4
        ctrlp[10] =  0.0;  // x5
        ctrlp[11] =  0.5;  // y5
        ctrlp[12] =  0.5;  // x6
        ctrlp[13] =  0.0;  // y6
        spline.setCtrlp(ctrlp);

        // Stores our evaluation results.
        std::vector<tinyspline::real> result;

        // Evaluate `spline` at u = 0.4 using 'evaluate'.
        result = spline.evaluate(0.4).result();
        std::cout << "x = " << result[0] << ", y = " << result[1] << std::endl;

        // Derive `spline` and subdivide it into a sequence of Bezier curves.
        tinyspline::BSpline beziers = spline.derive().toBeziers();

        // Evaluate `beziers` at u = 0.3 using '()' instead of 'evaluate'.
        result = beziers(0.3).result();
        std::cout << "x = " << result[0] << ", y = " << result[1] << std::endl;

        return 0;
}
```

You can download the full example here. The zip archive contains the files:

- `tinyspline.h` and `tinyspline.c` — The core library of TinySpline
  implemented in C.
- `tinysplinecpp.h` and `tinysplinecpp.cpp` — The object-oriented C++ wrapper.
- `quickstart.cpp` — The source code listed above.

Run the following command to compile the example using GCC:

```bash
g++ -std=c++11 tinyspline.c tinysplinecpp.cpp quickstart.cpp -o quickstart
```

If you prefer using MSVC on Windows, run the following command instead:

```bash
cl.exe tinyspline.c tinysplinecpp.cpp quickstart.cpp /out:quickstart.exe
```

Besides GCC and MSVC, Clang is officially supported, too. You can use the
preprocessor definition `TINYSPLINE_DISABLE_CXX11_FEATURES` to disable C++11
related features. That way compiler suites without proper support for C++11 are
able to compile source code using TinySpline. The following command shows how
to compile the example using GCC and C++98:

```bash
g++ -std=c++98 -DTINYSPLINE_DISABLE_CXX11_FEATURES tinyspline.c tinysplinecpp.cpp quickstart.cpp -o quickstart
```

Again, if you prefer using MSVC on Windows, run the following command instead:

```bash
cl.exe /DTINYSPLINE_DISABLE_CXX11_FEATURES tinyspline.c tinysplinecpp.cpp quickstart.cpp /out:quickstart.exe
```

As already indicated, TinySpline can be configured with single or double
precision support for _tinyspline::real_. The default configuration is
_double_. You can change this to _float_ by supplying the preprocessor
definition `TINYSPLINE_FLOAT_PRECISION`:

```bash
g++ -std=c++11 -DTINYSPLINE_FLOAT_PRECISION tinyspline.c tinysplinecpp.cpp quickstart.cpp -o quickstart
```

As always, MSVC users may run the following command instead:

```bash
cl.exe /DTINYSPLINE_FLOAT_PRECISION tinyspline.c tinysplinecpp.cpp quickstart.cpp /out:quickstart.exe
```

It is, of course, possible to combine `TINYSPLINE_DISABLE_CXX11_FEATURES` and
`TINYSPLINE_FLOAT_PRECISION` to adjust TinySpline to your requirements. Note,
however, that pre-built packages are compiled with default configuration.

## C Interface

```c
#include "tinyspline.h"
#include <stdio.h>

int main(int argc, char **argv)
{
    /* Create a spline... */
    tsBSpline spline;
    ts_bspline_new(
        3, /* ... of degree 3... */
        2, /* ... in 2D... */
        7, /* ... consisting of 7 control points... */
        TS_CLAMPED, /* ... using a clamped knot vector. */
        &spline
    );

    /* Setup control points. */
    spline.ctrlp[0]  = -1.75f; /* x0 */
    spline.ctrlp[1]  = -1.0f;  /* y0 */
    spline.ctrlp[2]  = -1.5f;  /* x1 */
    spline.ctrlp[3]  = -0.5f;  /* y1 */
    spline.ctrlp[4]  = -1.5f;  /* x2 */
    spline.ctrlp[5]  =  0.0f;  /* y2 */
    spline.ctrlp[6]  = -1.25f; /* x3 */
    spline.ctrlp[7]  =  0.5f;  /* y3 */
    spline.ctrlp[8]  = -0.75f; /* x4 */
    spline.ctrlp[9]  =  0.75f; /* y4 */
    spline.ctrlp[10] =  0.0f;  /* x5 */
    spline.ctrlp[11] =  0.5f;  /* y5 */
    spline.ctrlp[12] =  0.5f;  /* x6 */
    spline.ctrlp[13] =  0.0f;  /* y6 */

    /* Stores our evaluation results. */
    tsDeBoorNet net;

    /* Evaluate `spline` at u = 0.4. */
    ts_bspline_evaluate(&spline, 0.4f, &net);
    printf("x = %f, y = %f\n", net.result[0], net.result[1]);
    ts_deboornet_free(&net);

    /* Stores our transformed spline. */
    tsBSpline beziers;
    /* Derive `spline` ... */
    ts_bspline_derive(&spline, &beziers);
    /* ... and subdivide it into a sequence of Bezier curves. */
    ts_bspline_to_beziers(&beziers, &beziers);

    /* Evaluate `beziers` at u = 0.3. */
    ts_bspline_evaluate(&beziers, 0.3f, &net);
    printf("x = %f, y = %f\n", net.result[0], net.result[1]);
    ts_deboornet_free(&net);

    /* Cleanup */
    ts_bspline_free(&spline);
    ts_bspline_free(&beziers);

    return 0;
}
```

## C\# Interface

```csharp
using TinySpline;
using System;
using System.Collections.Generic;

class QuickStart
{
        public static void Main (string[] args)
        {
                // Create a cubic spline with 7 control points in 2D using
                // a clamped knot vector. This call is equivalent to:
                // BSpline spline = new BSpline(7, 2, 3, BSplineType.CLAMPED);
                BSpline spline = new BSpline(7);

                // Setup control points.
                IList<double> ctrlp = spline.ctrlp;
                ctrlp[0]  = -1.75; // x0
                ctrlp[1]  = -1.0;  // y0
                ctrlp[2]  = -1.5;  // x1
                ctrlp[3]  = -0.5;  // y1
                ctrlp[4]  = -1.5;  // x2
                ctrlp[5]  =  0.0;  // y2
                ctrlp[6]  = -1.25; // x3
                ctrlp[7]  =  0.5;  // y3
                ctrlp[8]  = -0.75; // x4
                ctrlp[9]  =  0.75; // y4
                ctrlp[10] =  0.0;  // x5
                ctrlp[11] =  0.5;  // y5
                ctrlp[12] =  0.5;  // x6
                ctrlp[13] =  0.0;  // y6
                spline.ctrlp = ctrlp;

                // Evaluate `spline` at u = 0.4.
                IList<double> result = spline.evaluate(0.4).result;
                Console.WriteLine("x = {0}, y = {1}", result[0], result[1]));

                // Derive `spline` and subdivide it into a sequence of Bezier curves.
                BSpline beziers = spline.derive().toBeziers();

                // Evaluate `beziers` at u = 0.3.
                result = beziers.evaluate(0.3).result;
                Console.WriteLine("x = {0}, y = {1}", result[0], result[1]));
        }
}
```

## D Interface

```d
import std.stdio;
import tinyspline;

void main()
{
        // Create a cubic spline with 7 control points in 2D using
        // a clamped knot vector. This call is equivalent to:
        // BSpline spline = new BSpline(7, 2, 3, BSplineType.CLAMPED);
        BSpline spline = new BSpline(7);

        // Setup control points.
        Vector ctrlp = spline.ctrlp();
        ctrlp[0]  = -1.75; // x0
        ctrlp[1]  = -1.0;  // y0
        ctrlp[2]  = -1.5;  // x1
        ctrlp[3]  = -0.5;  // y1
        ctrlp[4]  = -1.5;  // x2
        ctrlp[5]  =  0.0;  // y2
        ctrlp[6]  = -1.25; // x3
        ctrlp[7]  =  0.5;  // y3
        ctrlp[8]  = -0.75; // x4
        ctrlp[9]  =  0.75; // y4
        ctrlp[1]  =  0.0;  // x5
        ctrlp[11] =  0.5;  // y5
        ctrlp[12] =  0.5;  // x6
        ctrlp[13] =  0.0;  // y6
        spline.setCtrlp(ctrlp);

        // Evaluate `spline` at u = 0.4 using 'evaluate'.
        Vector result = spline.evaluate(0.4).result();
        writefln("x = %f, y = %f", result[0], result[1]);

        // Derive `spline` and subdivide it into a sequence of Bezier curves.
        BSpline beziers = spline.derive().toBeziers();

        // Evaluate `beziers` at u = 0.3 using '()' instead of 'evaluate'.
        result = beziers(0.3).result();
        writefln("x = %f, y = %f", result[0], result[1]);
}
```

## Java Interface

```java
import org.tinyspline.BSpline;
import org.tinyspline.BSplineType;

import java.util.List;

public class QuickStart {
        public static void main(final String[] args) {
                System.loadLibrary("tinysplinejava");

                // Create a cubic spline with 7 control points in 2D using
                // a clamped knot vector. This call is equivalent to:
                // BSpline spline = new BSpline(7, 2, 3, BSplineType.CLAMPED);
                final BSpline spline = new BSpline(7);

                // Setup control points.
                final List<Double> ctrlp = spline.getCtrlp();
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
                spline.setCtrlp(ctrlp);

                // Evaluate `spline` at u = 0.4.
                List<Double> result = spline.evaluate(0.4).getResult();
                System.out.printf("x = %f, y = %f%n",
                                result.get(0), result.get(1));

                // Derive `spline` and subdivide it into a sequence of Bezier curves.
                final BSpline beziers = spline.derive().toBeziers();

                // Evaluate `beziers` at u = 0.3.
                result = beziers.evaluate(0.3).getResult();
                System.out.printf("x = %f, y = %f%n",
                                result.get(0), result.get(1));
        }
}
```

## Lua Interface

```lua
local ts = require("tinysplinelua")

-- Create a cubic spline with 7 control points in 2D using
-- a clamped knot vector. This call is equivalent to:
-- spline = ts.BSpline(7, 2, 3, ts.CLAMPED)
spline = ts.BSpline(7)

-- Setup control points.
ctrlp = spline.ctrlp
ctrlp[1]  = -1.75 -- x0
ctrlp[2]  = -1.0  -- y0
ctrlp[3]  = -1.5  -- x1
ctrlp[4]  = -0.5  -- y1
ctrlp[5]  = -1.5  -- x2
ctrlp[6]  =  0.0  -- y2
ctrlp[7]  = -1.25 -- x3
ctrlp[8]  =  0.5  -- y3
ctrlp[9]  = -0.75 -- x4
ctrlp[10] =  0.75 -- y4
ctrlp[11] =  0.0  -- x5
ctrlp[12] =  0.5  -- y5
ctrlp[13] =  0.5  -- x6
ctrlp[14] =  0.0  -- y6
spline.ctrlp = ctrlp

-- Evaluate `spline` at u = 0.4 using 'evaluate'.
result = spline:evaluate(0.4).result
print("x = ", result[1], "y = ", result[2]);

-- Derive `spline` and subdivide it into a sequence of Bezier curves.
beziers = spline:derive():toBeziers()

-- Evaluate `beziers` at u = 0.3 using '()' instead of 'evaluate'.
result = beziers(0.3).result
print("x = ", result[1], "y = ", result[2]);
```

## Octave Interface

```octave
# Load Octave module.
tinyspline

# Create a cubic spline with 7 control points in 2D using
# a clamped knot vector. This call is equivalent to:
# spline = ts.BSpline(7, 2, 3, ts.CLAMPED)
spline = tinyspline.BSpline(7);

# Setup control points.
ctrlp = spline.ctrlp;
ctrlp(1)  = -1.75; # x0
ctrlp(2)  = -1.0;  # y0
ctrlp(3)  = -1.5;  # x1
ctrlp(4)  = -0.5;  # y1
ctrlp(5)  = -1.5;  # x2
ctrlp(6)  =  0.0;  # y2
ctrlp(7)  = -1.25; # x3
ctrlp(8)  =  0.5;  # y3
ctrlp(9)  = -0.75; # x4
ctrlp(10) =  0.75; # y4
ctrlp(11) =  0.0;  # x5
ctrlp(12) =  0.5;  # y5
ctrlp(13) =  0.5;  # x6
ctrlp(14) =  0.0;  # y6
spline.ctrlp = ctrlp;

# Evaluate `spline` at u = 0.4 using 'evaluate'.
result = spline.evaluate(0.4).result;
printf("x = %d, y = %d\n", result(1), result(2))

# Derive `spline` and subdivide it into a sequence of Bezier curves.
beziers = spline.derive().toBeziers();

# Evaluate `beziers` at u = 0.3 using '()' instead of 'evaluate'.
result = beziers(0.3).result;
printf("x = %d, y = %d\n", result(1), result(2))
```

## PHP Interface

```php
<?php
include("tinyspline.php");

// Create a cubic spline with 7 control points in 2D using
// a clamped knot vector. This call is equivalent to:
// $spline = new BSpline(7, 2, 3, CLAMPED);
$spline = new BSpline(7);

// Setup control points.
$ctrlp = $spline->ctrlp;
$ctrlp[0]  = -1.75; // x0
$ctrlp[1]  = -1.0;  // y0
$ctrlp[2]  = -1.5;  // x1
$ctrlp[3]  = -0.5;  // y1
$ctrlp[4]  = -1.5;  // x2
$ctrlp[5]  =  0.0;  // y2
$ctrlp[6]  = -1.25; // x3
$ctrlp[7]  =  0.5;  // y3
$ctrlp[8]  = -0.75; // x4
$ctrlp[9]  =  0.75; // y4
$ctrlp[10] =  0.0;  // x5
$ctrlp[11] =  0.5;  // y5
$ctrlp[12] =  0.5;  // x6
$ctrlp[13] =  0.0;  // y6
$spline->ctrlp = $ctrlp;

// Evaluate `spline` at u = 0.4.
$result = $spline->evaluate(0.4)->result;
echo "x = $result[0], y = $result[1]\n";

// Derive `spline` and subdivide it into a sequence of Bezier curves.
$beziers = $spline->derive()->toBeziers();

// Evaluate `beziers` at u = 0.3.
$result = $beziers->evaluate(0.3)->result;
echo "x = $result[0], y = $result[1]\n";
?>
```

## Python Interface

```python
from tinyspline import *

# Create a cubic spline with 7 control points in 2D using
# a clamped knot vector. This call is equivalent to:
# spline = BSpline(7, 2, 3, CLAMPED)
spline = BSpline(7)

# Setup control points.
ctrlp = spline.ctrlp
ctrlp[0]  = -1.75 # x0
ctrlp[1]  = -1.0  # y0
ctrlp[2]  = -1.5  # x1
ctrlp[3]  = -0.5  # y1
ctrlp[4]  = -1.5  # x2
ctrlp[5]  =  0.0  # y2
ctrlp[6]  = -1.25 # x3
ctrlp[7]  =  0.5  # y3
ctrlp[8]  = -0.75 # x4
ctrlp[9]  =  0.75 # y4
ctrlp[10] =  0.0  # x5
ctrlp[11] =  0.5  # y5
ctrlp[12] =  0.5  # x6
ctrlp[13] =  0.0  # y6
spline.ctrlp = ctrlp

# Evaluate `spline` at u = 0.4 using 'evaluate'.
result = spline.evaluate(0.4).result
print("x = ", result[0], ", y = ", result[1])

# Derive `spline` and subdivide it into a sequence of Bezier curves.
beziers = spline.derive().toBeziers()

# Evaluate `beziers` at u = 0.3 using '()' instead of 'evaluate'.
result = beziers(0.3).result
print("x = ", result[0], ", y = ", result[1])
```

## R Interface

```r
library(methods)
library(Rcpp)

dyn.load(paste("tinyspliner", .Platform$dynlib.ext, sep=""))
source("tinyspline.R")
cacheMetaData(1)

# Create a cubic spline with 7 control points in 2D using
# a clamped knot vector. This call is equivalent to:
# spline <- BSpline(7)
spline <- BSpline(7)

# Setup control points.
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

# Evaluate `spline` at u = 0.4.
result = BSpline_evaluate(spline, 0.4)
result = DeBoorNet_result_get(.Last.value)
cat("x = ", result[1], "y = ", result[2], "\n")

# Derive `spline` and subdivide it into a sequence of Bezier curves.
beziers = BSpline_derive(spline)
beziers = BSpline_toBeziers(.Last.value)

# Evaluate `beziers` at u = 0.3.
result = BSpline_evaluate(beziers, 0.3)
result = DeBoorNet_result_get(.Last.value)
cat("x = ", result[1], "y = ", result[2], "\n")
```

## Ruby Interface

```ruby
require 'tinysplineruby'

# Create a cubic spline with 7 control points in 2D using
# a clamped knot vector. This call is equivalent to:
# spline = Tinysplineruby::BSpline.new(7, 2, 3, Tinysplineruby::CLAMPED)
spline = Tinysplineruby::BSpline.new(7)

# Setup control points.
ctrlp = spline.ctrlp
ctrlp[0]  = -1.75; # x0
ctrlp[1]  = -1.0;  # y0
ctrlp[2]  = -1.5;  # x1
ctrlp[3]  = -0.5;  # y1
ctrlp[4]  = -1.5;  # x2
ctrlp[5]  =  0.0;  # y2
ctrlp[6]  = -1.25; # x3
ctrlp[7]  =  0.5;  # y3
ctrlp[8]  = -0.75; # x4
ctrlp[9]  =  0.75; # y4
ctrlp[10] =  0.0;  # x5
ctrlp[11] =  0.5;  # y5
ctrlp[12] =  0.5;  # x6
ctrlp[13] =  0.0;  # y6
spline.ctrlp = ctrlp

# Evaluate `spline` at u = 0.4 using 'evaluate'.
result = spline.evaluate(0.4).result
puts "x = #{result[0]}, y = #{result[1]}"

# Derive `spline` and subdivide it into a sequence of Bezier curves.
beziers = spline.derive.toBeziers

# Evaluate `beziers` at u = 0.3 using '()' instead of 'evaluate'.
result = beziers.call(0.3).result
puts "x = #{result[0]}, y = #{result[1]}"
```
