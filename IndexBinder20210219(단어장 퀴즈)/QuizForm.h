//QuizForm.h
#ifndef _QUIZFORM_H
#define _QUIZFORM_H
#include<windows.h>

BOOL CALLBACK QuizFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL QuizForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL QuizForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL QuizForm_OnNextButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL QuizForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam);
#endif//_QUIZFORM_H

