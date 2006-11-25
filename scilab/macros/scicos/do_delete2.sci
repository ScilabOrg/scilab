function [scs_m,DEL]=do_delete2(scs_m,K,gr)
//perform deletion of scs_m object whose index are given in the vector 
//K and all connected links. splits which are not given in K are not deleted
//!
// Copyright INRIA
DEL=[] //table of deleted objects
K0=K
while K<>[] do
  k=K(1);K(1)=[]
  o=scs_m.objs(k);
  if find(DEL==k)==[] then typ=typeof(o);else typ='Deleted',end
   
  DEL=[DEL k]

  if typ=='Link' then
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
  elseif typ=='Block' then
    // get connected links
    connected=get_connected(scs_m,k)
    //ask for connected links deletion
    K=[K connected]
    // erase and delete block
    if gr==%t then drawobj(scs_m.objs(k)),end
  elseif typ=='Text' then
    if gr==%t then drawobj(o),end
  elseif typ=='Deleted' then
  else
    message('This object can''t be deleted')
  end
end
//if gr==%t then 
//  if pixmap then xset('wshow'),end,
//end
for k=DEL,scs_m.objs(k)=mlist('Deleted'),end
endfunction
