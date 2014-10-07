// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

loadXcosLibs();

// Allocate a diagram and access its fields
scs_m = scicos_diagram()
scs_m.props
scs_m.objs
scs_m.version
scs_m.contrib

// Create a diagram containing 2 summation blocks and two links
Sum    = BIGSOM_f("define");
Scope1 = CSCOPE("define");
Scope2 = CSCOPE("define");
lnk1   = scicos_link();          // Data link
lnk2   = scicos_link(ct=[1,-1]); // Activation link

scs_m = scicos_diagram( objs=list(Sum,Scope1,Scope2,lnk1,lnk2) )
scs_m.objs

// Link output port #1 of block #1 with input port #1 of block #2 thanks to lnk1
scs_m.objs(4).from = [1 1 0]; // Link block #1
scs_m.objs(1).graphics.pout   // Check that block #1 is connected to lnk1
scs_m.objs(1).model.out       // "
scs_m.objs(4).to = [2 1 1];   // Link block #2
scs_m.objs(4).from
scs_m.objs(4).to
scs_m.objs(2).graphics.pin    // Check that block #2 is connected to lnk1
scs_m.objs(2).model.in        // "

// Change the end of the link to input port #2 of block #2
scs_m.objs(4).to = [2 2 1];
scs_m.objs(4).from
scs_m.objs(4).to
scs_m.objs(2).graphics.pin    // Check that block #2 is connected to lnk1
scs_m.objs(2).model.in        // "

// Disconnect the source
scs_m.objs(4).from = [0 0 0];
scs_m.objs(4).from
scs_m.objs(4).to
scs_m.objs(1).graphics.pout   // Check that block #1 is not connected
scs_m.objs(2).graphics.pin    // But block #2 is still connected to the link

// Disconnect the destination
scs_m.objs(4).to = [0 0 0];
scs_m.objs(2).graphics.pin    // Check that block #2 is not connected

// Now link the two Scope blocks together by adding an event output port to block #3 thanks to lnk2
scs_m.objs(5).from = [2 1 1]; // Link the input of block #2
scs_m.objs(5).to = [3 1 0];   // Add an event output to block #3 and link it to the previous
scs_m.objs(5).from
scs_m.objs(5).to
scs_m.objs(2).graphics.pein   // Check that block #2 is connected to lnk2
scs_m.objs(2).model.evtin     // "
scs_m.objs(3).graphics.peout  // Check that block #3 is connected to lnk2
scs_m.objs(3).model.evtout    // "

// Verify that it is impossible to link two inputs or two outputs together
scs_m.objs(5).from = [2 1 0]; // Two outputs
scs_m.objs(5).to = [3 1 1];   // Two inputs


//===================================================================================================
// Test predefined link at diagram creation
clear scs_m;

lnk   = scicos_link( from=[1 1 0],to=[2 1 1] );
scs_m = scicos_diagram( objs=list(Sum,Scope1,lnk) );

scs_m.objs(3).fromtests/unit_tests/model/SuperBlock.dia.ref
scs_m.objs(3).to
scs_m.objs(1).graphics.pout   // Check that block #1 is connected to lnk
scs_m.objs(1).model.out       // "
scs_m.objs(2).graphics.pin    // Check that block #2 is connected to lnk
scs_m.objs(2).model.in        // "


//===================================================================================================
// Test predefined link insertion
clear scs_m;

lnk   = scicos_link( from=[1 1 0],to=[2 1 1] );
scs_m = scicos_diagram( objs=list(Sum,Scope1) );

scs_m.objs(1).graphics.pout   // Check that block #1 is not connected
scs_m.objs(1).model.out       // "
scs_m.objs(2).graphics.pin    // Check that block #2 is not connected
scs_m.objs(2).model.in        // "

scs_m.objs(3) = lnk;          // Add the predefined Link

scs_m.objs
scs_m.objs(3).from
scs_m.objs(3).to
scs_m.objs(1).graphics.pout   // Check that block #1 is connected to lnk
scs_m.objs(1).model.out       // "
scs_m.objs(2).graphics.pin    // Check that block #2 is connected to lnk
scs_m.objs(2).model.in        // "


//===================================================================================================
// Test Links insertion before Block insertion
clear scs_m;

lnk   = scicos_link( from=[2 1 0],to=[3 1 1] );
scs_m = scicos_diagram();

scs_m.objs(1) = lnk;          // Add the predefined Link
scs_m.objs(1).from
scs_m.objs(1).to

scs_m.objs(2) = Sum;          // Add the Block that lnk's 'from' points to
scs_m.objs(3) = Scope1;       // Add the Block that lnk's 'to' points to

scs_m.objs

// Check that the linking has been done
// Sum (#2 in diagram) is linked to Scope1 (#3) through lnk (#1)
scs_m.objs(1).from
scs_m.objs(1).to

scs_m.objs(2).graphics.pout
scs_m.objs(2).model.out
scs_m.objs(3).graphics.pin
scs_m.objs(3).model.in
