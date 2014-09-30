/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include <sstream>
#include "core_gw.hxx"
#include "function.hxx"
#include "context.hxx"
#include "scilabWrite.hxx"
#include "string.hxx"
#include "double.hxx"

extern "C"
{
#include <string.h>
#include "stack-c.h"
#include "sci_malloc.h"
#include "stackinfo.h"
#include "sciprint.h"
#include "localization.h"
#include "Scierror.h"
#include "os_strdup.h"
#include "freeArrayOfString.h"

}


using namespace types;

struct VariableStruct
{
    char *NameVariable;
    int SizeVariable;
};

/*--------------------------------------------------------------------------*/
static BOOL SetVariablesStructs(struct VariableStruct **GVstruct, int *GlenStructArray, struct VariableStruct **LVstruct, int *LlenStructArray);
static void DispVariables(struct VariableStruct* Vstruct, char *Message, int lenStructArray, int memused, int memtotal, int varused, int vartotal);
static void SortVarsStructByStrings(struct VariableStruct *Vstruct, int SizeStruct);
static BOOL FreeVariableStructArray(struct VariableStruct* Vstruct, int lenStructArray);
/*--------------------------------------------------------------------------*/
static int NoRhs(struct VariableStruct* GVstruct, int GlenStructArray, struct VariableStruct* LVstruct, int LlenStructArray, BOOL Sorted);
static typed_list OneLhs(struct VariableStruct* Vstruct, int lenStructArray, BOOL Sorted);
static typed_list TwoLhs(struct VariableStruct* Vstruct, int lenStructArray, BOOL Sorted);
/*--------------------------------------------------------------------------*/

Function::ReturnValue sci_who(typed_list& in, int iRetCount, typed_list& out)
{
    static int l1, n1, m1;

    struct VariableStruct *GlobalVariables = NULL;
    struct VariableStruct *LocalVariables = NULL;

    int NbrVarsGlobal = 0;
    int NbrVarsLocal = 0;

    typed_list *o = NULL;

    if (in.size() > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "who", 0, 2);
        return types::Function::Error;
    }
    if (iRetCount > 2)
    {
        Scierror(77, _("%s: Wrong number of ouput argument(s): %d to %d expected.\n"), "who", 0, 2);
        return types::Function::Error;
    }

    SetVariablesStructs(&GlobalVariables, &NbrVarsGlobal, &LocalVariables, &NbrVarsLocal);

    if (in.size() == 0) /* who() */
    {
        NoRhs(GlobalVariables, NbrVarsGlobal, LocalVariables, NbrVarsLocal, FALSE);
    }
    else if (in.size() == 1) /* who('get') or who('global') or who('sorted')*/
    {
        if (in[0]->isString())
        {
            char *Param1String = wide_string_to_UTF8(in[0]->getAs<types::String>()->get(0));


            if ( (strcmp(Param1String, "get") == 0) ||
                    (strcmp(Param1String, "local") == 0) ||
                    (strcmp(Param1String, "global") == 0) ||
                    (strcmp(Param1String, "sorted") == 0) )
            {
                if (strcmp(Param1String, "sorted") == 0)
                {
                    NoRhs(GlobalVariables, NbrVarsGlobal, LocalVariables, NbrVarsLocal, TRUE);
                }
                else if (strcmp(Param1String, "global") == 0)
                {
                    if (iRetCount == 1)
                    {
                        *o = OneLhs(GlobalVariables, NbrVarsGlobal, FALSE);
                    }
                    else /* Lhs == 2 */
                    {
                        *o = TwoLhs(GlobalVariables, NbrVarsGlobal, FALSE);
                    }
                }
                else /* get or local */
                {
                    if (iRetCount == 1)
                    {
                        *o = OneLhs(LocalVariables, NbrVarsLocal, FALSE);
                    }
                    else /* Lhs == 2 */
                    {
                        *o = TwoLhs(LocalVariables, NbrVarsLocal, FALSE);
                    }
                }
            }
            else
            {
                FreeVariableStructArray(GlobalVariables, NbrVarsGlobal);
                FreeVariableStructArray(LocalVariables, NbrVarsLocal);

                Scierror(999, _("%s: Wrong value for input argument #%d: '%s', '%s', '%s' or '%s' expected.\n"), "who", 1, "local" , "get" , "global", "sorted");
                return types::Function::Error;
            }
        }
        else
        {
            FreeVariableStructArray(GlobalVariables, NbrVarsGlobal);
            FreeVariableStructArray(LocalVariables, NbrVarsLocal);

            Scierror(999, _("%s: Wrong value for input argument #%d: '%s', '%s', '%s' or '%s'.\n"), "who", 1, "local", "get", "global", "sorted");
            return types::Function::Error;
        }
    }
    else if (in.size() == 2) /* who('get','sorted') or who('global','sorted') */
    {
        if ( (in[0]->isString()) && (in[1]->isString()) )
        {
            char *Param1String = wide_string_to_UTF8(in[0]->getAs<types::String>()->get(0));;
            char *Param2String = wide_string_to_UTF8(in[1]->getAs<types::String>()->get(0));;


            if (strcmp(Param2String, "sorted"))
            {
                FreeVariableStructArray(GlobalVariables, NbrVarsGlobal);
                FreeVariableStructArray(LocalVariables, NbrVarsLocal);

                Scierror(999, _("%s: Wrong value for input argument #%d: '%s' expected.\n"), "who", 2, "sorted");
                return types::Function::Error;
            }
            else
            {
                if ( (strcmp(Param1String, "get") == 0) ||
                        (strcmp(Param1String, "local") == 0) ||
                        (strcmp(Param1String, "global") == 0) )
                {
                    if (strcmp(Param1String, "global") == 0)
                    {
                        if (iRetCount == 1)
                        {
                            *o = OneLhs(GlobalVariables, NbrVarsGlobal, TRUE);
                        }
                        else /* Lhs == 2 */
                        {
                            *o = TwoLhs(GlobalVariables, NbrVarsGlobal, TRUE);
                        }
                    }
                    else /* get or local */
                    {
                        if (iRetCount == 1)
                        {
                            *o = OneLhs(LocalVariables, NbrVarsLocal, TRUE);
                        }
                        else /* Lhs == 2 */
                        {
                            *o = TwoLhs(LocalVariables, NbrVarsLocal, TRUE);
                        }
                    }
                }
                else
                {
                    FreeVariableStructArray(GlobalVariables, NbrVarsGlobal);
                    FreeVariableStructArray(LocalVariables, NbrVarsLocal);

                    Scierror(999, _("%s: Wrong value for input argument #%d: '%s', '%s' or '%s'.\n"), "who", 1, "local", "get", "global");
                    return types::Function::Error;
                }
            }
        }
        else
        {
            FreeVariableStructArray(GlobalVariables, NbrVarsGlobal);
            FreeVariableStructArray(LocalVariables, NbrVarsLocal);

            Scierror(999, _("%s: Wrong value for input argument #%d: '%s', '%s', '%s' expected. Input argument #%d must be '%s'.\n"), "who", 1, "local", "get", "global", "sorted");
            return types::Function::Error;
        }
    }

    FreeVariableStructArray(GlobalVariables, NbrVarsGlobal);
    FreeVariableStructArray(LocalVariables, NbrVarsLocal);

    /* std::wstringstream wstream;
     wstream << *(symbol::Context::getInstance()) << std::endl;

     scilabForcedWriteW(wstream.str().c_str());*/

    for (int i = 0; i < o->size(); i++)
    {
        out.push_back(in[i]->getAs<types::GenericType>());
    }

    return Function::OK;
}

