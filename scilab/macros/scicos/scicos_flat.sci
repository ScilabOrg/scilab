function  [cor,corinv,links_table,cur_fictitious,sco_mat,ok]=scicos_flat(scs_m,ksup,sco_mat)
//This function takes a hierarchical Scicos diagram and computes the
//"flat" equivalent, removing "non computational" blocs like splits.
//S. Steer, R. Nikoukhah 2003. Copyright INRIA

//NB::--------------------------------------------------------------
//Modificating this function to take care of the GOTO and FROM blocks
//A negatif number in cor and in links_table is refer to a GOTO,FROM 
//or GotoTagVisibility block.
//Modification of shiftcors. It will not shift the negatives numbers.
// A sco_mat is a string matrix composed by the :
//	- the first column : the negatif of the value of a GOTO or FROM in the cor.
//	- the second column: 1 if it is a GOTO; -1 if it is a FROM.
//	- the third column : the tag value
//	- the forth column : the tag visibility value in GOTO; 1 for the FROM block
//	- the fifth column : 1=regular 2=event 3=modelica
// The local and scoped cases are studied in this function. 
// The global case is studied in the function global_case in c_pass1.
// A Modification of update_cor in c_pass1. For the negatives numbers 
// the cor will be set to 0. (In this case the blocks are considered as IN_f ...)
// Fady NASSIF 05/07/2007
//-------------------------------------------------------------------

  if argn(2)<=1 then ksup=0;end //used for recursion
  if ksup==0 then   // main scheme
    MaxBlock=countblocks(scs_m);
    //last created fictitious block (clock split,clock sum,super_blocks, superbloc))
    cur_fictitious=MaxBlock
    path=[];       // for delete_unconnected 
    scs_m_s=scs_m ;// for delete_unconnected 
  end
  //-------------- suppress blocks with an unconnected regular port -------------- 
  scs_m=delete_unconnected(scs_m);
  
  //list of blocks with are not retained in the final block list
  blocks_to_remove=['CLKSPLIT_f' 'SPLIT_f' 'IMPSPLIT_f' 'CLKSOM_f' 'CLKSOMV_f' 'NRMSOM_f']
  port_blocks=['IN_f','INIMPL_f','OUT_f','OUTIMPL_f','CLKIN_f','CLKINV_f','CLKOUT_f','CLKOUTV_f']
  block_goto=['GOTO','CLKGOTO','GOTOMO']
  block_from=['FROM','CLKFROM','FROMMO']
  block_tag=['GotoTagVisibility','CLKGotoTagVisibility','GotoTagVisibilityMO']
  n=lstsize(scs_m.objs) //number of "objects" in the data structure
  //-------------- initialize outputs --------------
  nb=0;
  links_table=[]; // 
  corinv=list();
  cor=list();for k=1:n, cor(k)=0;end

  ok=%t;
  Links=[] //to memorize links position in the data structure
  
  //-------------- Analyse blocks --------------
