/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2006-2008 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#pragma comment(lib,"../../bin/libintl.lib")

#include <cstdio>
#include <iostream>
#include <string.h>
#include <fstream>
#include <string>

extern "C"
{
#ifndef _MSC_VER
#include <unistd.h>
#endif

//#include "SetScilabEnvironment.h"
#include "prompt.h"
#include "InitializeLocalization.h"
#include "MALLOC.h"
#include "sci_path.h"
#include "inisci-c.h"
#include "sci_mode.h"
#ifdef _MSC_VER
#include "../src/c/scilab_windows/getScilabDirectory.h"
#endif
#include "ConsoleRead.h"
#include "../../../console/includes/InitializeConsole.h"
#include "../../../jvm/includes/InitializeJVM.h"
#include "InitializeCore.h"
#include "../../../shell/includes/InitializeShell.h"
#include "../../../console/includes/InitializeConsole.h"
#include "../../../tclsci/includes/InitializeTclTk.h"
#include "../../../localization/includes/InitializeLocalization.h"
#include "../../../graphics/includes/graphicModuleLoad.h"
#include "../../../jvm/includes/InitializeJVM.h"
#ifdef _MSC_VER
#include "../../../windows_tools/includes/InitializeWindows_tools.h"
#endif
#include "../../../gui/includes/InitializeGUI.h"
#include "../../../string/includes/InitializeString.h"
#include "../../../jvm/includes/loadBackGroundClassPath.h"

#include "HistoryManager.h"
#include "InitializeHistoryManager.h"
#include "TerminateHistoryManager.h"
#include "getCommentDateSession.h"
#include "os_swprintf.h"
#include "os_strdup.h"
#include "localization.h"
#include "diary.h"
#include "PATH_MAX.h"
#include "sci_tmpdir.h"
#include "deleteafile.h"

#ifdef __APPLE__
#include "../../../shell/src/c/others/initMacOSXEnv.h"
#endif
/*
** HACK HACK HACK
*/
    extern char *TermReadAndProcess(void);
}

#include "yaspio.hxx"
#include "tasks.hxx"
#include "exit_status.hxx"
#include "parser.hxx"
#include "context.hxx"
#include "configvariable.hxx"
#include "context.hxx"
#include "setenvvar.hxx"
#include "funcmanager.hxx"
#include "configvariable.hxx"
#include "filemanager.hxx"
#include "scilabexception.hxx"
#include "banner.hxx"
#include "execvisitor.hxx"

#define INTERACTIVE     -1

const wchar_t* prog_name;
const wchar_t* file_name;

bool parseTrace = false;
bool printAst = false;
bool execAst = true;
bool dumpAst = false;
bool dumpStack = false;
bool timed = false;
bool ASTtimed = false;
bool consoleMode = false;
bool noJvm = false;
bool noStart = false;
bool noBanner = false;
bool execCommand = false;
bool execFile = false;

using symbol::Context;
using std::string;

void Add_i(void);
void Add_pi(void);
void Add_s(void);
void Add_z(void);
void Add_true(void);
void Add_false(void);
void Add_Nan(void);
void Add_Inf(void);
void Add_WITH_DEMOS(void); //temporary variable
void Add_All_Variables(void);

void Add_Double_Constant(wstring _szName, double _dblReal, double _dblImg, bool _bComplex);
void Add_Poly_Constant(wstring _szName, wstring _szPolyVar, int _iRank, Double *_pdblReal);
void Add_Boolean_Constant(wstring _szName, bool _bBool);
void Add_String_Constant(wstring _szName, const char* _pstString);

int InitializeEnvironnement(void);
bool execScilabStart(void);

int StartScilabEngine(int argc, char*argv[], int iFileIndex);


void printLine(char* _stPrompt, char* _stLine, bool _bLF);
void printExp(std::ifstream* _pFile, Exp* _pExp, char* _pstPrompt, int* _piLine /* in/out */, char* _pstPreviousBuffer);

