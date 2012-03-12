/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Sylvestre LEdru
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#if !defined(MATIO_VERSION)
#define mat_complex_split_t ComplexSplit
#define mat_sparse_t sparse_t
#define MAT_F_DONT_COPY_DATA MEM_CONSERVE
#define MAT_BY_INDEX BY_INDEX
#endif

#define MATIO_ERROR if(_SciErr.iErr) \
    {				     \
      printError(&_SciErr, 0);	     \
      return 0;			     \
    }
