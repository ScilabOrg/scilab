// Installation d'une toolbox
// avril 2008 by Delphine

function result = installToolbox(nom, checkVersionScilab, version)
  global conflictingList
  global conflictLocal
  global nomconflictLocal
  global conflictVersion
  // On regarde si les arguments existent
  if argn(2) == 1
    checkVersionScilab = %t
  end
  if argn(2) <= 2
    version = ""
  end
  // On enlève les charactères spéciaux
  nom = atomsSubstituteString(nom)
  // On va dans le repertoire dédié
  rep = atomsToolboxDirectory()
  cd (rep)
  // Nécessité de vider les variables globales avant toute utilisation de atomsCheckConflict
  clearglobal conflictingList
  clearglobal conflictLocal
  clearglobal nomconflictLocal
  clearglobal conflictVersion
  // Récupération de la liste des toolboxes à installer
  listTool = atomsCheckConflict(nom, version, checkVersionScilab)
  // S'il existe une ligne vide, c'est qu'il manque une dépendance
  if find(listTool == "") <> []
    if conflictLocal == 1
      atomsDisplayMessage("La version de la Toolbox " + nomconflictLocal + " presente localement n''est pas la bonne. Updatez la.")
    else
      atomsDisplayMessage("Impossible d''installer toutes les dependances")
    end
    result =  %f
    return result
  elseif conflictVersion == 1
    atomsDisplayMessage("Conflit de version pour " + nomconflictLocal)
    result =  %f
    return result
  end
  // Récupération de la liste des toolboxes qui sont déjà en local
  cd (rep)
  listLocal = ls()
  // Traitement des toolboxes à installer
  [m, n] = size(listTool)
  for i=1:m
    [nom, version] = atomsSeparateVersionDep(listTool(i))
    // Si elle est présente en local on passe à la suivante
    if find(listLocal == nom) <> []
      continue
    else
      atomsDisplayMessage("Va etre installe : " + nom)
      if ~atomsDlInstall(nom, version)
        disp("Erreur lors de l''installation")
        result = %f
        return result
      end
    end
  end
  result = %t
  return result
endfunction
