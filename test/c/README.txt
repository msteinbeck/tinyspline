HOW TO USE

You can use CuTest to create unit tests to drive your development
in the style of Extreme Programming. You can also add unit tests to
existing code to ensure that it works as you suspect.

Your unit tests are an investment. They let you to change your
code and add new features confidently without worrying about
accidentally breaking earlier features.


LICENSING

For details on licensing see license.txt.


GETTING STARTED

To add unit testing to your C code the only files you need are
CuTest.c and CuTest.h. 

CuTestTest.c and AllTests.c have been included to provide an
example of how to write unit tests and then how to aggregate them
into suites and into a single AllTests.c file. Suites allow you
to put group tests into logical sets. AllTests.c combines all the
suites and runs them. 

You should not have to look inside CuTest.c. Looking in
CuTestTest.c and AllTests.c (for example usage) should be
sufficient. 

After downloading the sources, run your compiler to create an
executable called AllTests.exe. For example, if you are using
Windows with the cl.exe compiler you would type: 

    cl.exe AllTests.c CuTest.c CuTestTest.c
    AllTests.exe

This will run all the unit tests associated with CuTest and print
the output on the console. You can replace cl.exe with gcc or
your favorite compiler in the command above.


DETAILED EXAMPLE

Here is a more detailed example. We will work through a simple
test first exercise. The goal is to create a library of string
utilities. First, lets write a function that converts a
null-terminated string to all upper case.

Ensure that CuTest.c and CuTest.h are accessible from your C
project. Next, create a file called StrUtil.c with these
contents:

    #include "CuTest.h"
    
    char* StrToUpper(char* str) {
        return str;
    }
    
    void TestStrToUpper(CuTest *tc) {
        char* input = strdup("hello world");
        char* actual = StrToUpper(input);
        char* expected = "HELLO WORLD";
        CuAssertStrEquals(tc, expected, actual);
    }
   
    CuSuite* StrUtilGetSuite() {
        CuSuite* suite = CuSuiteNew();
        SUITE_ADD_TEST(suite, TestStrToUpper);
        return suite;
    }
    
Create another file called AllTests.c with these contents:

    #include "CuTest.h"
    
    CuSuite* StrUtilGetSuite();
    
    void RunAllTests(void) {
        CuString *output = CuStringNew();
        CuSuite* suite = CuSuiteNew();
        
        CuSuiteAddSuite(suite, StrUtilGetSuite());
    
        CuSuiteRun(suite);
        CuSuiteSummary(suite, output);
        CuSuiteDetails(suite, output);
        printf("%s\n", output->buffer);
    }
    
    int main(void) {
        RunAllTests();
    }

Then type this on the command line:

    gcc AllTests.c CuTest.c StrUtil.c

to compile. You can replace gcc with your favorite compiler.
CuTest should be portable enough to handle all Windows and Unix
compilers. Then to run the tests type:

    a.out

This will print an error because we haven't implemented the
StrToUpper function correctly. We are just returning the string
without changing it to upper case. 

    char* StrToUpper(char* str) {
        return str;
    }

Rewrite this as follows:

    char* StrToUpper(char* str) {
        char* p;
        for (p = str ; *p ; ++p) *p = toupper(*p);
        return str;
    }

Recompile and run the tests again. The test should pass this
time.


WHAT TO DO NEXT

At this point you might want to write more tests for the
StrToUpper function. Here are some ideas:

TestStrToUpper_EmptyString :  pass in ""
TestStrToUpper_UpperCase   :  pass in "HELLO WORLD"
TestStrToUpper_MixedCase   :  pass in "HELLO world"
TestStrToUpper_Numbers     :  pass in "1234 hello"

As you write each one of these tests add it to StrUtilGetSuite
function. If you don't the tests won't be run. Later as you write
other functions and write tests for them be sure to include those
in StrUtilGetSuite also. The StrUtilGetSuite function should
include all the tests in StrUtil.c

Over time you will create another file called FunkyStuff.c
containing other functions unrelated to StrUtil. Follow the same
pattern. Create a FunkyStuffGetSuite function in FunkyStuff.c.
And add FunkyStuffGetSuite to AllTests.c.

The framework is designed in the way it is so that it is easy to
organize a lot of tests.

THE BIG PICTURE

Each individual test corresponds to a CuTest. These are grouped
to form a CuSuite. CuSuites can hold CuTests or other CuSuites.
AllTests.c collects all the CuSuites in the program into a single
CuSuite which it then runs as a single CuSuite.

The project is open source so feel free to take a peek under the
hood at the CuTest.c file to see how it works. CuTestTest.c
contains tests for CuTest.c. So CuTest tests itself.

Since AllTests.c has a main() you will need to exclude this when
you are building your product. Here is a nicer way to do this if
you want to avoid messing with multiple builds. Remove the main()
in AllTests.c. Note that it just calls RunAllTests(). Instead
we'll call this directly from the main program.

Now in the main() of the actual program check to see if the
command line option "--test" was passed. If it was then I call
RunAllTests() from AllTests.c. Otherwise run the real program.

Shipping the tests with the code can be useful. If you customers
complain about a problem you can ask them to run the unit tests
and send you the output. This can help you to quickly isolate the
piece of your system that is malfunctioning in the customer's
environment. 

CuTest offers a rich set of CuAssert functions. Here is a list:

void CuAssert(CuTest* tc, char* message, int condition);
void CuAssertTrue(CuTest* tc, int condition);
void CuAssertStrEquals(CuTest* tc, char* expected, char* actual);
void CuAssertIntEquals(CuTest* tc, int expected, int actual);
void CuAssertPtrEquals(CuTest* tc, void* expected, void* actual);
void CuAssertPtrNotNull(CuTest* tc, void* pointer);

The project is open source and so you can add other more powerful
asserts to make your tests easier to write and more concise.
Please feel free to send me changes you make so that I can
incorporate them into future releases.

If you see any errors in this document please contact me at
asimjalis@peakprogramming.com.


AUTOMATING TEST SUITE GENERATION

make-tests.sh will grep through all the .c files in the current
directory and generate the code to run all the tests contained in
them. Using this script you don't have to worry about writing
AllTests.c or dealing with any of the other suite code.


CREDITS

These people have contributed useful code changes to the CuTest project.
Thanks!

- [02.23.2003] Dave Glowacki <dglo@hyde.ssec.wisc.edu>
- [04.17.2009] Tobias Lippert <herrmarder@googlemail.com>
- [11.13.2009] Eli Bendersky <eliben@gmail.com>
- [12.14.2009] Andrew Brown <abrown@datasci.com>
