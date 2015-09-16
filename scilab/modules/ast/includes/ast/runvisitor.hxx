/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef AST_RUNVISITOR_HXX
#define AST_RUNVISITOR_HXX

#include "context.hxx"
#include "all.hxx"
#include "types.hxx"
#include "double.hxx"
#include "bool.hxx"
#include "polynom.hxx"
#include "colon.hxx"
#include "string.hxx"
#include "void.hxx"
#include "configvariable.hxx"
#include "overload.hxx"
#include "scilabWrite.hxx"
#include "variables.hxx"

extern "C" {
#include "more.h"
#include "sci_malloc.h"
}

namespace ast
{
class EXTERN_AST RunVisitor : public ConstVisitor
{
public:
    RunVisitor()
    {
        _excepted_result = -1;
        _resultVect.push_back(nullptr);
        _result = nullptr;
        m_bSingleResult = true;
        m_pAns = symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"ans"));
    }

    ~RunVisitor()
    {
        clearResult();
    }

    void clearResultButFirst()
    {
        if (!isSingleResult() && _resultVect.size() > 1)
        {
            for (std::vector<types::InternalType*>::iterator rv = _resultVect.begin() + 1, end = _resultVect.end(); rv != end; ++rv)
            {
                if (*rv != nullptr)
                {
                    (*rv)->killMe();
                    *rv = nullptr;
                }
            }
        }
    }

    void clearResult()
    {
        if (isSingleResult())
        {
            if (_result != nullptr)
            {
                //                    std::cout << "before single delete : " << _result << std::endl;
                _result->killMe();
                //                    std::cout << "after single delete" << std::endl;
            }
        }
        else
        {
            for (std::vector<types::InternalType*>::iterator rv = _resultVect.begin(); rv != _resultVect.end(); rv++)
            {
                if (*rv != nullptr)
                {
                    (*rv)->killMe();
                }
            }
        }
        _resultVect.clear();
        m_bSingleResult = true;
        _result = nullptr;
    }

public:
    int getExpectedSize(void)
    {
        return _excepted_result;
    }

    void setExpectedSize(int _iSize)
    {
        _excepted_result = _iSize;
    }

    int getResultSize(void)
    {
        if (isSingleResult())
        {
            if (_result == nullptr)
            {
                return 0;
            }
            else
            {
                return 1;
            }
        }
        else
        {
            return static_cast<int>(_resultVect.size());
        }
    }

    inline types::InternalType* getResult(void)
    {
        if (isSingleResult())
        {
            return _result;
        }
        else
        {
            return _resultVect[0];
        }
    }

    types::InternalType* getResult(int _iPos)
    {
        if (isSingleResult() && _iPos == 0)
        {
            return _result;
        }

        if (_iPos >= static_cast<int>(_resultVect.size()))
        {
            return nullptr;
        }
        return _resultVect[_iPos];
    }

    std::vector<types::InternalType*>* getResultList()
    {
        // TODO: this function is not used but it could lead to a memleak
        // (in the first case the vector is allocated and so must be freed)
        if (getResultSize() == 1)
        {
            std::vector<types::InternalType*>* pList = new std::vector<types::InternalType*>;
            pList->push_back(_result);
            return pList;
        }
        else
        {
            return &_resultVect;
        }
    }

    void setResult(int _iPos, const types::InternalType *gtVal)
    {
        m_bSingleResult = false;
        if (_iPos >= static_cast<int>(_resultVect.size()))
        {
            _resultVect.resize(_iPos + 1, nullptr);
        }

        _resultVect[_iPos] = const_cast<types::InternalType *>(gtVal);
    }

    inline void setResult(const types::InternalType *gtVal)
    {
        m_bSingleResult = true;
        _result = const_cast<types::InternalType *>(gtVal);
    }

    inline void setResult(const types::typed_list & out)
    {
        if (out.size() == 0)
        {
            setResult(nullptr);
        }
        else if (out.size() == 1)
        {
            setResult(out[0]);
        }
        else
        {
            m_bSingleResult = false;
            _resultVect.clear();
            for (types::typed_list::const_iterator it = out.begin(); it != out.end(); ++it)
            {
                _resultVect.push_back(*it);
            }
        }
    }

    inline bool isSingleResult()
    {
        return m_bSingleResult;
    }

    void cleanIn(const types::typed_list & in, const types::typed_list & out)
    {
        // Check if in contains entries which are in out too.
        // When an entry is in in and not in out, then in is killed.
        if (!in.empty())
        {
            for (types::typed_list::const_iterator o = out.begin(); o != out.end(); ++o)
            {
                if (*o)
                {
                    (*o)->IncreaseRef();
                }
            }

            for (types::typed_list::const_iterator i = in.begin(); i != in.end(); ++i)
            {
                if (*i)
                {
                    (*i)->DecreaseRef();
                    (*i)->killMe();
                }
            }

            for (types::typed_list::const_iterator o = out.begin(); o != out.end(); ++o)
            {
                if (*o)
                {
                    (*o)->DecreaseRef();
                }
            }
        }
    }

    inline void cleanInOut(const types::typed_list & in, const types::typed_list & out)
    {
        cleanIn(in, out);
        cleanOut(out);
    }

    void cleanOut(const types::typed_list & out)
    {
        if (!out.empty())
        {
            for (types::typed_list::const_iterator o = out.begin(); o != out.end(); ++o)
            {
                if (*o)
                {
                    (*o)->killMe();
                }
            }
        }
    }

    void cleanOpt(const types::optional_list & opt)
    {
        if (!opt.empty())
        {
            for (types::optional_list::const_iterator o = opt.begin(); o != opt.end(); ++o)
            {
                if (o->second)
                {
                    //decreasef ref after increaseref in callexp
                    o->second->DecreaseRef();
                    o->second->killMe();
                }
            }
        }
    }

    /*-------------.
    | Attributes.  |
    `-------------*/
