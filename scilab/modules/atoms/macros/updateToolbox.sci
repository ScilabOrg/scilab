// update d'une toolbox
// avril 2008 by Delphine

function result = updateToolbox(nom, checkVersionScilab)
  global checkVersionScilab
  // On regarde si le 2eme argument existe
  if argn(2) == 1
    checkVersionScilab = %t
  end
  // On enlève les charactères spéciaux
  nom = atomsSubstituteString(nom)
  // Si nom = "all" on update toutes les Toolboxes
  if nom == "all"
    listLocal = ls()
    [n, m] = size(listLocal)
    for k=1:n
      if listLocal(i) <> ".svn"
        if ~updateToolbox(listLocal(k))
          break
        end
      end
    end
  else
    // On vérifie que la Toolboxe existe bien en local
    rep = atomsToolboxDirectory()
    d = rep + nom
    if ~isdir(d)
      atomsDisplayMessage("La toolbox " + nom + " n''est pas installee")
      result = %f
      return result
    end
    // On récupère la version de l'actuelle
    desc = atomsReadDesc(nom)
    versionActuelle = atomsExtractValue("Version", desc, 1)
    versionActuelle = atomsDecoupVersion(versionActuelle)
    // On recupère la liste de toutes les autres toolboxes disponibles
    listDesc = atomsReadDesc("")
    // On prend la version la plus récente
    position = atomsSelectPosition(listDesc, nom, "")
    // Si la toolbox n'est pas disponible en ligne
    if position == 0
      atomsDisplayMessage("Toolbox non disponible sur le net")
      result = %f
      return result
    end
    versionNew = atomsExtractValue("Version", listDesc, position)
    versionNew = atomsDecoupVersion(versionNew)
    // On regarde si c'est une mise à jour par rapport au local
    if atomsCompareVersion(versionNew, versionActuelle) == 1
      // On regarde s'il était une dépendance max pour d'autres toolboxes
      listLocal = ls()
      [n, m] = size(listLocal)
      for i=1:n
        if listLocal(i) <> ".svn"
          desc = atomsReadDesc(listLocal(i))
          depends = atomsExtractValue("Depends", desc, 1)
          depends = atomsSplitValue(depends, ",")
          [n, m] = size(depends)
          for j=1:n
            [depend, version] = atomsSeparateVersionDep(depends(j))
            if version <> ""
              [signe, version] = atomsSeparateSignVersion(version)
              if find(depend == nom) <> [] & signe == "<="
                version = atomsDecoupVersion(version)
                // On regarde si la nouvelle version valide cette dépendance max
                if atomsCompareVersion(versionNew, version) == 1
                  atomsDisplayMessage("probleme de dependance")
                  result = %f
                  return result
                end
              end
            end
          end
        end
      end
    end
    // On vérifie s'il y a besoin d'updater des dépendances
    dependsNew = atomsExtractValue("Depends", listDesc, position)
    dependsNew = atomsSplitValue(dependsNew, ",")
    [n, m] = size(dependsNew)
    for i=1:n
      [dependsNew, versionNew] = atomsSeparateVersionDep(dependsNew(i))
      listLocal = ls()
      if find(listLocal == dependsNew)
        if versionNew <> ""
          desc = atomsReadDesc(listLocal(i))
          v1 = atomsExtractValue("Version", desc, 1)
          v1 = atomsDecoupVersion(v1)
          [signeNew, versionNew] = atomsSeparateSignVersion(versionNew)
          versionNew = atomsDecoupVersion(versionNew)
          if ((versionNew == "<=" | versionNew == "=")  & atomsCompareVersion(v1, versionNew) == -1) | ((versionNew == ">=" | versionNew == "=") & atomsCompareVersion(v1, versionNew) == 1) 
            updateToolbox(dependsNew);
          end
        end
      end
    end
    // On le prévient de ce qu'on va faire
    versionNew = atomsExtractValue("Version", listDesc, position)
    atomsDisplayMessage("La toolbox " + nom + " va etre update en version " + versionNew)
    // On installe la nouvelle version
    rmdir(rep + nom, "s")
    installToolbox(nom);
  end
  result = %t
  return result
endfunction