/*
** Usage
**
** Display usage : options available in YaSp
*/
static void usage (void)
{
    std::cerr << "Usage: "<< prog_name << " <options>" << std::endl;
    std::cerr << "      -f file          : Batch mode on the given file." << std::endl;
    std::cerr << "      -l lang          : Change the language of scilab ( default : en_US )." << std::endl;
    std::cerr << "      -nw              : Enable console mode." << std::endl;
    std::cerr << "      -nwni            : Enable terminal mode." << std::endl;
    std::cerr << "      -ns              : Don't execute etc/scilab.start." << std::endl;
    std::cerr << "      --help           : Display this help." << std::endl;
    std::cerr << "Developer Trace arguments:" << std::endl;
    std::cerr << "      --parse-trace    : Display bison state machine evolution." << std::endl;
    std::cerr << "      --AST-trace      : Display ASCII-art AST to be human readable." << std::endl;
    std::cerr << "      --pretty-print   : Display pretty-printed code, standard Scilab syntax." << std::endl;
    std::cerr << " " << std::endl;
    std::cerr << "Developer Timer arguments:" << std::endl;
    std::cerr << "      --AST-timed      : Time each AST node." << std::endl;
    std::cerr << "      --timed          : Time global execution." << std::endl;
    std::cerr << " " << std::endl;
    std::cerr << "Developer Debug arguments:" << std::endl;
    std::cerr << "      --no-exec        : Only do Lexing/parsing do not execute instructions." << std::endl;
    std::cerr << "      --context-dump   : Display context status." << std::endl;
}


/*
** Get Options
**
**
*/
static int get_option (const int argc, char *argv[], int *_piFileIndex, int *_piLangIndex)
{
    int i = 0;

#ifdef DEBUG
    std::cerr << "-*- Getting Options -*-"<< std::endl;
#endif

    for (i = 1; i < argc; ++i) {
        if (!strcmp("--parse-trace", argv[i])) {
            parseTrace = true;
        }
        else if (!strcmp("--pretty-print", argv[i])) {
            printAst = true;
        }
        else if (!strcmp("--help", argv[i])) {
            usage ();
            exit (WELL_DONE);
        }
        else if (!strcmp("--AST-trace", argv[i])) {
            dumpAst = true;
        }
        else if (!strcmp("--no-exec", argv[i])) {
            execAst = false;
        }
        else if (!strcmp("--context-dump", argv[i])) {
            dumpStack = true;
        }
        else if (!strcmp("--timed", argv[i])) {
            timed = true;
        }
        else if (!strcmp("--AST-timed", argv[i])) {
            std::cout << "Timed execution" << std::endl;
            ASTtimed = true;
        }
        else if (!strcmp("-f", argv[i])) {
            i++;
            execFile = true;
            *_piFileIndex = i;
        }
        else if (!strcmp("-e", argv[i])) {
            i++;
            execCommand = true;
            *_piFileIndex = i;
        }
        else if (!strcmp("-l", argv[i])) {
            i++;
            *_piLangIndex = i;
        }
        else if (!strcmp("-nw", argv[i])) {
            consoleMode = true;
            setScilabMode(SCILAB_NW);
        }
        else if (!strcmp("-nwni", argv[i])) {
            consoleMode = true;
            noJvm = true;
            setScilabMode(SCILAB_NWNI);
        }
        else if (!strcmp("-ns", argv[i])) {
            noStart = true;
        }
        else if (!strcmp("-nb", argv[i])) {
            noBanner = true;
        }
    }

#ifdef DEBUG
    std::cerr << "File : " << argv[good] << std::endl;
#endif

    return 0;
}

/*
** HACK HACK HACK
*/

extern "C"
{
#include <stdio.h>

#ifndef _MSC_VER
#include <unistd.h>
#endif
    extern char *TermReadAndProcess(void);
    extern void ConsolePrintf(char*);
}


