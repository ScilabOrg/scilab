function scs_m=delete_unconnected(scs_m);
// Copyright INRIA
//**
// delete unconnected blocks and all relevant parts of a diagram
// may be used before compilation
//**

n = lstsize(scs_m.objs); 

if n==0 then return, end ; //** exit point 
DEL=[];
DELL=[]
finish=%f

while ~finish
  finish = %t
  for k=1:n  //loop on scs_m objects
    x = getfield(1,scs_m.objs(k))
    if x(1)=='Block' then
      if scs_m.objs(k).gui<>'SUM_f'&scs_m.objs(k).gui<>'SOM_f' then
	
	if find(scs_m.objs(k).gui==['IFTHEL_f','ESELECT_f']) then
	  kk=[find(scs_m.objs(k).graphics.pein==0),find(scs_m.objs(k).graphics.pin==0)]
	  if kk<> [] 	// a synchro block is not active, remove it
	    [scs_m,DEL1,DELL1]=do_delete1(scs_m,k,%f)
	    DEL=[DEL DEL1]
	    DELL=[DELL DELL1]
	    finish=%f
	  end
	
	else
	  kk=[find(scs_m.objs(k).graphics.pin==0)]
	  if kk<>[] then // at least one  input port is not connected delete the block

	    if or(getfield(1,scs_m.objs(k).graphics)=="in_implicit") then
	      if or(scs_m.objs(k).graphics.in_implicit(kk)<>"I") then 
		[scs_m,DEL1,DELL1]=do_delete1(scs_m,k,%f)
		DEL=[DEL DEL1]
		DELL=[DELL DELL1]
		finish=%f
	      end

	    else
	      [scs_m,DEL1,DELL1]=do_delete1(scs_m,k,%f)
	      DEL=[DEL DEL1]
	      DELL=[DELL DELL1]
	      finish=%f
	    end
	  
	  end
	end
      end
    end
  end 
end

//suppress rigth-most deleted elements
while getfield(1,scs_m.objs($))=='Deleted' then
  scs_m.objs($)=null();
  if lstsize(scs_m.objs)==0 then break,end
end

// Notify by hiliting and message edition

if DEL<>[] then 

  gh_wins = gcf();

  if path<>[] then
    mxwin = maxi(winsid()) ;

    for k=1:size(path,'*')
      hilite_obj(path(k))
      scs_m_s = scs_m_s.objs(path(k)).model.rpar;
      //scs_show must be modified for new editor
      scs_show(scs_m_s, mxwin+k)
      //DEL=[]; //only here to disable scicos crash.
              //Must be removed later
              //when scs_show is alright for new graphic
    end

  end

  //for k=DEL
  //  if find(k==DELL)==[] then
  k=DEL
  ind_k=find(k<>DELL)
  if ind_k<>[] then
    k=k(ind_k)
    hilite_obj(k) //** New Graphics
  //end
  //  end
  //end

    message(['Hilited block(s) or link(s) are ignored because of'
             'undefined input(s)'])
  //for k = DEL
  //  if find(k==DELL)==[] then
    unhilite_obj(k) //** New Graphics
  //  end
  end

  for k=size(path,'*'):-1:1
    //xdel(mxwin+k) //TOBEDONE
                    //Must be removed later
                    //when scs_show is alright for new graphic
    clf(mxwin+k)
  end

  scs_m_s = null()

  scf(gh_wins);

  if path<>[] then
    unhilite_obj(path(1))
  end
end

endfunction
