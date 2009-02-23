// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre KOUMAR 
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH TREE -->

// test to check the created node(Tree)

// node creation
myNode = createNode('My Node')
myNode = createNode('My Node', 'icon of My Node')
myNode = createNode('My Node', 'icon of My Node', 'callback of My Node')


// wrong inputs
if( execstr("myNode = createNode()","errcatch") == 0 ) then pause,end

if( execstr("myNode = createNode(123)","errcatch") == 0 ) then pause,end

if( execstr("myNode = createNode(123, ''icon of My Node'')","errcatch") == 0 ) then pause,end
if( execstr("myNode = createNode(''My Node'', 123)","errcatch") == 0 ) then pause,end

if( execstr("myNode = createNode(123, ''icon of My Node'', ''callback of My Node'')","errcatch") == 0 ) then pause,end
if( execstr("myNode = createNode(''My Node'', 123, ''callback of My Node'')","errcatch") == 0 ) then pause,end
if( execstr("myNode = createNode(''My Node'', ''icon of My Node'', 123)","errcatch") == 0 ) then pause,end

if( execstr("myNode = createNode(''My Node'', ''icon of My Node'', ''callback of My Node'', ''test'')","errcatch") == 0 ) then pause,end
if( execstr("myNode = createNode(''My Node'', ''icon of My Node'', ''callback of My Node'', 123)","errcatch") == 0 ) then pause,end
