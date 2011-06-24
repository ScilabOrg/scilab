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
#include "function.hxx"
#include "macro.hxx"
#include "macrofile.hxx"
#include "addGatewayInContext.h"

void addGatewayInContext(wchar_t* _pwstName , OLDGW_FUNC _pFunc, wchar_t* _pwstModule)
{
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(_pwstName, _pFunc, _pwstModule));
}

namespace symbol
{
    Context* Context::me;

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
        if (me == 0)
        {
            me = new Context();
        }
        return me;
    }

    void Context::scope_begin()
    {
        PrivateFunTable.scope_begin();
        PrivateVarTable.scope_begin();
        HeapFunTable.scope_begin();
        HeapVarTable.scope_begin();
        EnvFunTable.scope_begin();
        EnvVarTable.scope_begin();
    }

    void Context::scope_end()
    {
        PrivateFunTable.scope_end();
        PrivateVarTable.scope_end();
        EnvFunTable.scope_end();
        EnvVarTable.scope_end();
        HeapFunTable.scope_end();
        HeapVarTable.scope_end();
    }

    types::InternalType* Context::get(const symbol::Symbol& key) const
    {
        types::InternalType* pI = NULL;

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

    types::InternalType* Context::getCurrentLevel(const symbol::Symbol& key) const
    {
        // FIXME
        types::InternalType* pI = NULL;
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

    types::InternalType* Context::getAllButCurrentLevel(const symbol::Symbol& key) const
    {
       // FIXME
        types::InternalType* pI = NULL;
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

    types::InternalType* Context::get_fun(const symbol::Symbol& key) const
    {
        //FIXME Global
        return EnvFunTable.get(key);
    }

    std::list<symbol::Symbol>& Context::get_funlist(const std::wstring& _stModuleName)
    {
        return EnvFunTable.get_funlist(_stModuleName);
    }

    bool Context::put(const symbol::Symbol& key, types::InternalType &type)
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

    bool Context::remove(const symbol::Symbol& key)
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

    bool Context::put_in_previous_scope(const symbol::Symbol& key, types::InternalType &type)
    {
        // FIXME
        EnvVarTable.put_in_previous_scope(key, type);
        return true;
    }

    bool Context::AddFunction(types::Function *_info)
    {
        EnvFunTable.put(symbol::Symbol(_info->getName()), *_info);
        return true;
    }

    bool Context::AddMacro(types::Macro *_info)
    {
        EnvFunTable.put(symbol::Symbol(_info->getName()), *_info);
        return true;
    }

    bool Context::AddMacroFile(types::MacroFile *_info)
    {
        EnvFunTable.put(symbol::Symbol(_info->getName()), *_info);
        return true;
    }

    bool Context::isGlobalVisible(const symbol::Symbol& key) const
    {
        return HeapVarTable.isGlobalVisible(key);
    }

    /*return global variable, search in global scope ( highest )*/
    types::InternalType* Context::getGlobalValue(const symbol::Symbol& key) const
    {
        return HeapVarTable.getGlobalValue(key);
    }

    /*return global variable existance status*/
    bool Context::isGlobalExists(const symbol::Symbol& key) const
    {
        return HeapVarTable.isGlobalExists(key);
    }

    void Context::setGlobalValue(const symbol::Symbol& key, types::InternalType &value)
    {
        HeapVarTable.setGlobalValue(key, value);
    }

    void Context::createEmptyGlobalValue(const symbol::Symbol& key)
    {
        HeapVarTable.createEmptyGlobalValue(key);
    }

    void Context::setGlobalVisible(const symbol::Symbol& key, bool bVisible)
    {
        HeapVarTable.setGlobalVisible(key, bVisible);
    }

    void Context::removeGlobal(const symbol::Symbol& key)
    {
        HeapVarTable.removeGlobal(key);
    }

    void Context::removeGlobalAll()
    {
        HeapVarTable.removeGlobalAll();
    }

}

