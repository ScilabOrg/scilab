//Generate the model file
//
//Author : Maarten Meijer
//
//
function ok=generateModel(nrParameters, nrInitialValues, nrVariables, nrStates, nrRates);

  //** Generate model.c C-code
  Code=makeModelC(nrParameters, nrInitialValues, nrVariables, nrStates, nrRates);
 
  //** Create model.c */
  ierr=execstr('mputl(Code,rpat+''/src/'+XX_VARPREFIX+'model.c'')','errcatch');
  if ierr<>0 then
    message(lasterror())
    ok=%f
  end

  //** Generate model.h C-code
  Code=makeModelH(nrParameters, nrInitialValues, nrVariables, nrStates, nrRates);
 
  //**  Create model.c */
  ierr=execstr('mputl(Code,rpat+''/src/'+XX_VARPREFIX+'model.h'')','errcatch');
  if ierr<>0 then
    message(lasterror())
    ok=%f
  end
endfunction

//** make code for model.h */
function Code=makeModelH(nrParameters, nrInitialValues, nrVariables, nrStates, nrRates)
	Code=[makeHeaderComment(XX_VARPREFIX+'model.h')
		''
		'/* include Scilab type definitions */'
		'#include '"ivcscicos_block.h'"'
		''
		'/* 20-sim include files */'
		'#include '"xxtypes.h'"'
		''
		'/* simulation variables */'
		'extern XXDouble '+XX_VARPREFIX+'start_time;'
		'extern XXDouble '+XX_VARPREFIX+'finish_time;'
		'extern XXDouble '+XX_VARPREFIX+'step_size;'
		'extern XXDouble '+XX_VARPREFIX+'time;'
		'extern XXDouble '+XX_VARPREFIX+'executionTime;'
		'extern XXInteger '+XX_VARPREFIX+'steps;'
		'extern XXBoolean '+XX_VARPREFIX+'initialize;'
		'extern XXBoolean '+XX_VARPREFIX+'stop_simulation;'
		''
		'/* variable arrays */'
		'extern XXDouble '+XX_PARAMETER_ARRAY_NAME+'['+string(nrParameters)+' + 1];'
		'extern XXDouble '+XX_INIT_VALUE_ARRAY_NAME+'['+string(nrInitialValues)+' + 1];'
		'extern XXDouble '+XX_VARIABLE_ARRAY_NAME+'['+string(nrVariables)+' + 1];'
		'extern XXDouble '+XX_STATE_ARRAY_NAME+'['+string(nrStates)+' + 1];'
		'extern XXDouble '+XX_RATE_ARRAY_NAME+'['+string(nrRates)+' + 1];'
        'extern XXDouble '+XX_WORK_ARRAY_NAME+'[];'
		''
		'/* '+XX_VARPREFIX+'model.c intern variables */'
		'scicos_block block_'+rdnom+'['+string(nblk)+'];'
		'int local_flag;'
		'double* args[2];'
		'int nport;'
		'int nevprt;'
		'int solver;'
		''
		'/* initialization methods */'
		'int '+XX_FUNCTIONPREFIX+'ModelInitialize (void);'
		''
		'/* Calculation methods */'
		'void '+XX_FUNCTIONPREFIX+'Calculate (void);'
		''
		'/* Termination method */'
		'void '+XX_FUNCTIONPREFIX+'ModelTerminate (void);'
		''
		'/* Function prototypes for particular block calculations */'
		Protostalone
		''];
		
	//**  Add integration methods if necessary */
	if x<>[] then
    Code=[Code
          '/* Integration methods  */'
          'int '+rdnom+'simblk(XXDouble , double *, double *);'
          'int ode1(XXDouble, XXDouble);'
          'int ode2(XXDouble, XXDouble);'
          'int ode4(XXDouble, XXDouble);'
		  '']
	end
	
	//** Add additional Scilab functionallity
	Code=[Code
          '/* additional Scilab functions */'
          'int get_phase_simulation();'
		  'void set_block_error(int );'
		  'double get_scicos_time();'
          ''];

endfunction

