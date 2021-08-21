//QuizForm.c
#include"QuizForm.h"
#include"Vocabulary.h"
#include"IncorrectQuizForm.h"
#include"IndexBinder.h"
#include"resource.h"
#include<stdio.h>
#include<stdlib.h>
#pragma warning(disable:4996)

BOOL CALLBACK QuizFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	BOOL ret;
	switch (message) {
	case WM_INITDIALOG:
		ret = QuizForm_OnInitDialog(hWnd, wParam, lParam);
		break;

	case WM_CLOSE:
		ret = QuizForm_OnClose(hWnd, wParam, lParam);
		break;

	case WM_COMMAND:
		ret = QuizForm_OnCommand(hWnd, wParam, lParam);
		break;

	default:
		ret = FALSE;
		break;
	}
	return ret;
}

BOOL QuizForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;

	switch (LOWORD(wParam)) {
	case IDC_BUTTON_NEXT:
		ret = QuizForm_OnNextButtonClicked(hWnd, wParam, lParam);
		break;

	default:
		ret = FALSE;
		break;
	}
	return ret;
}


BOOL QuizForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Vocabulary* vocabulary;
	CorrectQuiz* correctQuiz;
	HWND vocabularyForm;
	WordCard* nextQuiz;
	WordCard* index;
	WordCard* previous = NULL;
	WordCard wordCard;
	int ret;

	//1. 윈도우가 생성될 때
	  //1.1 단어장 윈도우를 찾는다.
	  vocabularyForm = FindWindow("#32770", "단어장");
	  //1.2 단어장 윈도우의 단어장에서 질문을 찾는다.
	  vocabulary = (Vocabulary*)GetWindowLong(vocabularyForm, GWL_USERDATA);
	  correctQuiz = (CorrectQuiz*)GetProp(vocabularyForm, "correctQuiz");
	  nextQuiz = Vocabulary_First(vocabulary);

	  //1.3 질문이 있으면, 퀴즈 철자를 출력한다.
	  if (nextQuiz != NULL) {
		  SetProp(hWnd, "nextQuiz", nextQuiz);
		  SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0,
			  (LPARAM)nextQuiz->quiz.spelling);
	  }
	  //1.4 질문이 없으면
	  else {
		  //1.4.1 메세지를 출력한다.
		  ret = MessageBox(hWnd, "틀린 문제가 없습니다. 문제를 리셋 하시겠습니까?", "확인창", MB_YESNOCANCEL);
		  //1.4.2 '예'버튼을 클릭했을 때
		  if (ret == IDYES) {
			  //1.4.2.1 단어장 윈도우의 단어장에서 리셋한다.
			  CorrectQuiz_Reset(correctQuiz);
			  //1.4.2.2 단어장의 처음부터 끝까지 반복한다. 
			  index = CorrectQuiz_First(correctQuiz);
			  while (correctQuiz->length > 0) {
				  //1.4.2.2.1 퀴즈윈도우에서 뽑는다.
				  wordCard = CorrectQuiz_PutOut(correctQuiz, index);
				  //1.4.2.2.2 단어장 윈도우에 꽂는다.
				  Vocabulary_Put(vocabulary, wordCard);
				  index = correctQuiz->current;
			  }
			  //1.4.2.2 단어장 윈도우의 단어장에서 질문을 찾는다.
			  nextQuiz = Vocabulary_First(vocabulary);
			  SetProp(hWnd, "nextQuiz", nextQuiz);
			  //1.4.2.3 퀴즈 철자를 출력한다.
			  SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0,
				  (LPARAM)nextQuiz->quiz.spelling);
	      }
		  //3.3 '아니오'버튼을 클릭했을 때
		  else if (ret == IDNO) {
			  //3.3.1 퀴즈 윈도우를 종료한다.
			  EndDialog(hWnd, 0);
		  }
	  }
	  return TRUE;
}

