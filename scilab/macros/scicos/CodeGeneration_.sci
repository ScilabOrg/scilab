//Input editor function of Scicos code generator
//
//@l@n, 28/06/07
//
function CodeGeneration_()

  x_message (["This is the code generator of B4_scicos";
              "That''s the exprimental Rev. 28_06_07_18_03";
              "";
              "For the time being, please only report bugs.";
              "";
              "   Please mail to alan.layec@inria.fr"]);

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
      [ok,XX,alreadyran]=do_compile_superblock(XX,all_scs_m,k,alreadyran)
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
//produces the code for ddoit1, cdoit,edoit1, doit, odoit and ozdoit
//Copyright INRIA
function Code=c_make_doit1(cpr,stalone)
  funtyp=cpr.sim.funtyp;
  Code=[make_ddoit1()
        make_edoit1(stalone)
        make_odoit(stalone)
        make_cdoit(stalone)]

  if (find(funtyp < 0) <> []) then
    Code=[Code
          make_ozdoit(stalone)]
  end

  if (~stalone) then
    Code=[Code
          make_doit(stalone)
          make_zdoit(stalone)]
  end
endfunction

//procuces the code for ddoit2
//Copyright INRIA
//Author : Rachid Djenidi
function Code=c_make_doit2(cpr,stalone);
  //les pointeurs de cpr :
  z=cpr.state.z;
  funs=cpr.sim.funs;
  zptr=cpr.sim.zptr;
  inpptr=cpr.sim.inpptr;
  ordptr=cpr.sim.ordptr;
  ordclk=cpr.sim.ordclk;
  clkptr=cpr.sim.clkptr;
  evtspt=cpr.state.evtspt;

  Code=['';
        '/*'+part('-',ones(1,40))+' ddoit2 */';
        'int '
        cformatline(rdnom+'ddoit2(z, told, outtbptr, iwa)',70);
        '';
        '     double  *z, *told;';
        '     void **outtbptr;';
        '     integer *iwa;';
        '{';
        '  /* System generated locals */';
        '  integer  i1, i;';
        '';
        '  /* Local variables */';
        '  integer flag, keve, kiwa, nport;';
        '  double  *args[100];';
        '  double  rdouttb['+string(lstsize(outtb)+1)+'];';
        '  integer nevprt;';
        '';
        '  /* Function Body */';
        ''
        '  /*update continuous and discrete states on event */';
        '  kiwa = iwa['+string(clkptr($)-1)+'];';
        '  if (kiwa == 0) {';
        '    return 0 ;';
        '  }';
        '  i1 = kiwa;';
        '  for (i = 1; i <= i1; ++i) {';
        '    keve = iwa[i-1];';
        '    switch(keve) {'];

  maxkeve=size(ordptr,1)-1;
  for keve=1:maxkeve
    Code($+1)='    case '+string(keve)+':';
    for ii=ordptr(keve):ordptr(keve+1)-1
      //***********************************
      fun=ordclk(ii,1);
      //Actuator blocks have to be called even if they have no discrete
      //state
      is_act=or(fun==act)
      is_cap=or(fun==cap);
      if ~is_cap then    
        if ordclk(ii,2)>0 then
          if  is_act then
            if stalone then
              Code=[Code
                    '      flag = 2;';
                    '      nevprt='+string(ordclk(ii,2))+';';
                    '     '+wfunclist(fun)];
            end
          else
            Code=[Code
                  '    if (block_'+rdnom+'['+string(fun-1)+'].nx+block_'+rdnom+'['+string(fun-1)+'].nz > 0||'
                  '        *block_'+rdnom+'['+string(fun-1)+'].work !=NULL) {'
                  '      flag = 2;';
                  '      nevprt='+string(ordclk(ii,2))+';';
                  '     '+wfunclist(fun);
                  '     }'];
          end
        else
          if  is_act then
            if stalone then
              Code=[Code
                    '      flag = 2;';
                    '      nevprt=0;';
                    '     '+wfunclist(fun)];
            end
          else
            Code=[Code
                    '    if (*block_'+rdnom+'['+string(fun-1)+'].work !=NULL) {'
                    '      flag = 2;';
                    '      nevprt=0;';
                    '     '+wfunclist(fun);
                     '     }'];
          end
        end
      end
    end
    Code($+1)='    break;';
  end

  Code=[Code
        '    }'; //switch
        '  }';
        '  return 0;'
        '} /* ddoit2 */'];

endfunction

//Author : Rachid Djenidi
//Copyright INRIA
function Code=c_make_endi(cpr,stalone)
  //les pointeurs de cpr :
  z=cpr.state.z;
  funs=cpr.sim.funs;
  inpptr=cpr.sim.inpptr;

  Code=['/*'+part('-',ones(1,40))+' endi */';
        'int '+rdnom+'_endi(told, outtbptr, iwa)';
        '';
        '     double  *told;';
        '     void **outtbptr;';
        '     integer *iwa;';
        '{';
        '  /* Local variables */';
        '  integer flag;';
        '  double  rdouttb['+string(lstsize(outtb)+1)+'];';
        '  double  *args[100];';
        '  integer nport;';
        '  integer nevprt=0;';
        '';
        '  /* Function Body */';
        ''
        '  /*     ending  subroutine */';
        '  flag=5 ;'];

  for i=1:size(wfunclist)
    if or(i==act) | or(i==cap) then
      if stalone then
        Code=[Code;
              '';
              '  '+wfunclist(i)];
      end
    else
      Code=[Code;
            '';
            '  '+wfunclist(i)];
    end
  end

  Code=[Code;
        '  return 0;'
        '} /* ending */';
        ''];
endfunction

//Copyright INRIA
//Author : Rachid Djenidi
function Code=c_make_initi(cpr,stalone)

  //les pointeurs de cpr :
  z=cpr.state.z;
  funs=cpr.sim.funs;
  inpptr=cpr.sim.inpptr;

  Code=['/*'+part('-',ones(1,40))+' initi */';
        'int '+rdnom+'_initi(told,outtbptr, iwa)';
        '';
        '     /*Block initialization (flag=4)*/'
        '     double  *told;';
        '     void **outtbptr;'
        '     integer *iwa;';
        '{';
        '';
        '  /* Local variables */';
        '  integer flag;';
        '  double  rdouttb['+string(lstsize(outtb)+1)+'];';
        '  double  *args[100];';
        '  integer nport;';
        '  integer nevprt=0;';
        '';
        '  /* Function Body */';
        '';
        '  flag=4;'];

  for i=1:size(wfunclist)
    if or(i==act) | or(i==cap) then
      if stalone then
        Code=[Code
              '';
              '  '+wfunclist(i)];
      end
    else
       Code=[Code;
             '';
             '  '+wfunclist(i)];
    end
  end

  Code=[Code
        '  return 0;'
        '} /* '+rdnom+'_initi */'];
endfunction

//Copyright INRIA
//Author : Rachid Djenidi
function Code=c_make_outtb(stalone)

  //les pointeurs de cpr :
  z=cpr.state.z;
  funs=cpr.sim.funs;
  inpptr=cpr.sim.inpptr;
  outptr=cpr.sim.outptr;
  iord=cpr.sim.iord;

  niord=size(iord,1);

  Code=['/*'+part('-',ones(1,40))+' outtbini */';
        'int '
        rdnom+'_outtb(told, outtbptr, iwa)';
        '';
        '     /*Constants propagation*/'
        '     double  *told;';
        '     void **outtbptr;';
        '     integer *iwa;';
        '{';
        '';
        '  /* Local variables */';
        '  integer flag;';
        '  double  rdouttb['+string(lstsize(outtb)+1)+'];';
        '  double  *args[100];';
        '  integer nport;';
        '  integer nevprt=0;';
        '';
        '  /* Function Body */';
        '';
        '  flag=1 ;'];

  for i=iord(:,1)'
    //flag=1 for all blocks
    //if (outptr(i+1)-outptr(i))>0 then
      if or(i==act) then
        if stalone then
            Code=[Code
                  '';
                  '  '+wfunclist(i);]
        end
      else
          Code=[Code
                '';
                '  '+wfunclist(i);]
      end
    //end
  end

  Code=[Code
        '  return 0;'
        '} /* '+rdnom+'_outtb */'];
endfunction

//
//16/06/07 Author : ?, A. Layec
//
//Copyright INRIA
function [Code,actt,proto]=call_actuator(i)
  nin=inpptr(i+1)-inpptr(i);  //** number of input ports
  nout=outptr(i+1)-outptr(i); //** number of output ports

  if funtyp(i)==0 then
    if nin==0 then
      uk    = 0;
      nuk_1 = 0;
      nuk_2 = 0;
      uk_t  = 1;
      //Code($+1)=+'  args[0]=(double *)outtbptr[0]);';
    else
      uk    = inplnk(inpptr(i));
      nuk_1 = size(outtb(uk),1);
      nuk_2 = size(outtb(uk),2);
      uk_t  = mat2scs_c_nb(outtb(uk));
      //Code($+1)=' args[0]=('+mat2scs_c_ptr(outtb(uk))+' *)outtbptr['+string(uk-1)+'];';
    end
  end
  //pour la fonction gui ont a : num de bloc l'adresse dans
  //nouveau  z et la taille du port
  actt=[i uk nuk_1 nuk_2 uk_t bllst(i).ipar]

  Code($+1)='block_'+rdnom+'['+string(i-1)+'].nevprt=nevprt;'

  Code=['/* Call of actuator (blk nb '+string(i)+') */'
        Code;
        'nport = '+string(nbact)+';';
        rdnom+'_actuator(&flag, &nport, &block_'+rdnom+'['+string(i-1)+'].nevprt, told, '+..
        '('+mat2scs_c_ptr(outtb(uk))+' *)outtbptr['+string(uk-1)+'], &nrd_'+string(nuk_1)+', &nrd_'+..
        string(nuk_2)+', &nrd_'+string(uk_t)+',bbb);'];

  proto='void '+rdnom+'_actuator('+..
        'int *, int *, int *, double *, void *, int *, int *,int *,int);'
  proto=cformatline(proto,70);
endfunction

//CallBlock : generate C calling sequence
//            of a scicos block
//
//inputs : bk   : bloc index
//         pt   :
//         flag :block_'+rdnom+'[
//
//output : txt  :
//
//16/06/07 Authors : R.Nikoukhah, A.Layec
//Copyright INRIA
function txt=call_block4(bk)
//   if flag==2 & ((zptr(bk+1)-zptr(bk))+(ozptr(bk+1)-ozptr(bk))==0 | pt<=0) then
//     return // block without discrete state or continuously activated
//            // If work allocated in the absence of z and oz, it does not work
//   end

  //**
  nx=xptr(bk+1)-xptr(bk);
  nz=zptr(bk+1)-zptr(bk);
  nrpar=rpptr(bk+1)-rpptr(bk);
  nipar=ipptr(bk+1)-ipptr(bk);
  nin=inpptr(bk+1)-inpptr(bk);  //* number of input ports */
  nout=outptr(bk+1)-outptr(bk); //* number of output ports */

  //**
  //l'adresse du pointeur de ipar
  if nipar<>0 then ipar=ipptr(bk), else ipar=1;end
  //l'adresse du pointeur de rpar
  if nrpar<>0 then rpar=rpptr(bk), else rpar=1; end
  //l'adresse du pointeur de z attention -1 pas sur
  if nz<>0 then z=zptr(bk)-1, else z=0;end
  //l'adresse du pointeur de x
  if nx<>0 then x=xptr(bk)-1, else x=0;end

  //**
  ftyp=funtyp(bk)
  if ftyp>2000 then ftyp=ftyp-2000,end
  if ftyp>1000 then ftyp=ftyp-1000,end

  //** check function type
  if ftyp < 0 then //** ifthenelse eselect blocks
      txt = [];
      return;
  else
    if (ftyp<>0 & ftyp<>1 & ftyp<>2 & ftyp<>3 & ftyp<>4) then
      disp("types other than 0,1,2,3 or 4 are not supported.")
      txt = [];
      return;
    end
  end

  //** add comment
  txt=[get_comment('call_blk',list(funs(bk),funtyp(bk),bk));]

  //** write nevprt activation
//   nclock=abs(pt);
  txt=[txt;
       'block_'+rdnom+'['+string(bk-1)+'].nevprt=nevprt;']

  select ftyp
    //** zero funtyp
    case 0 then
      //**** input/output addresses definition ****//
      if nin>1 then
        for k=1:nin
          uk=inplnk(inpptr(bk)-1+k);
          nuk=size(outtb(uk),'*');
          TYPE=mat2scs_c_ptr(outtb(uk));//scilab index start from 1
          txt=[txt;
               'rdouttb['+string(k-1)+']=('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(uk-1)+'];']
        end
        txt=[txt;
             'args[0]=&(rdouttb[0]);']
      elseif nin==0
        uk=0;
        nuk=0;
        txt=[txt;
             'args[0]=(SCSREAL_COP *)'+rdnom+'_block_outtbptr[0];']
      else
        uk=inplnk(inpptr(bk));
        nuk=size(outtb(uk),'*');
        TYPE=mat2scs_c_ptr(outtb(uk));//scilab index start from 1
        txt=[txt;
             'args[0]=('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(uk-1)+'];']
      end

      if nout>1 then
        for k=1:nout
          yk=outlnk(outptr(bk)-1+k);
          nyk=size(outtb(yk),'*');
          TYPE=mat2scs_c_ptr(outtb(yk));//scilab index start from 1
          txt=[txt;
               'rdouttb['+string(k+nin-1)+']=('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(yk-1)+'];'];
        end
        txt=[txt;
             'args[1]=&(rdouttb['+string(nin)+']);'];
      elseif nout==0
        yk=0;
        nyk=0;
        txt=[txt;
             'args[1]=(SCSREAL_COP *)'+rdnom+'_block_outtbptr[0];'];
      else
        yk=outlnk(outptr(bk));
        nyk=size(outtb(yk),'*'),;
        TYPE=mat2scs_c_ptr(outtb(yk));//scilab index start from 1
        txt=[txt;
             'args[1]=('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(yk-1)+'];'];
      end
      //*******************************************//

      //*********** call seq definition ***********//
      txtc=['(&flag,&block_'+rdnom+'['+string(bk-1)+'].nevprt,told,block_'+rdnom+'['+string(bk-1)+'].xd, \';
            'block_'+rdnom+'['+string(bk-1)+'].x,&block_'+rdnom+'['+string(bk-1)+'].nx, \';
            'block_'+rdnom+'['+string(bk-1)+'].z,&block_'+rdnom+'['+string(bk-1)+'].nz,block_'+rdnom+'['+string(bk-1)+'].evout, \';
            '&block_'+rdnom+'['+string(bk-1)+'].nevout,block_'+rdnom+'['+string(bk-1)+'].rpar,&block_'+rdnom+'['+string(bk-1)+'].nrpar, \';
            'block_'+rdnom+'['+string(bk-1)+'].ipar,&block_'+rdnom+'['+string(bk-1)+'].nipar, \';
            '(double *)args[0],&nrd_'+string(nuk)+',(double *)args[1],&nrd_'+string(nyk)+');'];
      if (funtyp(bk)>2000 & funtyp(bk)<3000)
        blank = get_blank(funs(bk)+'( ');
        txtc(1) = funs(bk)+txtc(1);
      elseif (funtyp(bk)<2000)
        txtc(1) = 'C2F('+funs(bk)+')'+txtc(1);
        blank = get_blank('C2F('+funs(bk)+') ');
      end
      txtc(2:$) = blank + txtc(2:$);
      txt = [txt;txtc];
      //*******************************************//


    //**
    case 1 then
      //**** input/output addresses definition ****//
//       if nin>=1 then
//         for k=1:nin
//           uk=inplnk(inpptr(i)-1+k);
//           nuk=size(outtb(uk),'*');
//         end
//       end
//       if nout>=1 then
//         for k=1:nout
//           yk=outlnk(outptr(i)-1+k);
//           nyk=size(outtb(yk),'*');
//         end
//       end
      //*******************************************//

      //*********** call seq definition ***********//
      txtc=['(&flag,&block_'+rdnom+'['+string(bk-1)+'].nevprt,told,block_'+rdnom+'['+string(bk-1)+'].xd, \';
            'block_'+rdnom+'['+string(bk-1)+'].x,&block_'+rdnom+'['+string(bk-1)+'].nx, \';
            'block_'+rdnom+'['+string(bk-1)+'].z,&block_'+rdnom+'['+string(bk-1)+'].nz,block_'+rdnom+'['+string(bk-1)+'].evout, \';
            '&block_'+rdnom+'['+string(bk-1)+'].nevout,block_'+rdnom+'['+string(bk-1)+'].rpar,&block_'+rdnom+'['+string(bk-1)+'].nrpar, \';
            'block_'+rdnom+'['+string(bk-1)+'].ipar,&block_'+rdnom+'['+string(bk-1)+'].nipar'];
      if (funtyp(bk)>2000 & funtyp(bk)<3000)
        blank = get_blank(funs(bk)+'( ');
        txtc(1) = funs(bk)+txtc(1);
      elseif (funtyp(bk)<2000)
        txtc(1) = 'C2F('+funs(bk)+')'+txtc(1);
        blank = get_blank('C2F('+funs(bk)+') ');
      end
      if nin>=1 | nout>=1 then
        txtc($)=txtc($)+', \'
        txtc=[txtc;'']
        if nin>=1 then
          for k=1:nin
            uk=inplnk(inpptr(bk)-1+k);
            nuk=size(outtb(uk),'*');
            txtc($)=txtc($)+'(SCSREAL_COP *)'+rdnom+'_block_outtbptr['+string(uk-1)+'],&nrd_'+string(nuk)+',';
          end
          txtc($)=part(txtc($),1:length(txtc($))-1); //remove last ,
        end
        if nout>=1 then
          if nin>=1 then
            txtc($)=txtc($)+', \'
            txtc=[txtc;'']
          end
          for k=1:nout
            yk=outlnk(outptr(bk)-1+k);
            nyk=size(outtb(yk),'*');
            txtc($)=txtc($)+'(SCSREAL_COP *)'+rdnom+'_block_outtbptr['+string(yk-1)+'],&nrd_'+string(nyk)+',';
          end
          txtc($)=part(txtc($),1:length(txtc($))-1); //remove last ,
        end
      end

      if ztyp(bk) then
        txtc($)=txtc($)+', \'
        txtc=[txtc;'w,&nrd_0);'];
      else
        txtc($)=txtc($)+');';
      end

      txtc(2:$) = blank + txtc(2:$);
      txt = [txt;txtc];
      //*******************************************//

    //**
    case 2 then

      //*********** call seq definition ***********//
      txtc=[funs(bk)+'(&flag,&block_'+rdnom+'['+string(bk-1)+'].nevprt,told,block_'+rdnom+'['+string(bk-1)+'].xd, \';
            'block_'+rdnom+'['+string(bk-1)+'].x,&block_'+rdnom+'['+string(bk-1)+'].nx, \';
            'block_'+rdnom+'['+string(bk-1)+'].z,&block_'+rdnom+'['+string(bk-1)+'].nz,block_'+rdnom+'['+string(bk-1)+'].evout, \';
            '&block_'+rdnom+'['+string(bk-1)+'].nevout,block_'+rdnom+'['+string(bk-1)+'].rpar,&block_'+rdnom+'['+string(bk-1)+'].nrpar, \';
            'block_'+rdnom+'['+string(bk-1)+'].ipar,&block_'+rdnom+'['+string(bk-1)+'].nipar, \';
            '(double **)block_'+rdnom+'['+string(bk-1)+'].inptr,block_'+rdnom+'['+string(bk-1)+'].insz,&block_'+rdnom+'['+string(bk-1)+'].nin, \';
            '(double **)block_'+rdnom+'['+string(bk-1)+'].outptr,block_'+rdnom+'['+string(bk-1)+'].outsz, &block_'+rdnom+'['+string(bk-1)+'].nout'];
      if ~ztyp(bk) then
        txtc($)=txtc($)+');';
      else
        txtc($)=txtc($)+', \';
        txtc=[txtc;
              'block_'+rdnom+'['+string(bk-1)+'].g,&block_'+rdnom+'['+string(bk-1)+'].ng);']
      end
      blank = get_blank(funs(bk)+'( ');
      txtc(2:$) = blank + txtc(2:$);
      txt = [txt;txtc];
      //*******************************************//

    //**
    case 4 then
      txt=[txt;
           funs(bk)+'(&block_'+rdnom+'['+string(bk-1)+'],flag);'];

  end
endfunction

//Copyright INRIA
//Author : Rachid Djenidi
function [Code,proto]=call_block(i)

  nx=xptr(i+1)-xptr(i);
  nz=zptr(i+1)-zptr(i);
  nrpar=rpptr(i+1)-rpptr(i);
  nipar=ipptr(i+1)-ipptr(i);
  nin=inpptr(i+1)-inpptr(i); ///* number of input ports */
  nout=outptr(i+1)-outptr(i); ///* number of output ports */
  fun=funs(i);

   //l'adresse du pointeur de ipar
  if nipar<>0 then ipar=ipptr(i);else ipar=1;end

  //l'adresse du pointeur de rpar
  if nrpar<>0 then rpar=rpptr(i);else rpar=1; end

  //l'adresse du pointeur de z attention -1 pas sur
  if nz<>0 then z=zptr(i)-1;else z=0;end

  //l'adresse du pointeur de x
  if nx<>0 then x=xptr(i)-1;else x=0;end
  //
  [Code,proto]=callf(i);
endfunction

//Copyright INRIA
//Author : Rachid Djenidi
function [Code,proto]=callf(i)
//Generates the code associated with the call to the simulation function 
//of one original block

  Code=[];proto=''
  ftyp=funtyp(i)
  if ftyp<0 then return,end
  if ftyp>2000 then ftyp=ftyp-2000,end
  if ftyp>1000 then ftyp=ftyp-1000,end

  //******************************************
  //Generate c code for input output arguments
  //******************************************
  Code($+1)=' block_'+rdnom+'['+string(i-1)+'].nevprt=nevprt;'
  if ftyp==0 then
    if nin>1 then
      for k=1:nin
	lprt=inplnk(inpptr(i)-1+k);
	uk=lnkptr(lprt);
	nuk=(lnkptr(lprt+1))-uk;
	Code($+1)='rdouttb['+string(k-1)+']=outtb['+string(uk-1)+'];';
      end
      Code($+1)='args[0]=&(rdouttb[0]);';
    elseif nin==0
      uk=0;
      nuk=0;
      Code($+1)=+'args[0]=&(outtb[0]);';
    else

      lprt=inplnk(inpptr(i));
      uk=lnkptr(lprt);
      nuk=(lnkptr(lprt+1))-uk;
      Code($+1)='args[0]=&(outtb['+string(uk-1)+']);';
    end
    if nout>1 then
      for k=1:nout
	lprt=outlnk(outptr(i)-1+k);
	yk=lnkptr(lprt);
	nyk=(lnkptr(lprt+1))-yk;
	Code($+1)='rdouttb['+string(k+nin-1)+']=outtb['+..
	    string(yk-1)+'];';
      end
      Code($+1)='args[1]=&(rdouttb['+string(nin)+']);';
    elseif nout==0
      yk=0;
      nyk=0;
      Code($+1)=+'args[1]=&(outtb[0]);';
    else
      lprt=outlnk(outptr(i));
      yk=lnkptr(lprt);
      nyk=(lnkptr(lprt+1))-yk;
      Code($+1)='args[1]=&(outtb['+string(yk-1)+']);';
    end
  elseif ftyp==1
    if nin>=1 then
      for k=1:nin
	lprt=inplnk(inpptr(i)-1+k);
	uk=lnkptr(lprt);
	nuk=(lnkptr(lprt+1))-uk;
      end
    end
    if nout>=1 then
      for k=1:nout
	lprt=outlnk(outptr(i)-1+k);
	yk=lnkptr(lprt);
	nyk=(lnkptr(lprt+1))-yk;
      end
    end
  end
  //************************
  //generate the call itself
  //************************
  if ftyp==0 | ftyp==1 then
    if (cpr.sim.funtyp(i)> 2000 & cpr.sim.funtyp(i)< 3000)
      CodeC=fun+'(&flag,&block_'+rdnom+'['+string(i-1)+'].nevprt,told,block_'+rdnom+'['+string(i-1)+'].xd,..
	   block_'+rdnom+'['+string(i-1)+'].x,&block_'+rdnom+'['+string(i-1)+'].nx,..
           block_'+rdnom+'['+string(i-1)+'].z,&block_'+rdnom+'['+string(i-1)+'].nz,block_'+rdnom+'['+string(i-1)+'].evout,..
           &block_'+rdnom+'['+string(i-1)+'].nevout,block_'+rdnom+'['+string(i-1)+'].rpar,&block_'+rdnom+'['+string(i-1)+'].nrpar,.. 
           block_'+rdnom+'['+string(i-1)+'].ipar,&block_'+rdnom+'['+string(i-1)+'].nipar';
    proto='void '+fun+"(int *, int *, double *,"+...
	  " double *, double *, int *, double *, int *, double *,"+...
	  " int *, double *, int *,int *,int *"
    
    elseif (cpr.sim.funtyp(i)< 2000)
    CodeC='C2F(' +fun+')(&flag,&block_'+rdnom+'['+string(i-1)+'].nevprt,told,block_'+rdnom+'['+string(i-1)+'].xd,..
           block_'+rdnom+'['+string(i-1)+'].x,&block_'+rdnom+'['+string(i-1)+'].nx,..
           block_'+rdnom+'['+string(i-1)+'].z,&block_'+rdnom+'['+string(i-1)+'].nz,block_'+rdnom+'['+string(i-1)+'].evout,..
           &block_'+rdnom+'['+string(i-1)+'].nevout,block_'+rdnom+'['+string(i-1)+'].rpar,&block_'+rdnom+'['+string(i-1)+'].nrpar,.. 
           block_'+rdnom+'['+string(i-1)+'].ipar,&block_'+rdnom+'['+string(i-1)+'].nipar';
    proto='void '+"C2F(" +string(fun)+")(int *, int *, double *,"+...
	  " double *, double *, int *, double *, int *, double *,"+...
	  " int *, double *, int *,int *,int *"
    end
    if ftyp==0 then
      CodeC=CodeC+',(double *)args[0],&nrd_'+string(nuk)+..
	    ',(double *)args[1],&nrd_'+string(nyk)+');';
       proto=proto+", double *, int *, double *, int *);"
    elseif ftyp==1 
      if nin>=1 then
	for k=1:nin
	  lprt=inplnk(inpptr(i)-1+k);
	  uk=lnkptr(lprt);
          nuk=(lnkptr(lprt+1))-uk;
	  CodeC=CodeC+',&(outtb['+string(uk-1)+']),&nrd_'+string(nuk);
	  proto=proto+", double *, int *"
	end
      end
      if nout>=1 then
	for k=1:nout
	  lprt=outlnk(outptr(i)-1+k);
	  yk=lnkptr(lprt);
          nyk=(lnkptr(lprt+1))-yk;
	  CodeC=CodeC+',&(outtb['+string(yk-1)+']),&nrd_'+string(nyk);
	  proto=proto+", double *, int *"
	end
      end
