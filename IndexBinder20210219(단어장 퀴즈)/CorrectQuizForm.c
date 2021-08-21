//CorrectQuizForm.c
#include"CorrectQuizForm.h"
#include"Vocabulary.h"
#include"resource.h"
#include"IndexBinder.h"
#include<stdio.h>
#include<stdlib.h>
#pragma warning(disable:4996)

BOOL CALLBACK CorrectQuizFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	BOOL ret;
	switch (message) {
	case WM_INITDIALOG:
		ret = CorrectQuizForm_OnInitDialog(hWnd, wParam, lParam);
		break;

	case WM_COMMAND:
		ret = CorrectQuizForm_OnCommand(hWnd, wParam, lParam);
		break;

	case WM_CLOSE:
		ret = CorrectQuizForm_OnClose(hWnd, wParam, lParam);
		break;

	default:
		ret = FALSE;
		break;
	}
	return ret;
}

BOOL CorrectQuizForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;
	switch (LOWORD(wParam)) {
	case IDC_BUTTON_FIRST:
		ret = CorrectQuizForm_OnFirstButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_PREVIOUS:
		ret = CorrectQuizForm_OnPreviousButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_NEXT:
		ret = CorrectQuizForm_OnNextButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_LAST:
		ret = CorrectQuizForm_OnLastButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_RESET:
		ret = CorrectQuizForm_OnResetButtonClicked(hWnd, wParam, lParam);
		break;

	default:
		ret = FALSE;
		break;
	}
	return ret;
}

BOOL CorrectQuizForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	CorrectQuiz* correctQuiz;
	WordCard* index;
	HWND vocabularyForm;
	Long correctQuizCount;

	//1. �����찡 �����ɶ�
	//1.1 �ܾ��� �����츦 ã�´�.
	vocabularyForm = FindWindow("#32770", "�ܾ���");
	//1.2 �ܾ��� �������� �ܾ��忡�� ������ ã�´�.
	correctQuiz = (CorrectQuiz*)GetProp(vocabularyForm, "correctQuiz");
	correctQuizCount = (Long)GetProp(vocabularyForm, "correctQuizCount"); 

	if (correctQuizCount > 0) {
		//1.3 ���� ��ġ�� ù��° �ܾ�ī��� �ű��.
		index = CorrectQuiz_First(correctQuiz);

		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0, (LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0, (LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0, (LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0, (LPARAM)index->word.example);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ISIMPORTANT), WM_SETTEXT, 0, (LPARAM)index->quiz.isImportant);
	}

	else {
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ISIMPORTANT), WM_SETTEXT, 0, (LPARAM)"");
	}


	return TRUE;
}


BOOL CorrectQuizForm_OnFirstButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	CorrectQuiz* correctQuiz;
	WordCard* index;
	HWND vocabularyForm;

	//6. ó�� ��ư�� Ŭ��������
	if (HIWORD(wParam) == BN_CLICKED) {
	//6.0 �ܾ��� �����츦 ã�´�.
	vocabularyForm = FindWindow("#32770", "�ܾ���");
	//6.1 �ܾ��忡�� ù��° �ܾ�ī�带 ã�´�.
	correctQuiz = (CorrectQuiz*)GetProp(vocabularyForm, "correctQuiz");
		index = CorrectQuiz_First(correctQuiz);

		//6.2 ù��° �ܾ�ī�带 ����Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0, (LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0, (LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0, (LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0, (LPARAM)index->word.example);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ISIMPORTANT), WM_SETTEXT, 0, (LPARAM)index->quiz.isImportant);
	}
	return TRUE;
}

BOOL CorrectQuizForm_OnPreviousButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	CorrectQuiz* correctQuiz;
	WordCard* index;
	HWND vocabularyForm;

	//7. ���� ��ư�� Ŭ��������
	if (HIWORD(wParam) == BN_CLICKED) {
		//7.0 �ܾ��� �����츦 ã�´�.
		vocabularyForm = FindWindow("#32770", "�ܾ���");
		//7.1 �ܾ��忡�� ���� �ܾ�ī�带 ã�´�.
		correctQuiz = (CorrectQuiz*)GetProp(vocabularyForm, "correctQuiz");
		index = CorrectQuiz_Previous(correctQuiz);

		//7.2 ���� �ܾ�ī�带 ����Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0, (LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0, (LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0, (LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0, (LPARAM)index->word.example);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ISIMPORTANT), WM_SETTEXT, 0, (LPARAM)index->quiz.isImportant);
	}
	return TRUE;
}