/*
** -*- Batch Main -*-
*/
static int batchMain (void)
{
/*
** -*- PARSING -*-
*/
    //Parser *parser = new Parser();
    //parser->setParseTrace(parseTrace);
//
//    //Change parsing methode to parse line by line and not entire file
//    char* pstFilename = wide_string_to_UTF8(file_name);
//    char pstRead[4096] = {0};
//    char* pstCommand = NULL;
//    FILE* fileIn = NULL;
//#ifdef _MSC_VER
//    fopen_s(&fileIn, pstFilename, "r");
//#else
//    fileIn = fopen(pstFilename, "r");
//#endif
//    if(fileIn == NULL)
//    {
//        wchar_t szError[bsiz];
//        os_swprintf(szError, bsiz, _W("%ls: Cannot open file %ls.\n"), L"parser", file_name);
//        throw ast::ScilabError(szError, 999, *new Location());
//    }
//
//    while (!ConfigVariable::getForceQuit() && fgets(pstRead, 4096, fileIn) != 0)
//    {
//        if(pstCommand == NULL)
//        {
//            pstCommand = os_strdup(pstRead);
//        }
//        else
//        {
//            //+1 for null termination and +1 for '\n'
//            size_t iLen = strlen(pstCommand) + strlen(pstRead) + 2;
//            char* pstNewCommand = (char*)MALLOC(iLen * sizeof(char));
//#ifdef _MSC_VER
//            sprintf_s(pstNewCommand, iLen, "%s\n%s", pstCommand, pstRead);
//#else
//            sprintf(pstNewCommand, "%s\n%s", pstCommand, pstRead);
//#endif
//            FREE(pstCommand);
//            pstCommand = pstNewCommand;
//        }
//
//        if (strcmp(pstCommand, "") != 0 && strcmp(pstCommand, "\n") != 0)
//        {
//            wchar_t* pwstCommand = to_wide_string(pstCommand);
//            /*
//            ** -*- PARSING -*-
//            */
//            parseCommandTask(parser, timed, pwstCommand);
//
//            /*
//            ** -*- DUMPING TREE -*-
//            */
//            if (dumpAst == true) { dumpAstTask(parser->getTree(), timed); }
//
//            if (parser->getExitStatus() == Parser::Succeded)
//            {
//                //update diary
//                diaryWrite(pwstCommand, TRUE);
//
//                /*
//                ** -*- PRETTY PRINT TREE -*-
//                */
//                if (printAst == true) { printAstTask(parser->getTree(), timed); }
//
//                /*
//                ** -*- EXECUTING TREE -*-
//                */
//                if (execAst == true) { execAstTask(parser->getTree(), timed, ASTtimed); }
//
//                /*
//                ** -*- DUMPING STACK AFTER EXECUTION -*-
//                */
//                if (dumpStack == true) { dumpStackTask(timed); }
//
//                FREE(pstCommand);
//                pstCommand = NULL;
//            }
//            else if(parser->getExitStatus() == Parser::Failed && parser->getControlStatus() == Parser::AllControlClosed)
//            {
//                YaspWriteW(parser->getErrorMessage());
//            }
//
//            parser->freeTree();
//            FREE(pwstCommand);
//        }
//        else
//        {
//            FREE(pstCommand);
//            pstCommand = NULL;
//        }
//        pstRead[0] = '\0';
//    }
//
//    fclose(fileIn);
//
//    if(parser->getExitStatus() == Parser::Failed)
//    {
//        YaspWriteW(parser->getErrorMessage());
//        ConfigVariable::setExitStatus(parser->getExitStatus());
//    }
//
//#ifdef DEBUG
//    std::cerr << "To end program press [ENTER]" << std::endl;
//#endif
//    return ConfigVariable::getExitStatus();

    Parser parser;
    parser.setParseTrace(parseTrace);
    parseFileTask(&parser, timed, file_name, prog_name);

    /*
    ** -*- DUMPING TREE -*-
    */
    if (dumpAst == true) { dumpAstTask(parser.getTree(), timed); }

    if (parser.getExitStatus() != Parser::Succeded)
    {
        YaspWriteW(parser.getErrorMessage());
        return PARSE_ERROR;
    }

    /*
    ** -*- PRETTY PRINT TREE -*-
    */
    if (printAst == true) { printAstTask(parser.getTree(), timed); }

    /*
    ** -*- EXECUTING TREE -*-
    */
    if (execAst == true)
    {
        Exp* pExp = parser.getTree();
        std::list<Exp *>::iterator j;
        std::list<Exp *>LExp = ((SeqExp*)pExp)->exps_get();

        char stPrompt[64];
        //get prompt
        GetCurrentPrompt(stPrompt);

        char* pstFile = wide_string_to_UTF8(file_name);
        std::ifstream file(pstFile);


        char str[1024];
        int iCurrentLine = -1; //no data in str


        for(j = LExp.begin() 
            ; j != LExp.end() && !ConfigVariable::getForceQuit() 
            ; j++)
        {
            try
            {
                //if(checkPrompt(iMode, EXEC_MODE_MUTE))
                //{
                //    //manage mute option
                //    (*j)->mute();
                //    MuteVisitor mute;
                //    (*j)->accept(mute);
                //}
                //else if(checkPrompt(iMode, EXEC_MODE_VERBOSE))
                //{
                printExp(&file, *j, stPrompt, &iCurrentLine, str);
                //}

                //excecute script
                ExecVisitor execMe;
                (*j)->accept(execMe);

                bool bImplicitCall = false;

                //to manage call without ()
                if(execMe.result_get() != NULL && execMe.result_get()->getAsCallable())
                {
                    Callable *pCall = execMe.result_get()->getAsCallable();
                    types::typed_list out;
                    types::typed_list in;

                    ExecVisitor execCall;
                    Function::ReturnValue Ret = pCall->call(in, 1, out, &execCall);

                    if(Ret == Callable::OK)
                    {
                        if(out.size() == 0)
                        {
                            execMe.result_set(NULL);
                        }
                        else if(out.size() == 1)
                        {
                            out[0]->DecreaseRef();
                            execMe.result_set(out[0]);
                        }
                        else
                        {
                            for(int i = 0 ; i < static_cast<int>(out.size()) ; i++)
                            {
                                out[i]->DecreaseRef();
                                execMe.result_set(i, out[i]);
                            }
                        }

                        bImplicitCall = true;
                    }
                    else if(Ret == Callable::Error)
                    {
                        if(ConfigVariable::getLastErrorFunction() == L"")
                        {
                            ConfigVariable::setLastErrorFunction(pCall->getName());
                        }

                        if(pCall->isMacro() || pCall->isMacroFile())
                        {
                            wchar_t szError[bsiz];
                            os_swprintf(szError, bsiz, _W("at line % 5d of function %ls called by :\n"), (*j)->location_get().first_line, pCall->getName().c_str());
                            throw ScilabMessage(szError);
                        }
                        else
                        {
                            throw ScilabMessage();
                        }
                    }
                }

                //update ans variable.
                if(execMe.result_get() != NULL && execMe.result_get()->isDeletable())
                {
                    wstring varName = L"ans";
                    symbol::Context::getInstance()->put(varName, *execMe.result_get());
                    if((*j)->is_verbose())
                    {
                        std::wostringstream ostr;
                        ostr << L"ans = " << std::endl;
                        ostr << std::endl;
                        ostr << execMe.result_get()->toString(ConfigVariable::getFormat(), ConfigVariable::getConsoleWidth()) << std::endl;
                        YaspWriteW(ostr.str().c_str());
                    }
                }
                YaspWriteW(L"\n");

            }
            catch(ScilabMessage sm)
            {
                YaspWriteW(sm.GetErrorMessage().c_str());

                CallExp* pCall = dynamic_cast<CallExp*>(*j);
                if(pCall != NULL)
                {//to print call expression only of it is a macro
                    ExecVisitor execFunc;
                    pCall->name_get().accept(execFunc);

                    if(execFunc.result_get() != NULL &&
                        (execFunc.result_get()->isMacro() || execFunc.result_get()->isMacroFile()))
                    {
                        wostringstream os;

                        //add function failed
                        PrintVisitor printMe(os);
                        pCall->accept(printMe);
                        os << std::endl;

                        //add info on file failed
                        wchar_t szError[bsiz];
                        os_swprintf(szError, bsiz, _W("at line % 5d of exec file called by :\n"), (*j)->location_get().first_line);
                        os << szError;

                        if(ConfigVariable::getLastErrorFunction() == L"")
                        {
                            ConfigVariable::setLastErrorFunction(execFunc.result_get()->getAsCallable()->getName());
                        }

                        throw ScilabMessage(os.str(), 0, (*j)->location_get());
                    }
                }

                throw ScilabMessage((*j)->location_get());
            }
            catch(ScilabError se)
            {
                break;
            }
        }

        parser.freeTree();
        file.close();


        //old version

        ////save current prompt mode
        //ConfigVariable::PromptMode oldVal = ConfigVariable::getPromptMode();
        ////set mode silent for errors
        //ConfigVariable::setPromptMode(ConfigVariable::silent);
        //execAstTask(parser->getTree(), timed, ASTtimed);
        ////restore previous prompt mode
        //ConfigVariable::setPromptMode(oldVal);
    }

    /*
    ** -*- DUMPING STACK AFTER EXECUTION -*-
    */
    if (dumpStack == true) { dumpStackTask(timed); }

#ifdef DEBUG
    std::cerr << "To end program press [ENTER]" << std::endl;
#endif

    return ConfigVariable::getExitStatus();
}

