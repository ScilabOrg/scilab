/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "prettyprintvisitor.hxx"
#include "printvisitor.hxx"

#include "data/DataManager.hxx"
#include "data/FunctionBlock.hxx"
#include "data/CompleteMacroSignature.hxx"
#include "gvn/ConstraintManager.hxx"

namespace analysis
{

FunctionBlock::FunctionBlock(const unsigned int id, Block * parent, ast::Exp * exp) : Block(id, parent, exp), loopAnalyzer(exp), constraintManager(*this, dm->topFunction())
{
    gvn = &fgvn;
    dm->pushFunction(this);
    //std::wcerr << L"Loop analyze:" << loopAnalyzer << std::endl;
}

bool FunctionBlock::addIn(const TITypeSignatureTuple & tuple, const std::vector<GVN::Value *> & values)
{
    std::vector<TITypeSignature>::const_iterator i = tuple.types.begin();
    for (const auto & sym : in)
    {
        Info & info = addSym(sym, new Data(false, sym));
        info.type = TIType(fgvn, i->type, i->scalar);
        dm->registerData(info.data);//, __LINE__, __FILE__);
        ++i;
    }

    for (const auto & sym : globals)
    {
        Info & info = addSym(sym, new Data(false, sym));
        info.type = TIType(fgvn, i->type, i->scalar);
        dm->registerData(info.data);//, __LINE__, __FILE__);
        ++i;
    }
    inValues = values;
    maxVarId = fgvn.getCurrentValue() - 1;

    return true;
}

void FunctionBlock::addGlobal(const symbol::Symbol & sym)
{
    //globals.emplace(sym);
    Block::addGlobal(sym);
}

void FunctionBlock::setGlobals(const tools::SymbolOrdSet & v)
{
    //globals = v; => bug on mac
    std::copy(v.begin(), v.end(), std::inserter(globals, globals.begin()));
}

/*    TITypeSignatureTuple FunctionBlock::getGlobals(std::vector<symbol::Symbol> & v)
    {
        TITypeSignatureTuple tuple;
        std::vector<TITypeSignature> & t = tuple.types;
        t.reserve(globals.size());
        v.reserve(globals.size());

        for (const auto & sym : globals)
        {
            v.emplace_back(sym);
            tools::SymbolMap<Info>::iterator it;
            Block * block = parent->getDefBlock(sym, it);
            if (block)
            {
                t.emplace_back(it->second.type.type, it->second.type.isscalar());
            }
            else
            {
                bool exists;
                TIType ty = DataManager::getSymInScilabContext(fgvn, sym, exists);
                t.emplace_back(ty.type, ty.isscalar());
            }
        }

        return tuple;
    }
*/
MacroOut FunctionBlock::getOuts()
{
    MacroOut mo;
    mo.maxVarId = maxVarId;
    std::vector<TIType> & v = mo.tuple.types;
    v.reserve(lhs);
    unsigned int i = 0;
    for (std::vector<symbol::Symbol>::const_iterator s = out.begin(); i < lhs; ++i, ++s)
    {
        tools::SymbolMap<Info>::iterator it;
        Block * block = getDefBlock(*s, it, false);
        if (block == this)
        {
            v.emplace_back(it->second.type);
        }
        else
        {
            addGlobal(*s);
            if (block)
            {
                v.emplace_back(it->second.type);
            }
            else
            {
                // TODO: if exists is false then it is an error
                bool exists;
                v.emplace_back(DataManager::getSymInScilabContext(fgvn, *s, exists));
            }
        }
    }

    return mo;
}

void FunctionBlock::finalize()
{
    dm->popFunction();

    for (unsigned int i = 0; i != lhs; ++i)
    {
        auto it = symMap.find(out[i]);
        if (it != symMap.end())
        {
            const TIType & type = it->second.type;
            if (type.isscalar())
            {
                types_out.emplace_back(out[i], TypeLocal(type.type, 1, 1, false));
            }
            else
            {
                types_out.emplace_back(out[i], TypeLocal(type.type, -1, -1, false));
            }
        }
        else
        {
            types_out.emplace_back(out[i], TypeLocal(TIType::UNKNOWN, -1, -1, false));
        }

        auto jt = locals.find(out[i]);
        if (jt != locals.end())
        {
            jt->second.erase(types_out.back().second);
            if (jt->second.empty())
            {
                locals.erase(jt);
            }
        }
    }
}

void FunctionBlock::addLocal(const symbol::Symbol & sym, const TIType & type, const bool isAnInt)
{
    auto i = locals.find(sym);
    if (i == locals.end())
    {
        i = locals.emplace(sym, std::set<TypeLocal>()).first;
    }

    i->second.emplace(TypeLocal::get(type, isAnInt));
}

int FunctionBlock::getTmpId(const TIType & type, const bool isAnInt)
{
    return tempManager.getTmp(type, isAnInt);
}

void FunctionBlock::releaseTmp(const int id)
{
    tempManager.releaseTmp(id);
}

void FunctionBlock::setInOut(MacroDef * macrodef, const unsigned int rhs, const std::vector<TIType> & _in)
{
    in = macrodef->getIn();
    out = macrodef->getOut();

    for (unsigned int i = 0; i != rhs; ++i)
    {
        if (_in[i].isscalar())
        {
            types_in.emplace_back(in[i], TypeLocal(_in[i].type, 1, 1, false));
        }
        else
        {
            types_in.emplace_back(in[i], TypeLocal(_in[i].type, -1, -1, false));
        }
    }
}

Block * FunctionBlock::getDefBlock(const symbol::Symbol & sym, tools::SymbolMap<Info>::iterator & it, const bool global)
{
    it = symMap.find(sym);
    if (it == symMap.end())
    {
        if (!global && globals.find(sym) == globals.end())
        {
            // we have looked for a symbol which is not in this function
            // so we add it and give it an unknown type
            it = symMap.emplace(sym, new Data(false, sym)).first;
            Info & info = it->second;
            info.local = Info::Local::INFO_UNKNOWN;
            info.type = TIType(fgvn);
            dm->registerData(info.data);
        }
        else
        {
            return parent->getDefBlock(sym, it, true);
        }
    }
    return this;
}

std::wostream & operator<<(std::wostream & out, const FunctionBlock & fblock)
{
    out << L"Function " << fblock.name << L'\n'
        << L" -LHS: " << fblock.lhs << L'\n'
        << L" -RHS: " << fblock.rhs << L'\n'
        << L" -in:" << L'\n';
    for (const auto & p : fblock.types_in)
    {
        out << L"   -" << p.first << L" -> " << p.second << L'\n';
    }

    out << L'\n'
        << L" -out:" << L'\n';
    for (const auto & p : fblock.types_out)
    {
        out << L"   -" << p.first << L" -> " << p.second << L'\n';
    }
    out << L'\n';
    if (fblock.locals.empty())
    {
        out << L" -locals: none" << L'\n';
    }
    else
    {
        out << L" -locals:" << L'\n';
        for (const auto & p : fblock.locals)
        {
            out << L"   -" << p.first << L" -> ";
            tools::printSet(p.second, out);
            out << L'\n';
        }
    }

    out << L'\n';
    const std::map<TypeLocal, std::stack<int>> & temps = fblock.getTemp();
    if (temps.empty())
    {
        out << L" -temps: none" << L'\n';
    }
    else
    {
        out << L" -temps:" << L'\n';
        for (const auto & p : temps)
        {
            out << L"   -" << p.first << L" -> " << p.second.size() << L'\n';
        }
    }

    //ast::PrintVisitor pv(out, true, false);
    //fblock.exp->accept(pv);

    ast::PrettyPrintVisitor dv(out);
    fblock.exp->accept(dv);

    return out;
}
}