protected:
    std::vector<types::InternalType*>    _resultVect;
    types::InternalType*    _result;
    bool m_bSingleResult;
    int _excepted_result;
    symbol::Variable* m_pAns;
};

template <class T>
class EXTERN_AST RunVisitorT : public RunVisitor
{
public :
    RunVisitorT() : RunVisitor()
    {
    }

    types::typed_list* GetArgumentList(exps_t const & _plstArg)
    {
        types::typed_list* pArgs = new types::typed_list();
        for (exps_t::const_iterator it = _plstArg.begin() ; it != _plstArg.end() ; ++it)
        {
            (*it)->accept(*this);
            if (getResultSize() > 1)
            {
                const int size = getResultSize();
                for (int i = 0 ; i < size; i++)
                {
                    pArgs->push_back(getResult(i));
                }
            }
            else
            {
                pArgs->push_back(getResult());
            }
        }
        //to be sure, delete operation does not delete result
        setResult(nullptr);
        return pArgs;
    }

public :
    //process in another node
    void visitprivate(const MatrixLineExp &/*e*/) {}
    void visitprivate(const CommentExp &/*e*/) {}
    void visitprivate(const ArrayListVar &/*e*/) {}
    void visitprivate(const CaseExp &/*e*/) {}
    void visitprivate(const AssignListExp  &/*e*/) {}

    void visitprivate(const CellExp &e);
    void visitprivate(const FieldExp &e);
    void visitprivate(const IfExp  &e);
    void visitprivate(const WhileExp  &e);
    void visitprivate(const ForExp  &e);
    void visitprivate(const ReturnExp &e);
    void visitprivate(const SelectExp &e);
    void visitprivate(const SeqExp  &e);
    void visitprivate(const NotExp &e);
    void visitprivate(const TransposeExp &e);
    void visitprivate(const FunctionDec  &e);
    void visitprivate(const ListExp &e);
    void visitprivate(const AssignExp  &e);
    void visitprivate(const OpExp &e);
    void visitprivate(const LogicalOpExp &e);
    void visitprivate(const MatrixExp &e);
    void visitprivate(const CallExp &e);
    void visitprivate(const CellCallExp &e);
    void visitprivate(const OptimizedExp &e);
    void visitprivate(const MemfillExp &e);
    void visitprivate(const DAXPYExp &e);
    void visitprivate(const IntSelectExp &e);
    void visitprivate(const StringSelectExp &e);

    void visitprivate(const StringExp &e)
    {
        if (e.getConstant() == nullptr)
        {
            types::String *psz = new types::String(e.getValue().c_str());
            (const_cast<StringExp *>(&e))->setConstant(psz);

        }
        setResult(e.getConstant());
    }


    void visitprivate(const DoubleExp  &e)
    {
        if (e.getConstant() == nullptr)
        {
            types::Double *pdbl = new types::Double(e.getValue());
            (const_cast<DoubleExp *>(&e))->setConstant(pdbl);

        }
        setResult(e.getConstant());
    }