/*
** -*- stateView
** Used to show parser state.
** Find if we are stuck within some control structure.
*/

static void stateShow(Parser::ControlStatus status)
{
    switch (status)
    {
    case Parser::WithinFor :            SetTemporaryPrompt("-for       ->"); break;
    case Parser::WithinWhile :          SetTemporaryPrompt("-while     ->"); break;
    case Parser::WithinIf :             SetTemporaryPrompt("-if        ->"); break;
    case Parser::WithinElse :           SetTemporaryPrompt("-else      ->"); break;
    case Parser::WithinElseIf :         SetTemporaryPrompt("-elseif    ->"); break;
    case Parser::WithinTry :            SetTemporaryPrompt("-try       ->"); break;
    case Parser::WithinCatch :          SetTemporaryPrompt("-catch     ->"); break;
    case Parser::WithinFunction :       SetTemporaryPrompt("-function  ->"); break;
    case Parser::WithinSelect :         SetTemporaryPrompt("-select    ->"); break;
    case Parser::WithinCase :           SetTemporaryPrompt("-case      ->"); break;
    case Parser::WithinSwitch :         SetTemporaryPrompt("-switch    ->"); break;
    case Parser::WithinOtherwise :      SetTemporaryPrompt("-otherwise ->"); break;
    case Parser::WithinMatrix :         SetTemporaryPrompt("- [        ->"); break;
    case Parser::WithinCell :           SetTemporaryPrompt("- {        ->"); break;
    case Parser::WithinBlockComment :   SetTemporaryPrompt("- /*       ->"); break;
    case Parser::WithinDots :           SetTemporaryPrompt("- ...      ->"); break;
    case Parser::AllControlClosed :     break;
    }
}

