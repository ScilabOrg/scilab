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

#ifndef __COVER_MODULE_HXX__
#define __COVER_MODULE_HXX__

#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Counter.hxx"
#include "InstrumentVisitor.hxx"
#include "CoverMacroInfo.hxx"
#include "CoverResult.hxx"

namespace coverage
{

class CoverModule
{
    std::vector<Counter> counters;
    std::unordered_map<types::Callable *, CallCounter> callCounters;
    std::unordered_map<types::Macro *, CoverMacroInfo> macros;

    // { moduleName => functions }
    std::unordered_multimap<std::wstring, types::Callable *> functions;

    // { moduleName => { macroFilename => { macroName => CoverResult } } }
    std::unordered_map<std::wstring, std::unordered_map<std::wstring, std::unordered_map<std::wstring, CoverResult>>> results;

    InstrumentVisitor visitor;
    std::size_t totalInstrs;
    std::size_t totalBranches;
    std::size_t totalUncInstrs;
    std::size_t totalUncBranches;

    static CoverModule * instance;

    CoverModule(const std::vector<std::pair<std::wstring, std::wstring>> & paths_mods) : visitor(*this), totalInstrs(0), totalBranches(0), totalUncInstrs(0), totalUncBranches(0)
    {
        getMacros(paths_mods);
        getBuiltins(paths_mods);
    }

public:

    inline static void createInstance(const std::vector<std::pair<std::wstring, std::wstring>> & paths_mods)
    {
        delete instance;
        instance = new CoverModule(paths_mods);
    }

    inline static CoverModule * getInstance()
    {
        return instance;
    }

    void add(types::Macro * macro, ast::Exp * e);
    void invoke(types::Callable * f);
    void invoke(const uint64_t id);
    void print();
    void collect();
    void toHTML(const std::wstring & outputDir);

    inline bool isCovered(types::Callable * f) const
    {
        return callCounters.find(f) != callCounters.end();
    }

    inline void startChrono(const uint64_t id)
    {
        counters[id - 2].startChrono();
    }

    inline void stopChrono(const uint64_t id)
    {
        counters[id - 2].stopChrono();
    }

    inline static void invoke(const ast::Exp & e)
    {
        if (e.getCoverId() && instance)
        {
            instance->invoke(e.getCoverId());
        }
    }

    inline static void startChrono(const ast::Exp & e)
    {
        if (e.getCoverId() && instance)
        {
            instance->startChrono(e.getCoverId());
        }
    }

    inline static void stopChrono(const ast::Exp & e)
    {
        if (e.getCoverId() && instance)
        {
            instance->stopChrono(e.getCoverId());
        }
    }

private:

    void getMacros(const std::vector<std::pair<std::wstring, std::wstring>> & paths_mods);
    void getBuiltins(const std::vector<std::pair<std::wstring, std::wstring>> & paths_mods);
    void instrumentMacro(const std::wstring & module, const std::wstring & path, types::Macro * macro);
    void instrumentSingleMacro(const std::wstring & module, const std::wstring & path, types::Macro * macro);
    std::vector<std::pair<types::Callable *, uint64_t>> getFunctionCalls(const std::wstring & moduleName, const bool builtin) const;

    static bool getStringFromXPath(char * filePath, const char * xpquery, std::unordered_set<std::wstring> & set);
    static void copyDataFiles(const std::wstring & outputDir);
    static void copyFile(const std::wstring & inDir, const std::wstring & outDir, const std::wstring & filename);
    static void writeFile(const std::wostringstream & out, const std::wstring & outputDir, const std::wstring & filename);
};

} // namespace coverage

#endif // __COVER_MODULE_HXX__
