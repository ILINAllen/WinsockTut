﻿/**
 *  @brief:  A simple echo server, use winsock Asynchronous Select I/O model
 *
 * 	@author: ichenq@gmail.com
 *  @date:   Nov 24, 2011
 */

#include "../common/utility.h"
#include "resource.h"
#include "appdef.h"
#include <tchar.h>
#include <assert.h>
#include <algorithm>



#pragma warning(disable: 4996)



INT_PTR CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);



// main entry
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);   

    return DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG_ASYNCSELECT), NULL, DlgProc, (LPARAM)lpCmdLine);
}



INT_PTR CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch(uMsg)
    {
    case WM_INITDIALOG:
        {
            _tstring strAddr = (LPTSTR)lParam;
            std::replace(strAddr.begin(), strAddr.end(), _T(' '), _T(':'));
            if (!InitializeServer(hDlg, strAddr))
            {
                MessageBox(hDlg, _T("$program $host $port"), _T("Usage"), MB_OK);
                return 1;
            }
            _tstring msg = _T("server listen at ") + strAddr;
            SetDlgItemText(hDlg, IDC_EDIT_LOG, msg.data());
        }
        break;

    case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
            case IDOK:
            case IDCANCEL:
                {
                    EndDialog(hDlg, 0);
                }
                break;
            }
        }
        break;

    case WM_SOCKET:
        HandleNetEvents(hDlg, wParam, WSAGETSELECTEVENT(lParam), WSAGETSELECTERROR(lParam));
        break;

    }

    return 0;
}



bool AppendEditText(HWND hdlg, const TCHAR* text, int len)
{
    assert(text && len > 0);
    TCHAR textbuf[BUFSIZ];
    size_t count = GetDlgItemText(hdlg, IDC_EDIT_LOG, textbuf, BUFSIZ);
    if (count == 0 && GetLastError() != S_OK)
    {
        LOG_DEBUG(_T("GetDlgItemText() failed)"));
        return false;
    }
    else
    {
        textbuf[count++] = _T('\r');
        textbuf[count++] = _T('\n');
    }
    if (count + len + 2 >= BUFSIZ)
    {
        LOG_DEBUG(_T("buffer too small, count: %d, text: %s"), count, text);
        return false;
    }
    
    _tcsncpy_s(textbuf + count, BUFSIZ-count, text, len);
    return SetDlgItemText(hdlg, IDC_EDIT_LOG, textbuf) == TRUE;
}
