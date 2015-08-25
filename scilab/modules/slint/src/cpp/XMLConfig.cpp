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

#include <unordered_map>

#include "config/XMLConfig.hxx"
#include "SLintOptions.hxx"

#define SLINT_INSERT_IN_MAP(name) callbacks.emplace(#name, &createFromXmlNode<name##Checker>)

namespace slint
{

    std::unordered_map<std::string, XMLConfig::CBType> XMLConfig::callbacks = initCallbacks();

    void XMLConfig::getOptions(const std::wstring & path, SLintOptions & options)
    {
	xmlDoc * doc = slint::XMLtools::readXML(path);
	xmlNode * root = xmlDocGetRootElement(doc);
	for (xmlNode * child = root->children; child; child = child->next)
	{
	    const std::string nodeName((const char *)child->name);
	    auto i = callbacks.find(nodeName);
	    if (i != callbacks.end())
	    {
		if (SLintChecker * checker = i->second(child))
		{
		    options.addDefault(checker);
		}
	    }
	}

	xmlFreeDoc(doc);
    }

    template<>
    SLintChecker * XMLConfig::createFromXmlNode<FunctionNameChecker>(xmlNode * node)
    {
	bool enable = true;
	XMLtools::getBool(node, "enable", enable);
	if (enable)
	{
	    std::wstring pattern;
	    int min = -1;
	    int max = -1;
	    
	    XMLtools::getWString(node, "namePattern", pattern);
	    XMLtools::getInt(node, "minLength", min);
	    XMLtools::getInt(node, "maxLength", max);

	    return new FunctionNameChecker(pattern, min, max);
	}

	return nullptr;
    }

    template<>
    SLintChecker * XMLConfig::createFromXmlNode<VariableNameChecker>(xmlNode * node)
    {
	bool enable = true;
	XMLtools::getBool(node, "enable", enable);
	if (enable)
	{
	    std::wstring pattern;
	    int min = -1;
	    int max = -1;
	    
	    XMLtools::getWString(node, "namePattern", pattern);
	    XMLtools::getInt(node, "minLength", min);
	    XMLtools::getInt(node, "maxLength", max);

	    return new VariableNameChecker(pattern, min, max);
	}

	return nullptr;
    }

    template<>
    SLintChecker * XMLConfig::createFromXmlNode<MopenMcloseChecker>(xmlNode * node)
    {
	bool enable = true;
	XMLtools::getBool(node, "enable", enable);
	if (enable)
	{
	    bool checkError = true;
	    XMLtools::getBool(node, "checkError", checkError);

	    return new MopenMcloseChecker(checkError);
	}

	return nullptr;
    }

    template<>
    SLintChecker * XMLConfig::createFromXmlNode<McCabeChecker>(xmlNode * node)
    {
	bool enable = true;
	XMLtools::getBool(node, "enable", enable);
	if (enable)
	{
	    int max = -1;
	    XMLtools::getInt(node, "max", max);

	    return new McCabeChecker(max);
	}

	return nullptr;
    }

    template<>
    SLintChecker * XMLConfig::createFromXmlNode<DecimalChecker>(xmlNode * node)
    {
	bool enable = true;
	XMLtools::getBool(node, "enable", enable);
	if (enable)
	{
	    std::wstring character;
	    bool checkDot = false;
	    XMLtools::getWString(node, "character", character);
	    XMLtools::getBool(node, "checkDot", checkDot);
	    wchar_t c = character.empty() ? L'\0' : character.at(0);
	    
	    return new DecimalChecker(c, checkDot);
	}

	return nullptr;
    }

    template<>
    SLintChecker * XMLConfig::createFromXmlNode<LineLengthChecker>(xmlNode * node)
    {
	bool enable = true;
	XMLtools::getBool(node, "enable", enable);
	if (enable)
	{
	    int max = -1;
	    XMLtools::getInt(node, "max", max);

	    return new LineLengthChecker(max);
	}

	return nullptr;
    }

    template<>
    SLintChecker * XMLConfig::createFromXmlNode<LinesCountChecker>(xmlNode * node)
    {
	bool enable = true;
	XMLtools::getBool(node, "enable", enable);
	if (enable)
	{
	    int max = -1;
	    XMLtools::getInt(node, "max", max);

	    return new LinesCountChecker(max);
	}

	return nullptr;
    }

