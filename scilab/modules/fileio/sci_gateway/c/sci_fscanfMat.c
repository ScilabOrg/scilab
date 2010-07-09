/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#if defined(__linux__)
#define _GNU_SOURCE /* Bug 5673 fix: avoid dependency on GLIBC_2.7 */
#endif
/*--------------------------------------------------------------------------*/
#include "gw_fileio.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "scanf_functions.h"
#include "Scierror.h"
#include "localization.h"
#include "expandPathVariable.h"
#include "freeArrayOfString.h"
#include "PATH_MAX.h"
#include "charEncoding.h"
#include "StringConvert.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
#define INFOSIZE 1024
#define DEFAULT_FORMAT_FSCANFMAT "%lf"
#define NUMTOKENS_ERROR -1
/*--------------------------------------------------------------------------*/
static int  Info_size = 0;
static char *Info = NULL;
/*--------------------------------------------------------------------------*/
static int ReadLine(FILE *fd,int *mem);
static BOOL checkFormat(char *fmt);
static BOOL checkLineHaveSeparator(char *line);
static int NumTokens(char *string);
/*--------------------------------------------------------------------------*/
int sci_fscanfMat(char *fname,unsigned long fname_len)
{
    char **Str = NULL;
    int mem = 0;
    double x = 0.;
    int l1 = 0, m1 = 0, n1 = 0, l2 = 0, m2 = 0,n2 = 0;
    int i = 0, j = 0,rows = 0,cols = 0,lres = 0,n = 0;
    int vl = -1;
    FILE  *f = NULL;
    char *Format = NULL;

    char *shortcut_path = NULL;	// filename process
    char *real_path     = NULL; //       "

    if ( Info == NULL )
    {
        if (( Info = (char*)MALLOC(INFOSIZE*sizeof(char))) == NULL)
        {
            Scierror(999,_("%s: No more memory.\n"), fname);
            return 0;
        }
        Info_size = INFOSIZE;
    }

    Nbvars = 0;
    CheckRhs(1, 2); 
    CheckLhs(1, 2);

    if ( Rhs == 2)
    {
        GetRhsVar(2, STRING_DATATYPE, &m2, &n2, &l2); /* format */
        StringConvert(cstk(l2));  /* conversion */
        Format = cstk(l2);
        if (!checkFormat(Format))
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: A valid format expected.\n"), fname, 2);
            return 0;
        }
    }
    else
    {
        Format = DEFAULT_FORMAT_FSCANFMAT;
    }

    // filename process (Replaces SCI, ~, HOME, TMPDIR by the real path)
    // =================================================================

    GetRhsVar(1, STRING_DATATYPE, &m1, &n1, &l1);/* file name */

    /* BUG 3714 */
    shortcut_path = cstk(l1);
    real_path = expandPathVariable(shortcut_path);

