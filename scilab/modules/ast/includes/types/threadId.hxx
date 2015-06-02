/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __THREADID_HH__
#define __THREADID_HH__

#include "internal.hxx"

extern "C"
{
#include "Thread_Wrapper.h"
}

namespace types
{
class EXTERN_AST ThreadId : public InternalType
{
public :
    ThreadId(__threadId _id, __threadKey _key);
    virtual                 ~ThreadId();

    bool                    isThreadId()
    {
        return true;
    }
    bool                    toString(std::wostringstream& ostr);

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring    getTypeStr()
    {
        return L"ThreadId";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring    getShortTypeStr()
    {
        return L"tid";
    }
    InternalType*           clone();

    __threadId              getThreadId();
    void                    setThreadId(__threadId _id);
    __threadKey             getKey();
    void                    setKey(__threadKey _key);

    /*
    ** Status
    */
    enum Status
    {
        Running,
        Paused,
        Aborted,
        Done
    };

    Status                  getStatus();
    void                    setStatus(Status _status);

    void                    suspend();
    void                    resume();
    void                    abort();

    void                    setInterrupt(bool _doInterrupt);
    bool                    getInterrupt();

    void                    setInterruptible(bool _isInterruptible);
    bool                    isInterruptible();

    void                    setConsoleCommandFlag(bool _isConsoleCommand);
    bool                    isConsoleCommand();

private :
    std::wstring            StatusToString(Status _status);

protected :
    inline ScilabType       getType()
    {
        return ScilabThreadId;
    }
    inline ScilabId         getId()
    {
        return IdThreadId;
    }


private :
    __threadId              m_threadId;
    __threadKey             m_threadKey;
    __threadLock            m_threadLock;
    Status                  m_threadStatus;
    bool                    m_doInterrupt;
    bool                    m_isInterruptible;
    bool                    m_isConsoleCommand;
};
}
#endif /* !__THREADID_HH__ */
