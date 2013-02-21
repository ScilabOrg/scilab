// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Simon MARCHETTO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

//===============================
// unit tests completion
//===============================

// File paths

if (getos() == "Windows") then;
    r = completion('C:\Documents and');
    if r <> 'Documents and Settings\' then pause,end
else;
    r = completion('/tm');
    if r <> '/tmp' then pause,end
end;

r = completion(SCI+'/t');
if r <> ['thirdparty\'; 'tools\'] then pause,end


// Predefined variables

r = completion('ho', 'variables');
if r <> 'home' then pause,end

r = completion('%na');
if r <> '%nan' then pause,end

r = completion('TMP');
if r <> 'TMPDIR' then pause,end

r = completion('%', 'variables');
if r <> ['%F'; '%T'; '%e'; '%eps'; '%f'; '%fftw'; '%gui'; '%i'; '%inf'; '%io';'%modalWarning';'%nan';'%pi';'%s';'%t';'%tk';'%toolboxes';'%toolboxes_dir';'%z'] then pause, end


// User variables

variable_completion1 = 1;
variable_completion2 = 2;
r = completion('variable_comp');
if r <> ['variable_completion1'; 'variable_completion2'] then pause,end


// Predefined functions, macros and commands

r = completion('floo');
if r <> 'floor' then pause,end

r = completion('abort');
if r <> 'abort' then pause,end

// User functions, macros, commands
function [] = function_completion(x);
endfunction;
r = completion('function_comp');
if r <> 'function_completion' then pause,end


// Filter argument

r = completion('ho');
if r <> ['home'; 'horizontalalignment'; 'horner'; 'host'; 'householder'] then pause,end

r = completion('ho', 'functions');
if r <> 'host' then pause,end

r = completion('ho', 'commands');
if r <> [] then pause,end

r = completion('ho', 'macros');
if r <> ['horner'; 'householder'] then pause,end

r = completion('ho', 'variables');
if r <> 'home' then pause,end

r = completion('ho', 'graphic_properties');
if r <> 'horizontalalignment' then pause,end

r = completion('ho', 'files');
if r <> [] then pause,end

r = completion('abo');
if r <> ['abort'; 'about'] then pause,end

r = completion('abo', 'functions');
if r <> 'about' then pause,end

r = completion('abo', 'commands');
if r <> 'abort' then pause,end

r = completion('abo', 'variables');
if r <> [] then pause,end

r = completion('abo', 'macros');
if r <> [] then pause,end

r = completion('abo', 'graphic_properties');
if r <> [] then pause,end

r = completion('abo', 'files');
if r <> [] then pause,end


// Output arguments

[functions,commands,variables,macros,graphic_properties,files] = completion('ho');
if functions <> ['host'] then pause,end
if commands <> [] then pause,end
if variables <> ['home'] then pause,end
if macros <> ['horner'; 'householder'] then pause,end
if graphic_properties <> ['horizontalalignment'] then pause,end
if files <> [] then pause,end

[functions,commands,variables,macros,graphic_properties,files] = completion('abo');
if functions <> ['about'] then pause,end
if commands <> ['abort'] then pause,end
if variables <> [] then pause,end
if macros <> [] then pause,end
if graphic_properties <> [] then pause,end
if files <> [] then pause,end

if (getos() == "Windows") then;
    [functions,commands,variables,macros,graphic_properties,files] = completion('C:\Documents and');
    if files <> ['Documents and Settings\'] then pause,end
else;
    [functions,commands,variables,macros,graphic_properties,files] = completion('/tm');
    if files <> ['/tmp'] then pause,end
end;
