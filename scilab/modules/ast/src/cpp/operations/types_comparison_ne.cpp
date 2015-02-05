/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
*  Copyright (C) 2014 - Scilab Enterprises - Sylvain GENIN
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "types_comparison_ne.hxx"
#include "bool.hxx"
#include "double.hxx"
#include "string.hxx"
#include "polynom.hxx"
#include "cell.hxx"
#include "struct.hxx"
#include "sparse.hxx"
#include "int.hxx"
#include "graphichandle.hxx"
#include "mlist.hxx"
#include "macro.hxx"
#include "macrofile.hxx"

using namespace types;

//define arrays on operation functions
static compnoequal_function pComparisonNonEqualfunction[types::InternalType::IdLast][types::InternalType::IdLast] = { NULL };


void fillComparisonNonEqualFunction()
{
#define scilab_fill_comparison_non_equal(id1, id2, func, typeIn1, typeIn2, typeOut) \
    pComparisonNonEqualfunction[types::InternalType::Id ## id1][types::InternalType::Id ## id2] = (compnoequal_function)&compnoequal_##func<typeIn1, typeIn2, typeOut>

    //String == String
    scilab_fill_comparison_non_equal(String, String, M_M, String, String, Bool);
    scilab_fill_comparison_non_equal(String, ScalarString, M_M, String, String, Bool);
    scilab_fill_comparison_non_equal(ScalarString, ScalarString, M_M, String, String, Bool);
    scilab_fill_comparison_non_equal(ScalarString, String, M_M, String, String, Bool);
    //String == Empty
    scilab_fill_comparison_non_equal(Empty, ScalarString, E_M, String, String, Bool);
    scilab_fill_comparison_non_equal(ScalarString, Empty, M_E, String, String, Bool);
    scilab_fill_comparison_non_equal(String, Empty, M_E, String, String, Bool);
    scilab_fill_comparison_non_equal(Empty, String, E_M, String, String, Bool);
    //String == Double
    scilab_fill_comparison_non_equal(Double, ScalarString, M_E, Double, String, Bool);
    scilab_fill_comparison_non_equal(ScalarString, Double, M_E, String, Double, Bool);
    scilab_fill_comparison_non_equal(String, Double, M_E, String, Double, Bool);
    scilab_fill_comparison_non_equal(Double, String, M_E, Double, String, Bool);
    //String == ScalarDouble
    scilab_fill_comparison_non_equal(ScalarDouble, ScalarString, M_E, Double, String, Bool);
    scilab_fill_comparison_non_equal(ScalarString, ScalarDouble, M_E, String, Double, Bool);
    scilab_fill_comparison_non_equal(String, ScalarDouble, M_E, String, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarDouble, String, M_E, Double, String, Bool);
    //String == Int8
    scilab_fill_comparison_non_equal(Int8, ScalarString, M_E, Int8, String, Bool);
    scilab_fill_comparison_non_equal(ScalarString, Int8, M_E, String, Int8, Bool);
    scilab_fill_comparison_non_equal(String, Int8, M_E, String, Int8, Bool);
    scilab_fill_comparison_non_equal(Int8, String, M_E, Int8, String, Bool);
    //String == UInt8
    scilab_fill_comparison_non_equal(UInt8, ScalarString, M_E, UInt8, String, Bool);
    scilab_fill_comparison_non_equal(ScalarString, UInt8, M_E, String, UInt8, Bool);
    scilab_fill_comparison_non_equal(String, UInt8, M_E, String, UInt8, Bool);
    scilab_fill_comparison_non_equal(UInt8, String, M_E, UInt8, String, Bool);
    //String == Int16
    scilab_fill_comparison_non_equal(Int16, ScalarString, M_E, Int16, String, Bool);
    scilab_fill_comparison_non_equal(ScalarString, Int16, M_E, String, Int16, Bool);
    scilab_fill_comparison_non_equal(String, Int16, M_E, String, Int16, Bool);
    scilab_fill_comparison_non_equal(Int16, String, M_E, Int16, String, Bool);
    //String == UInt16
    scilab_fill_comparison_non_equal(UInt16, ScalarString, M_E, UInt16, String, Bool);
    scilab_fill_comparison_non_equal(ScalarString, UInt16, M_E, String, UInt16, Bool);
    scilab_fill_comparison_non_equal(String, UInt16, M_E, String, UInt16, Bool);
    scilab_fill_comparison_non_equal(UInt16, String, M_E, UInt16, String, Bool);
    //String == Int32
    scilab_fill_comparison_non_equal(Int32, ScalarString, M_E, Int32, String, Bool);
    scilab_fill_comparison_non_equal(ScalarString, Int32, M_E, String, Int32, Bool);
    scilab_fill_comparison_non_equal(String, Int32, M_E, String, Int32, Bool);
    scilab_fill_comparison_non_equal(Int32, String, M_E, Int32, String, Bool);
    //String == UInt32
    scilab_fill_comparison_non_equal(UInt32, ScalarString, M_E, UInt32, String, Bool);
    scilab_fill_comparison_non_equal(ScalarString, UInt32, M_E, String, UInt32, Bool);
    scilab_fill_comparison_non_equal(String, UInt32, M_E, String, UInt32, Bool);
    scilab_fill_comparison_non_equal(UInt32, String, M_E, UInt32, String, Bool);
    //String == Int64
    scilab_fill_comparison_non_equal(Int64, ScalarString, M_E, Int64, String, Bool);
    scilab_fill_comparison_non_equal(ScalarString, Int64, M_E, String, Int64, Bool);
    scilab_fill_comparison_non_equal(String, Int64, M_E, String, Int64, Bool);
    scilab_fill_comparison_non_equal(Int64, String, M_E, Int64, String, Bool);
    //String == UInt64
    scilab_fill_comparison_non_equal(UInt64, ScalarString, M_E, UInt64, String, Bool);
    scilab_fill_comparison_non_equal(ScalarString, UInt64, M_E, String, UInt64, Bool);
    scilab_fill_comparison_non_equal(String, UInt64, M_E, String, UInt64, Bool);
    scilab_fill_comparison_non_equal(UInt64, String, M_E, UInt64, String, Bool);
    //String == Bool
    scilab_fill_comparison_non_equal(Bool, ScalarString, M_E, Bool, String, Bool);
    scilab_fill_comparison_non_equal(ScalarString, Bool, M_E, String, Bool, Bool);
    scilab_fill_comparison_non_equal(String, Bool, M_E, String, Bool, Bool);
    scilab_fill_comparison_non_equal(Bool, String, M_E, Bool, String, Bool);
    //String == Polynom
    scilab_fill_comparison_non_equal(Polynom, ScalarString, M_E, Polynom, String, Bool);
    scilab_fill_comparison_non_equal(ScalarString, Polynom, M_E, String, Polynom, Bool);
    scilab_fill_comparison_non_equal(String, Polynom, M_E, String, Polynom, Bool);
    scilab_fill_comparison_non_equal(Polynom, String, M_E, Polynom, String, Bool);
    //String == PolynomComplex
    scilab_fill_comparison_non_equal(PolynomComplex, ScalarString, M_E, Polynom, String, Bool);
    scilab_fill_comparison_non_equal(ScalarString, PolynomComplex, M_E, String, Polynom, Bool);
    scilab_fill_comparison_non_equal(String, PolynomComplex, M_E, String, Polynom, Bool);
    scilab_fill_comparison_non_equal(PolynomComplex, String, M_E, Polynom, String, Bool);
    //String == ScalarPolynom
    scilab_fill_comparison_non_equal(ScalarPolynom, ScalarString, M_E, Polynom, String, Bool);
    scilab_fill_comparison_non_equal(ScalarString, ScalarPolynom, M_E, String, Polynom, Bool);
    scilab_fill_comparison_non_equal(String, ScalarPolynom, M_E, String, Polynom, Bool);
    scilab_fill_comparison_non_equal(ScalarPolynom, String, M_E, Polynom, String, Bool);
    //String == ScalarPolynomComplex
    scilab_fill_comparison_non_equal(ScalarPolynomComplex, ScalarString, M_E, Polynom, String, Bool);
    scilab_fill_comparison_non_equal(ScalarString, ScalarPolynomComplex, M_E, String, Polynom, Bool);
    scilab_fill_comparison_non_equal(String, ScalarPolynomComplex, M_E, String, Polynom, Bool);
    scilab_fill_comparison_non_equal(ScalarPolynomComplex, String, M_E, Polynom, String, Bool);
    //String == Sparse
    scilab_fill_comparison_non_equal(Sparse, ScalarString, M_E, Sparse, String, Bool);
    scilab_fill_comparison_non_equal(ScalarString, Sparse, M_E, String, Sparse, Bool);
    scilab_fill_comparison_non_equal(String, Sparse, M_E, String, Sparse, Bool);
    scilab_fill_comparison_non_equal(Sparse, String, M_E, Sparse, String, Bool);
    //String == ScalarInt8
    scilab_fill_comparison_non_equal(ScalarInt8, ScalarString, M_E, Int8, String, Bool);
    scilab_fill_comparison_non_equal(ScalarString, ScalarInt8, M_E, String, Int8, Bool);
    scilab_fill_comparison_non_equal(String, ScalarInt8, M_E, String, Int8, Bool);
    scilab_fill_comparison_non_equal(ScalarInt8, String, M_E, Int8, String, Bool);
    //String == ScalarUInt8
    scilab_fill_comparison_non_equal(ScalarUInt8, ScalarString, M_E, UInt8, String, Bool);
    scilab_fill_comparison_non_equal(ScalarString, ScalarUInt8, M_E, String, UInt8, Bool);
    scilab_fill_comparison_non_equal(String, ScalarUInt8, M_E, String, UInt8, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt8, String, M_E, UInt8, String, Bool);
    //String == ScalarInt16
    scilab_fill_comparison_non_equal(ScalarInt16, ScalarString, M_E, Int16, String, Bool);
    scilab_fill_comparison_non_equal(ScalarString, ScalarInt16, M_E, String, Int16, Bool);
    scilab_fill_comparison_non_equal(String, ScalarInt16, M_E, String, Int16, Bool);
    scilab_fill_comparison_non_equal(ScalarInt16, String, M_E, Int16, String, Bool);
    //String == ScalarUInt16
    scilab_fill_comparison_non_equal(ScalarUInt16, ScalarString, M_E, UInt16, String, Bool);
    scilab_fill_comparison_non_equal(ScalarString, ScalarUInt16, M_E, String, UInt16, Bool);
    scilab_fill_comparison_non_equal(String, ScalarUInt16, M_E, String, UInt16, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt16, String, M_E, UInt16, String, Bool);
    //String == ScalarInt32
    scilab_fill_comparison_non_equal(ScalarInt32, ScalarString, M_E, Int32, String, Bool);
    scilab_fill_comparison_non_equal(ScalarString, ScalarInt32, M_E, String, Int32, Bool);
    scilab_fill_comparison_non_equal(String, ScalarInt32, M_E, String, Int32, Bool);
    scilab_fill_comparison_non_equal(ScalarInt32, String, M_E, Int32, String, Bool);
    //String == ScalarUInt32
    scilab_fill_comparison_non_equal(ScalarUInt32, ScalarString, M_E, UInt32, String, Bool);
    scilab_fill_comparison_non_equal(ScalarString, ScalarUInt32, M_E, String, UInt32, Bool);
    scilab_fill_comparison_non_equal(String, ScalarUInt32, M_E, String, UInt32, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt32, String, M_E, UInt32, String, Bool);
    //String == ScalarInt64
    scilab_fill_comparison_non_equal(ScalarInt64, ScalarString, M_E, Int64, String, Bool);
    scilab_fill_comparison_non_equal(ScalarString, ScalarInt64, M_E, String, Int64, Bool);
    scilab_fill_comparison_non_equal(String, ScalarInt64, M_E, String, Int64, Bool);
    scilab_fill_comparison_non_equal(ScalarInt64, String, M_E, Int64, String, Bool);
    //String == ScalarUInt64
    scilab_fill_comparison_non_equal(ScalarUInt64, ScalarString, M_E, UInt64, String, Bool);
    scilab_fill_comparison_non_equal(ScalarString, ScalarUInt64, M_E, String, UInt64, Bool);
    scilab_fill_comparison_non_equal(String, ScalarUInt64, M_E, String, UInt64, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt64, String, M_E, UInt64, String, Bool);
    //String == ScalarBool
    scilab_fill_comparison_non_equal(ScalarBool, ScalarString, M_E, Bool, String, Bool);
    scilab_fill_comparison_non_equal(ScalarString, ScalarBool, M_E, String, Bool, Bool);
    scilab_fill_comparison_non_equal(String, ScalarBool, M_E, String, Bool, Bool);
    scilab_fill_comparison_non_equal(ScalarBool, String, M_E, Bool, String, Bool);
    //String == ScalarPolynom
    scilab_fill_comparison_non_equal(ScalarPolynom, ScalarString, M_E, Polynom, String, Bool);
    scilab_fill_comparison_non_equal(ScalarString, ScalarPolynom, M_E, String, Polynom, Bool);
    scilab_fill_comparison_non_equal(String, ScalarPolynom, M_E, String, Polynom, Bool);
    scilab_fill_comparison_non_equal(ScalarPolynom, String, M_E, Polynom, String, Bool);
    //String == ScalarDoubleComplex
    scilab_fill_comparison_non_equal(ScalarDoubleComplex, ScalarString, M_E, Double, String, Bool);
    scilab_fill_comparison_non_equal(ScalarString, ScalarDoubleComplex, M_E, String, Double, Bool);
    scilab_fill_comparison_non_equal(String, ScalarDoubleComplex, M_E, String, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarDoubleComplex, String, M_E, Double, String, Bool);
    //String == DoubleComplex
    scilab_fill_comparison_non_equal(DoubleComplex, ScalarString, M_E, Double, String, Bool);
    scilab_fill_comparison_non_equal(ScalarString, DoubleComplex, M_E, String, Double, Bool);
    scilab_fill_comparison_non_equal(String, DoubleComplex, M_E, String, Double, Bool);
    scilab_fill_comparison_non_equal(DoubleComplex, String, M_E, Double, String, Bool);
    //String == ScalarPolynomComplex
    scilab_fill_comparison_non_equal(ScalarPolynomComplex, ScalarString, M_E, Polynom, String, Bool);
    scilab_fill_comparison_non_equal(ScalarString, ScalarPolynomComplex, M_E, String, Polynom, Bool);
    scilab_fill_comparison_non_equal(String, ScalarPolynomComplex, M_E, String, Polynom, Bool);
    scilab_fill_comparison_non_equal(ScalarPolynomComplex, String, M_E, Polynom, String, Bool);
    //String == SparseComplex
    scilab_fill_comparison_non_equal(SparseComplex, ScalarString, M_E, Sparse, String, Bool);
    scilab_fill_comparison_non_equal(ScalarString, SparseComplex, M_E, String, Sparse, Bool);
    scilab_fill_comparison_non_equal(String, SparseComplex, M_E, String, Sparse, Bool);
    scilab_fill_comparison_non_equal(SparseComplex, String, M_E, Sparse, String, Bool);
    //String == Identity
    scilab_fill_comparison_non_equal(Identity, ScalarString, M_E, Double, String, Bool);
    scilab_fill_comparison_non_equal(ScalarString, Identity, M_E, String, Double, Bool);
    scilab_fill_comparison_non_equal(String, Identity, M_E, String, Double, Bool);
    scilab_fill_comparison_non_equal(Identity, String, M_E, Double, String, Bool);
    //String == IdentityComplex
    scilab_fill_comparison_non_equal(IdentityComplex, ScalarString, M_E, Double, String, Bool);
    scilab_fill_comparison_non_equal(ScalarString, IdentityComplex, M_E, String, Double, Bool);
    scilab_fill_comparison_non_equal(String, IdentityComplex, M_E, String, Double, Bool);
    scilab_fill_comparison_non_equal(IdentityComplex, String, M_E, Double, String, Bool);

    //Double
    //Matrix == Matrix
    scilab_fill_comparison_non_equal(Double, Double, M_M, Double, Double, Bool);
    scilab_fill_comparison_non_equal(Double, Int8, M_M, Double, Int8, Bool);
    scilab_fill_comparison_non_equal(Double, UInt8, M_M, Double, UInt8, Bool);
    scilab_fill_comparison_non_equal(Double, Int16, M_M, Double, Int16, Bool);
    scilab_fill_comparison_non_equal(Double, UInt16, M_M, Double, UInt16, Bool);
    scilab_fill_comparison_non_equal(Double, Int32, M_M, Double, Int32, Bool);
    scilab_fill_comparison_non_equal(Double, UInt32, M_M, Double, UInt32, Bool);
    scilab_fill_comparison_non_equal(Double, Int64, M_M, Double, Int64, Bool);
    scilab_fill_comparison_non_equal(Double, UInt64, M_M, Double, UInt64, Bool);
    scilab_fill_comparison_non_equal(Double, Bool, M_M, Double, Bool, Bool);
    scilab_fill_comparison_non_equal(Double, Polynom, M_M, Double, Polynom, Bool);
    scilab_fill_comparison_non_equal(Double, Sparse, M_M, Double, Sparse, SparseBool);

    //Matrix == Matrix Complex
    scilab_fill_comparison_non_equal(Double, DoubleComplex, M_MC, Double, Double, Bool);
    scilab_fill_comparison_non_equal(Double, PolynomComplex, M_M, Double, Polynom, Bool);
    scilab_fill_comparison_non_equal(Double, SparseComplex, M_M, Double, Sparse, SparseBool);

    //Matrix == Scalar
    scilab_fill_comparison_non_equal(Double, ScalarDouble, M_S, Double, Double, Bool);
    scilab_fill_comparison_non_equal(Double, ScalarInt8, M_S, Double, Int8, Bool);
    scilab_fill_comparison_non_equal(Double, ScalarUInt8, M_S, Double, UInt8, Bool);
    scilab_fill_comparison_non_equal(Double, ScalarInt16, M_S, Double, Int16, Bool);
    scilab_fill_comparison_non_equal(Double, ScalarUInt16, M_S, Double, UInt16, Bool);
    scilab_fill_comparison_non_equal(Double, ScalarInt32, M_S, Double, Int32, Bool);
    scilab_fill_comparison_non_equal(Double, ScalarUInt32, M_S, Double, UInt32, Bool);
    scilab_fill_comparison_non_equal(Double, ScalarInt64, M_S, Double, Int64, Bool);
    scilab_fill_comparison_non_equal(Double, ScalarUInt64, M_S, Double, UInt64, Bool);
    scilab_fill_comparison_non_equal(Double, ScalarBool, M_S, Double, Bool, Bool);
    scilab_fill_comparison_non_equal(Double, ScalarPolynom, M_M, Double, Polynom, Bool);

    //Matrix == Scalar Complex
    scilab_fill_comparison_non_equal(Double, ScalarDoubleComplex, M_SC, Double, Double, Bool);
    scilab_fill_comparison_non_equal(Double, ScalarPolynomComplex, M_M, Double, Polynom, Bool);

    //Matrix == Empty
    scilab_fill_comparison_non_equal(Double, Empty, M_E, Double, Double, Bool);


    //Matrix Complex == Matrix
    scilab_fill_comparison_non_equal(DoubleComplex, Double, MC_M, Double, Double, Bool);
    scilab_fill_comparison_non_equal(DoubleComplex, DoubleComplex, MC_MC, Double, Double, Bool);
    scilab_fill_comparison_non_equal(DoubleComplex, ScalarDouble, MC_S, Double, Double, Bool);
    scilab_fill_comparison_non_equal(DoubleComplex, ScalarDoubleComplex, MC_SC, Double, Double, Bool);
    scilab_fill_comparison_non_equal(DoubleComplex, Empty, M_E, Double, Double, Bool);
    scilab_fill_comparison_non_equal(DoubleComplex, Polynom, M_M, Double, Polynom, Bool);
    scilab_fill_comparison_non_equal(DoubleComplex, PolynomComplex, M_M, Double, Polynom, Bool);
    scilab_fill_comparison_non_equal(DoubleComplex, ScalarPolynom, M_M, Double, Polynom, Bool);
    scilab_fill_comparison_non_equal(DoubleComplex, ScalarPolynomComplex, M_M, Double, Polynom, Bool);
    scilab_fill_comparison_non_equal(DoubleComplex, Sparse, M_M, Double, Sparse, SparseBool);
    scilab_fill_comparison_non_equal(DoubleComplex, SparseComplex, M_M, Double, Sparse, SparseBool);

    //Scalar == Matrix
    scilab_fill_comparison_non_equal(ScalarDouble, Double, S_M, Double, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarDouble, Int8, S_M, Double, Int8, Bool);
    scilab_fill_comparison_non_equal(ScalarDouble, UInt8, S_M, Double, UInt8, Bool);
    scilab_fill_comparison_non_equal(ScalarDouble, Int16, S_M, Double, Int16, Bool);
    scilab_fill_comparison_non_equal(ScalarDouble, UInt16, S_M, Double, UInt16, Bool);
    scilab_fill_comparison_non_equal(ScalarDouble, Int32, S_M, Double, Int32, Bool);
    scilab_fill_comparison_non_equal(ScalarDouble, UInt32, S_M, Double, UInt32, Bool);
    scilab_fill_comparison_non_equal(ScalarDouble, Int64, S_M, Double, Int64, Bool);
    scilab_fill_comparison_non_equal(ScalarDouble, UInt64, S_M, Double, UInt64, Bool);
    scilab_fill_comparison_non_equal(ScalarDouble, Bool, S_M, Double, Bool, Bool);
    scilab_fill_comparison_non_equal(ScalarDouble, Polynom, M_M, Double, Polynom, Bool);
    scilab_fill_comparison_non_equal(ScalarDouble, Sparse, M_M, Double, Sparse, SparseBool);

    //Scalar == Matrix Complex
    scilab_fill_comparison_non_equal(ScalarDouble, DoubleComplex, S_MC, Double, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarDouble, PolynomComplex, M_M, Double, Polynom, Bool);
    scilab_fill_comparison_non_equal(ScalarDouble, SparseComplex, M_M, Double, Sparse, SparseBool);

    //Scalar == Scalar
    scilab_fill_comparison_non_equal(ScalarDouble, ScalarDouble, S_S, Double, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarDouble, ScalarInt8, S_S, Double, Int8, Bool);
    scilab_fill_comparison_non_equal(ScalarDouble, ScalarUInt8, S_S, Double, UInt8, Bool);
    scilab_fill_comparison_non_equal(ScalarDouble, ScalarInt16, S_S, Double, Int16, Bool);
    scilab_fill_comparison_non_equal(ScalarDouble, ScalarUInt16, S_S, Double, UInt16, Bool);
    scilab_fill_comparison_non_equal(ScalarDouble, ScalarInt32, S_S, Double, Int32, Bool);
    scilab_fill_comparison_non_equal(ScalarDouble, ScalarUInt32, S_S, Double, UInt32, Bool);
    scilab_fill_comparison_non_equal(ScalarDouble, ScalarInt64, S_S, Double, Int64, Bool);
    scilab_fill_comparison_non_equal(ScalarDouble, ScalarUInt64, S_S, Double, UInt64, Bool);
    scilab_fill_comparison_non_equal(ScalarDouble, ScalarBool, S_S, Double, Bool, Bool);
    scilab_fill_comparison_non_equal(ScalarDouble, ScalarPolynom, M_M, Double, Polynom, Bool);

    //Scalar == Scalar Complex
    scilab_fill_comparison_non_equal(ScalarDouble, ScalarDoubleComplex, S_SC, Double, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarDouble, PolynomComplex, M_M, Double, Polynom, Bool);
    scilab_fill_comparison_non_equal(ScalarDouble, ScalarPolynomComplex, M_M, Double, Polynom, Bool);

    //Scalar == Empty
    scilab_fill_comparison_non_equal(ScalarDouble, Empty, M_E, Double, Double, Bool);

    //Scalar Complex == Matrix
    scilab_fill_comparison_non_equal(ScalarDoubleComplex, Double, SC_M, Double, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarDoubleComplex, Polynom, M_M, Double, Polynom, Bool);
    scilab_fill_comparison_non_equal(ScalarDoubleComplex, Sparse, M_M, Double, Sparse, SparseBool);
    //Scalar Complex == Matrix Complex
    scilab_fill_comparison_non_equal(ScalarDoubleComplex, DoubleComplex, SC_MC, Double, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarDoubleComplex, PolynomComplex, M_M, Double, Polynom, Bool);
    scilab_fill_comparison_non_equal(ScalarDoubleComplex, SparseComplex, M_M, Double, Sparse, SparseBool);
    //Scalar Complex == Scalar
    scilab_fill_comparison_non_equal(ScalarDoubleComplex, ScalarDouble, SC_S, Double, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarDoubleComplex, ScalarPolynom, M_M, Double, Polynom, Bool);
    //Scalar Complex == Scalar Complex
    scilab_fill_comparison_non_equal(ScalarDoubleComplex, ScalarDoubleComplex, SC_SC, Double, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarDoubleComplex, ScalarPolynomComplex, M_M, Double, Polynom, Bool);
    //Scalar Complex == Empty
    scilab_fill_comparison_non_equal(ScalarDoubleComplex, Empty, M_E, Double, Double, Bool);

    //Empty == Matrix
    scilab_fill_comparison_non_equal(Empty, Double, E_M, Double, Double, Bool);
    scilab_fill_comparison_non_equal(Empty, Int8, E_M, Double, Int8, Bool);
    scilab_fill_comparison_non_equal(Empty, UInt8, E_M, Double, UInt8, Bool);
    scilab_fill_comparison_non_equal(Empty, Int16, E_M, Double, Int16, Bool);
    scilab_fill_comparison_non_equal(Empty, UInt16, E_M, Double, UInt16, Bool);
    scilab_fill_comparison_non_equal(Empty, Int32, E_M, Double, Int32, Bool);
    scilab_fill_comparison_non_equal(Empty, UInt32, E_M, Double, UInt32, Bool);
    scilab_fill_comparison_non_equal(Empty, Int64, E_M, Double, Int64, Bool);
    scilab_fill_comparison_non_equal(Empty, UInt64, E_M, Double, UInt64, Bool);

    scilab_fill_comparison_non_equal(Empty, Polynom, E_M, Double, Polynom, Bool);
    scilab_fill_comparison_non_equal(Empty, PolynomComplex, E_M, Double, Polynom, Bool);
    scilab_fill_comparison_non_equal(Empty, Sparse, E_M, Double, Sparse, Bool);
    scilab_fill_comparison_non_equal(Empty, SparseComplex, E_M, Double, Sparse, Bool);

    //Empty == Matrix Complex
    scilab_fill_comparison_non_equal(Empty, DoubleComplex, E_M, Double, Double, Bool);
    //Empty == Scalar
    scilab_fill_comparison_non_equal(Empty, ScalarDouble, E_M, Double, Double, Bool);
    scilab_fill_comparison_non_equal(Empty, ScalarInt8, E_M, Double, Int8, Bool);
    scilab_fill_comparison_non_equal(Empty, ScalarUInt8, E_M, Double, UInt8, Bool);
    scilab_fill_comparison_non_equal(Empty, ScalarInt16, E_M, Double, Int16, Bool);
    scilab_fill_comparison_non_equal(Empty, ScalarUInt16, E_M, Double, UInt16, Bool);
    scilab_fill_comparison_non_equal(Empty, ScalarInt32, E_M, Double, Int32, Bool);
    scilab_fill_comparison_non_equal(Empty, ScalarUInt32, E_M, Double, UInt32, Bool);
    scilab_fill_comparison_non_equal(Empty, ScalarInt64, E_M, Double, Int64, Bool);
    scilab_fill_comparison_non_equal(Empty, ScalarUInt64, E_M, Double, UInt64, Bool);
    scilab_fill_comparison_non_equal(Empty, ScalarBool, E_M, Double, Bool, Bool);
    scilab_fill_comparison_non_equal(Empty, ScalarPolynom, E_M, Double, Polynom, Bool);

    //Empty == Scalar Complex
    scilab_fill_comparison_non_equal(Empty, ScalarDoubleComplex, E_M, Double, Double, Bool);
    scilab_fill_comparison_non_equal(Empty, ScalarPolynomComplex, E_M, Double, Polynom, Bool);
    //Empty == Empty
    scilab_fill_comparison_non_equal(Empty, Empty, E_E, Double, Double, Bool);
    //Empty == eye
    scilab_fill_comparison_non_equal(Empty, Identity, E_M, Double, Double, Bool);
    scilab_fill_comparison_non_equal(Empty, IdentityComplex, E_M, Double, Double, Bool);

    //Matrix == Identity
    scilab_fill_comparison_non_equal(Double, Identity, M_I, Double, Double, Bool);
    scilab_fill_comparison_non_equal(Double, IdentityComplex, M_IC, Double, Double, Bool);
    scilab_fill_comparison_non_equal(DoubleComplex, Identity, MC_I, Double, Double, Bool);
    scilab_fill_comparison_non_equal(DoubleComplex, IdentityComplex, MC_IC, Double, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarDouble, Identity, S_I, Double, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarDouble, IdentityComplex, S_IC, Double, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarDoubleComplex, Identity, SC_I, Double, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarDoubleComplex, IdentityComplex, SC_IC, Double, Double, Bool);

    //Int8
    //Matrix == Matrix
    scilab_fill_comparison_non_equal(Int8, Double, M_M, Int8, Double, Bool);
    scilab_fill_comparison_non_equal(Int8, Int8, M_M, Int8, Int8, Bool);
    scilab_fill_comparison_non_equal(Int8, UInt8, M_M, Int8, UInt8, Bool);
    scilab_fill_comparison_non_equal(Int8, Int16, M_M, Int8, Int16, Bool);
    scilab_fill_comparison_non_equal(Int8, UInt16, M_M, Int8, UInt16, Bool);
    scilab_fill_comparison_non_equal(Int8, Int32, M_M, Int8, Int32, Bool);
    scilab_fill_comparison_non_equal(Int8, UInt32, M_M, Int8, UInt32, Bool);
    scilab_fill_comparison_non_equal(Int8, Int64, M_M, Int8, Int64, Bool);
    scilab_fill_comparison_non_equal(Int8, UInt64, M_M, Int8, UInt64, Bool);
    scilab_fill_comparison_non_equal(Int8, Bool, M_M, Int8, Bool, Bool);
    scilab_fill_comparison_non_equal(Int8, Empty, M_E, Int8, Double, Bool);

    //Matrix == Scalar
    scilab_fill_comparison_non_equal(Int8, ScalarDouble, M_S, Int8, Double, Bool);
    scilab_fill_comparison_non_equal(Int8, ScalarInt8, M_S, Int8, Int8, Bool);
    scilab_fill_comparison_non_equal(Int8, ScalarUInt8, M_S, Int8, UInt8, Bool);
    scilab_fill_comparison_non_equal(Int8, ScalarInt16, M_S, Int8, Int16, Bool);
    scilab_fill_comparison_non_equal(Int8, ScalarUInt16, M_S, Int8, UInt16, Bool);
    scilab_fill_comparison_non_equal(Int8, ScalarInt32, M_S, Int8, Int32, Bool);
    scilab_fill_comparison_non_equal(Int8, ScalarUInt32, M_S, Int8, UInt32, Bool);
    scilab_fill_comparison_non_equal(Int8, ScalarInt64, M_S, Int8, Int64, Bool);
    scilab_fill_comparison_non_equal(Int8, ScalarUInt64, M_S, Int8, UInt64, Bool);
    scilab_fill_comparison_non_equal(Int8, ScalarBool, M_S, Int8, Bool, Bool);

    //Scalar == Matrix
    scilab_fill_comparison_non_equal(ScalarInt8, Double, S_M, Int8, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarInt8, Int8, S_M, Int8, Int8, Bool);
    scilab_fill_comparison_non_equal(ScalarInt8, UInt8, S_M, Int8, UInt8, Bool);
    scilab_fill_comparison_non_equal(ScalarInt8, Int16, S_M, Int8, Int16, Bool);
    scilab_fill_comparison_non_equal(ScalarInt8, UInt16, S_M, Int8, UInt16, Bool);
    scilab_fill_comparison_non_equal(ScalarInt8, Int32, S_M, Int8, Int32, Bool);
    scilab_fill_comparison_non_equal(ScalarInt8, UInt32, S_M, Int8, UInt32, Bool);
    scilab_fill_comparison_non_equal(ScalarInt8, Int64, S_M, Int8, Int64, Bool);
    scilab_fill_comparison_non_equal(ScalarInt8, UInt64, S_M, Int8, UInt64, Bool);
    scilab_fill_comparison_non_equal(ScalarInt8, Bool, S_M, Int8, Bool, Bool);
    scilab_fill_comparison_non_equal(ScalarInt8, Empty, M_E, Int8, Double, Bool);

    //Scalar == Scalar
    scilab_fill_comparison_non_equal(ScalarInt8, ScalarDouble, S_S, Int8, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarInt8, ScalarInt8, S_S, Int8, Int8, Bool);
    scilab_fill_comparison_non_equal(ScalarInt8, ScalarUInt8, S_S, Int8, UInt8, Bool);
    scilab_fill_comparison_non_equal(ScalarInt8, ScalarInt16, S_S, Int8, Int16, Bool);
    scilab_fill_comparison_non_equal(ScalarInt8, ScalarUInt16, S_S, Int8, UInt16, Bool);
    scilab_fill_comparison_non_equal(ScalarInt8, ScalarInt32, S_S, Int8, Int32, Bool);
    scilab_fill_comparison_non_equal(ScalarInt8, ScalarUInt32, S_S, Int8, UInt32, Bool);
    scilab_fill_comparison_non_equal(ScalarInt8, ScalarInt64, S_S, Int8, Int64, Bool);
    scilab_fill_comparison_non_equal(ScalarInt8, ScalarUInt64, S_S, Int8, UInt64, Bool);
    scilab_fill_comparison_non_equal(ScalarInt8, ScalarBool, S_S, Int8, Bool, Bool);

    //UInt8
    //Matrix == Matrix
    scilab_fill_comparison_non_equal(UInt8, Double, M_M, UInt8, Double, Bool);
    scilab_fill_comparison_non_equal(UInt8, Int8, M_M, UInt8, Int8, Bool);
    scilab_fill_comparison_non_equal(UInt8, UInt8, M_M, UInt8, UInt8, Bool);
    scilab_fill_comparison_non_equal(UInt8, Int16, M_M, UInt8, Int16, Bool);
    scilab_fill_comparison_non_equal(UInt8, UInt16, M_M, UInt8, UInt16, Bool);
    scilab_fill_comparison_non_equal(UInt8, Int32, M_M, UInt8, Int32, Bool);
    scilab_fill_comparison_non_equal(UInt8, UInt32, M_M, UInt8, UInt32, Bool);
    scilab_fill_comparison_non_equal(UInt8, Int64, M_M, UInt8, Int64, Bool);
    scilab_fill_comparison_non_equal(UInt8, UInt64, M_M, UInt8, UInt64, Bool);
    scilab_fill_comparison_non_equal(UInt8, Bool, M_M, UInt8, Bool, Bool);
    scilab_fill_comparison_non_equal(UInt8, Empty, M_E, UInt8, Double, Bool);

    //Matrix == Scalar
    scilab_fill_comparison_non_equal(UInt8, ScalarDouble, M_S, UInt8, Double, Bool);
    scilab_fill_comparison_non_equal(UInt8, ScalarInt8, M_S, UInt8, Int8, Bool);
    scilab_fill_comparison_non_equal(UInt8, ScalarUInt8, M_S, UInt8, UInt8, Bool);
    scilab_fill_comparison_non_equal(UInt8, ScalarInt16, M_S, UInt8, Int16, Bool);
    scilab_fill_comparison_non_equal(UInt8, ScalarUInt16, M_S, UInt8, UInt16, Bool);
    scilab_fill_comparison_non_equal(UInt8, ScalarInt32, M_S, UInt8, Int32, Bool);
    scilab_fill_comparison_non_equal(UInt8, ScalarUInt32, M_S, UInt8, UInt32, Bool);
    scilab_fill_comparison_non_equal(UInt8, ScalarInt64, M_S, UInt8, Int64, Bool);
    scilab_fill_comparison_non_equal(UInt8, ScalarUInt64, M_S, UInt8, UInt64, Bool);
    scilab_fill_comparison_non_equal(UInt8, ScalarBool, M_S, UInt8, Bool, Bool);

    //Scalar == Matrix
    scilab_fill_comparison_non_equal(ScalarUInt8, Double, S_M, UInt8, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt8, Int8, S_M, UInt8, Int8, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt8, UInt8, S_M, UInt8, UInt8, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt8, Int16, S_M, UInt8, Int16, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt8, UInt16, S_M, UInt8, UInt16, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt8, Int32, S_M, UInt8, Int32, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt8, UInt32, S_M, UInt8, UInt32, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt8, Int64, S_M, UInt8, Int64, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt8, UInt64, S_M, UInt8, UInt64, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt8, Bool, S_M, UInt8, Bool, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt8, Empty, M_E, UInt8, Double, Bool);

    //Scalar == Scalar
    scilab_fill_comparison_non_equal(ScalarUInt8, ScalarDouble, S_S, UInt8, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt8, ScalarInt8, S_S, UInt8, Int8, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt8, ScalarUInt8, S_S, UInt8, UInt8, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt8, ScalarInt16, S_S, UInt8, Int16, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt8, ScalarUInt16, S_S, UInt8, UInt16, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt8, ScalarInt32, S_S, UInt8, Int32, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt8, ScalarUInt32, S_S, UInt8, UInt32, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt8, ScalarInt64, S_S, UInt8, Int64, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt8, ScalarUInt64, S_S, UInt8, UInt64, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt8, ScalarBool, S_S, UInt8, Bool, Bool);

    //Int16
    //Matrix == Matrix
    scilab_fill_comparison_non_equal(Int16, Double, M_M, Int16, Double, Bool);
    scilab_fill_comparison_non_equal(Int16, Int8, M_M, Int16, Int8, Bool);
    scilab_fill_comparison_non_equal(Int16, UInt8, M_M, Int16, UInt8, Bool);
    scilab_fill_comparison_non_equal(Int16, Int16, M_M, Int16, Int16, Bool);
    scilab_fill_comparison_non_equal(Int16, UInt16, M_M, Int16, UInt16, Bool);
    scilab_fill_comparison_non_equal(Int16, Int32, M_M, Int16, Int32, Bool);
    scilab_fill_comparison_non_equal(Int16, UInt32, M_M, Int16, UInt32, Bool);
    scilab_fill_comparison_non_equal(Int16, Int64, M_M, Int16, Int64, Bool);
    scilab_fill_comparison_non_equal(Int16, UInt64, M_M, Int16, UInt64, Bool);
    scilab_fill_comparison_non_equal(Int16, Bool, M_M, Int16, Bool, Bool);
    scilab_fill_comparison_non_equal(Int16, Empty, M_E, Int16, Double, Bool);

    //Matrix == Scalar
    scilab_fill_comparison_non_equal(Int16, ScalarDouble, M_S, Int16, Double, Bool);
    scilab_fill_comparison_non_equal(Int16, ScalarInt8, M_S, Int16, Int8, Bool);
    scilab_fill_comparison_non_equal(Int16, ScalarUInt8, M_S, Int16, UInt8, Bool);
    scilab_fill_comparison_non_equal(Int16, ScalarInt16, M_S, Int16, Int16, Bool);
    scilab_fill_comparison_non_equal(Int16, ScalarUInt16, M_S, Int16, UInt16, Bool);
    scilab_fill_comparison_non_equal(Int16, ScalarInt32, M_S, Int16, Int32, Bool);
    scilab_fill_comparison_non_equal(Int16, ScalarUInt32, M_S, Int16, UInt32, Bool);
    scilab_fill_comparison_non_equal(Int16, ScalarInt64, M_S, Int16, Int64, Bool);
    scilab_fill_comparison_non_equal(Int16, ScalarUInt64, M_S, Int16, UInt64, Bool);
    scilab_fill_comparison_non_equal(Int16, ScalarBool, M_E, Int16, Bool, Bool);

    //Scalar == Matrix
    scilab_fill_comparison_non_equal(ScalarInt16, Double, S_M, Int16, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarInt16, Int8, S_M, Int16, Int8, Bool);
    scilab_fill_comparison_non_equal(ScalarInt16, UInt8, S_M, Int16, UInt8, Bool);
    scilab_fill_comparison_non_equal(ScalarInt16, Int16, S_M, Int16, Int16, Bool);
    scilab_fill_comparison_non_equal(ScalarInt16, UInt16, S_M, Int16, UInt16, Bool);
    scilab_fill_comparison_non_equal(ScalarInt16, Int32, S_M, Int16, Int32, Bool);
    scilab_fill_comparison_non_equal(ScalarInt16, UInt32, S_M, Int16, UInt32, Bool);
    scilab_fill_comparison_non_equal(ScalarInt16, Int64, S_M, Int16, Int64, Bool);
    scilab_fill_comparison_non_equal(ScalarInt16, UInt64, S_M, Int16, UInt64, Bool);
    scilab_fill_comparison_non_equal(ScalarInt16, Bool, S_M, Int16, Bool, Bool);
    scilab_fill_comparison_non_equal(ScalarInt16, Empty, M_E, Int16, Double, Bool);

    //Scalar == Scalar
    scilab_fill_comparison_non_equal(ScalarInt16, ScalarDouble, S_S, Int16, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarInt16, ScalarInt8, S_S, Int16, Int8, Bool);
    scilab_fill_comparison_non_equal(ScalarInt16, ScalarUInt8, S_S, Int16, UInt8, Bool);
    scilab_fill_comparison_non_equal(ScalarInt16, ScalarInt16, S_S, Int16, Int16, Bool);
    scilab_fill_comparison_non_equal(ScalarInt16, ScalarUInt16, S_S, Int16, UInt16, Bool);
    scilab_fill_comparison_non_equal(ScalarInt16, ScalarInt32, S_S, Int16, Int32, Bool);
    scilab_fill_comparison_non_equal(ScalarInt16, ScalarUInt32, S_S, Int16, UInt32, Bool);
    scilab_fill_comparison_non_equal(ScalarInt16, ScalarInt64, S_S, Int16, Int64, Bool);
    scilab_fill_comparison_non_equal(ScalarInt16, ScalarUInt64, S_S, Int16, UInt64, Bool);
    scilab_fill_comparison_non_equal(ScalarInt16, ScalarBool, S_S, Int16, Bool, Bool);

    //UInt16
    //Matrix == Matrix
    scilab_fill_comparison_non_equal(UInt16, Double, M_M, UInt16, Double, Bool);
    scilab_fill_comparison_non_equal(UInt16, Int8, M_M, UInt16, Int8, Bool);
    scilab_fill_comparison_non_equal(UInt16, UInt8, M_M, UInt16, UInt8, Bool);
    scilab_fill_comparison_non_equal(UInt16, Int16, M_M, UInt16, Int16, Bool);
    scilab_fill_comparison_non_equal(UInt16, UInt16, M_M, UInt16, UInt16, Bool);
    scilab_fill_comparison_non_equal(UInt16, Int32, M_M, UInt16, Int32, Bool);
    scilab_fill_comparison_non_equal(UInt16, UInt32, M_M, UInt16, UInt32, Bool);
    scilab_fill_comparison_non_equal(UInt16, Int64, M_M, UInt16, Int64, Bool);
    scilab_fill_comparison_non_equal(UInt16, UInt64, M_M, UInt16, UInt64, Bool);
    scilab_fill_comparison_non_equal(UInt16, Bool, M_M, UInt16, Bool, Bool);
    scilab_fill_comparison_non_equal(UInt16, Empty, M_E, UInt16, Double, Bool);

    //Matrix == Scalar
    scilab_fill_comparison_non_equal(UInt16, ScalarDouble, M_S, UInt16, Double, Bool);
    scilab_fill_comparison_non_equal(UInt16, ScalarInt8, M_S, UInt16, Int8, Bool);
    scilab_fill_comparison_non_equal(UInt16, ScalarUInt8, M_S, UInt16, UInt8, Bool);
    scilab_fill_comparison_non_equal(UInt16, ScalarInt16, M_S, UInt16, Int16, Bool);
    scilab_fill_comparison_non_equal(UInt16, ScalarUInt16, M_S, UInt16, UInt16, Bool);
    scilab_fill_comparison_non_equal(UInt16, ScalarInt32, M_S, UInt16, Int32, Bool);
    scilab_fill_comparison_non_equal(UInt16, ScalarUInt32, M_S, UInt16, UInt32, Bool);
    scilab_fill_comparison_non_equal(UInt16, ScalarInt64, M_S, UInt16, Int64, Bool);
    scilab_fill_comparison_non_equal(UInt16, ScalarUInt64, M_S, UInt16, UInt64, Bool);
    scilab_fill_comparison_non_equal(UInt16, ScalarBool, M_S, UInt16, Bool, Bool);

    //Scalar == Matrix
    scilab_fill_comparison_non_equal(ScalarUInt16, Double, S_M, UInt16, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt16, Int8, S_M, UInt16, Int8, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt16, UInt8, S_M, UInt16, UInt8, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt16, Int16, S_M, UInt16, Int16, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt16, UInt16, S_M, UInt16, UInt16, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt16, Int32, S_M, UInt16, Int32, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt16, UInt32, S_M, UInt16, UInt32, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt16, Int64, S_M, UInt16, Int64, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt16, UInt64, S_M, UInt16, UInt64, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt16, Bool, S_M, UInt16, Bool, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt16, Empty, M_E, UInt16, Double, Bool);

    //Scalar == Scalar
    scilab_fill_comparison_non_equal(ScalarUInt16, ScalarDouble, S_S, UInt16, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt16, ScalarInt8, S_S, UInt16, Int8, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt16, ScalarUInt8, S_S, UInt16, UInt8, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt16, ScalarInt16, S_S, UInt16, Int16, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt16, ScalarUInt16, S_S, UInt16, UInt16, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt16, ScalarInt32, S_S, UInt16, Int32, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt16, ScalarUInt32, S_S, UInt16, UInt32, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt16, ScalarInt64, S_S, UInt16, Int64, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt16, ScalarUInt64, S_S, UInt16, UInt64, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt16, ScalarBool, S_S, UInt16, Bool, Bool);

    //Int32
    //Matrix == Matrix
    scilab_fill_comparison_non_equal(Int32, Double, M_M, Int32, Double, Bool);
    scilab_fill_comparison_non_equal(Int32, Int8, M_M, Int32, Int8, Bool);
    scilab_fill_comparison_non_equal(Int32, UInt8, M_M, Int32, UInt8, Bool);
    scilab_fill_comparison_non_equal(Int32, Int16, M_M, Int32, Int16, Bool);
    scilab_fill_comparison_non_equal(Int32, UInt16, M_M, Int32, UInt16, Bool);
    scilab_fill_comparison_non_equal(Int32, Int32, M_M, Int32, Int32, Bool);
    scilab_fill_comparison_non_equal(Int32, UInt32, M_M, Int32, UInt32, Bool);
    scilab_fill_comparison_non_equal(Int32, Int64, M_M, Int32, Int64, Bool);
    scilab_fill_comparison_non_equal(Int32, UInt64, M_M, Int32, UInt64, Bool);
    scilab_fill_comparison_non_equal(Int32, Bool, M_M, Int32, Bool, Bool);
    scilab_fill_comparison_non_equal(Int32, Empty, M_E, Int32, Double, Bool);

    //Matrix == Scalar
    scilab_fill_comparison_non_equal(Int32, ScalarDouble, M_S, Int32, Double, Bool);
    scilab_fill_comparison_non_equal(Int32, ScalarInt8, M_S, Int32, Int8, Bool);
    scilab_fill_comparison_non_equal(Int32, ScalarUInt8, M_S, Int32, UInt8, Bool);
    scilab_fill_comparison_non_equal(Int32, ScalarInt16, M_S, Int32, Int16, Bool);
    scilab_fill_comparison_non_equal(Int32, ScalarUInt16, M_S, Int32, UInt16, Bool);
    scilab_fill_comparison_non_equal(Int32, ScalarInt32, M_S, Int32, Int32, Bool);
    scilab_fill_comparison_non_equal(Int32, ScalarUInt32, M_S, Int32, UInt32, Bool);
    scilab_fill_comparison_non_equal(Int32, ScalarInt64, M_S, Int32, Int64, Bool);
    scilab_fill_comparison_non_equal(Int32, ScalarUInt64, M_S, Int32, UInt64, Bool);
    scilab_fill_comparison_non_equal(Int32, ScalarBool, M_S, Int32, Bool, Bool);

    //Scalar == Matrix
    scilab_fill_comparison_non_equal(ScalarInt32, Double, S_M, Int32, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarInt32, Int8, S_M, Int32, Int8, Bool);
    scilab_fill_comparison_non_equal(ScalarInt32, UInt8, S_M, Int32, UInt8, Bool);
    scilab_fill_comparison_non_equal(ScalarInt32, Int16, S_M, Int32, Int16, Bool);
    scilab_fill_comparison_non_equal(ScalarInt32, UInt16, S_M, Int32, UInt16, Bool);
    scilab_fill_comparison_non_equal(ScalarInt32, Int32, S_M, Int32, Int32, Bool);
    scilab_fill_comparison_non_equal(ScalarInt32, UInt32, S_M, Int32, UInt32, Bool);
    scilab_fill_comparison_non_equal(ScalarInt32, Int64, S_M, Int32, Int64, Bool);
    scilab_fill_comparison_non_equal(ScalarInt32, UInt64, S_M, Int32, UInt64, Bool);
    scilab_fill_comparison_non_equal(ScalarInt32, Bool, S_M, Int32, Bool, Bool);
    scilab_fill_comparison_non_equal(ScalarInt32, Empty, M_E, Int32, Double, Bool);

    //Scalar == Scalar
    scilab_fill_comparison_non_equal(ScalarInt32, ScalarDouble, S_S, Int32, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarInt32, ScalarInt8, S_S, Int32, Int8, Bool);
    scilab_fill_comparison_non_equal(ScalarInt32, ScalarUInt8, S_S, Int32, UInt8, Bool);
    scilab_fill_comparison_non_equal(ScalarInt32, ScalarInt16, S_S, Int32, Int16, Bool);
    scilab_fill_comparison_non_equal(ScalarInt32, ScalarUInt16, S_S, Int32, UInt16, Bool);
    scilab_fill_comparison_non_equal(ScalarInt32, ScalarInt32, S_S, Int32, Int32, Bool);
    scilab_fill_comparison_non_equal(ScalarInt32, ScalarUInt32, S_S, Int32, UInt32, Bool);
    scilab_fill_comparison_non_equal(ScalarInt32, ScalarInt64, S_S, Int32, Int64, Bool);
    scilab_fill_comparison_non_equal(ScalarInt32, ScalarUInt64, S_S, Int32, UInt64, Bool);
    scilab_fill_comparison_non_equal(ScalarInt32, ScalarBool, S_S, Int32, Bool, Bool);

    //UInt32
    //Matrix == Matrix
    scilab_fill_comparison_non_equal(UInt32, Double, M_M, UInt32, Double, Bool);
    scilab_fill_comparison_non_equal(UInt32, Int8, M_M, UInt32, Int8, Bool);
    scilab_fill_comparison_non_equal(UInt32, UInt8, M_M, UInt32, UInt8, Bool);
    scilab_fill_comparison_non_equal(UInt32, Int16, M_M, UInt32, Int16, Bool);
    scilab_fill_comparison_non_equal(UInt32, UInt16, M_M, UInt32, UInt16, Bool);
    scilab_fill_comparison_non_equal(UInt32, Int32, M_M, UInt32, Int32, Bool);
    scilab_fill_comparison_non_equal(UInt32, UInt32, M_M, UInt32, UInt32, Bool);
    scilab_fill_comparison_non_equal(UInt32, Int64, M_M, UInt32, Int64, Bool);
    scilab_fill_comparison_non_equal(UInt32, UInt64, M_M, UInt32, UInt64, Bool);
    scilab_fill_comparison_non_equal(UInt32, Bool, M_M, UInt32, Bool, Bool);
    scilab_fill_comparison_non_equal(UInt32, Empty, M_E, UInt32, Double, Bool);

    //Matrix == Scalar
    scilab_fill_comparison_non_equal(UInt32, ScalarDouble, M_S, UInt32, Double, Bool);
    scilab_fill_comparison_non_equal(UInt32, ScalarInt8, M_S, UInt32, Int8, Bool);
    scilab_fill_comparison_non_equal(UInt32, ScalarUInt8, M_S, UInt32, UInt8, Bool);
    scilab_fill_comparison_non_equal(UInt32, ScalarInt16, M_S, UInt32, Int16, Bool);
    scilab_fill_comparison_non_equal(UInt32, ScalarUInt16, M_S, UInt32, UInt16, Bool);
    scilab_fill_comparison_non_equal(UInt32, ScalarInt32, M_S, UInt32, Int32, Bool);
    scilab_fill_comparison_non_equal(UInt32, ScalarUInt32, M_S, UInt32, UInt32, Bool);
    scilab_fill_comparison_non_equal(UInt32, ScalarInt64, M_S, UInt32, Int64, Bool);
    scilab_fill_comparison_non_equal(UInt32, ScalarUInt64, M_S, UInt32, UInt64, Bool);
    scilab_fill_comparison_non_equal(UInt32, ScalarBool, M_S, UInt32, Bool, Bool);

    //Scalar == Matrix
    scilab_fill_comparison_non_equal(ScalarUInt32, Double, S_M, UInt32, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt32, Int8, S_M, UInt32, Int8, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt32, UInt8, S_M, UInt32, UInt8, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt32, Int16, S_M, UInt32, Int16, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt32, UInt16, S_M, UInt32, UInt16, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt32, Int32, S_M, UInt32, Int32, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt32, UInt32, S_M, UInt32, UInt32, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt32, Int64, S_M, UInt32, Int64, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt32, UInt64, S_M, UInt32, UInt64, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt32, Bool, S_M, UInt32, Bool, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt32, Empty, M_E, UInt32, Double, Bool);

    //Scalar == Scalar
    scilab_fill_comparison_non_equal(ScalarUInt32, ScalarDouble, S_S, UInt32, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt32, ScalarInt8, S_S, UInt32, Int8, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt32, ScalarUInt8, S_S, UInt32, UInt8, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt32, ScalarInt16, S_S, UInt32, Int16, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt32, ScalarUInt16, S_S, UInt32, UInt16, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt32, ScalarInt32, S_S, UInt32, Int32, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt32, ScalarUInt32, S_S, UInt32, UInt32, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt32, ScalarInt64, S_S, UInt32, Int64, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt32, ScalarUInt64, S_S, UInt32, UInt64, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt32, ScalarBool, S_S, UInt32, Bool, Bool);

    //Int64
    //Matrix == Matrix
    scilab_fill_comparison_non_equal(Int64, Double, M_M, Int64, Double, Bool);
    scilab_fill_comparison_non_equal(Int64, Int8, M_M, Int64, Int8, Bool);
    scilab_fill_comparison_non_equal(Int64, UInt8, M_M, Int64, UInt8, Bool);
    scilab_fill_comparison_non_equal(Int64, Int16, M_M, Int64, Int16, Bool);
    scilab_fill_comparison_non_equal(Int64, UInt16, M_M, Int64, UInt16, Bool);
    scilab_fill_comparison_non_equal(Int64, Int32, M_M, Int64, Int32, Bool);
    scilab_fill_comparison_non_equal(Int64, UInt32, M_M, Int64, UInt32, Bool);
    scilab_fill_comparison_non_equal(Int64, Int64, M_M, Int64, Int64, Bool);
    scilab_fill_comparison_non_equal(Int64, UInt64, M_M, Int64, UInt64, Bool);
    scilab_fill_comparison_non_equal(Int64, Bool, M_M, Int64, Bool, Bool);
    scilab_fill_comparison_non_equal(Int64, Empty, M_E, Int64, Double, Bool);

    //Matrix == Scalar
    scilab_fill_comparison_non_equal(Int64, ScalarDouble, M_S, Int64, Double, Bool);
    scilab_fill_comparison_non_equal(Int64, ScalarInt8, M_S, Int64, Int8, Bool);
    scilab_fill_comparison_non_equal(Int64, ScalarUInt8, M_S, Int64, UInt8, Bool);
    scilab_fill_comparison_non_equal(Int64, ScalarInt16, M_S, Int64, Int16, Bool);
    scilab_fill_comparison_non_equal(Int64, ScalarUInt16, M_S, Int64, UInt16, Bool);
    scilab_fill_comparison_non_equal(Int64, ScalarInt32, M_S, Int64, Int32, Bool);
    scilab_fill_comparison_non_equal(Int64, ScalarUInt32, M_S, Int64, UInt32, Bool);
    scilab_fill_comparison_non_equal(Int64, ScalarInt64, M_S, Int64, Int64, Bool);
    scilab_fill_comparison_non_equal(Int64, ScalarUInt64, M_S, Int64, UInt64, Bool);
    scilab_fill_comparison_non_equal(Int64, ScalarBool, M_S, Int64, Bool, Bool);

    //Scalar == Matrix
    scilab_fill_comparison_non_equal(ScalarInt64, Double, S_M, Int64, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarInt64, Int8, S_M, Int64, Int8, Bool);
    scilab_fill_comparison_non_equal(ScalarInt64, UInt8, S_M, Int64, UInt8, Bool);
    scilab_fill_comparison_non_equal(ScalarInt64, Int16, S_M, Int64, Int16, Bool);
    scilab_fill_comparison_non_equal(ScalarInt64, UInt16, S_M, Int64, UInt16, Bool);
    scilab_fill_comparison_non_equal(ScalarInt64, Int32, S_M, Int64, Int32, Bool);
    scilab_fill_comparison_non_equal(ScalarInt64, UInt32, S_M, Int64, UInt32, Bool);
    scilab_fill_comparison_non_equal(ScalarInt64, Int64, S_M, Int64, Int64, Bool);
    scilab_fill_comparison_non_equal(ScalarInt64, UInt64, S_M, Int64, UInt64, Bool);
    scilab_fill_comparison_non_equal(ScalarInt64, Bool, S_M, Int64, Bool, Bool);
    scilab_fill_comparison_non_equal(ScalarInt64, Empty, M_E, Int64, Double, Bool);

    //Scalar == Scalar
    scilab_fill_comparison_non_equal(ScalarInt64, ScalarDouble, S_S, Int64, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarInt64, ScalarInt8, S_S, Int64, Int8, Bool);
    scilab_fill_comparison_non_equal(ScalarInt64, ScalarUInt8, S_S, Int64, UInt8, Bool);
    scilab_fill_comparison_non_equal(ScalarInt64, ScalarInt16, S_S, Int64, Int16, Bool);
    scilab_fill_comparison_non_equal(ScalarInt64, ScalarUInt16, S_S, Int64, UInt16, Bool);
    scilab_fill_comparison_non_equal(ScalarInt64, ScalarInt32, S_S, Int64, Int32, Bool);
    scilab_fill_comparison_non_equal(ScalarInt64, ScalarUInt32, S_S, Int64, UInt32, Bool);
    scilab_fill_comparison_non_equal(ScalarInt64, ScalarInt64, S_S, Int64, Int64, Bool);
    scilab_fill_comparison_non_equal(ScalarInt64, ScalarUInt64, S_S, Int64, UInt64, Bool);
    scilab_fill_comparison_non_equal(ScalarInt64, ScalarBool, S_S, Int64, Bool, Bool);

    //UInt64
    //Matrix == Matrix
    scilab_fill_comparison_non_equal(UInt64, Double, M_M, UInt64, Double, Bool);
    scilab_fill_comparison_non_equal(UInt64, Int8, M_M, UInt64, Int8, Bool);
    scilab_fill_comparison_non_equal(UInt64, UInt8, M_M, UInt64, UInt8, Bool);
    scilab_fill_comparison_non_equal(UInt64, Int16, M_M, UInt64, Int16, Bool);
    scilab_fill_comparison_non_equal(UInt64, UInt16, M_M, UInt64, UInt16, Bool);
    scilab_fill_comparison_non_equal(UInt64, Int32, M_M, UInt64, Int32, Bool);
    scilab_fill_comparison_non_equal(UInt64, UInt32, M_M, UInt64, UInt32, Bool);
    scilab_fill_comparison_non_equal(UInt64, Int64, M_M, UInt64, Int64, Bool);
    scilab_fill_comparison_non_equal(UInt64, UInt64, M_M, UInt64, UInt64, Bool);
    scilab_fill_comparison_non_equal(UInt64, Bool, M_M, UInt64, Bool, Bool);
    scilab_fill_comparison_non_equal(UInt64, Empty, M_E, UInt64, Double, Bool);

    //Matrix == Scalar
    scilab_fill_comparison_non_equal(UInt64, ScalarDouble, M_S, UInt64, Double, Bool);
    scilab_fill_comparison_non_equal(UInt64, ScalarInt8, M_S, UInt64, Int8, Bool);
    scilab_fill_comparison_non_equal(UInt64, ScalarUInt8, M_S, UInt64, UInt8, Bool);
    scilab_fill_comparison_non_equal(UInt64, ScalarInt16, M_S, UInt64, Int16, Bool);
    scilab_fill_comparison_non_equal(UInt64, ScalarUInt16, M_S, UInt64, UInt16, Bool);
    scilab_fill_comparison_non_equal(UInt64, ScalarInt32, M_S, UInt64, Int32, Bool);
    scilab_fill_comparison_non_equal(UInt64, ScalarUInt32, M_S, UInt64, UInt32, Bool);
    scilab_fill_comparison_non_equal(UInt64, ScalarInt64, M_S, UInt64, Int64, Bool);
    scilab_fill_comparison_non_equal(UInt64, ScalarUInt64, M_S, UInt64, UInt64, Bool);
    scilab_fill_comparison_non_equal(UInt64, ScalarBool, M_S, UInt64, Bool, Bool);

    //Scalar == Matrix
    scilab_fill_comparison_non_equal(ScalarUInt64, Double, S_M, UInt64, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt64, Int8, S_M, UInt64, Int8, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt64, UInt8, S_M, UInt64, UInt8, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt64, Int16, S_M, UInt64, Int16, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt64, UInt16, S_M, UInt64, UInt16, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt64, Int32, S_M, UInt64, Int32, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt64, UInt32, S_M, UInt64, UInt32, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt64, Int64, S_M, UInt64, Int64, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt64, UInt64, S_M, UInt64, UInt64, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt64, Bool, S_M, UInt64, Bool, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt64, Empty, M_E, UInt64, Double, Bool);

    //Scalar == Scalar
    scilab_fill_comparison_non_equal(ScalarUInt64, ScalarDouble, S_S, UInt64, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt64, ScalarInt8, S_S, UInt64, Int8, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt64, ScalarUInt8, S_S, UInt64, UInt8, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt64, ScalarInt16, S_S, UInt64, Int16, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt64, ScalarUInt16, S_S, UInt64, UInt16, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt64, ScalarInt32, S_S, UInt64, Int32, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt64, ScalarUInt32, S_S, UInt64, UInt32, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt64, ScalarInt64, S_S, UInt64, Int64, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt64, ScalarUInt64, S_S, UInt64, UInt64, Bool);
    scilab_fill_comparison_non_equal(ScalarUInt64, ScalarBool, S_S, UInt64, Bool, Bool);

    //Bool
    //Matrix == Matrix
    scilab_fill_comparison_non_equal(Bool, Double, M_M, Bool, Double, Bool);
    scilab_fill_comparison_non_equal(Bool, Int8, M_M, Bool, Int8, Bool);
    scilab_fill_comparison_non_equal(Bool, UInt8, M_M, Bool, UInt8, Bool);
    scilab_fill_comparison_non_equal(Bool, Int16, M_M, Bool, Int16, Bool);
    scilab_fill_comparison_non_equal(Bool, UInt16, M_M, Bool, UInt16, Bool);
    scilab_fill_comparison_non_equal(Bool, Int32, M_M, Bool, Int32, Bool);
    scilab_fill_comparison_non_equal(Bool, UInt32, M_M, Bool, UInt32, Bool);
    scilab_fill_comparison_non_equal(Bool, Int64, M_M, Bool, Int64, Bool);
    scilab_fill_comparison_non_equal(Bool, UInt64, M_M, Bool, UInt64, Bool);
    scilab_fill_comparison_non_equal(Bool, Bool, M_M, Bool, Bool, Bool);
    scilab_fill_comparison_non_equal(Bool, Empty, M_E, Bool, Double, Bool);

    //Matrix == Scalar
    scilab_fill_comparison_non_equal(Bool, ScalarDouble, M_S, Bool, Double, Bool);
    scilab_fill_comparison_non_equal(Bool, ScalarInt8, M_S, Bool, Int8, Bool);
    scilab_fill_comparison_non_equal(Bool, ScalarUInt8, M_S, Bool, UInt8, Bool);
    scilab_fill_comparison_non_equal(Bool, ScalarInt16, M_S, Bool, Int16, Bool);
    scilab_fill_comparison_non_equal(Bool, ScalarUInt16, M_S, Bool, UInt16, Bool);
    scilab_fill_comparison_non_equal(Bool, ScalarInt32, M_S, Bool, Int32, Bool);
    scilab_fill_comparison_non_equal(Bool, ScalarUInt32, M_S, Bool, UInt32, Bool);
    scilab_fill_comparison_non_equal(Bool, ScalarInt64, M_S, Bool, Int64, Bool);
    scilab_fill_comparison_non_equal(Bool, ScalarUInt64, M_S, Bool, UInt64, Bool);
    scilab_fill_comparison_non_equal(Bool, ScalarBool, M_S, Bool, Bool, Bool);

    //Scalar == Matrix
    scilab_fill_comparison_non_equal(ScalarBool, Double, S_M, Bool, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarBool, Int8, S_M, Bool, Int8, Bool);
    scilab_fill_comparison_non_equal(ScalarBool, UInt8, S_M, Bool, UInt8, Bool);
    scilab_fill_comparison_non_equal(ScalarBool, Int16, S_M, Bool, Int16, Bool);
    scilab_fill_comparison_non_equal(ScalarBool, UInt16, S_M, Bool, UInt16, Bool);
    scilab_fill_comparison_non_equal(ScalarBool, Int32, S_M, Bool, Int32, Bool);
    scilab_fill_comparison_non_equal(ScalarBool, UInt32, S_M, Bool, UInt32, Bool);
    scilab_fill_comparison_non_equal(ScalarBool, Int64, S_M, Bool, Int64, Bool);
    scilab_fill_comparison_non_equal(ScalarBool, UInt64, S_M, Bool, UInt64, Bool);
    scilab_fill_comparison_non_equal(ScalarBool, Bool, S_M, Bool, Bool, Bool);
    scilab_fill_comparison_non_equal(ScalarBool, Empty, M_E, Bool, Double, Bool);

    //Scalar == Scalar
    scilab_fill_comparison_non_equal(ScalarBool, ScalarDouble, S_S, Bool, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarBool, ScalarInt8, S_S, Bool, Int8, Bool);
    scilab_fill_comparison_non_equal(ScalarBool, ScalarUInt8, S_S, Bool, UInt8, Bool);
    scilab_fill_comparison_non_equal(ScalarBool, ScalarInt16, S_S, Bool, Int16, Bool);
    scilab_fill_comparison_non_equal(ScalarBool, ScalarUInt16, S_S, Bool, UInt16, Bool);
    scilab_fill_comparison_non_equal(ScalarBool, ScalarInt32, S_S, Bool, Int32, Bool);
    scilab_fill_comparison_non_equal(ScalarBool, ScalarUInt32, S_S, Bool, UInt32, Bool);
    scilab_fill_comparison_non_equal(ScalarBool, ScalarInt64, S_S, Bool, Int64, Bool);
    scilab_fill_comparison_non_equal(ScalarBool, ScalarUInt64, S_S, Bool, UInt64, Bool);
    scilab_fill_comparison_non_equal(ScalarBool, ScalarBool, S_S, Bool, Bool, Bool);

    //Identity
    scilab_fill_comparison_non_equal(Identity, Double, I_M, Double, Double, Bool);
    scilab_fill_comparison_non_equal(Identity, DoubleComplex, I_MC, Double, Double, Bool);
    scilab_fill_comparison_non_equal(Identity, ScalarDouble, I_S, Double, Double, Bool);
    scilab_fill_comparison_non_equal(Identity, ScalarDoubleComplex, I_SC, Double, Double, Bool);
    scilab_fill_comparison_non_equal(Identity, Identity, I_I, Double, Double, Bool);
    scilab_fill_comparison_non_equal(Identity, IdentityComplex, I_IC, Double, Double, Bool);
    scilab_fill_comparison_non_equal(Identity, Empty, M_E, Double, Double, Bool);

    scilab_fill_comparison_non_equal(Identity, Polynom, M_M, Double, Polynom, Bool);
    scilab_fill_comparison_non_equal(Identity, PolynomComplex, M_M, Double, Polynom, Bool);
    scilab_fill_comparison_non_equal(Identity, ScalarPolynom, M_M, Double, Polynom, Bool);
    scilab_fill_comparison_non_equal(Identity, ScalarPolynomComplex, M_M, Double, Polynom, Bool);
    scilab_fill_comparison_non_equal(Identity, Sparse, M_M, Double, Sparse, SparseBool);
    scilab_fill_comparison_non_equal(Identity, SparseComplex, M_M, Double, Sparse, SparseBool);

    scilab_fill_comparison_non_equal(IdentityComplex, Double, IC_M, Double, Double, Bool);
    scilab_fill_comparison_non_equal(IdentityComplex, DoubleComplex, IC_MC, Double, Double, Bool);
    scilab_fill_comparison_non_equal(IdentityComplex, ScalarDouble, IC_S, Double, Double, Bool);
    scilab_fill_comparison_non_equal(IdentityComplex, ScalarDoubleComplex, IC_SC, Double, Double, Bool);
    scilab_fill_comparison_non_equal(IdentityComplex, Identity, IC_I, Double, Double, Bool);
    scilab_fill_comparison_non_equal(IdentityComplex, IdentityComplex, IC_IC, Double, Double, Bool);
    scilab_fill_comparison_non_equal(IdentityComplex, Empty, M_E, Double, Double, Bool);

    scilab_fill_comparison_non_equal(IdentityComplex, Polynom, M_M, Double, Polynom, Bool);
    scilab_fill_comparison_non_equal(IdentityComplex, PolynomComplex, M_M, Double, Polynom, Bool);
    scilab_fill_comparison_non_equal(IdentityComplex, ScalarPolynom, M_M, Double, Polynom, Bool);
    scilab_fill_comparison_non_equal(IdentityComplex, ScalarPolynomComplex, M_M, Double, Polynom, Bool);
    scilab_fill_comparison_non_equal(IdentityComplex, Sparse, M_M, Double, Sparse, SparseBool);
    scilab_fill_comparison_non_equal(IdentityComplex, SparseComplex, M_M, Double, Sparse, SparseBool);

    //Polynom

    //poly == poly
    scilab_fill_comparison_non_equal(Polynom, Polynom, M_M, Polynom, Polynom, Bool);
    scilab_fill_comparison_non_equal(Polynom, PolynomComplex, M_M, Polynom, Polynom, Bool);
    scilab_fill_comparison_non_equal(PolynomComplex, Polynom, M_M, Polynom, Polynom, Bool);
    scilab_fill_comparison_non_equal(PolynomComplex, PolynomComplex, M_M, Polynom, Polynom, Bool);

    //poly == scalar poly
    scilab_fill_comparison_non_equal(Polynom, ScalarPolynom, M_M, Polynom, Polynom, Bool);
    scilab_fill_comparison_non_equal(Polynom, ScalarPolynomComplex, M_M, Polynom, Polynom, Bool);
    scilab_fill_comparison_non_equal(PolynomComplex, ScalarPolynom, M_M, Polynom, Polynom, Bool);
    scilab_fill_comparison_non_equal(PolynomComplex, ScalarPolynomComplex, M_M, Polynom, Polynom, Bool);

    //poly == double
    scilab_fill_comparison_non_equal(Polynom, Double, M_M, Polynom, Double, Bool);
    scilab_fill_comparison_non_equal(Polynom, DoubleComplex, M_M, Polynom, Double, Bool);
    scilab_fill_comparison_non_equal(PolynomComplex, Double, M_M, Polynom, Double, Bool);
    scilab_fill_comparison_non_equal(PolynomComplex, DoubleComplex, M_M, Polynom, Double, Bool);

    //poly == scalar double
    scilab_fill_comparison_non_equal(Polynom, ScalarDouble, M_M, Polynom, Double, Bool);
    scilab_fill_comparison_non_equal(Polynom, ScalarDoubleComplex, M_M, Polynom, Double, Bool);
    scilab_fill_comparison_non_equal(PolynomComplex, ScalarDouble, M_M, Polynom, Double, Bool);
    scilab_fill_comparison_non_equal(PolynomComplex, ScalarDoubleComplex, M_M, Polynom, Double, Bool);

    //poly == []
    scilab_fill_comparison_non_equal(Polynom, Empty, M_E, Polynom, Double, Bool);
    scilab_fill_comparison_non_equal(PolynomComplex, Empty, M_E, Polynom, Double, Bool);

    //poly == eye
    scilab_fill_comparison_non_equal(Polynom, Identity, M_M, Polynom, Double, Bool);
    scilab_fill_comparison_non_equal(Polynom, IdentityComplex, M_M, Polynom, Double, Bool);
    scilab_fill_comparison_non_equal(PolynomComplex, Identity, M_M, Polynom, Double, Bool);
    scilab_fill_comparison_non_equal(PolynomComplex, IdentityComplex, M_M, Polynom, Double, Bool);

    //scalar poly == poly
    scilab_fill_comparison_non_equal(ScalarPolynom, Polynom, M_M, Polynom, Polynom, Bool);
    scilab_fill_comparison_non_equal(ScalarPolynom, PolynomComplex, M_M, Polynom, Polynom, Bool);
    scilab_fill_comparison_non_equal(ScalarPolynomComplex, Polynom, M_M, Polynom, Polynom, Bool);
    scilab_fill_comparison_non_equal(ScalarPolynomComplex, PolynomComplex, M_M, Polynom, Polynom, Bool);

    //scalar poly == scalar poly
    scilab_fill_comparison_non_equal(ScalarPolynom, ScalarPolynom, M_M, Polynom, Polynom, Bool);
    scilab_fill_comparison_non_equal(ScalarPolynom, ScalarPolynomComplex, M_M, Polynom, Polynom, Bool);
    scilab_fill_comparison_non_equal(ScalarPolynomComplex, ScalarPolynom, M_M, Polynom, Polynom, Bool);
    scilab_fill_comparison_non_equal(ScalarPolynomComplex, ScalarPolynomComplex, M_M, Polynom, Polynom, Bool);

    //scalar poly == double
    scilab_fill_comparison_non_equal(ScalarPolynom, Double, M_M, Polynom, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarPolynom, DoubleComplex, M_M, Polynom, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarPolynomComplex, Double, M_M, Polynom, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarPolynomComplex, DoubleComplex, M_M, Polynom, Double, Bool);

    //scalar poly == scalar double
    scilab_fill_comparison_non_equal(ScalarPolynom, ScalarDouble, M_M, Polynom, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarPolynom, ScalarDoubleComplex, M_M, Polynom, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarPolynomComplex, ScalarDouble, M_M, Polynom, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarPolynomComplex, ScalarDoubleComplex, M_M, Polynom, Double, Bool);

    //scalar poly == []
    scilab_fill_comparison_non_equal(ScalarPolynom, Empty, M_E, Polynom, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarPolynomComplex, Empty, M_E, Polynom, Double, Bool);

    //scalar poly == eye
    scilab_fill_comparison_non_equal(ScalarPolynom, Identity, M_M, Polynom, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarPolynom, IdentityComplex, M_M, Polynom, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarPolynomComplex, Identity, M_M, Polynom, Double, Bool);
    scilab_fill_comparison_non_equal(ScalarPolynomComplex, IdentityComplex, M_M, Polynom, Double, Bool);

    //Sparse
    scilab_fill_comparison_non_equal(Sparse, Sparse, M_M, Sparse, Sparse, SparseBool);
    scilab_fill_comparison_non_equal(Sparse, SparseComplex, M_M, Sparse, Sparse, SparseBool);
    scilab_fill_comparison_non_equal(Sparse, Double, M_M, Sparse, Double, SparseBool);
    scilab_fill_comparison_non_equal(Sparse, DoubleComplex, M_M, Sparse, Double, SparseBool);
    scilab_fill_comparison_non_equal(Sparse, ScalarDouble, M_M, Sparse, Double, SparseBool);
    scilab_fill_comparison_non_equal(Sparse, ScalarDoubleComplex, M_M, Sparse, Double, SparseBool);

    scilab_fill_comparison_non_equal(Sparse, Empty, M_E, Sparse, Double, Bool);
    scilab_fill_comparison_non_equal(Sparse, Identity, M_M, Sparse, Double, SparseBool);
    scilab_fill_comparison_non_equal(Sparse, IdentityComplex, M_M, Sparse, Double, SparseBool);

    scilab_fill_comparison_non_equal(SparseComplex, Sparse, M_M, Sparse, Sparse, SparseBool);
    scilab_fill_comparison_non_equal(SparseComplex, SparseComplex, M_M, Sparse, Sparse, SparseBool);
    scilab_fill_comparison_non_equal(SparseComplex, Double, M_M, Sparse, Double, SparseBool);
    scilab_fill_comparison_non_equal(SparseComplex, DoubleComplex, M_M, Sparse, Double, SparseBool);
    scilab_fill_comparison_non_equal(SparseComplex, ScalarDouble, M_M, Sparse, Double, SparseBool);
    scilab_fill_comparison_non_equal(SparseComplex, ScalarDoubleComplex, M_M, Sparse, Double, SparseBool);

    scilab_fill_comparison_non_equal(SparseComplex, Empty, M_E, Sparse, Double, Bool);
    scilab_fill_comparison_non_equal(SparseComplex, Identity, M_M, Sparse, Double, SparseBool);
    scilab_fill_comparison_non_equal(SparseComplex, IdentityComplex, M_M, Sparse, Double, SparseBool);

#undef scilab_fill_comparison_non_equal

}

InternalType *GenericComparisonNonEqual(InternalType *_pLeftOperand, InternalType *_pRightOperand)
{
    InternalType *pResult = NULL;

    compnoequal_function compnoequal = pComparisonNonEqualfunction[_pRightOperand->getId()][_pLeftOperand->getId()];
    if (compnoequal)
    {
        pResult = compnoequal(_pRightOperand, _pLeftOperand);
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


//Matrix == x
template<class T, class U, class O>
InternalType* compnoequal_M_M(T *_pL, U *_pR)
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

    for (int i = 0; i < iDimsL; ++i)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }
    }

    O* pOut = new O(iDimsL, piDimsL);
    int iSize = pOut->getSize();

    compnoequal(_pL->get(), iSize, _pR->get(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_M_MC(T *_pL, U *_pR)
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

    for (int i = 0; i < iDimsL; ++i)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }
    }

    O* pOut = new O(iDimsL, piDimsL);
    int iSize = pOut->getSize();

    compnoequal(_pL->get(), iSize, _pR->get(), _pR->getImg(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_M_S(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    compnoequal(_pL->get(), (size_t)pOut->getSize(), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_M_SC(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    compnoequal(_pL->get(), pOut->getSize(), _pR->get(0), _pR->getImg(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_M_I(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    Double* pIdentity = Double::Identity(_pL->getDims(), _pL->getDimsArray(), _pR->get(0));
    compnoequal(_pL->get(), (size_t)pOut->getSize(), pIdentity->get(), pOut->get());
    delete pIdentity;
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_M_IC(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    Double* pIdentity = Double::Identity(_pL->getDims(), _pL->getDimsArray(), _pR->get(0), _pR->getImg(0));
    compnoequal(_pL->get(), (size_t)pOut->getSize(), pIdentity->get(), pIdentity->getImg(), pOut->get());
    delete pIdentity;
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_M_E(T *_pL, U *_pR)
{
    return new Bool(true);
}

template<class T, class U, class O>
InternalType* compnoequal_E_M(T *_pL, U *_pR)
{
    return new Bool(true);
}

template<class T, class U, class O>
InternalType* compnoequal_E_E(T *_pL, U *_pR)
{
    return new Bool(false);
}

//Matrix complex == x
template<class T, class U, class O>
InternalType* compnoequal_MC_M(T *_pL, U *_pR)
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

    for (int i = 0; i < iDimsL; ++i)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }
    }

    O* pOut = new O(iDimsL, piDimsL);
    int iSize = pOut->getSize();

    compnoequal(_pL->get(), _pL->getImg(), iSize, _pR->get(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_MC_MC(T *_pL, U *_pR)
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

    for (int i = 0; i < iDimsL; ++i)
    {
        if (piDimsL[i] != piDimsR[i])
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }
    }

    O* pOut = new O(iDimsL, piDimsL);
    int iSize = pOut->getSize();

    compnoequal(_pL->get(), _pL->getImg(), iSize, _pR->get(), _pR->getImg(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_MC_S(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    compnoequal(_pL->get(), _pL->getImg(), pOut->getSize(), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_MC_SC(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    compnoequal(_pL->get(), _pL->getImg(), pOut->getSize(), _pR->get(0), _pR->getImg(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_MC_I(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    Double* pIdentity = Double::Identity(_pL->getDims(), _pL->getDimsArray(), _pR->get(0));
    compnoequal(_pL->get(), _pL->getImg(), (size_t)pOut->getSize(), pIdentity->get(), pOut->get());
    delete pIdentity;
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_MC_IC(T *_pL, U *_pR)
{
    O* pOut = new O(_pL->getDims(), _pL->getDimsArray());
    Double* pIdentity = Double::Identity(_pL->getDims(), _pL->getDimsArray(), _pR->get(0), _pR->getImg(0));
    compnoequal(_pL->get(), _pL->getImg(), (size_t)pOut->getSize(), pIdentity->get(), pIdentity->getImg(), pOut->get());
    delete pIdentity;
    return pOut;
}

//Scalar == x
template<class T, class U, class O>
InternalType* compnoequal_S_M(T *_pL, U *_pR)
{
    O* pOut = new O(_pR->getDims(), _pR->getDimsArray());
    compnoequal(_pL->get(0), (size_t)pOut->getSize(), _pR->get(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_S_MC(T *_pL, U *_pR)
{
    O* pOut = new O(_pR->getDims(), _pR->getDimsArray());
    compnoequal(_pL->get(0), pOut->getSize(), _pR->get(), _pR->getImg(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_S_S(T *_pL, U *_pR)
{
    O* pOut = new O(1, 1);
    compnoequal(_pL->get(0), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_S_SC(T *_pL, U *_pR)
{
    O* pOut = new O(1, 1);
    compnoequal(_pL->get(), 1, _pR->get(0), _pR->getImg(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_S_I(T *_pL, U *_pR)
{
    O* pOut = new O(1, 1);
    compnoequal(_pL->get(0), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_S_IC(T *_pL, U *_pR)
{

    O* pOut = new  Bool(false);
    compnoequal(_pL->get(0), 1, _pR->get(), _pR->getImg(), pOut->get());
    return pOut;
}
//Scalar complex == x
template<class T, class U, class O>
InternalType* compnoequal_SC_M(T *_pL, U *_pR)
{
    O* pOut = new O(_pR->getDims(), _pR->getDimsArray());
    compnoequal(_pL->get(0), _pL->getImg(0), pOut->getSize(), _pR->get(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_SC_MC(T *_pL, U *_pR)
{
    O* pOut = new O(_pR->getDims(), _pR->getDimsArray());
    compnoequal(_pL->get(0), _pL->getImg(0), pOut->getSize(), _pR->get(), _pR->getImg(), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_SC_S(T *_pL, U *_pR)
{
    O* pOut = new O(true);
    if (_pL->getImg(0) == 0)
    {
        compnoequal(_pL->get(0), _pR->get(0), pOut->get());
    }
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_SC_SC(T *_pL, U *_pR)
{
    O* pOut = new O(true);
    compnoequal(_pL->get(0), _pL->getImg(0), _pR->get(0), _pR->getImg(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_SC_I(T *_pL, U *_pR)
{
    O* pOut = new Bool(true);
    if (_pL->getImg(0) == 0)
    {
        compnoequal(_pL->get(0), _pR->get(0), pOut->get());
    }
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_SC_IC(T *_pL, U *_pR)
{
    O* pOut = new Bool(true);
    compnoequal(_pL->get(0), _pL->getImg(0), _pR->get(0), _pR->getImg(0), pOut->get());
    return pOut;
}

//Identity == x
template<class T, class U, class O>
InternalType* compnoequal_I_M(T *_pL, U *_pR)
{
    O* pOut = new Bool(_pR->getDims(), _pR->getDimsArray());
    Double* pIdentity = Double::Identity(_pR->getDims(), _pR->getDimsArray(), _pL->get(0));
    compnoequal(pIdentity->get(), pOut->getSize(), _pR->get(), pOut->get());
    delete pIdentity;
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_I_MC(T *_pL, U *_pR)
{
    O* pOut = new Bool(_pR->getDims(), _pR->getDimsArray());
    Double* pIdentity = Double::Identity(_pR->getDims(), _pR->getDimsArray(), _pL->get(0));
    compnoequal(pIdentity->get(), pOut->getSize(), _pR->get(), _pR->getImg(), pOut->get());
    delete pIdentity;
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_I_S(T *_pL, U *_pR)
{
    O* pOut = new Bool(true);
    compnoequal(_pL->get(0), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_I_SC(T *_pL, U *_pR)
{
    return compnoequal_SC_I<U, T, O>(_pR, _pL);
}

template<class T, class U, class O>
InternalType* compnoequal_I_I(T *_pL, U *_pR)
{
    O* pOut = new Bool(1, 1);
    compnoequal(_pL->get(0), _pR->get(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_I_IC(T *_pL, U *_pR)
{
    O* pOut = new Bool(true);
    if (_pR->getImg(0) == 0)
    {
        compnoequal(_pL->get(0), _pR->get(0), pOut->get());
    }
    return pOut;
}

//Identity complex == x
template<class T, class U, class O>
InternalType* compnoequal_IC_M(T *_pL, U *_pR)
{
    O* pOut = new Bool(_pR->getDims(), _pR->getDimsArray());
    Double* pIdentity = Double::Identity(_pR->getDims(), _pR->getDimsArray(), _pL->get(0), _pL->getImg(0));
    compnoequal(pIdentity->get(), pIdentity->getImg(), pOut->getSize(), _pR->get(), pOut->get());
    delete pIdentity;
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_IC_MC(T *_pL, U *_pR)
{
    O* pOut = new Bool(_pR->getDims(), _pR->getDimsArray());
    Double* pIdentity = Double::Identity(_pR->getDims(), _pR->getDimsArray(), _pL->get(0), _pL->getImg(0));
    compnoequal(pIdentity->get(), pIdentity->getImg(), pOut->getSize(), _pR->get(), _pR->getImg(), pOut->get());
    delete pIdentity;
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_IC_S(T *_pL, U *_pR)
{
    O* pOut = new Bool(true);
    if (_pL->getImg(0) == 0)
    {
        compnoequal(_pL->get(0), _pR->get(0), pOut->get());
    }
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_IC_SC(T *_pL, U *_pR)
{
    O* pOut = new Bool(1, 1);
    compnoequal(_pL->get(0), _pL->getImg(0), _pR->get(0), _pR->getImg(0), pOut->get());
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_IC_I(T *_pL, U *_pR)
{
    O* pOut = new Bool(true);
    if (_pL->getImg(0) == 0)
    {
        compnoequal(_pL->get(0), _pR->get(0), pOut->get());
    }
    return pOut;
}

template<class T, class U, class O>
InternalType* compnoequal_IC_IC(T *_pL, U *_pR)
{
    O* pOut = new Bool(true);
    compnoequal(_pL->get(0), _pL->getImg(0), _pR->get(0), _pR->getImg(0), pOut->get());
    return pOut;
}


template<>
InternalType* compnoequal_M_M<Sparse, Sparse, SparseBool>(Sparse* _pL, Sparse* _pR)
{
    return NULL;
    //pending changes
    return _pR->newEqualTo(*_pL);
}

//[] == SP
template<>
InternalType* compnoequal_M_M<Double, Sparse, Bool>(Double* _pL, Sparse* _pR)
{
    return new Bool(true);
}

//SP == []
template<>
InternalType* compnoequal_M_M<Sparse, Double, Bool>(Sparse* _pL, Double* _pR)
{
    return new Bool(true);
}

//
template<>
InternalType* compnoequal_M_M<Double, Sparse, SparseBool>(Double* _pL, Sparse* _pR)
{
    return NULL;
    //pending changes

    //D -> SP == SP
    Sparse* pspConvert = NULL;
    types::SparseBool* pOut = NULL;


    if (_pR->isScalar())
    {
        int iSizeOut = _pR->getSize();
        if (_pL->isComplex())
        {
            pspConvert = new Sparse(_pR->getRows(), _pR->getCols(), true);
            std::complex<double> stComplex(_pL->get(0), _pL->getImg(0));
            for (int i = 0; i < iSizeOut; i++)
            {
                if (_pR->get(i) != 0)
                {
                    pspConvert->set(i, stComplex);
                }
            }
        }
        else
        {
            pspConvert = new Sparse(_pR->getRows(), _pR->getCols(), _pR->isComplex());
            for (int i = 0; i < iSizeOut; i++)
            {
                if (_pR->get(i) != 0)
                {
                    pspConvert->set(i, _pL->get(0));
                }
            }
        }
    }
    else
    {
        //check dimensions
        if (_pL->getDims() != 2 || _pL->getRows() != _pR->getRows() || _pL->getCols() != _pR->getCols())
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }
    }

    pOut = _pR->newEqualTo(*pspConvert);
    delete pspConvert;
    return pOut;
}

template<>
InternalType* compnoequal_M_M<Sparse, Double, SparseBool>(Sparse* _pL, Double* _pR)
{
    return NULL;
    //pending changes

    Sparse* pspConvert = NULL;
    types::SparseBool* pOut = NULL;

    if (_pR->isScalar())
    {
        int iSizeOut = _pL->getSize();
        if (_pR->isComplex())
        {
            pspConvert = new Sparse(_pL->getRows(), _pL->getCols(), true);
            std::complex<double> stComplex(_pR->get(0), _pR->getImg(0));
            for (int i = 0; i < iSizeOut; i++)
            {
                if (_pL->get(i) != 0)
                {
                    pspConvert->set(i, stComplex);
                }
            }
        }
        else
        {
            pspConvert = new Sparse(_pL->getRows(), _pL->getCols(), _pL->isComplex());
            for (int i = 0; i < iSizeOut; i++)
            {
                if (_pL->get(i) != 0)
                {
                    pspConvert->set(i, _pR->get(0));
                }
            }
        }
    }
    else
    {
        //check dimensions
        if (_pR->getDims() != 2 || _pR->getRows() != _pL->getRows() || _pR->getCols() != _pL->getCols())
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }
    }

    pOut = _pL->newEqualTo(*pspConvert);
    delete pspConvert;
    return pOut;

}

//Polynom
template<>
InternalType* compnoequal_M_M<Polynom, Polynom, Bool>(Polynom* _pL, Polynom* _pR)
{
    Bool* pbOut = NULL;
    bool bPoise = true;

    SinglePoly** pSPL = _pL->get();
    SinglePoly** pSPR = _pR->get();

    if (_pR->isScalar())
    {
        pbOut = new Bool(_pL->getDims(), _pL->getDimsArray());
        int iSize = pbOut->getSize();

        if (_pL->isComplex())
        {
            if (_pR->isComplex())
            {
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPL[i]->getSize() != pSPR[0]->getSize())
                    {
                        bPoise = true;
                    }
                    else
                    {
                        bPoise = false;
                        for (int j = 0; j < pSPL[0]->getSize() && (bPoise == false); j++)
                        {
                            compnoequal(pSPR[0]->get(j), pSPR[0]->getImg(j), pSPL[i]->get(j), pSPL[i]->getImg(j), &bPoise);
                        }
                    }
                    pbOut->set(i, bPoise);

                }
            }
            else
            {
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPL[i]->getSize() != pSPR[0]->getSize())
                    {
                        bPoise = true;
                    }
                    else
                    {
                        bPoise = false;
                        for (int j = 0; j < pSPL[0]->getSize() && (bPoise == false); j++)
                        {
                            compnoequal(pSPR[0]->get(j), (double)0, pSPL[i]->get(j), pSPL[i]->getImg(j), &bPoise);
                        }
                    }
                    pbOut->set(i, bPoise);
                }
            }
        }
        else
        {
            if (_pR->isComplex())
            {
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPL[i]->getSize() != pSPR[0]->getSize())
                    {
                        bPoise = true;
                    }
                    else
                    {
                        bPoise = false;
                        for (int j = 0; j < pSPL[0]->getSize() && (bPoise == false); j++)
                        {
                            compnoequal(pSPR[0]->get(j), pSPR[0]->getImg(j), pSPL[i]->get(j), (double)0, &bPoise);
                        }
                    }
                    pbOut->set(i, bPoise);
                }
            }
            else
            {
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPL[i]->getSize() != pSPR[0]->getSize())
                    {
                        bPoise = true;
                    }
                    else
                    {
                        bPoise = false;
                        for (int j = 0; j < pSPL[0]->getSize() && (bPoise == false); j++)
                        {
                            compnoequal(pSPR[0]->get(j), pSPL[i]->get(j), &bPoise);
                        }
                    }
                    pbOut->set(i, bPoise);
                }
            }
        }
    }
    else if (_pL->isScalar())
    {
        pbOut = new Bool(_pR->getDims(), _pR->getDimsArray());
        int iSize = pbOut->getSize();

        if (pSPR[0]->getSize() != 1)
        {
            if (_pL->isComplex())
            {
                if (_pR->isComplex())
                {
                    for (int i = 0; i < iSize; i++)
                    {
                        if (pSPL[0]->getSize() != pSPR[i]->getSize())
                        {
                            bPoise = true;
                        }
                        else
                        {
                            bPoise = false;
                            for (int j = 0; j < pSPL[0]->getSize() && (bPoise == false); j++)
                            {
                                compnoequal(pSPL[0]->get(j), pSPL[0]->getImg(j), pSPR[i]->get(j), pSPR[i]->getImg(j), &bPoise);
                            }
                        }
                        pbOut->set(i, bPoise);
                    }
                }
                else
                {
                    for (int i = 0; i < iSize; i++)
                    {
                        if (pSPL[0]->getSize() != pSPR[i]->getSize())
                        {
                            bPoise = true;
                        }
                        else
                        {
                            bPoise = false;
                            for (int j = 0; j < pSPL[0]->getSize() && (bPoise == false); j++)
                            {
                                compnoequal(pSPL[0]->get(j), pSPL[0]->getImg(j), pSPR[i]->get(j), (double)0, &bPoise);
                            }
                        }
                        pbOut->set(i, bPoise);
                    }
                }
            }
            else
            {

                if (_pR->isComplex())
                {
                    for (int i = 0; i < iSize; i++)
                    {
                        if (pSPL[0]->getSize() != pSPR[i]->getSize())
                        {
                            bPoise = true;
                        }
                        else
                        {
                            bPoise = false;
                            for (int j = 0; j < pSPL[0]->getSize() && (bPoise == false); j++)
                            {
                                compnoequal(pSPL[0]->get(j), (double)0, pSPR[i]->get(j), pSPR[i]->get(j), &bPoise);
                            }
                        }
                        pbOut->set(i, bPoise);
                    }
                }
                else
                {
                    for (int i = 0; i < iSize; i++)
                    {
                        if (pSPL[0]->getSize() != pSPR[i]->getSize())
                        {
                            bPoise = true;
                        }
                        else
                        {
                            bPoise = false;
                            for (int j = 0; j < pSPR[i]->getSize() && (bPoise == false); j++)
                            {
                                compnoequal(pSPL[0]->get(j), pSPR[i]->get(j), &bPoise);
                            }
                        }
                        pbOut->set(i, bPoise);
                    }
                }

            }
        }
    }
    else
    {
        //check dimensions
        if (_pL->getDims() != _pR->getDims())
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        for (int i = 0; i < _pL->getDims(); i++)
        {
            if (_pL->getDimsArray()[i] != _pR->getDimsArray()[i])
            {
                throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
            }
        }

        pbOut = new Bool(_pL->getDims(), _pL->getDimsArray());
        int iSize = pbOut->getSize();

        if (_pL->isComplex())
        {
            if (_pR->isComplex())
            {
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPL[i]->getSize() != pSPR[i]->getSize())
                    {
                        bPoise = true;
                    }
                    else
                    {
                        bPoise = false;
                        for (int j = 0; j < pSPR[i]->getSize() && (bPoise == false); j++)
                        {
                            compnoequal(pSPR[i]->get(j), pSPR[i]->getImg(j), pSPL[i]->get(j), pSPL[i]->getImg(j), &bPoise);
                        }
                    }
                    pbOut->set(i, bPoise);
                }
            }
            else
            {
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPL[i]->getSize() != pSPR[i]->getSize())
                    {
                        bPoise = true;
                    }
                    else
                    {
                        bPoise = false;
                        for (int j = 0; j < pSPR[i]->getSize() && (bPoise == false); j++)
                        {
                            compnoequal(pSPL[i]->get(j), pSPL[i]->getImg(j), pSPR[i]->get(j), (double)0, &bPoise);
                        }
                    }
                    pbOut->set(i, bPoise);
                }
            }
        }
        else
        {
            if (_pR->isComplex())
            {
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPL[i]->getSize() != pSPR[i]->getSize())
                    {
                        bPoise = true;
                    }
                    else
                    {
                        bPoise = false;
                        for (int j = 0; j < pSPR[i]->getSize() && (bPoise == false); j++)
                        {
                            compnoequal(pSPR[i]->get(j), pSPR[i]->getImg(j), pSPL[i]->get(j), (double)0, &bPoise);
                        }
                    }
                    pbOut->set(i, bPoise);
                }
            }
            else
            {
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPL[i]->getSize() != pSPR[i]->getSize())
                    {
                        bPoise = true;
                    }
                    else
                    {
                        bPoise = false;
                        for (int j = 0; j < pSPR[i]->getSize() && (bPoise == false); j++)
                        {
                            compnoequal(pSPR[i]->get(j), pSPL[i]->get(j), &bPoise);
                        }
                    }
                    pbOut->set(i, bPoise);
                }
            }
        }
    }

    return pbOut;
}

template<>
InternalType* compnoequal_M_M<Polynom, Double, Bool>(Polynom* _pL, Double* _pR)
{
    Bool* pbOut = NULL;

    SinglePoly** pSPR = _pL->get();

    if (_pR->isIdentity())
    {
        Double* pdblEye = NULL;
        pbOut = new Bool(_pL->getDims(), _pL->getDimsArray());
        int iSize = pbOut->getSize();

        if (_pL->isComplex())
        {
            if (_pR->isComplex())
            {
                pdblEye = Double::Identity(_pL->getDims(), _pL->getDimsArray(), _pR->get(0), _pR->getImg(0));
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPR[i]->getSize() != 1)
                    {
                        pbOut->set(i, true);
                    }
                    else
                    {
                        compnoequal(pSPR[i]->get(0), pSPR[i]->getImg(0), pdblEye->get(i), pdblEye->getImg(i), &(pbOut->get()[i]));
                    }
                }
            }
            else
            {
                pdblEye = Double::Identity(_pL->getDims(), _pL->getDimsArray(), _pR->get(0));
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPR[i]->getSize() != 1)
                    {
                        pbOut->set(i, true);
                    }
                    else
                    {
                        compnoequal(pSPR[i]->get(0), pSPR[i]->getImg(0), pdblEye->get(i), (double)0, &(pbOut->get()[i]));
                    }
                }
            }
        }
        else
        {
            if (_pR->isComplex())
            {
                pdblEye = Double::Identity(_pL->getDims(), _pL->getDimsArray(), _pR->get(0), _pR->getImg(0));
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPR[i]->getSize() != 1)
                    {
                        pbOut->set(i, true);
                    }
                    else
                    {
                        compnoequal(pSPR[i]->get(0), (double)0, pdblEye->get(i), pdblEye->getImg(i), &(pbOut->get()[i]));
                    }
                }
            }
            else
            {
                pdblEye = Double::Identity(_pL->getDims(), _pL->getDimsArray(), _pR->get(0));
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPR[i]->getSize() != 1)
                    {
                        pbOut->set(i, true);
                    }
                    else
                    {
                        compnoequal(pSPR[i]->get(0), pdblEye->get(i), &(pbOut->get()[i]));
                    }
                }
            }
        }
        delete pdblEye;
    }
    else if (_pR->isScalar())
    {
        pbOut = new Bool(_pL->getDims(), _pL->getDimsArray());
        int iSize = pbOut->getSize();

        if (_pL->isComplex())
        {
            if (_pR->isComplex())
            {
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPR[i]->getSize() != 1)
                    {
                        pbOut->set(i, true);
                    }
                    else
                    {
                        compnoequal(pSPR[i]->get(0), pSPR[i]->getImg(0), _pR->get(0), _pR->getImg(0), &(pbOut->get()[i]));
                    }
                }
            }
            else
            {
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPR[i]->getSize() != 1)
                    {
                        pbOut->set(i, true);
                    }
                    else
                    {
                        compnoequal(pSPR[i]->get(0), pSPR[i]->getImg(0), _pR->get(0), (double)0, &(pbOut->get()[i]));
                    }
                }
            }
        }
        else
        {
            if (_pR->isComplex())
            {
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPR[i]->getSize() != 1)
                    {
                        pbOut->set(i, true);
                    }
                    else
                    {
                        compnoequal(pSPR[i]->get(0), (double)0, _pR->get(0), _pR->getImg(0), &(pbOut->get()[i]));
                    }
                }
            }
            else
            {
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPR[i]->getSize() != 1)
                    {
                        pbOut->set(i, true);
                    }
                    else
                    {
                        compnoequal(pSPR[i]->get(0), _pR->get(0), &(pbOut->get()[i]));
                    }
                }
            }
        }
    }
    else if (_pL->isScalar())
    {
        pbOut = new Bool(_pR->getDims(), _pR->getDimsArray());
        int iSize = pbOut->getSize();

        if (pSPR[0]->getSize() == 1)
        {
            if (_pL->isComplex())
            {
                if (_pR->isComplex())
                {
                    for (int i = 0; i < iSize; i++)
                    {
                        compnoequal(pSPR[0]->get(0), pSPR[0]->getImg(0), _pR->get(i), _pR->getImg(i), &(pbOut->get()[i]));
                    }
                }
                else
                {
                    for (int i = 0; i < iSize; i++)
                    {
                        compnoequal(pSPR[0]->get(0), pSPR[0]->getImg(0), _pR->get(i), (double)0, &(pbOut->get()[i]));
                    }
                }
            }
            else
            {
                if (_pR->isComplex())
                {
                    for (int i = 0; i < iSize; i++)
                    {
                        compnoequal(pSPR[0]->get(0), (double)0, _pR->get(i), _pR->getImg(i), &(pbOut->get()[i]));
                    }
                }
                else
                {
                    for (int i = 0; i < iSize; i++)
                    {
                        compnoequal(pSPR[0]->get(0), _pR->get(i), &(pbOut->get()[i]));
                    }
                }

            }
        }
        else
        {
            for (int i = 0; i < iSize; i++)
            {
                pbOut->set(i, true);
            }
        }

    }
    else
    {
        //check dimensions
        if (_pL->getDims() != _pR->getDims())
        {
            throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
        }

        for (int i = 0; i < _pL->getDims(); i++)
        {
            if (_pL->getDimsArray()[i] != _pR->getDimsArray()[i])
            {
                throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
            }
        }

        pbOut = new Bool(_pL->getDims(), _pL->getDimsArray());
        int iSize = pbOut->getSize();

        if (_pL->isComplex())
        {
            if (_pR->isComplex())
            {
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPR[i]->getSize() != 1)
                    {
                        pbOut->set(i, true);
                    }
                    else
                    {
                        compnoequal(pSPR[i]->get(0), pSPR[i]->getImg(0), _pR->get(i), _pR->getImg(i), &(pbOut->get()[i]));
                    }
                }
            }
            else
            {
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPR[i]->getSize() != 1)
                    {
                        pbOut->set(i, true);
                    }
                    else
                    {
                        compnoequal(pSPR[i]->get(0), pSPR[i]->getImg(0), _pR->get(i), (double)0, &(pbOut->get()[i]));
                    }
                }
            }
        }
        else
        {
            if (_pR->isComplex())
            {
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPR[i]->getSize() != 1)
                    {
                        pbOut->set(i, true);
                    }
                    else
                    {
                        compnoequal(pSPR[i]->get(0), (double)0, _pR->get(i), _pR->getImg(i), &(pbOut->get()[i]));
                    }
                }
            }
            else
            {
                for (int i = 0; i < iSize; i++)
                {
                    if (pSPR[i]->getSize() != 1)
                    {
                        pbOut->set(i, true);
                    }
                    else
                    {
                        compnoequal(pSPR[i]->get(0), _pR->get(i), &(pbOut->get()[i]));
                    }
                }
            }
        }
    }

    return pbOut;
}

template<>
InternalType* compnoequal_M_M<Double, Polynom, Bool>(Double* _pL, Polynom* _pR)
{
    return compnoequal_M_M<Polynom, Double, Bool>(_pR, _pL);
}


template<>
InternalType* compnoequal_M_M<String, String, Bool>(String* _pL, String* _pR)
{
    if (_pL->getSize() == _pR->getSize())
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

        for (int i = 0; i < iDimsL; ++i)
        {
            if (piDimsL[i] != piDimsR[i])
            {
                throw ast::ScilabError(_W("Inconsistent row/column dimensions.\n"));
            }
        }
        Bool*  pOut = new Bool(iDimsL, piDimsL);

        for (int i = 0; i < _pL->getSize(); i++)
        {
            pOut->set(i, wcscmp(_pL->get(i), _pR->get(i)) != 0);
        }
        return pOut;
    }
    else if (_pL->getSize() == 1)
    {
        Bool*  pOut = new Bool(_pR->getDims(), _pR->getDimsArray());
        for (int i = 0; i < _pR->getSize(); i++)
        {
            pOut->set(i, wcscmp(_pL->get(0), _pR->get(i)) != 0);
        }
        return pOut;
    }
    else if (_pR->getSize() == 1)
    {
        Bool*  pOut = new Bool(_pL->getDims(), _pL->getDimsArray());
        for (int i = 0; i < _pL->getSize(); i++)
        {
            pOut->set(i, wcscmp(_pL->get(i), _pR->get(0)) != 0);
        }
        return pOut;
    }

    return NULL;
}