BOOL QuizForm_OnNextButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	IndexBinder* indexBinder;
	Vocabulary* vocabulary;
	CorrectQuiz* correctQuiz;
	HWND vocabularyForm;
	TCHAR spelling[32];
	TCHAR meaning[32];
	WordCard* index;
	WordCard* index_;
	WordCard* nextQuiz;
	WordCard* current;
	WordCard wordCard;
	int ret;
	Long correctQuizCount;

	//2. 다음 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//2.1 단어장 윈도우를 찾는다.
		vocabularyForm = FindWindow("#32770", "단어장");

		//2.2 단어장 윈도우의 단어장에서 퀴즈의 위치를 찾는다.
		indexBinder = (IndexBinder*)GetProp(vocabularyForm, "indexBinder");
		vocabulary = (Vocabulary*)GetWindowLong(vocabularyForm, GWL_USERDATA);
		correctQuiz = (CorrectQuiz*)GetProp(vocabularyForm, "correctQuiz");
		index=(WordCard*)GetProp(hWnd, "nextQuiz");

		//2.3 의미를 읽는다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_GETTEXT, (WPARAM)32, (LPARAM)meaning);


		//2.3.1 문제를 푼다.
		Vocabulary_SpellingQuiz(vocabulary, meaning, index);


		//2.4.0 다음 문제를 미리 구해놓는다.
		nextQuiz = NextQuiz(vocabulary);
		SetProp(hWnd, "nextQuiz", nextQuiz);

		//2.4.1 퀴즈가 맞았을 경우, correctQuiz로 옮긴다.

		if (strcmp(index->quiz.ox ,"O")==0) {
			
			//단어장에서 뽑는다.
			wordCard = Vocabulary_PutOut(vocabulary, index);
			//색인에서 뽑는다.
			if (wordCard.word.spelling[0] > 96 && wordCard.word.spelling[0] < 123) {
				IndexBinder_PutOut(indexBinder, wordCard.word.spelling[0] - 32, index);
			}
			else {
				IndexBinder_PutOut(indexBinder, wordCard.word.spelling[0], index);
			}

			//2.4.1.1 단어장 윈도우에 현재 위치의 단어를 출력한다.
			current = vocabulary->current;
			if (current != NULL) {
				SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_SPELLING), WM_SETTEXT, 0,
					(LPARAM)current->word.spelling);
				SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0,
					(LPARAM)current->word.wordClass);
				SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_MEANING), WM_SETTEXT, 0,
					(LPARAM)current->word.meaning);
				SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0,
					(LPARAM)current->word.example);
			}	
			else {
				SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_SPELLING), WM_SETTEXT, 0, (LPARAM)"");
				SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0, (LPARAM)"");
				SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_MEANING), WM_SETTEXT, 0, (LPARAM)"");
				SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0, (LPARAM)"");
			}

			//암기단어에 꽂는다.
			index_=CorrectQuiz_Put(correctQuiz, wordCard);
			correctQuizCount = correctQuiz->length;
			SetProp(vocabularyForm, "correctQuizCount", correctQuizCount);

			IndexBinder_Put(indexBinder, index_);
		}

	
		//2.5 다음 질문이 있으면
		if (nextQuiz != NULL) {
			
			//2.5.1 퀴즈 철자를 출력한다.
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0, (LPARAM)nextQuiz->quiz.spelling);
			//2.5.2 의미를 공백 처리한다.
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0, (LPARAM)"");
		}
		//2.6 다음 질문이 없으면
		else {
			//2.6.1 메세지 박스를 출력한다.
			ret = MessageBox(hWnd, "마지막 단어입니다. 결과를 확인하시겠습니까?", "확인창", MB_YESNOCANCEL);
			//2.6.2. '예'버튼을 클릭했을 때
			if (ret == IDYES) {
				//2.6.2.1 퀴즈 윈도우를 종료한다.
				EndDialog(hWnd, 0);
				//2.6.2.2 오답 윈도우를 출력한다.
				DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), 
					MAKEINTRESOURCE(IDD_INCORRECTQUIZFORM), NULL, IncorrectQuizFormProc);
				
			}
			//2.6.3 '아니오'버튼을 클릭했을 때
			else if (ret == IDNO) {
				//2.6.3.1 퀴즈 윈도우를 종료한다.
				EndDialog(hWnd, 0);
			}
		}
	}
	return TRUE;
}

BOOL QuizForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	int ret;

	//3. 닫기 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//3.1 메세지박스를 출력한다.
		ret = MessageBox(hWnd, "정답을 확인하시겠습니까?", "확인창", MB_YESNOCANCEL);
		//3.2 '예'버튼을 클릭했을 때
		if (ret == IDYES) {
			//3.2.1 오답 윈도우를 출력한다.
			DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_INCORRECTQUIZFORM),
				NULL, IncorrectQuizFormProc);
			//3.2.2 퀴즈 윈도우를 종료한다.
			EndDialog(hWnd, 0);
		}
		//3.3 '아니오'버튼을 클릭했을 때
		else if (ret == IDNO) {
			//3.3.1 퀴즈 윈도우를 종료한다.
			EndDialog(hWnd, 0);
		}
		RemoveProp(hWnd, "nextQuiz");
	}
	return TRUE;
}

