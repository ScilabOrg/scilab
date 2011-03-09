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
#include <list>
#include "configvariable.hxx"

/*
** Module List
** \{
*/
std::list<std::wstring> ConfigVariable::m_ModuleList;

void ConfigVariable::setModuleList(std::list<std::wstring>& _pModule_list)
{
    m_ModuleList.clear();

    std::list<std::wstring>::iterator it;
    for(it = _pModule_list.begin() ; it != _pModule_list.end() ; it++)
    {
        m_ModuleList.push_back(*it);
    }
}

std::list<std::wstring> ConfigVariable::getModuleList()
{
    std::list<std::wstring> moduleList;
    std::list<std::wstring>::iterator it;
    for(it = m_ModuleList.begin() ; it != m_ModuleList.end() ; it++)
    {
        moduleList.push_back(*it);
    }
    return moduleList;
}
/*
** \}
*/

/*
** SCI
** \{
*/
std::wstring ConfigVariable::m_SCIPath;

void ConfigVariable::setSCIPath(std::wstring& _SCIPath)
{
    m_SCIPath = _SCIPath;
}

std::wstring ConfigVariable::getSCIPath()
{
    return m_SCIPath;
}

/*
** \}
*/

/*
** SCIHOME
** \{
*/

std::wstring ConfigVariable::m_SCIHOME;

void ConfigVariable::setSCIHOME(std::wstring& _SCIHOME)
{
    m_SCIHOME = _SCIHOME;
}

std::wstring ConfigVariable::getSCIHOME()
{
    return m_SCIHOME;
}
/*
** \}
*/

/*
** TMPDIR
** \{
*/

std::wstring ConfigVariable::m_TMPDIR;

void ConfigVariable::setTMPDIR(std::wstring& _TMPDIR)
{
    m_TMPDIR = _TMPDIR;
}

std::wstring ConfigVariable::getTMPDIR()
{
    return m_TMPDIR;
}
/*
** \}
*/

/*
** Force Quit
** \{
*/
bool ConfigVariable::m_bForceQuit = false;

void ConfigVariable::setForceQuit(bool _bForceQuit)
{
    m_bForceQuit = _bForceQuit;
}

bool ConfigVariable::getForceQuit(void)
{
    return m_bForceQuit;
}
/*
** \}
*/

/*
** Exit Status
** \{
*/
int ConfigVariable::m_iExitStatus = 0;

void ConfigVariable::setExitStatus(int _iExitStatus)
{
    m_iExitStatus = _iExitStatus;
}

int ConfigVariable::getExitStatus(void)
{
    return m_iExitStatus;
}
/*
** \}
*/

/*
** Digit precision, ex format function
** \{
*/

int ConfigVariable::m_iFormat = 0;

void ConfigVariable::setFormat(int _iFormat)
{
    m_iFormat = _iFormat;
}

int ConfigVariable::getFormat(void)
{
    return m_iFormat;
}

/*
** \}
*/

/*
** Screen console width
** \{
*/

int ConfigVariable::m_iConsoleWidth = 0;

void ConfigVariable::setConsoleWidth(int _iConsoleWidth)
{
    m_iConsoleWidth = _iConsoleWidth;
}

int ConfigVariable::getConsoleWidth(void)
{
    return m_iConsoleWidth;
}
/*
** \}
*/

/*
** Scilab mode
** \{
*/

int ConfigVariable::m_iScilabMode = 0;

void ConfigVariable::setScilabMode(int _iScilabMode)
{
    m_iScilabMode = _iScilabMode;
}

int ConfigVariable::getScilabMode(void)
{
    return m_iScilabMode;
}
/*
** \}
*/

/*
** Warning Mode
** \{
*/
bool ConfigVariable::m_bWarningMode = true;

void ConfigVariable::setWarningMode(bool _bWarningMode)
{
    m_bWarningMode = _bWarningMode;
}

bool ConfigVariable::getWarningMode(void)
{
    return m_bWarningMode;
}
/*
** \}
*/

/*
** HOME
** \{
*/

std::wstring ConfigVariable::m_HOME;

void ConfigVariable::setHOME(std::wstring& _HOME)
{
    m_HOME = _HOME;
}

std::wstring ConfigVariable::getHOME()
{
    return m_HOME;
}
/*
** \}
*/

