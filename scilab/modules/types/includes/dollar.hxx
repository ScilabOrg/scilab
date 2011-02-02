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

#ifndef __DOLLAR_HXX__
#define __DOLLAR_HXX__

#include "matrixpoly.hxx"

namespace types
{
    class Dollar : public MatrixPoly
    {
    public :

        Dollar()
        {
            m_iRows     = 1;
            m_iCols     = 1;
            m_iSize     = 1;
            m_szVarName = L"$";
            m_bComplex  = false;

            m_poPolyMatrix = new Poly[m_iSize];
            
            Double dblCoef(1,2);
            dblCoef.set_real(0, 0, 0);
            dblCoef.set_real(0, 1, 1);

            m_poPolyMatrix[0].CreatePoly(NULL, NULL, 2);
            Poly *poPoly = poly_get(0,0);
            poPoly->coef_set(&dblCoef);
        }

        Dollar* clone()
        {
            return new Dollar();
        }
    };
}
#endif /* !__DOLLAR_HXX__ */
