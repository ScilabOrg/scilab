// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Clement DAVID
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//==============================================================================

function tbx_generate(NAME, TITLE, code, info)
    // Generate a new external module (eg. toolbox) using the toolbox_skeleton
    //
    // Calling Sequence
    // tbx_generate(NAME, TITLE) // generate a new module
    // tbx_generate(NAME, TITLE, code) // generate a new module with a kind of code
    // tbx_generate(NAME, TITLE, code, info) // generate a new module with extra informations
    //
    // Parameters   
    // NAME: the name (used for filenames) of the created external module
    // TITLE: the title (any character allowed) of the created external module
    // code: the kind of language used, can be : "c" "cpp" "java" "fortran"
    // info: a structure used to pass extra informations to the generator (described below)
    //
    
    if fileinfo("SCI/contrib/toolbox_skeleton") == [] then
        error(msprintf("%s: the toolbox skeleton is not available", "tbx_generate"));
    end
    
    rhs = argn(2);
    if 2 > rhs | rhs > 4 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"), "tbx_generate", 2, 4))
    end
    
    [status, message] = copyfile("SCI/contrib/toolbox_skeleton", NAME);
    if status == 0 then
        error(message);
    end
    
    //
    // Delete some files provided as references
    //
    deletefile(NAME+"/toolbox_skeleton_redist.iss")
    
    //
    // Rename files
    //
    [status, message] = movefile(NAME+"/etc/toolbox_skeleton.start", NAME+"/etc/"+NAME+".start")
    if status == 0 then
        error(message);
    end
    [status, message] = movefile(NAME+"/etc/toolbox_skeleton.quit", NAME+"/etc/"+NAME+".quit")
    if status == 0 then
        error(message);
    end
    [status, message] = movefile(NAME+"/etc/toolbox_skeleton_preferences.xsl", NAME+"/etc/"+NAME+"_preferences.xsl")
    if status == 0 then
        error(message);
    end
    [status, message] = movefile(NAME+"/etc/toolbox_skeleton_preferences.xml", NAME+"/etc/"+NAME+"_preferences.xml")
    if status == 0 then
        error(message);
    end
    [status, message] = movefile(NAME+"/demos/toolbox_skeleton.dem.gateway.sce", NAME+"/demos/"+NAME+".dem.gateway.sce")
    if status == 0 then
        error(message);
    end
    
    //
    // Update some file content accordingly to the NAME and TITLE
    //
    function update_name_and_title(f)
        content=mgetl(f);
        [row, which] = grep(content, ["toolbox_skeleton" "Toolbox Skeleton"]);
        for i=row(which==1),
            content(i)=strsubst(content(i), "toolbox_skeleton", NAME);
        end
        for i=row(which==2),
            content(i)=strsubst(content(i), "Toolbox Skeleton", TITLE);
        end
        mputl(content, f);
    endfunction
    
    update_name_and_title(NAME+"/readme.txt");
    update_name_and_title(NAME+"/changelog.txt");
    update_name_and_title(NAME+"/DESCRIPTION");
    update_name_and_title(NAME+"/builder.sce");
    update_name_and_title(NAME+"/etc/"+NAME+".start");
    update_name_and_title(NAME+"/etc/"+NAME+".quit");
    update_name_and_title(NAME+"/etc/"+NAME+"_preferences.xsl");
    update_name_and_title(NAME+"/etc/"+NAME+"_preferences.xml");
    update_name_and_title(NAME+"/demos/"+NAME+".dem.gateway.sce");
    
endfunction

// test
tbx_generate("foo", "Foo toolbox")