//**  make code for model.c */
function Code=makeModelC(nrParameters, nrInitialValues, nrVariables, nrStates, nrRates)

  x=cpr.state.x;
  modptr=cpr.sim.modptr;
  rpptr=cpr.sim.rpptr;
  ipptr=cpr.sim.ipptr;
  opptr=cpr.sim.opptr;
  rpar=cpr.sim.rpar;
  ipar=cpr.sim.ipar;
  opar=cpr.sim.opar;
  oz=cpr.state.oz;
  ordptr=cpr.sim.ordptr;
  oord=cpr.sim.oord;
  zord=cpr.sim.zord;
  iord=cpr.sim.iord;
  tevts=cpr.state.tevts;
  evtspt=cpr.state.evtspt;
  zptr=cpr.sim.zptr;
  clkptr=cpr.sim.clkptr;
  ordptr=cpr.sim.ordptr;
  pointi=cpr.state.pointi;
  funs=cpr.sim.funs;
  noord=size(cpr.sim.oord,1);
  nzord=size(cpr.sim.zord,1);
  niord=size(cpr.sim.iord,1);

  Indent='  ';
  Indent2=Indent+Indent;
  BigIndent='          ';

  work=zeros(nblk,1)
  Z=[z;zeros(lstsize(outtb),1);work]';
  nX=size(x,'*');
  nztotal=size(z,1);

  stalone = %t;

	Code=[makeHeaderComment(XX_VARPREFIX+'model.c')
		''
		'#include <math.h>'
		''
		'/* 20-sim include files */'
		'#include '"xxtypes.h'"'
		''
		'/* Scilab generated include files */'
		'#include '"ivcscicos_block.h'"'
		'#include '"'+XX_VARPREFIX+'model.h'"'
		''
		'/* Function prototypes for particular block calculations */'
		Protostalone
		''
		'/* the global variables */'
		'XXDouble '+XX_VARPREFIX+'start_time = 0;'
		'XXDouble '+XX_VARPREFIX+'finish_time;'
		'XXDouble '+XX_VARPREFIX+'step_size;'
		'XXDouble '+XX_VARPREFIX+'time;'
		'XXDouble '+XX_VARPREFIX+'executionTime = 0;'
		'XXInteger '+XX_VARPREFIX+'steps;'
		'XXBoolean '+XX_VARPREFIX+'initialize;'
		'XXBoolean '+XX_VARPREFIX+'stop_simulation = XXFALSE;'
		''
		'/* declare variable arrays */'
		'XXDouble '+XX_PARAMETER_ARRAY_NAME+'['+string(nrParameters)+' + 1];'
		'XXDouble '+XX_INIT_VALUE_ARRAY_NAME+'['+string(nrInitialValues)+' + 1];'
		'XXDouble '+XX_VARIABLE_ARRAY_NAME+'['+string(nrVariables)+' + 1];'
		'XXDouble '+XX_STATE_ARRAY_NAME+'['+string(nrStates)+' + 1];'
		'XXDouble '+XX_RATE_ARRAY_NAME+'['+string(nrRates)+' + 1];'
		''
		'/* initialize outtbptr array */']
	
	Code_var=[];
	for i=1:lstsize(outtb)
		Code_var=[Code_var+strcat(string(outtb(i)(:)),',')+', ']	
	end
		
	Code=[Code
		'XXDouble outtbptr[] = {'+Code_var+'};'
		''
		'/* declare and initialize work */'
		'XXDouble '+XX_WORK_ARRAY_NAME+'[] = {'+strcat(string( zeros(nblk,1) ),",") + '};'
		''];

	//*** Real parameters ***//
	  objectParameterOffset = 0;
	  if size(rpar,1) <> 0 then
		for i=1:(length(rpptr)-1)
		  if rpptr(i+1)-rpptr(i)>0  then
			objectParameterOffset = objectParameterOffset + rpptr(i+1)-rpptr(i);
		  end
		end
	  end
	//*************************//
		
  //*** Continuous state ***//
  if x <> [] then
   Code=[Code;
        '/* define continuous states */'
        'XXDouble '+XX_STATE_ARRAY_NAME+'[]={'+strcat(string(x),',')+'};'
		''
		'/* define continous rates */'
        'XXDouble '+XX_RATE_ARRAY_NAME+'[]={'+strcat(string(x),',')+'};'
		''
        'static int c__1 = 1;'
        'static double c_b14 = 0.;'
        'static int neq='+string(nX)+';'
        '']
  end
  //************************//

		
 Code=[Code;
		''
        '/* Table of constant values */'
        'static int nrd_'+string(0:maxtotal)'+' = '+string(0:maxtotal)'+';']

  if maxtotal<10 then
    Code=[Code;
          'static int nrd_10 = 10;']
  end
  if maxtotal<11 then
    Code=[Code;
          'static int nrd_11 = 11;']
  end

  if maxtotal<81 then
    Code=[Code;
          'static int nrd_81 = 81;']
  end
  if maxtotal<82 then
    Code=[Code;
          'static int nrd_82 = 82;']
  end
  if maxtotal<84 then
    Code=[Code;
          'static int nrd_84 = 84;']
  end
  if maxtotal<811 then
    Code=[Code;
          'static int nrd_811 = 811;']
  end
  if maxtotal<812 then
    Code=[Code;
          'static int nrd_812 = 812;']
  end
  if maxtotal<814 then
    Code=[Code;
          'static int nrd_814 = 814;']
  end

  Code=[Code;
        ''
        '/* Some general static variables */'
        'static double zero=0;'
        'static double w[1];'
        'static int aaa=0, bbb=0;'
		'int local_flag;'
		'double* args[2];'
		'int nport;'
		'int nevprt = 1;'
		''
		'/* initialize the solver type */'
        'int solver;'
        ''];

  Code=[Code;
        make_static_standalone42()];
		
  //** declaration of oz
  Code_oz = [];
  for i=1:lstsize(oz)
    if mat2scs_c_nb(oz(i)) <> 11 then
      Code_oz=[Code_oz;
               cformatline('  '+mat2c_typ(oz(i))+...
                           ' oz_'+string(i)+'[]={'+...
                           strcat(string(oz(i)(:)),',')+'};',70)]
    else //** cmplx test
      Code_oz=[Code_oz;
               cformatline('  '+mat2c_typ(oz(i))+...
                           ' oz_'+string(i)+'[]={'+...
                           strcat(string([real(oz(i)(:));
                                          imag(oz(i)(:))]),',')+'};',70)]
    end
  end

  if Code_oz <> [] then
    Code=[Code;
          '  /* oz declaration */'
          Code_oz]
  end

  //** declaration of outtb
  Code_outtb = [];
  for i=1:lstsize(outtb)
    if mat2scs_c_nb(outtb(i)) <> 11 then
      Code_outtb=[Code_outtb;
                  cformatline('  '+mat2c_typ(outtb(i))+...
                              ' outtb_'+string(i)+'[]={'+...
                              strcat(string(outtb(i)(:)),',')+'};',70)]
    else //** cmplx test
      Code_outtb=[Code_outtb;
                  cformatline('  '+mat2c_typ(outtb(i))+...
                              ' outtb_'+string(i)+'[]={'+...
                              strcat(string([real(outtb(i)(:));
                                             imag(outtb(i)(:))]),',')+'};',70)]
    end
  end

	Code=[Code
		'/* outtbptr and work pointer declaration */'
		'void **'+XX_VARPREFIX+'outtbptr;'
		'void **'+XX_VARPREFIX+'work;'
		''
		'/* declare and initialize variable array */']
		
  //** declaration of outtb
  Code_outtb = [];
  for i=1:lstsize(outtb)
    if mat2scs_c_nb(outtb(i)) <> 11 then
      Code_outtb=[Code_outtb+strcat(string(outtb(i)(:)),',')]
    else //** cmplx test
      Code_outtb=[Code_outtb+strcat(string([real(outtb(i)(:));
                                             imag(outtb(i)(:))]),',')]
    end
	Code_outtb=[Code_outtb+', ']
  end
	Code=[Code
		'XXDouble '+XX_VARIABLE_ARRAY_NAME+'[]= {'+Code_outtb+'};']
		
		//** initialization */
	Code=[Code;
		''
		'int '+XX_FUNCTIONPREFIX+'ModelInitialize(void)'
		'{'
		'  /* set the solver as the first entry in the initial values table */'
		'  solver = (int) '+XX_INIT_VALUE_ARRAY_NAME+'[0];'
          ''
		'  /* Get work ptr of blocks */'
		'  '+XX_VARPREFIX+'work = (void **)('+XX_WORK_ARRAY_NAME+');'
		  ''
		'  /* Get outtbptr of blocks */'
		'  '+XX_VARPREFIX+'outtbptr = (void **)(outtbptr);']
		  
		  offset=0
		  for i=1:lstsize(outtb)
			Code=[Code
					'  '+XX_VARPREFIX+'outtbptr['+string(outlnk(outptr(i)-1+size(outtb(i),'*')) -1)+'] = (void *) &'+XX_VARIABLE_ARRAY_NAME+'['+string(offset)+'];']
			offset = offset + size(outtb(i),'*')
		  end
		  
	Code=[Code		
		getBlockInitialization()
		''];

  for kf=1:nblk
    if or(kf==act) | or(kf==cap) then
        txt = call_block42(kf,0,4);
        if txt <> [] then
          Code=[Code;
                '';
                '  '+txt];
        end
    else
      txt = call_block42(kf,0,4);
      if txt <> [] then
        Code=[Code;
              '';
              '  '+txt];
      end
    end
  end

  //** cst blocks and it's dep
  txt=write_code_idoit()

  if txt<>[] then
    Code=[Code;
          ''
          '    /* Initial blocks must be called with flag 1 */'
          txt];
  end
		
		//** end of initialization function */
		Code = [Code;
		''
		'    /*update model*/'
		'    '+XX_FUNCTIONPREFIX+'Calculate();'
		''
		'    /* return 1 if correct initialized */'
		'    return 1;'
		'}'];

		//** begin of model calculation function */
		Code = [Code;
			''
			'void '+XX_FUNCTIONPREFIX+'Calculate (void)'
			'{'];
			
  //** find source activation number
  blks=find(funtyp>-1);
  evs=[];

  for blk=blks
    for ev=clkptr(blk):clkptr(blk+1)-1
      if funs(blk)=='bidon' then
        if ev > clkptr(howclk) -1
         evs=[evs,ev];
        end
      end
    end
  end

  //** flag 1,2,3
  // 1,2,3: Output update, State Update, Event scheduler
  
  numberOfEvents=0;
  for nrBlocks=1:nblk,
	name = string(nrBlocks)+' '+funs(nrBlocks)
  
  	if (cpr.sim.zptr(nrBlocks+1)-cpr.sim.zptr(nrBlocks))>0
		numberOfEvents=numberOfEvents+1;
	end
  end
  
  disp(['Number of events:' msprintf("%d",numberOfEvents) ]);
  
  for flag=[1,2,3]

    txt3=[]
	eventNumber=0;

    //** continuous time blocks must be activated
    //** for flag 1
    if flag==1 then
      txt = write_code_cdoit(flag);

      if txt <> [] then
        txt3=[''
              '    '+get_comment('ev',list(0))
              txt;
             ];
      end
    end

    //** blocks with input discrete event must be activated
    //** for flag 1, 2 and 3
    if size(evs,2)>=1 then
      txt4=[]
      //**
      for ev=evs
        txt2=write_code_doit(ev,flag);
        if txt2<>[] then
          //** adjust event number because of bidon block
          new_ev=ev-(clkptr(howclk)-1)
          //**
          txt4=[txt4
                Indent2+get_comment('ev',list(new_ev))
				Indent2+'/* If the remainder of sci_time/event_time < step size */'
				Indent2+'if(fmod(sci_time, '+XX_PARAMETER_ARRAY_NAME+...
				'['+string(nrParameters - numberOfEvents)+' + '+string(eventNumber)+'])'+...
				'< sci_step_size)'
				Indent2+'{'
                txt2
                Indent2+'}';'']
		  eventNumber=eventNumber+1;
        end
      end

      //**
      if txt4 <> [] then
        txt3=[txt3;
              Indent+'  /* Discrete activations */'
              txt4
              ''];
      end
    end

    //**
    if txt3<>[] then
      Code=[Code;
            '    '+get_comment('flag',list(flag))
            txt3];
    end
  end

  if x<>[] then
    Code=[Code
          ''
          '   /* integrate one time'
		  '    TODO: Make faster continuous sample-times possible '
          '    */'
          '      switch (solver) {'
          '      case 1:'
          '        ode1('+XX_VARPREFIX+'time, '+XX_VARPREFIX+'step_size);'
          '        break;'
          '      case 2:'
          '        ode2('+XX_VARPREFIX+'time, '+XX_VARPREFIX+'step_size);'
          '        break;'
          '      case 3:'
          '        ode4('+XX_VARPREFIX+'time, '+XX_VARPREFIX+'step_size);'
          '        break;'
          '      default :'
          '        ode4('+XX_VARPREFIX+'time, '+XX_VARPREFIX+'step_size);'
          '        break;'
          '        }'
          '']
  end

  //** fix bug provided by Roberto Bucher
  //** Alan, 13/10/07
  if nX <> 0 then
    Code=[Code;
          ''
          '    /* update ptrs of continuous array */']
    for kf=1:nblk
      nx=xptr(kf+1)-xptr(kf);  //** number of continuous state
      if nx<>0 then
        Code=[Code;
              '    block_'+rdnom+'['+string(kf-1)+'].nx = '+...
                string(nx)+';';
              '    block_'+rdnom+'['+string(kf-1)+'].x  = '+...
               '&('+XX_STATE_ARRAY_NAME+'['+string(xptr(kf)-1)+']);'
              '    block_'+rdnom+'['+string(kf-1)+'].xd = '+...
               '&('+XX_RATE_ARRAY_NAME+'['+string(xptr(kf)-1)+']);']
      end
    end
  end

  //**end of calculation function */
  Code = [Code;
		'}'
		''];
		
	//** begin of termination function */
	Code = [Code;
		'void '+XX_FUNCTIONPREFIX+'ModelTerminate(void)'
		'{'];
		
  //** flag 5
  Code=[Code;
        '  '+get_comment('flag',list(5))]

  for kf=1:nblk
    if or(kf==act) | or(kf==cap) then
        txt = call_block42(kf,0,5);
        if txt <> [] then
          Code=[Code;
                '';
                '  '+txt];
        end
    else
      txt = call_block42(kf,0,5);
      if txt <> [] then
        Code=[Code;
              '';
              '  '+txt];
      end
    end
  end
  
  //** End of termination function */
  Code=[Code;
		'}'
		''
		''];
  
	//** Add integration functions if necessary */
 if (x <> []) then
    Code=[Code;
          'int '+rdnom+'simblk(XXDouble t, XXDouble *'+XX_STATE_ARRAY_NAME+', XXDouble *'+XX_RATE_ARRAY_NAME+')'
          '     /*'
          '      *  !purpose'
          '      *  compute state derivative of the continuous part'
          '      *  !calling sequence'
          '      *  neq   : integer the size of the  continuous state'
          '      *  t     : current time'
          '      *  '+XX_STATE_ARRAY_NAME+'     : double precision vector whose contains the continuous state'
          '      *  '+XX_RATE_ARRAY_NAME+'    : double precision vector, contain the computed derivative'
          '      *  of the state'
          '      */'
          '{'
          '  int phase=2;'
          '  int local_flag;'
          '  int nport;'
          '  int nevprt=1;'
		  ''
          '  double* args[2];'
          '']

    Code=[Code;
          '    '+get_comment('update_xd',list())]

    for kf=1:nblk
      if (xptr(kf+1)-xptr(kf)) > 0 then
        Code=[Code;
              '    block_'+rdnom+'['+string(kf-1)+'].x='+...
                '&('+XX_STATE_ARRAY_NAME+'['+string(xptr(kf)-1)+']);'
              '    block_'+rdnom+'['+string(kf-1)+'].xd='+...
                '&('+XX_RATE_ARRAY_NAME+'['+string(xptr(kf)-1)+']);']
      end
    end

    Code=[Code;
          ''
          write_code_odoit(1)
          write_code_odoit(0)
         ]

    Code=[Code
          ''
          '  return 0;'
          '}'
          ''
          '/* Euler''s Method */'
          'int ode1(XXDouble t, XXDouble h)'
          '{'
          '  int i;'
          ''
          '  /**/'
          '  '+rdnom+'simblk(t,'+XX_STATE_ARRAY_NAME+', '+XX_RATE_ARRAY_NAME+');'
          ''
          '  for (i=0;i<neq;i++) {'
          '   '+XX_STATE_ARRAY_NAME+'[i]='+XX_STATE_ARRAY_NAME+'[i]+h*'+XX_RATE_ARRAY_NAME+'[i];'
          '  }'
          ''
          '  return 0;'
          '}'
          ''
          '/* Heun''s Method */'
          'int ode2(XXDouble t, XXDouble h)'
          '{'
          '  int i;'
          '  double y['+string(nX)+'],yh['+string(nX)+'],temp,f0['+string(nX)+'],th;'
          ''
          '  /**/'
          '  memcpy(y,'+XX_STATE_ARRAY_NAME+',neq*sizeof(double));'
          '  memcpy(f0,'+XX_RATE_ARRAY_NAME+',neq*sizeof(double));'
          ''
          '  /**/'
          '  '+rdnom+'simblk(t,y, f0);'
          ''
          '  /**/'
          '  for (i=0;i<neq;i++) {'
          '    '+XX_STATE_ARRAY_NAME+'[i]=y[i]+h*f0[i];'
          '  }'
          '  th=t+h;'
          '  for (i=0;i<neq;i++) {'
          '    yh[i]=y[i]+h*f0[i];'
          '  }'
          '  '+rdnom+'simblk(th,yh, '+XX_RATE_ARRAY_NAME+');'
          ''
          '  /**/'
          '  temp=0.5*h;'
          '  for (i=0;i<neq;i++) {'
          '    '+XX_STATE_ARRAY_NAME+'[i]=y[i]+temp*(f0[i]+'+XX_RATE_ARRAY_NAME+'[i]);'
          '  }'
          ''
          '  return 0;'
          '}'
          ''
          '/* Fourth-Order Runge-Kutta (RK4) Formula */'
          'int ode4(XXDouble t, XXDouble h)'
          '{'
          '  int i;'
          '  double y['+string(nX)+'],yh['+string(nX)+'],'+...
            'temp,f0['+string(nX)+'],th,th2,'+...
            'f1['+string(nX)+'],f2['+string(nX)+'];'
          ''
          '  /**/'
          '  memcpy(y,'+XX_STATE_ARRAY_NAME+',neq*sizeof(double));'
          '  memcpy(f0,'+XX_RATE_ARRAY_NAME+',neq*sizeof(double));'
          ''
          '  /**/'
          '  '+rdnom+'simblk(t,y, f0);'
          ''
          '  /**/'
          '  for (i=0;i<neq;i++) {'
          '    '+XX_STATE_ARRAY_NAME+'[i]=y[i]+h*f0[i];'
          '  }'
          '  th2=t+h/2;'
          '  for (i=0;i<neq;i++) {'
          '    yh[i]=y[i]+(h/2)*f0[i];'
          '  }'
          '  '+rdnom+'simblk(th2,yh, f1);'
          ''
          '  /**/'
          '  temp=0.5*h;'
          '  for (i=0;i<neq;i++) {'
          '    '+XX_STATE_ARRAY_NAME+'[i]=y[i]+temp*f1[i];'
          '  }'
          '  for (i=0;i<neq;i++) {'
          '    yh[i]=y[i]+(h/2)*f1[i];'
          '  }'
          '  '+rdnom+'simblk(th2,yh, f2);'
          ''
          '  /**/'
          '  for (i=0;i<neq;i++) {'
          '    '+XX_STATE_ARRAY_NAME+'[i]=y[i]+h*f2[i];'
          '  }'
          '  th=t+h;'
          '  for (i=0;i<neq;i++) {'
          '    yh[i]=y[i]+h*f2[i];'
          '  }'
          '  '+rdnom+'simblk(th2,yh, '+XX_RATE_ARRAY_NAME+');'
          ''
          '  /**/'
          '  temp=h/6;'
          '  for (i=0;i<neq;i++) {'
          '    '+XX_STATE_ARRAY_NAME+'[i]=y[i]+temp*(f0[i]+2.0*f1[i]+2.0*f2[i]+'+XX_RATE_ARRAY_NAME+'[i]);'
          '  }'
          ''
          'return 0;'
          '}'
		  ''
		  '/* This function specifiec the simulation phase:'
		  '    it returns 1 in normal time'
		  '    it returns 2 in advancing time'
		  '*/']
	end
	
	//** Add additional Scilab functionallity
	Code=[Code
		  ''
		  '/* Add additional Scilab functionallity */'
		  'int get_phase_simulation()'
		  '{'
		  '    return 1;'
		  '}'
		  ''
		  'void set_block_error(int err)'
		  '{'
		  '  return;'
		  '}'
		  'double get_scicos_time()'
		  '{'
		  '  return sci_time;'
		  '}'
		  '']
  
