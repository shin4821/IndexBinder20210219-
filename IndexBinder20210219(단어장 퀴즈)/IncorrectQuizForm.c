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


	//1. �����찡 ������ ��
	  //1.1 �ܾ��� �����츦 ã�´�.
	  vocabularyForm = FindWindow("#32770", "�ܾ���");
	  //1.2 �ܾ��� �������� �ܾ��忡�� ��Ǭ ��� ã�´�.
	  vocabulary= (Vocabulary*)GetWindowLong(vocabularyForm, GWL_USERDATA);

	  //1.3 Ʋ�� ��� �ִ� ���, Ʋ�� ������ ù���� �ܾ ����Ѵ�.
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
	  //1.4 �� Ǭ ��� ���� ���, �ܾ�ī�带 ���� ó���Ѵ�.
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
	
	
	 //1.1 �ܾ��� �����츦 ã�´�.
	vocabularyForm = FindWindow("#32770", "�ܾ���");
	//1.2 �ܾ��� �������� �ܾ��忡�� ��Ǭ ��� ã�´�.
	vocabulary = (Vocabulary*)GetWindowLong(vocabularyForm, GWL_USERDATA);

	//1.3 Ʋ�� ������ ù���� �ܾ ����Ѵ�.
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


	//1.1 �ܾ��� �����츦 ã�´�.
	vocabularyForm = FindWindow("#32770", "�ܾ���");
	//1.2 �ܾ��� �������� �ܾ��忡�� ��Ǭ ��� ã�´�.
	vocabulary = (Vocabulary*)GetWindowLong(vocabularyForm, GWL_USERDATA);

	//1.3 Ʋ�� ������ ���� �ܾ ����Ѵ�.
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


	//1.1 �ܾ��� �����츦 ã�´�.
	vocabularyForm = FindWindow("#32770", "�ܾ���");
	//1.2 �ܾ��� �������� �ܾ��忡�� ��Ǭ ��� ã�´�.
	vocabulary = (Vocabulary*)GetWindowLong(vocabularyForm, GWL_USERDATA);

	//1.3 Ʋ�� ������ ���� �ܾ ����Ѵ�.
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


	//1.1 �ܾ��� �����츦 ã�´�.
	vocabularyForm = FindWindow("#32770", "�ܾ���");
	//1.2 �ܾ��� �������� �ܾ��忡�� ��Ǭ ��� ã�´�.
	vocabulary = (Vocabulary*)GetWindowLong(vocabularyForm, GWL_USERDATA);

	//1.3 Ʋ�� ������ ������ �ܾ ����Ѵ�.
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

	//6. �ٽ�Ǯ�� ��ư�� Ŭ������ ��
	if (HIWORD(wParam) == BN_CLICKED) {
		//6.2 Ʋ�� �������� �����츦 �����Ѵ�.

		EndDialog(hWnd, 0);

		//6.1 ���� �����츦 ����Ѵ�.
		DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_QUIZFORM),
			NULL, QuizFormProc);
		
	}
	return TRUE;
}

BOOL IncorrectQuizForm_OnPutImportantWordButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Vocabulary* vocabulary;
	WordCard* index;
	HWND vocabularyForm;

	//1.�߿�ܾ� �߰� ��ư�� Ŭ������ ��
	if (HIWORD(wParam) == BN_CLICKED) {
		
		//1.1 �ܾ��� �����츦 ã�´�.
		vocabularyForm = FindWindow("#32770", "�ܾ���");
		vocabulary = (Vocabulary*)GetWindowLong(vocabularyForm, GWL_USERDATA);
		//1.2 ���� �ܾ�ī���� ��ġ�� �д´�.
		index = vocabulary->current;
		//1.3 �ܾ��� �������� �ܾ��忡�� �߿�ܾ� �߰��Ѵ�.
		index = PutImportantWord(vocabulary, index);
		//1.4 ����Ȯ�� �������� �ܾ�ī�忡 '�߿�'�� ����Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ISIMPORTANT), WM_SETTEXT, 0, (LPARAM)index->quiz.isImportant);
	}
	return TRUE;
}

BOOL IncorrectQuizForm_OnPutOutImportantWordButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Vocabulary* vocabulary;
	HWND vocabularyForm;
	WordCard* index;

	//2. �߿�ܾ� ��� ��ư�� Ŭ������ ��
	if (HIWORD(wParam) == BN_CLICKED) {
		//1.1 �ܾ��� �����츦 ã�´�.
		vocabularyForm = FindWindow("#32770", "�ܾ���");
		vocabulary = (Vocabulary*)GetWindowLong(vocabularyForm, GWL_USERDATA);
		//1.2 ���� �ܾ�ī���� ��ġ�� �д´�.
		index = vocabulary->current;
		//2.3 �ܾ��� �������� �ܾ��忡�� �߿�ܾ ����Ѵ�.
		index = PutOutImportantWord(vocabulary, index);
		//2.4 ����Ȯ�� �������� �ܾ�ī�忡 '�߿�'�� �����.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ISIMPORTANT), WM_SETTEXT, 0, (LPARAM)index->quiz.isImportant);
	}
	return TRUE;
}


BOOL IncorrectQuizForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {

	//7. �ݱ� ��ư�� Ŭ������ ��
	  //7.` Ʋ�� �������� �����츦 �����Ѵ�.
	EndDialog(hWnd, 0);
	return TRUE;
}


