#include <string>

#include "ScilabVariablesHandlerTest.hxx"
#include "ScilabToJava.hxx"

extern "C"
{
#include "stack-c.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "getScilabJavaVM.h"

    int sci_testHandler(char * fname, unsigned long fname_len);

}

int sci_testHandler(char * fname, unsigned long fname_len)
{
    SciErr err;
    int * addr = 0;
    int handlerId;
    const char * result = 0;

    CheckRhs(1,1);
    CheckLhs(1,1);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    handlerId = Test::ScilabVariablesHandlerTest::startTest(getScilabJavaVM());
    org_modules_types::ScilabToJava::sendVariable(std::string(""), addr, true, handlerId, pvApiCtx);
    result = Test::ScilabVariablesHandlerTest::getTestResult(getScilabJavaVM());
    Test::ScilabVariablesHandlerTest::endTest(getScilabJavaVM());

    err = createMatrixOfString(pvApiCtx, Rhs + 1, 1, 1, const_cast<const char* const*>(&result));
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    LhsVar(1) = Rhs + 1;

    return 0;
}
