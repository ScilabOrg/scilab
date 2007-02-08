function Cut_()
//** INRIA
//**  
  if Select==[] then Cmenu=[]; %pt=[]; %ppt=[];return,end
  
  gh_curwin = gh_current_window ; //** acquire the current window handler
  
  if Select(1,2)==curwin then
    scs_m_save = scs_m 
    nc_save = needcompile
    del = setdiff(1:size(scs_m.objs),Select(:,1))
    
    scs_m_sel = do_purge( do_delete1(scs_m,del,%f) ); 
    if size(scs_m_sel.objs)==1 then
       	Clipboard=scs_m_sel.objs(1)   
    else
    	Clipboard=scs_m_sel
    end
 
    [scs_m,DEL] = do_delete1(scs_m,Select(:,1)',%t)
 
    Select(find(Select(:,2)==curwin),:)=[];
    
    if DEL<>[] then 
      needcompile = 4,
      edited = %t
      enable_undo = %t
      //suppress right-most deleted elements
      while getfield(1,scs_m.objs($))=='Deleted' then
	  scs_m.objs($) = null(); //** erase the 'Deleted' elements from scs_m.objs
          gh_object_to_delete = gh_curwin.children.children(1); //** the top element
          delete(gh_object_to_delete) ; //** delete the elements from the graphics datastructure 
                                    //** in order to mantain the coherency 

	if lstsize(scs_m.objs)==0 then break,end
      end
    
    end
    Cmenu=[];
  else
    message(['Only current diagram can be edited'])
    Cmenu=[]; %pt=[]; %ppt=[]
  end
  
endfunction


