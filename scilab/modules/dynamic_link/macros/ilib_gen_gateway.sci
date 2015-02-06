// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA/ENPC
// Copyright (C) DIGITEO - 2010 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

//=============================================================================
// ilib_gen_gateway_cpp used by ilib_build
//=============================================================================
// generate an interface gateway named name
// from table table taking into account
// attributes of function i.e mex fmex or scilab interface
// if name is a full path just extract the filename

function gateway_filename = ilib_gen_gateway(name,tables)

    [lhs,rhs] = argn(0);
    if rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input argument(s).\n"), "ilib_gen_gateway"));
        return
    end

    gateway_filename = ["","",""];
    k = strindex(name,["/","\"]);
    if k~=[] then
        path = part(name,1:k($));
        name = part(name,k($)+1:length(name));
    else
        path="";
    end

    [path_name, file_name, ext_name] = fileparts(name);
    if ext_name == ".c" then
        name = path_name + file_name;
    else
        name = path_name + file_name + ext_name;
    end

    if typeof(tables)<>"list" then
        tables = list(tables);
    end

    L = length(tables);

    for itable = 1:L
        // loop on a list of tables
        if L <> 1 then
            tname = name +string(itable);
        else
            tname = name ;
        end

        table = tables(itable);
        [mt,nt] = size(table);

        if (nt == 2) then
            col = "csci";
            table = [table, col(ones(mt,1))];
            nt = 3;
        end

        if ( nt <> 3 ) then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: %d expected.\n"),"ilib_gen_gateway",2,3));
        end
        [gate,names,cppCompilation] = new_names(table);
        global cppCompilation;
        //generate cpp interface file
        if  cppCompilation then
            t = [
            "#include ""context.hxx""";
            "#include ""cpp_gateway_prototype.hxx""";
            "#include """ + tname + ".hxx""";
            "extern ""C""";
            "{";
            "#include """ + tname + ".h""";
            "}";
            "";
            "#define MODULE_NAME L""" + tname + """";
            "";
            "int " + tname + "(wchar_t* _pwstFuncName)";
            "{";
            "    if(wcscmp(_pwstFuncName, L""" + table(:,1) + """) == 0){ " + "symbol::Context::getInstance()->addFunction(types::Function::createFunction(L""" + table(:,1) + """, &" + names(:) + ", MODULE_NAME)); }";
            "";
            "    return 1;";
            "}"];
        else
            t = [
            "#include <wchar.h>";
            "#include """ + tname + ".hxx""";
            "extern ""C""";
            "{";
            "#include """ + tname + ".h""";
            "#include ""addfunction.h""";
            "}";
            "";
            "#define MODULE_NAME L""" + tname + """";
            "";
            "int " + tname + "(wchar_t* _pwstFuncName)";
            "{";];

            for kGw = 1:size(names, "*")
                if or(table(kGw, 3) == ["cmex" "fmex" "Fmex"]) then
                    t = [t;
                    "    if(wcscmp(_pwstFuncName, L""" + table(:,1) + """) == 0){ " + "addMexFunction(L""" + table(:,1) + """, &" + names(:) + ", MODULE_NAME); }"];
                else
                    t = [t;
                    "    if(wcscmp(_pwstFuncName, L""" + table(:,1) + """) == 0){ " + "addCFunction(L""" + table(:,1) + """, &" + names(:) + ", MODULE_NAME); }"];
                end
            end

            t = [t;
            "";
            "    return 1;";
            "}"];
        end

        gateway_filename(1) = path + tname + ".cpp";
        // first check if we have already a gateway
        [fd, ierr] = mopen(gateway_filename(1), "rt");
        if ierr == 0 then
            // file already exists
            t1 = mgetl(fd);
            mclose(fd);
            if or(t1 <> t) then
                mputl(t, gateway_filename(1));
            end
        else
            // file does not exist we create it
            mputl(t, gateway_filename(1)) ;
        end

        if ilib_verbose() > 1 then
            disp(t);
        end

        //prepare .hxx file
        TNAME = convstr(tname, "u");
        t = [
        "#ifndef __" + TNAME + "_GW_HXX__";
        "#define __" + TNAME + "_GW_HXX__";
        "";
        "#ifdef _MSC_VER";
        "#ifdef " + TNAME + "_GW_EXPORTS";
        "#define " + TNAME + "_GW_IMPEXP __declspec(dllexport)";
        "#else";
        "#define " + TNAME + "_GW_IMPEXP __declspec(dllimport)";
        "#endif";
        "#else";
        "#define " + TNAME + "_GW_IMPEXP";
        "#endif";
        "";
        "extern ""C"" " + TNAME + "_GW_IMPEXP int " + tname + "(wchar_t* _pwstFuncName);";
        "";
        gate(:, 2);
        "";
        "#endif /* __" + TNAME + "_GW_HXX__ */"];

        gateway_filename(2) = path + tname + ".hxx";
        // first check if we have already a gateway
        [fd, ierr] = mopen(gateway_filename(2), "rt");
        if ierr == 0 then
            // file already exists
            t1 = mgetl(fd);
            mclose(fd);
            if or(t1 <> t) then
                mputl(t, gateway_filename(2));
            end
        else
            // file does not exist we create it
            mputl(t, gateway_filename(2)) ;
        end

        //prepare .h file
        TNAME = convstr(tname, "u");
        t = [
        "#ifndef __" + TNAME + "_GW_H__";
        "#define __" + TNAME + "_GW_H__";
        "";
        "#include ""c_gateway_prototype.h""";
        "";
        gate(:, 1);
        "";
        "#endif /* __" + TNAME + "_GW_H__ */"];

        gateway_filename(3) = path + tname + ".h";
        // first check if we have already a gateway
        [fd, ierr] = mopen(gateway_filename(3), "rt");
        if ierr == 0 then
            // file already exists
            t1 = mgetl(fd);
            mclose(fd);
            if or(t1 <> t) then
                mputl(t, gateway_filename(3));
            end
        else
            // file does not exist we create it
            mputl(t, gateway_filename(3)) ;
        end

        if ilib_verbose() > 1 then
            disp(t);
        end
    end
endfunction
//=============================================================================
// new_names only used by ilib_gen_gateway
//=============================================================================
function [gate,names,cppCompilation] = new_names(table)
    // change names according to types
    cppCompilation = %f;
    [mt,nt] = size(table);
    gate = [""];
    gate = gate(ones(mt, 2));
    names = " ";
    names = names(ones(mt,1));
    for i = 1:mt
        select table(i,3)
        case "cmex" then
            names(i) = "mex_" + table(i,2);
            gate(i, 1) = "MEX_GATEWAY_PROTOTYPE(" + names(i) + ");";
        case "fmex" then
            names(i) = "C2F(mex" + table(i,2) + ")";
            gate(i, 1) = "MEX_GATEWAY_PROTOTYPE(" + names(i) + ");";
        case "Fmex" then
            names(i) = "C2F(mex" + table(i,2) + ")";
            gate(i, 1) = "MEX_GATEWAY_PROTOTYPE(" + names(i) + ");";
        case "csci"  then
            names(i) = table(i,2);
            gate(i, 1) = "C_GATEWAY_PROTOTYPE(" + names(i) + ");";
        case "fsci"  then
            names(i) = "C2F(" + table(i,2) + ")";
            gate(i, 1) = "C_GATEWAY_PROTOTYPE(" + names(i) + ");";
        case "cppsci"  then
            cppCompilation = %t;
            names(i) = table(i,2);
            gate(i, 2) = "CPP_GATEWAY_PROTOTYPE(" + names(i) + ");";
        case "direct"  then
            names(i) = table(i,2);
            gate(i, 1) = "C_GATEWAY_PROTOTYPE(" + names(i) + ");";
        else
            error(999,"Wrong interface type " + table(i,3));
        end
    end
endfunction
//=============================================================================
