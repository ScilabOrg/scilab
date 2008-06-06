// Construction de la liste des dépendances
// avril 2008 by Delphine

function listeDep = checkConflict(nom, version)
  global conflictLocal
  global conflictingList
  global nomconflictLocal
  global conflictVersion
  // S'il n'y a pas de version
  if argn(2) == 1
    version = ""
  end
  clearglobal conflictingList
  listeDep = checkDependencies(nom, version)
  [n, m] = size(listeDep)
  for i=1:n-1
    // On compare les nom
    [nom1, version1] = separateVersionDep(listeDep(i))
    [nom2, version2] = separateVersionDep(listeDep(i+1))
    if nom1 == nom2
      // s'il y a 2 versions possible c'est que forcément il y a une dep <=
      // Si c'est le cas, il faut que cette version max concorde avec les autres dép
      // on l'installe en local, et si le tag conflictLocal est à 1 il y a conflit version, sinon, c'est ok
      v1 = decoupVersion(version1)
      v2 = decoupVersion(version2)
      if compareVersion(v1, ">=", v2)
        dlInstall(nom1, version2)
      else
        dlInstall(nom1, version1)
      end
      clearglobal conflictLocal
      clearglobal conflictingList
      clearglobal nomconflictLocal
      clearglobal conflictVersion
      listeDep2 = checkDependencies(nom, version)
      if conflictLocal == 1
        conflictVersion = 1
        rep = toolboxDirectory()
        rmdir(rep + nom1, "s")
      end
    end
  end
endfunction

function listeDep = checkDependencies(nom, version)
  // Pour éviter les redondances et les boucles
  global conflictingList
  global conflictLocal
  global nomconflictLocal
  global conflictVersion
  // S'il n'y a pas de version
  if argn(2) == 1
    version = ""
  end
  // Lecture du fichier description
  desc = readDescription(nom)
  // Selection de la position de la toolbox dans la liste des toolboxes disponibles.
  position = selectPosition(desc, nom, version)
  // Cas ou la toolboxe n'est pas présente
  if position == 0
    displayMessage("Toolbox " + nom + " non trouvee")
    listeDep = ""
    return listeDep
  elseif position == 0.1
    conflictLocal = 1
    nomconflictLocal = nom
    listeDep = ""
    return listeDep
  end
  // Si la toolbox a déjà été rencontrée sous cette version, inutile de refaire l'arbre des dépendances
  if find(conflictingList == (desc("Toolbox")(position) + " (" + desc("Version")(position) + ")"))
    resume
  end
  listeDep = desc("Toolbox")(position) + " (" + desc("Version")(position) + ")"
  [a, b] = size(conflictingList)
  conflictingList(a+1) = desc("Toolbox")(position) + " (" + desc("Version")(position) + ")"
  // Récuperation des dependances
  depends = extractValue("Depends", desc, position)
  // S'il n'y a pas de dépendance
  if depends == " " | depends == ""
  	return listeDep
  end
  // S'il y a plusieurs dépendances on split en plusieurs string
  depends = splitValue(depends, ',')
  // Ajout dans la liste des toolboxes à installer
  [n, m] = size(depends)
  for i=1:n
	[nb, x] = size(listeDep)
	// Separation de la version et de la dependance
	[depend, version] = separateVersionDep(depends(i))
	// Recherche des dépendances
	deps = checkDependencies(depend, version)
	[o, p] = size(deps)
	for i=1:o
      listeDep(nb+i)=deps(i)
    end	
  end
  listeDep = unique(listeDep)
  return listeDep
endfunction
