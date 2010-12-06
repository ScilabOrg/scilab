/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <map>
#include <sstream>
#include "core_math.h"
#include "cell.hxx"
#include "threadId.hxx"
#include "tostring_common.hxx"

extern "C"
{
#include "charEncoding.h"
#include "os_wcsdup.h"
}

namespace types
{

    std::map<__threadId, ThreadId *>    ThreadId::m_threadList;

    ThreadId*       ThreadId::createThreadId(__threadId _id)
    {
        ThreadId* ptidThread = m_threadList[_id];

        if(ptidThread == NULL)
        {
            ptidThread = new ThreadId(_id);
            m_threadList[_id] = ptidThread;
        }


        return ptidThread;
    }

	ThreadId::~ThreadId() { }

	ThreadId::ThreadId(__threadId _id)
	{
        m_threadId = _id;
        m_threadStatus = Running;
	}

    __threadId ThreadId::getId()
    {
        return m_threadId;
    }

    void ThreadId::setId(__threadId _id)
    {
        this->m_threadId = _id;
    }

	ThreadId *ThreadId::clone()
	{
        return this;
	}

	ThreadId* ThreadId::getAsThreadId(void)
	{
		return this;
	}

	GenericType::RealType ThreadId::getType()
	{
		return GenericType::RealThreadId;
	}

    std::wstring ThreadId::StatusToString(Status _status)
    {
        switch(_status)
        {
        case Running :
            return L"Running";
        case Paused :
            return L"Paused";
        case Aborted :
            return L"Aborted";
        case Done :
            return L"Done";
        }
    }

    void ThreadId::setStatus(ThreadId::Status _status)
    {
        m_threadStatus = _status;
    }

    ThreadId::Status ThreadId::getStatus(void)
    {
        return m_threadStatus;
    }

	wstring ThreadId::toString(int _iPrecision, int _iLineLen)
	{
		wostringstream ostr;

        ostr << L"ThreadId : " << this << std::endl;
        ostr << L"Status : " << StatusToString(this->getStatus());

        return ostr.str();
    }

    void ThreadId::checkIn(__threadId _id)
    {
        ThreadId* ptidThread = new ThreadId(_id);
        m_threadList[_id] = ptidThread;
    }

    void ThreadId::checkOut(__threadId _id)
    {
        ThreadId *ptidThread = m_threadList[_id];
        ptidThread->setStatus(Done);
        //m_threadList[_id]->setStatus(Done);
        m_threadList[_id] = NULL;
        m_threadList.erase(_id);
    }

    Cell* ThreadId::getThreads(void)
    {
        int iSize = (int) m_threadList.size();

        if (iSize == 0)
        {
            return new Cell();
        }

        int i = 0;
        Cell *pcResult = new Cell(iSize, 1);
        std::map<__threadId, ThreadId *>::iterator it;

        for (it = m_threadList.begin() ; it != m_threadList.end() ; ++it, ++i)
        {
            pcResult->set(i, 0, it->second);
        }

        return pcResult;
    }

}