//RN if ztyp, add two dummy arguments at the end
      if ztyp(i) then
	CodeC=CodeC+',w,&nrd_0);';
	proto=proto+',double *,int * );'
      else
	CodeC=CodeC+');';
	proto=proto+');'
      end
    end
  elseif ftyp==2 
    if ~ztyp(i) then
      CodeC=fun+'(&flag,&block_'+rdnom+'['+string(i-1)+'].nevprt,told,block_'+rdnom+'['+string(i-1)+'].xd,..
            block_'+rdnom+'['+string(i-1)+'].x,&block_'+rdnom+'['+string(i-1)+'].nx';
      CodeC=CodeC+',block_'+rdnom+'['+string(i-1)+'].z,&block_'+rdnom+'['+string(i-1)+'].nz,block_'+rdnom+'['+string(i-1)+'].evout,..
	    &block_'+rdnom+'['+string(i-1)+'].nevout,block_'+rdnom+'['+string(i-1)+'].rpar,&block_'+rdnom+'['+string(i-1)+'].nrpar,..
	    block_'+rdnom+'['+string(i-1)+'].ipar';
      CodeC=CodeC+',&block_'+rdnom+'['+string(i-1)+'].nipar,block_'+rdnom+'['+string(i-1)+'].inptr,block_'+rdnom+'['+string(i-1)+'].insz';
      CodeC=CodeC+',&block_'+rdnom+'['+string(i-1)+'].nin,block_'+rdnom+'['+string(i-1)+'].outptr,block_'+rdnom+'['+string(i-1)+'].outsz';
      CodeC=CodeC+',&block_'+rdnom+'['+string(i-1)+'].nout);';
      proto='void '+fun+"(int *, int *,double *,"+...
	  " double *, double *, int *, double *, int *, double *, "+...
	  " int *, double *, int *, int *, int *, "+...
	  " double **, int *, int *, double **,int"+...
	  " *, int *);" 
    else
	CodeC=fun+'(&flag,&block_'+rdnom+'['+string(i-1)+'].nevprt,told,block_'+rdnom+'['+string(i-1)+'].xd,..
        block_'+rdnom+'['+string(i-1)+'].x,&block_'+rdnom+'['+string(i-1)+'].nx';
	CodeC=CodeC+',block_'+rdnom+'['+string(i-1)+'].z,&block_'+rdnom+'['+string(i-1)+'].nz,block_'+rdnom+'['+string(i-1)+'].evout,..
	&block_'+rdnom+'['+string(i-1)+'].nevout,block_'+rdnom+'['+string(i-1)+'].rpar,&block_'+rdnom+'['+string(i-1)+'].nrpar,..
	block_'+rdnom+'['+string(i-1)+'].ipar';
	CodeC=CodeC+',&block_'+rdnom+'['+string(i-1)+'].nipar,block_'+rdnom+'['+string(i-1)+'].inptr,block_'+rdnom+'['+string(i-1)+'].insz,..
	    &block_'+rdnom+'['+string(i-1)+'].nin,block_'+rdnom+'['+string(i-1)+'].outptr,block_'+rdnom+'['+string(i-1)+'].outsz,..
            &block_'+rdnom+'['+string(i-1)+'].nout,block_'+rdnom+'['+string(i-1)+'].g,&block_'+rdnom+'['+string(i-1)+'].ng);';
	proto='void '+fun+"(int *, int *,double *,"+...
	  " double *, double *, int *, double *, int *, double *, "+...
	  " int *, double *, int *, int *, int *, "+...
	  " double **, int *, int *, double **,int"+...
	  " *, int *,double *,int * );" 
    end
  elseif ftyp==4
     CodeC=fun+'(&block_'+rdnom+'['+string(i-1)+'], flag);'
     proto='void '+fun+"(scicos_block *, int );" 
  end
  //
  Code=[Code;cformatline(CodeC,70);' ']
  proto=cformatline(proto,70);
endfunction

//
//16/06/07 Author : ?, A. Layec
//
//Copyright INRIA
function [Code,capt,proto]=call_sensor(i)
  nin=inpptr(i+1)-inpptr(i); ///* number of input ports */
  nout=outptr(i+1)-outptr(i); ///* number of output ports */

  //declaration des I/O des blocs de type 1
  if funtyp(i)==0 then
    if nout==0 then
      yk    = 0;
      nyk_1 = 0;
      nyk_2 = 0;
      yk_t  = 1;
      //Code($+1)=+'  args[1]=(double *)(outtbptr[0]);';
    else
      yk    = outlnk(outptr(i));
      nyk_1 = size(outtb(yk),1);
      nyk_2 = size(outtb(yk),2);
      yk_t  = mat2scs_c_nb(outtb(yk));
      //Code($+1)='  args[1]=('+mat2scs_c_ptr(outtb(yk))+' *)(outtbptr['+string(yk-1)+']);';
    end

  end
  capt=[i yk nyk_1 nyk_2 yk_t bllst(i).ipar]
  Code($+1)='block_'+rdnom+'['+string(i-1)+'].nevprt=nevprt;'
  Code=['/* Call of sensor (blk nb '+string(i)+') */'
        Code;
        'nport = '+string(nbcap)+';';
        rdnom+'_sensor(&flag, &nport, &block_'+rdnom+'['+string(i-1)+'].nevprt, '+..
        'told, ('+mat2scs_c_ptr(outtb(yk))+' *)(outtbptr['+string(yk-1)+']), &nrd_'+string(nyk_1)+..
        ', &nrd_'+string(nyk_2)+', &nrd_'+string(yk_t)+',aaa);'];
  proto='void '+rdnom+'_sensor('+..
        'int *, int *, int *, double *,void *, int *, int *,int *, int);'
  proto=cformatline(proto,70);
endfunction

//Generates Code for dynamically linked Fortran and C Blocks
function [CCode,FCode]=gen_blocks()
  CCode=[]
  FCode=[]

  kdyn=find(funtyp>1000) //dynamically linked blocs
                         //100X : Fortran blocks
                         //200X : C blocks

  //if kdyn==[] then
    //return
  //end

  if (size(kdyn,'*') >1)
    kfuns=[]; 
    //get the block data structure in the initial scs_m structure
    if size(corinv(kdyn(1)),'*')==1 then
      O=scs_m.objs(corinv(kdyn(1)));
    else
      path=list('objs');
      for l=corinv(kdyn(1))(1:$-1)
        path($+1)=l;
        path($+1)='model';
        path($+1)='rpar';
        path($+1)='objs';
      end
      path($+1)=corinv(kdyn(1))($);
      O=scs_m(path);
    end
    if funtyp(kdyn(1))>2000 then
      //C block
      CCode=[CCode;O.graphics.exprs(2)]
    else
      FCode=[FCode;O.graphics.exprs(2)]
    end
    kfuns=funs(kdyn(1));
    for i=2:size(kdyn,'*')
      //get the block data structure in the initial scs_m structure
      if size(corinv(kdyn(i)),'*')==1 then
        O=scs_m.objs(corinv(kdyn(i)));
      else
        path=list('objs');
         for l=corinv(kdyn(i))(1:$-1)
           path($+1)=l;
           path($+1)='model';
           path($+1)='rpar';
           path($+1)='objs';
        end
        path($+1)=corinv(kdyn(i))($);
        O=scs_m(path);
      end
      if (find(kfuns==funs(kdyn(i))) == [])
        kfuns=[kfuns;funs(kdyn(i))];
        if funtyp(kdyn(i))>2000  then
          //C block
          CCode=[CCode;O.graphics.exprs(2)]
        else
          FCode=[FCode;O.graphics.exprs(2)]
        end
      end
    end
  elseif (size(kdyn,'*')==1)
    //get the block data structure in the initial scs_m structure
    if size(corinv(kdyn),'*')==1 then
      O=scs_m.objs(corinv(kdyn));
    else
      path=list('objs');
      for l=corinv(kdyn)(1:$-1)
        path($+1)=l;
        path($+1)='model';
        path($+1)='rpar';
        path($+1)='objs';
      end
      path($+1)=corinv(kdyn)($);
      O=scs_m(path);
    end
    if funtyp(kdyn)>2000 then
      //C block
      CCode=[CCode;O.graphics.exprs(2)]
    else
      FCode=[FCode;O.graphics.exprs(2)]
    end
  end
  if CCode==[]
    CCode=['void no_ccode()'
           '{'
           '  return;'
           '}']
  end
endfunction

//Generates the C code for new block simulation
//
//Author : Rachid Djenidi
//
//Copyright INRIA
function ok=gen_ccode();

  [CCode,FCode]=gen_blocks()

  Code=[make_decl();
        Protos;
        make_static()
        make_computational();
        make_main0();
        make_main1(%f);
        make_main2(%f);
        make_init(%f);
        make_end(%f)
        c_make_doit1(cpr,%f);
        c_make_doit2(cpr,%f);
        c_make_outtb(%f);
        c_make_initi(cpr,%f);
        c_make_endi(cpr,%f);
        make_putevs()]

  ierr=execstr('mputl(Code,rpat+''/''+rdnom+''.c'')','errcatch')
  if ierr<>0 then
    message(lasterror())
    ok=%f
    return
  end
  if FCode<>[] then
    ierr=execstr('mputl(FCode,rpat+''/''+rdnom+''f.f'')','errcatch')
    if ierr<>0 then
      message(lasterror())
      ok=%f
      return
    end
  end
  if CCode<>[] then
    ierr=execstr('mputl(CCode,rpat+''/''+rdnom+''_Cblocks.c'')','errcatch')
    if ierr<>0 then
      message(lasterror())
      ok=%f
      return
    end
  end

  //** generate _void_io.c
  Code=['/*---------------------------------------- Actuators */'
        'void '+rdnom+'_actuator(flag,nport,nevprt,t,u,nu1,nu2,ut,flag1)'
             '     int *flag,*nevprt,*nport;'
             '     int *nu1,*nu2,*ut,flag1;'
        '     double *t;'
        '     void *u;'
        '{'
        '  int k,l;'
        '}'
        '/*---------------------------------------- Sensor */'
        'void '+rdnom+'_sensor(flag,nport,nevprt,t,y,ny1,ny2,yt,flag1)'
        '     int *flag,*nevprt,*nport;'
        '     int *ny1,*ny2,*yt,flag1;'
        '     double *t;'
        '     void *y;'
        '{'
        '  int k,l;'
        '}']
  ierr=execstr('mputl(Code,rpat+''/''+rdnom+''_void_io.c'')','errcatch')
  if ierr<>0 then
    message(lasterror())
    ok=%f
    return
  end

  //** generate _standalone.c
  Code=[make_decl_standalone()
        Protostalone;
        make_static_standalone()
        make_standalone()]
  ierr=execstr('mputl(Code,rpat+''/''+rdnom+''_standalone.c'')','errcatch')
  if ierr<>0 then
    message(lasterror())
    ok=%f
    return
  end

  //** generate _act_sens_events.c
  Code=['#include <stdio.h>'
        '#include <stdlib.h>'
        '#include <math.h>'
        '#include <string.h>'
        'extern void **'+rdnom+'_block_outtbptr;'
        'extern char input[50],output[50];'
        'FILE *fprr,*fprw;'
        make_outevents()
        make_actuator(%t)
        make_sensor(%t)]

  created=[];
  reponse=[];

  created=fileinfo(rpat+'/'+rdnom+'_act_sens_events.c')
  if created~=[] then
    reponse=x_message(['File: ""'+rdnom+'_act_sens_events.c"" already exists,';
                       'do you want to replace it ?'],['Yes','No']);
  end

  if reponse==1 |  reponse==[] then
    ierr=execstr('mputl(Code,rpat+''/''+rdnom+''_act_sens_events.c'')', ...
                 'errcatch')
  end

  if ierr<>0 then
    message(lasterror())
    ok=%f
    return
  end
endfunction

//creates the Scicos GUI function associated with the new block
//
//Author : Rachid Djenidi, A.Layec
//
//20/06/07, A.Layec : update with in2,out2,intyp,outtyp
//27/06/07, A.Layec : update opar,oz
//
//Copyright INRIA
function ok=gen_gui();
  clkinput=ones(clkIN)';
  //outtb=outtb;
  oz=cpr.state.oz;

  new_oz=list();
  for i=1:lstsize(oz)
    new_oz($+1) = oz(i)
  end
  for i=1:lstsize(outtb)
    new_oz($+1) = outtb(i)
  end

  //outtb($+1) = zeros(nblk,1);
  Code=['function [x,y,typ]='+rdnom+'_c(job,arg1,arg2)';
        '// Copyright INRIA';
        ' x=[];y=[];typ=[];';
        ' select job';
        ' case ''plot'' then';
        '   standard_draw(arg1)';
        ' case ''getinputs'' then';
        '   [x,y,typ]=standard_inputs(arg1)';
        ' case ''getoutputs'' then';
        '   [x,y,typ]=standard_outputs(arg1)';
        ' case ''getorigin'' then';
        '   [x,y]=standard_origin(arg1)';
        ' case ''set'' then';
        '   x=arg1;';
        ' case ''define'' then'
        '   '+sci2exp(capt(:,3),'in',70); //input ports sizes 1
        '   '+sci2exp(capt(:,4),'in2',70); //input ports sizes 2
        '   '+sci2exp(scs_c_nb2scs_nb(capt(:,5)),'intyp',70); //input ports type
        '   '+sci2exp(actt(:,3),'out',70); //output ports sizes 1
        '   '+sci2exp(actt(:,4),'out2',70); //output ports sizes 2
        '   '+sci2exp(scs_c_nb2scs_nb(actt(:,5)),'outtyp',70); //output ports type
        '   '+sci2exp(x,'x',70); //initial continuous state
        '   '+sci2exp(z,'z',70); //initial discrete state
        '   iwa=zeros('+string(clkptr($))+',1)';
        '   work=zeros('+string(nblk)+',1)';
        '   Z=[z;iwa;work]';
        '   //odstate=[oz;outtb]'
        '   '+sci2exp(new_oz,'odstate',70);
        '   '+sci2exp(cpr.sim.rpar,'rpar',70); //real parameters
        '   '+sci2exp(cpr.sim.ipar,'ipar',70); //integer parameters
        '   '+sci2exp(cpr.sim.opar,'opar',70); //object parameters
        '   '+sci2exp(clkinput,'clkinput',70);
        '   nzcross='+string(sum(cpr.sim.zcptr(2:$)-cpr.sim.zcptr(1:$-1)))';
        '   nmode='+string(sum(cpr.sim.modptr(2:$)-cpr.sim.modptr(1:$-1)))';]

  for i=1:length(bllst)
    deput=[%t,%f]
    if (bllst(i).dep_ut(2) == %t) then
      deput(1,2)=%t;
      break;
    end
  end
  Code($+1)='   '+sci2exp(deput,'dep_ut',70);
  Code=[Code
        '   model=scicos_model(sim=list('''+rdnom+''',4),..'
        '                      in=in,..'
        '                      in2=in2,..'
        '                      intyp=intyp,..'
        '                      out=out,..'
        '                      out2=out2,..'
        '                      outtyp=outtyp,..'
        '                      evtin=clkinput,..'
        '                      state=x,..'
        '                      dstate=Z,..'
        '                      odstate=odstate,..'
        '                      rpar=rpar,..'
        '                      ipar=ipar,..'
        '                      opar=opar,..'
        '                      blocktype=''c'',..'
        '                      dep_ut=dep_ut,..'
        '                      nzcross=nzcross,..'
        '                      nmode=nmode)'
        '   gr_i=''xstringb(orig(1),orig(2),'''''+rdnom+''''',sz(1),..'
        '          sz(2),''''fill'''')''';
        '   x=standard_define([2 2],model,[],gr_i)';
        ' end'
        'endfunction'];
  //Create file
  ierr=execstr('mputl(Code,rpat+''/''+rdnom+''_c.sci'')','errcatch')
  if ierr<>0 then
    message(lasterror())
    ok=%f
  end
endfunction

//generates the Scilab script for defining the newly created block
//into Scilab.
function ok=gen_loader()
  SCode=['//exec file used to load the ""compiled"" block into Scilab'
         'rdnom='+sci2exp(rdnom);
         '// get the absolute path of this loader file' 
         'DIR=get_absolute_file_path(rdnom+''_loader.sce'')'
         'Makename = DIR+rdnom+''_Makefile'';'
         'select COMPILER'
         'case ''VC++'' then'
         '  Makename = strsubst(Makename,''/'',''\'')+''.mak'';'
         'end'
         '//unlink if necessary'
         '[a,b]=c_link(rdnom); while a ;ulink(b);[a,b]=c_link(rdnom);end';
         'libn=ilib_compile('+sci2exp('lib'+rdnom)+',Makename)';
         'if MSDOS then'
         '  fileso=strsubst(libn,''/'',''\'')';
         'else'
         '  fileso=strsubst(libn,''\'',''/'')';
         'end';
         'link(fileso,rdnom,''c'')';
         '//load the gui function';
         'getf(DIR+''/''+rdnom+''_c.sci'');']
  ierr=execstr('mputl(SCode,rpat+''/''+rdnom+''_loader.sce'')','errcatch')
  if ierr<>0 then
    message(lasterror())
    ok=%f
    return
  end
endfunction

function Makename=gen_make(name,files,libs,Makename)
  if getenv('WIN32','NO')=='OK' then
    Makename=gen_make_win32(name,files,libs)
  else
    Makename=gen_make_unix(name,files,libs)
  end
endfunction

function Makename=gen_make_unix(name,files,libs,Makename)
  //   "OBJSSTAN="+strcat(strsubst(files,'_void_io','_standalone')+'.o',' ...
  //		')+' '+rdnom+'_act_sens_events.o'

  T=["# generated by builder.sce: Please do not edit this file"
     "# ------------------------------------------------------"
     "SCIDIR = "+SCI
     "OBJS = "+strcat(files+'.o',' ')
     "OBJSSTAN="+rdnom+'_standalone.o '+rdnom+'_act_sens_events.o '+rdnom+'_Cblocks.o'
     "SCILIBS = $(SCIDIR)/libs/scicos.a $(SCIDIR)/libs/lapack.a "+..
     "$(SCIDIR)/libs/poly.a $(SCIDIR)/libs/calelm.a "+..
     "$(SCIDIR)/libs/blas.a $(SCIDIR)/libs/lapack.a $(SCIDIR)/libs/os_specific.a"
     "LIBRARY =  lib"+name
     "OTHERLIBS = "+libs
     "include $(SCIDIR)/Makefile.incl";
     "CFLAGS = $(CC_OPTIONS) -I$(SCIDIR)/routines/"
     "FFLAGS = $(FC_OPTIONS) -I$(SCIDIR)/routines/"
     "include $(SCIDIR)/config/Makeso.incl"
     "standalone: $(OBJSSTAN) $(OTHERLIBS) "
      ascii(9)+"f77 $(FFLAGS) -o $@  $(OBJSSTAN) $(OTHERLIBS) $(SCILIBS)"];
  mputl(T,Makename)
endfunction
//     "OBJSSTAN="+strcat(strsubst(files,'_void_io','_standalone')+'.o',' ') 