endfunction

//generates  static table definitions
//
//Author : Rachid Djenidi, Alan Layec
function txt=make_static_standalone42()
  txt=[];

  txt=[txt;
       'char s[1],sf[1],se[1],ss[1],**p;'
       'static int errflg = 0;'
       'static char *optarg = NULL ;'
       'static int optind = 1, offset = 0 ;'
       'scicos_block block_'+rdnom+'['+string(nblk)+'];'
       ''];

  
  //*** Integer parameters ***//
  if size(ipar,1) <> 0 then
    txt=[txt;
         '/* def integer parameters */'
         'static int IPAR1[ ] = {'];

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
        txt($+1)='/* Routine name of block: '+strcat(string(cpr.sim.funs(i)));
        txt($+1)=' * Gui name of block: '+strcat(string(OO.gui));
        txt($+1)=' * Compiled structure index: '+strcat(string(i));
        if stripblanks(OO.model.label)~=emptystr() then
          txt=[txt;cformatline(' * Label: '+strcat(string(OO.model.label)),70)];
        end

        if stripblanks(OO.graphics.exprs(1))~=emptystr() then
          txt=[txt;
               cformatline(' * Exprs: '+strcat(OO.graphics.exprs(1),","),70)];
        end
        if stripblanks(OO.graphics.id)~=emptystr() then
          txt=[txt;
               cformatline(' * Identification: '+strcat(string(OO.graphics.id)),70)];
        end
        txt=[txt;
             cformatline(' * ipar= {'+strcat(string(ipar(ipptr(i):ipptr(i+1)-1)),",")+'};',70)];
        txt($+1)=' */';
        //******************//

        txt=[txt;cformatline(strcat(string(ipar(ipptr(i):ipptr(i+1)-1))+','),70)];
        txt($+1)='';
      end
    end
    txt=[txt;
         '};'
         '']
  else
    txt($+1)='static int IPAR1[1];';
  end
  //**************************//