loc_mat=[];from_mat=[];tag_exprs=[];sco_mat=[];
  for k=1:n //loop on all objects
    o=scs_m.objs(k);
    x=getfield(1,o);
    cor(k)=0
    if x(1)=='Block' then
      if or(o.gui==block_goto) then
        //if the goto is local we put it in the loc_from matrix
	//the loc_mat is composed of:
	//1st column : the number given to the block.
	//2nd column : 1 because it is a GOTO
	//3rd column : the tag of the GOTO.
	//4th column : A flag identifying the type of the block (regular or event)
	// if the GOTO is scoped or global we put it in a sco_mat matrix
	//the sco_mat is composed of:
	//1st column : the number given to the block.
	//2nd column : 1 if it is a GOTO, -1 if it is FROM
	//3rd column : the tag of the GOTO.
	//4th column : A flag identifying the type of the block (1=regular, 2=event 3=modelica)
        cur_fictitious=cur_fictitious+1;
	cor(k)=-cur_fictitious;
	if (o.graphics.exprs(2)=='1') then
	 loc_mat=[loc_mat;[string(cur_fictitious),string(1),(o.graphics.exprs(1)),string(find(block_goto(:)==o.gui))]]
	 locomat=[];
         for i=1:size(loc_mat,1)
         locomat=[locomat;strcat([loc_mat(i,3) loc_mat(i,4)])]
	 end
	 vec=unique(locomat)
	 if size(vec,1)<>size(loc_mat,1) then
            if flgcdgen<>-1 then path=[numk path];scs_m=all_scs_m; end
           if (ksup==0)|flgcdgen<>-1  then
	     hilite_path([path,k],"There is another local GOTO in this diagram with the same tag ''"+loc_mat($,3)+"''",%t);
           else
             gh_wins = gcf();
             mxwin=maxi(winsid());
             scs_show(scs_m,mxwin+1);
             hilite_obj(k);
             message("There is another local GOTO in this diagram with the same tag ''"+loc_mat($,3)+"''");
             gh_del = scf(mxwin+1);
             unhilite_obj(k);
	     delete(gh_del);
             scf(gh_wins);
           end
	    ok=%f;return
	 end
       	else
	 sco_mat=[sco_mat;[string(cur_fictitious),string(1),o.graphics.exprs(1),o.graphics.exprs(2),string(find(block_goto(:)==o.gui))]]
	end
      elseif or(o.gui==block_from) then
	cur_fictitious=cur_fictitious+1;
	cor(k)=-cur_fictitious
	sco_mat=[sco_mat;[string(cur_fictitious),string(-1),o.graphics.exprs(1),string(1),string(find(block_from(:)==o.gui))]]
 	from_mat=[from_mat;[string(cur_fictitious),string(-1),o.graphics.exprs(1),string(find(block_from(:)==o.gui))]]
      elseif or(o.gui==block_tag) then
	tag_exprs=[tag_exprs;[o.graphics.exprs(1),string(find(block_tag(:)==o.gui))]]
	cur_fictitious=cur_fictitious+1;
	cor(k)=-cur_fictitious
      elseif o.gui=='SampleCLK' then
        cur_fictitious=cur_fictitious+1;
        cor(k)=-cur_fictitious
        sco_mat=[sco_mat;[string(cur_fictitious),string(1),string(o.model.rpar(1)),..
                  	   string(o.model.rpar(2)),string(4)]]
      elseif or(o.gui==blocks_to_remove) then
	cur_fictitious=cur_fictitious+1;
	cor(k)=cur_fictitious
      elseif o.gui=='SUM_f'|o.gui=='SOM_f' then
	nb=nb+1;
	corinv(nb)=k;
	cor(k)=nb
	//scs_m=adjust_sum(scs_m,k)
      elseif or(o.gui==port_blocks) then
	//here we suppose to be inside a superblock
	//may be we can handle this blocks just as blocks_to_remove
	if ksup==0 then 
	  scs_m=scs_m_s
	  hilite_path([path,k],'Port blocks must be only used in a Super Block',%f)
	  ok=%f;return
	end
	connected=get_connected(scs_m,k)
	if connected==[] then
	  scs_m=scs_m_s
	  hilite_path([path,k],'This Super Block Input port is unconnected',%t)
	  ok=%f;return
	end
	if or(o.gui==['IN_f','INIMPL_f']) then
	  pind=Pind(1)
	elseif or(o.gui==['OUT_f','OUTIMPL_f']) then
	  pind=Pind(2)
	elseif or(o.gui==['CLKIN_f','CLKINV_f']) then
	  pind=Pind(3)
	elseif or(o.gui==['CLKOUT_f','CLKOUTV_f']) then
	  pind=Pind(4)
	end 
	//connect the link to the fictitious bloc replacing the superblock
	if scs_m.objs(connected).from(1)==k then
	  scs_m.objs(connected).from(1)=-(pind+o.model.ipar)
	end
	if scs_m.objs(connected).to(1)==k then
	  scs_m.objs(connected).to(1)=-(pind+o.model.ipar)
	end

      elseif o.model.sim=='super'|o.model.sim=='csuper' then
	path=[path k] //superbloc path in the hierarchy
	//replace superbloc by a set of fictitious blocks (one per port)
        //and reconnect links connected to the superblock to these
        //ficitious blocks
	Pinds=[];if exists('Pind') then Pinds=Pind,end
	Pind=[] //base of ports numbering
	//mprintf("entering superblock at level '+string(size(path,'*'))+"\r\n")
	for port_type=['pin','pout','pein','peout']
	  Pind=[Pind cur_fictitious]
	  ip=scs_m.objs(k).graphics(port_type);ki=find(ip>0)
	  for kk=ki
	    kc=ip(kk)
	    if scs_m.objs(kc).to(1)==k then  // a link going to the superblock
	      scs_m.objs(kc).to(1)=-(cur_fictitious+scs_m.objs(kc).to(2));
	      scs_m.objs(kc).to(2)=1
	    end
	    if scs_m.objs(kc).from(1)==k then  // a link coming from the superblock
	      scs_m.objs(kc).from(1)=-(cur_fictitious+scs_m.objs(kc).from(2));
	      scs_m.objs(kc).from(2)=1
	    end
	  end
	  cur_fictitious=cur_fictitious+size(ip,'*')
	end


	//Analyze the superblock contents
        [cors,corinvs,lt,cur_fictitious,scop_mat,ok]=scicos_flat(o.model.rpar,cur_fictitious)
	if ~ok then return,end
	sco_mat=[sco_mat;scop_mat]
	nbs=size(corinvs) 
	
	//catenate superbloc data with current data

	f=find(lt(:,1)>0&lt(:,1)<=nbs);if f<>[] then lt(f,1)=lt(f,1)+nb,end
	links_table=[links_table;lt]
	
	for kk=1:nbs, corinv(nb+kk)=[k,corinvs(kk)];end
	cors=shiftcors(cors,nb)
