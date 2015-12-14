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

#ifndef __CONSTRAINT_MANAGER_HXX__
#define __CONSTRAINT_MANAGER_HXX__

#include <iostream>
#include <memory>
#include <string>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "InferenceConstraint.hxx"
#include "dynlib_ast.h"

namespace analysis
{

class FunctionBlock;

class EXTERN_AST ConstraintManager
{

public:

    typedef std::unordered_set<MPolyConstraintSet, MPolyConstraintSet::Hash, MPolyConstraintSet::Eq> UnverifiedSet;

private:

    ConstraintManager * parent;
    FunctionBlock * function;
    MPolyConstraintSet verified;
    std::set<symbol::Symbol> constantConstraints;
    UnverifiedSet unverified;

    static std::vector<std::shared_ptr<InferenceConstraint>> generalConstraints;

public:

    enum Kind { SAMEDIMS = 0, EQUAL, POSITIVE, STRICT_POSITIVE, GREATER, STRICT_GREATER, VALID_INDEX, VALID_RANGE, COUNT };

    ConstraintManager(FunctionBlock * _function, FunctionBlock * _parent = nullptr);
    ConstraintManager(FunctionBlock & _function, FunctionBlock * _parent = nullptr) : ConstraintManager(&_function, _parent) { }
    ConstraintManager() : ConstraintManager(nullptr, nullptr) { }

    inline bool isRoot() const
    {
        return parent == nullptr;
    }

    inline const MPolyConstraintSet & getVerifiedConstraints() const
    {
        return verified;
    }

    inline const UnverifiedSet & getUnverifiedConstraints() const
    {
        return unverified;
    }

    inline const std::set<symbol::Symbol> & getGlobalConstants() const
    {
        return constantConstraints;
    }

    bool check(const MPolyConstraintSet & set, const std::vector<GVN::Value *> & values);
    bool check(Kind kind, const std::vector<GVN::Value *> & values);
    bool checkGlobalConstant(const symbol::Symbol & sym);

    friend std::wostream & operator<<(std::wostream & out, const ConstraintManager & cm);

    static bool checkGlobalConstants(const std::set<symbol::Symbol> & gc);

    template<typename... Args>
    inline bool check(Kind kind, Args... args)
    {
        std::vector<GVN::Value *> v;
        return __check(kind, v, args...);
    }

private:

    inline bool __check(Kind kind, const std::vector<GVN::Value *> & v)
    {
        return check(kind, v);
    }

    template<typename... Args>
    inline bool __check(Kind kind, std::vector<GVN::Value *> & v, GVN::Value & val, Args... args)
    {
        v.emplace_back(&val);
        return __check(kind, v, args...);
    }

    template<typename... Args>
    inline bool __check(Kind kind, std::vector<GVN::Value *> & v, GVN::Value * val, Args... args)
    {
        v.emplace_back(val);
        return __check(kind, v, args...);
    }

    static std::vector<std::shared_ptr<InferenceConstraint>> init();

};

} // namespace analysis

#endif // __CONSTRAINT_MANAGER_HXX__
