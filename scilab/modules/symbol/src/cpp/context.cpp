/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "context.hxx"
#include <iostream>
#include <omp.h>
namespace symbol
{
    std::vector<Context*> Context::we;

    Context::Context()
    {
        PrivateFunTable.scope_begin();
        PrivateVarTable.scope_begin();
        HeapFunTable.scope_begin();
        HeapVarTable.scope_begin();
        EnvFunTable.scope_begin();
        EnvVarTable.scope_begin();
    }

    Context* Context::getInstance(void)
    {
        int const n(omp_get_thread_num());
#pragma omp critical (context)
        {
//            if(n) std::cerr<<"in a worker thread of id: "<<n<<std::endl;
            if (we.size() <= n)
            {
                we.resize(n+1, 0);
            }
            if (!we[n])
            {
                if(n) std::cerr<<"we should not be here, call initThreadedContext before calling macros in threads\n";
                we[n] = n ? new Context(*(we[0])) : new Context();
            }
        }
            return we[n];
    }

    /* init a new context for a slave thread, copying from parent of master thread (0)
     */
    void Context::initThreadedContext()
    {
        int const n( omp_get_thread_num());
        if(n) // only for slaves
        {
#pragma omp critical// (initThreadContext)
            {
                int ancestor(omp_get_ancestor_thread_num(omp_get_level()));
//            std::cerr<<"init thread context for thread "<<n<<" with parent "<<omp_get_ancestor_thread_num(omp_get_level())<<std::endl;
            if (ancestor == n)
            {
                ancestor= 0;
            }
            if(!we[0])
            {
                we[0] = new Context();
            }
            if( we.size() <= n)
            {
                we.resize(n+1, 0);
            }
            delete we[n];
#if 0
            std::cerr<<"cloning:";
            we[ancestor]->print();
            std::cerr<<std::endl;
#endif
            we[n]= new Context(*we[ancestor]);
            }
        }
    }

    void Context::scope_begin() {
        PrivateFunTable.scope_begin();
        PrivateVarTable.scope_begin();
        HeapFunTable.scope_begin();
        HeapVarTable.scope_begin();
        EnvFunTable.scope_begin();
        EnvVarTable.scope_begin();
    }

    void Context::scope_end() {
        PrivateFunTable.scope_end();
        PrivateVarTable.scope_end();
        EnvFunTable.scope_end();
        EnvVarTable.scope_end();
        HeapFunTable.scope_end();
        HeapVarTable.scope_end();
    }

    InternalType* Context::get(const wstring& key) const
    {
        InternalType* pI = NULL;

        //global scope
        if(HeapVarTable.isGlobalVisible(key))
        {
            pI = HeapVarTable.getGlobalValue(key);
        }

        if(pI != NULL)
        {
            return pI;
        }
        else
        {
            pI = HeapFunTable.get(key);
            if(pI != NULL)
            {
                return pI;
            }
        }

        //local scope
        pI = EnvVarTable.get(key);

        if(pI != NULL)
        {
            return pI;
        }
        else
        {
            pI = EnvFunTable.get(key);
            if(pI != NULL)
            {
                return pI;
            }
        }

        return NULL;
    }

    InternalType* Context::getCurrentLevel(const wstring& key) const
    {
        // FIXME
        InternalType* pI = NULL;
        pI = EnvVarTable.getCurrentLevel(key);

        if(pI != NULL)
        {
            return pI;
        }
        else
        {
            pI = EnvFunTable.getCurrentLevel(key);
            if(pI != NULL)
            {
                return pI;
            }
            else
            {
                return NULL;
            }
        }
    }

    InternalType* Context::getAllButCurrentLevel(const wstring& key) const
    {
       // FIXME
        InternalType* pI = NULL;
        pI = EnvVarTable.getAllButCurrentLevel(key);

        if(pI != NULL)
        {
            return pI;
        }
        else
        {
            pI = EnvFunTable.getAllButCurrentLevel(key);
            if(pI != NULL)
            {
                return pI;
            }
            else
            {
                return NULL;
            }
        }
    }

