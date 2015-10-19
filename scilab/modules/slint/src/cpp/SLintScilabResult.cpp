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

#include <string>

#include "struct.hxx"
#include "string.hxx"
#include "double.hxx"
#include "scilabWrite.hxx"
#include "output/SLintScilabResult.hxx"
#include "checkers/SLintChecker.hxx"

namespace slint
{

SLintScilabResult::SLintScilabResult() { }
SLintScilabResult::~SLintScilabResult() { }

void SLintScilabResult::handleMessage(SLintContext & context, const Location & loc, const SLintChecker & checker, const std::wstring & msg)
{
    auto & mmap = results[context.getFilename()];
    mmap.emplace(loc, checker.getId() + L": " + msg);
}

void SLintScilabResult::finalize()
{
    for (const auto & p1 : results)
    {
        std::wstring str = L"In " + p1.first + L":\n";
        scilabWriteW(str.c_str());
        for (const auto & p2 : p1.second)
        {
            std::wstring str = L"  At line " + std::to_wstring(p2.first.first_line) + L": " + p2.second + L"\n";
            scilabWriteW(str.c_str());
        }
    }
}

SLintScilabOut::SLintScilabOut() { }
SLintScilabOut::~SLintScilabOut() { }

void SLintScilabOut::handleMessage(SLintContext & context, const Location & loc, const SLintChecker & checker, const std::wstring & msg)
{
    results[context.getFilename()][checker.getId()].emplace_back(loc, msg);
}

void SLintScilabOut::finalize()
{

}

types::Struct * SLintScilabOut::getStruct() const
{
    /**
     * struct { file : string;
                info : struct { id1 : struct { loc : double[2,2];
    	                                   msg : string;
    					 };
    			    id2 : struct { loc : double[2,2];
    	                                   msg : string;
    					 };
    			    ...
    			   };
    	  }

     */

    types::Struct * st = new types::Struct(results.size(), 1);
    st->addField(L"file");
    st->addField(L"info");
    int index = 0;
    for (const auto & p1 : results)
    {
        types::SingleStruct * sst = st->get(index++);
        types::Struct * info = new types::Struct(1, 1);
        sst->set(L"file", new types::String(p1.first.c_str()));
        sst->set(L"info", info);
        types::SingleStruct * sst_info = info->get(0);

        for (const auto & p2 : p1.second)
        {
            // p2: { Id => { Location, Msg } }
            const std::wstring & id = p2.first;
            const std::vector<std::pair<Location, std::wstring>> & infos = p2.second;

            if (!infos.empty())
            {
                info->addField(id);
                types::Struct * loc_msg = new types::Struct(infos.size(), 1);
                sst_info->set(id, loc_msg);
                loc_msg->addField(L"loc");
                loc_msg->addField(L"msg");

                int index_info = 0;
                for (const auto & p3 : infos)
                {
                    double * loc = nullptr;
                    types::Double * pDbl = new types::Double(2, 2, &loc);
                    loc[0] = p3.first.first_line;
                    loc[1] = p3.first.last_line;
                    loc[2] = p3.first.first_column;
                    loc[3] = p3.first.last_column;
                    loc_msg->get(index_info)->set(L"loc", pDbl);
                    loc_msg->get(index_info++)->set(L"msg", new types::String(p3.second.c_str()));
                }
            }
        }
    }

    return st;
}

} // namespace slint
