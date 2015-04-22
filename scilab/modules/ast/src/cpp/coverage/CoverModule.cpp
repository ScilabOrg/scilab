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

#ifdef _MSC_VER
#pragma comment(lib, "../../bin/libxml2.lib")
#endif

#include <libxml/xpath.h>
#include <libxml/xmlreader.h>

#include "CoverModule.hxx"
#include "CovHTMLCodePrinter.hxx"
#include "cover_tools.hxx"
#include "allexp.hxx"
#include "allvar.hxx"
#include "alltypes.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "os_string.h"
#include "expandPathVariable.h"
#include "createdirectory.h"
#include "copyfile.h"
}

namespace coverage
{

CoverModule * CoverModule::instance = nullptr;

void CoverModule::getMacros(const std::vector<std::pair<std::wstring, std::wstring>> & paths_mods)
{
    for (const auto & p : paths_mods)
    {
        std::unordered_set<std::wstring> _macros;
        std::wstring _path = p.first + DIR_SEPARATORW + L"macros" + DIR_SEPARATORW + L"lib";
        wchar_t * pwstPathLib = expandPathVariableW((wchar_t *)_path.c_str());
        std::wstring libPath(pwstPathLib);
        FREE(pwstPathLib);

        char * libFile = wide_string_to_UTF8(libPath.c_str());

        if (getStringFromXPath(libFile, "//scilablib/macro/@name", _macros))
        {
            for (const auto & name : _macros)
            {
                types::InternalType * pIT = symbol::Context::getInstance()->get(symbol::Symbol(name));
                if (pIT && pIT->isMacroFile())
                {
                    types::MacroFile * pMF = static_cast<types::MacroFile *>(pIT);
                    if (types::Macro * macro = pMF->getMacro())
                    {
                        const std::wstring & file = pMF->getPath();
                        std::size_t pos = file.find_last_of(L'.');
                        if (pos != std::string::npos)
                        {
                            instrumentMacro(p.second, file.substr(0, pos) + L".sci", macro);
                        }
                        else
                        {
                            instrumentMacro(p.second, file, macro);
                        }
                    }
                }
            }
        }
        FREE(libFile);
    }
}

void CoverModule::getBuiltins(const std::vector<std::pair<std::wstring, std::wstring>> & paths_mods)
{
    for (const auto & p : paths_mods)
    {
        std::wstring _path = p.first + DIR_SEPARATORW + L"sci_gateway" + DIR_SEPARATORW + p.second + L"_gateway.xml";
        wchar_t * pwstPathMod = expandPathVariableW((wchar_t *)_path.c_str());
        std::wstring modPath(pwstPathMod);
        std::unordered_set<std::wstring> _builtins;
        FREE(pwstPathMod);

        char * modFile = wide_string_to_UTF8(modPath.c_str());
        if (getStringFromXPath(modFile, "//module/gateway/@function", _builtins) || getStringFromXPath(modFile, "//GATEWAY/PRIMITIVE/@primitiveName", _builtins))
        {
            for (const auto & name : _builtins)
            {
                types::InternalType * pIT = symbol::Context::getInstance()->get(symbol::Symbol(name));
                if (pIT && pIT->isFunction())
                {
                    pIT->IncreaseRef();
                    functions.emplace(p.second, static_cast<types::Function *>(pIT));
                    callCounters.emplace(static_cast<types::Callable *>(pIT), CallCounter());
                }
            }
        }
        FREE(modFile);
    }
}

void CoverModule::instrumentMacro(const std::wstring & module, const std::wstring & path, types::Macro * macro)
{
    const std::map<symbol::Variable *, types::Macro *> & submacros = macro->getSubMacros();
    for (const auto & p : submacros)
    {
        instrumentSingleMacro(module, path, p.second, true);
    }

    instrumentSingleMacro(module, path, macro, true);
}

void CoverModule::instrumentSingleMacro(const std::wstring & module, const std::wstring & path, types::Macro * macro, bool instrumentInners)
{
    macro->IncreaseRef();
    visitor.setMacro(macro);
    macro->getBody()->accept(visitor);
    macros.emplace(macro, CoverMacroInfo(module, path, visitor.getInstrsCount(), visitor.getBranchesCount(), visitor.getPathsCount()));
    callCounters.emplace(static_cast<types::Callable *>(macro), CallCounter());
    functions.emplace(module, static_cast<types::Callable *>(macro));

    if (instrumentInners)
    {
        // We make a copy since the call to instrumentSingleMacro will modify visitor.getInnerMacros()
        const std::vector<types::Macro *> inners = visitor.getInnerMacros();
        for (auto inner : inners)
        {
            instrumentSingleMacro(module, path, inner, true);
        }
    }
}

void CoverModule::add(types::Macro * macro, ast::Exp * e)
{
    if (e)
    {
        counters.emplace_back(macro, e);
        const std::size_t id = counters.size() + 1;
        e->setCoverId(id);
    }
}

void CoverModule::invoke(types::Callable * f)
{
    if (f->isMacroFile())
    {
        f = static_cast<types::MacroFile *>(f)->getMacro();
    }
    auto i = callCounters.find(f);
    if (i != callCounters.end())
    {
        i->second.inc();
    }
}

void CoverModule::invoke(const uint64_t id)
{
    counters[id - 2].inc();
}

bool CoverModule::getStringFromXPath(char * filePath, const char * xpquery, std::unordered_set<std::wstring> & set)
{
    xmlDocPtr doc = xmlReadFile(filePath, "utf-8", XML_PARSE_NOWARNING);
    if (!doc)
    {
        return false;
    }

    if (!doc->encoding || stricmp((const char *)doc->encoding, "utf-8") != 0)
    {
        xmlFreeDoc(doc);
        return false;
    }

    xmlXPathContextPtr ctxt = xmlXPathNewContext(doc);
    xmlXPathObjectPtr xp = xmlXPathEval((const xmlChar *)xpquery, ctxt);
    xmlNodeSetPtr nodeSet = xp->nodesetval;

    if (nodeSet)
    {
        for (unsigned int i = 0; i < nodeSet->nodeNr; ++i)
        {
            wchar_t * ws = to_wide_string((const char *)xmlNodeGetContent(nodeSet->nodeTab[i]));
            set.emplace(ws);
            FREE(ws);
        }
    }

    xmlXPathFreeObject(xp);
    xmlXPathFreeContext(ctxt);
    xmlFreeDoc(doc);

    return nodeSet;
}

void CoverModule::collect()
{
    if (!counters.empty())
    {
        std::vector<Counter>::const_iterator b = counters.begin();
        types::Macro * current = counters.front().getMacro();
        totalInstrs = 0;
        totalBranches = 0;
        totalUncInstrs = 0;
        totalUncBranches = 0;

        for (std::vector<Counter>::const_iterator i = std::next(b), e = counters.end(); i != e; ++i)
        {
            if (i->getMacro() != current)
            {
                const std::wstring & name = current->getName();
                CoverMacroInfo & info = macros.find(current)->second;
                auto & map1 = results[info.macroModule];
                auto & map2 = map1[info.macroFilePath];
                auto j = map2.find(name);
                if (j == map2.end())
                {
                    CoverResult & result = map2.emplace(name, CoverResult(name, info)).first->second;
                    result.populate(b, i);

                    result.setCounter(callCounters.find(current)->second.get());
                    b = i;
                    current = i->getMacro();

                    totalInstrs += result.getInfo().instrsCount;
                    totalBranches += result.getInfo().branchesCount;
                    totalUncInstrs += result.getUncInstrs();
                    totalUncBranches += result.getUncBranches();
                }
            }
        }

#ifdef _MSC_VER
        toHTML(L"d:\\test\\coverage\\out22");
#else
        toHTML(L"/tmp/out");
#endif
    }
}

void CoverModule::print()
{
    /*std::wcerr << L"Builtin calls" << std::endl << tools::getUnderline(L"Builtin calls") << std::endl;
      for (const auto & p : callCounters)
      {
      if (p.second.get())
      {
      std::wcerr << p.first->getName() << L" called " << p.second.get() << L" time" << tools::getPlural(p.second.get()) << L"." << std::endl;
      }
      }

      std::wcerr << std::endl;*/

    /*      for (const auto & counter : counters)
            {
            if (counter.get())
            {

            const std::wstring & name = counter.getMacro()->getName();
            std::wcerr << L"Macro " << name << L" visited at " << counter.getExp()->getLocation() << std::endl;


            //<< L"Seq not visited:" << std::endl;

            }
            }
    */
    collect();
}

void CoverModule::toHTML(const std::wstring & outputDir)
{
    wchar_t * _outputDir = expandPathVariableW((wchar_t *)outputDir.c_str());
    createdirectoryW(_outputDir);

    // We make all the reports for the macros
    for (auto & p1 : results)
    {
        const std::wstring & moduleName = p1.first;
        const std::wstring __outputDir = std::wstring(_outputDir) + DIR_SEPARATORW + moduleName;
        createdirectoryW((wchar_t *)__outputDir.c_str());
        uint64_t totalCalls = 0;

        for (auto & p2 : p1.second)
        {
            writeMacroHTMLReport(p2.first, moduleName, p2.second, __outputDir);
        }

        // Now we make the module's reports
        std::wostringstream out;

        out << L"<html lang=\'en\'>\n"
            << L"<meta charset=\'UTF-8\'>\n"
            << L"<head>\n"
            << L"<link rel=\'icon\' href=\'../favicon.ico\'/>\n"
            << L"<title>Scilab | Module " << moduleName << L" | Scilab\'s code coverage" <<  L"</title>\n"
            << L"<style type=\'text/css\' media=\'all\'>\n"
            << L"@import url(\'../mod_style.css\');\n"
            << L"</style>\n"
            << L"</head>\n"
            << L"<body>\n"
            << L"<h2 class=\'title\'>Builtins calls</h2>\n"
            << L"<table class='module'>\n"
            << L"<tr><td><div class='modulePath'>" << moduleName << L"</div></td></tr>\n"
            << L"<tr><td><div class=\'builtins_stats\'>\n";
        //<< L"<table>\n";

        // compute total calls
        auto range = functions.equal_range(moduleName);
        for (auto fptr = range.first; fptr != range.second; ++fptr)
        {
            auto i = callCounters.find(static_cast<types::Callable *>(fptr->second));
            if (i != callCounters.end())
            {
                totalCalls += i->second.get();
            }
        }

        auto calls = getFunctionCalls(moduleName, true);
        bool altern = false;
        unsigned int count = 0;
        bool mustClose = false;
        for (const auto & p : calls)
        {
            if (count == 0)
            {
                out << L"<div class=\'builtins_cell\'><table>\n"
                    << L"<tr class=\'col_name\'><td>Name</td><td class=\'col_name\'>Calls</td></tr>\n";
                mustClose = true;
            }
            const std::wstring countercls = p.second == 0 ? L"null_stats" : L"stats";
            const std::wstring trcls = altern ? L"altern1" : L"altern2";
            out << L"<tr class=\'" << trcls << L"\'><td>" << p.first->getName() << L"</td><td class=\'" << countercls << L"\'>x" << p.second << L"</td></tr>\n";
            altern = !altern;
            count = (count + 1) % 20;
            if (count == 0)
            {
                out << L"</table></div>\n";
                mustClose = false;
            }
        }

        if (mustClose)
        {
            out << L"</table></div>\n";
        }

        out << L"</div></tr></td>\n"
            << L"</table>\n"
            << L"<h2 class=\'title\'>Macros calls</h2>\n"
            << L"<table class='module'>\n"
            << L"<tr><td><div class=\'macros_stats\'>\n";

        calls = getFunctionCalls(moduleName, false);
        altern = false;

        out << L"<div class=\'macros_cell\'><table>\n"
            << L"<tr class=\'col_name\'><td>Name</td><td>File</td><td>Calls</td><td>Covered</td></tr>\n";
        for (const auto & p : calls)
        {
            types::Macro * pMacro = static_cast<types::Macro *>(p.first);
            auto i = macros.find(pMacro);
            if (i != macros.end())
            {
                CoverMacroInfo & info = i->second;
                auto j = p1.second.find(info.macroFilePath);
                if (j != p1.second.end())
                {
                    auto k = j->second.find(pMacro->getName());
                    if (k != j->second.end())
                    {
                        std::size_t pos = info.macroFilePath.find_last_of(L'.');
                        std::wstring filename = info.macroFilePath.substr(0, pos);
                        pos = filename.find_last_of(L"\\/");
                        if (pos != std::string::npos)
                        {
                            filename = filename.substr(pos + 1);
                        }
                        CoverResult & res = k->second;
                        const std::wstring countercls = p.second == 0 ? L"null_stats" : L"stats";
                        const std::wstring trcls = altern ? L"altern1" : L"altern2";
                        out << L"<tr class=\'" << trcls << L"\'><td>" << pMacro->getName() << L"</td>"
                            << L"<td><a class=\'filepath' href=\'" << filename << L".html\'>" << moduleName << L"/macros/" << filename << L".sci</a></td>"
                            << L"<td class=\'" << countercls << L"\'>x" << p.second << L"</td>"
                            << L"<td>";
                        CovHTMLCodePrinter::getDivPercent(out, res.getCovInstrsPercent());
                        out << L"</td></tr>\n";

                        altern = !altern;
                    }
                }
            }
        }

        if (mustClose)
        {
            out << L"</table></div>\n";
        }

        /*      calls = getFunctionCalls(moduleName, false);
                for (const auto & p : calls)
                {
                const std::wstring countercls = p.second == 0 ? L"null_stats" : L"stats";
                out << L"<tr><td>" << p.first << L"</td><td class=\'" << countercls << L"\'>" << p.second << L"</td></tr>\n";
                }
        */

        out << L"</div></tr></td>\n"
            << L"</table>\n"
            << L"</body>\n"
            << L"</html>\n";

        out.flush();
        writeFile(out, __outputDir, moduleName + L".html");
    }

    FREE(_outputDir);
    copyDataFiles(outputDir);
}

void CoverModule::copyDataFiles(const std::wstring & outputDir)
{
    const std::wstring _outputDir = outputDir + DIR_SEPARATORW;
    const std::wstring _inputDir = std::wstring(L"SCI") + DIR_SEPARATORW + L"modules" + DIR_SEPARATORW + L"ast" + DIR_SEPARATORW + L"data";
    copyFile(_inputDir, _outputDir, L"scilab_code.css");
    copyFile(_inputDir, _outputDir, L"src_style.css");
    copyFile(_inputDir, _outputDir, L"mod_style.css");
    copyFile(_inputDir, _outputDir, L"favicon.ico");
}

void CoverModule::copyFile(const std::wstring & inDir, const std::wstring & outDir, const std::wstring & filename)
{
    const std::wstring in = inDir + DIR_SEPARATORW + filename;
    const std::wstring out = outDir + DIR_SEPARATORW + filename;
    wchar_t * _input = expandPathVariableW((wchar_t *)in.c_str());
    wchar_t * _output = expandPathVariableW((wchar_t *)out.c_str());
    CopyFileFunction(_output, _input);
    FREE(_input);
    FREE(_output);
}

void CoverModule::writeFile(const std::wostringstream & out, const std::wstring & outputDir, const std::wstring & filename)
{
    char * code = wide_string_to_UTF8(out.str().c_str());
    const std::wstring _filename = outputDir + DIR_SEPARATORW + filename;
    char * __filename = wide_string_to_UTF8(_filename.c_str());
    std::fstream file(__filename, std::ios::out | std::ios::trunc);
    file.write(code, std::strlen(code));
    file.close();
    FREE(code);
    FREE(__filename);
}

std::vector<std::pair<types::Callable *, uint64_t>> CoverModule::getFunctionCalls(const std::wstring & moduleName, const bool builtin) const
{
    struct _Res
    {
        types::Callable * const fptr;
        const uint64_t counter;

        _Res(types::Callable * const _fptr, const uint64_t _counter) : fptr(_fptr), counter(_counter) { }
        inline bool operator<(const _Res & res) const
        {
            return (counter < res.counter) || (counter == res.counter && fptr->getName() < res.fptr->getName());
        }
    };
    std::set<_Res> set;
    std::vector<std::pair<types::Callable *, uint64_t>> calls;
    auto range = functions.equal_range(moduleName);
    for (auto fptr = range.first; fptr != range.second; ++fptr)
    {
        if ((builtin && fptr->second->isFunction()) || (!builtin && fptr->second->isMacro()))
        {
            auto i = callCounters.find(static_cast<types::Callable *>(fptr->second));
            if (i != callCounters.end())
            {
                set.emplace(fptr->second, i->second.get());
            }
        }
    }

    calls.reserve(set.size());
    for (const auto r : set)
    {
        calls.emplace_back(r.fptr, r.counter);
    }

    return calls;
}

ast::Exp * CoverModule::getTree(const std::wstring & path)
{
    if (!path.empty())
    {
        char * _path = wide_string_to_UTF8(path.c_str());
        std::ifstream src(_path, ios::in | ios::binary | ios::ate);
        FREE(_path);
        if (src.is_open())
        {
            src.seekg(0, src.end);
            int len = src.tellg();
            src.seekg (0, src.beg);
            char * buffer = new char[len + 1];
            buffer[len] = '\0';
            src.read(buffer, len);
            src.close();

            wchar_t * _wstr = to_wide_string(buffer);
            delete[] buffer;
            Parser parser;
            parser.parse(_wstr);
            FREE(_wstr);

            return parser.getTree();
        }
    }
    return nullptr;
}

const std::wstring CoverModule::getName(const std::wstring & path)
{
    std::size_t pos = path.find_last_of(L'.');
    std::wstring name = path.substr(0, pos);
    pos = name.find_last_of(L"\\/");
    if (pos != std::string::npos)
    {
        name = name.substr(pos + 1);
    }
    return name;
}

void CoverModule::writeMacroHTMLReport(ast::Exp * tree, const std::wstring & filename, const std::wstring & path, const std::wstring & moduleName, std::unordered_map<std::wstring, CoverResult> & results, const std::wstring & outputDir)
{
    std::wostringstream out;
    std::wstring mod;
    if (!moduleName.empty())
    {
        mod = L" | Module " + moduleName;
    }
    std::wstring pa;
    if (!path.empty())
    {
        pa = L" | " + path;
    }

    out << L"<html lang=\'en\'>\n"
        << L"<meta charset=\'UTF-8\'>\n"
        << L"<head>\n"
        << L"<link rel=\'icon\' href=\'../favicon.ico\'/>\n"
        << L"<title>Scilab" << mod << pa << L" | Scilab\'s code coverage" <<  L"</title>\n"
        << L"<style type=\'text/css\' media=\'all\'>\n"
        << L"@import url(\'../scilab_code.css\');\n"
        << L"@import url(\'../src_style.css\');\n"
        << L"</style>\n"
        << L"<script>\n"
        << L"function show(did,fid) {\n"
        << L"  x = document.getElementById(did).style;\n"
        << L"  y = document.getElementById(fid);\n"
        << L"  x.visibility = 'visible';\n"
        << L"  x.display = 'block';\n"
        << L"  x.height = 'auto';\n"
        << L"  x.left = y.offsetLeft + 'px';\n"
        << L"  x.top = y.offsetTop + y.offsetHeight + 'px';\n"
        << L"}\n"
        << L"function hide(did) {\n"
        << L"  document.getElementById(did).style.visibility = \'hidden\';\n"
        << L"}\n"
        << L"</script>\n"
        << L"</head>\n"
        << L"<body>\n"
        << L"<h2 class=\'title\'>Source File</h2>\n"
        << L"<table class='sourceFile'>\n";
    if (!path.empty())
    {
        out << L"<tr><td><div class='sourcePath'>" << path << L"</div></td></tr>\n";
    }
    out << L"<tr><td><div class=\'source\'>\n"
        << L"<table>\n";

    CovHTMLCodePrinter printer(out, results);
    CodePrinterVisitor visitor(printer);
    tree->accept(visitor);
    printer.close();
    delete tree;

    out << L"</table>\n"
        << L"</div></tr></td>\n"
        << L"</table>\n"
        << L"</body>\n"
        << L"</html>\n";

    out.flush();
    writeFile(out, outputDir, filename);
}

bool CoverModule::writeMacroHTMLReport(const std::wstring & path, const std::wstring & moduleName, std::unordered_map<std::wstring, CoverResult> & results, const std::wstring & outputDir)
{
    if (ast::Exp * tree = getTree(path))
    {
        writeMacroHTMLReport(tree, getName(path) + L".html", path, moduleName, results, outputDir);
        return true;
    }

    return false;
}
}
