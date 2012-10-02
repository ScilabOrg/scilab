// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2009 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->

// Execute with exec("SCI/modules/xcos/tests/unit_tests/rk10-2.tst");

for i=1:5	// n, order
 for j=1:5	// K, initial slope
  for k=1:5	// tau, integration time

// RK + modification du contexte + sauvegarde
assert_checktrue(importXcosDiagram("SCI/modules/xcos/demos/AArk10-2.zcos"));
scs_m.props.context = ["n = "+string(i)+";" "K = "+string(j)+";" "tau = "+string(k)+";"];
xcos_simulate(scs_m, 4);
rkval = rk.values;
time = rk.time;

// CVode + modification du contexte + sauvegarde
assert_checktrue(importXcosDiagram("SCI/modules/xcos/demos/AAcv10-2.zcos"));
scs_m.props.context = ["n = "+string(i)+";" "K = "+string(j)+";" "tau = "+string(k)+";"];
xcos_simulate(scs_m, 4);
cvval = cv.values;

// Comparaison des résultats
compa = abs(rkval-cvval);

// Moyenne, Ecart-type, Max
moy = mean(compa);
[maxi, indmaxi] = max(compa);
ecarttype = st_deviation(compa);

// Vérification de la proximité des méthodes
if (maxi >= 0.001) then pause, end

// Vecteur [moyenne, écart-type], vecteur [max, temps_max]
vec = [moy, ecarttype];
maxiT = [maxi time(indmaxi)];

// Affichages
//disp("[moyenne, ecart-type] : ");
disp(vec);
//disp("[ecart max, au temps]: ");
//disp(maxiT);

  end
 end
end
