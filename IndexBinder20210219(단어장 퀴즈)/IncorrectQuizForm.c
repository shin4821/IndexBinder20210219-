//IncorrectQuizForm.c
#include"IncorrectQuizForm.h"
#include"Vocabulary.h"
#include"QuizForm.h"
#include"resource.h"
#include<stdio.h>
#include<stdlib.h>
#pragma warning(disable:4996)

BOOL CALLBACK IncorrectQuizFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	BOOL ret;
	switch (message) {
	case WM_INITDIALOG:
		ret = IncorrectQuizForm_OnInitDialog(hWnd, wParam, lParam);
		break;

	case WM_COMMAND:
		ret = IncorrectQuizForm_OnCommand(hWnd, wParam, lParam);
		break;

	case WM_CLOSE:
		ret = IncorrectQuizForm_OnClose(hWnd, wParam, lParam);
		break;

	default:
		ret = FALSE;
		break;
	}
	return ret;
}

BOOL IncorrectQuizForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;
	switch (LOWORD(wParam)) {
	case IDC_BUTTON_FIRST:
		ret = IncorrectQuizForm_OnFirstButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_PREVIOUS:
		ret = IncorrectQuizForm_OnPreviousButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_NEXT:
		ret = IncorrectQuizForm_OnNextButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_LAST:
		ret = IncorrectQuizForm_OnLastButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_RESOLVE:
		ret = IncorrectQuizForm_OnResolveButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_PUTIMPORTANTWORD:
		ret = IncorrectQuizForm_OnPutImportantWordButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_PUTOUTIMPORTANTWORD:
		ret = IncorrectQuizForm_OnPutOutImportantWordButtonClicked(hWnd, wParam, lParam);
		break;

	default:
		ret = FALSE;
		break;
	}
	return ret;
}


BOOL IncorrectQuizForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Vocabulary* vocabulary;
	HWND vocabularyForm;
	WordCard* index;


	//1. 윈도우가 생성될 때
	  //1.1 단어장 윈도우를 찾는다.
	  vocabularyForm = FindWindow("#32770", "단어장");
	  //1.2 단어장 윈도우의 단어장에서 못푼 퀴즈를 찾는다.
	  vocabulary= (Vocabulary*)GetWindowLong(vocabularyForm, GWL_USERDATA);

	  //1.3 틀린 퀴즈가 있는 경우, 틀린 퀴즈의 첫번재 단어를 출력한다.
	  index = vocabulary->current;

	  if (index != NULL) {
		  index = Vocabulary_First(vocabulary);
		  SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0,
			  (LPARAM)index->quiz.spelling);
		  SendMessage(GetDlgItem(hWnd, IDC_EDIT_ANSWER), WM_SETTEXT, 0,
			  (LPARAM)index->quiz.answer);
		  SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0,
			  (LPARAM)index->quiz.meaning);
		  SendMessage(GetDlgItem(hWnd, IDC_EDIT_QUIZOX), WM_SETTEXT, 0,
			  (LPARAM)index->quiz.ox);

	  }
	  //1.4 못 푼 퀴즈가 없을 경우, 단어카드를 공백 처리한다.
	  else {
		  SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0, (LPARAM)"");
		  SendMessage(GetDlgItem(hWnd, IDC_EDIT_ANSWER), WM_SETTEXT, 0, (LPARAM)"");
		  SendMessage(GetDlgItem(hWnd, IDC_EDIT_OX), WM_SETTEXT, 0, (LPARAM)"");
		  SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0, (LPARAM)"");
	  }
	  return TRUE;
}

BOOL IncorrectQuizForm_OnFirstButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Vocabulary* vocabulary;
	HWND vocabularyForm;
	WordCard* index;
	
	
	 //1.1 단어장 윈도우를 찾는다.
	vocabularyForm = FindWindow("#32770", "단어장");
	//1.2 단어장 윈도우의 단어장에서 못푼 퀴즈를 찾는다.
	vocabulary = (Vocabulary*)GetWindowLong(vocabularyForm, GWL_USERDATA);

	//1.3 틀린 퀴즈의 첫번재 단어를 출력한다.
	index = Vocabulary_First(vocabulary);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0,
			(LPARAM)index->quiz.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ANSWER), WM_SETTEXT, 0,
			(LPARAM)index->quiz.answer);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0,
			(LPARAM)index->quiz.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_QUIZOX), WM_SETTEXT, 0,
			(LPARAM)index->quiz.ox);
	return TRUE;
}

BOOL IncorrectQuizForm_OnPreviousButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Vocabulary* vocabulary;
	HWND vocabularyForm;
	WordCard* index;


	//1.1 단어장 윈도우를 찾는다.
	vocabularyForm = FindWindow("#32770", "단어장");
	//1.2 단어장 윈도우의 단어장에서 못푼 퀴즈를 찾는다.
	vocabulary = (Vocabulary*)GetWindowLong(vocabularyForm, GWL_USERDATA);

	//1.3 틀린 퀴즈의 이전 단어를 출력한다.
	index = Vocabulary_Previous(vocabulary);
	SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0,
		(LPARAM)index->quiz.spelling);
	SendMessage(GetDlgItem(hWnd, IDC_EDIT_ANSWER), WM_SETTEXT, 0,
		(LPARAM)index->quiz.answer);
	SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0,
		(LPARAM)index->quiz.meaning);
	SendMessage(GetDlgItem(hWnd, IDC_EDIT_QUIZOX), WM_SETTEXT, 0,
		(LPARAM)index->quiz.ox);
	return TRUE;
}

