#include <stdio.h>
#define __API_UNSAFE__
#include "api_scilab.h"
#include "gw_functions.h"
#include "string.h"

/*old*/
int sci_api_old_empty(char* fname, void* pvApiCtx)
{
    int rhs = nbInputArgument(pvApiCtx);
    createScalarDouble(pvApiCtx, rhs + 1, 1);
    LhsVar(1) = rhs + 1;
    PutLhsVar();
    return 0;
}

int sci_api_old_work(char *fname, void* pvApiCtx)
{
    int rhs = nbInputArgument(pvApiCtx);
    double ret = 0;
    for (int i = 0; i < rhs; ++i)
    {
        int* addr = NULL;
        getVarAddressFromPosition(pvApiCtx, i + 1, &addr);
        if (addr)
        {
            int type;
            getVarType(pvApiCtx, addr, &type);
            switch (type)
            {
                case sci_matrix:
                {
                    double val;
                    getScalarDouble(pvApiCtx, addr, &val);
                    ret += val;
                    break;
                }
                case sci_strings:
                {
                    wchar_t* val;
                    getAllocatedSingleWideString(pvApiCtx, addr, &val);
                    ret += wcslen(val);
                    freeAllocatedSingleWideString(val);
                    break;
                }

                case sci_mlist:
                {
                    break;
                }

                default:
                {
                    return 0;
                }
            }
        }
    }

    createScalarDouble(pvApiCtx, rhs + 1, ret);
    LhsVar(1) = rhs + 1;
    PutLhsVar();
    return 0;
}

int sci_api_empty(scilabEnv env, int nin, scilabVar* in, int nout, scilabVar* out)
{
    out[0] = scilab_createDouble(env, 1);
    if (out[0] == NULL)
    {
        printf("check works !\n");
    }
    return 0;
}

double computework(scilabEnv env, scilabVar item)
{
    double ret = 0;
    switch (scilab_getType(env, item))
    {
        case sci_matrix:
        {
            double* val;
            scilab_getDoubleArray(env, item, &val);
            ret = val[0];
            break;
        }

        case sci_strings:
        {
            wchar_t** wc = NULL;
            scilab_getStringArray(env, item, &wc);
            ret = (int)wcslen(wc[0]);
            break;
        }
        case sci_cell:
        {
            int j = 0;
            scilabVar* items;
            int size = scilab_getSize(env, item);

            scilab_getCellArray(env, item, &items);
            for (j = 0; j < size; ++j)
            {
                void* itemaddr = items[j];
                ret += computework(env, itemaddr);
            }

            break;
        }
        case sci_struct:
        {
            int i = 0;
            wchar_t** fields = NULL;
            int count = scilab_getFields(env, item, &fields);
            for (i = 0; i < count; ++i)
            {
                scilabVar f = scilab_getStructData2d(env, item, fields[i], 0, 0);
                ret += computework(env, f);
            }

            break;
        }
    }

    return ret;
}

int sci_api_work(scilabEnv env, int nin, scilabVar* in, int nout, scilabVar* out)
{
    int i = 0;
    double ret = 0;
    for (i = 0; i < nin; ++i)
    {
        ret += computework(env, in[i]);
    }

    out[0] = scilab_createDouble(env, ret);
    return 0;
}