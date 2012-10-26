// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================
//
// <-- ENGLISH IMPOSED -->
// <-- XCOS TEST -->
//
// <-- Short Description -->
// Blocks must have valid dimensions for their settings.
// Some dimensions were not coherents between theirs "set" and "define" method.


defaultlibs = ["Branching",
               "Electrical",
               "Hydraulics",
               "Linear",
               "Sources",
               "Misc",
               "Events",
               "Integerop",
               "Matrixop",
               "Nonlinear",
               "Sinks",
               "Threshold"];

defaultlibs  = defaultlibs + "lib";
notTested = ["DSUPER" "SUPER_f" "TEXT_f" "PAL_f" ,..   // Specific blocks
            , ..                                       // native blocks
            "CBLOCK" "CBLOCK4" "c_block" "fortran_block",..
            ,..                                        // not interface function
            "WFILE_f", ..                              // Deprecated blocks
            "vanne_inputs" "vanne_outputs" "vanne_draw_ports" ,..
            "source_inputs" "source_outputs" "source_draw_ports" ,..
            "puit_inputs" "puit_outputs" "puit_draw_ports" ,..
            "bache_inputs" "bache_outputs" "bache_draw_ports" ,..
            "tkscaleblk", "m_sin", "bplatform2", "anim_pen" ,..
            "BARXY_sim" ];

funcprot(0);
ilib_verbose(0);
// Stubbing global scicos flags
needcompile = 0;
alreadyran = %f;
%scicos_context = struct();

// If the following block display something, the test is failed.
for i = 1:size(defaultlibs,"*")
    [macros, path] = libraryinfo(defaultlibs(i));
    macros = gsort(macros);
    for j = 1:size(macros,"*")
        interfunction = macros(j);

        // Not tested blocks (Xcos customs)
        if or(interfunction == notTested) then
          continue;
        end

        // Test scs_m = BLOCK("define") then scs_m = BLOCK("set", scs_m)
        [status, message] = xcosValidateBlockSet(interfunction);
        if status == %f
            disp(message)
        end
        assert_checktrue(status);
    end
end