BOOL CorrectQuizForm_OnNextButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	CorrectQuiz* correctQuiz;
	WordCard* index;
	HWND vocabularyForm;

	//8. ���� ��ư�� Ŭ��������
	if (HIWORD(wParam) == BN_CLICKED) {
		//8.0 �ܾ��� �����츦 ã�´�.
		vocabularyForm = FindWindow("#32770", "�ܾ���");
		//8.1 �ܾ��忡�� ���� �ܾ�ī�带 ã�´�.
		correctQuiz = (CorrectQuiz*)GetProp(vocabularyForm, "correctQuiz");
		index = CorrectQuiz_Next(correctQuiz);

		//8.2 ���� �ܾ�ī�带 ����Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0, (LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0, (LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0, (LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0, (LPARAM)index->word.example);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ISIMPORTANT), WM_SETTEXT, 0, (LPARAM)index->quiz.isImportant);
	}
	return TRUE;
}

BOOL CorrectQuizForm_OnLastButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	CorrectQuiz* correctQuiz;
	WordCard* index;
	HWND vocabularyForm;

	//9. ������ ��ư�� Ŭ������ ��
	if (HIWORD(wParam) == BN_CLICKED) {
		//9.0 �ܾ��� �����츦 ã�´�.
		vocabularyForm = FindWindow("#32770", "�ܾ���");
		//9.1 �ܾ��忡�� ������ �ܾ�ī�带 ã�´�.
		correctQuiz = (CorrectQuiz*)GetProp(vocabularyForm, "correctQuiz");
		index = CorrectQuiz_Last(correctQuiz);

		//9.2 ������ �ܾ�ī�带 ����Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0, (LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0, (LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0, (LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0, (LPARAM)index->word.example);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ISIMPORTANT), WM_SETTEXT, 0, (LPARAM)index->quiz.isImportant);
	}
	return TRUE;
}

BOOL CorrectQuizForm_OnResetButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	IndexBinder* indexBinder;
	CorrectQuiz* correctQuiz;
	Vocabulary* vocabulary;
	WordCard* index;
	WordCard* index_;
	HWND vocabularyForm;
	WordCard wordCard;
	WordCard* previous = NULL;
	WordCard* current;
	Long correctQuizCount=0;

	//10. ���� ��ư�� Ŭ������ ��
	if (HIWORD(wParam) == BN_CLICKED) {
		//10.1 �ܾ��� �����츦 ã�´�.
		vocabularyForm = FindWindow("#32770", "�ܾ���");
		//10.2 �ܾ��忡�� �����Ѵ�.
		indexBinder = (IndexBinder*)GetProp(vocabularyForm, "indexBinder");
		vocabulary = (Vocabulary*)GetWindowLong(vocabularyForm, GWL_USERDATA);
		correctQuiz = (CorrectQuiz*)GetProp(vocabularyForm, "correctQuiz");

		CorrectQuiz_Reset(correctQuiz);

		//10.3 �ϱ�ܾ� �� �̾Ƽ� �ܾ��� �������� �ܾ��忡 �ȴ´�.
		index = CorrectQuiz_First(correctQuiz);
		while (index!=previous) {
			current= CorrectQuiz_Next(correctQuiz);
			previous = index;

			//10.3.1 ���������쿡�� �̴´�.
			wordCard = CorrectQuiz_PutOut(correctQuiz, index);
			//���ο��� �̴´�.
			if (wordCard.word.spelling[0] > 96 && wordCard.word.spelling[0] < 123) {
				IndexBinder_PutOut(indexBinder, wordCard.word.spelling[0] - 32, index);
			}
			else {
				IndexBinder_PutOut(indexBinder, wordCard.word.spelling[0], index);
			}

			//10.3.2 �ܾ��� �����쿡 �ȴ´�.
			index_=Vocabulary_Put(vocabulary, wordCard);
			IndexBinder_Put(indexBinder, index_);

			index = current;
		}
		
		//correctQuizCount�� 0���� �缳���Ѵ�.
		SetProp(vocabularyForm, "correctQuizCount", correctQuizCount);

		//10.4 �ϱ� ��� ���� ó���Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ISIMPORTANT), WM_SETTEXT, 0, (LPARAM)"");


		//10.5 �ܾ��� �����쿡 ù��° �ܾ ����Ѵ�.
		index = Vocabulary_First(vocabulary);
		SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_SPELLING), WM_SETTEXT, 0, 
			(LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0, 
			(LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_MEANING), WM_SETTEXT, 0, 
			(LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0, 
			(LPARAM)index->word.example);
		SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_ISIMPORTANT), WM_SETTEXT, 0, 
			(LPARAM)index->quiz.isImportant);
	}
}

BOOL CorrectQuizForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {

	//11. �ݱ� ��ư�� Ŭ������ ��
	  //11.1 �����츦 �����Ѵ�.
	  EndDialog(hWnd, 0);
	  return TRUE;
}
