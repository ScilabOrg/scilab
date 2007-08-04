//Input editor function of Scicos code generator
//
//@l@n, 31/07/07
//
function CodeGeneration_()

  //**----------- Old Working code ----------------------------------
  Cmenu='Open/Set'
  xinfo('Click on a Superblock (without activation output)'+..
        ' to obtain a coded block ! ')
    k=[]
    while %t
      if %pt==[] then
        [btn,%pt,win,Cmenu]=cosclick()
        if Cmenu<>[] then
          [%win,Cmenu]=resume(win,Cmenu)
        end
      else
        win=%win
      end
      xc=%pt(1);yc=%pt(2);%pt=[]
      k=getobj(scs_m,[xc;yc])
      if k<>[] then break,end
    end

    if scs_m.objs(k).model.sim(1)=='super' then
      disablemenus()
      all_scs_m=scs_m;

      XX=scs_m.objs(k);

      [ok,XX,alreadyran]=do_compile_superblock42(XX,all_scs_m,k,alreadyran)
      enablemenus()

      if ok then 
        scs_m.objs(k)=XX
        edited=%t;
        needcompile=4
        Cmenu='Replot';
      else
        Cmenu='Open/Set'
      end
    else
      message('Generation Code only work for a Superblock ! ')
    end
endfunction