function Makename=gen_make_win32(name,files,libs,Makename)
  WSCI=strsubst(SCI,'/','\')
//  "OBJSSTAN="+strcat(strsubst(files,'_void_io','_standalone')+'.obj',' ')+' '+rdnom+'_act_sens_events.obj'
  T=["# generated by builder.sce: Please do not edit this file"
     "# ------------------------------------------------------"
     "SHELL = /bin/sh"
     "SCIDIR = "+SCI
     "SCIDIR1 = "+WSCI
     "SCILIBS = """+WSCI+"\bin\LibScilab.lib"""
     "LIBRARY = lib"+name
     "OBJS = "+strcat(files+'.obj',' ')
     'OBJSSTAN='+rdnom+'_standalone.obj '+rdnom+'_act_sens_events.obj '+rdnom+'_Cblocks.obj'
     "OTHERLIBS = "+libs
     ""
     "DUMPEXTS="""+WSCI+"\bin\dumpexts"""
     "SCIIMPLIB="""+WSCI+"\bin\LibScilab.lib"""
     ""
     "all::"
     "CC=cl"
     "LINKER=link"
     "LINKER_FLAGS=/NOLOGO /machine:ix86"
     "INCLUDES=-I"""+WSCI+"\routines\f2c""" 
     "CC_COMMON=-D__MSC__ -DWIN32 -c -DSTRICT -nologo $(INCLUDES)" 
     "CC_OPTIONS = $(CC_COMMON) -Od  -GB -Gd -W3"
     "CC_LDFLAGS = "
     "CFLAGS = $(CC_OPTIONS) -DFORDLL -I"""+WSCI+"\routines"""
     "FFLAGS = $(FC_OPTIONS) -DFORDLL -I"""+WSCI+"\routines"""
     ""
     "all :: $(LIBRARY).dll"
     " "
     "$(LIBRARY).dll: $(OBJS)"
     ascii(9)+"@echo Creation of dll $(LIBRARY).dll and import lib from ..."
     ascii(9)+"@echo $(OBJS)"
     ascii(9)+"@$(DUMPEXTS) -o ""$*.def"" ""$*.dll"" $**"
     ascii(9)+"@$(LINKER) $(LINKER_FLAGS) $(OBJS) $(SCIIMPLIB) $(XLIBSBIN) $(TERMCAPLIB) /nologo /dll /out:""$*.dll"" /implib:""$*.ilib"" /def:""$*.def""" 
     ".c.obj:"
     ascii(9)+"@echo ------------- Compile file $< --------------"
     ascii(9)+"$(CC) $(CFLAGS) $< "
     ".f.obj:"
     ascii(9)+"@echo ----------- Compile file $*.f (using f2c) -------------"
     ascii(9)+"@"""+WSCI+"\bin\f2c.exe"" $(FFLAGS) $*.f "
     ascii(9)+"@$(CC) $(CFLAGS) $*.c "
     ascii(9)+"@del $*.c "
     "clean::"
     ascii(9)+"@del *.CKP "
     ascii(9)+"@del *.ln "
     ascii(9)+"@del *.BAK "
     ascii(9)+"@del *.bak "
     ascii(9)+"@del *.def"
     ascii(9)+"@del *.dll"
     ascii(9)+"@del *.exp"
     ascii(9)+"@del *.ilib"
     ascii(9)+"@del errs" 
     ascii(9)+"@del *~ "
     ascii(9)+"@del *.obj"
     ascii(9)+"@del .emacs_* "
     ascii(9)+"@del tags "
     ascii(9)+"@del tags "
     ascii(9)+"@del TAGS "
     ascii(9)+"@del make.log "
     ""
     "distclean:: clean "
     " "
     "standalone: $(OBJSSTAN) $(OTHERLIBS) "
      ascii(9)+"$(LINKER) $(LINKER_FLAGS)  $(OBJSSTAN) $(OTHERLIBS) $(SCILIBS)  /out:standalone.exe "]

  select COMPILER;
    case 'VC++'   then 
    makename = strsubst(Makename,'/','\')+'.mak'
  end
  mputl(T,makename)
endfunction


//compiles the generated C code and link it with Scilab
//Copyright INRIA
//Author : Rachid Djenidi
function [ok,Makename]=buildnewblock()
  files=[rdnom rdnom+'_void_io' rdnom+'_Cblocks']
  [fd,ierr]=mopen(rpat+'/'+rdnom+'f.f','r')
  if ierr==0 then
    mclose(fd)
    files=[files,rdnom+'f']
  end
  if (archname ~= emptystr()) then
    archname=pathconvert(archname,%f,%t)
  end
  Makename=gen_make(rdnom,files,archname,rpat+'/'+rdnom+'_Makefile')
  //unlink if necessary
  [a,b]=c_link(rdnom);
  while a
    ulink(b);
    [a,b]=c_link(rdnom);
  end
  ierr=execstr('libn=ilib_compile(''lib''+rdnom,Makename)','errcatch')
  if ierr<>0 then
     ok=%f;x_message(['sorry compiling problem';lasterror()]);
     return;
  end
  libn=pathconvert(libn,%f,%t)
  ierr=execstr('libnumber=link(libn)','errcatch')
  ierr=execstr('link(libnumber,rdnom,''c'')','errcatch')
  if ierr<>0 then
    ok=%f;
    x_message(['sorry link problem';lasterror()]);
    return;
  end
endfunction

function  [ok,XX,alreadyran]=do_compile_superblock(XX,all_scs_m,numk,alreadyran)
// Transforms a given Scicos discrete and continuous SuperBlock into a C defined Block
// Copyright INRIA

  scs_m=XX.model.rpar
  par=scs_m.props;
  if alreadyran then
    //terminate current simulation
    do_terminate()
    alreadyran=%f
  end
  hname=scs_m.props.title(1) //superblock name
  //***********************************************************
  //Check blocks properties and adapt them if necessary
  //***********************************************************

  IN=[];OUT=[];clkIN=[];numa=[];numc=[];
  for i=1:size(scs_m.objs)
    if typeof(scs_m.objs(i))=='Block' then
      if scs_m.objs(i).gui=='CLKOUT_f' then
        ok=%f;%cpr=list()
        message('Superblock should not have any activation output port.')
        return
      elseif scs_m.objs(i).gui=='IN_f' then
        //replace input ports by sensor blocks
        numc=numc+1
        scs_m.objs(i).gui='INPUTPORTEVTS';
        scs_m.objs(i).model.evtin=1
        scs_m.objs(i).model.sim(1)='capteur'+string(numc)
        IN=[IN scs_m.objs(i).model.ipar]
      elseif scs_m.objs(i).gui=='OUT_f' then
        //replace output ports by actuator blocks
        numa=numa+1
        scs_m.objs(i).gui='OUTPUTPORTEVTS';
        scs_m.objs(i).model.sim(1)='actionneur'+string(numa)
        OUT=[OUT  scs_m.objs(i).model.ipar]
      elseif scs_m.objs(i).gui=='CLKINV_f' then
        //replace event input ports by  fictious block
        scs_m.objs(i).gui='EVTGEN_f';
        scs_m.objs(i).model.sim(1)='bidon'
        clkIN=[clkIN scs_m.objs(i).model.ipar];
        //elseif scs_m.objs(i).model.dep_ut(2)==%t then
        //check for time dependency PAS IICI
        //ok=%f;%cpr=list()
        //message('a block have time dependence.')
        //return
      end
    end
  end

  //Check if input/output ports are numered properly
  IN=-sort(-IN);
  if or(IN<>[1:size(IN,'*')]) then
    ok=%f;%cpr=list()
    message('Input ports are not numbered properly.')
    return
  end
  OUT=-sort(-OUT);
  if or(OUT<>[1:size(OUT,'*')]) then
    ok=%f;%cpr=list()
    message('Output ports are not numbered properly.')
    return
  end
  clkIN=-sort(-clkIN);
  if or(clkIN<>[1:size(clkIN,'*')]) then
    ok=%f;%cpr=list()
    message('Event input ports are not numbered properly.')
    return
  end

  //Check if there is more than one clock in the diagram
  szclkIN=size(clkIN,2);
  if szclkIN==0 then
    szclkIN=[]
  end

  [bllst,connectmat,clkconnect,cor,corinv,ok]=c_pass1(scs_m);

  //Test for  ALWAYS_ACTIVE sblock (RN -25/06/07)
  ALWAYS_ACTIVE=%f;
  for blki=bllst
    if blki.dep_ut($) then
      ALWAYS_ACTIVE=%t;
      break;
    end
  end
  if ALWAYS_ACTIVE then
    CAPTEURS=[];
    for Ii=1:length(bllst)
//      if part(bllst(Ii).sim(1),1:7)=='capteur' then
        bllst(Ii).dep_ut($)=%t
//      elseif part(bllst(Ii).sim(1),1:10)=='actionneur' then
//        bllst(Ii).dep_ut($)=%t
//      end
    end
  end

  if ~ok then
    message('Sorry: problem in the pre-compilation step.')
    return
  end
  a=[];
  b=[];
  tt=[];
  howclk=[];
  allhowclk=[];
  cap=[];
  act=[];

  ///**********************************
  for i=1:size(bllst)
    for j=1:size(bllst)
      if (bllst(i).sim(1)=='actionneur'+string(j)) then
        if tt<>i then
          act=[act;i];
          tt=i;
        end
      elseif (bllst(i).sim(1)=='capteur'+string(j)) then
        if tt<>i then
          cap=[cap;i];
          tt=i;
        end
      elseif (bllst(i).sim(1)=='bidon') then
        if tt<>i then
          allhowclk=[allhowclk;i];
          tt=i;
        end
      end
    end
    ///**********************************
  end
  if szclkIN>1 then
    //replace the N Event inputs by a fictious block with 2^N as many event
    //outputs
    output=ones((2^szclkIN)-1,1)
    bllst($+1)=scicos_model(sim=list('bidon',1),evtout=output,..
                            blocktype='d',..
                            firing=-output',dep_ut=[%f %f])
    corinv(size(bllst))=size(bllst)+1;
    howclk=size(bllst)
    // adjust the links accordingly
    for i=1:(2^szclkIN)-1
      vec=codebinaire(i,szclkIN)
      for j=1:szclkIN
        if vec(j)*allhowclk(j)>=1 then
          for k=1:size(clkconnect,1)
            if clkconnect(k,1)==allhowclk(j) then
              clkconnect=[clkconnect;[howclk i clkconnect(k,3:4)]]
            end
          end
        end
      end
    end
  elseif szclkIN==[]&~ALWAYS_ACTIVE then
    //superblock has no Event input, add a fictious clock
    output=ones((2^(size(cap,'*')))-1,1)
    if (output == []) then
      output=0;
    end
    bllst($+1)=scicos_model(sim=list('bidon',1),evtout=output,..
                            firing=-output,blocktype='d',dep_ut=[%f %f])
    corinv(size(bllst))=size(bllst)+1;
    howclk=size(bllst);
  elseif szclkIN==1  then
    howclk=allhowclk;
  end

  //mise en ordre de clkconnect
  if szclkIN>1 then
    newclkconnect=clkconnect;
    clkconnect=[];
    for i=1:size(newclkconnect,1)-1
      if or(newclkconnect(i,:)<>newclkconnect(i+1,:)) then
        clkconnect=[clkconnect;newclkconnect(i,:)]
      end
    end
    if or(newclkconnect($-1,:)<>newclkconnect($,:)) then
      clkconnect=[clkconnect;newclkconnect($,:)]
    end

    //suppression des blocs bidons
    newclkconnect=clkconnect;nkt=[];
    for i=1:szclkIN
      for k=1:size(newclkconnect,1)
        if newclkconnect(k,1)~=allhowclk(i) then
          nkt=[nkt;newclkconnect(k,:)];
        end
      end
      newclkconnect=nkt;
      nkt=[];
    end
    clkconnect=newclkconnect;
  end

  //**************************************************
  // nouveau clkconnect avec liaisons sur les capteurs
  //**************************************************
  //Test for  ALWAYS_ACTIVE sblock (RN -25/06/07)
  if ~(ALWAYS_ACTIVE&szclkIN==[]) then
    n=size(cap,1)
    for i=1:n
      if szclkIN>1 then
        for j=1:(2^szclkIN)-1
          clkconnect=[clkconnect;[howclk j cap(i) 1]];
        end
      elseif szclkIN==1 then
        clkconnect=[clkconnect;[howclk 1 cap(i) 1]];
      end
    end

    //codage de l'activation des capteurs dans le cas de l'heritage
    for i=1:2^n-1
      vec=codebinaire(i,n);
      for j=1:n
        if (vec(j)==1) then
          clkconnect=[clkconnect;[howclk i cap(j) 1]];
        end
      end
    end
  end

  Code_gene_run=[];
  %windo=xget('window')

  cpr=c_pass2(bllst,connectmat,clkconnect,cor,corinv)

  xset('window',%windo)

  if cpr==list() then ok=%f,return, end

  ///////////////////
  //les pointeurs de cpr :
  x=cpr.state.x;
  z=cpr.state.z;
  outtb=cpr.state.outtb;

  //RN
  zcptr=cpr.sim.zcptr;
  ozptr=cpr.sim.ozptr;
  rpptr=cpr.sim.rpptr;
  ipptr=cpr.sim.ipptr;
  opptr=cpr.sim.opptr;
  funs=cpr.sim.funs;
  xptr=cpr.sim.xptr;
  zptr=cpr.sim.zptr;
  inpptr=cpr.sim.inpptr;
  inplnk=cpr.sim.inplnk;
  outptr=cpr.sim.outptr;
  outlnk=cpr.sim.outlnk;
  //@l@n lnkptr=cpr.sim.lnkptr;
  ordclk=cpr.sim.ordclk;
  funtyp=cpr.sim.funtyp;
  cord=cpr.sim.cord;
  ncord=size(cord,1);
  nblk=cpr.sim.nb;
  ztyp=cpr.sim.ztyp;
  clkptr=cpr.sim.clkptr
  //taille totale de z : nztotal
  nztotal=size(z,1);

  //*******************************
  //Checking if superblock is valid
  //*******************************
  msg=[]
  for i=1:length(funs)-1
    if funtyp(i)==3 then
      msg=[msg;'Type 3 block''s not allowed']
//RN   elseif ztyp(i)<>0 then
    //elseif (zcptr(i+1)-zcptr(i))<>0 then
//
      //msg=[msg;'Active zero crossing block''s not allowed']
     elseif (clkptr(i+1)-clkptr(i))<>0 &funtyp(i)>-1 &funs(i)~='bidon' then
      msg=[msg;'Regular block generating activation not allowed yet']
    end
    if msg<>[] then message(msg),ok=%f,return,end
  end

  //********************************************************
  // Change logical units for readf and writef blocks if any ???
  //********************************************************
  lunit=0
  for d=1:length(funs)
    if funs(d)=='readf'  then
      z(zptr(d)+2)=lunit
      lunit=lunit+1;
    elseif funs(d)=='writef'
      z(zptr(d)+1)=lunit
      lunit=lunit+1;
    end
  end

  //***********************************
  // Get the name of the file
  //***********************************
  foo=3;
  okk=%f;
  rdnom='foo';
  rpat=getcwd();
  archname='';
  label1=[hname;getcwd()+'/'+hname;''];

  //ab=[];
  //routines=create_palette('all');

  //for kf=1:nblk
    //if (part(funs(kf),1:7) ~= 'capteur' & part(funs(kf),1:10) ~= 'actionneur' & funs(kf) ~= 'bidon') then 
      //ab=[ab,funs(kf)];
   //end
 //end
  //[ab,kab]=setdiff(ab,routines);

  while %t do
    [okk,..
     rdnom,..
     rpat,..
     archname,..
     label1]=getvalue('PLEASE, GIVE US SOME INFORMATION.',..
                      ['New block''s name :';
                       'Created files Path';
                       'Other object files to link with ( if any)'],..
                       list('str',1,'str',1,'str',1),label1);
    if okk==%f then
      ok=%f
      return
    end
    rpat=stripblanks(rpat);

    //** Put a warning here in order to inform the user
    //** that the name of the superblock will change
    //** because the "-" char could generate GCC problems 
    //** (the C functions contains the name of the superblok).

    if grep(rdnom,"-")<>[] then
    message(['For full C compiler compatibility ';
             'Superblock name cannot contains ""-"" characters';
             '""-"" chars will be automatically substituted by ""_"" '])
    end

    rdnom = strsubst(rdnom,'-','_');

    //**
    //** rpat=strsubst(rpat,'-','_');
    //**

    dirinfo=fileinfo(rpat)
    if dirinfo==[] then
      [pathrp,fnamerp,extensionrp]=fileparts(rpat)
      ok=mkdir(pathrp,fnamerp+extensionrp)
      if ~ok then 
        x_message('Directory '+rpat+' cannot be created');
      end
    elseif filetype(dirinfo(2))<>'Directory' then
      ok=%f;
      x_message(rpat+' is not a directory');
    end

    if stripblanks(rdnom)==emptystr() then 
      ok=%f;
      x_message('sorry C file name not defined');
    end
    if ok then break,end
  end

  //////////////////////////////////////////////////
  maxnrpar=max(rpptr(2:$)-rpptr(1:$-1))
  maxnipar=max(ipptr(2:$)-ipptr(1:$-1))
  maxnx=max(xptr(2:$)-xptr(1:$-1))
  maxnz=max(zptr(2:$)-zptr(1:$-1))
  maxnin=max(inpptr(2:$)-inpptr(1:$-1))
  maxnout=max(outptr(2:$)-outptr(1:$-1))

  maxtotal=max([maxnrpar;maxnipar;maxnx;maxnz;maxnin;maxnout]);
  if maxtotal< length(cpr(2).outtb) then
    maxtotal=length(cpr(2).outtb)
  end

  //************************************************************************
  //generate the call to the blocks and blocs simulation function prototypes
  //************************************************************************
  wfunclist=list();
  nbcap=0;nbact=0;capt=[];actt=[];Protostalone=[];Protos=[];
  dfuns=[]
  for i=1:length(funs)
    if or(i==act) then //block is an actuator
      nbact=nbact+1;
      [Code,actti,protoi]=call_actuator(i)
      wfunclist($+1)=[Code;'if(flag < 0 ) return(5 - flag);']
      if nbact==1 then Protostalone=[Protostalone;'';protoi],end
      actt=[actt;actti]
    elseif or(i==cap) then //block is a sensor
      nbcap=nbcap+1;
      [Code,capti,protoi]=call_sensor(i)
      wfunclist($+1)=[Code;'if(flag < 0 ) return(5 - flag);']
      if nbcap==1 then Protostalone=[Protostalone;'';protoi] ,end
      capt=[capt;capti]
    elseif funs(i)=='bidon'
      wfunclist($+1)=' '
    else
      ki=find(funs(i)==dfuns)
      dfuns=[dfuns;funs(i)]
      //[Code,protoi]=call_block(i)
      [Code]=call_block4(i)
      if Code<>[] then
        wfunclist($+1)=[Code;'if(flag < 0 ) return(5 - flag);']
      else
        wfunclist($+1)=' ';
      end
      if ki==[] then
        Protos=[Protos;'';BlockProto(i)];
        Protostalone=[Protostalone;'';BlockProto(i)];
      end
    end
  end

  //**************************************
  //on prend en compte l'ordre des numerotation graphique des
  //capteur et des actionneur
  [junk,index]=sort(-actt(:,$));
  actt=actt(index,1:$) ;
  [junk,index]=sort(-capt(:,$));
  capt=capt(index,1:$) ;

  //***************************************************
  //Compute the initial state and outtb (links)  values
  //***************************************************
  //petite modification pour uniformiser les fichiers capteurs
  //et actionneurs ???
  rdcpr=cpr.sim.funs;
  for r=1:length(cap),rdcpr(cap(r))='bidon';end
  for r=1:length(act),rdcpr(act(r))='bidon';end
  Total_rdcpr=cpr.sim;Total_rdcpr.funs=rdcpr;
  //
  tcur=0;
  tf=scs_m.props.tf;
  tolerances=scs_m.props.tol;
  //[state,t]=scicosim(cpr.state,tcur,tf,Total_rdcpr,'start',tolerances);
  //cpr.state=state;
  z=cpr.state.z;
  outtb=cpr.state.outtb;
  //[junk_state,t]=scicosim(cpr.state,tcur,tf,Total_rdcpr,'finish',tolerances);

  //***********************************
  // Scilab and C files generation
  //***********************************
  ok=gen_gui();
  if ok then ok=gen_ccode(),end

  //*****************
  //Compile and link
  //*****************

  if ok then [ok,Makename]=buildnewblock(),end
  if ok then ok=gen_loader(),end

  if ok then
    //load the gui function
    getf(rpat+'/'+rdnom+'_c.sci');
    //Change diagram superblock to new generated block
    XX=update_block(XX);
    execstr(rdnom+'_c=resume('+rdnom+'_c)')
  end
endfunction

//BlockProto : generate prototype
//            of a scicos block
//
//inputs : bk   : bloc index
//
//output : txt  :
//
//16/06/07 Author : A.Layec
//Copyright INRIA
function [txt]=BlockProto(bk)

  nin=inpptr(bk+1)-inpptr(bk);  //* number of input ports */
  nout=outptr(bk+1)-outptr(bk); //* number of output ports */

  //**
  ftyp=funtyp(bk)
  if ftyp>2000 then ftyp=ftyp-2000,end
  if ftyp>1000 then ftyp=ftyp-1000,end

  //** check function type
  if ftyp < 0 then //** ifthenelse eselect blocks
      txt = [];
      return;
  else
    if (ftyp<>0 & ftyp<>1 & ftyp<>2 & ftyp<>3 & ftyp<>4) then
      disp("types other than 0,1,2,3 or 4 are not yet supported.")
      txt = [];
      return;
    end
  end

  //** add comment
  txt=[get_comment('proto_blk',list(funs(bk),funtyp(bk),bk));]

  select ftyp
    //** zero funtyp
    case 0 then

      //*********** prototype definition ***********//
      txtp=['(int *, int *, double *, double *, double *, int *, double *, \';
            ' int *, double *, int *, double *, int *,int *, int *, \';
            ' double *, int *, double *, int *);'];
      if (funtyp(bk)>2000 & funtyp(bk)<3000)
        blank = get_blank('void '+funs(bk)+'(');
        txtp(1) = 'void '+funs(bk)+txtp(1);
      elseif (funtyp(bk)<2000)
        txtp(1) = 'void C2F('+funs(bk)+')'+txtp(1);
        blank = get_blank('void C2F('+funs(bk)+')');
      end
      txtp(2:$) = blank + txtp(2:$);
      txt = [txt;txtp];
      //*******************************************//


    //**
    case 1 then

      //*********** prototype definition ***********//
      txtp=['(int *, int *, double *, double *, double *, int *, double *, \';
            ' int *, double *, int *, double *, int *,int *, int *';]
      if (funtyp(bk)>2000 & funtyp(bk)<3000)
        blank = get_blank('void '+funs(bk)+'(');
        txtp(1) = 'void '+funs(bk)+txtp(1);
      elseif (funtyp(bk)<2000)
        txtp(1) = 'void C2F('+funs(bk)+')'+txtp(1);
        blank = get_blank('void C2F('+funs(bk)+')');
      end
      if nin>=1 | nout>=1 then
        txtp($)=txtp($)+', \'
        txtp=[txtp;'']
        if nin>=1 then
          for k=1:nin
            txtp($)=txtp($)+' double *, int * ,'
          end
          txtp($)=part(txtp($),1:length(txtp($))-1); //remove last ,
        end
        if nout>=1 then
          if nin>=1 then
            txtp($)=txtp($)+', \'
            txtp=[txtp;'']
          end
          for k=1:nout
            txtp($)=txtp($)+' double *, int * ,'
          end
          txtp($)=part(txtp($),1:length(txtp($))-1); //remove last ,
        end
      end

      if ztyp(bk) then
        txtp($)=txtp($)+', \'
        txtp=[txtp;' double *,int *);'];
      else
        txtp($)=txtp($)+');';
      end

      txtp(2:$) = blank + txtp(2:$);
      txt = [txt;txtp];
      //*******************************************//

    //**
    case 2 then

      //*********** prototype definition ***********//

      txtp=['void '+funs(bk)+...
            '(int *, int *, double *, double *, double *, int *, double *, \';
            ' int *, double *, int *, double *, int *, int *, int *, \'
            ' double **, int *, int *, double **,int *, int *'];
      if ~ztyp(bk) then
        txtp($)=txtp($)+');';
      else
        txtp($)=txtp($)+', \';
        txtp=[txtp;
              ' double *,int *);']
      end
      blank = get_blank('void '+funs(bk));
      txtp(2:$) = blank + txtp(2:$);
      txt = [txt;txtp];
      //********************************************//

    //**
    case 4 then
      txt=[txt;
           'void '+funs(bk)+'(scicos_block *, int );'];

  end
endfunction

//Generating the routine for actuators interfacing
//
//Copyright INRIA
//
//Authors : R. Djenid, R. Nikoukhah, A. Layec
//
//actt=[i uk nuk_1 nuk_2 uk_t bllst(i).ipar]
function Code=make_actuator(standalone)
  Call=['/*'+part('-',ones(1,40))+' Actuators */';
        'void '+rdnom+'_actuator(flag,nport,nevprt,t,u,nu1,nu2,ut,flag1)']

  comments=['     /*'
            '      * To be customized for standalone execution';
            '      * flag  : specifies the action to be done'
            '      * nport : specifies the  index of the Super Bloc'
            '      *         regular input (The input ports are numbered'
            '      *         from the top to the bottom )'
            '      * nevprt: indicates if an activation had been received'
            '      *         0 = no activation'
            '      *         1 = activation'
            '      * t     : the current time value'
            '      * u     : the vector inputs value'
            '      * nu1   : the input size 1'
            '      * nu2   : the input size 2'
            '      * ut    : the input type'
            '      * flag1 : learn mode (0 from terminal,1 from input file'
            '      */']

  dcl=['     int *flag,*nevprt,*nport;'
       '     int *nu1,*nu2,*ut,flag1;'
       ''
       '     double *t;'
       '     void *u;'
       '{'
       '  int j,k,l;'];

  if standalone then
    a_actuator=['  /* skeleton to be customized */'
                '    switch (*flag) {'
                '    case 2 :'
                '      /*if(*nevprt>0) { get the input value */'
                '        switch (*ut) {'
                '        case 10 :'
                '          for (l=0;l<*nu2;l++) {'
                '            for (k=0;k<*nu1;k++) {'
                '              printf(""Actuator: time=%f, '+...
                                  'u(%d,%d) of actuator %d is %f '+...
                                  '\n"", \'
                '                     *t, k, l, *nport,'+...
                                      '*((double *) u+(k+l*(*nu1))));'
                '            }'
                '          }'
                '          break;'
                ''
                '        case 11 :'
                '          for (l=0;l<*nu2;l++) {'
                '            for (k=0;k<*nu1;k++) {'
                '              printf(""Actuator: time=%f, '+...
                                  'u(%d,%d) of actuator %d is %f,%f '+...
                                  '\n"", \'
                '                     *t, k, l, *nport,'+...
                                      '*((double *) u+(k+l*(*nu1))),'+...
                                      '*((double *) u+((*nu1)*(*nu2)+k+l*(*nu1))));'
                '            }'
                '          }'
                '          break;'
                ''
                '        case 81 :'
                '          for (l=0;l<*nu2;l++) {'
                '            for (k=0;k<*nu1;k++) {'
                '              printf(""Actuator: time=%f, '+...
                                  'u(%d,%d) of actuator %d is %i '+...
                                  '\n"", \'
                '                     *t, k, l, *nport,'+...
                                      '*((char *) u+(k+l*(*nu1))));'
                '            }'
                '          }'
                '          break;'
                ''
                '        case 82 :'
                '          for (l=0;l<*nu2;l++) {'
                '            for (k=0;k<*nu1;k++) {'
                '              printf(""Actuator: time=%f, '+...
                                  'u(%d,%d) of actuator %d is %hd '+...
                                  '\n"", \'
                '                     *t, k, l, *nport,'+...
                                      '*((short *) u+(k+l*(*nu1))));'
                '            }'
                '          }'
                '          break;'
                ''
                '        case 84 :'
                '          for (l=0;l<*nu2;l++) {'
                '            for (k=0;k<*nu1;k++) {'
                '              printf(""Actuator: time=%f, '+...
                                  'u(%d,%d) of actuator %d is %ld '+...
                                  '\n"", \'
                '                     *t, k, l, *nport,'+...
                                      '*((long *) u+(k+l*(*nu1))));'
                '            }'
                '          }'
                '          break;'
                ''
                '        case 811 :'
                '          for (l=0;l<*nu2;l++) {'
                '            for (k=0;k<*nu1;k++) {'
                '              printf(""Actuator: time=%f, '+...
                                  'u(%d,%d) of actuator %d is %d '+...
                                  '\n"", \'
                '                     *t, k, l, *nport,'+...
                                      '*((unsigned char *) u+(k+l*(*nu1))));'
                '            }'
                '          }'
                '          break;'
                ''
                '        case 812 :'
                '          for (l=0;l<*nu2;l++) {'
                '            for (k=0;k<*nu1;k++) {'
                '              printf(""Actuator: time=%f, '+...
                                  'u(%d,%d) of actuator %d is %hu '+...
                                  '\n"", \'
                '                     *t, k, l, *nport,'+...
                                      '*((unsigned short *) u+(k+l*(*nu1))));'
                '            }'
                '          }'
                '          break;'
                ''
                '        case 814 :'
                '          for (l=0;l<*nu2;l++) {'
                '            for (k=0;k<*nu1;k++) {'
                '              printf(""Actuator: time=%f, '+...
                                  'u(%d,%d) of actuator %d is %lu '+...
                                  '\n"", \'
                '                     *t, k, l, *nport,'+...
                                      '*((unsigned long *) u+(k+l*(*nu1))));'
                '            }'
                '          }'
                '          break;'
                '        }'
                '      /*} */'
                '      break;'
                '    case 4 : /* actuator initialisation */'
                '      /* do whatever you want to initialize the actuator */'
                '      break;'
                '    case 5 : /* actuator ending */'
                '      /* do whatever you want to end the actuator */'
                '      break;'
                '    }']
  else
    a_actuator=[]
  end

  // pour fprintf
  nc=size(act,'*') // Alan : d'où viens act ?????
                   // reponse : de do_compile_superblock!
  typ=['""%f ']; //time
  for i=1:nc
    typ($)=typ($)+'"" \'
    typ=[typ;'""'];
    for j=1:sum(actt(i,3)*actt(i,4))
      //typ=typ+'%f ';
      typ($)=typ($)+scs_c_n2c_fmt(actt(i,5))+' ';
    end
  end
  typ($)=typ($)+'\n"", \'
  typ(1)='    fprintf(fprw,'+typ(1);
  bl    ='                 ';
  if size(typ,1) <> 1 then
    typ(2:$) = bl+typ(2:$);
  end
  //Code1='    fprintf(fprw, '"'+typ+' \n'",*t'
  Code1=[typ;bl+'*t'];
  //actt=[i uk nuk_1 nuk_2 uk_t bllst(i).ipar]
  for i=1:size(actt,1)
    ni=actt(i,3)*actt(i,4) // dimension of ith output
    Code1($)=Code1($)+','
    Code1=[Code1;bl];
    for j=1:ni
      if actt(i,5)<>11 then
        Code1($)=Code1($)+...
                  '*((('+scs_c_n2c_typ(actt(i,5))+' *)('+...
                   rdnom+'_block_outtbptr+'+string(actt(i,2)-1)+'))'+...
                   '+'+string(j-1)+')';
      else //CAS CMPLX
        Code1($)=Code1($)+...
                  '*((('+scs_c_n2c_typ(actt(i,5))+' *)('+...
                   rdnom+'_block_outtbptr+'+string(actt(i,2)-1)+'))'+...
                   '+'+string((j-1))+'), '+...
                  '*((('+scs_c_n2c_typ(actt(i,5))+' *)('+...
                   rdnom+'_block_outtbptr+'+string(actt(i,2)-1)+'))'+...
                   '+'+string(ni+(j-1))+')';
      end
      if j<>ni then
       Code1($)=Code1($)+', ';
      end
    end
  end
  Code1($)=Code1($)+');'

  Code=[]

  if nc==1|~standalone then
    Code=[Call
          comments
          dcl
          '  if (flag1 == 0) {'
          a_actuator
          '  }'
          '  else if (flag1 == 1) {'
          '    if (*flag == 4 && *nport == 1) {'
          '      fprw=fopen(output,'"wt'");'
          '      if( fprw == NULL )'
          '        {'
          '          printf('"Error opening file: %s\n'", output);'
          '          return;'
          '        }'
          '    }else if (*flag == 2 /* && *nevprt>0 */) {'
          Code1
          '    }else if (*flag == 5 && *nport == 1) {'
          '      fclose(fprw);'
          '    }'
          '  }'
          '}']
  elseif nc>1 then
    S='  switch (*nport) {'
    for k=1:nc
      S=[S;
         '  case '+string(k)+' :/* Port number '+string(k)+' ----------*/'
         '  '+a_actuator
         '  break;']
    end
    S=[S;'  }']

    Code=[Code
          Call
          comments
          dcl
          '  if (flag1 == 0){'
          S
          '  }'
          '  else if (flag1 == 1) {'
          '    if (*flag == 4 && *nport == 1) {'
          '      fprw=fopen(output,'"wt'");'
          '      if( fprw == NULL ) {'
          '        printf('"Error opening file: %s\n'", output);'
          '        return;'
          '        }'
          '    }'
          '    else if (*flag == 2 /*&& *nevprt>0*/ ) {'
          Code1
          '    }'
          '    else if (*flag == 5 && *nport == 1) {'
          '      fclose(fprw);'
          '    }'
          '  }'
          '}']
  end
