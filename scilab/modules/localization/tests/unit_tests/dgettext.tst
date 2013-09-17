// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Sylvestre LEDRU <sylvestre.ledru@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//===============================
// unit tests dgettext
//===============================

// Void call
if execstr('dgettext()','errcatch')==0 then pause,end

// one input argument
str1="plop"; if execstr('dgettext(str1)','errcatch')<>0 then pause,end

// three input argument
str1="plop"; str2="plip"; str3="plup"; if execstr('dgettext(str1, str2, str3)','errcatch')==0 then pause,end

// Try to translate from en_US to en_US to domain/string which doesn't exist
lang="en_US"; setlanguage(lang); domain="fake_domain"; msg="Localization does not exist"; if dgettext(domain,msg) <> msg then pause, end

// Try to translate from en_US to fr_FR to a domain/string which doesn't exist
lang="fr_FR"; setlanguage(lang); domain="fake_domain"; msg="Localization does not exist"; if dgettext(domain,msg) <> msg then pause, end


// Check if it is working with a good domain & good msgid
lang="fr_FR"; setlanguage(lang); domain="scilab"; msg="Startup execution:"; if dgettext(domain,msg) <> "Initialisation :" then pause, end

// Check if it is working with a bad domain & good msgid
lang="fr_FR"; setlanguage(lang); domain="fake_domain"; msg="Startup execution:"; if dgettext(domain,msg) == "Initialisation :" then pause, end

// Check if it is working with a good domain & good msgid and alias
lang="fr"; setlanguage(lang); domain="scilab"; msg="Startup execution:"; if dgettext(domain,msg) <> "Initialisation :" then pause, end

// Check if it is working with a bad domain & good msgid and alias
lang="fr"; setlanguage(lang); domain="fake_domain"; msg="Startup execution:"; if dgettext(domain,msg) == "Initialisation :" then pause, end


// Check if it is working with a good domain & good msgid
lang="en_US"; setlanguage(lang); domain="scilab"; msg="Startup execution:"; if dgettext(domain,msg) <> msg then pause, end

// Check if it is working with a bad domain & good msgid
lang="en_US"; setlanguage(lang); domain="fake_domain"; msg="Startup execution:"; if dgettext(domain,msg) <> msg then pause, end

// Check if it is working with a good domain & good msgid and alias
lang="en"; setlanguage(lang); domain="scilab"; msg="Startup execution:"; if dgettext(domain,msg) <> msg then pause, end

// Check if it is working with a bad domain & good msgid and alias
lang="en"; setlanguage(lang); domain="fake_domain"; msg="Startup execution:"; if dgettext(domain,msg) <> msg then pause, end

// @TODO : 
// Add test to use other domain which are working (ie not fake_domain)