/*
** -*- Interactive Main -*-
*/
static int interactiveMain (void)
{
    bool exit = false;
    int pause = 0;
    char *command = NULL;
    Parser *parser = new Parser();
    parser->setParseTrace(parseTrace);

    if(noBanner == false)
    {
        banner();
    }

    InitializeHistoryManager();
/* add date & time @ begin session */
    char *commentbeginsession = getCommentDateSession();
    if (commentbeginsession)
    {
        appendLineToScilabHistory(commentbeginsession);
        FREE(commentbeginsession);
        commentbeginsession=NULL;
    }



    while (!ConfigVariable::getForceQuit())
    {
        // Show Parser Sate before prompt
        stateShow(parser->getControlStatus());

        //set prompt value
        C2F(setprlev)(&pause);

        if (parser->getControlStatus() == Parser::AllControlClosed)
        {
            if(command)
            {
                FREE(command);
                command = NULL;
            }
            command = YaspRead();
        }
        else
        {
            char* pstRead = YaspRead();
            //+1 for null termination and +1 for '\n'
            size_t iLen = strlen(command) + strlen(pstRead) + 2;
            char* pstNewCommand = (char*)MALLOC(iLen * sizeof(char));
#ifdef _MSC_VER
            sprintf_s(pstNewCommand, iLen, "%s\n%s", command, pstRead);
#else
            sprintf(pstNewCommand, "%s\n%s", command, pstRead);
#endif
            FREE(pstRead);
            FREE(command);
            command = pstNewCommand;
        }

        //std::cout << "---" << std::endl << "Command = " << std::endl;
        //std::cout << command << std::endl;
        //std::cout << "---" << std::endl;

        if (strcmp(command, "") != 0)
        {
            wchar_t* pstCommand = to_wide_string(command);
            /*
            ** -*- PARSING -*-
            */
            parseCommandTask(parser, timed, pstCommand);

            /*
            ** -*- DUMPING TREE -*-
            */
            if (dumpAst == true) { dumpAstTask(parser->getTree(), timed); }

            if (parser->getExitStatus() == Parser::Succeded)
            {
                /*
                ** -*- PRETTY PRINT TREE -*-
                */
                if (printAst == true) { printAstTask(parser->getTree(), timed); }

                /*
                ** -*- EXECUTING TREE -*-
                */
                if (execAst == true) { execAstTask(parser->getTree(), timed, ASTtimed); }

                /*
                ** -*- DUMPING STACK AFTER EXECUTION -*-
                */
                if (dumpStack == true) { dumpStackTask(timed); }
            }
            else if(parser->getExitStatus() == Parser::Failed && parser->getControlStatus() == Parser::AllControlClosed)
            {
                YaspWriteW(parser->getErrorMessage());
            }

            parser->freeTree();
            FREE(pstCommand);
        }
    }
#ifdef DEBUG
    std::cerr << "To end program press [ENTER]" << std::endl;
#endif
    return ConfigVariable::getExitStatus();
}

