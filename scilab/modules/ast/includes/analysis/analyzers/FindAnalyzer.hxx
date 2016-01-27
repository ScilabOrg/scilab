/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 * === LICENSE_END ===
 *
 */

#ifndef __FIND_ANALYZER_HXX__
#define __FIND_ANALYZER_HXX__

#include "CallAnalyzer.hxx"

namespace analysis
{

class FindAnalyzer : public CallAnalyzer
{

public:

    FindAnalyzer() : CallAnalyzer() { }

    bool analyze(AnalysisVisitor & visitor, const unsigned int lhs, ast::CallExp & e);
};

} // namespace analysis

#endif // __FIND_ANALYZER_HXX__
