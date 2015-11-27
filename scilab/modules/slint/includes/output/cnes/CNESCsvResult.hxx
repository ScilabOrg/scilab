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

#ifndef __SLINT_CNES_CSV_RESULT_HXX__
#define __SLINT_CNES_CSV_RESULT_HXX__

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "config/cnes/ToolConfiguration.hxx"
#include "output/SLintResult.hxx"

namespace types
{
class String;
}

namespace slint
{

class SciFile;

namespace CNES
{

class CNESCsvResult : public SLintResult
{
    const ToolConfigurationType tct;
    const std::wstring path;
    std::ofstream * out;
    SciFilePtr current;
    std::unordered_map<std::wstring, std::vector<std::pair<Location, std::wstring>>> res;

public:

    CNESCsvResult(const ToolConfiguration & tc, types::String * conf, const std::wstring & id, const std::wstring & _path);

    virtual ~CNESCsvResult();
    virtual void handleFiles(const std::vector<SciFilePtr> & files);
    virtual void handleMessage(SLintContext & context, const Location & loc, const SLintChecker & checker, const unsigned sub, const std::wstring & msg);
    virtual void finalize();

private:

    void printRes();
    const std::string getStr(const std::wstring & str);
    const std::string getStr(const std::string & str);
};

} // namespace CNES

} // namespace slint

#endif // __SLINT_CNES_CSV_RESULT_HXX__