static void TermPrintf(char *text)
{
    std::cout << text;
}

/*
** -*- MAIN -*-
*/
int main(int argc, char *argv[])
{
    int iFileIndex = INTERACTIVE;
    int iLangIndex = 0;

    prog_name = to_wide_string(argv[0]);

    setScilabMode(SCILAB_STD);
    get_option(argc, argv, &iFileIndex, &iLangIndex);

// if WITHOUT_GUI is defined
// force Terminal IO -> Terminal IO + StartScilabEngine

// WITHOUT_GUI (All Platform) => Terminal IO + StartScilabEngine
// GUI (MacOSX) =>      [no option]     -> Console IO + InitMacOSXEnv
//                      | [-nwni]       -> Terminal IO + StartScilabEngine
//                      | [-nw]         -> Terminal IO + InitMacOSXEnv
#ifndef WITHOUT_GUI
    if (consoleMode)
    {
        setYaspInputMethod(&TermReadAndProcess);
        setYaspOutputMethod(&TermPrintf);
  #if defined(__APPLE__)
        if(!noJvm)
        {
            return initMacOSXEnv(argc, argv, iFileIndex);
        }
  #endif // !defined(__APPLE__)
        return StartScilabEngine(argc, argv, iFileIndex);
    }
    else
    {
        setYaspInputMethod(&ConsoleRead);
        setYaspOutputMethod(&ConsolePrintf);
  #if defined(__APPLE__)
        return initMacOSXEnv(argc, argv, iFileIndex);
  #else
        return StartScilabEngine(argc, argv, iFileIndex);
  #endif // !defined(__APPLE__)
    }
#else
        setYaspInputMethod(&TermReadAndProcess);
        setYaspOutputMethod(&TermPrintf);
        return StartScilabEngine(argc, argv, iFileIndex);
#endif // defined(WITHOUT_GUI)
}

