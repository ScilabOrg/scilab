/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#pragma comment (lib, "gdiplus.lib")
/*--------------------------------------------------------------------------*/ 
#include <Windows.h>
#include <stdio.h>
#include <CommCtrl.h>
#include <GdiPlus.h>
extern "C"
{
#include "splashScreen.h"
#include "localization.h"
#include "resource.h"
#include "version.h"
#include "WndThread.h"
#include "charEncoding.h"
#include "getScilabDirectory.h"
#include "MALLOC.h"
};
/*--------------------------------------------------------------------------*/ 
#define SPLASH_WINDOW_CLASSNAME "Scilab splashscreen"
#define FORMAT_FULLPATH_SPLASH_IMAGE "%s/modules/gui/images/icons/aboutScilab.png" 
#define ID_TIMER_SPLASH 200
/*--------------------------------------------------------------------------*/ 
static DWORD WINAPI ThreadSplashScreen( LPVOID lpParam ) ;
static BOOL stopSplashScreen(void);
static int timeSplashScreen = 0;
UINT_PTR         m_TimerId;
static Gdiplus::Image* pImage = NULL;
/*--------------------------------------------------------------------------*/ 
void splashScreen(void)
{
    DWORD dwThreadId, dwThrdParam = 0; 
    HANDLE hThreadSplashScreen = NULL;
    hThreadSplashScreen = CreateThread(NULL,0,ThreadSplashScreen, &dwThrdParam, 0, &dwThreadId);                

}
/*--------------------------------------------------------------------------*/ 
LRESULT CALLBACK SplashWndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch (uMsg)
    {
    case WM_PAINT:
        {
            if (pImage)
            {
                Gdiplus::Graphics gdip(hwnd);
                gdip.DrawImage(pImage, 0, 0, pImage->GetWidth(), pImage->GetHeight());
            }
            ValidateRect(hwnd, NULL);
            return 0;
        } break;
    case WM_TIMER:
        {
            KillTimer(hwnd,  m_TimerId);
            DestroyWindow(hwnd);
        }
        break;
    case WM_DESTROY: 
        {
            KillTimer(hwnd,  m_TimerId);
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
/*--------------------------------------------------------------------------*/ 

static DWORD WINAPI ThreadSplashScreen( LPVOID lpParam ) 
{ 
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;

    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    HINSTANCE hInstanceThisDll = (HINSTANCE)GetModuleHandle("scilab_windows");

    char *ScilabDirectory = getScilabDirectory(TRUE);
    size_t len = strlen(FORMAT_FULLPATH_SPLASH_IMAGE) + strlen(ScilabDirectory) + 1;
    char *ImageFilename = (char*)MALLOC(sizeof(char) * len);
    sprintf(ImageFilename, FORMAT_FULLPATH_SPLASH_IMAGE, ScilabDirectory);
    wchar_t *wImageFilename = to_wide_string(ImageFilename);
    pImage = Gdiplus::Image::FromFile((const WCHAR *)wImageFilename);

   if (pImage == NULL) return 0;

    WNDCLASS wndcls = {0};

    wndcls.style = CS_HREDRAW | CS_VREDRAW;
    wndcls.lpfnWndProc = SplashWndProc; 
    wndcls.hInstance = GetModuleHandle(NULL);
    wndcls.hCursor = LoadCursor(NULL, IDC_APPSTARTING);
    wndcls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wndcls.lpszClassName = SPLASH_WINDOW_CLASSNAME;
    wndcls.hIcon = LoadIcon(wndcls.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

    if (!RegisterClass(&wndcls))
    {
        if (GetLastError() != 0x00000582) // already registered
        {
            return 0;
        }
    }

    // try to find monitor where mouse was last time
    POINT point = { 0 };
    MONITORINFO mi = { sizeof(MONITORINFO), 0 };
    HMONITOR hMonitor = 0;
    RECT rcArea = { 0 };

    ::GetCursorPos( &point );
    hMonitor = ::MonitorFromPoint( point, MONITOR_DEFAULTTONEAREST );
    if ( ::GetMonitorInfo( hMonitor, &mi ) )
    {
        rcArea.left = ( mi.rcMonitor.right + mi.rcMonitor.left -  pImage->GetWidth())  /2;
        rcArea.top = ( mi.rcMonitor.top + mi.rcMonitor.bottom - pImage->GetHeight())  /2;
    }
    else
    {
        SystemParametersInfo(SPI_GETWORKAREA, NULL, &rcArea, NULL);
        rcArea.left = (rcArea.right + rcArea.left - pImage->GetWidth())/2;
        rcArea.top = (rcArea.top + rcArea.bottom -  pImage->GetHeight())/2;
    }  

    HWND hParent = NULL;


    HWND hdlg = CreateWindowEx(WS_EX_TOOLWINDOW,
        SPLASH_WINDOW_CLASSNAME,
        SPLASH_WINDOW_CLASSNAME, 
        WS_CLIPCHILDREN|WS_POPUP,
        rcArea.left,
        rcArea.top,
        pImage->GetWidth(),
        pImage->GetHeight(), 
        hParent,
        NULL,
        wndcls.hInstance,
        NULL);

    ShowWindow(hdlg, SW_SHOWNORMAL);
    UpdateWindow(hdlg);

    SetWindowPos(hdlg,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
    m_TimerId = SetTimer(hdlg, ID_TIMER_SPLASH, 100, NULL);

    while ( !stopSplashScreen() )
    {
        Sleep(20);
    }

    DestroyWindow(hdlg);
    return 0; 
} 
/*--------------------------------------------------------------------------*/ 
static BOOL stopSplashScreen(void)
{
    HWND hWndMainScilab = NULL;
    char titleMainWindow[MAX_PATH];

    wsprintf(titleMainWindow,"%s (%d)", SCI_VERSION_STRING, getCurrentScilabId());
    hWndMainScilab = FindWindow(NULL, titleMainWindow);

    if ( hWndMainScilab && (timeSplashScreen > 1000) )
    {
        return TRUE;
    }
    else
    {
        wsprintf(titleMainWindow, _("Scilab Console"));
        hWndMainScilab = FindWindow(NULL, titleMainWindow);

        if ( hWndMainScilab && (timeSplashScreen > 1000) )
        {
            return TRUE;
        }
        else
        {
            timeSplashScreen = timeSplashScreen + 50;
        }
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/ 
