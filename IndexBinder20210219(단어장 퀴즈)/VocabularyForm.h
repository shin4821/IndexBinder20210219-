//VocabularyForm.h
#ifndef _VOCABULARYFORM_H
#define _VOCABULARYFORM_H
#include<windows.h>

BOOL CALLBACK VocabularyFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL VocabularyForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL VocabularyForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL VocabularyForm_OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL VocabularyForm_OnPutButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL VocabularyForm_OnFindButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL VocabularyForm_OnPutOutButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL VocabularyForm_OnArrangeButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL VocabularyForm_OnFirstButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL VocabularyForm_OnPreviousButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL VocabularyForm_OnNextButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL VocabularyForm_OnLastButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL VocabularyForm_OnTreeViewItemButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL VocabularyForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam);

#endif//_VOCABULARYFORM_H

