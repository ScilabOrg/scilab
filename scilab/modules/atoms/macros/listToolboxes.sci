// Affichage des toolboxes installées en local
// juin 2008 by Delphine

function listToolboxes()
  rep = toolboxDirectory()
  cd(rep)
  listLocal = ls()
  [n, m] = size(listLocal)
  displayMessage("Les toolboxes installees localement sont :")
  for i=1:n
    if listLocal(i) <> ".svn"
      desc = readDescription(listLocal(i))
      displayMessage(desc("Toolbox") + " - version " + desc("Version"))
    end
  end
endfunction
