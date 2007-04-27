doc_path=get_absolute_file_path('test_BUILD4_scicos.sce');

opath2=SCI+"/macros/scicos_blocks/"+...
       ["scicos"     //1
        "Branching"  //2
        "Electrical" //3
        "Events"     //4
        "Hydraulics" //5
        "Linear"     //6
        "Misc"       //7
        "NonLinear"  //8
        "PDE"        //9
        "Sinks"      //10
        "Sources"    //11
        "Threshold"  //12
        "MatrixOp"   //13
       ]+"/";

palpath=SCI+'/macros/scicos/';
autopath=SCI+'/macros/auto/';

%gendoc=gendoc_def(lang=['fr' 'eng'],..
                   man_path=doc_path+'man/',..
                   rout_path=SCI+'/routines/scicos/',..
                   web_path='',..
                   pdf_path='',..
                   clean_html=%t,..
                   ext_block='',..
                   ext_mblock='',..
                   ext_sci='',..
                   with_gimp=%f,..
                   verbose=%t,..
                   with_log=%t,..
                   html_subtitle_color='green',..
                   name_log='BUILD_4_scicos_doc.log');

%gendoc=set_gendoc_def(%gendoc,1);

%gendoc.mpath.data=%gendoc.mpath.man+...
                   ['fr/data_revB/';
                    'eng/data_revB/'];
%gendoc.mpath.html=%gendoc.mpath.man+...
                   ['fr/scicos/';
                    'eng/scicos/'];
%gendoc.mpath.html_img='';

//
create_gendoc_dirs(%gendoc);