    InternalType* Context::get_fun(const wstring& key) const
    {
        //FIXME Global
        return EnvFunTable.get(key);
    }

    std::list<wstring>& Context::get_funlist(const wstring& _stModuleName)
    {
        return EnvFunTable.get_funlist(_stModuleName);
    }

    bool Context::put(const wstring& key, InternalType &type)
    {
        if(HeapVarTable.isGlobalVisible(key))
        {
           HeapVarTable.setGlobalValue(key, type);
        }
        else
        {//variable not in current global scope
            EnvVarTable.put(key, type);
        }

        return true;
    }

    bool Context::remove(const wstring& key)
    {
        // look in local global scope
        if(HeapVarTable.isGlobalVisible(key))
        {
            HeapVarTable.setGlobalVisible(key, false);
            return true;
        }
        // look in Variables Environment
        else if (EnvVarTable.getCurrentLevel(key) == NULL)
        {
            // If not found, look in Functions Environment
            if (EnvFunTable.getCurrentLevel(key) == NULL)
            {
                return false;
            }
            else
            {
                EnvFunTable.remove(key);
                return true;
            }
        }

        EnvVarTable.remove(key);
        return true;
    }

    bool Context::put_in_previous_scope(const wstring& key, InternalType &type)
    {
        // FIXME
        EnvVarTable.put_in_previous_scope(key, type);
        return true;
    }

    void Context::print()
    {
        //std::wcout << "PrivateFunTable : " << std::endl;
        //std::wcout << PrivateFunTable << std::endl << std::endl;
        //std::wcout << "PrivateVarTable : " << std::endl;
        //std::wcout << PrivateVarTable << std::endl << std::endl;
        //std::wcout << "EnvFunTable : " << std::endl;
        //std::wcout << EnvFunTable << std::endl << std::endl;
        std::wcout << "EnvVarTable : " << std::endl;
        std::wcout << EnvVarTable << std::endl << std::endl;
        //std::wcout << "HeapFunTable : " << std::endl;
        //std::wcout << HeapFunTable << std::endl << std::endl;
        std::wcout << "HeapVarTable : " << std::endl;
        std::wcout << HeapVarTable << std::endl << std::endl;
        //FIXME
    }

    bool Context::AddFunction(types::Function *_info)
    {
        EnvFunTable.put(_info->getName(), *_info);
        return true;
    }

    bool Context::AddMacro(types::Macro *_info)
    {
        EnvFunTable.put(_info->getName(), *_info);
        return true;
    }

    bool Context::AddMacroFile(types::MacroFile *_info)
    {
        EnvFunTable.put(_info->getName(), *_info);
        return true;
    }

    bool Context::isGlobalVisible(const wstring& key) const
    {
        return HeapVarTable.isGlobalVisible(key);
    }

    /*return global variable, search in global scope ( highest )*/
    InternalType* Context::getGlobalValue(const wstring& key) const
    {
        return HeapVarTable.getGlobalValue(key);
    }

    /*return global variable existance status*/
    bool Context::isGlobalExists(const wstring& key) const
    {
        return HeapVarTable.isGlobalExists(key);
    }

    void Context::setGlobalValue(const wstring& key, InternalType &value)
    {
        HeapVarTable.setGlobalValue(key, value);
    }

    void Context::createEmptyGlobalValue(const wstring& key)
    {
        HeapVarTable.createEmptyGlobalValue(key);
    }

    void Context::setGlobalVisible(const wstring& key, bool bVisible)
    {
        HeapVarTable.setGlobalVisible(key, bVisible);
    }

    void Context::removeGlobal(const wstring &key)
    {
        HeapVarTable.removeGlobal(key);
    }

    void Context::removeGlobalAll()
    {
        HeapVarTable.removeGlobalAll();
    }

}