endfunction

//generates the scicos computational function associated with the block
//
// 21/06/07 : Alan
//
//Copyright INRIA
function Code=make_computational()
  z=cpr.state.z;
  oz=cpr.state.oz;
  outtb=cpr.state.outtb;
  tevts=cpr.state.tevts;
  evtspt=cpr.state.evtspt;

  // n'est pas utilise
  //nevts=size(evtspt,1);
  outptr=cpr.sim.outptr;
  funtyp=cpr.sim.funtyp;
  clkptr=cpr.sim.clkptr;
  ordptr=cpr.sim.ordptr;
  pointi=cpr.state.pointi;
  ztyp=cpr.sim.ztyp;
  zcptr=cpr.sim.zcptr;
  zptr=cpr.sim.zptr;
  ozptr=cpr.sim.ozptr;
  opptr=cpr.sim.opptr;
  opar=cpr.sim.opar;
  rpptr=cpr.sim.rpptr;
  ipptr=cpr.sim.ipptr;
  inpptr=cpr.sim.inpptr;
  funs=cpr.sim.funs;
  xptr=cpr.sim.xptr;
  modptr=cpr.sim.modptr;
  inplnk=cpr.sim.inplnk;
  nblk=cpr.sim.nb;
  outlnk=cpr.sim.outlnk;

  //nZ=size(z,'*')+size(outtb,'*')+clkptr($)
  //niwa=size(z,'*')+size(outtb,'*');
  //nZ=size(z,'*')+lstsize(outtb)+clkptr($)

  nZ=size(z,'*')+clkptr($); //index of work in z
  nO=lstsize(oz); //index of outtb in oz
  //niwa=size(z,'*')+lstsize(outtb);
  niwa=size(z,'*'); //index of iwa in z

  Code=['/*'+part('-',ones(1,40))+' Block Computational function */ ';
        'int  '+rdnom+'(scicos_block *block, int flag)'
        '{'
        '  double* z      = block->z;'
        '  void **ozptr   = block->ozptr;'
        '  void **oparptr = block->oparptr;'
        '  int nopar      = block->nopar;'
        '  double* x      = block->x;'
        '  double* xd     = block->xd;'
        '  double* res    = block->res;'
        '  void** u       = block->inptr;'
        '  void** y       = block->outptr;'
        '  int nevprt     = block->nevprt;'
        '  double* rpar   = block->rpar;'
        '  int nrpar      = block->nrpar;'
        '  int* ipar      = block->ipar;'
        '  int nipar      = block->nipar;'
        '  double* g      = block->g;'
        '  int* mode      = block->mode;'
        '  void **work    = block->work;'
        ''
        '  double t;'
        '  int kf, phase=get_phase_simulation();'
        '  int* reentryflag;'
        ''
        '  /*  work of blocks are catenated at the end of z*/'
        '  work = (void **)z+'+string(nZ)+';'
        '  /*  '+rdnom+'_block_outtbptr is catenated at the end of oz*/'
        '  '+rdnom+'_block_outtbptr = (void **)(ozptr+'+string(nO)+');'
        '  '+rdnom+'_iwa = (int *)(z+'+string(niwa)+');'
        ''
        '  t=get_scicos_time();'
        ''];

  Code=[Code;
        '  /*Copy inputs in the block outtb */'];
//   for i=1:size(capt,1)
//     ni=capt(i,3)*capt(i,4); // dimension of ith input
//     Code=[Code;
//           '  '+rdnom+'_block_outtbptr['+string(capt(i,2)-1+(0:ni-1)')+'] = u['+..
//                 string(capt(i,6)-1)+']['+string(0:ni-1)'+'];*/';]
//   end

  for i=1:size(capt,1)
    ni=capt(i,3)*capt(i,4); // dimension of ith input
    if capt(i,5)<>11 then
      Code=[Code;
            '  memcpy(*('+rdnom+'_block_outtbptr+'+string(capt(i,2)-1)+'),'+...
            '*(u+'+string(capt(i,6)-1)+'),'+string(ni)+'*sizeof('+mat2c_typ(capt(i,5))+'));']
    else //CAS CMPLX
      Code=[Code;
            '  memcpy(*('+rdnom+'_block_outtbptr+'+string(capt(i,2)-1)+'),'+...
            '*(u+'+string(capt(i,6)-1)+'),'+string(2*ni)+'*sizeof('+mat2c_typ(capt(i,5))+'));']
    end
  end

  Code=[Code;
        ''
        '  set_nevprt(nevprt);';
        ''
        '  if (flag != 4 && flag != 6 && flag != 5){'
        '    reentryflag=(int*) ((scicos_block *)(*block->work)+'+string(nblk)+');'
        '    if (*reentryflag==0){'
        '      *reentryflag=1;'
        '      block_'+rdnom+'=(scicos_block*) *block->work;']
  for kf=1:nblk
    nin=inpptr(kf+1)-inpptr(kf);  //* number of input ports */
    nout=outptr(kf+1)-outptr(kf); //* number of output ports */
    nx=xptr(kf+1)-xptr(kf);

    //** add comment
    txt=[get_comment('set_blk',list(funs(kf),funtyp(kf),kf));]

    Code=[Code;
          ''
          '      '+txt];

    for k=1:nin
      lprt=inplnk(inpptr(kf)-1+k);
      Code=[Code;
            '      block_'+rdnom+'['+string(kf-1)+'].inptr['+string(k-1)+']  = '+rdnom+'_block_outtbptr['+string(lprt-1)+'];']
    end
    for k=1:nout
       lprt=outlnk(outptr(kf)-1+k);
       Code=[Code
             '      block_'+rdnom+'['+string(kf-1)+'].outptr['+string(k-1)+'] = '+rdnom+'_block_outtbptr['+string(lprt-1)+'];']
    end
    Code=[Code
          '      block_'+rdnom+'['+string(kf-1)+'].z         = &(z['+string(zptr(kf)-1)+']);']
    if nx <> 0 then
      Code=[Code;
            '      block_'+rdnom+'['+string(kf-1)+'].x         = &(x['+string(xptr(kf)-1)+']);']
    else
      Code=[Code;
            '      block_'+rdnom+'['+string(kf-1)+'].x         = &(zero);'
            '      block_'+rdnom+'['+string(kf-1)+'].xd        = w;']
    end
    if (part(funs(kf),1:7) ~= 'capteur' & part(funs(kf),1:10) ~= 'actionneur' & funs(kf) ~= 'bidon') then
      //** rpar **//
      if (rpptr(kf+1)-rpptr(kf)>0) then
        Code=[Code;
              '      block_'+rdnom+'['+string(kf-1)+'].rpar      = &(rpar['+string(rpptr(kf)-1)+']);']
      end
      //** ipar **//
      if (ipptr(kf+1)-ipptr(kf)>0) then
         Code=[Code;
               '      block_'+rdnom+'['+string(kf-1)+'].ipar      = &(ipar['+string(ipptr(kf)-1)+']);']
      end
      //** opar **//
      if (opptr(kf+1)-opptr(kf)>0) then
        nopar = opptr(kf+1)-opptr(kf);
        for k=1:nopar
          Code=[Code;
                '      block_'+rdnom+'['+string(kf-1)+'].oparptr['+string(k-1)+'] = oparptr['+string(opptr(kf)-1+k-1)+'];'];
        end
      end
      //** oz **//
      if (ozptr(kf+1)-ozptr(kf)>0) then
        noz = ozptr(kf+1)-ozptr(kf);
        for k=1:noz
          Code=[Code;
                '      block_'+rdnom+'['+string(kf-1)+'].ozptr['+string(k-1)+']  = ozptr['+string(ozptr(kf)-1+k-1)+'];'];
        end
      end
    end
    Code=[Code;
          '      block_'+rdnom+'['+string(kf-1)+'].work      = (void **)(((double *)work)+'+string(kf-1)+');']
  end

  Code=[Code
        '    }'
        '  }'
        '  if (flag == 0) { /* update continuous state */']

  block_has_output=%f

  for kf=1:nblk
     nx=xptr(kf+1)-xptr(kf);
     if nx <> 0 then
          Code=[Code;
                '    block_'+rdnom+'['+string(kf-1)+'].xd=&(xd['+string(xptr(kf)-1)+']);']
     end
     if part(funs(kf),1:10) == 'actionneur' then
       block_has_output=%t
     end
  end

  Code=[Code
        '    '+rdnom+'main0(block_'+rdnom+','+rdnom+'_block_outtbptr,z,&t,phase);';
        '  }';
        '  else if (flag == 1) { /* update outputs */';
        '    '+rdnom+'main1(block_'+rdnom+','+rdnom+'_block_outtbptr,z,&t);';
        '  }else if (flag == 2 && nevprt>0) { /* update discrete states */']

  if block_has_output then
    Code=[Code;
          '    '+rdnom+'main2(block_'+rdnom+','+rdnom+'_block_outtbptr,z,&t);']
  else
     Code=[Code;
           '    /* exception block */';
           '    '+rdnom+'main1(block_'+rdnom+','+rdnom+'_block_outtbptr,z,&t);';
           '    '+rdnom+'main2(block_'+rdnom+','+rdnom+'_block_outtbptr,z,&t);']
     warning("Block has no output")
  end

  Code=[Code;
        '  }'
        '  else if (flag == 4) { /* initialisation */'
        '    if ((*block->work=scicos_malloc(sizeof(scicos_block)*'+string(nblk)+'+sizeof(int)))== NULL) return 0;';
        '    reentryflag=(int*) ((scicos_block *)(*block->work)+'+string(nblk)+');'
        '    *reentryflag=0;'
        '    block_'+rdnom+'=(scicos_block*) *block->work;'];

  for kf=1:nblk
    nin=inpptr(kf+1)-inpptr(kf);  //* number of input ports */
    nout=outptr(kf+1)-outptr(kf); //* number of output ports */
    nx=xptr(kf+1)-xptr(kf);

    //** add comment
    txt=[get_comment('set_blk',list(funs(kf),funtyp(kf),kf));]

    Code=[Code;
          ''
          '    '+txt];

    Code=[Code;
          '    block_'+rdnom+'['+string(kf-1)+'].type   = '+string(funtyp(kf))+';';
          '    block_'+rdnom+'['+string(kf-1)+'].ztyp   = '+string(ztyp(kf))+';';
          '    block_'+rdnom+'['+string(kf-1)+'].ng     = '+string(zcptr(kf+1)-zcptr(kf))+';']

    if nx <> 0 then
      Code=[Code;
            '    block_'+rdnom+'['+string(kf-1)+'].nx     = '+string(nx)+';';
            '    block_'+rdnom+'['+string(kf-1)+'].x      = &(x['+string(xptr(kf)-1)+']);']
    else
      Code=[Code;
            '    block_'+rdnom+'['+string(kf-1)+'].nx     = 1;';
            '    block_'+rdnom+'['+string(kf-1)+'].x      = &(zero);'
            '    block_'+rdnom+'['+string(kf-1)+'].xd     = w;']
    end

    Code=[Code;
          '    block_'+rdnom+'['+string(kf-1)+'].nz     = '+string(zptr(kf+1)-zptr(kf))+';';
          '    block_'+rdnom+'['+string(kf-1)+'].noz    = '+string(ozptr(kf+1)-ozptr(kf))+';';
          '    block_'+rdnom+'['+string(kf-1)+'].nrpar  = '+string(rpptr(kf+1)-rpptr(kf))+';';
          '    block_'+rdnom+'['+string(kf-1)+'].nopar  = '+string(opptr(kf+1)-opptr(kf))+';';
          '    block_'+rdnom+'['+string(kf-1)+'].nipar  = '+string(ipptr(kf+1)-ipptr(kf))+';'
          '    block_'+rdnom+'['+string(kf-1)+'].nin    = '+string(inpptr(kf+1)-inpptr(kf))+';';
          '    block_'+rdnom+'['+string(kf-1)+'].nout   = '+string(outptr(kf+1)-outptr(kf))+';';
          '    block_'+rdnom+'['+string(kf-1)+'].nevout = '+string(clkptr(kf+1)-clkptr(kf))+';';
          '    block_'+rdnom+'['+string(kf-1)+'].nmode  = '+string(modptr(kf+1)-modptr(kf))+';'];

    Code=[Code;
          '    if ((block_'+rdnom+'['+string(kf-1)+'].evout  = calloc(block_'+rdnom+'['+string(kf-1)+'].nevout,sizeof(double)))== NULL) return 0;'];

    //***************************** input port *****************************//
    //** alloc insz/inptr **//
    Code=[Code;
          '    if ((block_'+rdnom+'['+string(kf-1)+'].insz   = malloc(3*sizeof(int)*block_'+rdnom+'['+string(kf-1)+'].nin))== NULL) return 0;';
          '    if ((block_'+rdnom+'['+string(kf-1)+'].inptr  = malloc(sizeof(void *)*block_'+rdnom+'['+string(kf-1)+'].nin))== NULL) return 0;'];

    //** inptr **//
    for k=1:nin
       lprt=inplnk(inpptr(kf)-1+k);
       Code=[Code
             '    block_'+rdnom+'['+string(kf-1)+'].inptr['+string(k-1)+']  = '+rdnom+'_block_outtbptr['+string(lprt-1)+'];']
    end

    //** 1st dim **//
    for k=1:nin
       lprt=inplnk(inpptr(kf)-1+k);
       Code=[Code
             '    block_'+rdnom+'['+string(kf-1)+'].insz['+string((k-1))+']   = '+string(size(outtb(lprt),1))+';']
    end

    //** 2dn dim **//
    for k=1:nin
       lprt=inplnk(inpptr(kf)-1+k);
       Code=[Code
             '    block_'+rdnom+'['+string(kf-1)+'].insz['+string((k-1)+nin)+']   = '+string(size(outtb(lprt),2))+';']
    end

    //** typ **//
    for k=1:nin
       lprt=inplnk(inpptr(kf)-1+k);
       Code=[Code
             '    block_'+rdnom+'['+string(kf-1)+'].insz['+string((k-1)+2*nin)+']   = '+mat2scs_c_typ(outtb(lprt))+';'];
    end
    //**********************************************************************//

    //***************************** output port *****************************//
    //** alloc outsz/outptr **//
    Code=[Code
          '    if ((block_'+rdnom+'['+string(kf-1)+'].outsz  = malloc(3*sizeof(int)*block_'+rdnom+'['+string(kf-1)+'].nout))== NULL) return 0;';
          '    if ((block_'+rdnom+'['+string(kf-1)+'].outptr = malloc(sizeof(void*)*block_'+rdnom+'['+string(kf-1)+'].nout))== NULL) return 0;'];

    //** outptr **//
    for k=1:nout
       lprt=outlnk(outptr(kf)-1+k);
       Code=[Code
             '    block_'+rdnom+'['+string(kf-1)+'].outptr['+string(k-1)+'] = '+rdnom+'_block_outtbptr['+string(lprt-1)+'];']
    end

    //** 1st dim **//
    for k=1:nout
       lprt=outlnk(outptr(kf)-1+k);
       Code=[Code
             '    block_'+rdnom+'['+string(kf-1)+'].outsz['+string((k-1))+']  = '+string(size(outtb(lprt),1))+';']
    end

    //** 2dn dim **//
    for k=1:nout
       lprt=outlnk(outptr(kf)-1+k);
       Code=[Code
             '    block_'+rdnom+'['+string(kf-1)+'].outsz['+string((k-1)+nout)+']  = '+string(size(outtb(lprt),2))+';']
    end

    //** typ **//
    for k=1:nout
       lprt=outlnk(outptr(kf)-1+k);
       Code=[Code
             '    block_'+rdnom+'['+string(kf-1)+'].outsz['+string((k-1)+2*nout)+']  = '+mat2scs_c_typ(outtb(lprt))+';'];
    end
    //**********************************************************************//

    Code=[Code
          '    block_'+rdnom+'['+string(kf-1)+'].z         = &(z['+string(zptr(kf)-1)+']);']

    //***************************** object state *****************************//
    if (ozptr(kf+1)-ozptr(kf)>0) then
      noz = ozptr(kf+1)-ozptr(kf);
      Code=[Code
            '    if ((block_'+rdnom+'['+string(kf-1)+'].ozptr = malloc(sizeof(void *)*block_'+rdnom+'['+string(kf-1)+'].noz))== NULL) return 0;';
            '    if ((block_'+rdnom+'['+string(kf-1)+'].ozsz  = malloc(2*sizeof(int)*block_'+rdnom+'['+string(kf-1)+'].noz))== NULL) return 0;';
            '    if ((block_'+rdnom+'['+string(kf-1)+'].oztyp = malloc(sizeof(int)*block_'+rdnom+'['+string(kf-1)+'].noz))== NULL) return 0;';]

      //** ozptr **//
      for k=1:noz
        Code=[Code;
              '    block_'+rdnom+'['+string(kf-1)+'].ozptr['+string(k-1)+']  = ozptr['+string(ozptr(kf)-1+k-1)+'];'];
      end

      //** 1st dim **//
      for k=1:noz
        Code=[Code;
              '    block_'+rdnom+'['+string(kf-1)+'].ozsz['+string(k-1)+']   = '+string(size(oz(ozptr(kf)-1+k),1))+';'];
      end

      //** 2nd dim **//
      for k=1:noz
        Code=[Code;
              '    block_'+rdnom+'['+string(kf-1)+'].ozsz['+string(k-1)+']   = '+string(size(oz(ozptr(kf)-1+k),2))+';'];
      end

      //** typ **//
      for k=1:noz
        Code=[Code;
              '    block_'+rdnom+'['+string(kf-1)+'].oztyp['+string(k-1)+']  = '+mat2scs_c_typ(oz(ozptr(kf)-1+k))+';'];
      end
    end
    //************************************************************************//

    if (part(funs(kf),1:7) ~= 'capteur' & part(funs(kf),1:10) ~= 'actionneur' & funs(kf) ~= 'bidon') then
      if (rpptr(kf+1)-rpptr(kf)>0) then
        Code=[Code;
              '    block_'+rdnom+'['+string(kf-1)+'].rpar      = &(rpar['+string(rpptr(kf)-1)+']);']
      end
      if (ipptr(kf+1)-ipptr(kf)>0) then
        Code=[Code;
              '    block_'+rdnom+'['+string(kf-1)+'].ipar      = &(ipar['+string(ipptr(kf)-1)+']);'] 
      end
      //** opar
      if (opptr(kf+1)-opptr(kf)>0) then
        Code=[Code;
              '    if ((block_'+rdnom+'['+string(kf-1)+'].oparptr = malloc(sizeof(void *)*block_'+rdnom+'['+string(kf-1)+'].nopar))== NULL) return 0;';
              '    if ((block_'+rdnom+'['+string(kf-1)+'].oparsz  = malloc(2*sizeof(int)*block_'+rdnom+'['+string(kf-1)+'].nopar))== NULL) return 0;';
              '    if ((block_'+rdnom+'['+string(kf-1)+'].opartyp = malloc(sizeof(int)*block_'+rdnom+'['+string(kf-1)+'].nopar))== NULL) return 0;';
              ]
        nopar = opptr(kf+1)-opptr(kf);
        //** oparptr **//
        for k=1:nopar
          Code=[Code;
                '    block_'+rdnom+'['+string(kf-1)+'].oparptr['+string(k-1)+']  = oparptr['+string(opptr(kf)-1+k-1)+'];'];
        end
        //** 1st dim **//
        for k=1:nopar
          Code=[Code;
                '    block_'+rdnom+'['+string(kf-1)+'].oparsz['+string(k-1)+']   = '+string(size(opar(opptr(kf)-1+k),1))+';'];
        end
        //** 2dn dim **//
        for k=1:nopar
          Code=[Code;
                '    block_'+rdnom+'['+string(kf-1)+'].oparsz['+string(nopar+(k-1))+']   = '+string(size(opar(opptr(kf)-1+k),2))+';'];
        end
        //** typ **//
        for k=1:nopar
          Code=[Code;
                '    block_'+rdnom+'['+string(kf-1)+'].opartyp['+string(k-1)+']  = '+mat2scs_c_typ(opar(opptr(kf)-1+k))+';'];
        end
      end
    end

    Code=[Code;
          '    block_'+rdnom+'['+string(kf-1)+'].work      = (void **)(((double *)work)+'+string(kf-1)+');'
          '    block_'+rdnom+'['+string(kf-1)+'].nevprt    = nevprt;']

  end //for kf=1:nblk

  Code=[Code;
        '';
        '    /* call init function */'
        '    '+rdnom+'_init(block_'+rdnom+','+rdnom+'_block_outtbptr,z,&t);';
        '  }';
        '  else if (flag == 5) { /* ending */';
        '    block_'+rdnom+'=*block->work;']
  for kf=1:nblk
     nin=inpptr(kf+1)-inpptr(kf);  //* number of input ports */
     nout=outptr(kf+1)-outptr(kf); //* number of output ports */

     //** add comment
     txt=[get_comment('set_blk',list(funs(kf),funtyp(kf),kf));]

     Code=[Code;
           ''
           '    '+txt];

     for k=1:nin
        lprt=inplnk(inpptr(kf)-1+k);
         Code=[Code;
               '    block_'+rdnom+'['+string(kf-1)+'].inptr['+string(k-1)+']  = '+rdnom+'_block_outtbptr['+string(lprt-1)+'];']
     end
     for k=1:nout
        lprt=outlnk(outptr(kf)-1+k);
        Code=[Code
              '    block_'+rdnom+'['+string(kf-1)+'].outptr['+string(k-1)+'] = '+rdnom+'_block_outtbptr['+string(lprt-1)+'];']
     end
     Code=[Code
           '    block_'+rdnom+'['+string(kf-1)+'].z=&(z['+string(zptr(kf)-1)+']);']
     if (part(funs(kf),1:7) ~= 'capteur' & part(funs(kf),1:10) ~= 'actionneur' & funs(kf) ~= 'bidon') then
       //** rpar **//
       if (rpptr(kf+1)-rpptr(kf)>0) then
         Code=[Code;
               '    block_'+rdnom+'['+string(kf-1)+'].rpar=&(rpar['+string(rpptr(kf)-1)+']);']
       end
       //** ipar **//
       if (ipptr(kf+1)-ipptr(kf)>0) then
         Code=[Code;
               '    block_'+rdnom+'['+string(kf-1)+'].ipar=&(ipar['+string(ipptr(kf)-1)+']);'] 
       end
       //** opar **//
       if (opptr(kf+1)-opptr(kf)>0) then
         nopar = opptr(kf+1)-opptr(kf);
         for k=1:nopar
           Code=[Code;
                 '    block_'+rdnom+'['+string(kf-1)+'].oparptr['+string(k-1)+']  = oparptr['+string(opptr(kf)-1+k-1)+'];'];
         end
       end
       //** oz **//
       if (ozptr(kf+1)-ozptr(kf)>0) then
         noz = ozptr(kf+1)-ozptr(kf);
         for k=1:noz
           Code=[Code;
                 '    block_'+rdnom+'['+string(kf-1)+'].ozptr['+string(k-1)+']  = ozptr['+string(ozptr(kf)-1+k-1)+'];'];
         end
       end
     end

     Code=[Code;
           '    block_'+rdnom+'['+string(kf-1)+'].work=(void **)(((double *)work)+'+string(kf-1)+');']
  end

  Code=[Code
        ''
        '    /* call end function */'
        '    '+rdnom+'_end(block_'+rdnom+','+rdnom+'_block_outtbptr,z,&t);'
        '']
  Code=[Code;
        '    for (kf = 0; kf < '+string(nblk)+'; ++kf) {'
        '      if (block_'+rdnom+'[kf].insz!=NULL) {'
        '        free(block_'+rdnom+'[kf].insz);'
        '      }else {'
        '        break;'
        '      }'
        '      if (block_'+rdnom+'[kf].outsz!=NULL){'
        '        free(block_'+rdnom+'[kf].outsz);'
        '      }else {'
        '        break;'
        '      }'
        '      if (block_'+rdnom+'[kf].nopar!=0){'
        '        if (block_'+rdnom+'[kf].oparptr!=NULL){'
        '          free(block_'+rdnom+'[kf].oparptr);'
        '        }else {'
        '          break;'
        '        }'
        '        if (block_'+rdnom+'[kf].oparsz!=NULL){'
        '          free(block_'+rdnom+'[kf].oparsz);'
        '        }else {'
        '          break;'
        '        }'
        '        if (block_'+rdnom+'[kf].opartyp!=NULL){'
        '          free(block_'+rdnom+'[kf].opartyp);'
        '        }else {'
        '          break;'
        '        }'
        '      }'
        '      if (block_'+rdnom+'[kf].noz!=0){'
        '        if (block_'+rdnom+'[kf].ozptr!=NULL){'
        '          free(block_'+rdnom+'[kf].ozptr);'
        '        }else {'
        '          break;'
        '        }'
        '        if (block_'+rdnom+'[kf].ozsz!=NULL){'
        '          free(block_'+rdnom+'[kf].ozsz);'
        '        }else {'
        '          break;'
        '        }'
        '        if (block_'+rdnom+'[kf].oztyp!=NULL){'
        '          free(block_'+rdnom+'[kf].oztyp);'
        '        }else {'
        '          break;'
        '        }'
        '      }'
        '      if (block_'+rdnom+'[kf].evout!=NULL){'
        '        free(block_'+rdnom+'[kf].evout);'
        '      }else {'
        '        break;'
        '      }'
        '    }'
        '    scicos_free(block_'+rdnom+');'
        '  }'];

  ng=zcptr($)-1;
  if (ng ~= 0) then
    Code=[Code;
          '';
          '  else if (flag == 9) { /* update zero crossing surfaces */';]

    for kf=1:nblk
       if zcptr(kf+1)-zcptr(kf) <> 0 then 
          Code=[Code;
                '    block_'+rdnom+'['+string(kf-1)+'].g=&(g['+string(zcptr(kf)-1)+']);']
       end
       if modptr(kf+1)-modptr(kf) <> 0 then 
         Code=[Code;
               '    block_'+rdnom+'['+string(kf-1)+'].mode=&(mode['+string(modptr(kf)-1)+']);']
       end
    end

    Code($+1)=cformatline('    '+rdnom+'zdoit(g,&t,'+...
                          rdnom+'_block_outtbptr, '+..
                          '(int *)(z+'+string(size(z,1))+'), phase);',70);
    Code($+1)='  }';
  end

  //copy outputs