//	cur_fictitious=cur_fictitious+nb
	cor(k)=cors
	nb=nb+nbs
	Pind=Pinds
	path($)=[]

      else//standard blocks
	nb=nb+1
	corinv(nb)=k
	//[model,ok]=build_block(o.model)
	cor(k)=nb
      end
    elseif x(1)=='Deleted'|x(1)=='Text' then
	//this objects are ignored
    else //links
      Links=[Links k] // memorize their position for use during links analysis
    end
  end //end of loop on objects
  
  if ksup==0&nb==0 then
    message('Empty diagram')
    ok=%f
    return
  end

  //-------------- Analyse  links -------------- 
  for k=Links
    o=scs_m.objs(k);
    f=0
    if o.from(1)<0|o.from(1)>MaxBlock then //Link coming from a superblock input port
    else
      o.from(1)=cor(o.from(1));
    end
    if o.to(1)<0 |o.to(1)>MaxBlock then //Link going to a superblock output port
    else
      o.to(1)=cor(o.to(1)),
    end

    if o.ct(2)==2 //implicit links
      //if abs(o.from(1))==125|abs(o.to(1))==125 then pause,end
      links_table=[links_table
		   o.from(1:3)    o.ct(2) 
		   o.to(1:3)      o.ct(2) ]
    else //regular or event links
      links_table=[links_table
		   o.from(1:2)  -1  o.ct(2) //outputs are tagged with -1 
		   o.to(1:2)    1   o.ct(2) ] //inputs are tagged with 1
    end
  end
//----------------------Goto From Analyses--------------------------
// Local case
  if loc_mat<>[] then
     for i=1:size(loc_mat,1)
	index1=find((from_mat(:,2)=='-1')&(from_mat(:,3)==loc_mat(i,3))&(from_mat(:,4)==loc_mat(i,4)))
	for j=index1
	     index2=find(links_table(:,1)==-evstr(from_mat(j,1)))
// 	     for k=index2
// 		  links_table(k,1)=-evstr(loc_mat(i,1))
// 	     end
	     if index2<>[] then
	         links_table(index2',1)=-evstr(loc_mat(i,1))
	     end
	     index2=find(sco_mat(:,1)==from_mat(j,1))
	     sco_mat(index2',:)=[]
	end
      end
   end
//scoped case
   if tag_exprs<>[] then
   //to be modified !!!!!
//      vec=unique(tag_exprs)
      for i=1:size(tag_exprs,1)
	  index=find((tag_exprs(:,1)==tag_exprs(i,1))&(tag_exprs(:,2)==tag_exprs(i,2)))
          if size(index,'*') > 1  then
	      message(["Error In Compilation. You cannot have multiple GotoTagVisibility";..
		       " with the same tag value in the same scs_m"])
	      ok=%f;
	      return
          end
      end 

      for i=1:size(tag_exprs,1)
	   index=find((sco_mat(:,2)=='1')&(sco_mat(:,3)==tag_exprs(i,1))&(sco_mat(:,4)=='2')&(sco_mat(:,5)==tag_exprs(i,2)))
	   if index<>[] then
	   if size(index,'*')>1 then
	      message(["Error in compilation";"Multiple GOTO are taged by the same GotoTagVisibility"])
	      ok=%f
	      return
	   end
	   index1=find((sco_mat(:,2)=='-1')&(sco_mat(:,3)==tag_exprs(i,1))&(sco_mat(:,5)==tag_exprs(i,2)))
	   if index1<>[] then
	   for j=index1
		index2=find(links_table(:,1)==-evstr(sco_mat(j,1)))
		if index2<>[] then
		   links_table(index2',1)=-evstr(sco_mat(index,1))
		end
	   end
	   end
	       sco_mat([index1';index'],:)=[]
	       //sco_mat(index,:)=[]
	   end
      end
   end
//global case
// function global_case in c_pass1
//------------------------------------------------------------------------
endfunction