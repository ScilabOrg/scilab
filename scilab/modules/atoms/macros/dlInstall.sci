// DL et install
// avril 2008 by Delphine

function result = dlInstall(nom, version)
  rep = toolboxDirectory()
  arch = nom + "." + version + ".tar.gz"
  // Téléchargement de l'archive
  cd (rep)
  listMirror = toolboxMirror()
  if dlArchive(listMirror, 1, arch)
    // Dezippage
    if dezipp(arch)
      // Suppression de l'archive
      if removeFile(arch)
        result = %t
        return result
      else
        result = %f
        return result
      end
    else
      result = %f
      return result
    end
  else
    result = %f
    return result  
  end
endfunction

// Récupération d'une archive sur le web
function result = dlArchive(listWeb, position, name)
  temp = unix_g("wget " + listWeb(position) + "/" + name + " 2>&1 > /dev/null")
  // Si le fichier n'est pas present
  if temp == ""
    [n, m] = size(listWeb)
    // Si on dépasse la taille de la liste
    if (position + 1) > m
      displayMessage("Verifiez la validite du repository")
      result = %f
      return result
    else
      result = dlArchive(listWeb, position+1, name)
    end
  else 
    result = %t
  end
  return result  
endfunction

// Dezippage
function result = dezipp(fileD)
  if getos() == "Windows"
    temp = unix_g("zcat " + fileD + " | tar -xv")
  else // linux et mac
    temp = unix_g("tar -zxvf " + fileD)
  end
  if temp == ""
    result = %f
    return result
  else
    result = %t
    return result
  end
endfunction

// Fonction de suppression d'un fichier
function result = removeFile(fileR)
  result = deletefile(fileR)
  return result
endfunction
