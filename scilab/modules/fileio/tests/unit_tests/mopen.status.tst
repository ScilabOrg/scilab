// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab - Sylvain GENIN
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//Bug 13331

assert_checkerror("mopen(""texte.txt"", ""rw"")","mopen : Statut invalide.")

assert_checkerror("mopen(""texte.txt"", ""r+w"")","mopen : Statut invalide.")

assert_checkerror("mopen(""texte.txt"", ""r="")","mopen : Statut invalide.")

assert_checkerror("mopen(""texte.txt"", ""L"")","mopen : Statut invalide.")

assert_checkerror("mopen(""texte.txt"", ""w+lb"")","mopen : Statut invalide.")

//=================================
