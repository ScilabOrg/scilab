// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2009 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->

// Execute with exec("SCI/modules/xcos/tests/unit_tests/rk.tst");

// RK + sauvegarde
assert_checktrue(importXcosDiagram("SCI/modules/xcos/demos/Aordre1rk.zcos"));
xcos_simulate(scs_m, 4);
rkval = y.values;
time = y.time;

// CVode + sauvegarde
assert_checktrue(importXcosDiagram("SCI/modules/xcos/demos/Aordre1cv.zcos"));
xcos_simulate(scs_m, 4);
cvval = y.values;

// Comparaison des résultats
compa = abs(rkval-cvval);
assert_checktrue(compa <= 0.01);

// Moyenne, Ecart-type, Max
moy = mean(compa);
[maxi, indmaxi] = max(compa);
ecarttype = st_deviation(compa);
maxiT = [maxi time(indmaxi)];

// Vecteur [moyenne, écart-type]
vec = [moy, ecarttype];

// Affichages
//disp("[moyenne, ecart-type] : ");
//disp(vec);
//disp("[ecart max, au temps]: ");
//disp(maxiT);
