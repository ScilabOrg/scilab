/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - 2005 - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#ifndef __SCI_MEM_ALLOC__
#define __SCI_MEM_ALLOC__

#undef Top

#ifdef _MSC_VER
#error This file should not be used on windows
#else
#include <stdlib.h>
#include <stddef.h> /* for size_t */
#endif

#ifdef MALLOC
#undef MALLOC
#endif

#ifdef FREE
#undef FREE
#endif

#ifdef CALLOC
#undef CALLOC
#endif

#ifdef REALLOC
#undef REALLOC
#endif

#ifdef NULL
#undef NULL
#endif
#define NULL 0

#define MALLOC(x) malloc((size_t)x)
#define FREE(x) free((void *) x)
#define REALLOC(x,y) realloc((void *)x, y)
#define CALLOC(x,y) calloc((size_t) x, (size_t) y)

/* Only for Scilab Stacksize use VirtualAlloc on Window */
#define SCISTACKMALLOC(x) malloc((unsigned long)x)
#define SCISTACKFREE(x) free((void *) x)

#define Top C2F(vstk).top

#endif /* __SCI_MEM_ALLOC__ */
