/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

// This code is separated in colon.hxx
// but will be inlined in arrayof.hxx
//
// If you need additionnal headers, please add it in arrayof.hxx

#ifndef __ARRAYOF_HXX__
    #error This file must only be include by arrayof.hxx
#endif

#include "dollar.hxx"

namespace types
{
    class Colon : public ImplicitList
    {
    public :

        Colon()
        {
            m_iSize = -1;
            m_eOutSubType = Int::Type8;
            m_eOutType = RealGeneric;
            start_set(new Double(1));
            step_set(new Double(1));
            end_set(new Dollar());
            compute();
        }

        Colon* clone()
        {
            return new Colon();
        }
    };
}

