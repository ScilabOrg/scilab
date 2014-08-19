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

#include "types_multiplication.hxx"
#include "types_addition.hxx"
#include "double.hxx"
#include "int.hxx"
#include "sparse.hxx"
#include "polynom.hxx"

#include "scilabexception.hxx"

extern "C"
{
#include "matrix_multiplication.h"
#include "matrix_addition.h"
#include "operation_f.h"
#include "localization.h"
#include "charEncoding.h"
#include "elem_common.h"
}


using namespace types;

//define arrays on operation functions
static dotmul_function pDotMulfunction[types::InternalType::IdLast][types::InternalType::IdLast] = {NULL};

void fillDotMulFunction()
{
#define scilab_fill_dotmul(id1, id2, func, typeIn1, typeIn2, typeOut) \
    pDotMulfunction[types::InternalType::Id ## id1][types::InternalType::Id ## id2] = (dotmul_function)&dotmul_##func<typeIn1, typeIn2, typeOut>

    //Double
    //Matrix .* Matrix
    scilab_fill_dotmul(Double, Double, M_M, Double, Double, Double);
    scilab_fill_dotmul(Double, Int8, M_M, Double, Int8, Int8);
    scilab_fill_dotmul(Double, UInt8, M_M, Double, UInt8, UInt8);
    scilab_fill_dotmul(Double, Int16, M_M, Double, Int16, Int16);
    scilab_fill_dotmul(Double, UInt16, M_M, Double, UInt16, UInt16);
    scilab_fill_dotmul(Double, Int32, M_M, Double, Int32, Int32);
    scilab_fill_dotmul(Double, UInt32, M_M, Double, UInt32, UInt32);
    scilab_fill_dotmul(Double, Int64, M_M, Double, Int64, Int64);
    scilab_fill_dotmul(Double, UInt64, M_M, Double, UInt64, UInt64);
    scilab_fill_dotmul(Double, Bool, M_M, Double, Bool, Double);
    scilab_fill_dotmul(Double, Polynom, M_M, Double, Polynom, Polynom);
    scilab_fill_dotmul(Double, Sparse, M_M, Double, Sparse, Sparse);

    //Matrix .* Matrix Complex
    scilab_fill_dotmul(Double, DoubleComplex, M_MC, Double, Double, Double);
    scilab_fill_dotmul(Double, PolynomComplex, M_M, Double, Polynom, Polynom);
    scilab_fill_dotmul(Double, SparseComplex, M_M, Double, Sparse, Sparse);

    //Matrix .* Scalar
    scilab_fill_dotmul(Double, ScalarDouble, M_S, Double, Double, Double);
    scilab_fill_dotmul(Double, ScalarInt8, M_S, Double, Int8, Int8);
    scilab_fill_dotmul(Double, ScalarUInt8, M_S, Double, UInt8, UInt8);
    scilab_fill_dotmul(Double, ScalarInt16, M_S, Double, Int16, Int16);
    scilab_fill_dotmul(Double, ScalarUInt16, M_S, Double, UInt16, UInt16);
    scilab_fill_dotmul(Double, ScalarInt32, M_S, Double, Int32, Int32);
    scilab_fill_dotmul(Double, ScalarUInt32, M_S, Double, UInt32, UInt32);
    scilab_fill_dotmul(Double, ScalarInt64, M_S, Double, Int64, Int64);
    scilab_fill_dotmul(Double, ScalarUInt64, M_S, Double, UInt64, UInt64);
    scilab_fill_dotmul(Double, ScalarBool, M_S, Double, Bool, Double);
    scilab_fill_dotmul(Double, ScalarPolynom, M_M, Double, Polynom, Polynom);

    //Matrix .* Scalar Complex
    scilab_fill_dotmul(Double, ScalarDoubleComplex, M_SC, Double, Double, Double);
    scilab_fill_dotmul(Double, ScalarPolynomComplex, M_M, Double, Polynom, Polynom);
    //Matrix .* Empty
    scilab_fill_dotmul(Double, Empty, M_E, Double, Double, Double);


    //Matrix Complex .* Matrix
    scilab_fill_dotmul(DoubleComplex, Double, MC_M, Double, Double, Double);
    scilab_fill_dotmul(DoubleComplex, DoubleComplex, MC_MC, Double, Double, Double);
    scilab_fill_dotmul(DoubleComplex, ScalarDouble, MC_S, Double, Double, Double);
    scilab_fill_dotmul(DoubleComplex, ScalarDoubleComplex, MC_SC, Double, Double, Double);
    scilab_fill_dotmul(DoubleComplex, Empty, M_E, Double, Double, Double);
    scilab_fill_dotmul(DoubleComplex, Polynom, M_M, Double, Polynom, Polynom);
    scilab_fill_dotmul(DoubleComplex, PolynomComplex, M_M, Double, Polynom, Polynom);
    scilab_fill_dotmul(DoubleComplex, ScalarPolynom, M_M, Double, Polynom, Polynom);
    scilab_fill_dotmul(DoubleComplex, ScalarPolynomComplex, M_M, Double, Polynom, Polynom);
    scilab_fill_dotmul(DoubleComplex, Sparse, M_M, Double, Sparse, Sparse);
    scilab_fill_dotmul(DoubleComplex, SparseComplex, M_M, Double, Sparse, Sparse);

    //Scalar .* Matrix
    scilab_fill_dotmul(ScalarDouble, Double, S_M, Double, Double, Double);
    scilab_fill_dotmul(ScalarDouble, Int8, S_M, Double, Int8, Int8);
    scilab_fill_dotmul(ScalarDouble, UInt8, S_M, Double, UInt8, UInt8);
    scilab_fill_dotmul(ScalarDouble, Int16, S_M, Double, Int16, Int16);
    scilab_fill_dotmul(ScalarDouble, UInt16, S_M, Double, UInt16, UInt16);
    scilab_fill_dotmul(ScalarDouble, Int32, S_M, Double, Int32, Int32);
    scilab_fill_dotmul(ScalarDouble, UInt32, S_M, Double, UInt32, UInt32);
    scilab_fill_dotmul(ScalarDouble, Int64, S_M, Double, Int64, Int64);
    scilab_fill_dotmul(ScalarDouble, UInt64, S_M, Double, UInt64, UInt64);
    scilab_fill_dotmul(ScalarDouble, Bool, S_M, Double, Bool, Double);
    scilab_fill_dotmul(ScalarDouble, Polynom, M_M, Double, Polynom, Polynom);
    scilab_fill_dotmul(ScalarDouble, Sparse, M_M, Double, Sparse, Sparse);

    //Scalar .* Matrix Complex
    scilab_fill_dotmul(ScalarDouble, DoubleComplex, S_MC, Double, Double, Double);
    scilab_fill_dotmul(ScalarDouble, PolynomComplex, M_M, Double, Polynom, Polynom);
    scilab_fill_dotmul(ScalarDouble, SparseComplex, M_M, Double, Sparse, Sparse);

    //Scalar .* Scalar
    scilab_fill_dotmul(ScalarDouble, ScalarDouble, S_S, Double, Double, Double);
    scilab_fill_dotmul(ScalarDouble, ScalarInt8, S_S, Double, Int8, Int8);
    scilab_fill_dotmul(ScalarDouble, ScalarUInt8, S_S, Double, UInt8, UInt8);
    scilab_fill_dotmul(ScalarDouble, ScalarInt16, S_S, Double, Int16, Int16);
    scilab_fill_dotmul(ScalarDouble, ScalarUInt16, S_S, Double, UInt16, UInt16);
    scilab_fill_dotmul(ScalarDouble, ScalarInt32, S_S, Double, Int32, Int32);
    scilab_fill_dotmul(ScalarDouble, ScalarUInt32, S_S, Double, UInt32, UInt32);
    scilab_fill_dotmul(ScalarDouble, ScalarInt64, S_S, Double, Int64, Int64);
    scilab_fill_dotmul(ScalarDouble, ScalarUInt64, S_S, Double, UInt64, UInt64);
    scilab_fill_dotmul(ScalarDouble, ScalarBool, S_S, Double, Bool, Double);
    scilab_fill_dotmul(ScalarDouble, ScalarPolynom, M_M, Double, Polynom, Polynom);

    //Scalar .* Scalar Complex
    scilab_fill_dotmul(ScalarDouble, ScalarDoubleComplex, S_SC, Double, Double, Double);
    scilab_fill_dotmul(ScalarDouble, PolynomComplex, M_M, Double, Polynom, Polynom);
    scilab_fill_dotmul(ScalarDouble, ScalarPolynomComplex, M_M, Double, Polynom, Polynom);

    //Scalar .* Empty
    scilab_fill_dotmul(ScalarDouble, Empty, M_E, Double, Double, Double);

    //Scalar Complex .* Matrix
    scilab_fill_dotmul(ScalarDoubleComplex, Double, SC_M, Double, Double, Double);
    scilab_fill_dotmul(ScalarDoubleComplex, Polynom, M_M, Double, Polynom, Polynom);
    scilab_fill_dotmul(ScalarDoubleComplex, Sparse, M_M, Double, Sparse, Sparse);
    //Scalar Complex .* Matrix Complex
    scilab_fill_dotmul(ScalarDoubleComplex, DoubleComplex, SC_MC, Double, Double, Double);
    scilab_fill_dotmul(ScalarDoubleComplex, PolynomComplex, M_M, Double, Polynom, Polynom);
    scilab_fill_dotmul(ScalarDoubleComplex, SparseComplex, M_M, Double, Sparse, Sparse);
    //Scalar Complex .* Scalar
    scilab_fill_dotmul(ScalarDoubleComplex, ScalarDouble, SC_S, Double, Double, Double);
    scilab_fill_dotmul(ScalarDoubleComplex, ScalarPolynom, M_M, Double, Polynom, Polynom);
    //Scalar Complex .* Scalar Complex
    scilab_fill_dotmul(ScalarDoubleComplex, ScalarDoubleComplex, SC_SC, Double, Double, Double);
    scilab_fill_dotmul(ScalarDoubleComplex, ScalarPolynomComplex, M_M, Double, Polynom, Polynom);
    //Scalar Complex .* Empty
    scilab_fill_dotmul(ScalarDoubleComplex, Empty, M_E, Double, Double, Double);

    //Empty .* Matrix
    scilab_fill_dotmul(Empty, Double, E_M, Double, Double, Double);
    scilab_fill_dotmul(Empty, Int8, E_M, Double, Int8, Double);
    scilab_fill_dotmul(Empty, UInt8, E_M, Double, UInt8, Double);
    scilab_fill_dotmul(Empty, Int16, E_M, Double, Int16, Double);
    scilab_fill_dotmul(Empty, UInt16, E_M, Double, UInt16, Double);
    scilab_fill_dotmul(Empty, Int32, E_M, Double, Int32, Double);
    scilab_fill_dotmul(Empty, UInt32, E_M, Double, UInt32, Double);
    scilab_fill_dotmul(Empty, Int64, E_M, Double, Int64, Double);
    scilab_fill_dotmul(Empty, UInt64, E_M, Double, UInt64, Double);
    scilab_fill_dotmul(Empty, Polynom, E_M, Double, Polynom, Double);
    scilab_fill_dotmul(Empty, PolynomComplex, E_M, Double, Polynom, Double);
    scilab_fill_dotmul(Empty, Sparse, E_M, Double, Sparse, Double);
    scilab_fill_dotmul(Empty, SparseComplex, E_M, Double, Sparse, Double);

    //Empty .* Matrix Complex
    scilab_fill_dotmul(Empty, DoubleComplex, E_M, Double, Double, Double);
    //Empty .* Scalar
    scilab_fill_dotmul(Empty, ScalarDouble, E_M, Double, Double, Double);
    scilab_fill_dotmul(Empty, ScalarInt8, E_M, Double, Int8, Double);
    scilab_fill_dotmul(Empty, ScalarUInt8, E_M, Double, UInt8, Double);
    scilab_fill_dotmul(Empty, ScalarInt16, E_M, Double, Int16, Double);
    scilab_fill_dotmul(Empty, ScalarUInt16, E_M, Double, UInt16, Double);
    scilab_fill_dotmul(Empty, ScalarInt32, E_M, Double, Int32, Double);
    scilab_fill_dotmul(Empty, ScalarUInt32, E_M, Double, UInt32, Double);
    scilab_fill_dotmul(Empty, ScalarInt64, E_M, Double, Int64, Double);
    scilab_fill_dotmul(Empty, ScalarUInt64, E_M, Double, UInt64, Double);
    scilab_fill_dotmul(Empty, ScalarBool, E_M, Double, Bool, Double);
    scilab_fill_dotmul(Empty, ScalarPolynom, E_M, Double, Polynom, Double);

    //Empty .* Scalar Complex
    scilab_fill_dotmul(Empty, ScalarDoubleComplex, E_M, Double, Double, Double);
    scilab_fill_dotmul(Empty, ScalarPolynomComplex, E_M, Double, Polynom, Double);
    //Empty .* Empty
    scilab_fill_dotmul(Empty, Empty, E_M, Double, Double, Double);
    //Empty .* eye
    scilab_fill_dotmul(Empty, Identity, E_M, Double, Double, Double);
    scilab_fill_dotmul(Empty, IdentityComplex, E_M, Double, Double, Double);

    //Matrix .* Identity
    scilab_fill_dotmul(Double, Identity, M_I, Double, Double, Double);
    scilab_fill_dotmul(Double, IdentityComplex, M_IC, Double, Double, Double);
    scilab_fill_dotmul(DoubleComplex, Identity, MC_I, Double, Double, Double);
    scilab_fill_dotmul(DoubleComplex, IdentityComplex, MC_IC, Double, Double, Double);
    scilab_fill_dotmul(ScalarDouble, Identity, S_I, Double, Double, Double);
    scilab_fill_dotmul(ScalarDouble, IdentityComplex, S_IC, Double, Double, Double);
    scilab_fill_dotmul(ScalarDoubleComplex, Identity, SC_I, Double, Double, Double);
    scilab_fill_dotmul(ScalarDoubleComplex, IdentityComplex, SC_IC, Double, Double, Double);

    //Int8
    //Matrix .* Matrix
    scilab_fill_dotmul(Int8, Double, M_M, Int8, Double, Int8);
    scilab_fill_dotmul(Int8, Int8, M_M, Int8, Int8, Int8);
    scilab_fill_dotmul(Int8, UInt8, M_M, Int8, UInt8, UInt8);
    scilab_fill_dotmul(Int8, Int16, M_M, Int8, Int16, Int16);
    scilab_fill_dotmul(Int8, UInt16, M_M, Int8, UInt16, UInt16);
    scilab_fill_dotmul(Int8, Int32, M_M, Int8, Int32, Int32);
    scilab_fill_dotmul(Int8, UInt32, M_M, Int8, UInt32, UInt32);
    scilab_fill_dotmul(Int8, Int64, M_M, Int8, Int64, Int64);
    scilab_fill_dotmul(Int8, UInt64, M_M, Int8, UInt64, UInt64);
    scilab_fill_dotmul(Int8, Bool, M_M, Int8, Bool, Int8);
    scilab_fill_dotmul(Int8, Empty, M_E, Int8, Double, Double);

    //Matrix .* Scalar
    scilab_fill_dotmul(Int8, ScalarDouble, M_S, Int8, Double, Int8);
    scilab_fill_dotmul(Int8, ScalarInt8, M_S, Int8, Int8, Int8);
    scilab_fill_dotmul(Int8, ScalarUInt8, M_S, Int8, UInt8, UInt8);
    scilab_fill_dotmul(Int8, ScalarInt16, M_S, Int8, Int16, Int16);
    scilab_fill_dotmul(Int8, ScalarUInt16, M_S, Int8, UInt16, UInt16);
    scilab_fill_dotmul(Int8, ScalarInt32, M_S, Int8, Int32, Int32);
    scilab_fill_dotmul(Int8, ScalarUInt32, M_S, Int8, UInt32, UInt32);
    scilab_fill_dotmul(Int8, ScalarInt64, M_S, Int8, Int64, Int64);
    scilab_fill_dotmul(Int8, ScalarUInt64, M_S, Int8, UInt64, UInt64);
    scilab_fill_dotmul(Int8, ScalarBool, M_S, Int8, Bool, Int8);

    //Scalar .* Matrix
    scilab_fill_dotmul(ScalarInt8, Double, S_M, Int8, Double, Int8);
    scilab_fill_dotmul(ScalarInt8, Int8, S_M, Int8, Int8, Int8);
    scilab_fill_dotmul(ScalarInt8, UInt8, S_M, Int8, UInt8, UInt8);
    scilab_fill_dotmul(ScalarInt8, Int16, S_M, Int8, Int16, Int16);
    scilab_fill_dotmul(ScalarInt8, UInt16, S_M, Int8, UInt16, UInt16);
    scilab_fill_dotmul(ScalarInt8, Int32, S_M, Int8, Int32, Int32);
    scilab_fill_dotmul(ScalarInt8, UInt32, S_M, Int8, UInt32, UInt32);
    scilab_fill_dotmul(ScalarInt8, Int64, S_M, Int8, Int64, Int64);
    scilab_fill_dotmul(ScalarInt8, UInt64, S_M, Int8, UInt64, UInt64);
    scilab_fill_dotmul(ScalarInt8, Bool, S_M, Int8, Bool, Int8);
    scilab_fill_dotmul(ScalarInt8, Empty, M_E, Int8, Double, Double);

    //Scalar .* Scalar
    scilab_fill_dotmul(ScalarInt8, ScalarDouble, S_S, Int8, Double, Int8);
    scilab_fill_dotmul(ScalarInt8, ScalarInt8, S_S, Int8, Int8, Int8);
    scilab_fill_dotmul(ScalarInt8, ScalarUInt8, S_S, Int8, UInt8, UInt8);
    scilab_fill_dotmul(ScalarInt8, ScalarInt16, S_S, Int8, Int16, Int16);
    scilab_fill_dotmul(ScalarInt8, ScalarUInt16, S_S, Int8, UInt16, UInt16);
    scilab_fill_dotmul(ScalarInt8, ScalarInt32, S_S, Int8, Int32, Int32);
    scilab_fill_dotmul(ScalarInt8, ScalarUInt32, S_S, Int8, UInt32, UInt32);
    scilab_fill_dotmul(ScalarInt8, ScalarInt64, S_S, Int8, Int64, Int64);
    scilab_fill_dotmul(ScalarInt8, ScalarUInt64, S_S, Int8, UInt64, UInt64);
    scilab_fill_dotmul(ScalarInt8, ScalarBool, S_S, Int8, Bool, Int8);

    //UInt8
    //Matrix .* Matrix
    scilab_fill_dotmul(UInt8, Double, M_M, UInt8, Double, UInt8);
    scilab_fill_dotmul(UInt8, Int8, M_M, UInt8, Int8, UInt8);
    scilab_fill_dotmul(UInt8, UInt8, M_M, UInt8, UInt8, UInt8);
    scilab_fill_dotmul(UInt8, Int16, M_M, UInt8, Int16, UInt16);
    scilab_fill_dotmul(UInt8, UInt16, M_M, UInt8, UInt16, UInt16);
    scilab_fill_dotmul(UInt8, Int32, M_M, UInt8, Int32, UInt32);
    scilab_fill_dotmul(UInt8, UInt32, M_M, UInt8, UInt32, UInt32);
    scilab_fill_dotmul(UInt8, Int64, M_M, UInt8, Int64, UInt64);
    scilab_fill_dotmul(UInt8, UInt64, M_M, UInt8, UInt64, UInt64);
    scilab_fill_dotmul(UInt8, Bool, M_M, UInt8, Bool, UInt8);
    scilab_fill_dotmul(UInt8, Empty, M_E, UInt8, Double, Double);

    //Matrix .* Scalar
    scilab_fill_dotmul(UInt8, ScalarDouble, M_S, UInt8, Double, UInt8);
    scilab_fill_dotmul(UInt8, ScalarInt8, M_S, UInt8, Int8, UInt8);
    scilab_fill_dotmul(UInt8, ScalarUInt8, M_S, UInt8, UInt8, UInt8);
    scilab_fill_dotmul(UInt8, ScalarInt16, M_S, UInt8, Int16, UInt16);
    scilab_fill_dotmul(UInt8, ScalarUInt16, M_S, UInt8, UInt16, UInt16);
    scilab_fill_dotmul(UInt8, ScalarInt32, M_S, UInt8, Int32, UInt32);
    scilab_fill_dotmul(UInt8, ScalarUInt32, M_S, UInt8, UInt32, UInt32);
    scilab_fill_dotmul(UInt8, ScalarInt64, M_S, UInt8, Int64, UInt64);
    scilab_fill_dotmul(UInt8, ScalarUInt64, M_S, UInt8, UInt64, UInt64);
    scilab_fill_dotmul(UInt8, ScalarBool, M_S, UInt8, Bool, UInt8);

    //Scalar .* Matrix
    scilab_fill_dotmul(ScalarUInt8, Double, S_M, UInt8, Double, UInt8);
    scilab_fill_dotmul(ScalarUInt8, Int8, S_M, UInt8, Int8, UInt8);
    scilab_fill_dotmul(ScalarUInt8, UInt8, S_M, UInt8, UInt8, UInt8);
    scilab_fill_dotmul(ScalarUInt8, Int16, S_M, UInt8, Int16, UInt16);
    scilab_fill_dotmul(ScalarUInt8, UInt16, S_M, UInt8, UInt16, UInt16);
    scilab_fill_dotmul(ScalarUInt8, Int32, S_M, UInt8, Int32, UInt32);
    scilab_fill_dotmul(ScalarUInt8, UInt32, S_M, UInt8, UInt32, UInt32);
    scilab_fill_dotmul(ScalarUInt8, Int64, S_M, UInt8, Int64, UInt64);
    scilab_fill_dotmul(ScalarUInt8, UInt64, S_M, UInt8, UInt64, UInt64);
    scilab_fill_dotmul(ScalarUInt8, Bool, S_M, UInt8, Bool, UInt8);
    scilab_fill_dotmul(ScalarUInt8, Empty, M_E, UInt8, Double, Double);

    //Scalar .* Scalar
    scilab_fill_dotmul(ScalarUInt8, ScalarDouble, S_S, UInt8, Double, UInt8);
    scilab_fill_dotmul(ScalarUInt8, ScalarInt8, S_S, UInt8, Int8, UInt8);
    scilab_fill_dotmul(ScalarUInt8, ScalarUInt8, S_S, UInt8, UInt8, UInt8);
    scilab_fill_dotmul(ScalarUInt8, ScalarInt16, S_S, UInt8, Int16, UInt16);
    scilab_fill_dotmul(ScalarUInt8, ScalarUInt16, S_S, UInt8, UInt16, UInt16);
    scilab_fill_dotmul(ScalarUInt8, ScalarInt32, S_S, UInt8, Int32, UInt32);
    scilab_fill_dotmul(ScalarUInt8, ScalarUInt32, S_S, UInt8, UInt32, UInt32);
    scilab_fill_dotmul(ScalarUInt8, ScalarInt64, S_S, UInt8, Int64, UInt64);
    scilab_fill_dotmul(ScalarUInt8, ScalarUInt64, S_S, UInt8, UInt64, UInt64);
    scilab_fill_dotmul(ScalarUInt8, ScalarBool, S_S, UInt8, Bool, UInt8);

    //Int16
    //Matrix .* Matrix
    scilab_fill_dotmul(Int16, Double, M_M, Int16, Double, Int16);
    scilab_fill_dotmul(Int16, Int8, M_M, Int16, Int8, Int16);
    scilab_fill_dotmul(Int16, UInt8, M_M, Int16, UInt8, UInt16);
    scilab_fill_dotmul(Int16, Int16, M_M, Int16, Int16, Int16);
    scilab_fill_dotmul(Int16, UInt16, M_M, Int16, UInt16, UInt16);
    scilab_fill_dotmul(Int16, Int32, M_M, Int16, Int32, Int32);
    scilab_fill_dotmul(Int16, UInt32, M_M, Int16, UInt32, UInt32);
    scilab_fill_dotmul(Int16, Int64, M_M, Int16, Int64, Int64);
    scilab_fill_dotmul(Int16, UInt64, M_M, Int16, UInt64, UInt64);
    scilab_fill_dotmul(Int16, Bool, M_M, Int16, Bool, Int16);
    scilab_fill_dotmul(Int16, Empty, M_E, Int16, Double, Double);

    //Matrix .* Scalar
    scilab_fill_dotmul(Int16, ScalarDouble, M_S, Int16, Double, Int16);
    scilab_fill_dotmul(Int16, ScalarInt8, M_S, Int16, Int8, Int16);
    scilab_fill_dotmul(Int16, ScalarUInt8, M_S, Int16, UInt8, UInt16);
    scilab_fill_dotmul(Int16, ScalarInt16, M_S, Int16, Int16, Int16);
    scilab_fill_dotmul(Int16, ScalarUInt16, M_S, Int16, UInt16, UInt16);
    scilab_fill_dotmul(Int16, ScalarInt32, M_S, Int16, Int32, Int32);
    scilab_fill_dotmul(Int16, ScalarUInt32, M_S, Int16, UInt32, UInt32);
    scilab_fill_dotmul(Int16, ScalarInt64, M_S, Int16, Int64, Int64);
    scilab_fill_dotmul(Int16, ScalarUInt64, M_S, Int16, UInt64, UInt64);
    scilab_fill_dotmul(Int16, ScalarBool, M_E, Int16, Bool, Int16);

    //Scalar .* Matrix
    scilab_fill_dotmul(ScalarInt16, Double, S_M, Int16, Double, Int16);
    scilab_fill_dotmul(ScalarInt16, Int8, S_M, Int16, Int8, Int16);
    scilab_fill_dotmul(ScalarInt16, UInt8, S_M, Int16, UInt8, UInt16);
    scilab_fill_dotmul(ScalarInt16, Int16, S_M, Int16, Int16, Int16);
    scilab_fill_dotmul(ScalarInt16, UInt16, S_M, Int16, UInt16, UInt16);
    scilab_fill_dotmul(ScalarInt16, Int32, S_M, Int16, Int32, Int32);
    scilab_fill_dotmul(ScalarInt16, UInt32, S_M, Int16, UInt32, UInt32);
    scilab_fill_dotmul(ScalarInt16, Int64, S_M, Int16, Int64, Int64);
    scilab_fill_dotmul(ScalarInt16, UInt64, S_M, Int16, UInt64, UInt64);
    scilab_fill_dotmul(ScalarInt16, Bool, S_M, Int16, Bool, Int16);
    scilab_fill_dotmul(ScalarInt16, Empty, M_E, Int16, Double, Double);

    //Scalar .* Scalar
    scilab_fill_dotmul(ScalarInt16, ScalarDouble, S_S, Int16, Double, Int16);
    scilab_fill_dotmul(ScalarInt16, ScalarInt8, S_S, Int16, Int8, Int16);
    scilab_fill_dotmul(ScalarInt16, ScalarUInt8, S_S, Int16, UInt8, UInt16);
    scilab_fill_dotmul(ScalarInt16, ScalarInt16, S_S, Int16, Int16, Int16);
    scilab_fill_dotmul(ScalarInt16, ScalarUInt16, S_S, Int16, UInt16, UInt16);
    scilab_fill_dotmul(ScalarInt16, ScalarInt32, S_S, Int16, Int32, Int32);
    scilab_fill_dotmul(ScalarInt16, ScalarUInt32, S_S, Int16, UInt32, UInt32);
    scilab_fill_dotmul(ScalarInt16, ScalarInt64, S_S, Int16, Int64, Int64);
    scilab_fill_dotmul(ScalarInt16, ScalarUInt64, S_S, Int16, UInt64, UInt64);
    scilab_fill_dotmul(ScalarInt16, ScalarBool, S_S, Int16, Bool, Int16);

    //UInt16
    //Matrix .* Matrix
    scilab_fill_dotmul(UInt16, Double, M_M, UInt16, Double, UInt16);
    scilab_fill_dotmul(UInt16, Int8, M_M, UInt16, Int8, UInt16);
    scilab_fill_dotmul(UInt16, UInt8, M_M, UInt16, UInt8, UInt16);
    scilab_fill_dotmul(UInt16, Int16, M_M, UInt16, Int16, UInt16);
    scilab_fill_dotmul(UInt16, UInt16, M_M, UInt16, UInt16, UInt16);
    scilab_fill_dotmul(UInt16, Int32, M_M, UInt16, Int32, UInt32);
    scilab_fill_dotmul(UInt16, UInt32, M_M, UInt16, UInt32, UInt32);
    scilab_fill_dotmul(UInt16, Int64, M_M, UInt16, Int64, UInt64);
    scilab_fill_dotmul(UInt16, UInt64, M_M, UInt16, UInt64, UInt64);
    scilab_fill_dotmul(UInt16, Bool, M_M, UInt16, Bool, UInt16);
    scilab_fill_dotmul(UInt16, Empty, M_E, UInt16, Double, Double);

    //Matrix .* Scalar
    scilab_fill_dotmul(UInt16, ScalarDouble, M_S, UInt16, Double, UInt16);
    scilab_fill_dotmul(UInt16, ScalarInt8, M_S, UInt16, Int8, UInt16);
    scilab_fill_dotmul(UInt16, ScalarUInt8, M_S, UInt16, UInt8, UInt16);
    scilab_fill_dotmul(UInt16, ScalarInt16, M_S, UInt16, Int16, UInt16);
    scilab_fill_dotmul(UInt16, ScalarUInt16, M_S, UInt16, UInt16, UInt16);
    scilab_fill_dotmul(UInt16, ScalarInt32, M_S, UInt16, Int32, UInt32);
    scilab_fill_dotmul(UInt16, ScalarUInt32, M_S, UInt16, UInt32, UInt32);
    scilab_fill_dotmul(UInt16, ScalarInt64, M_S, UInt16, Int64, UInt64);
    scilab_fill_dotmul(UInt16, ScalarUInt64, M_S, UInt16, UInt64, UInt64);
    scilab_fill_dotmul(UInt16, ScalarBool, M_S, UInt16, Bool, UInt16);

    //Scalar .* Matrix
    scilab_fill_dotmul(ScalarUInt16, Double, S_M, UInt16, Double, UInt16);
    scilab_fill_dotmul(ScalarUInt16, Int8, S_M, UInt16, Int8, UInt16);
    scilab_fill_dotmul(ScalarUInt16, UInt8, S_M, UInt16, UInt8, UInt16);
    scilab_fill_dotmul(ScalarUInt16, Int16, S_M, UInt16, Int16, UInt16);
    scilab_fill_dotmul(ScalarUInt16, UInt16, S_M, UInt16, UInt16, UInt16);
    scilab_fill_dotmul(ScalarUInt16, Int32, S_M, UInt16, Int32, UInt32);
    scilab_fill_dotmul(ScalarUInt16, UInt32, S_M, UInt16, UInt32, UInt32);
    scilab_fill_dotmul(ScalarUInt16, Int64, S_M, UInt16, Int64, UInt64);
    scilab_fill_dotmul(ScalarUInt16, UInt64, S_M, UInt16, UInt64, UInt64);
    scilab_fill_dotmul(ScalarUInt16, Bool, S_M, UInt16, Bool, UInt16);
    scilab_fill_dotmul(ScalarUInt16, Empty, S_M, UInt16, Double, UInt16);

    //Scalar .* Scalar
    scilab_fill_dotmul(ScalarUInt16, ScalarDouble, S_S, UInt16, Double, UInt16);
    scilab_fill_dotmul(ScalarUInt16, ScalarInt8, S_S, UInt16, Int8, UInt16);
    scilab_fill_dotmul(ScalarUInt16, ScalarUInt8, S_S, UInt16, UInt8, UInt16);
    scilab_fill_dotmul(ScalarUInt16, ScalarInt16, S_S, UInt16, Int16, UInt16);
    scilab_fill_dotmul(ScalarUInt16, ScalarUInt16, S_S, UInt16, UInt16, UInt16);
    scilab_fill_dotmul(ScalarUInt16, ScalarInt32, S_S, UInt16, Int32, UInt32);
    scilab_fill_dotmul(ScalarUInt16, ScalarUInt32, S_S, UInt16, UInt32, UInt32);
    scilab_fill_dotmul(ScalarUInt16, ScalarInt64, S_S, UInt16, Int64, UInt64);
    scilab_fill_dotmul(ScalarUInt16, ScalarUInt64, S_S, UInt16, UInt64, UInt64);
    scilab_fill_dotmul(ScalarUInt16, ScalarBool, S_S, UInt16, Bool, UInt16);

    //Int32
    //Matrix .* Matrix
    scilab_fill_dotmul(Int32, Double, M_M, Int32, Double, Int32);
    scilab_fill_dotmul(Int32, Int8, M_M, Int32, Int8, Int32);
    scilab_fill_dotmul(Int32, UInt8, M_M, Int32, UInt8, UInt32);
    scilab_fill_dotmul(Int32, Int16, M_M, Int32, Int16, Int32);
    scilab_fill_dotmul(Int32, UInt16, M_M, Int32, UInt16, UInt32);
    scilab_fill_dotmul(Int32, Int32, M_M, Int32, Int32, Int32);
    scilab_fill_dotmul(Int32, UInt32, M_M, Int32, UInt32, UInt32);
    scilab_fill_dotmul(Int32, Int64, M_M, Int32, Int64, Int64);
    scilab_fill_dotmul(Int32, UInt64, M_M, Int32, UInt64, UInt64);
    scilab_fill_dotmul(Int32, Bool, M_M, Int32, Bool, Int32);
    scilab_fill_dotmul(Int32, Empty, M_E, Int32, Double, Double);

    //Matrix .* Scalar
    scilab_fill_dotmul(Int32, ScalarDouble, M_S, Int32, Double, Int32);
    scilab_fill_dotmul(Int32, ScalarInt8, M_S, Int32, Int8, Int32);
    scilab_fill_dotmul(Int32, ScalarUInt8, M_S, Int32, UInt8, UInt32);
    scilab_fill_dotmul(Int32, ScalarInt16, M_S, Int32, Int16, Int32);
    scilab_fill_dotmul(Int32, ScalarUInt16, M_S, Int32, UInt16, UInt32);
    scilab_fill_dotmul(Int32, ScalarInt32, M_S, Int32, Int32, Int32);
    scilab_fill_dotmul(Int32, ScalarUInt32, M_S, Int32, UInt32, UInt32);
    scilab_fill_dotmul(Int32, ScalarInt64, M_S, Int32, Int64, Int64);
    scilab_fill_dotmul(Int32, ScalarUInt64, M_S, Int32, UInt64, UInt64);
    scilab_fill_dotmul(Int32, ScalarBool, M_S, Int32, Bool, Int32);

    //Scalar .* Matrix
    scilab_fill_dotmul(ScalarInt32, Double, S_M, Int32, Double, Int32);
    scilab_fill_dotmul(ScalarInt32, Int8, S_M, Int32, Int8, Int32);
    scilab_fill_dotmul(ScalarInt32, UInt8, S_M, Int32, UInt8, UInt32);
    scilab_fill_dotmul(ScalarInt32, Int16, S_M, Int32, Int16, Int32);
    scilab_fill_dotmul(ScalarInt32, UInt16, S_M, Int32, UInt16, UInt32);
    scilab_fill_dotmul(ScalarInt32, Int32, S_M, Int32, Int32, Int32);
    scilab_fill_dotmul(ScalarInt32, UInt32, S_M, Int32, UInt32, UInt32);
    scilab_fill_dotmul(ScalarInt32, Int64, S_M, Int32, Int64, Int64);
    scilab_fill_dotmul(ScalarInt32, UInt64, S_M, Int32, UInt64, UInt64);
    scilab_fill_dotmul(ScalarInt32, Bool, S_M, Int32, Bool, Int32);
    scilab_fill_dotmul(ScalarInt32, Empty, M_E, Int32, Double, Double);

    //Scalar .* Scalar
    scilab_fill_dotmul(ScalarInt32, ScalarDouble, S_S, Int32, Double, Int32);
    scilab_fill_dotmul(ScalarInt32, ScalarInt8, S_S, Int32, Int8, Int32);
    scilab_fill_dotmul(ScalarInt32, ScalarUInt8, S_S, Int32, UInt8, UInt32);
    scilab_fill_dotmul(ScalarInt32, ScalarInt16, S_S, Int32, Int16, Int32);
    scilab_fill_dotmul(ScalarInt32, ScalarUInt16, S_S, Int32, UInt16, UInt32);
    scilab_fill_dotmul(ScalarInt32, ScalarInt32, S_S, Int32, Int32, Int32);
    scilab_fill_dotmul(ScalarInt32, ScalarUInt32, S_S, Int32, UInt32, UInt32);
    scilab_fill_dotmul(ScalarInt32, ScalarInt64, S_S, Int32, Int64, Int64);
    scilab_fill_dotmul(ScalarInt32, ScalarUInt64, S_S, Int32, UInt64, UInt64);
    scilab_fill_dotmul(ScalarInt32, ScalarBool, S_S, Int32, Bool, Int32);

    //UInt32
    //Matrix .* Matrix
    scilab_fill_dotmul(UInt32, Double, M_M, UInt32, Double, UInt32);
    scilab_fill_dotmul(UInt32, Int8, M_M, UInt32, Int8, UInt32);
    scilab_fill_dotmul(UInt32, UInt8, M_M, UInt32, UInt8, UInt32);
    scilab_fill_dotmul(UInt32, Int16, M_M, UInt32, Int16, UInt32);
    scilab_fill_dotmul(UInt32, UInt16, M_M, UInt32, UInt16, UInt32);
    scilab_fill_dotmul(UInt32, Int32, M_M, UInt32, Int32, UInt32);
    scilab_fill_dotmul(UInt32, UInt32, M_M, UInt32, UInt32, UInt32);
    scilab_fill_dotmul(UInt32, Int64, M_M, UInt32, Int64, UInt64);
    scilab_fill_dotmul(UInt32, UInt64, M_M, UInt32, UInt64, UInt64);
    scilab_fill_dotmul(UInt32, Bool, M_M, UInt32, Bool, UInt32);
    scilab_fill_dotmul(UInt32, Empty, M_E, UInt32, Double, Double);

    //Matrix .* Scalar
    scilab_fill_dotmul(UInt32, ScalarDouble, M_S, UInt32, Double, UInt32);
    scilab_fill_dotmul(UInt32, ScalarInt8, M_S, UInt32, Int8, UInt32);
    scilab_fill_dotmul(UInt32, ScalarUInt8, M_S, UInt32, UInt8, UInt32);
    scilab_fill_dotmul(UInt32, ScalarInt16, M_S, UInt32, Int16, UInt32);
    scilab_fill_dotmul(UInt32, ScalarUInt16, M_S, UInt32, UInt16, UInt32);
    scilab_fill_dotmul(UInt32, ScalarInt32, M_S, UInt32, Int32, UInt32);
    scilab_fill_dotmul(UInt32, ScalarUInt32, M_S, UInt32, UInt32, UInt32);
    scilab_fill_dotmul(UInt32, ScalarInt64, M_S, UInt32, Int64, UInt64);
    scilab_fill_dotmul(UInt32, ScalarUInt64, M_S, UInt32, UInt64, UInt64);
    scilab_fill_dotmul(UInt32, ScalarBool, M_S, UInt32, Bool, UInt32);

    //Scalar .* Matrix
    scilab_fill_dotmul(ScalarUInt32, Double, S_M, UInt32, Double, UInt32);
    scilab_fill_dotmul(ScalarUInt32, Int8, S_M, UInt32, Int8, UInt32);
    scilab_fill_dotmul(ScalarUInt32, UInt8, S_M, UInt32, UInt8, UInt32);
    scilab_fill_dotmul(ScalarUInt32, Int16, S_M, UInt32, Int16, UInt32);
    scilab_fill_dotmul(ScalarUInt32, UInt16, S_M, UInt32, UInt16, UInt32);
    scilab_fill_dotmul(ScalarUInt32, Int32, S_M, UInt32, Int32, UInt32);
    scilab_fill_dotmul(ScalarUInt32, UInt32, S_M, UInt32, UInt32, UInt32);
    scilab_fill_dotmul(ScalarUInt32, Int64, S_M, UInt32, Int64, UInt64);
    scilab_fill_dotmul(ScalarUInt32, UInt64, S_M, UInt32, UInt64, UInt64);
    scilab_fill_dotmul(ScalarUInt32, Bool, S_M, UInt32, Bool, UInt32);
    scilab_fill_dotmul(ScalarUInt32, Empty, S_M, UInt32, Double, UInt32);

    //Scalar .* Scalar
    scilab_fill_dotmul(ScalarUInt32, ScalarDouble, S_S, UInt32, Double, UInt32);
    scilab_fill_dotmul(ScalarUInt32, ScalarInt8, S_S, UInt32, Int8, UInt32);
    scilab_fill_dotmul(ScalarUInt32, ScalarUInt8, S_S, UInt32, UInt8, UInt32);
    scilab_fill_dotmul(ScalarUInt32, ScalarInt16, S_S, UInt32, Int16, UInt32);
    scilab_fill_dotmul(ScalarUInt32, ScalarUInt16, S_S, UInt32, UInt16, UInt32);
    scilab_fill_dotmul(ScalarUInt32, ScalarInt32, S_S, UInt32, Int32, UInt32);
    scilab_fill_dotmul(ScalarUInt32, ScalarUInt32, S_S, UInt32, UInt32, UInt32);
    scilab_fill_dotmul(ScalarUInt32, ScalarInt64, S_S, UInt32, Int64, UInt64);
    scilab_fill_dotmul(ScalarUInt32, ScalarUInt64, S_S, UInt32, UInt64, UInt64);
    scilab_fill_dotmul(ScalarUInt32, ScalarBool, S_S, UInt32, Bool, UInt32);

    //Int64
    //Matrix .* Matrix
    scilab_fill_dotmul(Int64, Double, M_M, Int64, Double, Int64);
    scilab_fill_dotmul(Int64, Int8, M_M, Int64, Int8, Int64);
    scilab_fill_dotmul(Int64, UInt8, M_M, Int64, UInt8, UInt64);
    scilab_fill_dotmul(Int64, Int16, M_M, Int64, Int16, Int64);
    scilab_fill_dotmul(Int64, UInt16, M_M, Int64, UInt16, UInt64);
    scilab_fill_dotmul(Int64, Int32, M_M, Int64, Int32, Int64);
    scilab_fill_dotmul(Int64, UInt32, M_M, Int64, UInt32, UInt64);
    scilab_fill_dotmul(Int64, Int64, M_M, Int64, Int64, Int64);
    scilab_fill_dotmul(Int64, UInt64, M_M, Int64, UInt64, UInt64);
    scilab_fill_dotmul(Int64, Bool, M_M, Int64, Bool, Int64);
    scilab_fill_dotmul(Int64, Empty, M_E, Int64, Double, Double);

    //Matrix .* Scalar
    scilab_fill_dotmul(Int64, ScalarDouble, M_S, Int64, Double, Int64);
    scilab_fill_dotmul(Int64, ScalarInt8, M_S, Int64, Int8, Int64);
    scilab_fill_dotmul(Int64, ScalarUInt8, M_S, Int64, UInt8, UInt64);
    scilab_fill_dotmul(Int64, ScalarInt16, M_S, Int64, Int16, Int64);
    scilab_fill_dotmul(Int64, ScalarUInt16, M_S, Int64, UInt16, UInt64);
    scilab_fill_dotmul(Int64, ScalarInt32, M_S, Int64, Int32, Int64);
    scilab_fill_dotmul(Int64, ScalarUInt32, M_S, Int64, UInt32, UInt64);
    scilab_fill_dotmul(Int64, ScalarInt64, M_S, Int64, Int64, Int64);
    scilab_fill_dotmul(Int64, ScalarUInt64, M_S, Int64, UInt64, UInt64);
    scilab_fill_dotmul(Int64, ScalarBool, M_S, Int64, Bool, Int64);

    //Scalar .* Matrix
    scilab_fill_dotmul(ScalarInt64, Double, S_M, Int64, Double, Int64);
    scilab_fill_dotmul(ScalarInt64, Int8, S_M, Int64, Int8, Int64);
    scilab_fill_dotmul(ScalarInt64, UInt8, S_M, Int64, UInt8, UInt64);
    scilab_fill_dotmul(ScalarInt64, Int16, S_M, Int64, Int16, Int64);
    scilab_fill_dotmul(ScalarInt64, UInt16, S_M, Int64, UInt16, UInt64);
    scilab_fill_dotmul(ScalarInt64, Int32, S_M, Int64, Int32, Int64);
    scilab_fill_dotmul(ScalarInt64, UInt32, S_M, Int64, UInt32, UInt64);
    scilab_fill_dotmul(ScalarInt64, Int64, S_M, Int64, Int64, Int64);
    scilab_fill_dotmul(ScalarInt64, UInt64, S_M, Int64, UInt64, UInt64);
    scilab_fill_dotmul(ScalarInt64, Bool, S_M, Int64, Bool, Int64);
    scilab_fill_dotmul(ScalarInt64, Empty, M_E, Int64, Double, Double);

    //Scalar .* Scalar
    scilab_fill_dotmul(ScalarInt64, ScalarDouble, S_S, Int64, Double, Int64);
    scilab_fill_dotmul(ScalarInt64, ScalarInt8, S_S, Int64, Int8, Int64);
    scilab_fill_dotmul(ScalarInt64, ScalarUInt8, S_S, Int64, UInt8, UInt64);
    scilab_fill_dotmul(ScalarInt64, ScalarInt16, S_S, Int64, Int16, Int64);
    scilab_fill_dotmul(ScalarInt64, ScalarUInt16, S_S, Int64, UInt16, UInt64);
    scilab_fill_dotmul(ScalarInt64, ScalarInt32, S_S, Int64, Int32, Int64);
    scilab_fill_dotmul(ScalarInt64, ScalarUInt32, S_S, Int64, UInt32, UInt64);
    scilab_fill_dotmul(ScalarInt64, ScalarInt64, S_S, Int64, Int64, Int64);
    scilab_fill_dotmul(ScalarInt64, ScalarUInt64, S_S, Int64, UInt64, UInt64);
    scilab_fill_dotmul(ScalarInt64, ScalarBool, S_S, Int64, Bool, Int64);

    //UInt64
    //Matrix .* Matrix
    scilab_fill_dotmul(UInt64, Double, M_M, UInt64, Double, UInt64);
    scilab_fill_dotmul(UInt64, Int8, M_M, UInt64, Int8, UInt64);
    scilab_fill_dotmul(UInt64, UInt8, M_M, UInt64, UInt8, UInt64);
    scilab_fill_dotmul(UInt64, Int16, M_M, UInt64, Int16, UInt64);
    scilab_fill_dotmul(UInt64, UInt16, M_M, UInt64, UInt16, UInt64);
    scilab_fill_dotmul(UInt64, Int32, M_M, UInt64, Int32, UInt64);
    scilab_fill_dotmul(UInt64, UInt32, M_M, UInt64, UInt32, UInt64);
    scilab_fill_dotmul(UInt64, Int64, M_M, UInt64, Int64, UInt64);
    scilab_fill_dotmul(UInt64, UInt64, M_M, UInt64, UInt64, UInt64);
    scilab_fill_dotmul(UInt64, Bool, M_M, UInt64, Bool, UInt64);
    scilab_fill_dotmul(UInt64, Empty, M_E, UInt64, Double, Double);

    //Matrix .* Scalar
    scilab_fill_dotmul(UInt64, ScalarDouble, M_S, UInt64, Double, UInt64);
    scilab_fill_dotmul(UInt64, ScalarInt8, M_S, UInt64, Int8, UInt64);
    scilab_fill_dotmul(UInt64, ScalarUInt8, M_S, UInt64, UInt8, UInt64);
    scilab_fill_dotmul(UInt64, ScalarInt16, M_S, UInt64, Int16, UInt64);
    scilab_fill_dotmul(UInt64, ScalarUInt16, M_S, UInt64, UInt16, UInt64);
    scilab_fill_dotmul(UInt64, ScalarInt32, M_S, UInt64, Int32, UInt64);
    scilab_fill_dotmul(UInt64, ScalarUInt32, M_S, UInt64, UInt32, UInt64);
    scilab_fill_dotmul(UInt64, ScalarInt64, M_S, UInt64, Int64, UInt64);
    scilab_fill_dotmul(UInt64, ScalarUInt64, M_S, UInt64, UInt64, UInt64);
    scilab_fill_dotmul(UInt64, ScalarBool, M_S, UInt64, Bool, UInt64);

    //Scalar .* Matrix
    scilab_fill_dotmul(ScalarUInt64, Double, S_M, UInt64, Double, UInt64);
    scilab_fill_dotmul(ScalarUInt64, Int8, S_M, UInt64, Int8, UInt64);
    scilab_fill_dotmul(ScalarUInt64, UInt8, S_M, UInt64, UInt8, UInt64);
    scilab_fill_dotmul(ScalarUInt64, Int16, S_M, UInt64, Int16, UInt64);
    scilab_fill_dotmul(ScalarUInt64, UInt16, S_M, UInt64, UInt16, UInt64);
    scilab_fill_dotmul(ScalarUInt64, Int32, S_M, UInt64, Int32, UInt64);
    scilab_fill_dotmul(ScalarUInt64, UInt32, S_M, UInt64, UInt32, UInt64);
    scilab_fill_dotmul(ScalarUInt64, Int64, S_M, UInt64, Int64, UInt64);
    scilab_fill_dotmul(ScalarUInt64, UInt64, S_M, UInt64, UInt64, UInt64);
    scilab_fill_dotmul(ScalarUInt64, Bool, S_M, UInt64, Bool, UInt64);
    scilab_fill_dotmul(ScalarUInt64, Empty, S_M, UInt64, Double, UInt64);

    //Scalar .* Scalar
    scilab_fill_dotmul(ScalarUInt64, ScalarDouble, S_S, UInt64, Double, UInt64);
    scilab_fill_dotmul(ScalarUInt64, ScalarInt8, S_S, UInt64, Int8, UInt64);
    scilab_fill_dotmul(ScalarUInt64, ScalarUInt8, S_S, UInt64, UInt8, UInt64);
    scilab_fill_dotmul(ScalarUInt64, ScalarInt16, S_S, UInt64, Int16, UInt64);
    scilab_fill_dotmul(ScalarUInt64, ScalarUInt16, S_S, UInt64, UInt16, UInt64);
    scilab_fill_dotmul(ScalarUInt64, ScalarInt32, S_S, UInt64, Int32, UInt64);
    scilab_fill_dotmul(ScalarUInt64, ScalarUInt32, S_S, UInt64, UInt32, UInt64);
    scilab_fill_dotmul(ScalarUInt64, ScalarInt64, S_S, UInt64, Int64, UInt64);
    scilab_fill_dotmul(ScalarUInt64, ScalarUInt64, S_S, UInt64, UInt64, UInt64);
    scilab_fill_dotmul(ScalarUInt64, ScalarBool, S_S, UInt64, Bool, UInt64);

    //Bool
    //Matrix + Matrix
    scilab_fill_dotmul(Bool, Double, M_M, Bool, Double, Double);
    scilab_fill_dotmul(Bool, Int8, M_M, Bool, Int8, Int8);
    scilab_fill_dotmul(Bool, UInt8, M_M, Bool, UInt8, UInt8);
    scilab_fill_dotmul(Bool, Int16, M_M, Bool, Int16, Int16);
    scilab_fill_dotmul(Bool, UInt16, M_M, Bool, UInt16, UInt16);
    scilab_fill_dotmul(Bool, Int32, M_M, Bool, Int32, Int32);
    scilab_fill_dotmul(Bool, UInt32, M_M, Bool, UInt32, UInt32);
    scilab_fill_dotmul(Bool, Int64, M_M, Bool, Int64, Int64);
    scilab_fill_dotmul(Bool, UInt64, M_M, Bool, UInt64, UInt64);
    scilab_fill_dotmul(Bool, Bool, M_M, Bool, Bool, Bool);
    scilab_fill_dotmul(Bool, Empty, M_E, Bool, Double, Double);

    //Matrix + Scalar
    scilab_fill_dotmul(Bool, ScalarDouble, M_S, Bool, Double, Double);
    scilab_fill_dotmul(Bool, ScalarInt8, M_S, Bool, Int8, Int8);
    scilab_fill_dotmul(Bool, ScalarUInt8, M_S, Bool, UInt8, UInt8);
    scilab_fill_dotmul(Bool, ScalarInt16, M_S, Bool, Int16, Int16);
    scilab_fill_dotmul(Bool, ScalarUInt16, M_S, Bool, UInt16, UInt16);
    scilab_fill_dotmul(Bool, ScalarInt32, M_S, Bool, Int32, Int32);
    scilab_fill_dotmul(Bool, ScalarUInt32, M_S, Bool, UInt32, UInt32);
    scilab_fill_dotmul(Bool, ScalarInt64, M_S, Bool, Int64, Int64);
    scilab_fill_dotmul(Bool, ScalarUInt64, M_S, Bool, UInt64, UInt64);
    scilab_fill_dotmul(Bool, ScalarBool, M_S, Bool, Bool, Bool);

    //Scalar + Matrix
    scilab_fill_dotmul(ScalarBool, Double, S_M, Bool, Double, Double);
    scilab_fill_dotmul(ScalarBool, Int8, S_M, Bool, Int8, Int8);
    scilab_fill_dotmul(ScalarBool, UInt8, S_M, Bool, UInt8, UInt8);
    scilab_fill_dotmul(ScalarBool, Int16, S_M, Bool, Int16, Int16);
    scilab_fill_dotmul(ScalarBool, UInt16, S_M, Bool, UInt16, UInt16);
    scilab_fill_dotmul(ScalarBool, Int32, S_M, Bool, Int32, Int32);
    scilab_fill_dotmul(ScalarBool, UInt32, S_M, Bool, UInt32, UInt32);
    scilab_fill_dotmul(ScalarBool, Int64, S_M, Bool, Int64, Int64);
    scilab_fill_dotmul(ScalarBool, UInt64, S_M, Bool, UInt64, UInt64);
    scilab_fill_dotmul(ScalarBool, Bool, S_M, Bool, Bool, Bool);
    scilab_fill_dotmul(ScalarBool, Empty, M_E, Bool, Double, Double);

    //Scalar + Scalar
    scilab_fill_dotmul(ScalarBool, ScalarDouble, S_S, Bool, Double, Double);
    scilab_fill_dotmul(ScalarBool, ScalarInt8, S_S, Bool, Int8, Int8);
    scilab_fill_dotmul(ScalarBool, ScalarUInt8, S_S, Bool, UInt8, UInt8);
    scilab_fill_dotmul(ScalarBool, ScalarInt16, S_S, Bool, Int16, Int16);
    scilab_fill_dotmul(ScalarBool, ScalarUInt16, S_S, Bool, UInt16, UInt16);
    scilab_fill_dotmul(ScalarBool, ScalarInt32, S_S, Bool, Int32, Int32);
    scilab_fill_dotmul(ScalarBool, ScalarUInt32, S_S, Bool, UInt32, UInt32);
    scilab_fill_dotmul(ScalarBool, ScalarInt64, S_S, Bool, Int64, Int64);
    scilab_fill_dotmul(ScalarBool, ScalarUInt64, S_S, Bool, UInt64, UInt64);
    scilab_fill_dotmul(ScalarBool, ScalarBool, S_S, Bool, Bool, Bool);

    //Identity
    scilab_fill_dotmul(Identity, Double, I_M, Double, Double, Double);
    scilab_fill_dotmul(Identity, DoubleComplex, I_MC, Double, Double, Double);
    scilab_fill_dotmul(Identity, ScalarDouble, I_S, Double, Double, Double);
    scilab_fill_dotmul(Identity, ScalarDoubleComplex, I_SC, Double, Double, Double);
    scilab_fill_dotmul(Identity, Identity, I_I, Double, Double, Double);
    scilab_fill_dotmul(Identity, IdentityComplex, I_IC, Double, Double, Double);
    scilab_fill_dotmul(Identity, Empty, M_E, Double, Double, Double);

    scilab_fill_dotmul(Identity, Polynom, M_M, Double, Polynom, Polynom);
    scilab_fill_dotmul(Identity, PolynomComplex, M_M, Double, Polynom, Polynom);
    scilab_fill_dotmul(Identity, ScalarPolynom, M_M, Double, Polynom, Polynom);
    scilab_fill_dotmul(Identity, ScalarPolynomComplex, M_M, Double, Polynom, Polynom);
    //scilab_fill_dotmul(Identity, Sparse, M_M, Double, Sparse, Sparse);
    //scilab_fill_dotmul(Identity, SparseComplex, M_M, Double, Sparse, Sparse);

    scilab_fill_dotmul(IdentityComplex, Double, IC_M, Double, Double, Double);
    scilab_fill_dotmul(IdentityComplex, DoubleComplex, IC_MC, Double, Double, Double);
    scilab_fill_dotmul(IdentityComplex, ScalarDouble, IC_S, Double, Double, Double);
    scilab_fill_dotmul(IdentityComplex, ScalarDoubleComplex, IC_SC, Double, Double, Double);
    scilab_fill_dotmul(IdentityComplex, Identity, IC_I, Double, Double, Double);
    scilab_fill_dotmul(IdentityComplex, IdentityComplex, IC_IC, Double, Double, Double);
    scilab_fill_dotmul(IdentityComplex, Empty, M_E, Double, Double, Double);

    scilab_fill_dotmul(IdentityComplex, Polynom, M_M, Double, Polynom, Polynom);
    scilab_fill_dotmul(IdentityComplex, PolynomComplex, M_M, Double, Polynom, Polynom);
    scilab_fill_dotmul(IdentityComplex, ScalarPolynom, M_M, Double, Polynom, Polynom);
    scilab_fill_dotmul(IdentityComplex, ScalarPolynomComplex, M_M, Double, Polynom, Polynom);
    //scilab_fill_dotmul(IdentityComplex, Sparse, M_M, Double, Sparse, Sparse);
    //scilab_fill_dotmul(IdentityComplex, SparseComplex, M_M, Double, Sparse, Sparse);

    //Polynom

    //poly + poly
    scilab_fill_dotmul(Polynom, Polynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_dotmul(Polynom, PolynomComplex, M_M, Polynom, Polynom, Polynom);
    scilab_fill_dotmul(PolynomComplex, Polynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_dotmul(PolynomComplex, PolynomComplex, M_M, Polynom, Polynom, Polynom);

    //poly + scalar poly
    scilab_fill_dotmul(Polynom, ScalarPolynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_dotmul(Polynom, ScalarPolynomComplex, M_M, Polynom, Polynom, Polynom);
    scilab_fill_dotmul(PolynomComplex, ScalarPolynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_dotmul(PolynomComplex, ScalarPolynomComplex, M_M, Polynom, Polynom, Polynom);

    //poly + double
    scilab_fill_dotmul(Polynom, Double, M_M, Polynom, Double, Polynom);
    scilab_fill_dotmul(Polynom, DoubleComplex, M_M, Polynom, Double, Polynom);
    scilab_fill_dotmul(PolynomComplex, Double, M_M, Polynom, Double, Polynom);
    scilab_fill_dotmul(PolynomComplex, DoubleComplex, M_M, Polynom, Double, Polynom);

    //poly + scalar double
    scilab_fill_dotmul(Polynom, ScalarDouble, M_M, Polynom, Double, Polynom);
    scilab_fill_dotmul(Polynom, ScalarDoubleComplex, M_M, Polynom, Double, Polynom);
    scilab_fill_dotmul(PolynomComplex, ScalarDouble, M_M, Polynom, Double, Polynom);
    scilab_fill_dotmul(PolynomComplex, ScalarDoubleComplex, M_M, Polynom, Double, Polynom);

    //poly + []
    scilab_fill_dotmul(Polynom, Empty, M_E, Polynom, Double, Double);
    scilab_fill_dotmul(PolynomComplex, Empty, M_E, Polynom, Double, Double);

    //poly + eye
    scilab_fill_dotmul(Polynom, Identity, M_M, Polynom, Double, Polynom);
    scilab_fill_dotmul(Polynom, IdentityComplex, M_M, Polynom, Double, Polynom);
    scilab_fill_dotmul(PolynomComplex, Identity, M_M, Polynom, Double, Polynom);
    scilab_fill_dotmul(PolynomComplex, IdentityComplex, M_M, Polynom, Double, Polynom);

    //scalar poly + poly
    scilab_fill_dotmul(ScalarPolynom, Polynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_dotmul(ScalarPolynom, PolynomComplex, M_M, Polynom, Polynom, Polynom);
    scilab_fill_dotmul(ScalarPolynomComplex, Polynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_dotmul(ScalarPolynomComplex, PolynomComplex, M_M, Polynom, Polynom, Polynom);

    //scalar poly + scalar poly
    scilab_fill_dotmul(ScalarPolynom, ScalarPolynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_dotmul(ScalarPolynom, ScalarPolynomComplex, M_M, Polynom, Polynom, Polynom);
    scilab_fill_dotmul(ScalarPolynomComplex, ScalarPolynom, M_M, Polynom, Polynom, Polynom);
    scilab_fill_dotmul(ScalarPolynomComplex, ScalarPolynomComplex, M_M, Polynom, Polynom, Polynom);

    //scalar poly + double
    scilab_fill_dotmul(ScalarPolynom, Double, M_M, Polynom, Double, Polynom);
    scilab_fill_dotmul(ScalarPolynom, DoubleComplex, M_M, Polynom, Double, Polynom);
    scilab_fill_dotmul(ScalarPolynomComplex, Double, M_M, Polynom, Double, Polynom);
    scilab_fill_dotmul(ScalarPolynomComplex, DoubleComplex, M_M, Polynom, Double, Polynom);

    //scalar poly + scalar double
    scilab_fill_dotmul(ScalarPolynom, ScalarDouble, M_M, Polynom, Double, Polynom);
    scilab_fill_dotmul(ScalarPolynom, ScalarDoubleComplex, M_M, Polynom, Double, Polynom);
    scilab_fill_dotmul(ScalarPolynomComplex, ScalarDouble, M_M, Polynom, Double, Polynom);
    scilab_fill_dotmul(ScalarPolynomComplex, ScalarDoubleComplex, M_M, Polynom, Double, Polynom);

    //scalar poly + []
    scilab_fill_dotmul(ScalarPolynom, Empty, M_E, Polynom, Double, Double);
    scilab_fill_dotmul(ScalarPolynomComplex, Empty, M_E, Polynom, Double, Double);

    //scalar poly + eye
    scilab_fill_dotmul(ScalarPolynom, Identity, M_M, Polynom, Double, Polynom);
    scilab_fill_dotmul(ScalarPolynom, IdentityComplex, M_M, Polynom, Double, Polynom);
    scilab_fill_dotmul(ScalarPolynomComplex, Identity, M_M, Polynom, Double, Polynom);
    scilab_fill_dotmul(ScalarPolynomComplex, IdentityComplex, M_M, Polynom, Double, Polynom);

    //Sparse
    scilab_fill_dotmul(Sparse, Sparse, M_M, Sparse, Sparse, Sparse);
    scilab_fill_dotmul(Sparse, SparseComplex, M_M, Sparse, Sparse, Sparse);
    scilab_fill_dotmul(Sparse, Double, M_M, Sparse, Double, Sparse);
    scilab_fill_dotmul(Sparse, DoubleComplex, M_M, Sparse, Double, Sparse);
    scilab_fill_dotmul(Sparse, ScalarDouble, M_M, Sparse, Double, Sparse);
    scilab_fill_dotmul(Sparse, ScalarDoubleComplex, M_M, Sparse, Double, Sparse);

    scilab_fill_dotmul(Sparse, Empty, M_E, Sparse, Double, Double);
    //scilab_fill_dotmul(Sparse, Identity, M_M, Sparse, Double, Sparse);
    //scilab_fill_dotmul(Sparse, IdentityComplex, M_M, Sparse, Double, Sparse);

    scilab_fill_dotmul(SparseComplex, Sparse, M_M, Sparse, Sparse, Sparse);
    scilab_fill_dotmul(SparseComplex, SparseComplex, M_M, Sparse, Sparse, Sparse);
    scilab_fill_dotmul(SparseComplex, Double, M_M, Sparse, Double, Sparse);
    scilab_fill_dotmul(SparseComplex, DoubleComplex, M_M, Sparse, Double, Sparse);
    scilab_fill_dotmul(SparseComplex, ScalarDouble, M_M, Sparse, Double, Sparse);
    scilab_fill_dotmul(SparseComplex, ScalarDoubleComplex, M_M, Sparse, Double, Sparse);

    scilab_fill_dotmul(SparseComplex, Empty, M_E, Sparse, Double, Double);
    //scilab_fill_dotmul(SparseComplex, Identity, M_M, Sparse, Double, Sparse);
    //scilab_fill_dotmul(SparseComplex, IdentityComplex, M_M, Sparse, Double, Sparse);

#undef scilab_fill_dotmul
}

InternalType *GenericDotTimes(InternalType *_pLeftOperand, InternalType *_pRightOperand)
{
    InternalType *pResult = NULL;

    dotmul_function dotmul = pDotMulfunction[_pLeftOperand->getId()][_pRightOperand->getId()];
    if (dotmul)
    {
        pResult = dotmul(_pLeftOperand, _pRightOperand);
        if (pResult)
        {
            return pResult;
        }
    }

    /*
    ** Default case : Return NULL will Call Overloading.
    */
    return NULL;
}

InternalType *GenericTimes(InternalType *_pLeftOperand, InternalType *_pRightOperand)
{
    InternalType *pResult = NULL;
    GenericType::ScilabType TypeL = _pLeftOperand->getType();
    GenericType::ScilabType TypeR = _pRightOperand->getType();

    if (TypeL == GenericType::ScilabDouble && _pLeftOperand->getAs<Double>()->isEmpty())
    {
        return Double::Empty();
    }

    if (TypeR == GenericType::ScilabDouble && _pRightOperand->getAs<Double>()->isEmpty())
    {
        return Double::Empty();
    }

    /*
    ** DOUBLE * DOUBLE
    */
    if (TypeL == GenericType::ScilabDouble && TypeR == GenericType::ScilabDouble)
    {
        Double *pL   = _pLeftOperand->getAs<Double>();
        Double *pR   = _pRightOperand->getAs<Double>();

        int iResult = MultiplyDoubleByDouble(pL, pR, (Double**)&pResult);
        if (iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** DOUBLE * POLY
    */
    else if (TypeL == InternalType::ScilabDouble && TypeR == InternalType::ScilabPolynom)
    {
        Double *pL   = _pLeftOperand->getAs<Double>();
        Polynom *pR     = _pRightOperand->getAs<types::Polynom>();

        int iResult = MultiplyDoubleByPoly(pL, pR, (Polynom**)&pResult);
        if (iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** POLY * DOUBLE
    */
    else if (TypeL == InternalType::ScilabPolynom && TypeR == InternalType::ScilabDouble)
    {
        Polynom *pL          = _pLeftOperand->getAs<types::Polynom>();
        Double *pR              = _pRightOperand->getAs<Double>();

        int iResult = MultiplyPolyByDouble(pL, pR, (Polynom**)&pResult);
        if (iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** POLY * POLY
    */
    else if (TypeL == InternalType::ScilabPolynom && TypeR == InternalType::ScilabPolynom)
    {
        Polynom *pL          = _pLeftOperand->getAs<types::Polynom>();
        Polynom *pR          = _pRightOperand->getAs<types::Polynom>();

        int iResult = MultiplyPolyByPoly(pL, pR, (Polynom**)&pResult);
        if (iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** SPARSE * SPARSE
    */
    if (TypeL == GenericType::ScilabSparse && TypeR == GenericType::ScilabSparse)
    {
        Sparse *pL   = _pLeftOperand->getAs<Sparse>();
        Sparse *pR   = _pRightOperand->getAs<Sparse>();

        int iResult = MultiplySparseBySparse(pL, pR, (Sparse**)&pResult);
        if (iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** DOUBLE * SPARSE
    */
    if (TypeL == GenericType::ScilabDouble && TypeR == GenericType::ScilabSparse)
    {
        Double *pL   = _pLeftOperand->getAs<Double>();
        Sparse *pR   = _pRightOperand->getAs<Sparse>();

        int iResult = MultiplyDoubleBySparse(pL, pR, (GenericType**)&pResult);
        if (iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** SPARSE * DOUBLE
    */
    if (TypeL == GenericType::ScilabSparse && TypeR == GenericType::ScilabDouble)
    {
        Sparse *pL   = _pLeftOperand->getAs<Sparse>();
        Double *pR   = _pRightOperand->getAs<Double>();

        int iResult = MultiplySparseByDouble(pL, pR, (GenericType**)&pResult);
        if (iResult)
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        return pResult;
    }

    /*
    ** Default case : Return NULL will Call Overloading.
    */
    return NULL;

}

int MultiplyDoubleByDouble(Double* _pDouble1, Double* _pDouble2, Double** _pDoubleOut)
{
    if (_pDouble1->isScalar())
    {
        bool bComplex1  = _pDouble1->isComplex();
        bool bComplex2  = _pDouble2->isComplex();

        (*_pDoubleOut) = new Double(_pDouble2->getDims(), _pDouble2->getDimsArray(), bComplex1 | bComplex2);

        if (bComplex1 == false && bComplex2 == false)
        {
            iMultiRealScalarByRealMatrix(_pDouble1->get(0), _pDouble2->get(), _pDouble2->getSize(), 1, (*_pDoubleOut)->get());
        }
        else if (bComplex1 == false && bComplex2 == true)
        {
            iMultiRealScalarByComplexMatrix(_pDouble1->get(0), _pDouble2->get(), _pDouble2->getImg(), _pDouble2->getSize(), 1, (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }
        else if (bComplex1 == true && bComplex2 == false)
        {
            iMultiComplexScalarByRealMatrix(_pDouble1->get(0), _pDouble1->getImg(0), _pDouble2->get(), _pDouble2->getSize(), 1, (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }
        else //if(bComplex1 == true && bComplex2 == true)
        {
            iMultiComplexScalarByComplexMatrix(_pDouble1->get(0), _pDouble1->getImg(0), _pDouble2->get(), _pDouble2->getImg(), _pDouble2->getSize(), 1, (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }

        return 0;
    }

    if (_pDouble2->isScalar())
    {
        bool bComplex1  = _pDouble1->isComplex();
        bool bComplex2  = _pDouble2->isComplex();

        (*_pDoubleOut) = new Double(_pDouble1->getDims(), _pDouble1->getDimsArray(), bComplex1 | bComplex2);

        if (bComplex1 == false && bComplex2 == false)
        {
            //Real Matrix by Real Scalar
            iMultiRealScalarByRealMatrix(_pDouble2->get(0), _pDouble1->get(), _pDouble1->getSize(), 1, (*_pDoubleOut)->get());
        }
        else if (bComplex1 == false && bComplex2 == true)
        {
            //Real Matrix by Scalar Complex
            iMultiComplexScalarByRealMatrix(_pDouble2->get(0), _pDouble2->getImg(0), _pDouble1->get(), _pDouble1->getSize(), 1, (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }
        else if (bComplex1 == true && bComplex2 == false)
        {
            iMultiRealScalarByComplexMatrix(_pDouble2->get(0, 0), _pDouble1->get(), _pDouble1->getImg(), _pDouble1->getSize(), 1, (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }
        else //if(bComplex1 == true && bComplex2 == true)
        {
            iMultiComplexScalarByComplexMatrix(_pDouble2->get(0, 0), _pDouble2->getImg(0, 0), _pDouble1->get(), _pDouble1->getImg(), _pDouble1->getSize(), 1, (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }

        return 0;
    }

    if (_pDouble1->getDims() > 2 || _pDouble2->getDims() > 2 || _pDouble1->getCols() != _pDouble2->getRows())
    {
        return 1;
    }

    bool bComplex1  = _pDouble1->isComplex();
    bool bComplex2  = _pDouble2->isComplex();
    (*_pDoubleOut) = new Double(_pDouble1->getRows(), _pDouble2->getCols(), bComplex1 | bComplex2);

    if (bComplex1 == false && bComplex2 == false)
    {
        //Real Matrix by Real Matrix
        iMultiRealMatrixByRealMatrix(
            _pDouble1->get(), _pDouble1->getRows(), _pDouble1->getCols(),
            _pDouble2->get(), _pDouble2->getRows(), _pDouble2->getCols(),
            (*_pDoubleOut)->get());
    }
    else if (bComplex1 == false && bComplex2 == true)
    {
        //Real Matrix by Matrix Complex
        iMultiRealMatrixByComplexMatrix(
            _pDouble1->get(), _pDouble1->getRows(), _pDouble1->getCols(),
            _pDouble2->get(), _pDouble2->getImg(), _pDouble2->getRows(), _pDouble2->getCols(),
            (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
    }
    else if (bComplex1 == true && bComplex2 == false)
    {
        //Complex Matrix by Real Matrix
        iMultiComplexMatrixByRealMatrix(
            _pDouble1->get(), _pDouble1->getImg(), _pDouble1->getRows(), _pDouble1->getCols(),
            _pDouble2->get(), _pDouble2->getRows(), _pDouble2->getCols(),
            (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
    }
    else //if(bComplex1 == true && bComplex2 == true)
    {
        //Complex Matrix by Complex Matrix
        iMultiComplexMatrixByComplexMatrix(
            _pDouble1->get(), _pDouble1->getImg(), _pDouble1->getRows(), _pDouble1->getCols(),
            _pDouble2->get(), _pDouble2->getImg(), _pDouble2->getRows(), _pDouble2->getCols(),
            (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
    }
    return 0;
}

int DotMultiplyDoubleByDouble(Double* _pDouble1, Double* _pDouble2, Double**  _pDoubleOut)
{
    bool bComplex1  = _pDouble1->isComplex();
    bool bComplex2  = _pDouble2->isComplex();
    bool bScalar1   = _pDouble1->isScalar();
    bool bScalar2   = _pDouble2->isScalar();

    if (bScalar1)
    {
        (*_pDoubleOut) = new Double(_pDouble2->getDims(), _pDouble2->getDimsArray(), _pDouble1->isComplex() | _pDouble2->isComplex());
        if (bComplex1 == false && bComplex2 == false)
        {
            iMultiRealScalarByRealMatrix(_pDouble1->get(0), _pDouble2->get(), _pDouble2->getSize(), 1, (*_pDoubleOut)->get());
        }
        else if (bComplex1 == false && bComplex2 == true)
        {
            iMultiRealScalarByComplexMatrix(_pDouble1->get(0), _pDouble2->get(), _pDouble2->getImg(), _pDouble2->getSize(), 1, (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }
        else if (bComplex1 == true && bComplex2 == false)
        {
            iMultiComplexScalarByRealMatrix(_pDouble1->get(0), _pDouble1->getImg(0), _pDouble2->get(), _pDouble2->getSize(), 1, (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }
        else //if(bComplex1 == true && bComplex2 == true)
        {
            iMultiComplexScalarByComplexMatrix(_pDouble1->get(0), _pDouble1->getImg(0), _pDouble2->get(), _pDouble2->getImg(), _pDouble2->getSize(), 1, (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }

        return 0;
    }

    if (bScalar2)
    {
        (*_pDoubleOut) = new Double(_pDouble1->getDims(), _pDouble1->getDimsArray(), _pDouble1->isComplex() | _pDouble2->isComplex());
        if (bComplex1 == false && bComplex2 == false)
        {
            //Real Matrix by Real Scalar
            iMultiRealScalarByRealMatrix(_pDouble2->get(0), _pDouble1->get(), _pDouble1->getSize(), 1, (*_pDoubleOut)->get());
        }
        else if (bComplex1 == false && bComplex2 == true)
        {
            //Real Matrix by Scalar Complex
            iMultiComplexScalarByRealMatrix(_pDouble2->get(0), _pDouble2->getImg(0), _pDouble1->get(), _pDouble1->getSize(), 1, (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }
        else if (bComplex1 == true && bComplex2 == false)
        {
            iMultiRealScalarByComplexMatrix(_pDouble2->get(0), _pDouble1->get(), _pDouble1->getImg(), _pDouble1->getSize(), 1, (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }
        else //if(bComplex1 == true && bComplex2 == true)
        {
            iMultiComplexScalarByComplexMatrix(_pDouble2->get(0), _pDouble2->getImg(0), _pDouble1->get(), _pDouble1->getImg(), _pDouble1->getSize(), 1, (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg());
        }

        return 0;
    }

    if (_pDouble1->getDims() != _pDouble2->getDims())
    {
        return 1;
    }

    int* piDims1 = _pDouble1->getDimsArray();
    int* piDims2 = _pDouble2->getDimsArray();

    for (int i = 0 ; i < _pDouble1->getDims() ; i++)
    {
        if (piDims1[i] != piDims2[i])
        {
            return 0;
        }
    }

    (*_pDoubleOut) = new Double(_pDouble1->getDims(), _pDouble1->getDimsArray(), _pDouble1->isComplex() | _pDouble2->isComplex());
    if (bComplex1 == false && bComplex2 == false)
    {
        iDotMultiplyRealMatrixByRealMatrix(_pDouble1->get(), _pDouble2->get(), (*_pDoubleOut)->get(), _pDouble1->getSize(), 1);
    }
    else if (bComplex1 == false && bComplex2 == true)
    {
        iDotMultiplyRealMatrixByComplexMatrix(_pDouble1->get(), _pDouble2->get(), _pDouble2->getImg(), (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg(), _pDouble1->getSize(), 1);
    }
    else if (bComplex1 == true && bComplex2 == false)
    {
        iDotMultiplyComplexMatrixByRealMatrix(_pDouble1->get(), _pDouble1->getImg(), _pDouble2->get(), (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg(), _pDouble1->getSize(), 1);
    }
    else //if(bComplex1 == true && bComplex2 == true)
    {
        iDotMultiplyComplexMatrixByComplexMatrix(_pDouble1->get(), _pDouble1->getImg(), _pDouble2->get(), _pDouble2->getImg(), (*_pDoubleOut)->get(), (*_pDoubleOut)->getImg(), _pDouble1->getSize(), 1);
    }

    return 0;
}
int MultiplyDoubleByPoly(Double* _pDouble, Polynom* _pPoly, Polynom** _pPolyOut)
{
    bool bComplex1  = _pDouble->isComplex();
    bool bComplex2  = _pPoly->isComplex();
    bool bScalar1   = _pDouble->isScalar();
    bool bScalar2   = _pPoly->isScalar();

    if (_pDouble->isScalar())
    {
        int* piRank = new int[_pPoly->getSize()];
        for (int i = 0 ; i < _pPoly->getSize() ; i++)
        {
            piRank[i] = _pPoly->get(i)->getRank();
        }

        (*_pPolyOut) = new Polynom(_pPoly->getVariableName(), _pPoly->getDims(), _pPoly->getDimsArray(), piRank);
        delete[] piRank;
        if (bComplex1 || bComplex2)
        {
            (*_pPolyOut)->setComplex(true);
        }

        for (int i = 0 ; i < _pPoly->getSize() ; i++)
        {
            SinglePoly *pPolyIn     = _pPoly->get(i);
            double* pRealIn         = pPolyIn->get();
            double* pImgIn          = pPolyIn->getImg();

            SinglePoly *pPolyOut    = (*_pPolyOut)->get(i);
            double* pRealOut        = pPolyOut->get();
            double* pImgOut         = pPolyOut->getImg();

            if (bComplex1 == false && bComplex2 == false)
            {
                iMultiRealScalarByRealMatrix(_pDouble->get(0), pRealIn, 1, pPolyIn->getSize(), pRealOut);
            }
            else if (bComplex1 == false && bComplex2 == true)
            {
                iMultiRealScalarByComplexMatrix(_pDouble->get(0), pRealIn, pImgIn, 1, pPolyIn->getSize(), pRealOut, pImgOut);
            }
            else if (bComplex1 == true && bComplex2 == false)
            {
                iMultiComplexScalarByRealMatrix(_pDouble->get(0), _pDouble->getImg(0), pRealIn, 1, pPolyIn->getSize(), pRealOut, pImgOut);
            }
            else if (bComplex1 == true && bComplex2 == true)
            {
                iMultiComplexScalarByComplexMatrix(_pDouble->get(0), _pDouble->getImg(0), pRealIn, pImgIn, 1, pPolyIn->getSize(), pRealOut, pImgOut);
            }
        }
        (*_pPolyOut)->updateRank();
        return 0;
    }

    if (_pPoly->isScalar())
    {
        int* piRank = new int[_pDouble->getSize()];
        for (int i = 0 ; i < _pDouble->getSize() ; i++)
        {
            piRank[i] = _pPoly->get(0)->getRank();
        }

        (*_pPolyOut) = new Polynom(_pPoly->getVariableName(), _pDouble->getDims(), _pDouble->getDimsArray(), piRank);
        delete[] piRank;
        if (bComplex1 || bComplex2)
        {
            (*_pPolyOut)->setComplex(true);
        }

        double *pDoubleR    = _pDouble->get();
        double *pDoubleI    = _pDouble->getImg();

        SinglePoly *pPolyIn = _pPoly->get(0);
        double* pRealIn     = pPolyIn->get();
        double* pImgIn      = pPolyIn->getImg();

        for (int i = 0 ; i < _pDouble->getSize() ; i++)
        {
            SinglePoly *pPolyOut    = (*_pPolyOut)->get(i);
            double* pRealOut        = pPolyOut->get();
            double* pImgOut         = pPolyOut->getImg();

            if (bComplex1 == false && bComplex2 == false)
            {
                iMultiRealScalarByRealMatrix(pDoubleR[i], pRealIn, 1, pPolyIn->getSize(), pRealOut);
            }
            else if (bComplex1 == false && bComplex2 == true)
            {
                iMultiRealScalarByComplexMatrix(pDoubleR[i], pRealIn, pImgIn, 1, pPolyIn->getSize(), pRealOut, pImgOut);
            }
            else if (bComplex1 == true && bComplex2 == false)
            {
                iMultiComplexScalarByRealMatrix(pDoubleR[i], pDoubleI[i], pRealIn, 1, pPolyIn->getSize(), pRealOut, pImgOut);
            }
            else if (bComplex1 == true && bComplex2 == true)
            {
                iMultiComplexScalarByComplexMatrix(pDoubleR[i], pDoubleI[i], pRealIn, pImgIn, 1, pPolyIn->getSize(), pRealOut, pImgOut);
            }
        }

        (*_pPolyOut)->updateRank();
        return 0;
    }

    if (_pPoly->getDims() > 2 || _pDouble->getDims() > 2 || _pDouble->getCols() != _pPoly->getRows())
    {
        return 1;
    }

    int* piRank = new int[_pDouble->getRows() * _pPoly->getCols()];
    int iMaxRank = _pPoly->getMaxRank();
    for (int i = 0 ; i < _pDouble->getRows() * _pPoly->getCols() ; i++)
    {
        piRank[i] = iMaxRank;
    }

    (*_pPolyOut) = new Polynom(_pPoly->getVariableName(), _pDouble->getRows(), _pPoly->getCols(), piRank);
    delete[] piRank;
    if (bComplex1 || bComplex2)
    {
        (*_pPolyOut)->setComplex(true);
    }

    Double *pCoef = _pPoly->getCoef();
    Double *pTemp = new Double(_pDouble->getRows(), pCoef->getCols(), bComplex1 || bComplex2);

    if (bComplex1 == false && bComplex2 == false)
    {
        iMultiRealMatrixByRealMatrix(_pDouble->get(), _pDouble->getRows(), _pDouble->getCols(),
                                     pCoef->get(), pCoef->getRows(), pCoef->getCols(),
                                     pTemp->get());
    }
    else if (bComplex1 == false && bComplex2 == true)
    {
        iMultiRealMatrixByComplexMatrix(_pDouble->get(), _pDouble->getRows(), _pDouble->getCols(),
                                        pCoef->get(), pCoef->getImg(), pCoef->getRows(), pCoef->getCols(),
                                        pTemp->get(), pTemp->getImg());

    }
    else if (bComplex1 == true && bComplex2 == false)
    {
        iMultiComplexMatrixByRealMatrix(_pDouble->get(), _pDouble->getImg(), _pDouble->getRows(), _pDouble->getCols(),
                                        pCoef->get(), pCoef->getRows(), pCoef->getCols(),
                                        pTemp->get(), pTemp->getImg());
    }
    else //if(bComplex1 == true && bComplex2 == true)
    {
        iMultiComplexMatrixByComplexMatrix(_pDouble->get(), _pDouble->getImg(), _pDouble->getRows(), _pDouble->getCols(),
                                           pCoef->get(), pCoef->getImg(), pCoef->getRows(), pCoef->getCols(),
                                           pTemp->get(), pTemp->getImg());
    }

    (*_pPolyOut)->setCoef(pTemp);
    (*_pPolyOut)->updateRank();
    delete pTemp;
    return 0;
}

int MultiplyPolyByDouble(Polynom* _pPoly, Double* _pDouble, Polynom **_pPolyOut)
{
    bool bComplex1  = _pPoly->isComplex();
    bool bComplex2  = _pDouble->isComplex();
    bool bScalar1   = _pPoly->isScalar();
    bool bScalar2   = _pDouble->isScalar();

    if (bScalar1)
    {
        int* piRank = new int[_pDouble->getSize()];
        for (int i = 0 ; i < _pDouble->getSize() ; i++)
        {
            piRank[i] = _pPoly->get(0)->getRank();
        }

        (*_pPolyOut) = new Polynom(_pPoly->getVariableName(), _pDouble->getDims(), _pDouble->getDimsArray(), piRank);
        delete[] piRank;
        if (bComplex1 || bComplex2)
        {
            (*_pPolyOut)->setComplex(true);
        }

        double *pDoubleR    = _pDouble->get();
        double *pDoubleI    = _pDouble->getImg();

        SinglePoly *pPolyIn = _pPoly->get(0);
        double* pRealIn     = pPolyIn->get();
        double* pImgIn      = pPolyIn->getImg();

        for (int i = 0 ; i < _pDouble->getSize() ; i++)
        {
            SinglePoly *pPolyOut    = (*_pPolyOut)->get(i);
            double* pRealOut        = pPolyOut->get();
            double* pImgOut         = pPolyOut->getImg();

            if (bComplex1 == false && bComplex2 == false)
            {
                iMultiRealScalarByRealMatrix(pDoubleR[i], pRealIn, 1, pPolyIn->getSize(), pRealOut);
            }
            else if (bComplex1 == false && bComplex2 == true)
            {
                iMultiComplexScalarByRealMatrix(pDoubleR[i], pDoubleI[i], pRealIn, 1, pPolyIn->getSize(), pRealOut, pImgOut);
            }
            else if (bComplex1 == true && bComplex2 == false)
            {
                iMultiRealScalarByComplexMatrix(pDoubleR[i], pRealIn, pImgIn, 1, pPolyIn->getSize(), pRealOut, pImgOut);
            }
            else if (bComplex1 == true && bComplex2 == true)
            {
                iMultiComplexScalarByComplexMatrix(pDoubleR[i], pDoubleI[i], pRealIn, pImgIn, 1, pPolyIn->getSize(), pRealOut, pImgOut);
            }
        }

        (*_pPolyOut)->updateRank();
        return 0;
    }
    else if (bScalar2)
    {
        int* piRank = new int[_pPoly->getSize()];
        for (int i = 0 ; i < _pPoly->getSize() ; i++)
        {
            piRank[i] = _pPoly->get(i)->getRank();
        }

        (*_pPolyOut) = new Polynom(_pPoly->getVariableName(), _pPoly->getDims(), _pPoly->getDimsArray(), piRank);
        delete[] piRank;
        if (bComplex1 || bComplex2)
        {
            (*_pPolyOut)->setComplex(true);
        }

        for (int i = 0 ; i < _pPoly->getSize() ; i++)
        {
            SinglePoly *pPolyIn = _pPoly->get(i);
            double* pRealIn     = pPolyIn->get();
            double* pImgIn      = pPolyIn->getImg();

            SinglePoly *pPolyOut    = (*_pPolyOut)->get(i);
            double* pRealOut        = pPolyOut->get();
            double* pImgOut         = pPolyOut->getImg();

            if (bComplex1 == false && bComplex2 == false)
            {
                iMultiRealScalarByRealMatrix(_pDouble->get(0), pRealIn, 1, pPolyIn->getSize(), pRealOut);
            }
            else if (bComplex1 == false && bComplex2 == true)
            {
                iMultiComplexScalarByRealMatrix(_pDouble->get(0), _pDouble->getImg(0), pRealIn, 1, pPolyIn->getSize(), pRealOut, pImgOut);
            }
            else if (bComplex1 == true && bComplex2 == false)
            {
                iMultiRealScalarByComplexMatrix(_pDouble->get(0), pRealIn, pImgIn, 1, pPolyIn->getSize(), pRealOut, pImgOut);
            }
            else if (bComplex1 == true && bComplex2 == true)
            {
                iMultiComplexScalarByComplexMatrix(_pDouble->get(0), _pDouble->getImg(0), pRealIn, pImgIn, 1, pPolyIn->getSize(), pRealOut, pImgOut);
            }
        }

        (*_pPolyOut)->updateRank();
        return 0;
    }

    if (_pDouble->getDims() > 2 || _pPoly->getDims() > 2 || _pPoly->getCols() != _pDouble->getRows())
    {
        return 1;
    }

    int* piRank = new int[_pPoly->getRows() * _pDouble->getCols()];
    int iMaxRank = _pPoly->getMaxRank();
    for (int i = 0 ; i < _pPoly->getRows() * _pDouble->getCols() ; i++)
    {
        piRank[i] = iMaxRank;
    }

    (*_pPolyOut) = new Polynom(_pPoly->getVariableName(), _pPoly->getRows(), _pDouble->getCols(), piRank);
    delete[] piRank;
    if (bComplex1 || bComplex2)
    {
        (*_pPolyOut)->setComplex(true);
    }

    //Distribution a la mano par appels a des sous-fonctions ( iMulti...ScalarBy...Scalar ) plus iAdd...To... )

    //for each line of _pPoly
    for (int iRow1 = 0 ; iRow1 < _pPoly->getRows() ; iRow1++)
    {
        //for each col of _pDouble
        for (int iCol2 = 0 ; iCol2 < _pDouble->getCols() ; iCol2++)
        {
            SinglePoly* pSPOut = (*_pPolyOut)->get(iRow1, iCol2);
            pSPOut->setZeros();

            //for each rows of _pDouble / cols of _pPoly
            for (int iRow2 = 0 ; iRow2 < _pDouble->getRows() ; iRow2++)
            {
                // SinglePoly(iRow1, iRow2) * Double(iRow2, iCol2)
                SinglePoly* pSPIn = _pPoly->get(iRow1, iRow2);
                int iSize = pSPIn->getSize();
                double* pdblMult = new double[iSize];

                if (bComplex1 == false && bComplex2 == false)
                {
                    //Real Matrix by Real Scalar
                    iMultiRealScalarByRealMatrix(_pDouble->get(iRow2, iCol2), pSPIn->get(), iSize, 1, pdblMult);
                    add(pSPOut->get(), (long long)iSize, pdblMult, pSPOut->get());
                }
                else if (bComplex1 == false && bComplex2 == true)
                {
                    //Real Matrix by Scalar Complex
                    double* pdblMultImg = new double[iSize];
                    iMultiComplexScalarByRealMatrix(_pDouble->get(iRow2, iCol2), _pDouble->getImg(iRow2, iCol2), pSPIn->get(), pSPIn->getSize(), 1, pdblMult, pdblMultImg);
                    add(pSPOut->get(), pSPOut->getImg(), (long long)iSize, pdblMult, pdblMultImg, pSPOut->get(), pSPOut->getImg());
                    delete[] pdblMultImg;
                }
                else if (bComplex1 == true && bComplex2 == false)
                {
                    double* pdblMultImg = new double[iSize];
                    iMultiRealScalarByComplexMatrix(_pDouble->get(iRow2, iCol2), pSPIn->get(), pSPIn->getImg(), pSPIn->getSize(), 1, pdblMult, pdblMultImg);
                    add(pSPOut->get(), pSPOut->getImg(), (long long)iSize, pdblMult, pdblMultImg, pSPOut->get(), pSPOut->getImg());
                    delete[] pdblMultImg;
                }
                else //if(bComplex1 == true && bComplex2 == true)
                {
                    double* pdblMultImg = new double[iSize];
                    iMultiComplexScalarByComplexMatrix(_pDouble->get(iRow2, iCol2), _pDouble->getImg(iRow2, iCol2), pSPIn->get(), pSPIn->getImg(), pSPIn->getSize(), 1, pdblMult, pdblMultImg);
                    add(pSPOut->get(), pSPOut->getImg(), (long long)iSize, pdblMult, pdblMultImg, pSPOut->get(), pSPOut->getImg());
                    delete[] pdblMultImg;
                }

                delete[] pdblMult;
            }//for(int iRow2 = 0 ; iRow2 < _pDouble->getRows() ; iRow2++)
        }//for(int iCol2 = 0 ; iCol2 < _pDouble->getCols() ; iCol2++)
    }//for(int iRow1 = 0 ; iRow1 < _pPoly->getRows() ; iRow1++)

    (*_pPolyOut)->updateRank();
    return 0;
}

int MultiplyPolyByPoly(Polynom* _pPoly1, Polynom* _pPoly2, Polynom** _pPolyOut)
{
    bool bComplex1  = _pPoly1->isComplex();
    bool bComplex2  = _pPoly2->isComplex();

    if (_pPoly1->isScalar() && _pPoly2->isScalar())
    {
        //poly1(0) * poly2(0)
        int iRank = _pPoly1->get(0)->getRank() + _pPoly2->get(0)->getRank();
        (*_pPolyOut) = new Polynom(_pPoly1->getVariableName(), 1, 1, &iRank);
        if (bComplex1 || bComplex2)
        {
            (*_pPolyOut)->setComplex(true);
        }

        if (bComplex1 == false && bComplex2 == false)
        {
            SinglePoly *pPoly1  = _pPoly1->get(0);
            SinglePoly *pPoly2  = _pPoly2->get(0);
            SinglePoly *pPolyOut = (*_pPolyOut)->get(0);

            pPolyOut->setZeros();

            iMultiScilabPolynomByScilabPolynom(
                pPoly1->get(), pPoly1->getSize(),
                pPoly2->get(), pPoly2->getSize(),
                pPolyOut->get(), pPolyOut->getSize());
        }
        else if (bComplex1 == false && bComplex2 == true)
        {
            SinglePoly *pPoly1  = _pPoly1->get(0);
            SinglePoly *pPoly2  = _pPoly2->get(0);
            SinglePoly *pPolyOut = (*_pPolyOut)->get(0);

            pPolyOut->setZeros();

            iMultiScilabPolynomByComplexPoly(
                pPoly1->get(), pPoly1->getSize(),
                pPoly2->get(), pPoly2->getImg(), pPoly2->getSize(),
                pPolyOut->get(), pPolyOut->getImg(), pPolyOut->getSize());
        }
        else if (bComplex1 == true && bComplex2 == false)
        {
            SinglePoly *pPoly1  = _pPoly1->get(0);
            SinglePoly *pPoly2  = _pPoly2->get(0);
            SinglePoly *pPolyOut = (*_pPolyOut)->get(0);

            pPolyOut->setZeros();

            iMultiComplexPolyByScilabPolynom(
                pPoly1->get(), pPoly1->getImg(), pPoly1->getSize(),
                pPoly2->get(), pPoly2->getSize(),
                pPolyOut->get(), pPolyOut->getImg(), pPolyOut->getSize());
        }
        else if (bComplex1 == true && bComplex2 == true)
        {
            SinglePoly *pPoly1   = _pPoly1->get(0);
            SinglePoly *pPoly2   = _pPoly2->get(0);
            SinglePoly *pPolyOut  = (*_pPolyOut)->get(0);

            pPolyOut->setZeros();

            iMultiComplexPolyByComplexPoly(
                pPoly1->get(), pPoly1->getImg(), pPoly1->getSize(),
                pPoly2->get(), pPoly2->getImg(), pPoly2->getSize(),
                pPolyOut->get(), pPolyOut->getImg(), pPolyOut->getSize());
        }

        (*_pPolyOut)->updateRank();
        return 0;
    }

    if (_pPoly1->isScalar())
    {
        //poly1(0) * poly2(n)
        int* piRank = new int[_pPoly2->getSize()];
        for (int i = 0 ; i < _pPoly2->getSize() ; i++)
        {
            piRank[i] = _pPoly1->get(0)->getRank() + _pPoly2->get(i)->getRank();
        }

        (*_pPolyOut) = new Polynom(_pPoly1->getVariableName(), _pPoly2->getDims(), _pPoly2->getDimsArray(), piRank);
        if (bComplex1 || bComplex2)
        {
            (*_pPolyOut)->setComplex(true);
        }
        delete[] piRank;


        SinglePoly *pPoly1  = _pPoly1->get(0);
        if (bComplex1 == false && bComplex2 == false)
        {
            for (int iPoly = 0 ; iPoly < _pPoly2->getSize() ; iPoly++)
            {
                SinglePoly *pPoly2  = _pPoly2->get(iPoly);
                SinglePoly *pPolyOut = (*_pPolyOut)->get(iPoly);

                pPolyOut->setZeros();

                iMultiScilabPolynomByScilabPolynom(
                    pPoly1->get(), pPoly1->getSize(),
                    pPoly2->get(), pPoly2->getSize(),
                    pPolyOut->get(), pPolyOut->getSize());
            }
        }
        else if (bComplex1 == false && bComplex2 == true)
        {
            for (int iPoly = 0 ; iPoly < _pPoly2->getSize() ; iPoly++)
            {
                SinglePoly *pPoly2  = _pPoly2->get(iPoly);
                SinglePoly *pPolyOut = (*_pPolyOut)->get(iPoly);

                pPolyOut->setZeros();

                iMultiScilabPolynomByComplexPoly(
                    pPoly1->get(), pPoly1->getSize(),
                    pPoly2->get(), pPoly2->getImg(), pPoly2->getSize(),
                    pPolyOut->get(), pPolyOut->getImg(), pPolyOut->getSize());
            }
        }
        else if (bComplex1 == true && bComplex2 == false)
        {
            for (int iPoly = 0 ; iPoly < _pPoly2->getSize() ; iPoly++)
            {
                SinglePoly *pPoly2  = _pPoly2->get(iPoly);
                SinglePoly *pPolyOut = (*_pPolyOut)->get(iPoly);

                pPolyOut->setZeros();

                iMultiComplexPolyByScilabPolynom(
                    pPoly1->get(), pPoly1->getImg(), pPoly1->getSize(),
                    pPoly2->get(), pPoly2->getSize(),
                    pPolyOut->get(), pPolyOut->getImg(), pPolyOut->getSize());
            }
        }
        else if (bComplex1 == true && bComplex2 == true)
        {
            for (int iPoly = 0 ; iPoly < _pPoly2->getSize() ; iPoly++)
            {
                SinglePoly *pPoly2   = _pPoly2->get(iPoly);
                SinglePoly *pPolyOut  = (*_pPolyOut)->get(iPoly);

                pPolyOut->setZeros();

                iMultiComplexPolyByComplexPoly(
                    pPoly1->get(), pPoly1->getImg(), pPoly1->getSize(),
                    pPoly2->get(), pPoly2->getImg(), pPoly2->getSize(),
                    pPolyOut->get(), pPolyOut->getImg(), pPolyOut->getSize());
            }
        }

        (*_pPolyOut)->updateRank();
        return 0;
    }

    if (_pPoly2->isScalar())
    {
        //poly1(n) * poly2(0)
        int* piRank = new int[_pPoly1->getSize()];
        for (int i = 0 ; i < _pPoly1->getSize() ; i++)
        {
            piRank[i] = _pPoly2->get(0)->getRank() + _pPoly1->get(i)->getRank();
        }

        (*_pPolyOut) = new Polynom(_pPoly1->getVariableName(), _pPoly1->getDims(), _pPoly1->getDimsArray(), piRank);
        if (bComplex1 || bComplex2)
        {
            (*_pPolyOut)->setComplex(true);
        }
        delete[] piRank;

        SinglePoly *pPoly2  = _pPoly2->get(0);
        if (bComplex1 == false && bComplex2 == false)
        {
            for (int iPoly = 0 ; iPoly < _pPoly1->getSize() ; iPoly++)
            {
                SinglePoly *pPoly1  = _pPoly1->get(iPoly);
                SinglePoly *pPolyOut = (*_pPolyOut)->get(iPoly);

                pPolyOut->setZeros();

                iMultiScilabPolynomByScilabPolynom(
                    pPoly1->get(), pPoly1->getSize(),
                    pPoly2->get(), pPoly2->getSize(),
                    pPolyOut->get(), pPolyOut->getSize());
            }
        }
        else if (bComplex1 == false && bComplex2 == true)
        {
            for (int iPoly = 0 ; iPoly < _pPoly1->getSize() ; iPoly++)
            {
                SinglePoly *pPoly1  = _pPoly1->get(iPoly);
                SinglePoly *pPolyOut = (*_pPolyOut)->get(iPoly);

                pPolyOut->setZeros();

                iMultiScilabPolynomByComplexPoly(
                    pPoly1->get(), pPoly1->getSize(),
                    pPoly2->get(), pPoly2->getImg(), pPoly2->getSize(),
                    pPolyOut->get(), pPolyOut->getImg(), pPolyOut->getSize());
            }
        }
        else if (bComplex1 == true && bComplex2 == false)
        {
            for (int iPoly = 0 ; iPoly < _pPoly1->getSize() ; iPoly++)
            {
                SinglePoly *pPoly1  = _pPoly1->get(iPoly);
                SinglePoly *pPolyOut = (*_pPolyOut)->get(iPoly);

                pPolyOut->setZeros();

                iMultiComplexPolyByScilabPolynom(
                    pPoly1->get(), pPoly1->getImg(), pPoly1->getSize(),
                    pPoly2->get(), pPoly2->getSize(),
                    pPolyOut->get(), pPolyOut->getImg(), pPolyOut->getSize());
            }
        }
        else if (bComplex1 == true && bComplex2 == true)
        {
            for (int iPoly = 0 ; iPoly < _pPoly1->getSize() ; iPoly++)
            {
                SinglePoly *pPoly1   = _pPoly1->get(iPoly);
                SinglePoly *pPolyOut  = (*_pPolyOut)->get(iPoly);

                pPolyOut->setZeros();

                iMultiComplexPolyByComplexPoly(
                    pPoly1->get(), pPoly1->getImg(), pPoly1->getSize(),
                    pPoly2->get(), pPoly2->getImg(), pPoly2->getSize(),
                    pPolyOut->get(), pPolyOut->getImg(), pPolyOut->getSize());
            }
        }

        (*_pPolyOut)->updateRank();
        return 0;
    }

    if (_pPoly1->getDims() > 2 || _pPoly2->getDims() > 2 || _pPoly1->getCols() != _pPoly2->getRows())
    {
        return 1;
    }

    // matrix by matrix
    int* piRank = new int[_pPoly1->getRows() * _pPoly2->getCols()];
    int iMaxRank = _pPoly1->getMaxRank() + _pPoly2->getMaxRank();
    for (int i = 0 ; i < _pPoly1->getRows() * _pPoly2->getCols() ; i++)
    {
        piRank[i] = iMaxRank;
    }

    (*_pPolyOut) = new Polynom(_pPoly1->getVariableName(), _pPoly1->getRows(), _pPoly2->getCols(), piRank);
    if (bComplex1 || bComplex2)
    {
        (*_pPolyOut)->setComplex(true);
    }

    delete[] piRank;


    if (bComplex1 == false && bComplex2 == false)
    {
        double *pReal = NULL;
        SinglePoly *pTemp  = new SinglePoly(&pReal, (*_pPolyOut)->getMaxRank());

        for (int iRow = 0 ; iRow < _pPoly1->getRows() ; iRow++)
        {
            for (int iCol = 0 ; iCol < _pPoly2->getCols() ; iCol++)
            {
                SinglePoly *pResult = (*_pPolyOut)->get(iRow, iCol);
                pResult->setZeros();

                for (int iCommon = 0 ; iCommon < _pPoly1->getCols() ; iCommon++)
                {
                    SinglePoly *pL   = _pPoly1->get(iRow, iCommon);
                    SinglePoly *pR   = _pPoly2->get(iCommon, iCol);

                    pTemp->setZeros();

                    iMultiScilabPolynomByScilabPolynom(
                        pL->get(), pL->getSize(),
                        pR->get(), pR->getSize(),
                        pTemp->get(), pL->getRank() + pR->getRank() + 1);

                    iAddScilabPolynomToScilabPolynom(
                        pResult->get(), pResult->getSize(),
                        pTemp->get(), pResult->getSize(),
                        pResult->get(), pResult->getSize());
                }
            }
        }
    }
    else if (bComplex1 == false && bComplex2 == true)
    {
        double *pReal = NULL;
        double *pImg = NULL;
        SinglePoly *pTemp  = new SinglePoly(&pReal, &pImg, (*_pPolyOut)->getMaxRank());

        for (int iRow = 0 ; iRow < _pPoly1->getRows() ; iRow++)
        {
            for (int iCol = 0 ; iCol < _pPoly2->getCols() ; iCol++)
            {
                SinglePoly *pResult = (*_pPolyOut)->get(iRow, iCol);
                pResult->setZeros();

                for (int iCommon = 0 ; iCommon < _pPoly1->getCols() ; iCommon++)
                {
                    SinglePoly *pL   = _pPoly1->get(iRow, iCommon);
                    SinglePoly *pR   = _pPoly2->get(iCommon, iCol);

                    pTemp->setZeros();

                    iMultiScilabPolynomByComplexPoly(
                        pL->get(), pL->getSize(),
                        pR->get(), pR->getImg(), pR->getSize(),
                        pTemp->get(), pTemp->getImg(), pL->getRank() + pR->getRank() + 1);

                    iAddComplexPolyToComplexPoly(
                        pResult->get(), pResult->getImg(), pResult->getSize(),
                        pTemp->get(), pTemp->getImg(), pResult->getSize(),
                        pResult->get(), pResult->getImg(), pResult->getSize());
                }
            }
        }
    }
    else if (bComplex1 == true && bComplex2 == false)
    {
        double *pReal = NULL;
        double *pImg = NULL;
        SinglePoly *pTemp  = new SinglePoly(&pReal, &pImg, (*_pPolyOut)->getMaxRank());

        for (int iRow = 0 ; iRow < _pPoly1->getRows() ; iRow++)
        {
            for (int iCol = 0 ; iCol < _pPoly2->getCols() ; iCol++)
            {
                SinglePoly *pResult = (*_pPolyOut)->get(iRow, iCol);
                pResult->setZeros();

                for (int iCommon = 0 ; iCommon < _pPoly1->getCols() ; iCommon++)
                {
                    SinglePoly *pL   = _pPoly1->get(iRow, iCommon);
                    SinglePoly *pR   = _pPoly2->get(iCommon, iCol);

                    pTemp->setZeros();

                    iMultiScilabPolynomByComplexPoly(
                        pR->get(), pR->getSize(),
                        pL->get(), pL->getImg(), pL->getSize(),
                        pTemp->get(), pTemp->getImg(), pL->getRank() + pR->getRank() + 1);

                    iAddComplexPolyToComplexPoly(
                        pResult->get(), pResult->getImg(), pResult->getSize(),
                        pTemp->get(), pTemp->getImg(), pResult->getSize(),
                        pResult->get(), pResult->getImg(), pResult->getSize());
                }
            }
        }
    }
    else if (bComplex1 == true && bComplex2 == true)
    {
        double *pReal = NULL;
        double *pImg = NULL;
        SinglePoly *pTemp  = new SinglePoly(&pReal, &pImg, (*_pPolyOut)->getMaxRank());

        for (int iRow = 0 ; iRow < _pPoly1->getRows() ; iRow++)
        {
            for (int iCol = 0 ; iCol < _pPoly2->getCols() ; iCol++)
            {
                SinglePoly *pResult = (*_pPolyOut)->get(iRow, iCol);
                pResult->setZeros();

                for (int iCommon = 0 ; iCommon < _pPoly1->getCols() ; iCommon++)
                {
                    SinglePoly *pL   = _pPoly1->get(iRow, iCommon);
                    SinglePoly *pR   = _pPoly2->get(iCommon, iCol);

                    pTemp->setZeros();

                    iMultiComplexPolyByComplexPoly(
                        pL->get(), pL->getImg(), pL->getSize(),
                        pR->get(), pR->getImg(), pR->getSize(),
                        pTemp->get(), pTemp->getImg(), pL->getRank() + pR->getRank() + 1);

                    iAddComplexPolyToComplexPoly(
                        pResult->get(), pResult->getImg(), pResult->getSize(),
                        pTemp->get(), pTemp->getImg(), pResult->getSize(),
                        pResult->get(), pResult->getImg(), pResult->getSize());
                }
            }
        }
    }
    (*_pPolyOut)->updateRank();

    return 0;
}

int MultiplySparseBySparse(Sparse* _pSparse1, Sparse* _pSparse2, Sparse** _pSparseOut)
{
    if (_pSparse1->isScalar())
    {
        //scalar * sp
        Double* pDbl = NULL;
        if (_pSparse1->isComplex())
        {
            std::complex<double> dbl = _pSparse1->getImg(0, 0);
            pDbl = new Double(dbl.real(), dbl.imag());
        }
        else
        {
            pDbl = new Double(_pSparse1->get(0, 0));
        }

        MultiplyDoubleBySparse(pDbl, _pSparse2, (GenericType**)_pSparseOut);
        delete pDbl;
        return 0;
    }

    if (_pSparse2->isScalar())
    {
        //sp * scalar
        Double* pDbl = NULL;
        if (_pSparse2->isComplex())
        {
            std::complex<double> dbl = _pSparse2->getImg(0, 0);
            pDbl = new Double(dbl.real(), dbl.imag());
        }
        else
        {
            pDbl = new Double(_pSparse2->get(0, 0));
        }

        MultiplySparseByDouble(_pSparse1, pDbl, (GenericType**)_pSparseOut);
        delete pDbl;
        return 0;
    }

    if (_pSparse1->getCols() != _pSparse2->getRows())
    {
        return 1;
    }

    *_pSparseOut = _pSparse1->multiply(*_pSparse2);
    return 0;
}

int MultiplyDoubleBySparse(Double* _pDouble, Sparse *_pSparse, GenericType** _pOut)
{
    //D * SP
    if (_pDouble->isScalar())
    {
        //d * SP -> SP
        Sparse* pOut = NULL;
        if (_pDouble->isComplex())
        {
            std::complex<double> dbl(_pDouble->get(0), _pDouble->getImg(0));
            pOut = _pSparse->multiply(dbl);
        }
        else
        {
            pOut = _pSparse->multiply(_pDouble->get(0));
        }
        *_pOut = pOut;
        return 0;
    }

    if (_pSparse->isScalar())
    {
        //D * sp -> D .* d
        Double* pD = NULL;

        if (_pSparse->isComplex())
        {
            std::complex<double> dbl(_pSparse->getImg(0, 0));
            pD = new Double(dbl.real(), dbl.imag());
        }
        else
        {
            pD = new Double(_pSparse->get(0, 0));
        }

        InternalType* pIT = GenericDotTimes(_pDouble, pD);
        *_pOut = pIT->getAs<GenericType>();
        delete pD;
        return 0;
    }

    if (_pDouble->getCols() != _pSparse->getRows())
    {
        return 1;
    }

    //try to be smart and only compute for non zero values

    //get some information
    int iNonZeros = static_cast<int>(_pSparse->nonZeros());
    int* pRows = new int[iNonZeros * 2];
    _pSparse->outputRowCol(pRows);
    int* pCols = pRows + iNonZeros;
    double* pValR = new double[iNonZeros];
    double* pValI = new double[iNonZeros];
    _pSparse->outputValues(pValR, pValI);

    Double* pOut = new Double(_pDouble->getRows(), _pSparse->getCols(), _pDouble->isComplex() | _pSparse->isComplex());
    pOut->setZeros();

    if (_pDouble->isComplex() == false && _pSparse->isComplex() == false)
    {
        for (int i = 0 ; i < iNonZeros ; i++)
        {
            int iRow = static_cast<int>(pRows[i]) - 1;
            int iCol = static_cast<int>(pCols[i]) - 1;
            double dbl = pValR[i];

            for (int j = 0 ; j < _pDouble->getRows() ; j++)
            {
                double dblVal = _pDouble->get(j, iRow) * dbl;
                pOut->set(j, iCol, pOut->get(j, iCol) + dblVal);
            }
        }
    }
    else if (_pDouble->isComplex() == false && _pSparse->isComplex() == true)
    {
        //a * (b ci) -> ab ac
        for (int i = 0 ; i < iNonZeros ; i++)
        {
            int iRow = static_cast<int>(pRows[i]) - 1;
            int iCol = static_cast<int>(pCols[i]) - 1;
            double dblR = pValR[i];
            double dblI = pValI[i];

            for (int j = 0 ; j < _pDouble->getRows() ; j++)
            {
                double dblValR = _pDouble->get(j, iRow) * dblR;
                double dblValI = _pDouble->get(j, iRow) * dblI;
                pOut->set(j, iCol, pOut->get(j, iCol) + dblValR);
                pOut->setImg(j, iCol, pOut->getImg(j, iCol) + dblValI);
            }
        }
    }
    else if (_pDouble->isComplex() == true && _pSparse->isComplex() == false)
    {
        //(a bi) * c -> ac + bc
        for (int i = 0 ; i < iNonZeros ; i++)
        {
            int iRow = static_cast<int>(pRows[i]) - 1;
            int iCol = static_cast<int>(pCols[i]) - 1;
            double dblR = pValR[i];

            for (int j = 0 ; j < _pDouble->getRows() ; j++)
            {
                double dblValR = _pDouble->get(j, iRow) * dblR;
                double dblValI = _pDouble->getImg(j, iRow) * dblR;
                pOut->set(j, iCol, pOut->get(j, iCol) + dblValR);
                pOut->setImg(j, iCol, pOut->getImg(j, iCol) + dblValI);
            }
        }
    }
    else if (_pDouble->isComplex() == true && _pSparse->isComplex() == true)
    {
        for (int i = 0 ; i < iNonZeros ; i++)
        {
            int iRow = static_cast<int>(pRows[i]) - 1;
            int iCol = static_cast<int>(pCols[i]) - 1;
            double dblR = pValR[i];
            double dblI = pValI[i];

            for (int j = 0 ; j < _pDouble->getRows() ; j++)
            {
                double dblValR = _pDouble->get(j, iRow) * dblR - _pDouble->getImg(j, iRow) * dblI;
                double dblValI = _pDouble->get(j, iRow) * dblI + _pDouble->getImg(j, iRow) * dblR;
                pOut->set(j, iCol, pOut->get(j, iCol) + dblValR);
                pOut->setImg(j, iCol, pOut->getImg(j, iCol) + dblValI);
            }
        }
    }

    *_pOut = pOut;
    delete[] pRows;
    delete[] pValR;
    delete[] pValI;

    return 0;
}

int MultiplySparseByDouble(Sparse *_pSparse, Double*_pDouble, GenericType** _pOut)
{
    if (_pDouble->isScalar())
    {
        //SP * d -> SP
        Sparse* pOut = NULL;
        if (_pDouble->isComplex())
        {
            std::complex<double> dbl(_pDouble->get(0), _pDouble->getImg(0));
            pOut = _pSparse->multiply(dbl);
        }
        else
        {
            pOut = _pSparse->multiply(_pDouble->get(0));
        }
        *_pOut = pOut;
        return 0;
    }

    if (_pSparse->isScalar())
    {
        //D * sp -> D .* d
        Double* pD = NULL;

        if (_pSparse->isComplex())
        {
            std::complex<double> dbl(_pSparse->getImg(0, 0));
            pD = new Double(dbl.real(), dbl.imag());
        }
        else
        {
            pD = new Double(_pSparse->get(0, 0));
        }

        InternalType* pIT = GenericDotTimes(_pDouble, pD);
        *_pOut = pIT->getAs<GenericType>();
        delete pD;
        return 0;
    }

    if (_pSparse->getCols() != _pDouble->getRows())
    {
        return 1;
    }

    //try to be smart and only compute for non zero values

    //get some information
    int iNonZeros = static_cast<int>(_pSparse->nonZeros());
    int* pRows = new int[iNonZeros * 2];
    _pSparse->outputRowCol(pRows);
    int* pCols = pRows + iNonZeros;
    double* pValR = new double[iNonZeros];
    double* pValI = new double[iNonZeros];
    _pSparse->outputValues(pValR, pValI);

    Double* pOut = new Double(_pSparse->getRows(), _pDouble->getCols(), _pDouble->isComplex() | _pSparse->isComplex());
    pOut->setZeros();

    if (_pDouble->isComplex() == false && _pSparse->isComplex() == false)
    {
        for (int i = 0 ; i < iNonZeros ; i++)
        {
            int iRow    = static_cast<int>(pRows[i]) - 1;
            int iCol    = static_cast<int>(pCols[i]) - 1;
            double dbl  = pValR[i];

            for (int j = 0 ; j < _pDouble->getCols() ; j++)
            {
                double dblVal = _pDouble->get(iCol, j) * dbl;
                pOut->set(iRow, j, pOut->get(iRow, j) + dblVal);
            }
        }
    }
    else if (_pDouble->isComplex() == false && _pSparse->isComplex() == true)
    {
        //a * (b ci) -> ab ac
        for (int i = 0 ; i < iNonZeros ; i++)
        {
            int iRow = static_cast<int>(pRows[i]) - 1;
            int iCol = static_cast<int>(pCols[i]) - 1;
            double dblR = pValR[i];
            double dblI = pValI[i];

            for (int j = 0 ; j < _pDouble->getCols() ; j++)
            {
                double dblValR = _pDouble->get(iCol, j) * dblR;
                double dblValI = _pDouble->get(iCol, j) * dblI;
                pOut->set(iRow, j, pOut->get(iRow, j) + dblValR);
                pOut->setImg(iRow, j, pOut->getImg(iRow, j) + dblValI);
            }
        }
    }
    else if (_pDouble->isComplex() == true && _pSparse->isComplex() == false)
    {
        //(a bi) * c -> ac + bc
        for (int i = 0 ; i < iNonZeros ; i++)
        {
            int iRow = static_cast<int>(pRows[i]) - 1;
            int iCol = static_cast<int>(pCols[i]) - 1;
            double dblR = pValR[i];

            for (int j = 0 ; j < _pDouble->getCols() ; j++)
            {
                double dblValR = _pDouble->get(iCol, j) * dblR;
                double dblValI = _pDouble->getImg(iCol, j) * dblR;
                pOut->set(iRow, j, pOut->get(iRow, j) + dblValR);
                pOut->setImg(iRow, j, pOut->getImg(iRow, j) + dblValI);
            }
        }
    }
    else if (_pDouble->isComplex() == true && _pSparse->isComplex() == true)
    {
        for (int i = 0 ; i < iNonZeros ; i++)
        {
            int iRow = static_cast<int>(pRows[i]) - 1;
            int iCol = static_cast<int>(pCols[i]) - 1;
            double dblR = pValR[i];
            double dblI = pValI[i];

            for (int j = 0 ; j < _pDouble->getCols() ; j++)
            {
                double dblValR = _pDouble->get(iCol, j) * dblR - _pDouble->getImg(iCol, j) * dblI;
                double dblValI = _pDouble->get(iCol, j) * dblI + _pDouble->getImg(iCol, j) * dblR;
                pOut->set(iRow, j, pOut->get(iRow, j) + dblValR);
                pOut->setImg(iRow, j, pOut->getImg(iRow, j) + dblValI);
            }
        }
    }

    *_pOut = pOut;
    delete[] pRows;
    delete[] pValR;
    delete[] pValI;

    return 0;
}

int DotMultiplySparseBySparse(Sparse* _pSparse1, Sparse* _pSparse2, Sparse** _pOut)
{
    if (_pSparse1->isScalar() || _pSparse2->isScalar())
    {
        //SP .* sp or sp .* SP
        return MultiplySparseBySparse(_pSparse1, _pSparse2, _pOut);
    }

    if (_pSparse1->getRows() != _pSparse2->getRows() || _pSparse1->getCols() != _pSparse2->getCols())
    {
        return 1;
    }

    *_pOut = _pSparse1->dotMultiply(*_pSparse2);

    return 0;
}

int DotMultiplyDoubleBySparse(Double* _pDouble, Sparse* _pSparse, GenericType**  _pOut)
{
    if (_pDouble->isScalar())
    {
        return MultiplyDoubleBySparse(_pDouble, _pSparse, _pOut);
    }

    if (_pSparse->isScalar())
    {
        return MultiplyDoubleBySparse(_pDouble, _pSparse, _pOut);
    }

    if (_pSparse->getRows() != _pDouble->getRows() || _pSparse->getCols() != _pDouble->getCols())
    {
        return 1;
    }

    Sparse* pOut = new Sparse(_pDouble->getRows(), _pDouble->getCols(), _pSparse->isComplex() || _pDouble->isComplex());
    //get some information
    int iNonZeros = static_cast<int>(_pSparse->nonZeros());
    int* pRows = new int[iNonZeros * 2];
    _pSparse->outputRowCol(pRows);
    int* pCols = pRows + iNonZeros;

    if (_pDouble->isComplex() == false && _pSparse->isComplex() == false)
    {
        for (int i = 0 ; i < iNonZeros ; i++)
        {
            int iRow = static_cast<int>(pRows[i]) - 1;
            int iCol = static_cast<int>(pCols[i]) - 1;
            pOut->set(iRow, iCol, _pSparse->get(iRow, iCol) * _pDouble->get(iRow, iCol));
        }
    }
    else if (_pDouble->isComplex() == false && _pSparse->isComplex() == true)
    {
        for (int i = 0 ; i < iNonZeros ; i++)
        {
            int iRow = static_cast<int>(pRows[i]) - 1;
            int iCol = static_cast<int>(pCols[i]) - 1;
            std::complex<double> dbl = _pSparse->getImg(iRow, iCol);
            std::complex<double> newVal(dbl.real() * _pDouble->get(iRow, iCol), dbl.imag() * _pDouble->get(iRow, iCol));
            pOut->set(iRow, iCol, newVal);
        }
    }
    else if (_pDouble->isComplex() == true && _pSparse->isComplex() == false)
    {
        for (int i = 0 ; i < iNonZeros ; i++)
        {
            int iRow = static_cast<int>(pRows[i]) - 1;
            int iCol = static_cast<int>(pCols[i]) - 1;
            std::complex<double> dbl = _pSparse->getImg(iRow, iCol);
            std::complex<double> newVal(dbl.real() * _pDouble->get(iRow, iCol), dbl.real() * _pDouble->getImg(iRow, iCol));
            pOut->set(iRow, iCol, newVal);
        }
    }
    else if (_pDouble->isComplex() == true && _pSparse->isComplex() == true)
    {
        for (int i = 0 ; i < iNonZeros ; i++)
        {
            int iRow = static_cast<int>(pRows[i]) - 1;
            int iCol = static_cast<int>(pCols[i]) - 1;
            std::complex<double> dbl = _pSparse->getImg(iRow, iCol);
            double dblR = _pDouble->get(iRow, iCol) * dbl.real() - _pDouble->getImg(iRow, iCol) * dbl.imag();
            double dblI = _pDouble->getImg(iRow, iCol) * dbl.real() + _pDouble->get(iRow, iCol) * dbl.imag();

            std::complex<double> newVal(dblR, dblI);
            pOut->set(iRow, iCol, newVal);
        }
    }

    *_pOut = pOut;
    delete[] pRows;

    return 0;
}

int DotMultiplySparseByDouble(Sparse* _pSparse, Double* _pDouble, GenericType** _pOut)
{
    return DotMultiplyDoubleBySparse(_pDouble, _pSparse, _pOut);
}

int DotMultiplyPolyByDouble(Polynom* _pPoly, Double* _pDouble, Polynom** _pPolyOut)
{
    return DotMultiplyDoubleByPoly(_pDouble, _pPoly, _pPolyOut);
}

int DotMultiplyDoubleByPoly(Double* _pDouble, Polynom* _pPoly, Polynom** _pPolyOut)
{
    int iSize = _pDouble->getSize();
    if (_pDouble->isScalar() == false &&
            _pPoly->isScalar() == false &&
            iSize != _pPoly->getSize())
    {
        return 1;
    }

    int* piRanks = new int[iSize];
    memset(piRanks, 0x00, iSize * sizeof(int));
    Polynom* pPolyTemp = new Polynom(_pPoly->getVariableName(), _pDouble->getDims(), _pDouble->getDimsArray(), piRanks);
    delete[] piRanks;
    pPolyTemp->setCoef(_pDouble);
    int iErr = DotMultiplyPolyByPoly(pPolyTemp, _pPoly, _pPolyOut);
    delete pPolyTemp;
    return iErr;
}

int DotMultiplyPolyByPoly(Polynom* _pPoly1, Polynom* _pPoly2, Polynom** _pPolyOut)
{
    if (_pPoly1->isScalar() || _pPoly2->isScalar())
    {
        return MultiplyPolyByPoly(_pPoly1, _pPoly2, _pPolyOut);
    }
    else
    {
        if (_pPoly1->getSize() != _pPoly2->getSize())
        {
            return 1;
        }

        int* piRank = new int[_pPoly1->getSize()];
        for (int i = 0 ; i < _pPoly1->getSize() ; i++)
        {
            piRank[i] = _pPoly1->get(i)->getRank() + _pPoly2->get(i)->getRank();
        }

        (*_pPolyOut) = new Polynom(_pPoly1->getVariableName(), _pPoly1->getDims(), _pPoly1->getDimsArray(), piRank);

        if (_pPoly1->isComplex() && _pPoly2->isComplex())
        {
            (*_pPolyOut)->setComplex(true);
            for (int i = 0; i < _pPoly1->getSize(); i++)
            {
                SinglePoly *pSP1    = _pPoly1->get(i);
                SinglePoly *pSP2    = _pPoly2->get(i);
                SinglePoly *pSPOut  = (*_pPolyOut)->get(i);

                pSPOut->setZeros();

                iMultiComplexPolyByComplexPoly(
                    pSP1->get(), pSP1->getImg(), pSP1->getSize(),
                    pSP2->get(), pSP2->getImg(), pSP2->getSize(),
                    pSPOut->get(), pSPOut->getImg(), pSPOut->getSize());

            }
        }
        else if (_pPoly1->isComplex())
        {
            (*_pPolyOut)->setComplex(true);
            for (int i = 0; i < _pPoly1->getSize(); i++)
            {
                SinglePoly *pSP1   = _pPoly1->get(i);
                SinglePoly *pSP2   = _pPoly2->get(i);
                SinglePoly *pSPOut = (*_pPolyOut)->get(i);

                pSPOut->setZeros();

                iMultiComplexPolyByScilabPolynom(
                    pSP1->get(), pSP1->getImg(), pSP1->getSize(),
                    pSP2->get(), pSP2->getSize(),
                    pSPOut->get(), pSPOut->getImg(), pSPOut->getSize());
            }
        }
        else if (_pPoly2->isComplex())
        {
            (*_pPolyOut)->setComplex(true);
            for (int i = 0; i < _pPoly1->getSize(); i++)
            {
                SinglePoly *pSP1   = _pPoly1->get(i);
                SinglePoly *pSP2   = _pPoly2->get(i);
                SinglePoly *pSPOut = (*_pPolyOut)->get(i);

                pSPOut->setZeros();

                iMultiScilabPolynomByComplexPoly(
                    pSP1->get(), pSP1->getSize(),
                    pSP2->get(), pSP2->getImg(), pSP2->getSize(),
                    pSPOut->get(), pSPOut->getImg(), pSPOut->getSize());
            }
        }
        else
        {
            for (int i = 0; i < _pPoly1->getSize(); i++)
            {
                SinglePoly *pSP1   = _pPoly1->get(i);
                SinglePoly *pSP2   = _pPoly2->get(i);
                SinglePoly *pSPOut = (*_pPolyOut)->get(i);

                pSPOut->setZeros();

                iMultiScilabPolynomByScilabPolynom(
                    pSP1->get(), pSP1->getSize(),
                    pSP2->get(), pSP2->getSize(),
                    pSPOut->get(), pSPOut->getSize());
            }
        }
    }

    return 0;
}

//Matrix .* x
template<class T, class U, class O>
InternalType* dotmul_M_M(T *_pL, U *_pR)
{
    //check dims
    int iDimsL = _pL->getDims();
    int iDimsR = _pR->getDims();

    if (iDimsL != iDimsR)
    {
        throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
    }

    int* piDimsL = _pL->getDimsArray();
    int* piDimsR = _pR->getDimsArray();

    for (int i = 0 ; i < iDimsL ; ++i)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }
    }

    O* pOut = new O(iDimsL, piDimsL);
    int iSize = pOut->getSize();

    dotmul(_pL->get(), iSize, _pR->get(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotmul_M_MC(T *_pL, U *_pR)
{
    //check dims
    int iDimsL = _pL->getDims();
    int iDimsR = _pR->getDims();

    if (iDimsL != iDimsR)
    {
        throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
    }

    int* piDimsL = _pL->getDimsArray();
    int* piDimsR = _pR->getDimsArray();

    for (int i = 0 ; i < iDimsL ; ++i)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }
    }

    O* pOut = new O(iDimsL, piDimsL, true);
    int iSize = pOut->getSize();

    dotmul(_pL->get(), iSize, _pR->get(), _pR->getImg(), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotmul_M_S(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    dotmul(_pL->get(), (long long)pOut->getSize(), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotmul_M_SC(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray(), true);
    dotmul(_pL->get(), pOut->getSize(), _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotmul_M_I(T *_pL, U *_pR)
{
    return NULL;
}

template<class T, class U, class O>
InternalType* dotmul_M_IC(T *_pL, U *_pR)
{
    return NULL;
}

template<class T, class U, class O>
InternalType* dotmul_M_E(T *_pL, U *_pR)
{
    return _pR;
}

//Matrix complex .* x
template<class T, class U, class O>
InternalType* dotmul_MC_M(T *_pL, U *_pR)
{
    //check dims
    int iDimsL = _pL->getDims();
    int iDimsR = _pR->getDims();

    if (iDimsL != iDimsR)
    {
        throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
    }

    int* piDimsL = _pL->getDimsArray();
    int* piDimsR = _pR->getDimsArray();

    for (int i = 0 ; i < iDimsL ; ++i)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }
    }

    O* pOut = new O(iDimsL, piDimsL, true);
    int iSize = pOut->getSize();

    dotmul(_pL->get(), _pL->getImg(), iSize, _pR->get(), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotmul_MC_MC(T *_pL, U *_pR)
{
    //check dims
    int iDimsL = _pL->getDims();
    int iDimsR = _pR->getDims();

    if (iDimsL != iDimsR)
    {
        throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
    }

    int* piDimsL = _pL->getDimsArray();
    int* piDimsR = _pR->getDimsArray();

    for (int i = 0 ; i < iDimsL ; ++i)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }
    }

    O* pOut = new O(iDimsL, piDimsL, true);
    int iSize = pOut->getSize();

    dotmul(_pL->get(), _pL->getImg(), iSize, _pR->get(), _pR->getImg(), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotmul_MC_S(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray(), true);
    dotmul(_pL->get(), _pL->getImg(), pOut->getSize(), _pR->get(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotmul_MC_SC(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray(), true);
    dotmul(_pL->get(), _pL->getImg(), pOut->getSize(), _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotmul_MC_I(T *_pL, U *_pR)
{
    return NULL;
}

template<class T, class U, class O>
InternalType* dotmul_MC_IC(T *_pL, U *_pR)
{
    return NULL;
}

//Scalar .* x
template<class T, class U, class O>
InternalType* dotmul_S_M(T *_pL, U *_pR)
{
    return dotmul_M_S<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
InternalType* dotmul_S_MC(T *_pL, U *_pR)
{
    return dotmul_MC_S<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
InternalType* dotmul_S_S(T *_pL, U *_pR)
{
    O* pOut = new O(0);
    dotmul(_pL->get(0), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotmul_S_SC(T *_pL, U *_pR)
{
    O* pOut = new O(0.0, 0.0);
    dotmul(_pL->get(), 1, _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotmul_S_I(T *_pL, U *_pR)
{
    O* pOut = (O*)_pR->clone();
    dotmul(_pL->get(0), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotmul_S_IC(T *_pL, U *_pR)
{
    O* pOut = (O*)_pR->clone();
    dotmul(_pL->get(0), 1, _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

//Scalar complex .* x
template<class T, class U, class O>
InternalType* dotmul_SC_M(T *_pL, U *_pR)
{
    return dotmul_M_SC<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
InternalType* dotmul_SC_MC(T *_pL, U *_pR)
{
    return dotmul_MC_SC<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
InternalType* dotmul_SC_S(T *_pL, U *_pR)
{
    return dotmul_S_SC<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
InternalType* dotmul_SC_SC(T *_pL, U *_pR)
{
    O* pOut = new O(0.0, 0.0);
    dotmul(_pL->get(0), _pL->getImg(0), 1, _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotmul_SC_I(T *_pL, U *_pR)
{
    O* pOut = (O*)_pR->clone();
    pOut->setComplex(true);
    dotmul(_pL->get(0), _pL->getImg(0), 1, _pR->get(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotmul_SC_IC(T *_pL, U *_pR)
{
    O* pOut = (O*)_pR->clone();
    dotmul(_pL->get(0), _pL->getImg(0), 1, _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

//Identity .* x
template<class T, class U, class O>
InternalType* dotmul_I_M(T *_pL, U *_pR)
{
    return NULL;
}

template<class T, class U, class O>
InternalType* dotmul_I_MC(T *_pL, U *_pR)
{
    return NULL;
}

template<class T, class U, class O>
InternalType* dotmul_I_S(T *_pL, U *_pR)
{
    return dotmul_S_I<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
InternalType* dotmul_I_SC(T *_pL, U *_pR)
{
    return dotmul_SC_I<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
InternalType* dotmul_I_I(T *_pL, U *_pR)
{
    O* pOut = (O*)_pL->clone();
    dotmul(_pL->get(0), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotmul_I_IC(T *_pL, U *_pR)
{
    O* pOut = (O*)_pR->clone();
    dotmul(_pL->get(0), 1, _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

//Identity complex .* x
template<class T, class U, class O>
InternalType* dotmul_IC_M(T *_pL, U *_pR)
{
    return NULL;
}

template<class T, class U, class O>
InternalType* dotmul_IC_MC(T *_pL, U *_pR)
{
    return NULL;
}

template<class T, class U, class O>
InternalType* dotmul_IC_S(T *_pL, U *_pR)
{
    return dotmul_S_IC<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
InternalType* dotmul_IC_SC(T *_pL, U *_pR)
{
    return dotmul_SC_IC<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
InternalType* dotmul_IC_I(T *_pL, U *_pR)
{
    O* pOut = (O*)_pL->clone();
    dotmul(_pL->get(0), _pL->getImg(0), 1, _pR->get(0), pOut->get(), pOut->getImg());
    return pOut;
}

template<class T, class U, class O>
InternalType* dotmul_IC_IC(T *_pL, U *_pR)
{
    O* pOut = (O*)_pL->clone();
    dotmul(_pL->get(0), _pL->getImg(0), 1, _pR->get(0), _pR->getImg(0), pOut->get(), pOut->getImg());
    return pOut;
}

//Empty .* x
template<class T, class U, class O>
InternalType* dotmul_E_M(T *_pL, U *_pR)
{
    return _pL;
}

template<>
InternalType* dotmul_M_M<Sparse, Sparse, Sparse>(Sparse* _pL, Sparse* _pR)
{
    return NULL;
}

//[] .* SP
template<>
InternalType* dotmul_M_M<Double, Sparse, Double>(Double* _pL, Sparse* _pR)
{
    return _pL;
}

//SP .* []
template<>
InternalType* dotmul_M_M<Sparse, Double, Double>(Sparse* _pL, Double* _pR)
{
    return _pR;
}

template<>
InternalType* dotmul_M_M<Double, Sparse, Sparse>(Double* _pL, Sparse* _pR)
{
    return NULL;
}

template<>
InternalType* dotmul_M_M<Sparse, Double, Sparse>(Sparse* _pL, Double* _pR)
{
    return NULL;
}

//Polynom
template<>
InternalType* dotmul_M_M<Polynom, Polynom, Polynom>(Polynom* _pL, Polynom* _pR)
{
    return NULL;
}

template<>
InternalType* dotmul_M_M<Polynom, Double, Polynom>(Polynom* _pL, Double* _pR)
{
    return NULL;
}

template<>
InternalType* dotmul_M_M<Double, Polynom, Polynom>(Double* _pL, Polynom* _pR)
{
    Polynom* pOut = NULL;

    bool isComplexL = _pL->isComplex();
    bool isComplexR = _pR->isComplex();
    bool isComplexOut = isComplexL || isComplexR;

    if (_pL->isScalar())
    {
        pOut = (Polynom*)_pR->clone();
        SinglePoly** pSPR = _pR->get();
        SinglePoly** pSP = pOut->get();
        int iSize = pOut->getSize();

        double dblR = _pL->get(0);
        if (isComplexL)
        {
            double dblI = _pL->getImg(0);
            pOut->setComplex(true);
            if (isComplexR)
            {
                for (int i = 0 ; i < iSize ; i++)
                {
                    dotmul(pSPR[i]->get(), pSPR[i]->getImg(), (long long)pSPR[i]->getSize(), dblR, dblI, pSP[i]->get(), pSP[i]->getImg());
                }
            }
            else
            {
                for (int i = 0 ; i < iSize ; i++)
                {
                    dotmul(pSP[i]->get(), (long long)pSP[i]->getSize(), dblR, dblI, pSP[i]->get(), pSP[i]->getImg());
                }
            }
        }
        else
        {
            if (isComplexR)
            {
                for (int i = 0 ; i < iSize ; i++)
                {
                    dotmul(pSP[i]->get(), pSP[i]->getImg(), (long long)pSP[i]->getSize(), dblR, pSP[i]->get(), pSP[i]->getImg());
                }
            }
            else
            {
                //r .* P
                for (int i = 0 ; i < iSize ; i++)
                {
                    dotmul(pSP[i]->get(), (long long)pSP[i]->getSize(), dblR, pSP[i]->get());
                }
            }
        }

        return pOut;
    }

    if (_pR->isComplex())
    {
    }

    //check dims
    int iDimsL = _pL->getDims();
    int iDimsR = _pR->getDims();

    if (iDimsL != iDimsR)
    {
        throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
    }

    int* piDimsL = _pL->getDimsArray();
    int* piDimsR = _pR->getDimsArray();

    for (int i = 0 ; i < iDimsL ; ++i)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }
    }


    return pOut;
}
