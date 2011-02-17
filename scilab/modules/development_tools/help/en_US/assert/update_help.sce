// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Updates the .xml files by deleting existing files and 
// creating them again from the .sci with help_from_sci.

function b = isContentUptodate ( content , filename )
  // Check if a content matches a file.
  // 
  // Calling Sequence
  //   b = isContentUptodate ( content , filename )
  //
  // Parameters
  //   content : a 1x1 matrix of strings, the new content
  //   filename : a 1x1 matrix of strings, the file to be checked.
  //   b : a 1x1 matrix of booleans
  //
  // Description
  //   Returns true if the <literal>filename</literal> is up-to-date, that is,
  //   if the file does not require to be changed.
  //   The file is to be updated if one of the following conditions is satisfied.
  // <itemizedlist>
  //   <listitem>
  //     The file does not exist.
  //   </listitem>
  //   <listitem>
  //     The file exists, but its content is not equal to <literal>content</literal>.
  //     The content comparison ignores the leading and trailing blanks.
  //   </listitem>
  // </itemizedlist>
  //   Last update : 02/09/2010.
  //
  // Example
  //   // TODO...
  //
  // Author
  //   2010 - DIGITEO - Michael Baudin
  //   
  
  b = %t
  if ( fileinfo(filename) == [] ) then
    b = %f
  else
    txt = mgetl(filename)
    if ( or ( stripblanks(content) <> stripblanks(txt) ) ) then
      b = %f
    end
  end
endfunction

function b = fileUpdateIfNeeded ( content , filename )
  // Update a file if this is needed.
  //
  // Calling Sequence
  //   b = fileUpdateIfNeeded ( content , filename )
  //
  // Parameters
  //   content : a 1x1 matrix of strings, the new content
  //   filename : a 1x1 matrix of strings, the file to be checked.
  //   b : a 1x1 matrix of booleans
  //
  // Description
  //   Returns true if the <literal>filename</literal> is up-to-date, that is,
  //   if the file was not changed.
  //   Returns false if the file was changed.
  //   The file is to be updated according to the rules of <literal>isContentUptodate</literal>.
  //   Generates an error if the file was to be changed, but that was not possible.
  //   Last update : 02/09/2010.
  //
  // Example
  //   // TODO...
  //
  // Author
  //   2010 - DIGITEO - Michael Baudin
  //   

  b = isContentUptodate ( content , filename )
  if ( ~b ) then
    r = mputl ( content , filename )
    if ( ~r ) then
      error(sprintf(gettext("%s: Unable to write xml file: %s\n"),"fileUpdateIfNeeded",filename));
    end
  end
endfunction

