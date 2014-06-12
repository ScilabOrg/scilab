/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#ifndef __IS_TRUE_HXX__
#define __IS_TRUE_HXX__

namespace types
{

struct is_true
{

    template<typename T>
    inline static bool isTrue(const int size, const T * const data)
    {
        for (int i = 0; i < size; i++)
        {
            if (data[i] == 0)
            {
                return false;
            }
        }

        return true;
    }
};

} // namespace types

#endif // __IS_TRUE_HXX__