//   for i=1:size(actt,1)
//     ni=actt(i,3)*actt(i,4) // dimension of ith output
//     Code=[Code;
//           '/*  y['+string(actt(i,6)-1)+']['+string(0:ni-1)'+'] =  '+rdnom+'_block_outtb['+..
//                              string(actt(i,2)-1+(0:ni-1)')+'];*/']
//   end

  for i=1:size(actt,1)
    ni=actt(i,3)*actt(i,4); // dimension of ith input
    if actt(i,5)<>11 then
      Code=[Code;
            '  memcpy(*(y+'+string(actt(i,6)-1)+'),'+...
            '*('+rdnom+'_block_outtbptr+'+string(actt(i,2)-1)+'),'+string(ni)+'*sizeof('+mat2c_typ(actt(i,5))+'));']
    else //CAS CMPLX
      Code=[Code;
            '  memcpy(*(y+'+string(actt(i,6)-1)+'),'+...
            '*('+rdnom+'_block_outtbptr+'+string(actt(i,2)-1)+'),'+string(2*ni)+'*sizeof('+mat2c_typ(actt(i,5))+'));']
    end
  end

  //Assemble the code
  Code=[Code;
        '  return 0;'
        ''
        '} /* '+rdnom+' */']

endfunction

//generates  procedure declarations
//Author : Rachid Djenidi
//
//Copyright INRIA

function Code=make_decl()
  Code=['/* SCILAB Computational function  */';
        '/*     Copyright INRIA */';
        '/*     Generated by Code_Generation toolbox of Scicos with '+ ..
        getversion()+' */';
        '/*     date : '+date(getdate())+' */';
        ''
        '/* ---- Headers ---- */'
        '#include <stdio.h>';
        '#include <memory.h>';
        '#include <string.h>';
        '#include '"'+SCI+'/routines/machine.h'"';
        '#include '"'+SCI+'/routines/os_specific/link.h'"';
        '#include '"'+SCI+'/routines/scicos/scicos.h'"';
        '';
        '/* ---- Internals functions declaration ---- */'
        cformatline('void '+rdnom+'main0(scicos_block *, void **, double *, double *, int);',70);
        '';
        cformatline('void '+rdnom+'main1(scicos_block *, void **, double *, double *);',70);
        '';
        cformatline('void '+rdnom+'main2(scicos_block *, void **, double *, double *);',70);
        '';
        cformatline('void '+rdnom+'_init(scicos_block *, void **, double *, double *);',70);
        '';
        cformatline('void '+rdnom+'_end(scicos_block *, void **, double *, double *);',70);
        '';
        'int '+rdnom+'();';
        '';
        cformatline('int '+rdnom+'ddoit1(double *, double *, void **, int *);',70);
        '';
        cformatline('int '+rdnom+'cdoit(double *, double *, void **);',70);
        '';
        cformatline('int '+rdnom+'odoit(double *, void **, int*, int);',70);
        '';
        cformatline('int '+rdnom+'zdoit(double *, double *, void **, int*, int);',70);
        '';
        cformatline('int '+rdnom+'ddoit2(double *, double *, void **, int *);',70);
        '';
        cformatline('int '+rdnom+'edoit1(double *, void **, int *, int *);',70);
        '';
        cformatline('int '+rdnom+'ozdoit(double *, void **, int *, int *,int);',70);
        '';
        cformatline('int '+rdnom+'doit(double *, double *, void **);',70);
        '';
        cformatline('int '+rdnom+'_initi(double *, void **, int *);',70);
        '';
        cformatline('int '+rdnom+'_endi(double *, void **, int *);',70);
        '';
        cformatline('int '+rdnom+'_outtb(double *, void **, int *);',70);
        '';
        cformatline('int '+rdnom+'_putevs(double *, int *, int *);',70);
        '';
        '/* ---- Block computational functions declaration ---- */'
        ''];
endfunction

//generates  procedure declarations
//Author : Rachid Djenidi
//
//Copyright INRIA
function Code=make_decl_standalone()
  x=cpr.state.x
  Code=['/* Code prototype for standalone use  */';
        '/*     Generated by Code_Generation toolbox of Scicos with '+ ..
        getversion()+' */';
        '/*     date : '+date(getdate())+' */';
        ''
        '/* To learn how to use the standalone code, type '"./standalone -h'" */'
        ''
        '/* ---- Headers ---- */'
        '#include <stdio.h>';
        '#include <stdlib.h>';
        '#include <math.h>';
        '#include <string.h>';
        '#include <memory.h>';
        '#include '"scicos_block4.h'"';
        '#include '"machine.h'"';
        '';
        '/* ---- Internals functions declaration ---- */'
        cformatline('void '+rdnom+'main0(scicos_block *, void **, double *, double *, int);',70);
        '';
        cformatline('void '+rdnom+'main1(scicos_block *, void **, double *, double *);',70);
        '';
        cformatline('void '+rdnom+'main2(scicos_block *, void **, double *, double *);',70);
        '';
        cformatline('void '+rdnom+'_init(scicos_block *, void **, double *, double *);',70);
        '';
        cformatline('void '+rdnom+'_end(scicos_block *, void **, double *, double *);',70);
        ''
        cformatline('int '+rdnom+'_sim(double, double, double, int);',70);
        ''
        cformatline('int '+rdnom+'ddoit1(double *, double *, void **, int *);',70);
        ''
        cformatline('int '+rdnom+'cdoit(double *, double *, void **);',70);
        '';]
        //' ';
        //cformatline('int '+rdnom+'doit(double *, double *, double *);',70);
        //' ';]
  if x <> [] then
    Code=[Code
          cformatline('int '+rdnom+'odoit(double *, void **, int* , int , double *, double *, double *);',70);
          '';]
  else
    Code=[Code
          cformatline('int '+rdnom+'odoit(double *, void **, int* , int );',70);
          '';]
  end
  //Code=[Code
        //cformatline('int '+rdnom+'zdoit(double *, double *, double *, int* , int );',70);
        //' ';
  Code=[Code
        cformatline('int '+rdnom+'ddoit2(double *, double *, void **, int *);',70);
        '';
        cformatline('int '+rdnom+'edoit1(double *, void **, int *, int *);',70);
        '';
        cformatline('int '+rdnom+'ozdoit(double *, void **, int *, int *,int);',70);
        '';
        cformatline('int '+rdnom+'_initi(double *, void **, int *);',70);
        '';
        cformatline('int '+rdnom+'_endi(double *, void **, int *);',70);
        '';
        cformatline('int '+rdnom+'_outtb(double *, void **, int *);',70);
        '';
        cformatline('int '+rdnom+'_putevs(double *, int *, int *);',70);
        ''
        cformatline('void '+rdnom+'_events(int *nevprt, double *t);',70)
        ''
        'static void set_nevprt(int nevprt);']

  if (x <>[]) then
    Code=[Code
          '/* Code prototype for standalone use  */';
          'int C2F('+rdnom+'simblk)(double *, double *, double *);'
          'extern  integer C2F(dset)();'
          'int ode1();'
          'int ode2();'
          'int ode4();']
  end
endfunction

//Author : Rachid Djenidi
//Copyright INRIA
function Code=make_end(stalone)
  Code=['/*'+part('-',ones(1,40))+' end */';
        'void '
         cformatline(rdnom+'_end(scicos_block *block_'+rdnom+', void ** outtbptr,double *z, double *t)',70);
        '{'];

  if ~stalone then
    Code=[Code
          cformatline('  '+rdnom+'_endi(t, '+..
                      'outtbptr, '+..
                      '(int *)(z+'+string(size(z,1))+'));',70);
                      '}'];
  else
    Code=[Code
          cformatline('  '+rdnom+'_endi(t, '+..
                      'outtbptr, '+..
                      '(int *)(z+'+string(size(z,1)+lstsize(outtb))+'));',70);
                      '}'];
  end
endfunction

//Author : Rachid Djenidi
//
//Copyright INRIA
function Code=make_init(stalone)
  Code=['/*'+part('-',ones(1,40))+' init */ ';
        'void '
         cformatline(rdnom+'_init(scicos_block *block_'+rdnom+', void ** outtbptr,double *z, double *t)',70);
        '{'];

  if ~stalone then
    Code=[Code;
          '  /*Block initializations*/'
           cformatline('  '+rdnom+'_initi(t, '+..
                       'outtbptr, '+..
                       '(int *)(z+'+string(size(z,1))+'));',70);
          '  /*Constants propagation*/'
           cformatline('  '+rdnom+'_outtb(t, '+..
                       'outtbptr, '+..
                       '(int *)(z+'+string(size(z,1))+'));',70);
         '}'];
  else
    Code=[Code;
          '  /*Block initializations*/'
           cformatline('  '+rdnom+'_initi(t, '+..
                       'outtbptr, '+..
                       '(int *)(z+'+string(size(z,1)+lstsize(outtb))+'));',70);
          '  /*Constants propagation*/'
           cformatline('  '+rdnom+'_outtb(t, '+..
                       'outtbptr, '+..
                       '(int *)(z+'+string(size(z,1)+lstsize(outtb))+'));',70);
         '}'];
  end
endfunction

//generate code for flag=0 case
//Copyright INRIA
function Code=make_main0()
  Code=['/*'+part('-',ones(1,40))+' main0 */';
         cformatline('void '+rdnom+'main0(scicos_block *block_'+rdnom+',void ** outtbptr,double *z, double *t, int phase)',70);
        '{'];

  Code($+1)=cformatline('  '+rdnom+'odoit(t,'+..
                        'outtbptr, '+..
                        '(int *)(z+'+string(size(z,1))+'), phase);',70);
  Code($+1)='}'
endfunction

//generate code for flag=1 case
//Author : Rachid Djenidi
//
//Copyright INRIA
function Code=make_main1(stalone)
  Code=['/*'+part('-',ones(1,40))+' main1 */';
        'void '
         cformatline(rdnom+'main1(scicos_block *block_'+rdnom+',void ** outtbptr,double *z, double *t)',70);
        '{'];

  //RN update of 27/06/07
  if ~(ALWAYS_ACTIVE&szclkIN==[]) then
    if ~stalone then
      Code=[Code;
            cformatline('  '+ rdnom+'ddoit1(z, t, '+..
                        'outtbptr, '+..
                        '(int *)(z+'+string(size(z,1))+'));',70);
           ];
    else
      Code=[Code;
            cformatline('  '+ rdnom+'ddoit1(z, t, '+..
                        'outtbptr, '+..
                        '(int *)(z+'+string(size(z,1)+lstsize(outtb))+'));',70);
           ];
    end
  else
    Code=[Code;
          cformatline('  '+ rdnom+'cdoit(z, t, '+..
                      'outtbptr);',70);
         ];
  end
//   Code($+1)=cformatline('  '+rdnom+'ddoit1(z, t, '+..
//                         'outtbptr, '+..
//                         '(int *)(z+'+string(size(z,1))+'));',70);

  Code($+1)='}'
endfunction

//generate code for flag=2 case
//
//Author : Rachid Djenidi
//
//Copyright INRIA
function Code=make_main2(stalone)
  Code=['/*'+part('-',ones(1,40))+' main2 */';
        'void '
        cformatline(rdnom+'main2(scicos_block *block_'+rdnom+', void ** outtbptr,double *z, double *t)',70);
        '{'];

  if ~stalone then
    Code=[Code;
          cformatline('  '+rdnom+'ddoit2(z, t, '+..
                      'outtbptr, '+..
                      '(int *)(z+'+string(size(z,1))+'));',70);
          '}'];
  else
    Code=[Code;
          cformatline('  '+rdnom+'ddoit2(z, t, '+..
                      'outtbptr, '+..
                      '(int *)(z+'+string(size(z,1)+lstsize(outtb))+'));',70);
          '}'];
  end
endfunction

//generates skeleton of external world events handling function
function Code=make_outevents()
  z='0'
  if szclkIN==[] then
    newszclkIN=0;
  else
    newszclkIN=szclkIN;
  end

  Code=['/*'+part('-',ones(1,40))+'  External events handling function */';
         'void '+rdnom+'_events(int *nevprt,double *t)';
         '{'
         '/*  set next event time and associated events ports'
         ' *  nevprt has binary expression b1..b'+string(newszclkIN)+' where bi is a bit'
         ' *  bi is set to 1 if an activation is received by port i. Note that'
         ' *  more than one activation can be received simultaneously'
         ' *  Caution: at least one bi should be equal to one */'
         '']

  if (newszclkIN <> 0) then
    Code=[Code;
          '    int i,p,b[]={'+strcat(z(ones(1,newszclkIN)),',')+'};'
          ''
          '/* this is an example for the activation of events ports */'
          '    b[0]=1;']

    if newszclkIN>1 then
      for bb=2:newszclkIN
        Code($+1)='    b['+string(bb-1)+']=1;'
      end
    end

    Code=[Code;
          ''
          '/* definition of the step time  */'
          '    *t = *t + 0.1;'
          ''
          '/* External events handling process */'
          '    *nevprt=0;p=1;'
          '    for (i=0;i<'+string(newszclkIN)+';i++) {'
          '      *nevprt=*nevprt+b[i]*p;'
          '      p=p*2;'
          '    }'
          '}']
  else
    Code=[Code;
          '';
          '/* definition of the step time  */'
          '    *t = *t + 0.1;'
          '}']
  end
endfunction

function Code=make_putevs()
  //RN
  Code=['int '+rdnom+'_putevs(t, evtnb, ierr)';
        '  double *t;'
        '  integer *evtnb, *ierr;'
        '{'
        '  *ierr = 0;'
        '  if (evtspt[*evtnb-1] != -1) {'
        '    *ierr = 1;'
        '    return 0;'
        '  }'
        ' else {'
        '    evtspt[*evtnb-1] = 0;'
        '    tevts[*evtnb-1] = *t;'
        '  }'
        '  if (pointi == 0) {'
        '    pointi = *evtnb;'
        '    return 0;'
        '  }'
        '  evtspt[*evtnb-1] = pointi;'
        '  pointi = *evtnb;'
        '  return 0;'
        '} ']
endfunction

//Generating the routine for sensors interfacing
//
//Copyright INRIA
//
//Author : R. Djenidi, R. Nikoukhah, A. Layec
//
function Code=make_sensor(standalone)
  Call=['/*'+part('-',ones(1,40))+' Sensor */';
        'void '+rdnom+'_sensor(flag,nport,nevprt,t,y,ny1,ny2,yt,flag1)']

  comments=['     /*'
            '      * To be customized for standalone execution';
            '      * flag  : specifies the action to be done'
            '      * nport : specifies the  index of the Super Bloc'
            '      *         regular input (The input ports are numbered'
            '      *         from the top to the bottom )'
            '      * nevprt: indicates if an activation had been received'
            '      *         0 = no activation'
            '      *         1 = activation'
            '      * t     : the current time value'
            '      * y     : the vector outputs value'
            '      * ny1   : the output size 1'
            '      * ny2   : the output size 2'
            '      * yt    : the output type'
            '      * flag1 : learn mode (0 from terminal,1 from input file'
            '      */']

  dcl=['     int *flag,*nevprt,*nport;'
       '     int *ny1,*ny2,*yt,flag1;'
       ''
       '     double *t;'
       '     void *y;'
       '{'
       '  int j,k,l;'
       '  double temps;']

  if standalone then

    a_sensor=['    switch (*flag) {'
              '    case 1 : /* set the output value */'
              '      printf(""Require outputs of sensor number %d\n"", *nport);'
              '      printf(""time is: %f\n"", *t);'
              '      printf(""sizes of the sensor output is: %d,%d\n"", *ny1,*ny2);'
              '      switch (*yt) {'
              '      case 10 :'
              '        printf(""type of the sensor output is: %d (double) \n"", *yt);'
              '        puts(""Please set the sensor output values"");'
              '        for (l=0;l<*ny2;l++) {'
              '          for (k=0;k<*ny1;k++) {'
              '            printf(""y(%d,%d) : "",k,l);'
              '            scanf(""%lf"", (double *) y+(k+l*(*ny1)));'
              '          }'
              '        }'
              '        break;'
              ''
              '      case 11 :'
              '        printf(""type of the sensor output is: %d (complex) \n"", *yt);'
              '        puts(""Please set the sensor output values"");'
              '        for (l=0;l<*ny2;l++) {'
              '          for (k=0;k<*ny1;k++) {'
              '            printf(""y(%d,%d) real part : "",k,l);'
              '            scanf(""%lf"", (double *) y+(k+l*(*ny1)));'
              '            printf(""y(%d,%d) imag part : "",k,l);'
              '            scanf(""%lf"", (double *) y+((*ny1)*(*ny2)+k+l*(*ny1)));'
              '          }'
              '        }'
              '        break;'
              ''
              '      case 81 :'
              '        printf(""type of the sensor output is: %d (char) \n"", *yt);'
              '        puts(""Please set the sensor output values"");'
              '        for (l=0;l<*ny2;l++) {'
              '          for (k=0;k<*ny1;k++) {'
              '            printf(""y(%d,%d) : "",k,l);'
              '            scanf(""%i"", (char *) y+(k+l*(*ny1)));'
              '          }'
              '        }'
              '        break;'
              ''
              '      case 82 :'
              '        printf(""type of the sensor output is: %d (char) \n"", *yt);'
              '        puts(""Please set the sensor output values"");'
              '        for (l=0;l<*ny2;l++) {'
              '          for (k=0;k<*ny1;k++) {'
              '            printf(""y(%d,%d) : "",k,l);'
              '            scanf(""%hd"", (short *) y+(k+l*(*ny1)));'
              '          }'
              '        }'
              '        break;'
              ''
              '      case 84 :'
              '        printf(""type of the sensor output is: %d (long) \n"", *yt);'
              '        puts(""Please set the sensor output values"");'
              '        for (l=0;l<*ny2;l++) {'
              '          for (k=0;k<*ny1;k++) {'
              '            printf(""y(%d,%d) : "",k,l);'
              '            scanf(""%ld"", (long *) y+(k+l*(*ny1)));'
              '          }'
              '        }'
              '        break;'
              ''
              '      case 811 :'
              '        printf(""type of the sensor output is: %d (unsigned char) \n"", *yt);'
              '        puts(""Please set the sensor output values"");'
              '        for (l=0;l<*ny2;l++) {'
              '          for (k=0;k<*ny1;k++) {'
              '            printf(""y(%d,%d) : "",k,l);'
              '            scanf(""%d"", (unsigned char *) y+(k+l*(*ny1)));'
              '          }'
              '        }'
              '        break;'
              ''
              '      case 812 :'
              '        printf(""type of the sensor output is: %d (unsigned short) \n"", *yt);'
              '        puts(""Please set the sensor output values"");'
              '        for (l=0;l<*ny2;l++) {'
              '          for (k=0;k<*ny1;k++) {'
              '            printf(""y(%d,%d) : "",k,l);'
              '            scanf(""%hu"", (unsigned short *) y+(k+l*(*ny1)));'
              '          }'
              '        }'
              '        break;'
              ''
              '      case 814 :'
              '        printf(""type of the sensor output is: %d (unsigned long) \n"", *yt);'
              '        puts(""Please set the sensor output values"");'
              '        for (l=0;l<*ny2;l++) {'
              '          for (k=0;k<*ny1;k++) {'
              '            printf(""y(%d,%d) : "",k,l);'
              '            scanf(""%lu"", (unsigned long *) y+(k+l*(*ny1)));'
              '          }'
              '        }'
              '        break;'
              ''
              '      }'
              '    case 4 : /* sensor initialisation */'
              '      /* do whatever you want to initialize the sensor */'
              '      break;'
              '    case 5 : /* sensor ending */'
              '      /* do whatever you want to end the sensor */'
              '      break;'
              '    }']
  else
    a_sensor=[]
  end

  nc=size(cap,'*')

  // pour fscanf
  typ=['""%lf ']; //temps
  for i=1:nc
    typ($)=typ($)+'"" \'
    typ=[typ;'""'];
    for j=1:sum(capt(i,3)*capt(i,4))
      //typ=typ+'%f ';
      typ($)=typ($)+scs_c_n2c_fmt(capt(i,5))+' ';
    end
  end
  typ($)=typ($)+'\n"", \'
  typ=strsubst(typ,'%f','%lf');
  typ(1)='    fscanf(fprr,'+typ(1);
  bl    ='                ';
  if size(typ,1) <> 1 then
    typ(2:$) = bl+typ(2:$);
  end
  //Code1=['      fscanf( fprr, '"'+typ+' \n'",&temps'] 
  Code1=[typ;bl+'&temps'];
  for i=1:size(capt,1)
    ni=capt(i,3)*capt(i,4); // dimension of ith input
    Code1($)=Code1($)+',';
    Code1=[Code1;bl];
    for j=1:ni
      if capt(i,5)<>11 then
        Code1($)=Code1($)+...
                  '('+scs_c_n2c_typ(capt(i,5))+' *)('+...
                   rdnom+'_block_outtbptr+'+string(capt(i,2)-1)+')'+...
                   '+'+string(j-1)+'';
      else //CAS CMPLX
        Code1($)=Code1($)+...
                  '('+scs_c_n2c_typ(capt(i,5))+' *)('+...
                   rdnom+'_block_outtbptr+'+string(capt(i,2)-1)+')'+...
                   '+'+string((j-1))+', '+...
                  '('+scs_c_n2c_typ(capt(i,5))+' *)('+...
                   rdnom+'_block_outtbptr+'+string(capt(i,2)-1)+')'+...
                   '+'+string(ni+(j-1))+'';
      end
      if j<>ni then
       Code1($)=Code1($)+', ';
      end
    end
  end
  Code1($)=Code1($)+');'

  Code=[]
  if nc==1|~standalone then
    Code=[Code;
          Call
          comments
          dcl
          '  if (flag1 == 0) {'
          a_sensor;
          '  } '
          '  else if (flag1 == 1) {'
          '    if (*flag == 4 && *nport == 1) {'
          '      fprr=fopen(input,'"r'");'
          '      if( fprr == NULL ) {'
          '        printf('"Error opening file: %s\n'", input);'
          '        return;'
          '      }'
          '    }'
          '    else if (*flag == 1) {'
          Code1
          '    }'
          '    else if (*flag == 5 && *nport == 1) {'
          '      fclose(fprr);'
          '    }'
          '  }'
          '}'];

  elseif nc>1 then
    S='  switch (*nport) {'
    for k=1:nc
      S=[S;
         '  case '+string(k)+' : /* Port number '+string(k)+' ----------*/'
         '  '+a_sensor
         '  break;']
    end
    S=[S;'  }']

    Code=[Code
          Call
          comments
          dcl
          '  if (flag1 == 0) {'
          S
          '  }'
          '  else if (flag1 == 1){'
          '    if (*flag == 4 && *nport == 1) {'
          '      fprr=fopen(input,'"r'");'
          '      if( fprr == NULL ) {'
          '        printf('"Error opening file: %s\n'", input);'
          '        return ;'
          '      }'
          '    }'
          '    else if (*flag == 1) {'
          Code1
          '    }'
          '    else if (*flag == 5 && *nport == 1) {'
          '      fclose(fprr);'
          '    }'
          '  }'
          '}']
  end
endfunction

