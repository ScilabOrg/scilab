// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA -
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//===============================
// unit tests Toolboxes functions
//===============================

// on modifie la position du dossier contenant les toolboxes pour les tests
function rep = toolboxDirectory()
  rep = "home/dev/package_manager/modules/atoms/tests/unit_tests/toolboxes/"
endfunction

// lecture DESCRIPTION en local (/!\ l'ordre des champs importe pour la comparaison verif <> test)
verif("Toolbox") = "testA";
verif("Version") = "1.0.0";
verif("Title") = "TestA";
verif("Author") = "Delphine";
verif("Maintainer") = "Delphine";
verif("Description") = "Test de code scilab pour lire les DESCRIPTION. Avec une ligne de description qui est plus longue que la longueur d''une ligne, pour tester les lignes multiples.";
verif("License") = "Aucune";
verif("Category") = "essai";
verif("Date") = "2008-03-31";
verif("Depends") = "testB, testC, testH";
verif("URL") = "";
verif("Entity") = "";
verif("Function")("1") = "Toolbox - testA, TestA";
verif("Function")("2") = "mafonction1 - ma super fonction fait le cafe";
verif("Function")("3") = "mafonction2 - ma super fonction 2 fait le menage";
verif("Function")("4") = "mafonction3 - ma super fonction 3 fait la vaisselle, lave le linge, repasse, lave la cuisine et la salle de bain, il ne lui manque rien de rien.";
verif("Function")("5") = "mafonction4 - ne sert a rien";
test = readDescription("testA"); if verif <> test then pause, end;
// chargement de toutes les toolbox sur un mirror
function listMirror = toolboxMirror()
  listMirror = ["http://128.93.23.238/scilab/src/contrib"]
endfunction
test = readDescription("")
[n, m] = size(test("Toolbox")); if n <> 20 then pause, end;
[n, m] = size(test("Version")); if n <> 20 then pause, end;
[n, m] = size(test("Title")); if n <> 20 then pause, end;
[n, m] = size(test("Author")); if n <> 20 then pause, end;
[n, m] = size(test("Maintainer")); if n <> 20 then pause, end;
[n, m] = size(test("Description")); if n <> 20 then pause, end;
[n, m] = size(test("License")); if n <> 20 then pause, end;
[n, m] = size(test("Date")); if n <> 20 then pause, end;
[n, m] = size(test("Depends")); if n <> 20 then pause, end;
[n, m] = size(test("URL")); if n <> 20 then pause, end;
[n, m] = size(test("Entity")); if n <> 20 then pause, end;
[n, m] = size(test("Function")); if n <> 20 then pause, end;

// on choisi un Mirror non conforme
function listMirror = toolboxMirror()
  listMirror = ["http://128.93.23.238/scilab/bin/linux/contrib"];
endfunction
// chargement de toutes les toolbox sur le web
test = readDescription(""); if size(test("Toolbox")) <> 0 then pause, end;
// on remet les bon Mirrors
function listMirror = toolboxMirror()
  listMirror = ["http://128.93.23.238/scilab/src/contrib", "http://128.93.23.238/scilab/bin/windows/contrib"];
endfunction

// installation avec la dernière dependance non présente sur le web ni en local
if installToolbox("testA") <> %f then pause, end;
// installation avec conflit versions
if installToolbox("conflitA") <> %f then pause, end;
// installation avec la dépendance locale ne vérifiant pas la condition
if installToolbox("local1") <> %f then pause, end;

// supression de toolbox non présente
if removeToolbox("non") <> %f then pause, end;

// update d'une toolbox non présente
if updateToolbox("non") <> %f then pause, end;
// update d'une toolbox non présente sur le net
if updateToolbox("truc") <> %f then pause, end;
// update d'une toolbox dependance d'une autre
function listMirror = toolboxMirror()
  listMirror = ["http://128.93.23.238/scilab/src/contrib"];
endfunction
if installToolbox("updateA") <> %t then pause, end;
function listMirror = toolboxMirror()
  listMirror = ["http://128.93.23.238/scilab/src/contrib", "http://128.93.23.238/scilab/bin/macosX/contrib"];
endfunction
// Mauvaise version
if updateToolbox("updateB") <> %f then pause, end;
function listMirror = toolboxMirror()
  listMirror = ["http://128.93.23.238/scilab/src/contrib", "http://128.93.23.238/scilab/bin/windows/contrib"];
endfunction
// Bonne version
if updateToolbox("updateB") <> %t then pause, end;
// Suppression des toolboxes
if removeToolbox("updateA") <> %t then pause, end;
if removeToolbox("updateB") <> %t then pause, end;