function helpupdate ( funarray , helpdir , macrosdir , demosdir , modulename , verbose )
  // Update the help and the demos from the .sci files.
  //
  // Calling Sequence
  //   updatehelp ( funarray , helpdir , macrosdir , demosdir , modulename , verbose )
  //
  // Parameters
  //   funarray : column matrix of strings. The list of functions to update
  //   helpdir : the help directory
  //   macrosdir : the macros directory
  //   demosdir : the demonstration directory
  //   modulename: a 1x1 matrix of strings, the name of the module to update
  //   verbose: a 1x1 matrix of booleans, verbose = %t prints messages
  //   modulename : the name of the module
  //
  // Description
  //   Update the .xml help files and the demos scripts
  //   from the macros corresponding to the function array
  //   of strings funarray.
  //   The existing .xml files in the help dir which 
  //   correspond to file in the funarray are deleted (Caution !).
  //   Generates the .xml and the .sce files from the help_from_sci function.
  //   Generates a demonstration gateway.
  //
  //   If demosdir is an empty matrix, do not generate the demonstrations.
  //   Last update : 04/10/2010.
  //
  // Author
  //   2010, Michael Baudin
  
  if ( fileinfo ( helpdir ) == [] ) then
    error(sprintf(gettext("%s: Wrong help directory: %s does not exist.\n"),"updatehelp",helpdir));
  end
  if ( fileinfo ( macrosdir ) == [] ) then
    error(sprintf(gettext("%s: Wrong macros directory: %s does not exist.\n"),"updatehelp",macrosdir));
  end
  if ( demosdir <> [] ) then
    if ( fileinfo ( demosdir ) == [] ) then
      error(sprintf(gettext("%s: Wrong demos directory: %s does not exist.\n"),"updatehelp",demosdir));
    end
  end
  
  //
  // 2. Generate each .xml and each .sce from the .sci
  flist = ls(macrosdir)';
  for f = flist
    issci = regexp(f,"/(.*).sci/");
    kf = find(funarray==basename(f))
    if ( issci <> [] & kf <> [] ) then
      scifile = fullfile ( macrosdir , f )
      if ( verbose ) then
        mprintf("Processing %s\n",scifile);
      end
      funname = funarray(kf)
      xmlfile = fullfile ( helpdir , funname + ".xml" )
      // Generate the xml and the demo content
      [helptxt,demotxt]= help_from_sci (scifile)
      // Delete the "info" tag, containing the date (3 lines)
      k = find(stripblanks(helptxt)=="<info>")
      helptxt(k:k+2) = []
      // Update the xml file, if necessary
      isuptodate = fileUpdateIfNeeded ( helptxt , xmlfile )
      if ( ~isuptodate & verbose ) then
        changetxt = "XML Changed"
        mprintf("  %s %s\n",changetxt,xmlfile);
      end
      // Create the demo
      if ( demosdir <> [] ) then
        demofile = fullfile ( demosdir , funname + ".sce" )
        // Update the demo script
        header = []
        header($+1) = "//"
        header($+1) = "// This help file was automatically generated from "+funname+".sci using help_from_sci()."
        header($+1) = "// PLEASE DO NOT EDIT"
        header($+1) = "//"
        footer = []
        footer($+1) = msprintf("//\n");
        footer($+1) = msprintf("// Load this script into the editor\n");
        footer($+1) = msprintf("//\n");
        footer($+1) = msprintf("filename = ""%s"";\n",funname + ".sce");
        footer($+1) = msprintf("dname = get_absolute_file_path(filename);\n");
        footer($+1) = msprintf("editor ( fullfile(dname,filename) );\n");
        demotxt = [header;demotxt;footer]
        // Update the demo file, if necessary
        isuptodate = fileUpdateIfNeeded ( demotxt , demofile )
        if ( ~isuptodate & verbose ) then
          changetxt = "SCE Changed"
          mprintf("  %s %s\n",changetxt,demofile);
        end
      end
    end
  end
  //
  // 3. Generates the Demonstration gateway
  // Include all .sce files in the gateway, including 
  // handcrafted scripts which might have been written
  // by the user (and not generated by this function).
  // To make so, use the list of .sce files in the demo dir and 
  /// not simply the funarray.
  if ( demosdir <> [] ) then
    gatetxt = []
    gatetxt ($+1) = "// This help file was automatically generated using helpupdate"
    gatetxt ($+1) = "// PLEASE DO NOT EDIT"
    gatetxt ($+1) = "demopath = get_absolute_file_path(""" + modulename + ".dem.gateway.sce"");"
    gatetxt ($+1) = "subdemolist = ["
    flist = ls(demosdir)';
    for f = flist
      issce = regexp(f,"/(.*).sce/");
      isgateway = regexp(f,"/(.*).gateway.sce/");
      if ( issce <> [] & isgateway == [] ) then
        flen = length(f)
        funname = part(f,[1:flen-4])
        gatetxt($+1) = """" + funname + """, """ + funname + ".sce""; .."
      end
    end
    //
    gatetxt ($+1) = "];"
    gatetxt ($+1) = "subdemolist(:,2) = demopath + subdemolist(:,2)"
    gatefile = fullfile ( demosdir , modulename+".dem.gateway.sce" )
    // Update the gateway file, if necessary
    isuptodate = fileUpdateIfNeeded ( gatetxt , gatefile )
    if ( ~isuptodate & verbose ) then
      changetxt = "SCE Gateway Changed"
      mprintf("%s %s\n",changetxt,gatefile);
    end
  end
endfunction

//
cwd = get_absolute_file_path("update_help.sce");
mprintf("Working dir = %s\n",cwd);
//
// Generate the library help
mprintf("Updating assert\n");
helpdir = cwd;
funmat = [
  "assert_checkalmostequal"
  "assert_checkequal"
  "assert_checktrue"
  "assert_checkfalse"
  "assert_checkfilesequal"
  "assert_checkerror"
  "assert_generror"
  ];
macrosdir = cwd +"../../macros";
demosdir = cwd +"../../demos";
modulename = "assert";
helpupdate ( funmat , helpdir , macrosdir , demosdir , modulename , %t )

//
// Generate the sorting help
mprintf("Updating assert/sorting\n");
helpdir = fullfile(cwd,"sorting");
funmat = [
  "assert_sort"
  "assert_compare"
  "assert_comparecomplex"
  "assert_sortcomplex"
  ];
macrosdir = cwd +"../../macros";
demosdir = cwd +"../../demos";
modulename = "assert";
helpupdate ( funmat , helpdir , macrosdir , demosdir , modulename , %t )

//
// Generate the support help
mprintf("Updating assert/support\n");
helpdir = fullfile(cwd,"support");
funmat = [
  "assert_computedigits"
  "assert_cond2reltol"
  "assert_cond2reqdigits"
  "assert_csvwrite"
  "assert_csvread"
  ];
macrosdir = cwd +"../../macros";
demosdir = cwd +"../../demos";
modulename = "assert";
helpupdate ( funmat , helpdir , macrosdir , demosdir , modulename , %t )