endfunction

//CallBlock : generate C calling sequence
//            of a scicos block
//
//inputs : bk   : bloc index
//         pt   : evt activation number
//         flag : flag
//
//output : txt  :
//
//16/06/07 Authors : Alan Layec
function txt=call_block42(bk,pt,flag)
  txt=[]
  //**
  if flag==2 & ((zptr(bk+1)-zptr(bk))+..
                (ozptr(bk+1)-ozptr(bk))+..
                (xptr(bk+1)-xptr(bk)+..
                with_work(bk))==0 |..
                pt<=0) & ~(stalone & or(bk==actt(:,1))) then
    return // block without state or continuously activated
  end
  if flag==0 & ((xptr(bk+1)-xptr(bk))==0) then
    return // block without continuous state
  end
  if flag==9 & ((zcptr(bk+1)-zcptr(bk))==0) then
    return // block without continuous state
  end
  if flag==3 & ((clkptr(bk+1)-clkptr(bk))==0) then
    return
  end

  //** adjust pt
  if ~(flag==3 & ((zcptr(bk+1)-zcptr(bk))<>0)) then
    pt=abs(pt)
  end

  //** add comment
  txt=[get_comment('call_blk',list(funs(bk),funtyp(bk),bk));]

  //** set nevprt and flag for called block
  txt=[txt;
       'block_'+rdnom+'['+string(bk-1)+'].nevprt = '+string(pt)+';'
       'local_flag = '+string(flag)+';']

  //**see if its bidon, actuator or sensor
  if funs(bk)=='bidon' then
    txt=[];
    return
  elseif funs(bk)=='bidon2' then
    txt=[];
    return
  elseif or(bk==actt(:,1)) then
    txt=[];
    return
  elseif or(bk==capt(:,1)) then
    txt=[];
    return
  end

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

  select ftyp

    case 0 then
      //**** input/output addresses definition ****//
      if nin>1 then
        for k=1:nin
          uk=inplnk(inpptr(bk)-1+k);
          nuk=size(outtb(uk),'*');
          txt=[txt;
               'rdouttb['+string(k-1)+']=(double *)'+XX_VARPREFIX+'outtbptr['+string(uk-1)+'];']
        end
        txt=[txt;
             'args[0]=&(rdouttb[0]);']
      elseif nin==0
        uk=0;
        nuk=0;
        txt=[txt;
             'args[0]=(double *)'+XX_VARPREFIX+'outtbptr[0];']
      else
        uk=inplnk(inpptr(bk));
        nuk=size(outtb(uk),'*');
        txt=[txt;
             'args[0]=(double *)'+XX_VARPREFIX+'outtbptr['+string(uk-1)+'];']
      end

      if nout>1 then
        for k=1:nout
          yk=outlnk(outptr(bk)-1+k);
          nyk=size(outtb(yk),'*');
          txt=[txt;
               'rdouttb['+string(k+nin-1)+']=(double *)'+XX_VARPREFIX+'outtbptr['+string(yk-1)+'];'];
        end
        txt=[txt;
             'args[1]=&(rdouttb['+string(nin)+']);'];
      elseif nout==0
        yk=0;
        nyk=0;
        txt=[txt;
             'args[1]=(double *)'+XX_VARPREFIX+'outtbptr[0];'];
      else
        yk=outlnk(outptr(bk));
        nyk=size(outtb(yk),'*'),;
        txt=[txt;
             'args[1]=(double *)'+XX_VARPREFIX+'outtbptr['+string(yk-1)+'];'];
      end
      //*******************************************//

      //*********** call seq definition ***********//
      txtc=['(&local_flag,&block_'+rdnom+'['+string(bk-1)+'].nevprt,&'+XX_VARPREFIX+'time,block_'+rdnom+'['+string(bk-1)+'].xd, \';
            'block_'+rdnom+'['+string(bk-1)+'].x,&block_'+rdnom+'['+string(bk-1)+'].nx, \';
            'block_'+rdnom+'['+string(bk-1)+'].z,&block_'+rdnom+'['+string(bk-1)+'].nz,block_'+rdnom+'['+string(bk-1)+'].evout, \';
            '&block_'+rdnom+'['+string(bk-1)+'].nevout,block_'+rdnom+'['+string(bk-1)+'].rpar,&block_'+rdnom+'['+string(bk-1)+'].nrpar, \';
            'block_'+rdnom+'['+string(bk-1)+'].ipar,&block_'+rdnom+'['+string(bk-1)+'].nipar, \';
            'args[0],&nrd_'+string(nuk)+',args[1],&nrd_'+string(nyk)+');'];
      if (funtyp(bk)>2000 & funtyp(bk)<3000)
        blank = get_blank(funs(bk)+'( ');
		//** if the function is orginal implemented in fortran **//
		if(isFortanImplementation(funs(bk)))
			txtc(1) = 'C2F('+funs(bk)+')'+txtc(1);
		else
			txtc(1) = funs(bk)+txtc(1);
		end
      elseif (funtyp(bk)<2000)
		//** if the function is orginal implemented in fortran **//
		if(isFortanImplementation(funs(bk)))
			txtc(1) = 'C2F('+funs(bk)+')'+txtc(1);
		else
			txtc(1) = funs(bk)+txtc(1);
		end
        blank = get_blank(funs(bk)+' ');
      end
      txtc(2:$) = blank + txtc(2:$);
      txt = [txt;txtc];
      //*******************************************//


    //**
    case 1 then
      //*********** call seq definition ***********//
      txtc=['(&local_flag,&block_'+rdnom+'['+string(bk-1)+'].nevprt,&'+XX_VARPREFIX+'time,block_'+rdnom+'['+string(bk-1)+'].xd, \';
            'block_'+rdnom+'['+string(bk-1)+'].x,&block_'+rdnom+'['+string(bk-1)+'].nx, \';
            'block_'+rdnom+'['+string(bk-1)+'].z,&block_'+rdnom+'['+string(bk-1)+'].nz,block_'+rdnom+'['+string(bk-1)+'].evout, \';
            '&block_'+rdnom+'['+string(bk-1)+'].nevout,block_'+rdnom+'['+string(bk-1)+'].rpar,&block_'+rdnom+'['+string(bk-1)+'].nrpar, \';
            'block_'+rdnom+'['+string(bk-1)+'].ipar,&block_'+rdnom+'['+string(bk-1)+'].nipar'];
      if (funtyp(bk)>2000 & funtyp(bk)<3000)
        blank = get_blank(funs(bk)+'( ');
		//** if the function is orginal implemented in fortran **//
		if(isFortanImplementation(funs(bk)))
			txtc(1) = 'C2F('+funs(bk)+')'+txtc(1);
		else
			txtc(1) = funs(bk)+txtc(1);
		end
      elseif (funtyp(bk)<2000)
		//** if the function is orginal implemented in fortran **//
		if(isFortanImplementation(funs(bk)))
			txtc(1) = 'C2F('+funs(bk)+')'+txtc(1);
		else
			txtc(1) = funs(bk)+txtc(1);
		end
        blank = get_blank(funs(bk)+' ');
      end
      if nin>=1 | nout>=1 then
        txtc($)=txtc($)+', \'
        txtc=[txtc;'']
        if nin>=1 then
          for k=1:nin
            uk=inplnk(inpptr(bk)-1+k);
            nuk=size(outtb(uk),'*');
            txtc($)=txtc($)+'(double *)'+XX_VARPREFIX+'outtbptr['+string(uk-1)+'],&nrd_'+string(nuk)+',';
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
            txtc($)=txtc($)+'(double *)'+XX_VARPREFIX+'outtbptr['+string(yk-1)+'],&nrd_'+string(nyk)+',';
          end
          txtc($)=part(txtc($),1:length(txtc($))-1); //remove last ,
        end
      end

      if ztyp(bk) then
        txtc($)=txtc($)+', \'
        txtc=[txtc;
              'block_'+rdnom+'['+string(bk-1)+'].g,&block_'+rdnom+'['+string(bk-1)+'].ng);']
      else
        txtc($)=txtc($)+');';
      end

      txtc(2:$) = blank + txtc(2:$);
      txt = [txt;txtc];
      //*******************************************//

    //**
    case 2 then

      //*********** call seq definition ***********//
		//** if the function is orginal implemented in fortran **//
		if(isFortanImplementation(funs(bk)))
			txtc = 'C2F('+funs(bk)+')';
		else
			txtc = funs(bk);
		end
      txtc=[txtc+'(&local_flag,&block_'+rdnom+'['+string(bk-1)+'].nevprt,&'+XX_VARPREFIX+'time,block_'+rdnom+'['+string(bk-1)+'].xd, \';
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
           funs(bk)+'(&block_'+rdnom+'['+string(bk-1)+'],local_flag);'];

  end