    void visitprivate(const BoolExp  &e)
    {
        if (e.getConstant() == nullptr)
        {
            types::Bool *pB = new types::Bool(e.getValue());
            (const_cast<BoolExp *>(&e))->setConstant(pB);

        }
        setResult(e.getConstant());
    }


    void visitprivate(const NilExp &/*e*/)
    {
        setResult(new types::Void());
    }


    void visitprivate(const SimpleVar &e)
    {
        symbol::Context* ctx = symbol::Context::getInstance();
        symbol::Variable* var = ((SimpleVar&)e).getStack();
        types::InternalType *pI = ctx->get(var);
        setResult(pI);
        if (pI != nullptr)
        {
            if (e.isVerbose() && pI->isCallable() == false && ConfigVariable::isPromptShow())
            {
                std::wostringstream ostr;
                ostr << e.getSymbol().getName() << L"  = ";
#ifndef NDEBUG
                ostr << L"(" << pI->getRef() << L")";
#endif
                ostr << std::endl;
                ostr << std::endl;
                scilabWriteW(ostr.str().c_str());
                std::wostringstream ostrName;
                ostrName  << e.getSymbol().getName();
                VariableToString(pI, ostrName.str().c_str());
            }

            //check if var is recalled in current scope like
            //function f()
            //  a; //<=> a=a;
            //  a(2) = 18;
            //endfunction
            if (e.getParent()->isSeqExp())
            {
                if (ctx->getScopeLevel() > 1 && var->empty() == false && var->top()->m_iLevel != ctx->getScopeLevel())
                {
                    //put var in current scope
                    ctx->put(var, pI);
                }
            }
        }
        else
        {
            char pstError[bsiz];
            wchar_t* pwstError;

            char* strErr =  wide_string_to_UTF8(e.getSymbol().getName().c_str());

            os_sprintf(pstError, _("Undefined variable: %s\n"), strErr);
            pwstError = to_wide_string(pstError);
            FREE(strErr);
            std::wstring wstError(pwstError);
            FREE(pwstError);
            throw InternalError(wstError, 999, e.getLocation());
            //Err, SimpleVar doesn't exist in Scilab scopes.
        }
    }


    void visitprivate(const ColonVar &/*e*/)
    {
        types::Colon *pC = new types::Colon();
        setResult(pC);
    }


    void visitprivate(const DollarVar &/*e*/)
    {
        setResult(types::Polynom::Dollar());
    }

    void visitprivate(const TryCatchExp  &e)
    {
        //save current prompt mode
        int oldVal = ConfigVariable::getSilentError();
        //set mode silent for errors
        ConfigVariable::setSilentError(1);
        try
        {
            e.getTry().accept(*this);
            //restore previous prompt mode
            ConfigVariable::setSilentError(oldVal);
        }
        catch (const InternalError& /* ie */)
        {
            //restore previous prompt mode
            ConfigVariable::setSilentError(oldVal);
            //to lock lasterror
            ConfigVariable::setLastErrorCall();
            // reset call stack filled when error occured
            ConfigVariable::resetWhereError();
            e.getCatch().accept(*this);
        }
    }

    void visitprivate(const BreakExp &e)
    {
        const_cast<BreakExp*>(&e)->setBreak();
    }

    void visitprivate(const ContinueExp &e)
    {
        const_cast<ContinueExp*>(&e)->setContinue();
    }

    void visitprivate(const ArrayListExp  &e)
    {
        exps_t::const_iterator it;
        int iNbExpSize = this->getExpectedSize();
        this->setExpectedSize(1);

        types::typed_list lstIT;
        for (it = e.getExps().begin() ; it != e.getExps().end() ; it++)
        {
            (*it)->accept(*this);
            for (int j = 0; j < getResultSize(); j++)
            {
                lstIT.push_back(getResult(j));
            }
        }

        setResult(lstIT);

        this->setExpectedSize(iNbExpSize);
    }

    void visitprivate(const VarDec  &e)
    {
        try
        {
            /*getting what to assign*/
            e.getInit().accept(*this);
            getResult()->IncreaseRef();
        }
        catch (const InternalError& error)
        {
            throw error;
        }
    }

    types::InternalType* callOverloadOpExp(OpExp::Oper _oper, types::InternalType* _paramL, types::InternalType* _paramR);
    types::InternalType* callOverloadMatrixExp(const std::wstring& strType, types::InternalType* _paramL, types::InternalType* _paramR);
};
}

#endif // !AST_RUNVISITOR_HXX
