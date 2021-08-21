//ImportantWordsForm.h
#ifndef _IMPORTANTWORDSFORM_H
#define _IMPORTANTWORDSFORM_H
#include<windows.h>

BOOL CALLBACK ImportantWordsFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL ImportantWordsForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL ImportantWordsForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL ImportantWordsForm_OnFirstButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL ImportantWordsForm_OnPreviousButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL ImportantWordsForm_OnNextButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL ImportantWordsForm_OnLastButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL ImportantWordsForm_OnPutOutImportantWordButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL ImportantWordsForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam);

#endif//_IMPORTANTWORDSFORM