endfunction

function Code=getBlockInitialization()
  //** loop through the different model blocks
  Code = '';
  for kf=1:nblk

  //** if the block is a sensor, an bidon or an actuator, then don't display it
  if (part(funs(kf),1:7) ~= 'capteur' &...
        part(funs(kf),1:10) ~= 'actionneur' & ...
        part(funs(kf),1:5) ~= 'bidon' ...
		) then

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
            '  block_'+rdnom+'['+string(kf-1)+'].x  = &('+XX_STATE_ARRAY_NAME+'['+string(xptr(kf)-1)+']);'
            '  block_'+rdnom+'['+string(kf-1)+'].xd = &('+XX_RATE_ARRAY_NAME+'['+string(xptr(kf)-1)+']);']
    end

    Code=[Code;
          '  if ((block_'+rdnom+'['+string(kf-1)+'].evout  = '+...
          'calloc(block_'+rdnom+'['+string(kf-1)+'].nevout,sizeof(double)))== NULL) return 0;'];

    //***************************** input port *****************************//
    //** alloc insz/inptr **//
    Code=[Code;
          '  if ((block_'+rdnom+'['+string(kf-1)+'].inptr  = '+...
          'malloc(sizeof(double*)*block_'+rdnom+'['+string(kf-1)+'].nin))== NULL) return 0;';
          '  if ((block_'+rdnom+'['+string(kf-1)+'].insz   = '+...
          'malloc(3*sizeof(int)*block_'+rdnom+'['+string(kf-1)+'].nin))== NULL) return 0;'];

    //** inptr **//
    for k=1:nin
       lprt=inplnk(inpptr(kf)-1+k);
       Code=[Code
             '  block_'+rdnom+'['+string(kf-1)+'].inptr['+string(k-1)+...
             ']  = '+XX_VARPREFIX+'outtbptr['+string(lprt-1)+'];']
    end

    //** 1st dim **//
    for k=1:nin
       lprt=inplnk(inpptr(kf)-1+k);
       Code=[Code
             '  block_'+rdnom+'['+string(kf-1)+'].insz['+string((k-1))+...
             ']   = '+string(size(outtb(lprt),1))+';']
    end

    //** 2dn dim **//
    for k=1:nin
       lprt=inplnk(inpptr(kf)-1+k);
       Code=[Code
             '  block_'+rdnom+'['+string(kf-1)+'].insz['+string((k-1)+nin)+...
             ']   = '+string(size(outtb(lprt),2))+';']
    end

    //** typ **//
    for k=1:nin
       lprt=inplnk(inpptr(kf)-1+k);
       Code=[Code
             '  block_'+rdnom+'['+string(kf-1)+'].insz['+string((k-1)+2*nin)+...
             ']   = '+mat2scs_c_typ(outtb(lprt))+';'];
    end
    //**********************************************************************//

    //***************************** output port *****************************//
    //** alloc outsz/outptr **//
    Code=[Code
          '  if ((block_'+rdnom+'['+string(kf-1)+'].outsz  = '+...
          'malloc(3*sizeof(int)*block_'+rdnom+'['+string(kf-1)+'].nout))== NULL) return 0;';
          '  if ((block_'+rdnom+'['+string(kf-1)+'].outptr = '+...
          'malloc(sizeof(double*)*block_'+rdnom+'['+string(kf-1)+'].nout))== NULL) return 0;'];

    //** outptr **//
    for k=1:nout
       lprt=outlnk(outptr(kf)-1+k);
       Code=[Code
             '  block_'+rdnom+'['+string(kf-1)+'].outptr['+string(k-1)+...
             '] = '+XX_VARPREFIX+'outtbptr['+string(lprt-1)+'];']
    end

    //** 1st dim **//
    for k=1:nout
       lprt=outlnk(outptr(kf)-1+k);
       Code=[Code
             '  block_'+rdnom+'['+string(kf-1)+'].outsz['+string((k-1))+...
             ']  = '+string(size(outtb(lprt),1))+';']
    end

    //** 2dn dim **//
    for k=1:nout
       lprt=outlnk(outptr(kf)-1+k);
       Code=[Code
             '  block_'+rdnom+'['+string(kf-1)+'].outsz['+string((k-1)+nout)+...
             ']  = '+string(size(outtb(lprt),2))+';']
    end

    //** typ **//
    for k=1:nout
       lprt=outlnk(outptr(kf)-1+k);
       Code=[Code
             '  block_'+rdnom+'['+string(kf-1)+'].outsz['+string((k-1)+2*nout)+...
             ']  = '+mat2scs_c_typ(outtb(lprt))+';']
    end
    //**********************************************************************//

    Code=[Code
          '  block_'+rdnom+'['+string(kf-1)+...
          '].z = &('+XX_INIT_VALUE_ARRAY_NAME+'['+string(kf-1 +1)+']);']

    if (part(funs(kf),1:7) ~= 'capteur' &...
        part(funs(kf),1:10) ~= 'actionneur' &...
        funs(kf) ~= 'bidon') then
      //** rpar **//
      if (rpptr(kf+1)-rpptr(kf)>0) then
        Code=[Code;
              '  block_'+rdnom+'['+string(kf-1)+...
              '].rpar=&('+XX_PARAMETER_ARRAY_NAME+'['+string(rpptr(kf)-1)+']);']
      end
      //** ipar **//
      if (ipptr(kf+1)-ipptr(kf)>0) then
        Code=[Code;
              '  block_'+rdnom+'['+string(kf-1)+...
              '].ipar=&(IPAR1['+string(ipptr(kf)-1)+']);']
      end
      //** opar **//
      if (opptr(kf+1)-opptr(kf)>0) then
        Code=[Code;
              '  if ((block_'+rdnom+'['+string(kf-1)+'].oparptr = '+...
               'malloc(sizeof(void *)*block_'+rdnom+'['+string(kf-1)+'].nopar))== NULL ) return 0;';
              '  if ((block_'+rdnom+'['+string(kf-1)+'].oparsz  = '+...
               'malloc(2*sizeof(int)*block_'+rdnom+'['+string(kf-1)+'].nopar))== NULL ) return 0;';
              '  if ((block_'+rdnom+'['+string(kf-1)+'].opartyp = '+...
               'malloc(sizeof(int)*block_'+rdnom+'['+string(kf-1)+'].nopar))== NULL ) return 0;';
              ]
        nopar = opptr(kf+1)-opptr(kf);
        //** oparptr **//
        for k=1:nopar
          Code=[Code;
                '  block_'+rdnom+'['+string(kf-1)+'].oparptr['+string(k-1)+...
                 ']   = &'+XX_PARAMETER_ARRAY_NAME+'['+...
				 string(opptr(kf)-2+k+objectParameterOffset)+'];'];
        end
        //** 1st dim **//
        for k=1:nopar
          Code=[Code;
                '  block_'+rdnom+'['+string(kf-1)+'].oparsz['+string(k-1)+...
                 ']    = '+string(size(opar(opptr(kf)-1+k),1))+';'];
        end
        //** 2dn dim **//
        for k=1:nopar
          Code=[Code;
                '  block_'+rdnom+'['+string(kf-1)+'].oparsz['+string(nopar+(k-1))+...
                 ']    = '+string(size(opar(opptr(kf)-1+k),2))+';'];
        end
        //** typ **//
        for k=1:nopar
          Code=[Code;
                '  block_'+rdnom+'['+string(kf-1)+'].opartyp['+string(k-1)+...
                ']   = '+mat2scs_c_typ(opar(opptr(kf)-1+k))+';'];
        end
      end
      //** oz **//
      if (ozptr(kf+1)-ozptr(kf)>0) then
        noz = ozptr(kf+1)-ozptr(kf);
        Code=[Code
              '  if ((block_'+rdnom+'['+string(kf-1)+'].ozptr = '+...
               'malloc(sizeof(void *)*block_'+rdnom+'['+string(kf-1)+'].noz))== NULL) return 0;';
              '  if ((block_'+rdnom+'['+string(kf-1)+'].ozsz  = '+...
               'malloc(2*sizeof(int)*block_'+rdnom+'['+string(kf-1)+'].noz))== NULL) return 0;';
              '  if ((block_'+rdnom+'['+string(kf-1)+'].oztyp = '+...
               'malloc(sizeof(int)*block_'+rdnom+'['+string(kf-1)+'].noz))== NULL) return 0;';]
        //** ozptr **//
        for k=1:noz
          Code=[Code;
                '  block_'+rdnom+'['+string(kf-1)+'].ozptr['+string(k-1)+...
                 ']  = (void *) oz_'+string(ozptr(kf)-1+k)+';'];
        end
        //** 1st dim **//
        for k=1:noz
          Code=[Code;
                '  block_'+rdnom+'['+string(kf-1)+'].ozsz['+string(k-1)+...
                 ']  = '+string(size(oz(ozptr(kf)-1+k),1))+';'];
        end
        //** 2nd dim **//
        for k=1:noz
          Code=[Code;
                '  block_'+rdnom+'['+string(kf-1)+'].ozsz['+string(noz+(k-1))+...
                 ']  = '+string(size(oz(ozptr(kf)-1+k),2))+';'];
        end
        //** typ **//
        for k=1:noz
          Code=[Code;
                '  block_'+rdnom+'['+string(kf-1)+'].oztyp['+string(k-1)+...
                 ']  = '+mat2scs_c_typ(oz(ozptr(kf)-1+k))+';'];
        end
      end
    end
    Code=[Code;
          '  block_'+rdnom+'['+string(kf-1)+'].work = (void **)(((double *)'+XX_VARPREFIX+'work)+'+string(size(z,'*')+kf-1)+');']
  end //** if statement to remove sensors and actuators
  end //** for loop
