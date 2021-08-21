//IncorrectQuizForm.h
#ifndef _INCORRECTQUIZFORM_H
#define _INCORRECTQUIZFORM_H
#include<windows.h>

BOOL CALLBACK IncorrectQuizFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL IncorrectQuizForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL IncorrectQuizForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL IncorrectQuizForm_OnFirstButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL IncorrectQuizForm_OnPreviousButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL IncorrectQuizForm_OnNextButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL IncorrectQuizForm_OnLastButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL IncorrectQuizForm_OnResolveButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL IncorrectQuizForm_OnPutImportantWordButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL IncorrectQuizForm_OnPutOutImportantWordButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL IncorrectQuizForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam);

#endif//_INCORRECTQUIZFORM_H
