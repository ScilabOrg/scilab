// Affichage des toolboxes installées en local
// juin 2008 by Delphine

function listToolboxes()
  rep = atomsToolboxDirectory()
  cd(rep)
  listLocal = ls()
  [n, m] = size(listLocal)
  atomsDisplayMessage("Les toolboxes installees localement sont :")
  for i=1:n
    if listLocal(i) <> ".svn"
      desc = atomsReadDesc(listLocal(i))
      atomsDisplayMessage(desc("Toolbox") + " - version " + desc("Version"))
    end
  end
endfunction