endfunction

//write_code_doit : generate body of the code for
//                  ordering calls of blocks during
//                  flag 1,2 & flag 3
//
//input : ev  : evt number for block's call
//       flag : flag number for block's call
//
//output : txt for flag 1 or 2, or flag 3
//
//12/07/07 Alan Layec
//
//29/10/2010 Maarten Meijer display single events only
function [txt]=write_code_doit(ev,flag)
  txt=[];
  nrEventFunctionsCalled=0;

  for j=ordptr(ev):ordptr(ev+1)-1
    bk=ordclk(j,1);
    pt=ordclk(j,2);
    //** blk
    if funtyp(bk)>-1 then
        txt2=call_block42(bk,pt,flag);
        if txt2<>[] then
		  //If this function is event driven than increment the counter
		  if(zptr(bk+1)-zptr(bk)>0)
			  nrEventFunctionsCalled=nrEventFunctionsCalled+1;
		  end
          txt=[txt;
               '    '+txt2
               ''];
        end
    //** ifthenelse blk
    elseif funtyp(bk)==-1 then
      ix=-1+inplnk(inpptr(bk));
      TYPE=mat2c_typ(outtb(ix+1)); //** scilab index start from 1
      thentxt=write_code_doit(clkptr(bk),flag);
      elsetxt=write_code_doit(clkptr(bk)+1,flag);
      if thentxt<>[] | elsetxt<>[] then
        txt=[txt;
             '    '+get_comment('ifthenelse_blk',list(bk));]
        //** C **//
        tmp_ = '*(('+TYPE+' *)'+XX_VARPREFIX+'outtbptr['+string(ix)+'])'
        txt=[txt;
             '    if('+tmp_+'>0) {']
        //*******//
        txt=[txt;
             Indent+thentxt]
        if elsetxt<>[] then
           //** C **//
           txt=[txt;
                '    }';
                '    else {';]
           //*******//
           txt=[txt;
                Indent+elsetxt];
        end
        //** C **//
        txt=[txt;
             '    }']
        //*******//
      end
    //** eventselect blk
    elseif funtyp(bk)==-2 then
      Noutport=clkptr(bk+1)-clkptr(bk);
      ix=-1+inplnk(inpptr(bk));
      TYPE=mat2c_typ(outtb(ix+1)); //** scilab index start from 1
      II=[];
      switchtxt=list()
      for i=1: Noutport
        switchtxt(i)=write_code_doit(clkptr(bk)+i-1,flag);
        if switchtxt(i)<>[] then II=[II i];end
      end
	  
      if II<>[] then
        txt=[txt;
             '    '+get_comment('evtselect_blk',list(bk));]
        tmp_='*(('+TYPE+' *)'+XX_VARPREFIX+'outtbptr['+string(ix)+'])'
        //** C **//
        txt=[txt;
             '    i=max(min((int) '+...
              tmp_+',block_'+rdnom+'['+string(bk-1)+'].evout),1);'
             '    switch(i)'
             '    {']
        //*******//
        for i=II
          //** C **//
          txt=[txt;
               '     case '+string(i)+' :';]
          //*******//
          txt=[txt;
               BigIndent+write_code_doit(clkptr(bk)+i-1,flag);]
          //** C **//
          txt=[txt;
               BigIndent+'break;']
          //*******//
        end
        //** C **//
        txt=[txt;
             '    }']
        //*******//
      end
    //** Unknown block
    else
      error('Unknown block type '+string(bk));
    end
  end
  
  //if there are more type=0 functions called than one, just return an empty string
  if(nrEventFunctionsCalled>1)
	txt=[];
  end