BOOL IncorrectQuizForm_OnNextButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Vocabulary* vocabulary;
	HWND vocabularyForm;
	WordCard* index;


	//1.1 단어장 윈도우를 찾는다.
	vocabularyForm = FindWindow("#32770", "단어장");
	//1.2 단어장 윈도우의 단어장에서 못푼 퀴즈를 찾는다.
	vocabulary = (Vocabulary*)GetWindowLong(vocabularyForm, GWL_USERDATA);

	//1.3 틀린 퀴즈의 다음 단어를 출력한다.
	index = Vocabulary_Next(vocabulary);
	SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0,
		(LPARAM)index->quiz.spelling);
	SendMessage(GetDlgItem(hWnd, IDC_EDIT_ANSWER), WM_SETTEXT, 0,
		(LPARAM)index->quiz.answer);
	SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0,
		(LPARAM)index->quiz.meaning);
	SendMessage(GetDlgItem(hWnd, IDC_EDIT_QUIZOX), WM_SETTEXT, 0,
		(LPARAM)index->quiz.ox);

	return TRUE;
}

BOOL IncorrectQuizForm_OnLastButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Vocabulary* vocabulary;
	HWND vocabularyForm;
	WordCard* index;


	//1.1 단어장 윈도우를 찾는다.
	vocabularyForm = FindWindow("#32770", "단어장");
	//1.2 단어장 윈도우의 단어장에서 못푼 퀴즈를 찾는다.
	vocabulary = (Vocabulary*)GetWindowLong(vocabularyForm, GWL_USERDATA);

	//1.3 틀린 퀴즈의 마지막 단어를 출력한다.
	index = Vocabulary_Last(vocabulary);
	SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0,
		(LPARAM)index->quiz.spelling);
	SendMessage(GetDlgItem(hWnd, IDC_EDIT_ANSWER), WM_SETTEXT, 0,
		(LPARAM)index->quiz.answer);
	SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0,
		(LPARAM)index->quiz.meaning);
	SendMessage(GetDlgItem(hWnd, IDC_EDIT_QUIZOX), WM_SETTEXT, 0,
		(LPARAM)index->quiz.ox);

	return TRUE;
}

BOOL IncorrectQuizForm_OnResolveButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {

	//6. 다시풀기 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//6.2 틀린 문제보기 윈도우를 종료한다.

		EndDialog(hWnd, 0);

		//6.1 퀴즈 윈도우를 출력한다.
		DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_QUIZFORM),
			NULL, QuizFormProc);
		
	}
	return TRUE;
}

BOOL IncorrectQuizForm_OnPutImportantWordButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Vocabulary* vocabulary;
	WordCard* index;
	HWND vocabularyForm;

	//1.중요단어 추가 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		
		//1.1 단어장 윈도우를 찾는다.
		vocabularyForm = FindWindow("#32770", "단어장");
		vocabulary = (Vocabulary*)GetWindowLong(vocabularyForm, GWL_USERDATA);
		//1.2 현재 단어카드의 위치를 읽는다.
		index = vocabulary->current;
		//1.3 단어장 윈도우의 단어장에서 중요단어 추가한다.
		index = PutImportantWord(vocabulary, index);
		//1.4 오답확인 윈도우의 단어카드에 '중요'를 출력한다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ISIMPORTANT), WM_SETTEXT, 0, (LPARAM)index->quiz.isImportant);
	}
	return TRUE;
}

BOOL IncorrectQuizForm_OnPutOutImportantWordButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Vocabulary* vocabulary;
	HWND vocabularyForm;
	WordCard* index;

	//2. 중요단어 취소 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//1.1 단어장 윈도우를 찾는다.
		vocabularyForm = FindWindow("#32770", "단어장");
		vocabulary = (Vocabulary*)GetWindowLong(vocabularyForm, GWL_USERDATA);
		//1.2 현재 단어카드의 위치를 읽는다.
		index = vocabulary->current;
		//2.3 단어장 윈도우의 단어장에서 중요단어를 취소한다.
		index = PutOutImportantWord(vocabulary, index);
		//2.4 오답확인 윈도우의 단어카드에 '중요'를 지운다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ISIMPORTANT), WM_SETTEXT, 0, (LPARAM)index->quiz.isImportant);
	}
	return TRUE;
}


BOOL IncorrectQuizForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {

	//7. 닫기 버튼을 클릭했을 때
	  //7.` 틀린 문제보기 윈도우를 종료한다.
	EndDialog(hWnd, 0);
	return TRUE;
}