/*--------------------------------------------------------------------------*/
static void SortVarsStructByStrings(struct VariableStruct *Vstruct, int SizeStruct)
{
    int fin, i;
    for (fin = SizeStruct - 1; fin > 0; fin--)
    {
        int Sorted = FALSE;
        for (i = 0; i < fin; i++)
        {
            if (strcmp(Vstruct[i].NameVariable, Vstruct[i + 1].NameVariable) > 0)
            {
                struct VariableStruct StructTmp;

                StructTmp.NameVariable = Vstruct[i].NameVariable;
                StructTmp.SizeVariable = Vstruct[i].SizeVariable;

                Vstruct[i].NameVariable = Vstruct[i + 1].NameVariable;
                Vstruct[i].SizeVariable = Vstruct[i + 1].SizeVariable;

                Vstruct[i + 1].NameVariable = StructTmp.NameVariable;
                Vstruct[i + 1].SizeVariable = StructTmp.SizeVariable;

                Sorted = TRUE;
            }
        }
        if (!Sorted)
        {
            break;
        }
    }
}
/*--------------------------------------------------------------------------*/
static void DispVariables(struct VariableStruct* Vstruct, char *Message, int lenStructArray, int memused, int memtotal, int varused, int vartotal)
{
    int i = 0;
    sciprint("\n%s\n\n", Message);
    for (i = 1; i < lenStructArray + 1; i++)
    {
        sciprint("%+24s ", Vstruct[i - 1].NameVariable);
        if (i % 4 == 0)
        {
            sciprint("\n");
        }
    }

    sciprint("\n\n");
    sciprint(_(" using %10d elements out of  %10d.\n"), memused, memtotal);
    sciprint(_(" and   %10d variables out of %10d.\n"), varused, vartotal);

}
/*--------------------------------------------------------------------------*/
static BOOL FreeVariableStructArray(struct VariableStruct* Vstruct, int lenStructArray)
{
    BOOL bOK = FALSE;
    if (Vstruct)
    {
        int i = 0;
        for (i = 0; i < lenStructArray; i++)
        {
            if (Vstruct[i].NameVariable)
            {
                FREE(Vstruct[i].NameVariable);
                Vstruct[i].NameVariable = NULL;
            }
            Vstruct[i].SizeVariable = -1;
        }
        FREE(Vstruct);
        Vstruct = NULL;
        bOK = TRUE;
    }
    return bOK;
}
/*--------------------------------------------------------------------------*/
static BOOL SetVariablesStructs(struct VariableStruct **GVstruct, int *GlenStructArray, struct VariableStruct **LVstruct, int *LlenStructArray)
{
    BOOL bOK = FALSE;
    int j = 0;

    int Gused = 0;
    int Gtotal = 0;
    int Lused = 0;
    int Ltotal = 0;

    C2F(getvariablesinfo)(&Ltotal, &Lused);
    C2F(getgvariablesinfo)(&Gtotal, &Gused);

    *LlenStructArray = Lused;
    *GlenStructArray = Gused;

    if (*GlenStructArray)
    {
        *GVstruct = (struct VariableStruct *)MALLOC(sizeof(struct VariableStruct) * (*GlenStructArray + 1));
    }
    if (*LlenStructArray)
    {
        *LVstruct = (struct VariableStruct *)MALLOC(sizeof(struct VariableStruct) * (*LlenStructArray + 1));
    }

    if ( (GVstruct) && (LVstruct) )
    {
        for (j = 1; j < *LlenStructArray + 1; ++j)
        {
            (*LVstruct)[j - 1].NameVariable = getLocalNamefromId(j);
            (*LVstruct)[j - 1].SizeVariable = getLocalSizefromId(j - 1);
        }

        for (j = 0; j < *GlenStructArray; j++)
        {
            (*GVstruct)[j].NameVariable = getGlobalNamefromId(j);
            (*GVstruct)[j].SizeVariable = getGlobalSizefromId(j);
        }
        bOK = TRUE;
    }
    return bOK;
}
/*--------------------------------------------------------------------------*/
static int NoRhs(struct VariableStruct* GVstruct, int GlenStructArray, struct VariableStruct* LVstruct, int LlenStructArray, BOOL Sorted)
{
    int memtotal = 0;
    int memused = 0;
    int vartotal = 0;
    int varused = 0;

    int gmemtotal = 0;
    int gmemused = 0;
    int gvartotal = 0;
    int gvarused = 0;

    C2F(getstackinfo)(&memtotal, &memused);
    C2F(getvariablesinfo)(&vartotal, &varused);

    if (Sorted)
    {
        SortVarsStructByStrings(LVstruct, LlenStructArray);
    }

    DispVariables(LVstruct, _("Your variables are:"), LlenStructArray, memused, memtotal, varused, vartotal);

    C2F(getgstackinfo)(&gmemtotal, &gmemused);
    C2F(getgvariablesinfo)(&gvartotal, &gvarused);

    if (Sorted)
    {
        SortVarsStructByStrings(GVstruct, GlenStructArray);
    }

    DispVariables(GVstruct, _("Your global variables are:"), GlenStructArray, gmemused, gmemtotal, gvarused, gvartotal);

    /*LhsVar(1) = 0;
    PutLhsVar();*/
    return 0;
}
/*--------------------------------------------------------------------------*/
typed_list TwoLhs(struct VariableStruct* Vstruct, int lenStructArray, BOOL Sorted)
{
    char **Tab = NULL;
    int *Size = NULL;
    int i = 0;
    int m = 0;
    int n = 0;
    String *str1 = NULL;
    Double *str2 = NULL;
    typed_list *out = NULL;

    if (Sorted)
    {
        SortVarsStructByStrings(Vstruct, lenStructArray);
    }

    Tab = (char **)MALLOC(sizeof(char*) * lenStructArray);
    Size = (int *)MALLOC(sizeof(int) * lenStructArray);

    for (i = 0; i < lenStructArray; i++)
    {
        Tab[i] = strdup(Vstruct[i].NameVariable);
        Size[i] = Vstruct[i].SizeVariable;
    }

    m = lenStructArray;
    n = 1;

    str1->set(m, n, *Tab);
    out->push_back(str1);

    str2->set(m, n, *Size);
    out->push_back(str2);

    if (Size)
    {
        FREE(Size);
        Size = NULL;
    }


    return *out;
}
/*--------------------------------------------------------------------------*/
typed_list OneLhs(struct VariableStruct* Vstruct, int lenStructArray, BOOL Sorted)
{
    int i = 0;
    int m = 0;
    int n = 0;
    typed_list *out = NULL;
    String *str1 = NULL;

    char **LocalTab = NULL;

    if (Sorted)
    {
        SortVarsStructByStrings(Vstruct, lenStructArray);
    }

    LocalTab = (char **)MALLOC(sizeof(char*) * (lenStructArray));

    for (i = 0; i < lenStructArray; i++)
    {
        if (strlen(Vstruct[i].NameVariable) > 0)
        {
            LocalTab[i] = strdup(Vstruct[i].NameVariable);
        }
    }

    m = lenStructArray;
    n = 1;

    str1->set(m, n, *LocalTab);
    out->push_back(str1);

    return *out;
}
/*--------------------------------------------------------------------------*/
