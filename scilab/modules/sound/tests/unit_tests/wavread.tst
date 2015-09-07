//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

a = wavread('SCI/modules/sound/demos/chimes.wav','size');
assert_checkequal(a, [2 13921]);
[y,Fs,bits]=wavread('SCI/modules/sound/demos/chimes.wav');
assert_checkequal(Fs, 22050);
assert_checkequal(bits, 16);
assert_checkequal(size(y), [ 2 13921]);

y=wavread('SCI/modules/sound/demos/chimes.wav',[1 5]); //the first five samples
valeur = [ 0.00006103515625, 0.000274658203125, 0.000213623046875, 0.000152587890625, 0.000091552734375; 0.000091552734375, 0.00018310546875, 0.00006103515625, 0, 0.000091552734375];
assert_checkequal( y, valeur);
assert_checkequal(size(y), [2 5]);
