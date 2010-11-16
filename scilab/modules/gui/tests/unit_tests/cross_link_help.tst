// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// <-- INTERACTIVE TEST -->

tmphelp = TMPDIR + "/help/";
mkdir(tmphelp);
tmphelp = tmphelp + "en_US/"
mkdir(tmphelp);
tmpfile = tmphelp + "/cross_link.xml";
mputl(["<?xml version=""1.0"" encoding=""UTF-8""?>";
       "<refentry version=""5.0-subset Scilab"" xml:id=""titi"" xml:lang=""en""";
       "xmlns=""http://docbook.org/ns/docbook""";
       "xmlns:xlink=""http://www.w3.org/1999/xlink""";
       "xmlns:ns3=""http://www.w3.org/1999/xhtml""";
       "xmlns:db=""http://docbook.org/ns/docbook"">";
       "<refnamediv><refname>titi</refname><refpurpose>calculate titi</refpurpose></refnamediv>";
       "<refsection><para>Click me to go to bvode help in scilab <ulink url=""scilab://scilab.help.en_US/bvode"">go to bvode</ulink></para></refsection>";
       "</refentry>"], tmpfile);

tmphelp = tmphelp + "/..";
xmltojar(tmphelp,'titi','en_US','en_US','MyTitiToolbox');
tmphelp = tmphelp + "/../../jar";
add_help_chapter('titi',tmphelp);
help titi;