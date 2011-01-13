/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Bernard HUGUENEY
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "SharedData.hxx"
#include "SharedDouble.hxx"

namespace parallel
{
/*
 * Factory member function used to create the buffer s in shared memory
 * @param v ptr to a Scilab variable
 * @return ptr to a newly allocated shared memory buffer of the correct SharedData subtype.
 */
    SharedData* SharedData::create(types::InternalType* v)
    {
        switch(v->getType())
        {
        case types::InternalType::RealDouble : return new SharedDouble(*dynamic_cast<types::Double*>(v));
        default: {std::wcerr<<L"unhandled sperarate memory model for result of type "<<v->getTypeStr()<<L"in parallel_run";return 0;}
        }
        return 0;
    }
}