//generates code of the standalone simulation procedure
//
//Copyright INRIA
//
// rmq : Les fonction zdoit, cdoit, doit sont pas utilisé pour le moment
function Code=make_standalone()

  x=cpr.state.x;
  nX=size(x,'*');
  modptr=cpr.sim.modptr;

  opar=cpr.sim.opar;

  oz=cpr.state.oz;

  work=zeros(nblk,1)
  iwa=zeros(clkptr($),1)

  Z=[z;zeros(lstsize(outtb),1);iwa;work]';

  nztotal=size(z,1);
  niwa=size(z,'*')+lstsize(outtb);
  nZ=size(z,'*')+lstsize(outtb)+clkptr($);

  txt1=mgetl(SCI+'/routines/machine.h');
  mputl(txt1,rpat+'/machine.h');
  txt2=mgetl(SCI+'/routines/scicos/scicos_block4.h');
  mputl(txt2,rpat+'/scicos_block4.h');

  //Generates simulation routine for standalone simulation
  Code=[''
        'static void usage();'
        ''
        '/*Main program */'
        'int main(int argc, char *argv[])'
        '{'
        '  double tf=30,dt=0.1,h=0.001;';
        '  int solver=3;'
        '  char * progname = argv[0];'
        '  int c;' 
        '  strcpy(input,'"'");'
        '  strcpy(output,'"'");'
        '  while ((c = getopt(argc , argv, '"i:o:d:t:e:s:hv'")) != -1)'
        '    switch (c) {'
        '    case ''i'':'
        '      strcpy(input,argv[optind-1]);'
        '      break;'
        '    case ''o'':'
        '      strcpy(output,argv[optind-1]);'
        '      break;'
        '    case ''d'':'
        '      strcpy(s,argv[optind-1]);'
        '      dt=strtod(s,p);'
        '      break;'
        '    case ''t'':'
        '      strcpy(sf,argv[optind-1]);'
        '      tf=strtod(sf,p);'
        '      break;'
        '    case ''e'':'
        '      strcpy(se,argv[optind-1]);'
        '      h=strtod(se,p);'
        '      break;'
        '    case ''s'':'
        '      strcpy(ss,argv[optind-1]);'
        '      solver=(int) strtod(ss,p);'
        '      break;'
        '    case ''h'':'
        '      usage(progname);'
        '      printf('"Options : \n'");'
        '      printf('"         -h for the help  \n'");'
        '      printf('"         -v for printing the Scilab Version \n'");'
        '      printf('"         -i for input file name, by default is Terminal \n'");'
        '      printf('"         -o for output file name, by default is Terminal \n'");'
        '      printf('"         -d for the clock period, by default is 0.1 \n'");'
        '      printf('"         -t for the final time, by default is 30 \n'");'
        '      printf('"         -e for the solvers step size, by default is 0.001 \n'");'
        '      printf('"         -s integer parameter for select the numerical solver : \n'");'
        '      printf('"            1 for Euler''s method \n'");'
        '      printf('"            2 for Heun''s method \n'");'
        '      printf('"            3 (default value) for the Fourth-Order Runge-Kutta (RK4) Formula \n'");'
        '      return 0;'
        '      break;'
        '    case ''v'':'
        '      printf('" Generated by Code_Generation toolbox of Scicos with '+getversion()+' version \n'");'
        '      return 0;'
        '      break;'
        '    case ''?'':'
        '      errflg++;'
        '      break;'
        '    }'
        '    if (errflg){'
        '      usage(progname);'
        '      return 0;'
        '    }'
        ' '
        '  '+rdnom+'_sim(tf,dt,h,solver);';
        '  return 0;'
        '}'
        ''
        'static void'
        'usage(prog) char *prog;{'
        '  fprintf(stderr, '"Usage: %s [-h] [-v] [-i arg] [-o arg] [-d arg] [-t arg] [-e arg] [-s arg]\n'", prog);'
        '}'
        ''
        '/*'+part('-',ones(1,40))+'  External simulation function */ ';
        'int '
        rdnom+'_sim(tf,dt,h,solver)';
        ' '
        '     double tf,dt,h; ';
        '     int solver;';
        '{'
        '  double t;'
        '  int nevprt=1;'
        ''
        '  /*Initial values */';
        cformatline('  double z[]={'+strcat(string(Z),',')+'};',70);
        '  /* Note that z[]=[z_initial_condition;outtb;iwa;work]';
        cformatline('     z_initial_condition= {'+strcat(string(z),",")+'};',70);
        cformatline('     outtb= {0,1,2,3,4};',70);
        cformatline('     outtb= {0,1,2,3,4};',70);
        cformatline('     iwa= {'+strcat(string(iwa),"," )+'};',70);
        cformatline('     work= {'+strcat(string(work),"," )+'};',70);
        '  */']

  //** declaration of outtb
  Code_outtb = [];
  for i=1:lstsize(outtb)
    if mat2scs_c_nb(outtb(i)) <> 11 then
      Code_outtb = [Code_outtb;
                    cformatline('  '+mat2c_typ(outtb(i))+...
                                ' outtb_'+string(i)+'[]={'+strcat(string(outtb(i)(:)),',')+'};',70);];
    else //** cmplx test
      Code_outtb = [Code_outtb;
                    cformatline('  '+mat2c_typ(outtb(i))+...
                                ' outtb_'+string(i)+'[]={'+...
                                strcat(string([real(outtb(i)(:));imag(outtb(i)(:))]),',')+'};',70);];
    end
  end

  Code=[Code;Code_outtb];

  //** declaration of oz
  Code_oz = [];
  for i=1:lstsize(oz)
    if mat2scs_c_nb(oz(i)) <> 11 then
      Code_oz = [Code_oz;
                    cformatline('  '+mat2c_typ(oz(i))+...
                                ' oz_'+string(i)+'[]={'+strcat(string(oz(i)(:)),',')+'};',70);];
    else //** cmplx test
      Code_oz = [Code_oz;
                    cformatline('  '+mat2c_typ(oz(i))+...
                                ' oz_'+string(i)+'[]={'+...
                                strcat(string([real(oz(i)(:));imag(oz(i)(:))]),',')+'};',70);];
    end
  end

  Code=[Code;Code_oz];

  if (x <> []) then
    Code=[Code
          '  double tout;'
          '  double he=0.1;'];
  end

  Code=[Code
        '  void **work;'
        '  work = (void **)(z+'+string(nZ)+');'
        '  '+rdnom+'_block_outtbptr = (void **)(z+'+string(nztotal)+');'];

  Code_outtbptr = [];
  for i=1:lstsize(outtb)
      Code_outtbptr=[Code_outtbptr;
                     '  '+rdnom+'_block_outtbptr['+string(i-1)+'] = (void *) outtb_'+string(i)+';'];
  end

  Code=[Code;Code_outtbptr];

  Code=[Code
        '  '+rdnom+'_iwa = (int *)(z+'+string(niwa)+');']

  if size(z,1) <> 0 then
    for i=1:(length(zptr)-1)
      if zptr(i+1)-zptr(i)>0 then
        if size(corinv(i),'*')==1 then
          OO=scs_m.objs(corinv(i));
        else
          path=list('objs');
          for l=cpr.corinv(i)(1:$-1)
            path($+1)=l;path($+1)='model'
            path($+1)='rpar'
            path($+1)='objs'
          end
          path($+1)=cpr.corinv(i)($);
          OO=scs_m(path);
        end
        aaa=OO.gui
        bbb=emptystr(3,1);
        if and(aaa+bbb~= ['INPUTPORTEVTS';'OUTPUTPORTEVTS';'EVTGEN_f']) then
          Code($+1)='';
          Code($+1)='/* Routine name of block: '+strcat(string(cpr.sim.funs(i)));
          Code($+1)=' Gui name of block: '+strcat(string(OO.gui));
          //Code($+1)='/* Name block: '+strcat(string(cpr.sim.funs(i)));
          //Code($+1)='Object number in diagram: '+strcat(string(cpr.corinv(i)));
          Code($+1)='Compiled structure index: '+strcat(string(i));
          if stripblanks(OO.model.label)~=emptystr() then
            Code=[Code;cformatline('Label: '+strcat(string(OO.model.label)),70)];
          end
          if stripblanks(OO.graphics.exprs(1))~=emptystr() then
            Code=[Code;cformatline('Exprs: '+strcat(OO.graphics.exprs(1),","),70)];
          end
          if stripblanks(OO.graphics.id)~=emptystr() then
            Code=[Code;
                  cformatline('Identification: '+..
                     strcat(string(OO.graphics.id)),70)];
          end
          Code=[Code;cformatline('z= {'+strcat(string(z(zptr(i):zptr(i+1)-1)),",")+'};',70)];
          Code($+1)='*/';
        end
      end
    end
  end

  for kf=1:nblk
    nx=xptr(kf+1)-xptr(kf);       //** number of continuous state
    nin=inpptr(kf+1)-inpptr(kf);  //** number of input ports
    nout=outptr(kf+1)-outptr(kf); //** number of output ports

    //** add comment
    txt=[get_comment('set_blk',list(funs(kf),funtyp(kf),kf));]

    Code=[Code;
          ''
          '  '+txt];

    Code=[Code;
          '  block_'+rdnom+'['+string(kf-1)+'].type   = '+string(funtyp(kf))+';';
          '  block_'+rdnom+'['+string(kf-1)+'].ztyp   = '+string(ztyp(kf))+';';
          '  block_'+rdnom+'['+string(kf-1)+'].ng     = '+string(zcptr(kf+1)-zcptr(kf))+';';
          '  block_'+rdnom+'['+string(kf-1)+'].nz     = '+string(zptr(kf+1)-zptr(kf))+';';
          '  block_'+rdnom+'['+string(kf-1)+'].noz    = '+string(ozptr(kf+1)-ozptr(kf))+';';
          '  block_'+rdnom+'['+string(kf-1)+'].nrpar  = '+string(rpptr(kf+1)-rpptr(kf))+';';
          '  block_'+rdnom+'['+string(kf-1)+'].nopar  = '+string(opptr(kf+1)-opptr(kf))+';';
          '  block_'+rdnom+'['+string(kf-1)+'].nipar  = '+string(ipptr(kf+1)-ipptr(kf))+';'
          '  block_'+rdnom+'['+string(kf-1)+'].nin    = '+string(inpptr(kf+1)-inpptr(kf))+';';
          '  block_'+rdnom+'['+string(kf-1)+'].nout   = '+string(outptr(kf+1)-outptr(kf))+';';
          '  block_'+rdnom+'['+string(kf-1)+'].nevout = '+string(clkptr(kf+1)-clkptr(kf))+';';
          '  block_'+rdnom+'['+string(kf-1)+'].nmode  = '+string(modptr(kf+1)-modptr(kf))+';';]

    if nx <> 0 then
      Code=[Code;
            '  block_'+rdnom+'['+string(kf-1)+'].nx = '+string(nx)+';';
            '  block_'+rdnom+'['+string(kf-1)+'].x  = &(x['+string(xptr(kf)-1)+']);'
            '  block_'+rdnom+'['+string(kf-1)+'].xd = &(xd['+string(xptr(kf)-1)+']);']
    end

    Code=[Code;
          '  if ((block_'+rdnom+'['+string(kf-1)+'].evout  = calloc(block_'+rdnom+'['+string(kf-1)+'].nevout,sizeof(double)))== NULL) return 0;'];

    //***************************** input port *****************************//
    //** alloc insz/inptr **//
    Code=[Code;
          '  if ((block_'+rdnom+'['+string(kf-1)+'].inptr  = malloc(sizeof(double*)*block_'+rdnom+'['+string(kf-1)+'].nin))== NULL) return 0;';
          '  if ((block_'+rdnom+'['+string(kf-1)+'].insz   = malloc(3*sizeof(int)*block_'+rdnom+'['+string(kf-1)+'].nin))== NULL) return 0;'];

    //** inptr **//
    for k=1:nin
       lprt=inplnk(inpptr(kf)-1+k);
       Code=[Code
             '  block_'+rdnom+'['+string(kf-1)+'].inptr['+string(k-1)+']  = '+rdnom+'_block_outtbptr['+string(lprt-1)+'];']
    end

    //** 1st dim **//
    for k=1:nin
       lprt=inplnk(inpptr(kf)-1+k);
       Code=[Code
             '  block_'+rdnom+'['+string(kf-1)+'].insz['+string((k-1))+']   = '+string(size(outtb(lprt),1))+';']
    end

    //** 2dn dim **//
    for k=1:nin
       lprt=inplnk(inpptr(kf)-1+k);
       Code=[Code
             '  block_'+rdnom+'['+string(kf-1)+'].insz['+string((k-1)+nin)+']   = '+string(size(outtb(lprt),2))+';']
    end

    //** typ **//
    for k=1:nin
       lprt=inplnk(inpptr(kf)-1+k);
       Code=[Code
             '  block_'+rdnom+'['+string(kf-1)+'].insz['+string((k-1)+2*nin)+']   = '+mat2scs_c_typ(outtb(lprt))+';'];
    end
    //**********************************************************************//
//     for k=1:nin
//       lprt=inplnk(inpptr(kf)-1+k);
//       Code=[Code
//             '  block_'+rdnom+'['+string(kf-1)+'].inptr['+string(k-1)+'] = (double *)('+rdnom+'_block_outtbptr['+string(lprt-1)+']);'
//             '  block_'+rdnom+'['+string(kf-1)+'].insz['+string(3*(k-1))+']  = '+string(size(outtb(lprt),1))+';'
//             '  block_'+rdnom+'['+string(kf-1)+'].insz['+string(3*(k-1)+1)+']  = '+string(size(outtb(lprt),2))+';';
//             '  block_'+rdnom+'['+string(kf-1)+'].insz['+string(3*(k-1)+2)+']  = '+string(mat2scs_c_nb(outtb(lprt)))+';'];
//     end

    //***************************** output port *****************************//
    //** alloc outsz/outptr **//
    Code=[Code
          '  if ((block_'+rdnom+'['+string(kf-1)+'].outsz  = malloc(3*sizeof(int)*block_'+rdnom+'['+string(kf-1)+'].nout))== NULL) return 0;';
          '  if ((block_'+rdnom+'['+string(kf-1)+'].outptr = malloc(sizeof(double*)*block_'+rdnom+'['+string(kf-1)+'].nout))== NULL) return 0;'];

    //** outptr **//
    for k=1:nout
       lprt=outlnk(outptr(kf)-1+k);
       Code=[Code
             '  block_'+rdnom+'['+string(kf-1)+'].outptr['+string(k-1)+'] = '+rdnom+'_block_outtbptr['+string(lprt-1)+'];']
    end

    //** 1st dim **//
    for k=1:nout
       lprt=outlnk(outptr(kf)-1+k);
       Code=[Code
             '  block_'+rdnom+'['+string(kf-1)+'].outsz['+string((k-1))+']  = '+string(size(outtb(lprt),1))+';']
    end

    //** 2dn dim **//
    for k=1:nout
       lprt=outlnk(outptr(kf)-1+k);
       Code=[Code
             '  block_'+rdnom+'['+string(kf-1)+'].outsz['+string((k-1)+nout)+']  = '+string(size(outtb(lprt),2))+';']
    end

    //** typ **//
    for k=1:nout
       lprt=outlnk(outptr(kf)-1+k);
       Code=[Code
             '  block_'+rdnom+'['+string(kf-1)+'].outsz['+string((k-1)+2*nout)+']  = '+mat2scs_c_typ(outtb(lprt))+';'];
    end
    //**********************************************************************//

    Code=[Code
          '  block_'+rdnom+'['+string(kf-1)+'].z = &(z['+string(zptr(kf)-1)+']);']

    if (part(funs(kf),1:7) ~= 'capteur' & part(funs(kf),1:10) ~= 'actionneur' & funs(kf) ~= 'bidon') then
      //** rpar **//
      if (rpptr(kf+1)-rpptr(kf)>0) then
        Code=[Code;
              '  block_'+rdnom+'['+string(kf-1)+'].rpar=&(RPAR1['+string(rpptr(kf)-1)+']);']
      end
      //** ipar **//
      if (ipptr(kf+1)-ipptr(kf)>0) then
        Code=[Code;
              '  block_'+rdnom+'['+string(kf-1)+'].ipar=&(IPAR1['+string(ipptr(kf)-1)+']);']
      end
      //** opar **//
      if (opptr(kf+1)-opptr(kf)>0) then
        Code=[Code;
              '  if ((block_'+rdnom+'['+string(kf-1)+'].oparptr = malloc(sizeof(void *)*block_'+rdnom+'['+string(kf-1)+'].nopar))== NULL ) return 0;';
              '  if ((block_'+rdnom+'['+string(kf-1)+'].oparsz  = malloc(2*sizeof(int)*block_'+rdnom+'['+string(kf-1)+'].nopar))== NULL ) return 0;';
              '  if ((block_'+rdnom+'['+string(kf-1)+'].opartyp = malloc(sizeof(int)*block_'+rdnom+'['+string(kf-1)+'].nopar))== NULL ) return 0;';
              ]
        nopar = opptr(kf+1)-opptr(kf);
        //** oparptr **//
        for k=1:nopar
          Code=[Code;
                '  block_'+rdnom+'['+string(kf-1)+'].oparptr['+string(k-1)+']   = (void *) OPAR_'+string(opptr(kf)-1+k)+';'];
        end
        //** 1st dim **//
        for k=1:nopar
          Code=[Code;
                '  block_'+rdnom+'['+string(kf-1)+'].oparsz['+string(k-1)+']    = '+string(size(opar(opptr(kf)-1+k),1))+';'];
        end
        //** 2dn dim **//
        for k=1:nopar
          Code=[Code;
                '  block_'+rdnom+'['+string(kf-1)+'].oparsz['+string(nopar+(k-1))+']    = '+string(size(opar(opptr(kf)-1+k),2))+';'];
        end
        //** typ **//
        for k=1:nopar
          Code=[Code;
                '  block_'+rdnom+'['+string(kf-1)+'].opartyp['+string(k-1)+']   = '+mat2scs_c_typ(opar(opptr(kf)-1+k))+';'];
        end
      end
      //** oz **//
      if (ozptr(kf+1)-ozptr(kf)>0) then
        noz = ozptr(kf+1)-ozptr(kf);
        Code=[Code
              '  if ((block_'+rdnom+'['+string(kf-1)+'].ozptr = malloc(sizeof(void *)*block_'+rdnom+'['+string(kf-1)+'].noz))== NULL) return 0;';
              '  if ((block_'+rdnom+'['+string(kf-1)+'].ozsz  = malloc(2*sizeof(int)*block_'+rdnom+'['+string(kf-1)+'].noz))== NULL) return 0;';
              '  if ((block_'+rdnom+'['+string(kf-1)+'].oztyp = malloc(sizeof(int)*block_'+rdnom+'['+string(kf-1)+'].noz))== NULL) return 0;';]
        //** ozptr **//
        for k=1:noz
          Code=[Code;
                '  block_'+rdnom+'['+string(kf-1)+'].ozptr['+string(k-1)+']  = (void *) oz_'+string(ozptr(kf)-1+k)+';'];
        end
        //** 1st dim **//
        for k=1:noz
          Code=[Code;
                '  block_'+rdnom+'['+string(kf-1)+'].ozsz['+string(k-1)+']  = '+string(size(oz(ozptr(kf)-1+k),1))+';'];
        end
        //** 2nd dim **//
        for k=1:noz
          Code=[Code;
                '  block_'+rdnom+'['+string(kf-1)+'].ozsz['+string(noz+(k-1))+']  = '+string(size(oz(ozptr(kf)-1+k),2))+';'];
        end
        //** typ **//
        for k=1:noz
          Code=[Code;
                '  block_'+rdnom+'['+string(kf-1)+'].oztyp['+string(k-1)+']  = '+mat2scs_c_typ(oz(ozptr(kf)-1+k))+';'];
        end
      end
    end
    Code=[Code;
          '  block_'+rdnom+'['+string(kf-1)+'].work=(void **)(((double *)work)+'+string(kf-1)+');']
  end

  Code=[Code;
        '  if (strlen(input) > 0)'
        '    aaa=1;'
        '  if (strlen(output)> 0)'
        '    bbb=1;'
        '';
        '  /* call init function */'
        '  t=0.0;'
        '  '+rdnom+'_init(block_'+rdnom+','+rdnom+'_block_outtbptr,z,&t);'
        ''
        '  while (t<=tf) {';
        '    sci_time=t;']

  if (x ==[]) then
    Code=[Code
          '    /*'+rdnom+'_events(&nevprt,&t);*/'
          '    set_nevprt(nevprt);'
          '    '+rdnom+'main1(block_'+rdnom+','+rdnom+'_block_outtbptr,z,&t);'
          '    '+rdnom+'main2(block_'+rdnom+','+rdnom+'_block_outtbptr,z,&t);']
  else
    Code=[Code
          '    set_nevprt(nevprt);'
          '    '+rdnom+'main1(block_'+rdnom+','+rdnom+'_block_outtbptr,z,&t);'
          '    '+rdnom+'main2(block_'+rdnom+','+rdnom+'_block_outtbptr,z,&t);'
          '    tout=t;'
          '    while (tout+h<t+dt){'
          '      switch (solver) {'
          '      case 1:'
          '        ode1(C2F('+rdnom+'simblk),tout,h);'
          '        break;'
          '      case 2:'
          '        ode2(C2F('+rdnom+'simblk),tout,h);'
          '        break;'
          '      case 3:'
          '        ode4(C2F('+rdnom+'simblk),tout,h);'
          '        break;'
          '      default :'
          '        ode4(C2F('+rdnom+'simblk),tout,h);'
          '        break;'
          '        }'
          '       tout=tout+h;'
          '    }'
          '    he=t+dt-tout;'
          '    switch (solver) {'
          '    case 1:'
          '      ode1(C2F('+rdnom+'simblk),tout,he);'
          '      break;'
          '    case 2:'
          '      ode2(C2F('+rdnom+'simblk),tout,he);'
          '      break;'
          '    case 3:'
          '      ode4(C2F('+rdnom+'simblk),tout,he);'
          '      break;'
          '    default :'
          '      ode4(C2F('+rdnom+'simblk),tout,he);'
          '      break;'
          '    }']
  end

  if nx <> 0 then
    Code=[Code;
          '  block_'+rdnom+'['+string(kf-1)+'].nx = '+string(nx)+';';
          '  block_'+rdnom+'['+string(kf-1)+'].x  = &(x['+string(xptr(kf)-1)+']);'
          '  block_'+rdnom+'['+string(kf-1)+'].xd = &(xd['+string(xptr(kf)-1)+']);']
  end

  Code=[Code
        '    t=t+dt;'
        '  }'
        ''
        '  /* call end function */'
        '  '+rdnom+'_end(block_'+rdnom+','+rdnom+'_block_outtbptr,z,&t);'
        '  return 0;'
        '}'  
        '/*'+part('-',ones(1,40))+'  Lapack messag function */';
        'void'
        'C2F(xerbla)(SRNAME,INFO,L)'
        '     char *SRNAME;'
        '     int *INFO;'
        '     long int L;'
        '{'
        '  printf(""** On entry to %s, parameter number %d  had an illegal value\n"",SRNAME,*INFO);'
        '}']

  //duplication de main1 et main2 pour le standalone
  Code=[Code
        make_main1(%t);
        make_main2(%t);
        make_init(%t);
        make_end(%t)
        c_make_doit1(cpr,%t);
        c_make_doit2(cpr,%t);
        c_make_outtb(%t);
        c_make_initi(cpr,%t);
        c_make_endi(cpr,%t);
        make_putevs()]

  Code=[Code
        'void set_block_error(int err)'
        '{'
        '  return;'
        '}'
        'int get_phase_simulation()'
        '{'
        '  return 1;'
        '}'
        'void * scicos_malloc(size_t size)'
        '{'
        '  return malloc(size);'
        '}'
        'void scicos_free(void *p)'
        '{'
        '  free(p);'
        '}'
        'double get_scicos_time()'
        '{'
          '  return sci_time;'
        '}'
        'void do_cold_restart()'
        '{'
        '  return;'
        '}'
        'void sciprint (char *fmt){'
        '  return;'
        '}'
        ''
        'int getopt ( int argc, char *argv[], char *optstring )'
        '{'
        '  char *group, option, *sopt;'
        '  int len;'
        '  option = -1;'
        '  optarg = NULL;'
        '  while ( optind < argc )'
        '    { '
        '      group = argv[optind];'
        '      if ( *group != ''-'' )'
        '        {' 
        '         option = -1;'
        '         optarg = group;'
        '         optind++;'
        '         break;'
        '        }'
        '      len = strlen (group);'
        '      group = group + offset;'
        '      if ( *group == ''-'' )'
        '        {'
        '         group++;'
        '         offset += 2;'
        '        }'
        '      else'
        '        offset++ ;'
        '      option = *group ;'
        '      sopt = strchr ( optstring, option ) ;'
        '      if ( sopt != NULL )'
        '        {'
        '         sopt++ ;'
        '         if ( *sopt == '':'' )'
        '           {' 
        '             optarg = group + 1;'
        '             if ( *optarg == ''\0'' )'
        '                optarg = argv[++optind];'
        '             if ( *optarg == ''-'' )'
        '                { '
        '                 fprintf ( stderr, '"\n%s: illegal option -- %c'",'
        '                           argv[0], option );'
        '                 option = ''?'';'
        '                 break;'
        '                } '
        '             else '
        '                { '
        '                 optind++;'
        '                 offset = 0;'
        '                 break;'
        '                } '
        '           }'
        '         if ( offset >= len )'
        '           {'
        '             optind++;'
        '             offset = 0;'
        '           }'
        '         break;'
        '        }'
        '      else'
        '        {'
        '         fprintf ( stderr, '"\n%s: illegal option -- %c'", argv[0], option );'
        '         option = ''?'';'
        '         break;'
        '        } '
        '    }'
        '  return ( option );'
        '}']

  if (x <> []) then
    Code=[Code
          'int C2F('+rdnom+'simblk)(t, xc, xcdot)'
          '     double *t, *xc, *xcdot;'
          '     /* '
          '        !purpose '
          '        compute state derivative of the continuous part'
          '        !calling sequence '
          '        neq   : integer the size of the  continuous state'
          '        t     : current time '
          '        xc    : double precision vector whose  contains the continuous state.' 
          '        xcdot : double precision vector, contain the computed derivative '
          '        of the state' 
          '     */'
          '{ '
          '  /* int iwa[1]={0}; */'
          '  int phase=2;'
          '  C2F(dset)(&neq, &c_b14,xcdot , &c__1);'
          '  '+rdnom+'odoit(t, '+rdnom+'_block_outtbptr,'+rdnom+'_iwa,phase,xcdot, xc, xcdot);'
          ' '
          '  return 0;'
          '}'
          ''
          '/* Euler''s Method */'
          'int ode1(f,t,h)'
          '  int (*f) ();'
          '  double t, h;'
          '{'
          '  int i;'
          '  (*f)(&t,x, xd);'
          '  for (i=0;i<neq;i++){'
          '  x[i]=x[i]+h*xd[i];'
          '  }'
          '  return 0;'
          '}'
          ''
          '/* Heun''s Method */'
          'int ode2(f,t,h)'
          '  int (*f) ();'
          '  double t, h;'
          '{'
          '  int i;'
          '  double y['+string(nX)+'],yh['+string(nX)+'],temp,f0['+string(nX)+'],th;'
          '  memcpy(y,x,neq*sizeof(double));'
          '  memcpy(f0,xd,neq*sizeof(double));'
          '  (*f)(&t,y, f0);'
          '  for (i=0;i<neq;i++) x[i]=y[i]+h*f0[i];'
          '  temp=0.5*h;'
          '  th=t+h;'
          '  for (i=0;i<neq;i++) yh[i]=y[i]+h*f0[i];'
          '  (*f)(&th,yh, xd);'
          '  for (i=0;i<neq;i++) x[i]=y[i]+temp*(f0[i]+xd[i]);'
          '  return 0;'
          '}'
          ''
          '  /* Fourth-Order Runge-Kutta (RK4) Formula */'
          'int ode4(f,t,h)'
          '  int (*f) ();'
          '  double t, h;'
          '{'
          '  int i;'
          '  double y['+string(nX)+'],yh['+string(nX)+'],temp,f0['+string(nX)+'],th,th2,f1['+string(nX)+'],f2['+string(nX)+'];'
          '  memcpy(y,x,neq*sizeof(double));'
          '  memcpy(f0,xd,neq*sizeof(double));'
          '  (*f)(&t,y, f0);'
          '  for (i=0;i<neq;i++) x[i]=y[i]+h*f0[i];'
          '  temp=0.5*h;'
          '  th2=t+h/2;'
          '  for (i=0;i<neq;i++) yh[i]=y[i]+(h/2)*f0[i];'
          '  (*f)(&th2,yh, f1);'
          '  for (i=0;i<neq;i++) x[i]=y[i]+temp*f1[i];'  
          '  for (i=0;i<neq;i++) yh[i]=y[i]+(h/2)*f1[i];'
          '  (*f)(&th2,yh, f2);'
          '  for (i=0;i<neq;i++) x[i]=y[i]+h*f2[i];'
          '  th=t+h;'
          '  for (i=0;i<neq;i++) yh[i]=y[i]+h*f2[i];'
          '  (*f)(&th2,yh, xd);'
          '  temp=h/6;'
          '  for (i=0;i<neq;i++) x[i]=y[i]+temp*(f0[i]+2.0*f1[i]+2.0*f2[i]+xd[i]);'
          'return 0;'
          '}']
  end
