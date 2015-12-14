/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include "JITScalars.hxx"
#include "JITArrayofs.hxx"
#include "JITVisitor.hxx"
#include "calls/JITBinOpCall.hxx"
#include "calls/FunctionSignature.hxx"
#include "Cast.hxx"

namespace jit
{

void JITMultiplication::MM(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, JITScilabPtr & O, const analysis::TIType & Otype, JITVisitor & jit)
{
    //MMother(L, Ltype, R, Rtype, O, Otype, jit);return;
    if (Ltype.type == analysis::TIType::DOUBLE && Rtype.type == analysis::TIType::DOUBLE)
    {
        MdMd(L, Ltype, R, Rtype, O, Otype, jit);
    }
    else
    {
        MMother(L, Ltype, R, Rtype, O, Otype, jit);
    }
}

llvm::Function * JITMultiplication::getDGEMM(JITVisitor & jit, llvm::Value *& zero, llvm::Value *& one, llvm::Value *& n)
{
    n = jit.getSpecialVar("dgemm_n");
    if (!n)
    {
        llvm::IRBuilder<> & builder = jit.getBuilder();
        llvm::BasicBlock * cur = builder.GetInsertBlock();
        builder.SetInsertPoint(jit.getEntryBlock());
        n = jit.getAlloca<int8_t>((int8_t)'n', "dgemm_n");
        zero = jit.getAlloca<double>(0, "dgemm_zero");
        one = jit.getAlloca<double>(1, "dgemm_one");
        builder.SetInsertPoint(cur);
        jit.addSpecialVar("dgemm_n", n);
        jit.addSpecialVar("dgemm_zero", zero);
        jit.addSpecialVar("dgemm_one", one);
    }
    else
    {
        zero = jit.getSpecialVar("dgemm_zero");
        one = jit.getSpecialVar("dgemm_one");
    }

    llvm::Type * int8ptr_ty = jit.getTy<int8_t *>();
    llvm::Type * int32ptr_ty = jit.getTy<int32_t *>();
    llvm::Type * int32_ty = jit.getTy<int32_t>();
    llvm::Type * dblptr_ty = jit.getTy<double *>();

    llvm::Type * types[] = { int8ptr_ty, int8ptr_ty, int32ptr_ty, int32ptr_ty, int32ptr_ty, dblptr_ty, dblptr_ty, int32ptr_ty, dblptr_ty, int32ptr_ty, dblptr_ty, dblptr_ty, int32ptr_ty};

    return static_cast<llvm::Function *>(jit.getModule().getOrInsertFunction("dgemm_", llvm::FunctionType::get(jit.getTy<void>(), types, false)));
}

void JITMultiplication::MdMd(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, JITScilabPtr & O, const analysis::TIType & Otype, JITVisitor & jit)
{
    // C2F(dgemm)(&n, &n, &x_r, &y_c, &x_c, &one, x, &x_r, y, &x_c, &zero, o, &x_r);

    llvm::IRBuilder<> & builder = jit.getBuilder();

    llvm::Value * zero, * one, * n;
    llvm::Function * dgemm = getDGEMM(jit, zero, one, n);

    llvm::Type * int32_ty = jit.getTy<int32_t>();
    llvm::Value * L_rows = L->loadRows(jit);
    llvm::Value * L_cols = L->loadCols(jit);
    llvm::Value * R_cols = R->loadCols(jit);
    llvm::Value * size = builder.CreateMul(L_rows, R_cols);
    size = builder.CreateMul(size, jit.getConstant<int64_t>(sizeof(double)));
    llvm::Value * alloc = jit.callNew(size);
    llvm::Value * dbl_alloc = builder.CreateBitCast(alloc, jit.getTy<double *>());
    O->storeData(jit, dbl_alloc);
    llvm::Value * x_r = jit.getValue(builder.CreateTrunc(L_rows, int32_ty), true);
    llvm::Value * x_c = jit.getValue(builder.CreateTrunc(L_cols, int32_ty), true);
    llvm::Value * y_c = jit.getValue(builder.CreateTrunc(R_cols, int32_ty), true);

    llvm::Value * args[] = { n, n, x_r, y_c, x_c, one, L->loadData(jit), x_r, R->loadData(jit), x_c, zero, O->loadData(jit), x_r};

    builder.CreateCall(dgemm, args);
    O->storeRows(jit, L_rows);
    O->storeCols(jit, R_cols);
}

JITScilabPtr JITMultiplication::SS(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, const analysis::TIType & Otype, JITVisitor & jit)
{
    llvm::Value * ret = nullptr;
    llvm::IRBuilder<> & builder = jit.getBuilder();

    // Integral types
    if (!Ltype.isfloating() || !Rtype.isfloating())
    {
        llvm::Value * l = L->loadData(jit);
        llvm::Value * r = R->loadData(jit);
        Cast::promoteScilabValue(Ltype.issigned(), l, Rtype.issigned(), r, builder);
        ret = builder.CreateMul(l, r);
        if (Otype.type == analysis::TIType::DOUBLE)
        {
            // bool * bool = double !!
            ret = builder.CreateSIToFP(ret, jit.getTy<double>());
        }
    }
    else if (Ltype.isfloating() && Rtype.isfloating())
    {
        if (Ltype.type == analysis::TIType::DOUBLE)
        {
            llvm::Value * l = L->loadData(jit);
            if (Rtype.type == analysis::TIType::DOUBLE)
            {
                // double * double
                llvm::Value * r = R->loadData(jit);
                llvm::Type * int64_ty = jit.getTy<int64_t>();
                if (l->getType() == int64_ty && r->getType() == int64_ty)
                {
                    ret = builder.CreateMul(l, r);
                }
                else
                {
                    Cast::castInt64(l, jit);
                    Cast::castInt64(r, jit);
                    ret = builder.CreateFMul(l, r);
                }
            }
            else
            {
                // double * complex
                // if l is an int64, then it needs to be casted into a double
                Cast::castInt64(l, jit);
                std::pair<llvm::Value *, llvm::Value *> r_reim = R->loadReIm(jit);
                llvm::Value * res_re = builder.CreateFMul(l, r_reim.first);
                llvm::Value * res_im = builder.CreateFMul(l, r_reim.second);
                return jit.getScalar(res_re, res_im, Otype.type);
            }
        }
        else
        {
            if (Rtype.type == analysis::TIType::DOUBLE)
            {
                // complex * double
                std::pair<llvm::Value *, llvm::Value *> l_reim = L->loadReIm(jit);
                llvm::Value * r = R->loadData(jit);
                Cast::castInt64(r, jit);
                llvm::Value * res_re = builder.CreateFMul(l_reim.first, r);
                llvm::Value * res_im = builder.CreateFMul(l_reim.second, r);
                return jit.getScalar(res_re, res_im, Otype.type);
            }
            else
            {
                // complex * complex
                std::pair<llvm::Value *, llvm::Value *> l_reim = L->loadReIm(jit);
                std::pair<llvm::Value *, llvm::Value *> r_reim = R->loadReIm(jit);
                llvm::Value * res_re_1 = builder.CreateFMul(l_reim.first, r_reim.first);
                llvm::Value * res_re_2 = builder.CreateFMul(l_reim.second, r_reim.second);
                llvm::Value * res_re = builder.CreateFSub(res_re_1, res_re_2);
                llvm::Value * res_im_1 = builder.CreateFMul(l_reim.second, r_reim.first);
                llvm::Value * res_im_2 = builder.CreateFMul(l_reim.first, r_reim.second);
                llvm::Value * res_im = builder.CreateFAdd(res_im_1, res_im_2);

                /*
                  // FMA implementation (slower...)
                  llvm::Value * fma = llvm::Intrinsic::getDeclaration(&jit.getModule(), llvm::Intrinsic::fma, jit.getTy<double>());

                std::pair<llvm::Value *, llvm::Value *> l_reim = L->loadReIm(jit);
                std::pair<llvm::Value *, llvm::Value *> r_reim = R->loadReIm(jit);
                llvm::Value * bd = builder.CreateFMul(l_reim.second, r_reim.second);
                llvm::Value * m_bd = builder.CreateFNeg(bd);
                llvm::Value * args[] = { l_reim.first, r_reim.first,m_bd };
                llvm::Value * res_re = builder.CreateCall(fma, args);

                llvm::Value * bc = builder.CreateFMul(l_reim.second, r_reim.first);
                args[0] = l_reim.first;
                args[1] = r_reim.second;
                args[2] = bc;
                llvm::Value * res_im = builder.CreateCall(fma, args);*/

                return jit.getScalar(res_re, res_im, Otype.type);
            }
        }
    }

    return jit.getScalar(ret, Otype.type);
}

void JITMultiplication::MMother(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, JITScilabPtr & O, const analysis::TIType & Otype, JITVisitor & jit)
{
    llvm::IRBuilder<> & builder = jit.getBuilder();
    llvm::Type * int64_ty = jit.getTy<int64_t>();
    llvm::Value * Lr = L->loadRows(jit);
    llvm::Value * Lc = L->loadCols(jit);
    llvm::Value * Rc = R->loadCols(jit);
    const std::vector<llvm::Type *> types = FunctionSignature::getFunctionArgsTy(jit,
                                            In<llvm::Type>(int64_ty),
                                            In<llvm::Type>(int64_ty),
                                            In<llvm::Type>(int64_ty),
                                            In<analysis::TIType::Type>(Ltype.type, 1),
                                            In<analysis::TIType::Type>(Rtype.type, 1),
                                            In<analysis::TIType::Type>(Otype.type, 2));

    const std::vector<llvm::Value *> args = FunctionSignature::getFunctionArgs(jit,
                                            In<llvm::Value>(Lr),
                                            In<llvm::Value>(Lc),
                                            In<llvm::Value>(Rc),
                                            In<JITScilabPtr, 0>(L),
                                            In<JITScilabPtr, 0>(R),
                                            In<JITScilabPtr, 1>(O));

    llvm::Function * toCall = static_cast<llvm::Function *>(jit.getModule().getOrInsertFunction(analysis::TIType::get_binary_mangling("times", Ltype, Rtype), llvm::FunctionType::get(jit.getTy<void>(), types, false)));
    builder.CreateCall(toCall, args);
    O->storeRows(jit, Lr);
    O->storeCols(jit, Rc);
}

}
