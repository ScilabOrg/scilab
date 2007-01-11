function [scs_m,DEL] = do_delete2(scs_m,K,gr)
//perform deletion of scs_m object whose index are given in the vector 
//K and all connected links. splits which are not given in K are not deleted
//!
// Copyright INRIA

//** N.B. [reg,DEL] = do_delete2(scs_m,del,%f) from 'get_region2'
//** first pass  : gr = FALSE
//** second pass : gr = TRUE 
//** gr = TRUE generate some unuseful drawobj() ... the situation is restored by
//** the final "Replot" operation 


DEL=[] //table of deleted objects

K0 = K ; //** save the K 

while K<>[] do
  k=K(1); K(1)=[]
  o = scs_m.objs(k);
  //** filter 'deleted' object already present in scs_m
  if find(DEL==k)==[] then 
    typ = typeof(o)
  else
    typ = 'Deleted'
  end
   
  DEL = [DEL k]

  if typ=='Link' then
    //**--------------------------------- LINK ------------------------------------
    [ct,from,to]=(o.ct,o.from,o.to)
    tokill=[%t,%t]
    if ct(2)<>2 then
      //disconnect to block
      scs_m.objs(to(1))=mark_prt(scs_m.objs(to(1)),to(2),'in',ct(2),0)
      //disconnect from block
      scs_m.objs(from(1))=mark_prt(scs_m.objs(from(1)),from(2),'out',ct(2),0)
    else
      if or(scs_m.objs(to(1)).graphics.pin==k) then
	scs_m.objs(to(1))=mark_prt(scs_m.objs(to(1)),to(2),'in',ct(2),0)
      else
	scs_m.objs(to(1))=mark_prt(scs_m.objs(to(1)),to(2),'out',ct(2),0)
      end
       if or(scs_m.objs(from(1)).graphics.pin==k) then
	 scs_m.objs(from(1))=mark_prt(scs_m.objs(from(1)),from(2),'in',ct(2),0)
       else
	 scs_m.objs(from(1))=mark_prt(scs_m.objs(from(1)),from(2),'out',ct(2),0)
       end
    end
    
    // erase and delete link
    if gr==%t then drawobj(o),end
  
  //**-------------------------------- BLOCK ----------------------------------------  
  elseif typ=='Block' then
    // get connected links
    connected=get_connected(scs_m,k)
    //ask for connected links deletion
    K=[K connected]
    // erase and delete block
    if gr==%t then drawobj(scs_m.objs(k)),end
  
  //**-------------------------------- TEXT ------------------------------------------  
  elseif typ=='Text' then
    if gr==%t then drawobj(o),end
  //**-------------------------------- DELETED ---------------------------------------
  elseif typ=='Deleted' then
  //**-------------------------------- U.F.O. ----------------------------------------
  else
    message('This object can''t be deleted')
  end
end

//if gr==%t then 
//  if pixmap then xset('wshow'),end,
//end

for k=DEL
   scs_m.objs(k) = mlist('Deleted')
end

endfunction
