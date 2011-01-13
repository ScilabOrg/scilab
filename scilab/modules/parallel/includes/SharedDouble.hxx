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

#include "double.hxx"
#include "parallel_utility.hxx"

#define CONST

/*
 * The shared memory buffer used to hold the data for a  Double Scilab variable.
 */
namespace parallel
{
    struct SharedDouble: SharedData
    {
        // we have to alloc as for a complex var to be sure
        SharedDouble(types::Double CONST& d):rows(d.rows_get()), cols(d.cols_get()), real(allocSharedMem<double>(rows*cols)), imag(allocSharedMem<double>(rows*cols)), cplx(false)
        {
        }

        void readSlice(std::size_t c, types::InternalType CONST& i)
        {
            types::Double& d(dynamic_cast<types::Double&>(i));
            std::size_t const colSize(sizeof(double));// /!\ check
            std::memcpy(real+ c , d.real_get()+ c, colSize);
            if(d.isComplex())
            {
                cplx= true;
                std::memcpy(imag+ c, d.img_get()+ c , colSize);
            }
        }

        void write(types::InternalType & i)
        {
            types::Double& d(dynamic_cast<types::Double&>(i));
            d.complex_set(cplx);
#ifdef TRACE
            std::cerr<<"writing a types::Double with real values of ";
            std::copy(real, real+rows* cols, std::ostream_iterator<double>(std::cerr," "));
#endif
            std::memcpy(d.real_get(), real, rows * cols * sizeof(double));
            if(cplx)
            {
#ifdef TRACE
                std::cerr<<"\n and imag values of ";
                std::copy(real, real+rows* cols, std::ostream_iterator<double>(std::cerr," "));
                std::cerr<<std::endl;
#endif
                std::memcpy(d.img_get(), imag, rows * cols * sizeof(double));
            }
            std::cerr<<std::endl;
        }

        ~SharedDouble()
        {
            freeSharedMem(real, rows * cols * sizeof(double));
            freeSharedMem(imag, rows * cols * sizeof(double));
        }
        int rows, cols;
        double* real;
        double* imag;
        bool cplx;
    };
}