endfunction

//generates static table definitions
//Copyright INRIA
function [Code]=make_standalone_static()
  tevts=cpr.state.tevts;
  evtspt=cpr.state.evtspt;
  zptr=cpr.sim.zptr;
  clkptr=cpr.sim.clkptr;
  ordptr=cpr.sim.ordptr;
  pointi=cpr.state.pointi;
  funs=cpr.sim.funs;

  Code=['';
        '/* Table of constant values */';
        'static integer nrd_'+string(0:maxtotal)'+' = '+string(0:maxtotal)'+';';]

  if maxtotal<10 then
    Code=[Code;
          'static integer nrd_10 = 10;'];
  end
  if maxtotal<11 then
    Code=[Code;
          'static integer nrd_11 = 11;'];
  end

  Code=[Code;
        'static integer nrd_81 = 81;';
        'static integer nrd_82 = 82;';
        'static integer nrd_84 = 84;';
        'static integer nrd_811 = 811;';
        'static integer nrd_812 = 812;';
        'static integer nrd_814 = 814;';
        '';
        'static integer totalnevprt;'];

  if size(evtspt,1) <> 0 then
    Code=[Code;
          cformatline('static integer evtspt[ ]={'+..
                                            strcat(string(evtspt),",")+'};',70)] 
  else
    Code($+1)='static integer evtspt[1];';
  end

  Code($+1)='static double zero=0;';

  if size(clkptr,1) <> 0 then
    Code=[Code;cformatline('static integer clkptr[ ]={'+..
                                            strcat(string(clkptr),",")+'};',70)]
  else
    Code($+1)='static integer clkptr[1];';
  end

  if size(ordptr,1) <> 0 then
    Code=[Code;cformatline('static integer ordptr[ ]={'+..
                                            strcat(string(ordptr),",")+'};',70)]
  else
    Code($+1)='static integer ordptr[1];';
  end

  if size(tevts,1) <> 0 then
    Code=[Code;cformatline('static double tevts[ ] = {'+..
                                            strcat(string(tevts),",")+'};',70)]
  else
    Code($+1)='static double tevts[1];';
  end

  if size(pointi,1) <> 0 then
    Code($+1)='static integer pointi={'+strcat(string(pointi),",")+'};';
  else
    Code($+1)='static integer pointi=1;';
  end
  Code($+1)='static double w[1];'
  Code($+1)='void **'+rdnom+'_block_outtbptr;'
  Code($+1)='static int *'+rdnom+'_iwa;'
  Code($+1)='static int aaa=0, bbb=0;'
  Code=[Code;
        'static void set_nevprt(int nevprt)'
        '{'
        '  totalnevprt=nevprt;'
        '}']
endfunction

function Code=make_static()
  Code=make_standalone_static();
  Code($+1)='scicos_block *block_'+rdnom+';'
endfunction

//generates  static table definitions
//
//Copyright INRIA
//
//Author : Rachid Djenidi, Alan Layec

function Code=make_static_standalone()
  rpptr=cpr.sim.rpptr;
  ipptr=cpr.sim.ipptr;
  opptr=cpr.sim.opptr;
  rpar=cpr.sim.rpar;
  ipar=cpr.sim.ipar;
  opar=cpr.sim.opar;
  x=cpr.state.x;

  nX=size(x,'*');
  Code=make_standalone_static();

  //*** Continuous state ***//
  if x <> [] then
   Code=[Code;
         '/* def continuous state */'
         cformatline('double x[]={'+strcat(string(x),',')+'};',70)
         cformatline('double xd[]={'+strcat(string(x),',')+'};',70)
         'static integer c__1 = 1;'
         'static double c_b14 = 0.;'
         'static integer neq='+string(nX)+';'
         '']
  end
  //************************//

  Code=[Code;
        'char input[50], output[50],s[1],sf[1],se[1],ss[1],**p;'
        'static double sci_time;'
        'static int errflg = 0;'
        'static char *optarg = NULL ;'
        'static int optind = 1, offset = 0 ;'
        'scicos_block block_'+rdnom+'['+string(nblk)+'];'
        ''];

  //*** Real parameters ***//
  if size(rpar,1) <> 0 then
    Code=[Code;
          '/* def real parameters */'
          'static double RPAR1[ ] = {'];

    for i=1:(length(rpptr)-1)
      if rpptr(i+1)-rpptr(i)>0  then

        if size(corinv(i),'*')==1 then
          OO=scs_m.objs(corinv(i));
        else
          path=list('objs');
          for l=cpr.corinv(i)(1:$-1)
            path($+1)=l;
            path($+1)='model';
            path($+1)='rpar';
            path($+1)='objs';
          end
          path($+1)=cpr.corinv(i)($);
          OO=scs_m(path);
        end

        //** Add comments **//
        Code($+1)='/* Routine name of block: '+strcat(string(cpr.sim.funs(i)));
        Code($+1)=' * Gui name of block: '+strcat(string(OO.gui));
        Code($+1)=' * Compiled structure index: '+strcat(string(i));

        if stripblanks(OO.model.label)~=emptystr() then
          Code=[Code;cformatline(' * Label: '+strcat(string(OO.model.label)),70)];
        end
        if stripblanks(OO.graphics.exprs(1))~=emptystr() then
          Code=[Code;cformatline(' * Exprs: '+strcat(OO.graphics.exprs(1),","),70)];
        end
        if stripblanks(OO.graphics.id)~=emptystr() then
          Code=[Code;
                cformatline(' * Identification: '+strcat(string(OO.graphics.id)),70)];
        end
        Code=[Code;' * rpar='];
        Code($+1)=' */';
        //******************//

        Code=[Code;cformatline(strcat(msprintf('%.16g,\n',rpar(rpptr(i):rpptr(i+1)-1))),70)];
        Code($+1)='';
      end
    end
    Code=[Code;
          '};'
          '']

  else
    Code($+1)='static double RPAR1[1];';
  end
  //***********************//

  //*** Integer parameters ***//
  if size(ipar,1) <> 0 then
    Code=[Code;
          '/* def integer parameters */'
          'static integer IPAR1[ ] = {'];

    for i=1:(length(ipptr)-1)
      if ipptr(i+1)-ipptr(i)>0  then
        if size(corinv(i),'*')==1 then
          OO=scs_m.objs(corinv(i));
        else
          path=list('objs');
          for l=cpr.corinv(i)(1:$-1)
            path($+1)=l
            path($+1)='model'
            path($+1)='rpar'
            path($+1)='objs'
          end
          path($+1)=cpr.corinv(i)($);
          OO=scs_m(path);
        end

        //** Add comments **//
        Code($+1)='/* Routine name of block: '+strcat(string(cpr.sim.funs(i)));
        Code($+1)=' * Gui name of block: '+strcat(string(OO.gui));
        Code($+1)=' * Compiled structure index: '+strcat(string(i));
        if stripblanks(OO.model.label)~=emptystr() then
          Code=[Code;cformatline(' * Label: '+strcat(string(OO.model.label)),70)];
        end

        if stripblanks(OO.graphics.exprs(1))~=emptystr() then
          Code=[Code;cformatline(' * Exprs: '+strcat(OO.graphics.exprs(1),","),70)];
        end
        if stripblanks(OO.graphics.id)~=emptystr() then
          Code=[Code;
                cformatline(' * Identification: '+strcat(string(OO.graphics.id)),70)];
        end
        Code=[Code;
              cformatline(' * ipar= {'+strcat(string(ipar(ipptr(i):ipptr(i+1)-1)),",")+'};',70)];
        Code($+1)=' */';
        //******************//

        Code=[Code;cformatline(strcat(string(ipar(ipptr(i):ipptr(i+1)-1))+','),70)];
        Code($+1)='';
      end
    end
    Code=[Code;
          '};'
          '']
  else
    Code($+1)='static integer IPAR1[1];';
  end
  //**************************//

  //Alan added opar (27/06/07)
  //*** Object parameters ***//
  if lstsize(opar)<>0 then
    Code=[Code;
          '/* def object parameters */']
    for i=1:(length(opptr)-1)
      if opptr(i+1)-opptr(i)>0  then

        if size(corinv(i),'*')==1 then
          OO=scs_m.objs(corinv(i));
        else
          path=list('objs');
          for l=cpr.corinv(i)(1:$-1)
            path($+1)=l;
            path($+1)='model';
            path($+1)='rpar';
            path($+1)='objs';
          end
          path($+1)=cpr.corinv(i)($);
          OO=scs_m(path);
        end

        //** Add comments **//
        Code($+1)='';
        Code($+1)='/* Routine name of block: '+strcat(string(cpr.sim.funs(i)));
        Code($+1)=' * Gui name of block: '+strcat(string(OO.gui));
        Code($+1)=' * Compiled structure index: '+strcat(string(i));
        if stripblanks(OO.model.label)~=emptystr() then
          Code=[Code;cformatline(' * Label: '+strcat(string(OO.model.label)),70)];
        end
        if stripblanks(OO.graphics.id)~=emptystr() then
          Code=[Code;
                cformatline(' * Identification: '+strcat(string(OO.graphics.id)),70)];
        end
        Code($+1)=' */';
        //******************//

        for j=1:opptr(i+1)-opptr(i)
          Code =[Code;
                 cformatline('static '+mat2c_typ(opar(opptr(i)+j-1)) +...
                             ' OPAR_'+string(opptr(i)+j-1) + '[] = {'+...
                             strcat(string(opar(opptr(i)+j-1)),',')+'};',70)]
        end

      end
    end
  end
  //*************************//
endfunction

//nouveau
function Code=make_cdoit(stalone)
  //les pointeurs de cpr :
  z=cpr.state.z;
  pointi=cpr.state.pointi;
  funs=cpr.sim.funs;
  inpptr=cpr.sim.inpptr;
  clkptr=cpr.sim.clkptr;
  cord=cpr.sim.cord;

  ncord=size(cord,1);
  sztvec=max(clkptr(2:$)-clkptr(1:$-1))+1;

  Code=['';
        '/*'+part('-',ones(1,40))+' cdoit.c */';
        'int '
         cformatline(rdnom+'cdoit(z, told, outtbptr)',70);
        '';
        '     double  *z, *told;';
        '     void **outtbptr;';
        '{';
        '  /* System generated locals */';
        '  integer i2, ntvecm;';
        '';
        '  /* Local variables */';
        '  integer flag,nport,sz[100],ierr1,nevprt,ntvec;';
        '  double  tvec['+string(sztvec)+'],rdouttb['+string(lstsize(outtb)+1)+'],*args[100];';
        '  double *outtbdptr;'
        '';
        '  /* Function Body */';];

  for ii=1:ncord
    fun=cord(ii,1);
    //flag=1 for all blocks
    //if outptr(fun+1)-outptr(fun)>0  then
      nclock=cord(ii,2);
      if or(fun==act) | or(fun==cap) then
        if stalone then
          Code=[Code;
                '  flag = 1 ;';
                '  nevprt='+string(nclock)+';';
                ' '+wfunclist(fun);
                '';
                ' '];
        end
      else
        Code=[Code;
              '  flag = 1 ;';
              '  nevprt='+string(nclock)+';';
              ' '+wfunclist(fun);
              '';
              ''];
      end
    //end

    ntvec=clkptr(fun+1)-clkptr(fun);

    if ntvec >0  & funs(fun) <> 'bidon' then
      nxx=inplnk(inpptr(fun));
      if funtyp(fun)==-1 then
        Code=[Code;
              '    outtbdptr = (double *)outtbptr['+string(nxx-1)+']';
              '    if (*outtbdptr>0) {';
              '      ntvecm=1;';
              '    }';
              '    else {';
              '      ntvecm=2;';
              '    }']
      elseif funtyp(fun)==-2 then
        Code=[Code;
              '    outtbdptr = (double *)outtbptr['+string(nxx-1)+']';
              '    ntvecm=(integer)(*outtbdptr);';
              '    if(ntvecm>'+string(ntvec)+'){ntvecm='+string(ntvec)+';}';
              '    if(ntvecm<1){ntvecm=1;}']
      else
         message('Block '+fun+' has funtyp '+string(funtyp(fun))+': not allowed')
      end

      Code=[Code;
            '    i2 = ntvecm + clkptr['+string(fun-1)+'] - 1;';
            '    '+rdnom+'_putevs(told, &i2, &ierr1);';
            '    if (ierr1 != 0) return 3;'
            cformatline('    '+rdnom+'doit(z, told, outtbptr);',70);]
    end
  end //end for

  Code=[Code;
        '  return 0;'
        '';
        '} /* cdoit */';
        ' '];

endfunction

function Code=make_ddoit1()

  //les pointeurs de cpr :
  z=cpr.state.z;
  pointi=cpr.state.pointi;
  funs=cpr.sim.funs;
  inpptr=cpr.sim.inpptr;
  clkptr=cpr.sim.clkptr;

  Code=['';
        '/*'+part('-',ones(1,40))+' ddoit1.c */';
        'int '
        cformatline(rdnom+'ddoit1(z, told, outtbptr, iwa)',70);
        '';
        '     double  *z, *told;';
        '     void **outtbptr;';
        '     integer *iwa;';
        '{';
        '  /* System generated locals */';
        '';
        '  /* Local variables */';
        '  integer kiwa;';
        '';
        '  /* Function Body */';
        '  kiwa = 0; '
        '  if (totalnevprt==0) { ';
        '     '+rdnom+'cdoit(z, told, outtbptr);';
        '     return 0;'
        '  }'
       ];

  if  szclkIN>=1 then
    pointi=clkptr(howclk)-1;
    Code($+1)='  pointi='+string(pointi)+'+ totalnevprt;';
  else
    //RN update of 27/06/07
    if ALWAYS_ACTIVE then //  nothing to be done
       Code=[Code;
         '} /* ddoit1 */';
       '  '];
       return;
    elseif (size(cap,'*') >1) then
      pointi=clkptr(howclk)-1;
      Code($+1)='  pointi='+string(pointi)+'+ totalnevprt;';
    else
      pointi=clkptr(howclk)
      Code($+1)='  pointi='+string(pointi)+';';
    end
  end

  Code=[Code;
        '  tevts[pointi-1]=*told;'];

  if ~stalone then
//disp('laaa');pause
    Code=[Code;
          cformatline('  '+rdnom+'edoit1(told, '+..
                      'outtbptr, '+..
                      '(int *)(z+'+string(size(z,1))+..
                      '),&kiwa);',70);
          '  iwa['+string(clkptr($)-1)+']=kiwa;']
  else
    Code=[Code;
          cformatline('  '+rdnom+'edoit1(told, '+..
                      'outtbptr, '+..
                      '(int *)(z+'+string(size(z,1)+lstsize(outtb))+..
                      '),&kiwa);',70);
          '  iwa['+string(clkptr($)-1)+']=kiwa;']
  end

  Code=[Code;
        '  return 0;'
        '';
        '} /* ddoit1 */';
        ''];
endfunction

function Code=make_doit(stalone)
  //les pointeurs de cpr :
  z=cpr.state.z;
  funs=cpr.sim.funs;
  inpptr=cpr.sim.inpptr;
  outptr=cpr.sim.outptr;
  funtyp=cpr.sim.funtyp;
  ordptr=cpr.sim.ordptr;ordclk=cpr.sim.ordclk;
  clkptr=cpr.sim.clkptr;evtspt=cpr.state.evtspt;

  sztvec=max(clkptr(2:$)-clkptr(1:$-1))+1;
  //maxkeve=size(evtspt,1);
  maxkeve=size(ordptr,1)-1;

  Code=['/*'+part('-',ones(1,40))+' doit.c */';
        'int '
         cformatline(rdnom+'doit(z, told, outtbptr)',70)
        '     double  *z, *told;';
        '     void **outtbptr;';
        '{';
        '  /* System generated locals */';
        '  integer i2;';
        '';
        '  /* Local variables */';
        '  integer flag, kever, nport;';
        '  integer nord;';
        '  double  tvec['+string(sztvec)+'];';
        '  double  rdouttb['+string(lstsize(outtb)+1)+'];';
        '  double  *args[100];';
        '  double *outtbdptr;'
        '  integer sz[100];';
        '  integer ierr1, i, nx=0;';
        '  integer ntvec, ntvecm, nevprt;';
        '';
        '  /* Function Body */';
        '  kever = pointi;';
        '  pointi = evtspt[kever];';
        '  evtspt[kever] = -1;';
        '';
        '  nord = ordptr[kever + 1] - ordptr[kever];';
        '  if (nord == 0) {';
        '    return 0;';
        '  }';
        '  switch(kever) {'];

  for kever=1:maxkeve
    Code($+1)='  case '+string(kever)+':';

    for ii=ordptr(kever):ordptr(kever+1)-1
      //***********************************
      fun=ordclk(ii,1);
      //flag=1 for all blocks
      //if outptr(fun+1)-outptr(fun)>0  then
        nclock=ordclk(ii,2);
        if or(fun==act) | or(fun==cap) then
          if stalone then
            Code=[Code;
                  '  flag = 1;';
                  '  nevprt='+string(nclock)+';';
                  ' '+wfunclist(fun);
                  '';
                  ''];
          end
        else
            Code=[Code;
                 '  flag = 1 ;';
                 '  nevprt='+string(nclock)+';';
                 ' '+wfunclist(fun);
                 '';
                 ''];
        end
      //end

      nevprt=ordclk(ii,2);
      ntvec=clkptr(fun+1)-clkptr(fun);
      if ntvec >0  & funs(fun) <> 'bidon' then
        nxx=inplnk(inpptr(fun));
        if funtyp(fun)==-1 then
          Code=[Code;
                '    outtbdptr = (double *)outtbptr['+string(nxx-1)+'];';
                '    if (*outtbdptr>0) {';
                '      ntvecm=1;';
                '    }';
                '    else {';
                '      ntvecm=2;';
                '    }']
        elseif funtyp(fun)==-2 then
          Code=[Code;
                '    outtbdptr = (double *)outtbptr['+string(nxx-1)+'];';
                '    ntvecm=(integer)(*outtbdptr);';
                '    if(ntvecm>'+string(ntvec)+'){ntvecm='+string(ntvec)+';}';
                '    if(ntvecm<1){ntvecm=1;}']
        else
          message('Block '+fun+' has funtyp '+string(funtyp(fun))+': not allowed')
        end

        Code=[Code;
              '     i2 = ntvecm + clkptr['+string(fun-1)+'] - 1;';
              '    '+rdnom+'_putevs(told, &i2, &ierr1);';
              '     if (ierr1 != 0) return 3;'
              cformatline('    '+rdnom+'doit(z, told, outtbptr);',70)]
      end
    end
    Code($+1)='    break;';
  end
  Code=[Code;
        '  }';
        '  return 0;'
        '} /* doit */'];
endfunction

function Code=make_edoit1(stalone)
  //les pointeurs de cpr :
  z=cpr.state.z;
  funs=cpr.sim.funs;
  inpptr=cpr.sim.inpptr;
  outptr=cpr.sim.outptr;
  funtyp=cpr.sim.funtyp;
  ordptr=cpr.sim.ordptr;
  ordclk=cpr.sim.ordclk;
  clkptr=cpr.sim.clkptr;
  evtspt=cpr.state.evtspt;

  maxkeve=size(ordptr,1)-1;

  Code=[ '/*'+part('-',ones(1,40))+' edoit1.c */';
         'int '
          cformatline(rdnom+'edoit1(told, outtbptr, iwa, kiwa)',70)
         '     double  *told;';
         '     void **outtbptr;';
         '     integer *iwa, *kiwa;';
         '{';
         '  /* System generated locals */';
         '  integer i2;';
         '';
         '  /* Local variables */';
         '  integer flag, kever, nport, nord, ierr1, ntvecm, nevprt;';
         '  double  rdouttb['+string(lstsize(outtb)+1)+'], *args[100];';
         '  double *outtbdptr;'
         '';
         '  /* Function Body */';
         '  kever = pointi;';
         '  pointi = evtspt[kever-1];';
         '  evtspt[kever-1] = -1;';
         '';
         '  nord = ordptr[kever] - ordptr[kever-1];';
         '  if (nord == 0) { ';
         '    return 0; ';
         '  }';
         '  ++(*kiwa);';
         '  iwa[*kiwa-1] = kever;';
         '  switch(kever) {'];

  for kever=1:maxkeve
    Code($+1)='  case '+string(kever)+':';

    for ii=ordptr(kever):ordptr(kever+1)-1
      //***********************************
      fun=ordclk(ii,1);

      //RN update of 26/06/07
      nclock=abs(ordclk(ii,2));
      if or(fun==act) | or(fun==cap) then
        if stalone then
          Code=[Code;
                '    flag = 1 ;';
                '    nevprt='+string(nclock)+';';
                '    '+wfunclist(fun);
                '';
                ''];
         end
      else
         Code=[Code;
               '    flag = 1 ;';
               '    nevprt='+string(nclock)+';';
               '    '+wfunclist(fun);
               '';
               ''];
      end

      ntvec=clkptr(fun+1)-clkptr(fun);

      if ntvec >0  & funs(fun) <> 'bidon' then
        nxx=inplnk(inpptr(fun));
        if funtyp(fun)==-1 then
          Code=[Code;
                '    outtbdptr = (double *)outtbptr['+string(nxx-1)+'];';
                '    if (outtbdptr[0]>0) {';
                '      ntvecm=1;';
                '    }';
                '    else {';
                '      ntvecm=2;';
                '    }']
        elseif funtyp(fun)==-2 then
          Code=[Code;
                '    outtbdptr = (double *)outtbptr['+string(nxx-1)+'];';
                '    ntvecm=(integer)outtbdptr[0];';
                '    if(ntvecm>'+string(ntvec)+'){ntvecm='+string(ntvec)+';}';
                '    if(ntvecm<1){ntvecm=1;}']
        else
          message('Block '+fun+' has funtyp '+string(funtyp(fun))+': not allowed')
        end

         Code=[Code;
               '    i2 = ntvecm + clkptr['+string(fun-1)+'] - 1;';
               '    '+rdnom+'_putevs(told, &i2, &ierr1);';
               '    if (ierr1 != 0) return 3;';
                cformatline('    '+rdnom+'edoit1(told, outtbptr, iwa, kiwa);',70)
               ]
      end
    end //end for ii=ordptr...
    Code($+1)='    break;';
  end

  Code=[Code;
        '  }';
        '  return 0;'
        '} /* edoit1 */'];
endfunction

