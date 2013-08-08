function tbx_build_pal_loader(script_path,palettename,interfacefunctions)
    t=["function loaderpal()"
    "  xpal = xcosPal("""+palettename+""");"
    "  interfacefunctions ="+sci2exp(interfacefunctions);
    "  script_path = get_absolute_file_path(""loader_pal.sce"");"
    "  module_path = part(script_path,1:length(script_path)-7);"
    "  images_path = module_path+""images/"";"
    "  for i=1:size(interfacefunctions,""*"")"
    "    h5_instances  = ls(images_path + ""h5/""  + interfacefunctions(i) + "".sod"");"
    "    if h5_instances==[] then"
    "      error(msprintf(_(""%s: block %s has not been built.\n""),""loader_pal.sce"",interfacefunctions(i)))"
    "    end"
    "    pal_icons     = ls(images_path + ""gif/"" + interfacefunctions(i) + ""."" + [""png"" ""jpg"" ""gif""]);"
    "    if pal_icons==[] then"
    "      error(msprintf(_(""%s: block %s has no palette icon.\n""),""loader_pal.sce"",interfacefunctions(i)))"
    "    end"
    "    graph_icons   = ls(images_path + ""svg/"" + interfacefunctions(i) + ""."" + [""svg"" ""png"" ""jpg"" ""gif""]);"
    "    if graph_icons==[] then"
    "      error(msprintf(_(""%s: block %s has no editor icon.\n""),""loader_pal.sce"",interfacefunctions(i)))"
    "    end"
    "    xpal = xcosPalAddBlock(xpal, h5_instances(1), pal_icons , graph_icons(1));"
    "  end"
    "  xcosPalAdd(xpal);"
    "endfunction"
    "loaderpal(),clear loaderpal"]
    mputl(t,script_path+"loader_pal.sce")
endfunction
