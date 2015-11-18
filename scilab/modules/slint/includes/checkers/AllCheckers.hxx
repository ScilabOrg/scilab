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

#ifndef __SLINT_ALL_CHECKERS_HXX__
#define __SLINT_ALL_CHECKERS_HXX__

#include "checkers/FunctionNameChecker.hxx"
#include "checkers/FunctionArgsChecker.hxx"
#include "checkers/VariablesChecker.hxx"
#include "checkers/UselessArgChecker.hxx"
#include "checkers/UselessRetChecker.hxx"
#include "checkers/VariableNameChecker.hxx"
#include "checkers/RedefinitionChecker.hxx"
#include "checkers/SingleInstrChecker.hxx"
#include "checkers/EmptyBlockChecker.hxx"
#include "checkers/SemicolonAtEOLChecker.hxx"
#include "checkers/MopenMcloseChecker.hxx"
#include "checkers/McCabeChecker.hxx"
#include "checkers/DecimalChecker.hxx"
#include "checkers/PrintfChecker.hxx"
#include "checkers/TodoChecker.hxx"
#include "checkers/LineLengthChecker.hxx"
#include "checkers/IllegalCallsChecker.hxx"
#include "checkers/LinesCountChecker.hxx"
#include "checkers/NaNChecker.hxx"
#include "checkers/EqEqChecker.hxx"
#include "checkers/UselessOpChecker.hxx"
#include "checkers/UnreachableCodeChecker.hxx"
#include "checkers/DeprecatedChecker.hxx"
#include "checkers/SelectChecker.hxx"
#include "checkers/ImplicitListChecker.hxx"
#include "checkers/StructChecker.hxx"
#include "checkers/LoadSaveChecker.hxx"
#include "checkers/OldNotChecker.hxx"
#include "checkers/SpacesAroundOpChecker.hxx"
#include "checkers/BreaksInLoopChecker.hxx"
#include "checkers/NestedBlocksChecker.hxx"
#include "checkers/BracketedExpChecker.hxx"
#include "checkers/NotNotChecker.hxx"
#include "checkers/SpacesInArgsChecker.hxx"
#include "checkers/GlobalKeywordChecker.hxx"
#include "checkers/ExpInCondChecker.hxx"
#include "checkers/CommentRatioChecker.hxx"
#include "checkers/FunctionArgsOrderChecker.hxx"
#include "checkers/FunctionTestReturnChecker.hxx"
#include "checkers/ReturnsCountChecker.hxx"
#include "checkers/NotEqualChecker.hxx"

#endif // __SLINT_ALL_CHECKERS_HXX__