//nouveau
function Code=make_odoit(stalone)
  //les pointeurs de cpr :
  z=cpr.state.z;
  pointi=cpr.state.pointi;
  funs=cpr.sim.funs;
  inpptr=cpr.sim.inpptr;
  clkptr=cpr.sim.clkptr;
  oord=cpr.sim.oord;
  xptr=cpr.sim.xptr;
  evtspt=cpr.state.evtspt;
  ordptr=cpr.sim.ordptr;
  ordclk=cpr.sim.ordclk;

  noord=size(oord,1);
  sztvec=max(clkptr(2:$)-clkptr(1:$-1))+1;

  Code=['';
        '/*'+part('-',ones(1,40))+' odoit.c */';]

  if (stalone & x <> []) then
    Code=[Code
          'int '
           cformatline(rdnom+'odoit(told, outtbptr, iwa, phase,residual, xt, xtd)',70);
          '';
          '     double  *told, *residual, *xt, *xtd;';
          '     void **outtbptr;';
          '     integer *iwa, phase;';
         '{']
  else
    Code=[Code
          'int '
          cformatline(rdnom+'odoit(told, outtbptr, iwa, phase)',70);
          '';
          '     double  *told;';
          '     void **outtbptr;';
          '     integer *iwa, phase;';
          '{']
  end

  if (x <> []) then
    Code=[Code
          '  /* System generated locals */';
          '  integer i,i2;';
          '';
          '  /* Local variables */';
          '  integer flag, keve, kiwa, nport;';
          '  double  tvec['+string(sztvec)+'];';
          '  double  rdouttb['+string(lstsize(outtb)+1)+'];';
          '  double  *args[100];';
          '  integer sz[100];';
          '  integer ierr1;';
          '  integer nevprt;';
          '  integer ntvec;';
          '  double *outtbdptr;'
          '';
          '  /* Function Body */';
          '  kiwa = 0;';];

    //////////////////////////////////////////////////

    for ii=1:noord
      fun=oord(ii,1);
      //flag=1 for all blocks
      //if outptr(fun+1)-outptr(fun)>0  then
        nclock=oord(ii,2);
        Code=[Code;
              '  flag = 1 ;';
              '  nevprt='+string(nclock)+';';]
        if stalone then
          if (xptr(fun+1)-xptr(fun)) > 0 then
            Code=[Code;
                  '  block_'+rdnom+'['+string(fun-1)+'].x=&(xt['+string(xptr(fun)-1)+']);'
                  '  block_'+rdnom+'['+string(fun-1)+'].xd=&(xtd['+string(xptr(fun)-1)+']);']
          end
        end
        Code=[Code;
              '  '+wfunclist(fun);
              ''];
      //end
      ntvec=clkptr(fun+1)-clkptr(fun);
      if ntvec >0  & funs(fun) <> 'bidon' then
        if funtyp(fun)<0 then
          Code=[Code;
                '    i2 = ntvecm + clkptr['+string(fun-1)+'] - 1;';
                '    '+rdnom+'_putevs(told, &i2, &ierr1);';
                '    if (ierr1 != 0) return 3;'
                cformatline('    '+rdnom+'ozdoit(told, outtbptr, iwa, &kiwa, phase);',70)]
        end
      end
    end

    Code=[Code;
          '  /*       update states derivatives */'];

    for ii=1:noord
      fun=oord(ii,1);
      if xptr(fun+1)-xptr(fun)>0  then
        nclock=oord(ii,2);
        Code=[Code;
              '  flag = 0 ;';
              '  nevprt='+string(nclock)+';';
              '  '+wfunclist(fun);
              ''];
      end
    end

    Code=[Code;
          '  for (i = 1; i <= kiwa; ++i) {';
          '    keve = iwa[i-1];';
          '    switch(keve) {'];
    maxkeve=size(ordptr,1)-1;
    for keve=1:maxkeve
      Code($+1)='    case '+string(keve)+':';
      for ii=ordptr(keve):ordptr(keve+1)-1
        fun=ordclk(ii,1);
        if (xptr(fun+1)-xptr(fun))>0  then
          nclock=ordclk(ii,2);
          if or(fun==act) | or(fun==cap) then
            if stalone then
              Code=[Code;
                    '  flag = 0 ;';
                    '  nevprt='+string(nclock)+';';
                    '  '+wfunclist(fun);
                    ''];
            end
          else
            Code=[Code;
                   '  flag = 0 ;';
                   '  nevprt='+string(nclock)+';';
                   ' '+wfunclist(fun);
                   ''];
          end
        end
      end
      Code($+1)='    break;';
    end
    Code=[Code;
          '    }'; //switch
          '  }';]
  end

  for ii=1:noord
    fun=oord(ii,1);
    nclock=oord(ii,2);
    if stalone then
      if (xptr(fun+1)-xptr(fun)) > 0 then
        Code=[Code;
              '  block_'+rdnom+'['+string(fun-1)+'].x=&(x['+string(xptr(fun)-1)+']);'
              '  block_'+rdnom+'['+string(fun-1)+'].xd=&(xd['+string(xptr(fun)-1)+']);']
      end
    end
  end

  Code=[Code;
        '  return 0;'
        '';
        '} /* odoit */';
        ' '];
endfunction

function Code=make_ozdoit(stalone)
  //les pointeurs de cpr :
  z=cpr.state.z;
  funs=cpr.sim.funs;
  inpptr=cpr.sim.inpptr;
  outptr=cpr.sim.outptr;
  funtyp=cpr.sim.funtyp;
  ordptr=cpr.sim.ordptr;
  ordclk=cpr.sim.ordclk;
  clkptr=cpr.sim.clkptr;
  evtspt=cpr.state.evtspt;

  sztvec=max(clkptr(2:$)-clkptr(1:$-1))+1;
  //////////////////////////////////////////////////
  maxkeve=size(ordptr,1)-1;

  Code=['/*'+part('-',ones(1,40))+' ozdoit.c */';
        'int '
        cformatline(rdnom+'ozdoit(told, outtbptr, iwa, kiwa, phase)',70)
        '     double  *told;';
        '     void **outtbptr;';
        '     integer *iwa, *kiwa, phase;';
        '{';
        '  /* System generated locals */';
        '  integer i2;';
        '';
        '  /* Local variables */';
        '  integer flag, kever, nport;';
        '  integer nord;';
        '  double  tvec['+string(sztvec)+'];';
        '  double  rdouttb['+string(lstsize(outtb)+1)+'];';
        '  double  *args[100];';
        '  double *outtbdptr;'
        '  integer sz[100];';
        '  integer ierr1, i, nx=0;';
        '  integer ntvec, ntvecm, nevprt;';
        '';
        '  /* Function Body */';
        '  kever = pointi;';
        '  pointi = evtspt[kever];';
        '  evtspt[kever] = -1;';
        '';
        '  nord = ordptr[kever + 1] - ordptr[kever];';
        '  if (nord == 0) {';
        '    return 0;';
        '  }';
        '  ++(*kiwa);';
        '  iwa[*kiwa-1] = kever;';
        '  switch(kever) {'];

  for kever=1:maxkeve
    Code($+1)='  case '+string(kever)+':';

    for ii=ordptr(kever):ordptr(kever+1)-1
      //***********************************
      fun=ordclk(ii,1);
      //flag=1 for all blocks
      //if outptr(fun+1)-outptr(fun)>0  then
        nclock=ordclk(ii,2);
        if or(fun==act) | or(fun==cap) then
          if stalone then
            Code=[Code;
                  '  flag = 1 ;';
                  '  nevprt='+string(nclock)+';';
                  ' '+wfunclist(fun);
                  '';
                  ''];
          end
        else
            Code=[Code;
                 '  flag = 1 ;';
                 '  nevprt='+string(nclock)+';';
                 ' '+wfunclist(fun);
                 '';
                 ''];
        end
      //end
      i=fun;
      nevprt=ordclk(ii,2);
      ntvec=clkptr(fun+1)-clkptr(fun);
      if ntvec >0  & funs(fun) <> 'bidon' then
        nxx=inplnk(inpptr(fun));
        if funtyp(fun)==-1 then
          Code=[Code;
                '    if (phase==1){';
                '      outtbdptr = (double *)outtbptr['+string(nxx-1)+'];';
                '      if ((*outtbdptr)>0) {';
                '        ntvecm=1;';
                '      }';
                '      else {';
                '        ntvecm=2;';
                '      }';
                '    }'
               '     else {';
                '      ntvecm=block_'+rdnom+'['+string(i-1)+'].mode[0];';
                '    }']
        elseif funtyp(fun)==-2 then
          Code=[Code;
                '    if (phase==1){';
                '      outtbdptr = (double *)outtbptr['+string(nxx-1)+'];';
                '      ntvecm=(integer) (*outtbdptr);';
                '      if(ntvecm>'+string(ntvec)+'){ntvecm='+string(ntvec)+';}';
                '      if(ntvecm<1){ntvecm=1;}';
                '    }'
               '     else {';
                '      ntvecm=block_'+rdnom+'['+string(i-1)+'].mode[0];';
                '    }']
        else
          message('Block '+fun+' has funtyp '+string(funtyp(fun))+': not allowed')
        end

        Code=[Code;
              '    i2 = ntvecm + clkptr['+string(fun-1)+'] - 1;';
              '    '+rdnom+'_putevs(told, &i2, &ierr1);';
              '    if (ierr1 != 0) return 3;'
              cformatline('    '+rdnom+'ozdoit(told, outtbptr, iwa, kiwa, phase);',70)]
      end
    end
    Code($+1)='    break;';
  end

  Code=[Code;
        '  }';
        '  return 0;'
        '} /* ozdoit */'];
endfunction

function Code=make_zdoit(stalone)
  //les pointeurs de cpr :
  z=cpr.state.z;
  zcptr=cpr.sim.zcptr;
  pointi=cpr.state.pointi;
  funs=cpr.sim.funs;
  inpptr=cpr.sim.inpptr;
  clkptr=cpr.sim.clkptr;
  zord=cpr.sim.zord;
  xptr=cpr.sim.xptr;
  evtspt=cpr.state.evtspt;
  ordptr=cpr.sim.ordptr;
  ordclk=cpr.sim.ordclk;

  nzord=size(zord,1);
  sztvec=max(clkptr(2:$)-clkptr(1:$-1))+1;

  Code=['';
        '/*'+part('-',ones(1,40))+' zdoit.c */';
        'int '
         cformatline(rdnom+'zdoit(g,told, outtbptr, iwa, phase)',70);
        '';
        '     double *g, *told;';
        '     void **outtbptr;';
        '     integer *iwa, phase;';
        '{';
        '  /* System generated locals */';
        '  integer i,i2;';
        '';
        '  /* Local variables */';
        '  integer flag, keve, kiwa, nport, jj, j;';
        '  double  tvec['+string(sztvec)+'];';
        '  double  rdouttb['+string(lstsize(outtb)+1)+'];';
        '  double  *args[100];';
        '  double *outtbdptr;'
        '  integer sz[100];';
        '  integer ierr1;';
        '  integer nevprt;';
        '  integer ntvec, ntvecm;';
        '';
        '  /* Function Body */';
        '  kiwa = 0;';];

  //////////////////////////////////////////////////
  for ii=1:nzord
    fun=zord(ii,1);
    //flag=1 for all blocks
    //if outptr(fun+1)-outptr(fun)>0  then
      nclock=zord(ii,2);
      if or(fun==act) | or(fun==cap) then
        if stalone then
          Code=[Code;
                '  flag = 1 ;';
                '  nevprt='+string(nclock)+';';
                ' '+wfunclist(fun);
                '';
                ''];
        end
      else
        Code=[Code;
             '  flag = 1 ;';
             '  nevprt='+string(nclock)+';';
             ' '+wfunclist(fun);
             '';
             ''];
      end
    //end
    ntvec=clkptr(fun+1)-clkptr(fun);
    if ntvec >0  & funs(fun) <> 'bidon' then
      nxx=inplnk(inpptr(fun));
      if funtyp(fun)==-1 then
         Code=[Code;
               '    if (phase==1){';
               '      outtbdptr = (double *)outtbptr['+string(nxx-1)+']';
               '      if ((*outtbdptr)>0) {';
               '        ntvecm=1;';
               '      }';
               '      else {';
               '        ntvecm=2;';
               '      }';
               '    }else {';
               '      ntvecm=block_'+rdnom+'['+string(fun-1)+'].mode[0];';
               '    }']
      elseif funtyp(fun)==-2 then
         Code=[Code;
               '    if (phase==1){';
               '      outtbdptr = (double *)outtbptr['+string(nxx-1)+']';
               '      ntvecm=(integer)(*outtbdptr);';
               '      if(ntvecm>'+string(ntvec)+'){ntvecm='+string(ntvec)+';}';
               '      if(ntvecm<1){ntvecm=1;}';
               '    }else {';
               '      ntvecm=block_'+rdnom+'['+string(fun-1)+'].mode[0];';
               '    }']
      else
         message('Block '+fun+' has funtyp '+string(funtyp(fun))+': not allowed')
      end

      Code=[Code;
            '    i2 = ntvecm + clkptr['+string(fun-1)+'] - 1;';
            '    '+rdnom+'_putevs(told, &i2, &ierr1);';
            '    if (ierr1 != 0) return 3;'
             cformatline('    '+rdnom+'ozdoit(told, outtbptr, iwa, &kiwa, phase);',70)]
    end
  end //for ii=1:nzord

  Code=[Code;
        '  /*        update states derivatives */'];

  for ii=1:nzord
    fun=zord(ii,1);
    nclock=zord(ii,2);

    if funtyp(fun) > 0 then
      Code=[Code;
            ' if ( block_'+rdnom+'['+string(fun-1)+'].ng > 0){']
      if or(fun==act) | or(fun==cap) then
        if stalone then
          Code=[Code;
                '  flag = 9 ;';
                '  nevprt='+string(nclock)+';';
                ' '+wfunclist(fun);
                '';
                ''];
         end
      else
         Code=[Code;
               '  flag = 9 ;';
               '  nevprt='+string(nclock)+';';
               ' '+wfunclist(fun);
               '';
               ''];
      end
    else
      Code=[Code;
            ' if ( block_'+rdnom+'['+string(fun-1)+'].ng > 0){']
      if funtyp(fun)==-1 then
        Code=[Code;
              '  g['+string(zcptr(fun)-1)+']=outtb['+string(-1+(inplnk(inpptr(fun))))+'];'
              '  if(phase==1 && block_'+rdnom+'['+string(fun-1)+'].nmode > 0){'
              '    if (g['+string(zcptr(fun)-1)+'] <= 0.){'
              '      block_'+rdnom+'['+string(fun-1)+'].mode[0] = 2;'
              '    }'
              '    else {'
              '      block_'+rdnom+'['+string(fun-1)+'].mode[0] = 1;'
              '    }'
              '  }']
      elseif funtyp(fun)==-2 then
         Code=[Code;
               '  outtbdptr = (double *)outtbptr['+string(-1+(inplnk(inpptr(fun))))+'];';
               '  for (jj=0;jj<block_'+rdnom+'['+string(fun-1)+'].nevout-1;++jj) {'
               '    g['+string(zcptr(fun)-1)+'+jj]=(*outtbdptr)-(double)(jj+2);'
               '  }'
               '  if(phase==1 && block_'+rdnom+'['+string(fun-1)+'].nmode>0){'
               '    j=max(min((integer) (*outtbdptr),'
               '              block_'+rdnom+'['+string(fun-1)+'].nevout),1);'
               '    block_'+rdnom+'['+string(fun-1)+'].mode[0]= j;'
               '  }']
      end
    end
    Code($+1)=' }'
  end

  Code=[Code;
        '  for (i = 1; i <= kiwa; ++i) {';
        '    keve = iwa[i-1];';
        '    switch(keve) {'];
  maxkeve=size(ordptr,1)-1;
  for keve=1:maxkeve
    Code($+1)='    case '+string(keve)+':';
    for ii=ordptr(keve):ordptr(keve+1)-1
      fun=ordclk(ii,1);
      nclock=abs(ordclk(ii,2));
      Code=[Code;
         ' if ( block_'+rdnom+'['+string(fun-1)+'].ng > 0){']
      if funtyp(fun) > 0 then
        if or(fun==act) | or(fun==cap) then
          if stalone then
            Code=[Code;
                  '  flag = 9 ;';
                  '  nevprt='+string(nclock)+';';
                  ' '+wfunclist(fun);
                  '';
                  ''];
          end
        else
          Code=[Code;
                '  flag = 9 ;';
                '  nevprt='+string(nclock)+';';
                ' '+wfunclist(fun);
                '';
                ''];
        end
      else
        if funtyp(fun)==-1 then
          Code=[Code;
                '  outtbdptr = (double *)outtbptr['+string(-1+(inplnk(inpptr(fun))))+'];';
                '  g['+string(zcptr(fun)-1)+'] = (*outtbdptr);'
                '  if(phase==1 && block_'+rdnom+'['+string(fun-1)+'].nmode > 0){'
                '    if (g['+string(zcptr(fun)-1)+'] <= 0.){'
                '      block_'+rdnom+'['+string(fun-1)+'].mode[0] = 2;'
                '    }'
                '    else {'
                '      block_'+rdnom+'['+string(fun-1)+'].mode[0] = 1;'
                '    }'
                '  }'];
        elseif funtyp(fun)==-2 then
          Code=[Code;
                '  outtbdptr = (double *)outtbptr['+string(-1+(inplnk(inpptr(fun))))+'];';
                '  for (jj=0;jj<block_'+rdnom+'['+string(fun-1)+'].nevout-1;++jj) {'
                '    g['+string(zcptr(fun)-1)+'+jj]=(*outtbdptr)-(double)(jj+2);'
                '  }'
                '  if(phase==1 && block_'+rdnom+'['+string(fun-1)+'].nmode>0){'
                '    j=max(min((integer) (*outtbdptr),'
                '              block_'+rdnom+'['+string(fun-1)+'].nevout),1);'
                '    block_'+rdnom+'['+string(fun-1)+'].mode[0]= j;'
                '  }']
        end
      end
      Code($+1)=' }'
    end
    Code($+1)='    break;';
  end

  Code=[Code;
        '    }'; //switch
        '  }';
        '  return 0;'
        '';
        '} /* zdoit */';
        ''];
endfunction

//utilitary fonction used to format long C instruction
//t : a string containing a C instruction
//l : max line length allowed

// Copyright INRIA
//Author : Rachid Djenidi
function t1=cformatline(t ,l)

  sep=[',','+']
  l1=l-2
  t1=[]
  kw=strindex(t,' ')
  nw=0
  if kw<>[] then
    if kw(1)==1 then // there is leading blanks
      k1=find(kw(2:$)-kw(1:$-1)<>1)
      if k1==[] then // there is a single blank
	nw=1
      else
	nw=kw(k1(1))
      end
    end
  end
  t=part(t,nw+1:length(t));
  bl=part(' ',ones(1,nw))
  l1=l-nw;first=%t
  while %t 
    if length(t)<=l then t1=[t1;bl+t],return,end
    k=strindex(t,sep);
    if k==[] then t1=[t1;bl+t],return,end
    k($+1)=length(t)+1 // positions of the commas
    i=find(k(1:$-1)<=l&k(2:$)>l) //nearest left comma (reltively to l)
    if i==[] then i=1,end
    t1=[t1;bl+part(t,1:k(i))]
    t=part(t,k(i)+1:length(t))
    if first then l1=l1-2;bl=bl+'  ';first=%f;end
  end
endfunction

//used in do_compile_superblock
function vec=codebinaire(v,szclkIN)
  vec=zeros(1,szclkIN)
  for i=1:szclkIN
    w=v/2;
    vec(i)=v-2*int(w);
    v=int(w);
  end
endfunction

function t=filetype(m)
  m=int32(m)
  filetypes=['Directory','Character device','Block device',...
             'Regular file','FIFO','Symbolic link','Socket']
  bits=[16384,8192,24576,32768,4096,40960,49152]
  m=int32(m)&int32(61440)
  t=filetypes(find(m==int32(bits)))
endfunction

//get_blank : return blanks with a length
//            of the given input string
//
//input : str : a string
//
//output : txt : blanks
//
//16/06/07 Author : A.Layec
//Copyright INRIA
function [txt] = get_blank(str)
 txt='';
 for i=1:length(str)
     txt=txt+' ';
 end
endfunction

// get_comment : return a C comment
//               for generated code
//
//input : typ : a string
//        param : a list
//
//output : a C comment
//
//16/06/07 Author : A.Layec
//Copyright INRIA
function [txt]=get_comment(typ,param)
  txt = [];
  select typ
    //** main flag
    case 'flag' then
        select param(1)
          case 1 then
             txt = '/* Output computation */'
          case 2 then
             txt = '/* State computation */'
        end
    //** blk calling sequence
    case 'call_blk' then
        txt = ['/* Call of '''+param(1) + ...
               ''' (type '+string(param(2))+' - blk nb '+...
                    string(param(3))];
        if ztyp(param(3)) then
          txt=txt+' - with zcross) */';
        else
          txt=txt+') */';
        end
    //** proto calling sequence
    case 'proto_blk' then
        txt = ['/* prototype of '''+param(1) + ...
               ''' (type '+string(param(2))];
        if ztyp(param(3)) then
          txt=txt+' - with zcross) */';
        else
          txt=txt+') */';
        end
    //** ifthenelse calling sequence
    case 'ifthenelse_blk' then
        txt = ['/* Call of ''if-then-else'' blk (blk nb '+...
                    string(param(1))+') */']
    //** eventselect calling sequence
    case 'evtselect_blk' then
        txt = ['/* Call of ''event-select'' blk (blk nb '+...
                    string(param(1))+') */']
    //** set block structure
    case 'set_blk' then
        txt = ['/* set blk struc. of '''+param(1) + ...
               ''' (type '+string(param(2))+' - blk nb '+...
                    string(param(3))+') */'];
    else
      break;
  end
endfunction

//mat2c_typ : matrix to C type
//sci2c_ttyp : get the C string of a scicos type
//
//input : outtb : a matrix
//
//output : txt : the string of the C scicos type
//               of the data of outtb
//
//16/06/07 Author : A.Layec
//Copyright INRIA
function [txt]=mat2c_typ(outtb)
 select type(outtb)
   //real matrix
   case 1 then
      if isreal(outtb) then
        txt = "double"
      else
        txt = "double"
      end
   //integer matrix
   case 8 then
      select typeof(outtb)
         case 'int32' then
           txt = "long"
         case 'int16' then
           txt = "short"
         case 'int8' then
           txt = "char"
         case 'uint32' then
           txt = "unsigned long"
         case 'uint16' then
           txt = "unsigned short"
         case 'uint8' then
           txt = "unsigned char"
      end
   else
     break;
 end
endfunction

//mat2scs_c_nb  matrix to scicos C number (sci2sci_n)
//
//input : outtb : a matrix
//
//output : c_nb : the scicos C number
//
//16/06/07 Author : A.Layec
//Copyright INRIA
function [c_nb]=mat2scs_c_nb(outtb)
 select type(outtb)
   //real matrix
   case 1 then
      if isreal(outtb) then
        c_nb = 10
      else
        c_nb = 11
      end
   //integer matrix
   case 8 then
      select typeof(outtb)
         case 'int32' then
           c_nb = 84
         case 'int16' then
           c_nb = 82
         case 'int8' then
           c_nb = 81
         case 'uint32' then
           c_nb = 814
         case 'uint16' then
           c_nb = 812
         case 'uint8' then
           c_nb = 811
      end
   else
     break;
 end
endfunction

//mat2scs_c_ptr matrix to scicos C ptr (sci2c_typ)
//
//input : outtb : a matrix
//
//output : txt : the string of the C scicos type
//               of the data of outtb
//
//16/06/07 Author : A.Layec
//Copyright INRIA
function [txt]=mat2scs_c_ptr(outtb)
 select type(outtb)
   //real matrix
   case 1 then
      if isreal(outtb) then
        txt = "SCSREAL_COP"
      else
        txt = "SCSCOMPLEX_COP"
      end
   //integer matrix
   case 8 then
      select typeof(outtb)
         case 'int32' then
           txt = "SCSINT32_COP"
         case 'int16' then
           txt = "SCSINT16_COP"
         case 'int8' then
           txt = "SCSINT8_COP"
         case 'uint32' then
           txt = "SCSUINT32_COP"
         case 'uint16' then
           txt = "SCSUINT16_COP"
         case 'uint8' then
           txt = "SCSUINT8_COP"
      end
   else
     break;
 end
endfunction

//mat2scs_c_typ matrix to scicos C type
//
//input : outtb : a matrix
//
//output : txt : the string of the C scicos type
//               of the data of outtb
//
//16/06/07 Author : A.Layec
//Copyright INRIA
function [txt]=mat2scs_c_typ(outtb)
 select type(outtb)
   //real matrix
   case 1 then
      if isreal(outtb) then
        txt = "SCSREAL_N"
      else
        txt = "SCSCOMPLEX_N"
      end
   //integer matrix
   case 8 then
      select typeof(outtb)
         case 'int32' then
           txt = "SCSINT32_N"
         case 'int16' then
           txt = "SCSINT16_N"
         case 'int8' then
           txt = "SCSINT8_N"
         case 'uint32' then
           txt = "SCSUINT32_N"
         case 'uint16' then
           txt = "SCSUINT16_N"
         case 'uint8' then
           txt = "SCSUINT8_N"
      end
   else
     break;
 end
endfunction

//scs_c_n2c_fmt : scicos C number to C format
//
//input : c_nb : a C scicos type
//
//output : txt : the string of the C format string
//               of the data of outtb
//
//16/06/07 Author : A.Layec
//Copyright INRIA
function [txt]=scs_c_n2c_fmt(c_nb)
 select c_nb
   //real matrix
   case 10 then
     txt = '%f';
   //complex matrix
   case 11 then
     txt = '%f,%f';
   //int8 matrix
   case 81 then
     txt = '%d';
   //int16 matrix
   case 82 then
     txt = '%d';
   //int32 matrix
   case 84 then
     txt = '%d';
   //uint8 matrix
   case 811 then
     txt = '%d';
   //uint16 matrix
   case 812 then
     txt = '%d';
   //uint32 matrix
   case 814 then
     txt = '%d';
   else
     txt='%f'
     break;
 end
endfunction

//scs_c_n2c_typ scicos C number to C type
//
//input : c_nb : a C scicos number
//
//output : txt : the string of the C format string
//               of the data of outtb
//
//16/06/07 Author : A.Layec
//Copyright INRIA
function [txt]=scs_c_n2c_typ(c_nb)
 select c_nb
   //real matrix
   case 10 then
     txt = 'double';
   //complex matrix
   case 11 then
     txt = 'double';
   //int8 matrix
   case 81 then
     txt = 'char';
   //int16 matrix
   case 82 then
     txt = 'short';
   //int32 matrix
   case 84 then
     txt = 'long';
   //uint8 matrix
   case 811 then
     txt = 'unsigned char';
   //uint16 matrix
   case 812 then
     txt = 'unsigned short';
   //uint32 matrix
   case 814 then
     txt = 'unsigned long';
   else
     txt='double'
     break;
 end
endfunction

//scs_c_nb2scs_nb : scicos C number to scicos number
//
//input : c_nb  : the scicos C number type
//
//output : scs_nb : the scilab number type
//
//16/06/07 Author : A.Layec
//Copyright INRIA
function [scs_nb]=scs_c_nb2scs_nb(c_nb)
 scs_nb=zeros(size(c_nb,1),size(c_nb,2));
 for i=1:size(c_nb,1)
   for j=1:size(c_nb,2)
     select (c_nb(i,j))
       case 10 then
         scs_nb(i,j) = 1
       case 11 then
         scs_nb(i,j) = 2
       case 81 then
         scs_nb(i,j) = 5
       case 82 then
         scs_nb(i,j) = 4
       case 84 then
         scs_nb(i,j) = 3
       case 811 then
         scs_nb(i,j) = 8
       case 812 then
         scs_nb(i,j) = 7
       case 814 then
         scs_nb(i,j) = 6
       else
         scs_nb(i,j) = 1
     end
   end
 end
endfunction

//used in do_compile_superblock
function XX=update_block(XX)
  execstr('o='+rdnom+'_c(''define'')')
  XX.model=o.model
  XX.gui=rdnom+'_c';
  XX.graphics.gr_i=o.graphics.gr_i
endfunction