my_listf=["","var2vec","sci";
          "","vec2var","sci";
          "","curblock","sci";
          "","scicos_debug","sci";
          "","scicos_time","sci";
          "","getscicosvars","sci";
          "","buildouttb","sci";
          palpath,"Branching.cosf","pal";
          palpath,"Electrical.cosf","pal"; //Masoud
          palpath,"Events.cosf","pal";
          palpath,"Linear.cosf","pal";
          palpath,"Non_linear.cosf","pal";
          palpath,"Others.cosf","pal";
          palpath,"Sinks.cosf","pal";
          palpath,"Sources.cosf","pal";
          palpath,"ThermoHydraulics.cosf","pal";  //Masoud
          palpath,"Threshold.cosf","pal";
          palpath,"Matrix.cosf","pal";
          "","CONST_f.sci","block";
          "","GENSQR_f.sci","block";
          "","RAMP.sci","block";
          "","RAND_f.sci","block";
          "","RFILE_f.sci","block";
          "","CLKINV_f.sci","block";
          "","CURV_f.sci","block";
          "","INIMPL_f.sci","block";
          "","READAU_f.sci","block";
          "","SAWTOOTH_f.sci","block";
          "","STEP_FUNCTION.sci","block";
          "","CLOCK_f.sci","block";
          "","GENSIN_f.sci","block";
          "","IN_f.sci","block";
          "","READC_f.sci","block";
          "","TIME_f.sci","block";
          "","Modulo_Count.sci","block";
          "","AFFICH_f.sci","block";
          "","CMSCOPE.sci","block";
          //"","SCOPXY_f.sci","block"; //VIEUX BLOC
          "","WRITEC_f.sci","block";
          //"","ANIMXY_f.sci","block"; //VIEUX BLOC
          "","CSCOPE.sci","block";
          "","OUTIMPL_f.sci","block";
          "","CLKOUTV_f.sci","block";
          //"","EVENTSCOPE_f.sci","block"; //VIEUX BLOC
          "","OUT_f.sci","block";
          "","WFILE_f.sci","block";
          //"","FSCOPE_f.sci","block"; //VIEUX BLOC
          "","WRITEAU_f.sci","block";
          "","DEMUX.sci","block";
          "","MUX.sci","block";
          "","NRMSOM_f.sci","block";
          "","EXTRACTOR.sci","block";
          "","SELECT_f.sci","block";
          "","ISELECT_f.sci","block";
          "","RELAY_f.sci","block";
          "","SWITCH2.sci","block";
          "","IFTHEL_f.sci","block";
          "","ESELECT_f.sci","block";
          "","M_SWITCH.sci","block";
          "","SCALAR2VECTOR.sci","block";
          "","SWITCH_f.sci","block";
          "","ABS_VALUE.sci","block";
          "","TrigFun.sci","block";
          "","EXPBLK_f.sci","block";
          "","INTRP2BLK_f.sci","block";
          "","INTRPLBLK_f.sci","block";
          "","INVBLK_f.sci","block";
          "","LOGBLK_f.sci","block";
          "","LOOKUP_f.sci","block";
          "","MAXMIN.sci","block";
          "","POWBLK_f.sci","block";
          "","PROD_f.sci","block";
          "","PRODUCT.sci","block";
          "","QUANT_f.sci","block";
          "","EXPRESSION.sci","block";
          "","SATURATION.sci","block";
          "","SIGNUM.sci","block";
          "","ANDBLK.sci","block";
          "","HALT_f.sci","block";
          "","freq_div.sci","block";
          "","ANDLOG_f.sci","block";
          "","EVTDLY_f.sci","block";
          // "","IFTHEL_f.sci","block"; //CEST ENDOUBLE
          // "","ESELECT_f.sci","block"; //CEST ENDOUBLE
          "","CLKSOMV_f.sci","block";
          // "","CLOCK_f.sci","block";   //CEST ENDOUBLE
          "","EVTGEN_f.sci","block";
          "","EVTVARDLY.sci","block";
          "","NEGTOPOS_f.sci","block";
          "","POSTONEG_f.sci","block";
          "","ZCROSS_f.sci","block";
          "","c_block.sci","block";
          "","fortran_block.sci","block";
          "","SUPER_f.sci","block";
          "","scifunc_block.sci","block";
          "","TEXT_f.sci","block";
          "","CBLOCK.sci","block";
          "","RATELIMITER.sci","block";
          "","BACKLASH.sci","block";
          "","DEADBAND.sci","block";
          // "","EXPRESSION.sci","block"; //CEST ENDOUBLE
          "","CONSTRAINT_f.sci","block";
          "","HYSTHERESIS.sci","block";
          "","LOGICAL_OP.sci","block";
          "","RELATIONALOP.sci","block";
          "","generic_block2.sci","block";
          "","DLR_f.sci","block";
          "","TCLSS_f.sci","block";
          "","DOLLAR_f.sci","block";
          "","CLINDUMMY_f.sci","block";
          "","DLSS_f.sci","block";
          "","REGISTER_f.sci","block";
          "","TIME_DELAY.sci","block";
          "","CLR_f.sci","block";
          "","GAINBLK.sci","block";
          "","SAMPLEHOLD_f.sci","block";
          "","VARIABLE_DELAY.sci","block";
          "","CLSS_f.sci","block";
          "","SUMMATION.sci","block";
          "","INTEGRAL.sci","block";
          "","SUM_f.sci","block";
          "","DERIV.sci","block";

          "","DEBUG_SCICOS.sci","block";
          "","PDE.sci","block";
          "","EDGE_TRIGGER.sci","block";
          "","Extract_Activation.sci","block";

          autopath,"scicos.sci","sci";

          //Masoud
          opath2(3),"Capacitor.sci","mblock";
          opath2(3),"Ground.sci","mblock";
          opath2(3),"VVsourceAC.sci","mblock";
          opath2(3),"ConstantVoltage.sci","mblock";
          opath2(3),"Inductor.sci","mblock";
          opath2(3),"PotentialSensor.sci","mblock";
          opath2(3),"VariableResistor.sci","mblock";
          opath2(3),"CurrentSensor.sci","mblock";
          opath2(3),"Resistor.sci","mblock";
          opath2(3),"VoltageSensor.sci","mblock";
          opath2(3),"Diode.sci","mblock";
          opath2(3),"VsourceAC.sci","mblock";
          opath2(5),"Bache.sci","mblock";
          opath2(5),"PerteDP.sci","mblock";
          opath2(5),"VanneReglante.sci","mblock";
          opath2(5),"PuitsP.sci","mblock";
          opath2(5),"SourceP.sci","mblock";

          //Fady
          opath2(13),"CUMSUM.sci","block";
          opath2(13),"EXTRACT.sci","block";
          opath2(13),"EXTTRI.sci","block";
          opath2(13),"MATBKSL.sci","block";
          opath2(13),"MATCATH.sci","block";
          opath2(13),"MATCATV.sci","block";
          opath2(13),"MATDET.sci","block";
          opath2(13),"MATDIAG.sci","block";
          opath2(13),"MATDIV.sci","block";
          opath2(13),"MATEIG.sci","block";
          opath2(13),"MATEXPM.sci","block";
          opath2(13),"MATINV.sci","block";
          opath2(13),"MATLU.sci","block";
          opath2(13),"MATMAGPHI.sci","block";
          opath2(13),"MATMUL.sci","block";
          opath2(13),"MATPINV.sci","block";
          opath2(13),"MATRESH.sci","block";
          opath2(13),"MATSING.sci","block";

          //Alan
          autopath,"scicosim.sci","sci";
          autopath,"scicos_simulate.sci","sci";
          autopath,"lincos.sci","sci";
          autopath,"steadycos.sci","sci";
];

load SCI/macros/scicos/lib;
exec(loadpallibs,-1);

//Example of selection :
//my_listf=my_listf(122:138,:);

//generate_xml_file(my_listf,%gendoc);

//import_data_to_file('all',%gendoc);

//generate_aux_tex_file(my_listf,'html',%gendoc)

//generate_html_file(my_listf,%gendoc);

//gen_whatis(%gendoc.mpath.data(1)+'/outline.xml',%gendoc);