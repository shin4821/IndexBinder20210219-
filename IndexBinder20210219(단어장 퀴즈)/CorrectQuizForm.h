//CorrectQuizForm.h
#ifndef _CORRECTQUIZFORM_H
#define _CORRECTQUIZFORM_H
#include<windows.h>

BOOL CALLBACK CorrectQuizFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CorrectQuizForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL CorrectQuizForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL CorrectQuizForm_OnFirstButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL CorrectQuizForm_OnPreviousButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL CorrectQuizForm_OnNextButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL CorrectQuizForm_OnLastButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL CorrectQuizForm_OnResetButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL CorrectQuizForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam);

#endif//_VOCABULARYFORM_H