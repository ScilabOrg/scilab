//test_BUILD4_scicos.sce
//This is the scilab script for the parametrization
//of scitexgendoc and for definition of
//scicos files to be documented

// alan,27/04/2007 : change html target directories path with the "trunk" look
//                   scicos_doc/help
//                   scicos_doc/help/images
//                   scicos_doc/help/en_US
//                   scicos_doc/help/fr_FR
//
// alan,28/04/2007 : add module flag "scicos" in %gendoc list
//                   add palpath flag in %gendoc list

// fady,04/05/2007 : add the integer palette.


//**--------------definition of some functions----------------**//
//gen_outline_pal
function txt=gen_outline_pal(listf)
  txt=[];
  txt='<PAL varpath=""palpath"" name="""+listf(find(listf(:,3)=='pal'),2)+""">";
  for i=2:size(listf,1)
     txt=[txt;
          "   <BLK varpath="""" name="""+basename(listf(i,2))+"""></BLK>"]
  end
  txt=[txt;
       "</PAL>"];
endfunction

//gen_outline_dir
function txt=gen_outline_dir(listf)
  txt=[];
//  txt='<PAL varpath=""palpath"" name="""+listf(find(listf(:,3)=='pal'),2)+""">";
  txt="  <SUBCHAPTER eng=""subchapter"" fr=""souschapitre"">";
  for i=2:size(listf,1)
     txt=[txt;
          "   <BLK varpath="""" name="""+basename(listf(i,2))+"""></BLK>"]
  end
  txt=[txt;
       "</SUBCHAPTER>"];
endfunction

//gen_outline_pal=gen_outline_dir;

//gen_scs_outline
function tt=generate_scs_outline()
 tt = ["<WHATIS>";
              "  <TITLE eng=""Scicos Documentation"" fr=""Documentation Scicos""></TITLE>";
              "  <DATE>19 Septembre 2007</DATE>";
              "";
              "  <CHAPTER eng=""Editor"" fr=""Editeur"">";
              "    <SCI varpath="""" name=""Menu_entries""></SCI>"
              "    <SCI varpath="""" name=""Keyboard_shortcuts""></SCI>"
              "  </CHAPTER>"
              "";
              "  <CHAPTER eng=""Blocks list"" fr=""Liste des blocs"">"];

 tt = [tt;
              ""
              "  "+gen_outline_pal(listf_of_sources);
              ""
              "  "+gen_outline_pal(listf_of_linear);
              ""
              "  "+gen_outline_pal(listf_of_nonlinear);
              ""
              "  "+gen_outline_pal(listf_of_branching);
              ""
              "  "+gen_outline_pal(listf_of_misc);
              ""
              "  "+gen_outline_pal(listf_of_threshold);
              ""
              "  "+gen_outline_pal(listf_of_oldblocks);
              ""
              "  "+gen_outline_pal(listf_of_sinks);
              ""
              "  "+gen_outline_pal(listf_of_events);
              ""
              "  "+gen_outline_pal(listf_of_elec);
              ""
              "  "+gen_outline_pal(listf_of_thermo);
              ""
              "  "+ gen_outline_pal(listf_of_matop);
              ""
              "  "+ gen_outline_pal(listf_of_intop);
              ""
              "  "+ gen_outline_pal(listf_of_demos);]

  tt = [tt;
               "  </CHAPTER>"
               ""
               "  <CHAPTER eng=""Batch functions"" fr=""Fonctions en ligne de commande"">"
               "    <SCI varpath=""autopath"" name=""scicos.sci""></SCI>"
               "    <SCI varpath=""autopath"" name=""scicosim.sci""></SCI>"
               "    <SCI varpath=""autopath"" name=""scicos_simulate.sci""></SCI>"
               "    <SCI varpath=""autopath"" name=""lincos.sci""></SCI>"
               "    <SCI varpath=""autopath"" name=""steadycos.sci""></SCI>"
               "  </CHAPTER>"
               ""
               "  <CHAPTER eng=""Data Structure"" fr=""Structure de donnÈe"">  </CHAPTER>"
               ""
               "  <CHAPTER eng=""Scilab built-in functions"" fr=""Fonctions utilitaires Scilab"">"
               "    <SCI varpath=""autopath"" name=""buildouttb""></SCI>"
               "    <SCI varpath=""autopath"" name=""curblock""></SCI>"
               "    <SCI varpath=""autopath"" name=""getblocklabel""></SCI>"
               "    <SCI varpath=""autopath"" name=""getscicosvars""></SCI>"
               "    <SCI varpath=""autopath"" name=""phase_simulation""></SCI>"
               "    <SCI varpath=""autopath"" name=""pointer_xproperty""></SCI>"
               "    <SCI varpath=""autopath"" name=""scicos_debug""></SCI>"
               "    <SCI varpath=""autopath"" name=""scicos_time""></SCI>"
               "    <SCI varpath=""autopath"" name=""set_xproperty""></SCI>"
               "    <SCI varpath=""autopath"" name=""var2vec""></SCI>"
               "    <SCI varpath=""autopath"" name=""vec2var""></SCI>"
               "  </CHAPTER>"
               ""
               "</WHATIS>"];
endfunction

//gen_entries : generate tex file of entries of editor menu from
//initial_scicos_tables
function [txt_en] = gen_entries (typdoc,lang)
 [scicos_pal, %scicos_menu, %scicos_short, %scicos_help, ...
  %scicos_display_mode, modelica_libs,scicos_pal_libs, ...
  %scicos_lhb_list, %CmenuTypeOneVector ] = initial_scicos_tables();

 txt_en=[]
 txt_fr=[]
 %scicos_help_fr=%scicos_help;
 %scicos_menu_fr=%scicos_menu;

 //** loop on number of menu
 for i=1:lstsize(%scicos_menu)
   //** get the menu title
   menu_title=%scicos_menu(i)(1)

   //** get entries
   menu_lst=%scicos_menu(i)(2:$)

   //** Add title in txt of help
   txt_en=[txt_en;
           '\subsection{'+latexsubst(menu_title)+' menu}'
           '\begin{itemize}']

   //** loop on number of entries
   for j=1:size(menu_lst,2)
     //** get entry
     entry_title = menu_lst(j);

     //** Add title:entry in txt of help
     txt_en=[txt_en;
             '\item {\bf '+latexsubst(menu_title)+':'+entry_title]
             '      '

     //** Check if there is a shorcut
     sc_ind=find(%scicos_short(:,2)==entry_title)
     if sc_ind<>[] then
       txt_en($) = txt_en($) + ' ('+%scicos_short(sc_ind,1)+')'
     end

     //**retrieve help in %scicos_help
     if execstr('help_txt = %scicos_help(entry_title)','errcatch')==0 then
       txt_en($) = txt_en($) + '}\\'
       txt_en=[txt_en;
               '      '+latexsubst(help_txt);
               '']
     else
      txt_en($) = txt_en($) + '}'
       txt_en=[txt_en;
               '';
               '']
     end
   end

   txt_en = [txt_en;
             '\end{itemize}']
 end
endfunction

//gen_shortc : generate tex file of keyborad shorcuts of the editor
//from initial_scicos_tables
function [txt_en] = gen_shortc (typdoc,lang)
 [scicos_pal, %scicos_menu, %scicos_short, %scicos_help, ...
  %scicos_display_mode, modelica_libs,scicos_pal_libs, ...
  %scicos_lhb_list, %CmenuTypeOneVector ] = initial_scicos_tables();

 txt_en = [];

 txt_en = [txt_en;
           '\begin{itemize}']
 //** loop on number of menu
 for i=1:size(%scicos_short,1)

   txt_en = [txt_en;
             '  \item{\bf '''+%scicos_short(i,1)+''' :} ' + %scicos_short(i,2)]

 end

 txt_en = [txt_en;
           '\end{itemize}']
endfunction

//gen_scs_editor_help : main generator for scitexgendoc of tex files
//for the scicos editor
function gen_scs_editor_help(typdoc,%gd)

   for i=1:size(%gd.lang,1)
      //**------ entries ------**//
      //** generate body of the tex file
      tt=gen_entries(typdoc,%gd.lang(i))
      //** generate tex head
      head_tex=get_head_tex(["","Menu_entries","sci"],typdoc,i,%gd)
      //** change title of html head
      if %gd.lang(i)=='fr' then
         head_tex=strsubst(head_tex,'Fonction Scilab','Editeur Scicos')
      elseif %gd.lang(i)=='eng' then
         head_tex=strsubst(head_tex,'Scilab Function','Scicos Editor')
      end

      //** generate txt of tex file
      txt_entries=[head_tex;
                   tt
                   '\htmlinfo*'
                   '\end{document}']
      //**---------------------**//


      //**------ shortcuts ------**//
      //** generate body of the tex file
      tt=gen_shortc(typdoc,%gd.lang(i))
      //** generate tex head
      head_tex=get_head_tex(["","Keyboard_shortcuts","sci"],typdoc,i,%gd)
      //** change title of html head
      if %gd.lang(i)=='fr' then
         head_tex=strsubst(head_tex,'Fonction Scilab','Editeur Scicos')
      elseif %gd.lang(i)=='eng' then
         head_tex=strsubst(head_tex,'Scilab Function','Scicos Editor')
      end

      //** generate txt of tex file
      txt_shortcuts=[head_tex;
                     tt
                     '\htmlinfo*'
                     '\end{document}']
      //**---------------------**//

      //create lang directory
      if fileinfo(%gd.lang(i)+'/')==[] then
       mkdir(%gd.lang(i))
      end

      name=get_extname(["","Menu_entries","sci"],%gd)

      //create object directory for
      //tex compilation
      if fileinfo(%gd.lang(i)+'/'+...
                   name)==[] then
        mkdir(%gd.lang(i)+'/'+name)
      end

      mputl(txt_entries,%gd.lang(i)+...
            '/'+name+...
            '/'+name+'.tex');

      name=get_extname(["","Keyboard_shortcuts","sci"],%gd)

      //create object directory for
      //tex compilation
      if fileinfo(%gd.lang(i)+'/'+...
                   name)==[] then
        mkdir(%gd.lang(i)+'/'+name)
      end

      mputl(txt_shortcuts,%gd.lang(i)+...
            '/'+name+...
            '/'+name+'.tex');
   end
endfunction

//gen_scicos_doc : generate all the scicos doc
function gen_scicos_doc(my_list,typdoc,%gd)
  generate_aux_tex_file(my_list,typdoc,%gd);
  import_data_to_file('all',%gd);
  generate_aux_tex_file(my_list,typdoc,%gd);
  gen_scs_editor_help(typdoc,%gd);
  generate_html_file(my_list,%gd);
  gen_whatis(%gd.mpath.data(1)+'/outline.xml',%gd);
endfunction

//**----------------------------------------------------------**//

//**-------------------path definition-----------------------**//
//define directory of scicos_doc
doc_path=get_absolute_file_path('test_BUILD4_scicos.sce');

//define path of interfacing function of scicos block
opath2=SCI+"/macros/"+...
       ["scicos"                   //1
        "scicos_blocks/Branching"  //2
        "scicos_blocks/Electrical" //3
        "scicos_blocks/Events"     //4
        "scicos_blocks/Hydraulics" //5
        "scicos_blocks/Linear"     //6
        "scicos_blocks/Misc"       //7
        "scicos_blocks/NonLinear"  //8
        "scicos_blocks/PDE"        //9
        "scicos_blocks/Sinks"      //10
        "scicos_blocks/Sources"    //11
        "scicos_blocks/Threshold"  //12
        "scicos_blocks/MatrixOp"   //13
        "scicos_blocks/IntegerOp"   //14
       ]+"/";

//define path of cosf file of scicos palettes
palpath=SCI+'/macros/scicos/';

//define auto path
autopath=SCI+'/macros/auto/';

//define util path
utilpath=SCI+'/macros/util/';
//**----------------------------------------------------------**//


//**----------scitexgendoc global parameters------------------**//
//define %gendoc list (for scitexgendoc)
%gendoc=gendoc_def(lang=['fr' 'eng'],..
                   man_path=doc_path+'man/',..
                   rout_path=SCI+'/routines/scicos/',..
                   mod_flag='Scicos',..
                   path_flag='SCI',..
                   pal_path=palpath,..
                   block_flag='scs',..
                   web_path='',..
                   pdf_path='',..
                   clean_html=%t,..
                   ext_block='',..
                   ext_mblock='',..
                   ext_sci='',..
                   with_gimp=%f,..
                   verbose=%t,..
                   with_log=%t,..
                   html_subtitle_color='blue',..
                   name_log='BUILD_4_scicos_doc.log');

//set behavior of documentation target directories
//(0: scilab 4.1x 1:scilab 5)
%gendoc=set_gendoc_def(%gendoc,1);

//set source directories of data files
%gendoc.mpath.data=%gendoc.mpath.man+...
                   ['fr/data_revB/';
                    'eng/data_revB/'];

//set target directories of html files
// %gendoc.mpath.html=doc_path+'/help/'+...
//                    ['fr_FR/';
//                     'en_US/'];
%gendoc.mpath.html=SCI+'/man/'+...
                   ['fr/scicos/';
                    'eng/scicos/'];
%gendoc.mpath.html_img='../../images/scicos/';

//create target directories
create_gendoc_dirs(%gendoc);
//**----------------------------------------------------------**//

//define a variable for protected local xml/tex files
if ~exists('%already_import') then
  %already_import = %f;
end

//**-------------------Files declaration----------------------**//
//define list of files to be documented
// my_list(1,) : the path of the file
// my_list(2,) : the name of the file with extension
// my_list(3,) : the type of the object
//               current scitexgendoc version only support
//                 'mblock' : scicos modelica block
//                 'block'  : scicos block
//                 'diagr'  : scicos diagram
//                 'pal'    : scicos palette
//                 'scilib' : library of scilab macro
//                 'sci'    : scilab macro
//                 'sim'    : scilab script of scicos diagram simulation
//                 'sce'    : scilab script
//                 'rout'   : low level routine

//TO BE DONE
my_listf=[// "","EXPRESSION.sci","block"; //CEST ENDOUBLE
          ];

//util macros
listf_of_utilsci=[];

//auto macros
listf_of_autosci=[autopath,"scicos.sci","sci";
                  autopath,"scicos_simulate.sci","sci";
                  autopath,"lincos.sci","sci";
                  autopath,"steadycos.sci","sci"];

//Interfaced function
listf_of_interf=["","buildouttb","sci";
                 "","curblock","sci";
                 "","getblocklabel","sci";
                 "","phase_simulation","sci";
                 "","pointer_xproperty","sci";
                 "","scicos_debug","sci";
                 "","scicos_time","sci"
                 "","set_xproperty","sci";
                 "","scicosim.sci","sci";
                 "","getscicosvars","sci";
                 "","var2vec","sci";
                 "","vec2var","sci";];

//**--palettes--*/

//Sources palette
listf_of_sources=[palpath,"Sources.cosf","pal"];
listf = basename(listfiles(opath2(11)+"*.sci"))+".sci";
for i = 1:size(listf,1)
  listf_of_sources=[listf_of_sources;
                    opath2(11),listf(i),"block"];
end
clear i;clear listf;

//linear palette
listf_of_linear=[palpath,"Linear.cosf","pal"];
listf = basename(listfiles(opath2(6)+"*.sci"))+".sci";
for i = 1:size(listf,1)
  if listf(i)<>'SOM_f.sci' then //!!!!!!! YAUNBÈGICI
    listf_of_linear=[listf_of_linear;
                     opath2(6),listf(i),"block"];
  end
end
clear i;clear listf;

//nonlinear palette
listf_of_nonlinear=[palpath,"Non_linear.cosf","pal"];
listf = basename(listfiles(opath2(8)+"*.sci"))+".sci";
for i = 1:size(listf,1)
  listf_of_nonlinear=[listf_of_nonlinear;
                      opath2(8),listf(i),"block"];
end
clear i;clear listf;

//branching palette
listf_of_branching=[palpath,"Branching.cosf","pal"];
listf = basename(listfiles(opath2(2)+"*.sci"))+".sci";
for i = 1:size(listf,1)
  listf_of_branching=[listf_of_branching;
                      opath2(2),listf(i),"block"];
end
clear i;clear listf;

//Threshold palette
listf_of_threshold=[palpath,"Threshold.cosf","pal"];
listf = basename(listfiles(opath2(12)+"*.sci"))+".sci";
for i = 1:size(listf,1)
  listf_of_threshold=[listf_of_threshold;
                      opath2(12),listf(i),"block"];
end
clear i;clear listf;

//Oldblocks palette
listf_of_oldblocks=[palpath,"OldBlocks.cosf","pal"];

//Sinks palette
listf_of_sinks=[palpath,"Sinks.cosf","pal"];
listf = basename(listfiles(opath2(10)+"*.sci"))+".sci";
for i = 1:size(listf,1)
  listf_of_sinks=[listf_of_sinks;
                  opath2(10),listf(i),"block"];
end
clear i;clear listf;

//Events palette
listf_of_events=[palpath,"Events.cosf","pal"];
listf = basename(listfiles(opath2(4)+"*.sci"))+".sci";
for i = 1:size(listf,1)
  listf_of_events=[listf_of_events;
                   opath2(4),listf(i),"block"];
end
clear i;clear listf;

//Modelica blocks palettes
listf_of_elec=[palpath,"Electrical.cosf","pal"];
listf = basename(listfiles(opath2(3)+"*.sci"))+".sci";
for i = 1:size(listf,1)
  listf_of_elec=[listf_of_elec;
                 opath2(3),listf(i),"mblock"];
end
clear i;clear listf;
listf_of_thermo=[palpath,"ThermoHydraulics.cosf","pal"];
listf = ['Bache.sci';'PerteDP.sci';'PuitsP.sci';'SourceP.sci';'VanneReglante.sci'];
for i = 1:size(listf,1)
  listf_of_thermo=[listf_of_thermo;
                   opath2(5),listf(i),"mblock"];
end
clear i;clear listf;

//MatrixOp palette
listf_of_matop=[palpath,"Matrix.cosf","pal"];
listf = basename(listfiles(opath2(13)+"*.sci"))+".sci";
for i = 1:size(listf,1)
  if listf(i)<>'INTGAINBLK.sci' then //!!!!!!! YAUNBÈGICI
    listf_of_matop=[listf_of_matop;
                   opath2(13),listf(i),"block"];
  end
end
clear i;clear listf;

//IntegerOp palette
listf_of_intop=[palpath,"Integer.cosf","pal"];
listf = basename(listfiles(opath2(14)+"*.sci"))+".sci";
for i = 1:size(listf,1)
    listf_of_intop=[listf_of_intop;
                   opath2(14),listf(i),"block"];
end
clear i;clear listf;

//Misc directory
listf_of_misc=[palpath,"Others.cosf","pal"];
listf = basename(listfiles(opath2(7)+"*.sci"))+".sci";
for i = 1:size(listf,1)
  if listf(i)<>'m_sin.sci' then //!!!!!!! YAUNBÈGICI
    listf_of_misc=[listf_of_misc;
                   opath2(7),listf(i),"block"];
  end
end
listf_of_misc=[listf_of_misc;
               opath2(9),"PDE.sci","block"]; //??
clear i;clear listf;

listf_of_demos=[palpath,"DemoBlocks.cosf","pal"];
//**------------*/

//**--Editor--*/
list_of_editor = ["","Menu_entries","sci";
                  "","Keyboard_shortcuts","sci";]
//**------------*/

//**----------------------------------------------------------**//


//**------------------outline.xml generation------------------**//
outline_txt=generate_scs_outline();
mputl(outline_txt,%gendoc.mpath.data(1)+'/outline.xml');
//**----------------------------------------------------------**//


//load scicos libraries and palettes of blocks
load SCI/macros/scicos/lib;
exec(loadpallibs,-1);

//Examples of selection :
// 1 - my_list=my_listf(122:138,:); //documentation of files from 122 to 138
// 2 - my_list=my_listf(find(my_listf(:,2)=='scicos.sci'),:) //doc of the file scicos.sci
// 3 - my_list=listf_of_events; //doc of the event palette

//generate doc for all scicos files define above
my_list = [my_listf;
           listf_of_utilsci;
           listf_of_autosci;
           listf_of_interf;
           listf_of_sources;
           listf_of_linear;
           listf_of_nonlinear;
           listf_of_branching;
           listf_of_threshold;
           listf_of_oldblocks;
           listf_of_sinks;
           listf_of_events;
           listf_of_elec;
           listf_of_thermo;
           listf_of_matop;
           listf_of_intop;
           listf_of_misc;
           listf_of_demos;
           list_of_editor];

//STEP_1 : Get the current set of xml/tex files of B4_scicos doc.

//create skeleton of xml files
generate_xml_file(my_list,%gendoc);

//import all data in xml/tex files
//from data files
if %already_import then
  irr=message(["                       WARNING";
               "";
               "You have already import data in xml/tex files by";
               "running this script.";
               "By importing data now, you can earase locally";
               "the modifications that probably you have just made !";
               ""
               "        Do you really know what you are doing ?"],["yes","no"]);
  if irr==1 then, import_data_to_file('all',%gendoc), end;
  clear irr;
else
  import_data_to_file('all',%gendoc);
  %already_import = %t;
end

//STEP_2 : Inform xml/tex files with your informations.
//         You can begin the work of documentation with :
//          choice 1 : in generated xml files
//                     directories are
//                        scicos_doc/eng/xml
//                        scicos_doc/fr/xml
//
//          choice 2 : directly in LaTeX (for expert)
//                     directories are
//                        scicos_doc/eng/tex
//                        scicos_doc/fr/tex
//
//   PS : choice 1 & 2 can be mixed !

//STEP_3 : Generate auxiliaries tex files compilable with LaTeX.
//         This is done from xml/tex files provided by STEP 1 and 2.
//
//   PS : - All work of generation of this tex files is done
//          in the current directory.
//           For each documented object a directory is created.
//
//        - If your xml/tex files of STEP_2 are well formed
//          you can run latex at the root of each generated directories
//          without any errors.
//
//-->generate_aux_tex_file(my_list,'html',%gendoc);


//STEP_3a : do a second pass for import_data and generation of
//          auxiliaries tex files (not necessary)
//          That improves interfacing functions and
//          computational functions man pages
//
//-->import_data_to_file('all',%gendoc);generate_aux_tex_file(my_list,'html',%gendoc);

//STEP_3b : Generate tex files for the scicos editor
//
//-->gen_scs_editor_help('html',%gendoc);

//STEP_4 : Convert auxiliaries tex file in html.
//
//-->generate_html_file(my_list,%gendoc);

//STEP_5 : Look your generated html files in target html directories.
//         GOTO STEP_2 if you are not happy, else continue.
//
//  PS : NEVER re-run import_data_to_file at this step because
//       you will earase your modifications that you have just
//       made in xml/tex files of STEP_2.

//STEP_6 : generate whatis.htm (for all languages)
//
//-->gen_whatis(%gendoc.mpath.data(1)+'/outline.xml',%gendoc);

//STEP_7 : Update data files with your modifications.
//
//         Before doing that, save locally all your xml/tex files that you have
//         modified/created (don't save generated TeX files of STEP_3,
//         only files of STEP_2).
//
//         Then two choices can be done :
//
//         a - Ask to Alan to explain to you how commit your modification
//             in data files OR send him by email your tex/xml files that
//             you have locally saved (in a single tar.gz file please).
//
//         b - Demerden sie sich !
//             i    - Use :
//                   --> export_file_to_data('all',%gendoc);
//                   This cmd will update all the data files from you local xml/tex files
//                   modified/updated in STEP_2.
//                   (warning should properly work only with subversion of scitexgendoc >= 97)
//             ii   - erase all you generated files
//                   $ rm -fr scicos_doc/eng/xml
//                   $ rm -fr scicos_doc/eng/tex
//                   $ rm -fr scicos_doc/fr/xml
//                   $ rm -fr scicos_doc/fr/tex
//                   $ rm -fr scicos_doc/help
//             iii  - re-generate all html doc of scicos (STEP_1:STEP_5)
//             iiii - look if you dont't have forget any data in html doc freshly regenerated.
//                    If something is wrong please check what's your errors by comparing data files
//                    of STEP_7-b-i with current data files of the svn rep of B4_scicos.

//STEP_8 : Commit data files of scicos_doc/eng/data - scicos_doc/fr/data
//         in the svn rep of B4_scicos
