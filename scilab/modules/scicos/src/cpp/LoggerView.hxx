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

#ifndef LOGGERVIEW_HXX_
#define LOGGERVIEW_HXX_

#include <sstream>
#include "View.hxx"

namespace org_scilab_modules_scicos
{

enum LogLevel
{
    LOG_UNDEF = -1,   //!< Undefined value
    LOG_TRACE = 0,    //!< more detailed information. Expect these to be written to logs only.
    LOG_DEBUG = 1,    //!< detailed information on the flow through the system. Expect these to be written to logs only.
    LOG_INFO = 2,    //!< Interesting runtime events (startup/shutdown). Expect these to be immediately visible on a console, so be conservative and keep to a minimum.
    LOG_WARNING = 3,    //!<
    LOG_ERROR = 4,    //!< Other runtime errors or unexpected conditions. Expect these to be immediately visible on a status console.
    LOG_FATAL = 5,    //!< Severe errors that cause premature termination. Expect these to be immediately visible on a status console.
};

class LoggerView: public View
{
public:
    LoggerView();
    ~LoggerView();

    /*
     * Implement a classical Logger interface
     */

    static enum LogLevel indexOf(const wchar_t* name);
    static const wchar_t* toString(enum LogLevel level);

    enum LogLevel getLevel() const
    {
        return level;
    }
    void setLevel(enum LogLevel level)
    {
        this->level = level;
    }

    void log(enum LogLevel level, const std::stringstream& msg);
    void log(enum LogLevel level, const char* msg);
    void log(enum LogLevel level, const wchar_t* msg);

    /*
     * Implement the Logger as a View
     */

    void objectCreated(const ScicosID& uid, kind_t k);
    void objectDeleted(const ScicosID& uid, kind_t k);
    void propertyUpdated(const ScicosID& uid, kind_t k, object_properties_t p, update_status_t u);

private:
    enum LogLevel level;
};

} /* namespace org_scilab_modules_scicos */

#endif /* LOGGERVIEW_HXX_ */
