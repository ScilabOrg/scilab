/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __NESTED_BLOCKS_CHECKER_HXX__
#define __NESTED_BLOCKS_CHECKER_HXX__

#include <vector>

#include "SLintChecker.hxx"

namespace slint
{

class NestedBlocksChecker : public SLintChecker
{

    std::vector<const ast::Exp *> stack;
    const unsigned int max;

public:

    NestedBlocksChecker(const int _max) : max(_max >= 0 ? _max : 0) { }
    ~NestedBlocksChecker() { }

    void preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    void postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    const std::wstring getName() const;

};

} // namespace slint

#endif // __NESTED_BLOCKS_CHECKER_HXX__