int StartScilabEngine(int argc, char*argv[], int iFileIndex)
{
    int iMainRet = 0;
    /* Scilab Startup */
    InitializeEnvironnement();

    InitializeString();

#ifdef _MSC_VER
    InitializeWindows_tools();
#endif

    //InitializeCore();

    InitializeShell();

    if (!noJvm)
    {
        /* bug 3702 */
        /* tclsci creates a TK window on Windows */
        /* it changes focus on previous windows */
        /* we put InitializeTclTk before InitializeGUI */

        //InitializeTclTk();
        InitializeJVM();
        InitializeGUI();

        /* create needed data structure if not already created */
        loadGraphicModule() ;

        /* Standard mode -> init Java Console */
        if ( !consoleMode )
        {
            /* Initialize console: lines... */
            InitializeConsole();
        }

        loadBackGroundClassPath();
    }

    /* set current language of scilab */
    FuncManager *pFM = new FuncManager();
    pFM->LoadModules(noStart);

    //execute scilab.start
    if(noStart == false)
    {
        execScilabStartTask();
    }



    if(execCommand)
    {//-e option, create a file with command and run as batch
        ConfigVariable::setPromptMode(ConfigVariable::prompt);
        char szFile[PATH_MAX];
        sprintf(szFile, "%s\\%s", getTMPDIR(), "execcommand.temp");
        FILE* fExec = NULL;
#ifdef _MSC_VER
        fopen_s(&fExec, szFile, "w");
#else
        fExec = fopen(szFile, "w");
#endif
        fwrite(argv[iFileIndex], sizeof(char), strlen(argv[iFileIndex]), fExec);
        fclose(fExec);
        file_name = to_wide_string(szFile);
        iMainRet = batchMain();
        deleteafile(szFile);
    }
    else if(execFile)
    {//-f option
        ConfigVariable::setPromptMode(ConfigVariable::silent);
        file_name = to_wide_string(argv[iFileIndex]);
        iMainRet = batchMain();
    }
    ConfigVariable::setPromptMode(ConfigVariable::normal);

    //always run as interactiveMain even after -e or -f option
    file_name = L"prompt";
    iMainRet = interactiveMain();

    //close main scope
    symbol::Context::getInstance()->scope_end();
    delete pFM;
    return iMainRet;
}

int InitializeEnvironnement(void)
{
    SetScilabEnvironment();
    InitializeLocalization();

    ConfigVariable::setConsoleWidth(75);
    ConfigVariable::setFormat(16);
    Add_All_Variables();
    FileManager::initialize();
    return 0;
}

void Add_All_Variables(void)
{
    Add_pi();
    Add_i();
    Add_s();
    Add_z();
    Add_true();
    Add_false();
    Add_Nan();
    Add_Inf();
    Add_WITH_DEMOS();
}

void Add_WITH_DEMOS(void)
{
    Add_Boolean_Constant(L"WITH_DEMOS", false);
}

void Add_Nan(void)
{
    double dbl1 = 1.0;
    double dbl0 = dbl1 - dbl1;
    Add_Double_Constant(L"%nan", dbl0/dbl0, 0, false);
}

void Add_Inf(void)
{
    double dbl1 = 1.0;
    double dbl0 = dbl1 - dbl1;
    Add_Double_Constant(L"%inf", dbl1/dbl0, 0, false);
}

void Add_false(void)
{
    Add_Boolean_Constant(L"%f", false);
}

void Add_true(void)
{
    Add_Boolean_Constant(L"%t", true);
}

void Add_pi(void)
{
    Add_Double_Constant(L"%pi", 3.1415926535897931159980, 0, false);
}

void Add_i(void)
{
    Add_Double_Constant(L"%i", 0, 1, true);
}

void Add_s(void)
{
    Double dblCoef(1,2);
    dblCoef.val_set(0, 0, 0);
    dblCoef.val_set(0, 1, 1);

    Add_Poly_Constant(L"%s",L"s", 2, &dblCoef);
}

void Add_z(void)
{
    Double dblCoef(1,2);
    dblCoef.val_set(0, 0, 0);
    dblCoef.val_set(0, 1, 1);

    Add_Poly_Constant(L"%z",L"z", 2, &dblCoef);
}

