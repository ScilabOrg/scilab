/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "runvisitor.hxx"
#include "timedvisitor.hxx"
#include "execvisitor.hxx"


template <class T>
void RunVisitorT<T>::visitprivate(const WhileExp  &e)
{
    T execMeTest;
    T execMeAction;

    //allow break and continue operations
    const_cast<Exp*>(&e.body_get())->breakable_set();
    const_cast<Exp*>(&e.body_get())->continuable_set();
    //allow return operation
    if(e.is_returnable())
    {
        (&e.body_get())->is_returnable();
    }

    //condition
    e.test_get().accept(execMeTest);
    while(bConditionState(execMeTest.result_get()))
    {
        e.body_get().accept(execMeAction);
        if(e.body_get().is_break())
        {
            break;
        }

        if(e.body_get().is_return())
        {
            const_cast<WhileExp*>(&e)->return_set();
            break;
        }

        if(e.body_get().is_continue())
        {
            const_cast<WhileExp*>(&e)->continue_set();
            const_cast<Exp*>(&(e.body_get()))->continue_reset();
            e.test_get().accept(execMeTest);
            continue;
        }

        //clear old result value before evaluate new one
        if(execMeTest.result_get() != NULL)
        {
            if(execMeTest.result_get()->isDeletable())
            {
                delete execMeTest.result_get();
            }
        }

        e.test_get().accept(execMeTest);
    }
}


template <class T>
void RunVisitorT<T>::visitprivate(const ForExp  &e)
{
    T execVar;
    e.vardec_get().accept(execVar);

    //allow break and continue operations
    const_cast<Exp*>(&e.body_get())->breakable_set();
    const_cast<Exp*>(&e.body_get())->continuable_set();
    //allow return operation
    if(e.is_returnable())
    {
        (&e.body_get())->is_returnable();
    }

    if(execVar.result_get()->isImplicitList())
    {
        T execBody;
        ImplicitList* pVar = execVar.result_get()->getAsImplicitList();
        //			std::cout << "ImplicitList references : " << pVar->getRef() << std::endl;

        InternalType *pIT = NULL;
        pIT = pVar->extractValue(0);
        pIT->IncreaseRef();
        symbol::Symbol varName = e.vardec_get().name_get();
        //symbol::Context::getInstance()->put(varName, *pIT);

        Double *pDouble = pIT->getAs<Double>();
        for(int i = 0 ; i < pVar->getSize() ; i++)
        {
/*
            bool bNew = false;
            if(pIT->isRef(1))
            {
                //decrease reference to pIT
                //this variable does not push in context
                //so increase and decrease ref are not "automaticly" manage
                //std::wcout << varName.name_get() << " : " << pIT->getRef() << std::endl;
                //pIT->DecreaseRef();
                pIT = pIT->clone();
                pDouble = pIT->getAs<Double>();
                bNew = true;
            }
*/
            if(pIT->isDouble())
            {
                pDouble->get()[0] = pVar->extractValueInDouble(i);
            }
            else if(pIT->isInt())
            {
                switch(pIT->getType())
                {
                case InternalType::RealInt8 :
                    {
                        Int8* pI = pIT->getAs<Int8>();
                        pI->set(0, (char)pVar->extractValueInInteger(i));
                        break;
                    }
                case InternalType::RealUInt8 :
                    {
                        UInt8* pI = pIT->getAs<UInt8>();
                        pI->set(0, (unsigned char)pVar->extractValueInInteger(i));
                        break;
                    }
                case InternalType::RealInt16 :
                    {
                        Int16* pI = pIT->getAs<Int16>();
                        pI->set(0, (short)pVar->extractValueInInteger(i));
                        break;
                    }
                case InternalType::RealUInt16 :
                    {
                        UInt16* pI = pIT->getAs<UInt16>();
                        pI->set(0, (unsigned short)pVar->extractValueInInteger(i));
                        break;
                    }
                case InternalType::RealInt32 :
                    {
                        Int32* pI = pIT->getAs<Int32>();
                        pI->set(0, (int)pVar->extractValueInInteger(i));
                        break;
                    }
                case InternalType::RealUInt32 :
                    {
                        UInt32* pI = pIT->getAs<UInt32>();
                        pI->set(0, (unsigned int)pVar->extractValueInInteger(i));
                        break;
                    }
                case InternalType::RealInt64 :
                    {
                        Int64* pI = pIT->getAs<Int64>();
                        pI->set(0, (long long)pVar->extractValueInInteger(i));
                        break;
                    }
                case InternalType::RealUInt64 :
                    {
                        UInt64* pI = pIT->getAs<UInt64>();
                        pI->set(0, (unsigned long long)pVar->extractValueInInteger(i));
                        break;
                    }
                }
            }
            else
            {
                pIT = pVar->extractValue(i);
            }

            
            symbol::Context::getInstance()->put(varName, *pIT);
            e.body_get().accept(execBody);
            if(e.body_get().is_break())
            {
                break;
            }

            if(e.body_get().is_continue())
            {
                const_cast<Exp*>(&(e.body_get()))->continue_reset();
                continue;
            }

            if(e.body_get().is_return())
            {
                const_cast<ForExp*>(&e)->return_set();
                break;
            }
        }
        pIT->DecreaseRef();
        pVar->DecreaseRef();
        //delete pVar;
    }
    else
    {//Matrix i = [1,3,2,6] or other type
        T execBody;
        InternalType* pIT = execVar.result_get();
        GenericType* pVar = pIT->getAs<GenericType>();
        if(pVar->getDims() > 2)
        {
            throw ScilabError(_W("for expression can only manage 1 or 2 dimensions variables\n"), 999, e.vardec_get().location_get());
        }

        for(int i = 0 ; i < pVar->getCols() ; i++)
        {
            GenericType* pNew = pVar->getColumnValues(i);
            symbol::Context::getInstance()->put(e.vardec_get().name_get(), *pNew);
            e.body_get().accept(execBody);
            if(e.body_get().is_break())
            {
                break;
            }

            if(e.body_get().is_continue())
            {
                continue;
            }

            if(e.body_get().is_return())
            {
                const_cast<ForExp*>(&e)->return_set();
                break;
            }
        }
    }
}

template void RunVisitorT<TimedVisitor>::visitprivate(const ForExp &e);
template void RunVisitorT<ExecVisitor>::visitprivate(const ForExp &e);

template void RunVisitorT<TimedVisitor>::visitprivate(const WhileExp &e);
template void RunVisitorT<ExecVisitor>::visitprivate(const WhileExp &e);

