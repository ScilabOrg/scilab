function drawobjs(scs_m, gh_window)
// Copyright INRIA
//**

   drawlater() ;

   rhs = argn(2) ; //** get the number of right side arguments

   if rhs==1 then //** without arguments (default) assume ...
       //** It is NOT possible to modify [gh_current_window] directly outside [scicos_new]
       gh_curwin = gcf(); //** get the handle of the current graphics window
       drawgrid();
       for i=1 : lstsize(scs_m.objs) //** draw object by object
            gh_blk = drawobj(scs_m.objs(i));
       end
    else //** the argument is explicit
       //** It is NOT possible to modify [gh_current_window] directly outside [scicos_new]
       gh_curwin = gh_window ; //** get the handle of the current graphics window
       drawgrid(gh_curwin);
       for i=1 : lstsize(scs_m.objs) //** draw object by object
          gh_blk = drawobj(scs_m.objs(i), gh_curwin);
       end
   end

   //** Update Selection
   if (Select<>[]) & ...
           (find(Select(:,2)==gh_curwin.figure_id))<>[] then
     o_size = size (gh_curwin.children.children);
     for i=find(Select(:,2)==gh_curwin.figure_id)
       //gh_k = o_size(1) - Select(i,1) + 1 ; //** semi empirical equation :)
       gh_k=get_gri(Select(i,1),o_size(1))
       gh_curwin.children.children(gh_k).children(1).mark_mode="on"
       draw(gh_curwin.children.children(gh_k).children(1));
     end
   end

   gh_a=gca();
   drawtitle(scs_m.props)
   show_info(scs_m.props.doc)
   draw(gh_a);
   show_pixmap() ;
endfunction