#if _MSC_VER
#define MODEFD "rt"
#else
#define MODEFD "r"
#endif

    if (real_path)
    {
        wcfopen(f , real_path, MODEFD);
        FREE(real_path);
        real_path = NULL;
    }

    if ( f  == (FILE *)0)
    {
        Scierror(999,_("%s: Cannot open file '%s'.\n"), fname, shortcut_path);
        return 0;
    }

    /*** first pass to get colums and rows ***/
    strcpy(Info, "--------");
    n = 0;
    while ( (sscanf(Info, Format, &x) <= 0) && (n != EOF) )
    {
        n = ReadLine(f, &mem);
        if ( mem == 1)
        {
            if (Info)
            {
                FREE(Info);
                Info = NULL;
            }
            fclose(f);
            Scierror(999,_("%s: No more memory.\n"), fname);
            return 0;
        }
        vl++;
    }

    if ( n == EOF )
    {
        if (Info)
        {
            FREE(Info);
            Info = NULL;
        }
        fclose(f);
        Scierror(999,_("%s: Cannot read data in file '%s'.\n"), fname, shortcut_path);
        return 0;
    }

    cols = NumTokens(Info);
    if (cols == NUMTOKENS_ERROR)
    {
        if (Info)
        {
            FREE(Info);
            Info = NULL;
        }
        fclose(f);
        Scierror(999,_("%s: Cannot read data in file '%s'.\n"), fname, shortcut_path);
        return 0;
    }
    rows = 1;

    while (1)
    {
        n = ReadLine(f, &mem);

        if ( mem == 1)
        {
            if (Info)
            {
                FREE(Info);
                Info = NULL;
            }
            fclose(f);
            Scierror(999,_("%s: No more memory.\n"), fname);
            return 0;
        }

        if ( sscanf(Info, Format, &x) <= 0 ) break;
        rows++;
        if ( n == EOF ||  n == 0 ) break;
    }

    if ( cols == 0 || rows == 0) rows = cols = 0;

    CreateVar(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &rows, &cols, &lres);

    /** second pass to read data **/
    rewind(f);
    /** skip non numeric lines **/

    if ( Lhs >= 2 && vl != 0 )
    {
        if ((Str = (char**)MALLOC((vl + 1) * sizeof(char *))) == NULL)
        {
            if (Info)
            {
                FREE(Info);
                Info = NULL;
            }
            fclose(f);
            Scierror(999,_("%s: No more memory.\n"),fname);
            return 0;
        }

        Str[vl] = NULL;
    }

    for ( i = 0 ; i < vl ; i++)
    {
        ReadLine(f, &mem);
        if ( mem == 1 )
        {
            if (Info)
            {
                FREE(Info);
                Info = NULL;
            }
            fclose(f);
            freeArrayOfString(Str, i);
            Scierror(999,_("%s: No more memory.\n"), fname);
            return 0;
        }

        if ( Lhs >= 2)
        {
            if ((Str[i]= (char*)MALLOC((strlen(Info) + 1) * sizeof(char))) == NULL)
            {
                if (Info)
                {
                    FREE(Info);
                    Info = NULL;
                }
                fclose(f);
                freeArrayOfString(Str, i);
                Scierror(999,_("%s: No more memory.\n"),fname);
                return 0;
            }

            if (Info) strcpy(Str[i], Info);
        }
    }

    if ( Lhs >= 2)
    {
        int one = 1,zero = 0, l = 0;

        if ( vl > 0 )
        {
            int i2 = 0;
            CreateVarFromPtr(Rhs + 2, MATRIX_OF_STRING_DATATYPE, &vl, &one, Str);
            freeArrayOfString(Str, vl);
        }
        else
        {
            CreateVar(Rhs + 2, STRING_DATATYPE, &zero, &zero, &l);
        }

        LhsVar(2) = Rhs + 2;
    }

    for (i = 0; i < rows ;i++)
    {
        for (j = 0; j < cols;j++)
        {
            double xloc = 0.;
            fscanf(f, Format, &xloc);
            *stk(lres+i+rows*j) = (double)xloc;
        }
    }

    fclose(f);
    LhsVar(1) = Rhs+1;

    PutLhsVar();

    /* just in case Info is too Big */
    if ( Info_size > INFOSIZE )
    {
        Info_size = INFOSIZE;
        Info = (char*)REALLOC(Info, Info_size * sizeof(char));
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
static int ReadLine(FILE *fd,int *mem)
{
    int n = 0;

    while (1)
    {
        char c = (char) fgetc(fd);

        if ( n == Info_size )
        {
            char * Info1 = NULL;
            int New_Size = Info_size + INFOSIZE;

            Info1 = (char*)MALLOC(New_Size * sizeof(char));
            if (Info1 == NULL)
            {
                *mem = 1;
                return EOF;
            }
            else
            {
                memset(Info1, 0, New_Size);
                memcpy(Info1, Info, Info_size);
                Info_size = New_Size;
                FREE(Info);
                Info = Info1;
            }
        }

        Info[n] = c ;
        if (c == '\n') 
        { 
            Info[n] = '\0';
            return 1; 
        }
        else if ( c == (char)EOF ) return EOF;
        n++;
    }
}
/*--------------------------------------------------------------------------*/
static BOOL checkFormat(char *fmt)
{
    /* a very basic check for format */
    if (fmt)
    {
        if (fmt[0] == '%') 
        {
            int len = (int)strlen(fmt);
            if (len > 1)
            {
                int i = 0;
                if (fmt[1] == '%') return FALSE;
                for (i = 2; i < len ; i++)
                {
                    if (fmt[i] == '%') return FALSE;
                }
            }
            return TRUE;
        }
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
static int NumTokens(char *string)
{
    if (string)
    {
        int n = 1;
        int lnchar = 0;
        int ntok   = NUMTOKENS_ERROR;
        int length = (int)strlen(string) + 1;

        if (string != 0)
        {
            /** Counting leading white spaces **/
            int r = sscanf(string, "%*[ \r\t\n]%n", &lnchar);
            if (!checkLineHaveSeparator(string))
            {
                return ntok;
            }
        }

        while ( n != 0 && n != EOF && lnchar <= length  )
        {
            char buf[128];
            int nchar1 = 0;
            int nchar2 = 0;
            char *strTmp = NULL;

            if (lnchar >= length)
            {
                return(ntok);
            }
            else
            {
                strTmp = strdup(&string[lnchar]);
            }

            ntok++;

            if (!checkLineHaveSeparator(strTmp))
            {
                if (strTmp)
                {
                    if (strlen(strTmp) > 0)
                    {
                        ntok++;
                    }

                    FREE(strTmp);
                    strTmp = NULL;
                }
                return ntok;
            }

            n = sscanf(strTmp, "%[^ \r\t\n]%n%*[ \r\t\n]%n", buf, &nchar1, &nchar2);
            lnchar += (nchar2 <= nchar1) ? nchar1 : nchar2;

            if (strTmp)
            {
                FREE(strTmp);
                strTmp = NULL;
            }
        }
        if (ntok) return(ntok);
    }
    return NUMTOKENS_ERROR;
}
/*--------------------------------------------------------------------------*/
static BOOL checkLineHaveSeparator(char *line)
{
#define NUMBER_SEPARATOR_TYPE_1 ' '
#define NUMBER_SEPARATOR_TYPE_2 '\r'
#define NUMBER_SEPARATOR_TYPE_3 '\t'
#define NUMBER_SEPARATOR_TYPE_4 '\n'

    if ((strchr(line, NUMBER_SEPARATOR_TYPE_1) == NULL) &&
        (strchr(line, NUMBER_SEPARATOR_TYPE_2) == NULL) &&
        (strchr(line, NUMBER_SEPARATOR_TYPE_3) == NULL) &&
        (strchr(line, NUMBER_SEPARATOR_TYPE_4) == NULL))
    {
        return FALSE;
    }
    return TRUE;
}
/*--------------------------------------------------------------------------*/
