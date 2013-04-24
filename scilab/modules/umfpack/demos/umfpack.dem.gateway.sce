// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================


function subdemolist = demo_gateway()

    demopath = get_absolute_file_path("umfpack.dem.gateway.sce");
    add_demo(gettext("UMFPACK"), demopath+"umfpack.dem.gateway.sce");

    subdemolist = [_("Sparse Matrices"), "scisptdem1.dem"
    _("Harwell-Boeing"), "scisptdem2.dem"
    _("condestsp"), "scisptdem3.dem"];

    subdemolist(:,2) = demopath + subdemolist(:,2);

endfunction

subdemolist = demo_gateway();
clear demo_gateway;