/*
** Clear last error information
** \{
*/
bool ConfigVariable::m_bLastErrorCall = false;

void ConfigVariable::setLastErrorCall(void)
{
    m_bLastErrorCall = true;
}

void ConfigVariable::clearLastError(void)
{
    if(m_bLastErrorCall == false)
    {
        m_wstError          = L"";
        m_iError            = 0;
        m_iErrorLine        = 0;
        m_wstErrorFunction  = L"";
    }
    m_bLastErrorCall = false;
}
/*
** \}
*/

/*
** Last Error Message
** \{
*/

std::wstring ConfigVariable::m_wstError;

void ConfigVariable::setLastErrorMessage(std::wstring _wstError)
{
    m_wstError = _wstError;
}

std::wstring ConfigVariable::getLastErrorMessage()
{
    return m_wstError;
}
/*
** \}
*/

/*
** Last Error ID
** \{
*/
int ConfigVariable::m_iError = 0;

void ConfigVariable::setLastErrorNumber(int _iError)
{
    m_iError = _iError;
}

int ConfigVariable::getLastErrorNumber(void)
{
    return m_iError;
}
/*
** \}
*/

/*
** Last Error Line
** \{
*/
int ConfigVariable::m_iErrorLine = 0;

void ConfigVariable::setLastErrorLine(int _iErrorLine)
{
    m_iErrorLine = _iErrorLine;
}

int ConfigVariable::getLastErrorLine(void)
{
    return m_iErrorLine;
}
/*
** \}
*/

/*
** Last Error Function
** \{
*/

std::wstring ConfigVariable::m_wstErrorFunction;

void ConfigVariable::setLastErrorFunction(std::wstring _wstErrorFunction)
{
    m_wstErrorFunction = _wstErrorFunction;
}

std::wstring ConfigVariable::getLastErrorFunction()
{
    return m_wstErrorFunction;
}

/*
** \}
*/

/*
** Prompt Mode
** \{
*/

int ConfigVariable::m_iPromptMode = 0;

void ConfigVariable::setPromptMode(int _iPromptMode)
{
    m_iPromptMode = _iPromptMode;
}

int ConfigVariable::getPromptMode(void)
{
    return m_iPromptMode;
}

/*
** ThreadList
** \{
*/

std::map<__threadKey, types::ThreadId *> ConfigVariable::m_threadList;

types::ThreadId* ConfigVariable::getThread(__threadKey _key)
{
    std::map<__threadKey, types::ThreadId *>::const_iterator it;
    it = m_threadList.find(_key);
    if(it == m_threadList.end())
    {
        return NULL;
    }
    return it->second;
}

types::Cell* ConfigVariable::getAllThreads(void)
{
    int iSize = (int) ConfigVariable::m_threadList.size();

    if (iSize == 0)
    {
        return new types::Cell();
    }

    int i = 0;
    types::Cell *pcResult = new types::Cell(iSize, 1);
    std::map<__threadKey, types::ThreadId *>::iterator it;

    for (it = ConfigVariable::m_threadList.begin() ; it != ConfigVariable::m_threadList.end() ; ++it, ++i)
    {
        pcResult->set(i, 0, it->second);
    }

    return pcResult;
}


void ConfigVariable::setThread(__threadKey _key, types::ThreadId* _thread)
{
    ConfigVariable::deleteThread(_key);
    _thread->IncreaseRef();
    m_threadList[_key] = _thread;
}

void ConfigVariable::deleteThread(__threadKey _key)
{
    std::map<__threadKey, types::ThreadId *>::const_iterator it;
    it = m_threadList.find(_key);
    if(it != m_threadList.end())
    {
        it->second->DecreaseRef();
        if(it->second->isDeletable())
        {
            delete it->second;
        }
        m_threadList[_key] = NULL;
        m_threadList.erase(_key);
    }
}

/*
** \}
*/

/*
** Prompt Mode
** \{
*/

int ConfigVariable::m_iPauseLevel = 0;

void ConfigVariable::IncreasePauseLevel()
{
    m_iPauseLevel++;
}

void ConfigVariable::DecreasePauseLevel()
{
    m_iPauseLevel--;
}

int ConfigVariable::getPauseLevel()
{
    return m_iPauseLevel;
}

/*
** \}
*/
