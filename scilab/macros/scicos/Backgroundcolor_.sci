function Backgroundcolor_()
    Cmenu=[]
    if curwin==%win then
      xset("window",%win)
    else
      message("Only current window can be edited.")
    end
    [edited,options]=do_options(scs_m.props.options,'Background')
    scs_m.props.options=options
    
    if edited then
      scs_m.props.options=options
      set_background()
      Cmenu='Replot'
    end
endfunction
