// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Calixte DENIZET 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// <-- INTERACTIVE TEST -->

tmpdir = TMPDIR + "/testlinks";
mkdir(tmpdir);
tmpdir = tmpdir + "/help";
mkdir(tmpdir);
tmpdir = tmpdir + "/en_US";
mkdir(tmpdir);
tmpfile = tmpdir + "/foo.xml";
mputl(["<?xml version=""1.0"" encoding=""UTF-8""?>"
       "<refentry version=""5.0-subset Scilab"""
       "xmlns=""http://docbook.org/ns/docbook"""
       "xmlns:html=""http://www.w3.org/1999/xhtml"""
       "xmlns:db=""http://docbook.org/ns/docbook"">"
       "<refnamediv xml:id=""foo"">"
       "<refname>Foo</refname>"
       "<refpurpose>calculates foo</refpurpose>"
       "</refnamediv>"
       "<refsection>"
       "<para>Firstly a link on <link type=""scilab"" linkend=""scilab/help/bvode"">bvode</link> in the Scilab Doc</para>"
       "<para>Secondly a link on <link type=""scilab"" linkend=""scilab/demos/foodemo"">foodemo</link> </para>"
       "<para>Thirdly a link on <link type=""scilab"" linkend=""scilab/exec/macros/fooexec.sce"">macros/fooexec.sce</link> (just an about) </para>"
       "<para>Fourthly a link on <link type=""scilab"" linkend=""scilab/help/invalidlink"">invalid link</link></para>"
       "</refsection>"
       "<programlisting role=""example""><![CDATA[" 
       "plot3d() // should link on plot3d"
       "]]></programlisting>"
       "</refentry>"], tmpfile);
jar = xmltojar(tmpdir, 'foo', 'en_US');
tmpdir = TMPDIR + "/testlinks/demos";
mkdir(tmpdir);
tmpfile = tmpdir + "/foodemo.sce";
mputl("messagebox(""A demo from foodemo.sce"");", tmpfile);
tmpdir = TMPDIR + "/testlinks/macros";
mkdir(tmpdir);
tmpfile = tmpdir + "/fooexec.sce";
mputl("about;", tmpfile);
add_help_chapter('foo', TMPDIR + "/testlinks/jar")
help('foo')