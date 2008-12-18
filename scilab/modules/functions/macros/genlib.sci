// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) ENPC
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function genlib(nam,path,force,verbose,names)
  
// get all .sci files in the specified directory
  
  if exists('force','local')==0 then force = %f,end
  if exists('verbose','local')==0 then verbose = %f,end
  
  W          = who('get');
  np         = predef();
  predefined = or(W($-np+1:$)==nam);
  
  if verbose then
    mprintf(gettext("-- Creation of [%s] (Macros) --\n"),nam);
  end
  
  updatelib=%f //true if updating an already defined library
  
  //check if a library with the same name exists
  
  oldlib    = [];
  old_path  = [];
  old_names = [];
  
  if exists(nam)==1 then
    execstr('oldlib='+nam)
    if typeof(oldlib)=='library' then
      //yes, get its path and function list
      old_names = string(oldlib);
      clear oldlib
      old_path     = old_names(1);
      old_names(1) = [];
    end
  end
  
  if exists('path','local')==0&old_path<>[] then
    path      = old_path;
    updatelib = %t;
  end
  
  // convert path according to MSDOS flag
  // without env variable substitution
  
  path1 = pathconvert(path,%t,%f);
  // with env subsitution
  path = pathconvert(path,%t,%t);

  if exists('names','local')==0 then
    // list the sci files
    files = listfiles(path+'*.sci',%f);
    
    if MSDOS then
      // remove wrong files extension
      // bug 2289
      ext = fileext(files);
      if ~and( (ext == '.sci') ) then
	tmp_files = [];
	for i = 1:size(files,'*');
	  if ext(i) == '.sci' then
	    tmp_files = [tmp_files;files(i)];
	  end
	end
	files = tmp_files;
	clear tmp_files;
      end
    end
    
    if files==[] | files== "" then
      warning(msprintf(gettext("%s: No files with extension %s found in %s\n"),"genlib",".sci", path));
      return ;
    end
    names = basename(files,%f);
  else
    files = path+names
	names=strsubst(names, "/\.sci$/",'','r') 
  end
  
  names_changed = %t;
  
  if updatelib then
    if and(sort(names)==sort(old_names)) then names_changed=%f,end
  end
  
  modified=%f
  
  if force == %t then
    modified = %t;
    for i=1:size(files,'*')  // loop on .sci files
      scif = files(i);
      if verbose then
	mprintf(gettext("%s: %s file compilation forced\n"),"genlib",names(i)+".sci");
      end
      // getf sci file and save functions it defines as a .bin file
      getsave(scif);
    end
  else
    for i=1:size(files,'*')  // loop on .sci files
      scif      = files(i);
	  binf      = strsubst(names, "/\.sci$/",'.bin','r')
      binf_info = fileinfo(binf);
      recompile = %f;
      
      if binf_info == [] then
	recompile = %t;
      else
	scif_info = fileinfo(scif);
	if newest(scif,binf) == 1 then
	  recompile = %t ;
	end
      end
      
      if recompile == %t then
	
	if verbose then
	  mprintf(gettext("%s: Processing file: %s\n"),"genlib",names(i)+".sci");
	end
	
	// getf sci file and save functions it defines as a .bin file
	getsave(scif);
	modified = %t;
      end
    end
  end
  
  if modified then
    
    if verbose then
      mprintf(gettext("%s: Regenerate names and lib\n"),"genlib");
    end
    
    if names_changed
      mputl(names,path+'names') // write 'names' file in directory
    end
    
    // create library
    execstr(nam+'=lib('''+path1+''')')
    //save it
    
    if execstr('save('''+path1+'lib'''+','+nam+')','errcatch')<>0 then
      error(msprintf(gettext("%s: %s file cannot be created\n"),"genlib",path+'lib'));
    end
  else
    execstr(nam+'=lib('''+path1+''')')
  end
  
  if names_changed then
    if ~predefined then
      execstr(nam+'=resume('+nam+')')
    else
      mprintf(gettext("Library file %s has been updated,\nbut cannot be loaded into Scilab because %s is a protected variable.\n"),path1+'lib',nam);
    end
  end
  
endfunction

function result = getsave(fl)
  
// utility function
// performs a getf on file fl
  result = %f;
  prot   = funcprot();
  nold   = size(who('get'),'*');
  
  funcprot(0);
  
  ierr=execstr('exec(fl);','errcatch') // get functions defined in file 'fl'
  
  if ierr<> 0 then
    clear ierr;
    mprintf(gettext("%s: Warning: Error in file %s : %s. File ignored\n"),"genlib",fl,lasterror())
    result = %f;
  else
    clear ierr;
    
    // lookfor names of the functions defined in file 'fl'
    new = who('get')
    new = new(1:(size(new,'*')-nold-1))
    
    // create output file name
	fl  = strsubst(fl, "/\.sci$/",'.bin','r')
    // save all functions in the output file
    [u,ierr]=mopen(fl,'wb')
    if ierr<>0 then
      clear ierr;
      nf = length(fl);
      if nf>40 then
	fl='...'+part(fl,nf-40:nf);
      end
      error(msprintf(gettext("%s: Impossible to open file %s for writing\n"),"genlib",fl));
    end
    
    clear ierr
    
    if new<>[] then
      execstr('save(u,'+strcat(new($:-1:1),',')+')');
    else
      msprintf(gettext("%s: File %s does not contain any function.\n"),"genlib",fl)
      result = %f;
    end
    
    mclose(u);
  end
  
  funcprot(prot);
  
endfunction
