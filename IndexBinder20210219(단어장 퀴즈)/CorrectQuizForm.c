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

	//1. 윈도우가 생성될때
	//1.1 단어장 윈도우를 찾는다.
	vocabularyForm = FindWindow("#32770", "단어장");
	//1.2 단어장 윈도우의 단어장에서 질문을 찾는다.
	correctQuiz = (CorrectQuiz*)GetProp(vocabularyForm, "correctQuiz");
	correctQuizCount = (Long)GetProp(vocabularyForm, "correctQuizCount"); 

	if (correctQuizCount > 0) {
		//1.3 현재 위치를 첫번째 단어카드로 옮긴다.
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

	//6. 처음 버튼을 클릭했을때
	if (HIWORD(wParam) == BN_CLICKED) {
	//6.0 단어장 윈도우를 찾는다.
	vocabularyForm = FindWindow("#32770", "단어장");
	//6.1 단어장에서 첫번째 단어카드를 찾는다.
	correctQuiz = (CorrectQuiz*)GetProp(vocabularyForm, "correctQuiz");
		index = CorrectQuiz_First(correctQuiz);

		//6.2 첫번째 단어카드를 출력한다.
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

	//7. 이전 버튼을 클릭했을때
	if (HIWORD(wParam) == BN_CLICKED) {
		//7.0 단어장 윈도우를 찾는다.
		vocabularyForm = FindWindow("#32770", "단어장");
		//7.1 단어장에서 이전 단어카드를 찾는다.
		correctQuiz = (CorrectQuiz*)GetProp(vocabularyForm, "correctQuiz");
		index = CorrectQuiz_Previous(correctQuiz);

		//7.2 이전 단어카드를 출력한다.
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

	//8. 다음 버튼을 클릭했을때
	if (HIWORD(wParam) == BN_CLICKED) {
		//8.0 단어장 윈도우를 찾는다.
		vocabularyForm = FindWindow("#32770", "단어장");
		//8.1 단어장에서 다음 단어카드를 찾는다.
		correctQuiz = (CorrectQuiz*)GetProp(vocabularyForm, "correctQuiz");
		index = CorrectQuiz_Next(correctQuiz);

		//8.2 다음 단어카드를 출력한다.
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

	//9. 마지막 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//9.0 단어장 윈도우를 찾는다.
		vocabularyForm = FindWindow("#32770", "단어장");
		//9.1 단어장에서 마지막 단어카드를 찾는다.
		correctQuiz = (CorrectQuiz*)GetProp(vocabularyForm, "correctQuiz");
		index = CorrectQuiz_Last(correctQuiz);

		//9.2 마지막 단어카드를 출력한다.
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

	//10. 리셋 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//10.1 단어장 윈도우를 찾는다.
		vocabularyForm = FindWindow("#32770", "단어장");
		//10.2 단어장에서 리셋한다.
		indexBinder = (IndexBinder*)GetProp(vocabularyForm, "indexBinder");
		vocabulary = (Vocabulary*)GetWindowLong(vocabularyForm, GWL_USERDATA);
		correctQuiz = (CorrectQuiz*)GetProp(vocabularyForm, "correctQuiz");

		CorrectQuiz_Reset(correctQuiz);

		//10.3 암기단어 다 뽑아서 단어장 윈도우의 단어장에 꽂는다.
		index = CorrectQuiz_First(correctQuiz);
		while (index!=previous) {
			current= CorrectQuiz_Next(correctQuiz);
			previous = index;

			//10.3.1 퀴즈윈도우에서 뽑는다.
			wordCard = CorrectQuiz_PutOut(correctQuiz, index);
			//색인에서 뽑는다.
			if (wordCard.word.spelling[0] > 96 && wordCard.word.spelling[0] < 123) {
				IndexBinder_PutOut(indexBinder, wordCard.word.spelling[0] - 32, index);
			}
			else {
				IndexBinder_PutOut(indexBinder, wordCard.word.spelling[0], index);
			}

			//10.3.2 단어장 윈도우에 꽂는다.
			index_=Vocabulary_Put(vocabulary, wordCard);
			IndexBinder_Put(indexBinder, index_);

			index = current;
		}
		
		//correctQuizCount를 0으로 재설정한다.
		SetProp(vocabularyForm, "correctQuizCount", correctQuizCount);

		//10.4 암기 퀴즈를 공백 처리한다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ISIMPORTANT), WM_SETTEXT, 0, (LPARAM)"");


		//10.5 단어장 윈도우에 첫번째 단어를 출력한다.
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

	//11. 닫기 버튼을 클릭했을 때
	  //11.1 윈도우를 종료한다.
	  EndDialog(hWnd, 0);
	  return TRUE;
}
