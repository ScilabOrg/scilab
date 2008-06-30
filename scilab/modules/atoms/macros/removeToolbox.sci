// suppression d'une toolbox
// avril 2008 by Delphine

function result = removeToolbox(nom)
  rep = atomsToolboxDirectory()
  // Si nom = "all" on supprime toutes les Toolboxes
  if nom == "all"  
    rmdir(rep, "s")
    mkdir(rep)
  else
    // On vérifie que la toolbox existe
    d = rep + nom
    if ~isdir(d)
      atomsDisplayMessage("La toolbox " + nom + " n''est pas installee")
      result = %f
      return result
    end
    cd (rep)
    // On vérifie que la toolbox n'est pas une dépendance d'une autre
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
          if find(depend == nom) <> []
            atomsDisplayMessage("La Toolbox " + nom + " est une dependance de " + listLocal(i))
            reponse = input("Continuer? [""y""/""n""]")
            if (reponse <> "y" & reponse <> "Y")
              result = %f
              return result
            end
          end
        end
      end
    end
    rmdir(rep + nom, "s")
    atomsDisplayMessage(nom + " supprime")
    result = %t
    return result
  end
endfunction