void Add_Poly_Constant(wstring _szName, wstring _szPolyVar, int _iRank, Double *_pdbl)
{
    types::MatrixPoly *pVar = new types::MatrixPoly(_szPolyVar, 1, 1, &_iRank);
    Poly *poPoly = pVar->poly_get(0,0);
    poPoly->coef_set(_pdbl);
    Context::getInstance()->put(_szName, *pVar);
}

void Add_Double_Constant(wstring _szName, double _dblReal, double _dblImg, bool _bComplex)
{
    types::Double* pVal = new types::Double(1,1,_bComplex);
    pVal->val_set(0,0,_dblReal,_dblImg);
    symbol::Context::getInstance()->put(_szName, *pVal);
}

void Add_Boolean_Constant(wstring _szName, bool _bBool)
{
    types::Bool* pVal = new types::Bool(_bBool);
    symbol::Context::getInstance()->put(_szName, *pVal);
}

void Add_String_Constant(wstring _szName, const char* _pstString)
{
    types::String* ps = new types::String(_pstString);
    symbol::Context::getInstance()->put(_szName, *ps);
}

void printExp(std::ifstream* _pFile, Exp* _pExp, char* _pstPrompt, int* _piLine /* in/out */, char* _pstPreviousBuffer)
{
	char strLastLine[1024];
	//case 1, exp is on 1 line and take the entire line

	//case 2, exp is multiline

	//case 3, exp is part of a line.
	//ex : 3 exp on the same line a = 1; b = 2; c = 3;

	//case 4, exp is multiline but start and/or finish in the middle of a line
	//ex :
	//a = 10;for i = 1 : a
	//	a
	//end, b = 1;

	Location loc = _pExp->location_get();

	//positionning file curser at loc.first_line
	{
		//strange case, current position is after the wanted position
		if(*_piLine > loc.first_line)
		{
			//reset line counter and restart reading at the start of the file.
			*_piLine = -1;
			_pFile->seekg( 0, ios_base::beg );
		}

		//bypass previous lines
		for(int i = *_piLine ; i < loc.first_line - 1; i++)
		{
			(*_piLine)++;
			_pFile->getline(_pstPreviousBuffer, 1024);
		}
	}

	if(loc.first_line == loc.last_line)
	{//1 line
		strncpy(strLastLine, _pstPreviousBuffer + (loc.first_column - 1), loc.last_column - (loc.first_column - 1));
		strLastLine[loc.last_column - (loc.first_column - 1)] = 0;
        int iLineLen = strlen(_pstPreviousBuffer);
        int iExpLen = strlen(strLastLine);
        if(iExpLen + (loc.first_column - 1) == iLineLen)
        {
            printLine(_pstPrompt, strLastLine, true);
        }
        else
        {
            printLine(_pstPrompt, strLastLine, false);
        }
	}
	else
	{//multiline
		printLine(_pstPrompt, _pstPreviousBuffer + (loc.first_column - 1), true);

		//print other full lines
		for(int i = loc.first_line; i < (loc.last_line - 1) ; i++)
		{
			(*_piLine)++;
			_pFile->getline(_pstPreviousBuffer, 1024);
			printLine(_pstPrompt, _pstPreviousBuffer, true);
		}

		//last line
		_pFile->getline(_pstPreviousBuffer, 1024);
		(*_piLine)++;

		strncpy(strLastLine, _pstPreviousBuffer, loc.last_column);
		strLastLine[loc.last_column] = 0;
		printLine(_pstPrompt, strLastLine, true);
	}
}

void printLine(char* _stPrompt, char* _stLine, bool _bLF)
{
    wchar_t* pwstPrompt = to_wide_string(_stPrompt);
	diaryWrite(pwstPrompt, TRUE);
    FREE(pwstPrompt);

    wchar_t* pwstLine = to_wide_string(_stLine);
	diaryWrite(pwstLine, TRUE);
    FREE(pwstLine);

    if(_bLF)
    {
        diaryWrite(L"\n", TRUE);
    }

	////print prompt
	//YaspWrite(_stPrompt);
	////print first line
	//YaspWrite(_stLine);
	//YaspWrite("\n");
}
/*--------------------------------------------------------------------------*/