endfunction

//write_code_cdoit : generate body of the code for
//                   for all time dependant blocks
//
//input : flag : flag number for block's call
//
//output : txt for cord blocks
//
//12/07/07 Alan Layec
function [txt]=write_code_cdoit(flag)
  txt=[];

  for j=1:ncord
    bk=cord(j,1);
    pt=cord(j,2);
    //** blk
    if funtyp(bk)>-1 then
      if or(bk==act) | or(bk==cap) then
        if stalone then
          txt2=call_block42(bk,pt,flag);
          if txt2<>[] then
            txt=[txt;
                 '    '+txt2
                 ''];
          end
        end
      else
        txt2=call_block42(bk,pt,flag);
        if txt2<>[] then
          txt=[txt;
               '    '+txt2
               ''];
        end
      end
    //** ifthenelse blk
    elseif funtyp(bk)==-1 then
      ix=-1+inplnk(inpptr(bk));
      TYPE=mat2c_typ(outtb(ix+1)); //** scilab index start from 1
      thentxt=write_code_doit(clkptr(bk),flag);
      elsetxt=write_code_doit(clkptr(bk)+1,flag);
      if thentxt<>[] | elsetxt<>[] then
        txt=[txt;
             '    '+get_comment('ifthenelse_blk',list(bk));]
        //** C **//
        tmp_='*(('+TYPE+' *)'+XX_VARPREFIX+'outtbptr['+string(ix)+'])'
        txt=[txt;
             '    if('+tmp_+'>0) {']
        //*******//
        txt=[txt;
             Indent+thentxt];
        if elsetxt<>[] then
          //** C **//
          txt=[txt;
               '    }';
               '    else {';]
          //*******//
          txt=[txt;
               Indent+elsetxt];
        end
        //** C **//
        txt=[txt;
             '    }']
        //*******//
      end
    //** eventselect blk
    elseif funtyp(bk)==-2 then
      Noutport=clkptr(bk+1)-clkptr(bk);
      ix=-1+inplnk(inpptr(bk));
      TYPE=mat2c_typ(outtb(ix+1)); //** scilab index start from 1
      II=[];
      switchtxt=list()
      for i=1: Noutport
        switchtxt(i)=write_code_doit(clkptr(bk)+i-1,flag);
        if switchtxt(i)<>[] then II=[II i];end
      end
      if II<>[] then
        txt=[txt;
             '    '+get_comment('evtselect_blk',list(bk));]
        //** C **//
        tmp_='*(('+TYPE+' *)'+XX_VARPREFIX+'outtbptr['+string(ix)+'])'
        txt=[txt;
             '    i=max(min((int) '+...
              tmp_+',block_'+rdnom+'['+string(bk-1)+'].evout),1);'
             '    switch(i)'
             '    {']
        //*******//
        for i=II
         //** C **//
         txt=[txt;
              '     case '+string(i)+' :';]
         //*******//
         txt=[txt;
              BigIndent+write_code_doit(clkptr(bk)+i-1,flag);]
         //** C **//
         txt=[txt;
              BigIndent+'break;']
         //*******//
        end
        //** C **//
        txt=[txt;
             '    }'];
        //*******//
      end
    //** Unknown block
    else
      error('Unknown block type '+string(bk));
    end
  end

endfunction