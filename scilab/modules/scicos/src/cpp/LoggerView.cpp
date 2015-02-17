/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2014 - Scilab Enterprises - Clement DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <iostream>
#include <sstream>
#include <cwchar>

#include "scilabWrite.hxx"

#include "LoggerView.hxx"
#include "Controller.hxx"

namespace org_scilab_modules_scicos
{

static const bool USE_SCILAB_WRITE = true;

LoggerView::LoggerView() :
    View(), m_level(LOG_WARNING)
{
}

LoggerView::~LoggerView()
{
}

static std::wstring levelTable[] =
{
    L"TRACE",
    L"DEBUG",
    L"INFO",
    L"WARNING",
    L"ERROR",
    L"FATAL",
};

enum LogLevel LoggerView::indexOf(const wchar_t* name)
{
    for (int i = LOG_TRACE; i <= LOG_FATAL; i++)
    {
        if (!wcscmp(name, levelTable[i].data()))
        {
            return static_cast<enum LogLevel>(i);
        }
    }
    return LOG_UNDEF;
}

const wchar_t* LoggerView::toString(enum LogLevel level)
{
    if (LOG_TRACE <= level && level <= LOG_FATAL)
    {
        return levelTable[level].data();
    }
    return L"";
}

void LoggerView::log(enum LogLevel level, const std::stringstream& msg)
{
    if (level >= this->m_level)
    {
        std::string str = msg.str();
        if (USE_SCILAB_WRITE)
        {
            scilabForcedWrite(str.data());
        }
        else
        {
            std::cerr << str;
        }
    }
}

void LoggerView::log(enum LogLevel level, const char* msg)
{
    if (level >= this->m_level)
    {
        if (USE_SCILAB_WRITE)
        {
            scilabForcedWrite(msg);
        }
        else
        {
            std::wcerr << msg;
        }
    }
}

void LoggerView::log(enum LogLevel level, const wchar_t* msg)
{
    if (level >= this->m_level)
    {
        if (USE_SCILAB_WRITE)
        {
            scilabForcedWriteW(msg);
        }
        else
        {
            std::wcerr << msg;
        }
    }
}

// generated with :
// awk ' $2 == "//!<" {sub(",","", $1); print "case " $1 ":\n    os << \"" $1 "\";\n    break;" }' ~/work/branches/YaSp/scilab/modules/scicos/includes/utilities.hxx

std::ostream& operator<<(std::ostream& os, update_status_t u)
{
    switch (u)
    {
        case SUCCESS:
            os << "SUCCESS";
            break;
        case NO_CHANGES:
            os << "NO_CHANGES";
            break;
        case FAIL:
            os << "FAIL";
            break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, kind_t k)
{
    switch (k)
    {
        case ANNOTATION:
            os << "ANNOTATION";
            break;
        case BLOCK:
            os << "BLOCK";
            break;
        case DIAGRAM:
            os << "DIAGRAM";
            break;
        case LINK:
            os << "LINK";
            break;
        case PORT:
            os << "PORT";
            break;
    }
    return os;
}


std::ostream& operator<<(std::ostream& os, object_properties_t p)
{
    switch (p)
    {
        case PARENT_DIAGRAM:
            os << "PARENT_DIAGRAM";
            break;
        case GEOMETRY:
            os << "GEOMETRY";
            break;
        case DESCRIPTION:
            os << "DESCRIPTION";
            break;
        case FONT:
            os << "FONT";
            break;
        case FONT_SIZE:
            os << "FONT_SIZE";
            break;
        case RELATED_TO:
            os << "RELATED_TO";
            break;
        case INTERFACE_FUNCTION:
            os << "INTERFACE_FUNCTION";
            break;
        case SIM_FUNCTION_NAME:
            os << "SIM_FUNCTION_NAME";
            break;
        case SIM_FUNCTION_API:
            os << "SIM_FUNCTION_API";
            break;
        case SIM_SCHEDULE:
            os << "SIM_SCHEDULE";
            break;
        case SIM_BLOCKTYPE:
            os << "SIM_BLOCKTYPE";
            break;
        case SIM_DEP_UT:
            os << "SIM_DEP_UT";
            break;
        case ANGLE:
            os << "ANGLE";
            break;
        case EXPRS:
            os << "EXPRS";
            break;
        case INPUTS:
            os << "INPUTS";
            break;
        case OUTPUTS:
            os << "OUTPUTS";
            break;
        case EVENT_INPUTS:
            os << "EVENT_INPUTS";
            break;
        case EVENT_OUTPUTS:
            os << "EVENT_OUTPUTS";
            break;
        case STATE:
            os << "STATE";
            break;
        case DSTATE:
            os << "DSTATE";
            break;
        case ODSTATE:
            os << "ODSTATE";
            break;
        case NZCROSS:
            os << "NZCROSS";
            break;
        case NMODE:
            os << "NMODE";
            break;
        case RPAR:
            os << "RPAR";
            break;
        case IPAR:
            os << "IPAR";
            break;
        case OPAR:
            os << "OPAR";
            break;
        case EQUATIONS:
            os << "EQUATIONS";
            break;
        case UID:
            os << "UID";
            break;
        case PARENT_BLOCK:
            os << "PARENT_BLOCK";
            break;
        case CHILDREN:
            os << "CHILDREN";
            break;
        case PORT_REFERENCE:
            os << "PORT_REFERENCE";
            break;
        case STYLE:
            os << "STYLE";
            break;
        case LABEL:
            os << "LABEL";
            break;
        case DESTINATION_PORT:
            os << "DESTINATION_PORT";
            break;
        case SOURCE_PORT:
            os << "SOURCE_PORT";
            break;
        case CONTROL_POINTS:
            os << "CONTROL_POINTS";
            break;
        case THICK:
            os << "THICK";
            break;
        case COLOR:
            os << "COLOR";
            break;
        case KIND:
            os << "KIND";
            break;
        case FROM:
            os << "FROM";
            break;
        case TO:
            os << "TO";
            break;
        case DATATYPE:
            os << "DATATYPE";
            break;
        case DATATYPE_ROWS:
            os << "DATATYPE_ROWS";
            break;
        case DATATYPE_COLS:
            os << "DATATYPE_COLS";
            break;
        case DATATYPE_TYPE:
            os << "DATATYPE_TYPE";
            break;
        case FIRING:
            os << "FIRING";
            break;
        case SOURCE_BLOCK:
            os << "SOURCE_BLOCK";
            break;
        case PORT_KIND:
            os << "PORT_KIND";
            break;
        case IMPLICIT:
            os << "IMPLICIT";
            break;
        case PORT_NUMBER:
            os << "PORT_NUMBER";
            break;
        case CONNECTED_SIGNALS:
            os << "CONNECTED_SIGNALS";
            break;
        case TITLE:
            os << "TITLE";
            break;
        case PATH:
            os << "PATH";
            break;
        case PROPERTIES:
            os << "PROPERTIES";
            break;
        case DIAGRAM_CONTEXT:
            os << "CONTEXT";
            break;
        case VERSION_NUMBER:
            os << "VERSION_NUMBER";
            break;
        default:
            break;
    }
    return os;
}

void LoggerView::objectCreated(const ScicosID& uid, kind_t k)
{
    std::stringstream ss;
    ss << __FUNCTION__ << "( " << uid << " , " << k << " )" << std::endl;
    log(LOG_DEBUG, ss);
}

void LoggerView::objectDeleted(const ScicosID& uid, kind_t k)
{
    std::stringstream ss;
    ss << __FUNCTION__ << "( " << uid << " , " << k << " )" << std::endl;
    log(LOG_DEBUG, ss);
}

void LoggerView::objectUpdated(const ScicosID& uid, kind_t k)
{
    std::stringstream ss;
    ss << __FUNCTION__ << "( " << uid << " , " << k << " )" << std::endl;
    log(LOG_DEBUG, ss);
}

void LoggerView::propertyUpdated(const ScicosID& /*uid*/, kind_t /*k*/, object_properties_t /*p*/)
{
    // do not log anything on success; the message has already been logged
}

void LoggerView::propertyUpdated(const ScicosID& uid, kind_t k, object_properties_t p,
                                 update_status_t u)
{
    std::stringstream ss;
    ss << __FUNCTION__ << "( " << uid << " , " << k << " , " << p << " ) : " << u << std::endl;
    log(LOG_TRACE, ss);
}

} /* namespace org_scilab_modules_scicos */