    template<>
    SLintChecker * XMLConfig::createFromXmlNode<StructChecker>(xmlNode * node)
    {
	bool enable = true;
	XMLtools::getBool(node, "enable", enable);
	if (enable)
	{
	    std::wstring pattern;
	    int min = -1;
	    int max = -1;
	    
	    XMLtools::getWString(node, "fieldPattern", pattern);
	    XMLtools::getInt(node, "minLength", min);
	    XMLtools::getInt(node, "maxLength", max);

	    return new StructChecker(pattern, min, max);
	}

	return nullptr;
    }

    template<>
    SLintChecker * XMLConfig::createFromXmlNode<BreaksInLoopChecker>(xmlNode * node)
    {
	bool enable = true;
	XMLtools::getBool(node, "enable", enable);
	if (enable)
	{
	    int maxBreaks = -1;
	    int maxContinues = -1;
	    
	    XMLtools::getInt(node, "maxBreaks", maxBreaks);
	    XMLtools::getInt(node, "maxContinues", maxContinues);

	    return new BreaksInLoopChecker(maxBreaks, maxContinues);
	}

	return nullptr;
    }

    template<>
    SLintChecker * XMLConfig::createFromXmlNode<NestedBlocksChecker>(xmlNode * node)
    {
	bool enable = true;
	XMLtools::getBool(node, "enable", enable);
	if (enable)
	{
	    int max = -1;
	    XMLtools::getInt(node, "max", max);

	    return new NestedBlocksChecker(max);
	}

	return nullptr;
    }

    template<>
    SLintChecker * XMLConfig::createFromXmlNode<IllegalCallsChecker>(xmlNode * node)
    {
	bool enable = true;
	XMLtools::getBool(node, "enable", enable);
	if (enable)
	{
	    std::vector<std::wstring> names;
	    for (xmlNode * child = node->children; child; child = child->next)
	    {
		const std::string nodeName((const char *)child->name);
		if (nodeName == "keyword")
		{
		    std::wstring name;
		    XMLtools::getWString(node, "name", name);
		    if (!name.empty())
		    {
			names.emplace_back(name);
		    }
		}
	    }

	    return new IllegalCallsChecker(names);
	}

	return nullptr;
    }

    std::unordered_map<std::string, XMLConfig::CBType> XMLConfig::initCallbacks()
    {
	std::unordered_map<std::string, CBType> callbacks;

	SLINT_INSERT_IN_MAP(Redefinition);
	SLINT_INSERT_IN_MAP(Variables);
	SLINT_INSERT_IN_MAP(FunctionName);
	SLINT_INSERT_IN_MAP(FunctionArgs);
	SLINT_INSERT_IN_MAP(UselessArg);
	SLINT_INSERT_IN_MAP(UselessRet);
	SLINT_INSERT_IN_MAP(VariableName);
	SLINT_INSERT_IN_MAP(SingleInstr);
	SLINT_INSERT_IN_MAP(EmptyBlock);
	SLINT_INSERT_IN_MAP(SemicolonAtEOL);
	SLINT_INSERT_IN_MAP(MopenMclose);
	SLINT_INSERT_IN_MAP(McCabe);
	SLINT_INSERT_IN_MAP(Decimal);
	SLINT_INSERT_IN_MAP(Printf);
	SLINT_INSERT_IN_MAP(LineLength);
	SLINT_INSERT_IN_MAP(LinesCount);
	SLINT_INSERT_IN_MAP(Todo);
	SLINT_INSERT_IN_MAP(NaN);
	SLINT_INSERT_IN_MAP(EqEq);
	SLINT_INSERT_IN_MAP(UselessOp);
	SLINT_INSERT_IN_MAP(UnreachableCode);
	SLINT_INSERT_IN_MAP(Deprecated);
	SLINT_INSERT_IN_MAP(Select);
	SLINT_INSERT_IN_MAP(ImplicitList);
	SLINT_INSERT_IN_MAP(Struct);
	SLINT_INSERT_IN_MAP(LoadSave);
	SLINT_INSERT_IN_MAP(OldNot);
	SLINT_INSERT_IN_MAP(SpacesAroundOp);
	SLINT_INSERT_IN_MAP(SpacesInArgs);
	SLINT_INSERT_IN_MAP(BreaksInLoop);
	SLINT_INSERT_IN_MAP(NestedBlocks);
	SLINT_INSERT_IN_MAP(BracketedExp);
	SLINT_INSERT_IN_MAP(NotNot);
	SLINT_INSERT_IN_MAP(IllegalCalls);
	
	return callbacks;
    }
    
} // namespace slint
