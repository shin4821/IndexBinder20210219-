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

	//1. �����찡 ������ ��
	  //1.1 �ܾ��� �����츦 ã�´�.
	  vocabularyForm = FindWindow("#32770", "�ܾ���");
	  //1.2 �ܾ��� �������� �ܾ��忡�� ������ ã�´�.
	  vocabulary = (Vocabulary*)GetWindowLong(vocabularyForm, GWL_USERDATA);
	  correctQuiz = (CorrectQuiz*)GetProp(vocabularyForm, "correctQuiz");
	  nextQuiz = Vocabulary_First(vocabulary);

	  //1.3 ������ ������, ���� ö�ڸ� ����Ѵ�.
	  if (nextQuiz != NULL) {
		  SetProp(hWnd, "nextQuiz", nextQuiz);
		  SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0,
			  (LPARAM)nextQuiz->quiz.spelling);
	  }
	  //1.4 ������ ������
	  else {
		  //1.4.1 �޼����� ����Ѵ�.
		  ret = MessageBox(hWnd, "Ʋ�� ������ �����ϴ�. ������ ���� �Ͻðڽ��ϱ�?", "Ȯ��â", MB_YESNOCANCEL);
		  //1.4.2 '��'��ư�� Ŭ������ ��
		  if (ret == IDYES) {
			  //1.4.2.1 �ܾ��� �������� �ܾ��忡�� �����Ѵ�.
			  CorrectQuiz_Reset(correctQuiz);
			  //1.4.2.2 �ܾ����� ó������ ������ �ݺ��Ѵ�. 
			  index = CorrectQuiz_First(correctQuiz);
			  while (correctQuiz->length > 0) {
				  //1.4.2.2.1 ���������쿡�� �̴´�.
				  wordCard = CorrectQuiz_PutOut(correctQuiz, index);
				  //1.4.2.2.2 �ܾ��� �����쿡 �ȴ´�.
				  Vocabulary_Put(vocabulary, wordCard);
				  index = correctQuiz->current;
			  }
			  //1.4.2.2 �ܾ��� �������� �ܾ��忡�� ������ ã�´�.
			  nextQuiz = Vocabulary_First(vocabulary);
			  SetProp(hWnd, "nextQuiz", nextQuiz);
			  //1.4.2.3 ���� ö�ڸ� ����Ѵ�.
			  SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0,
				  (LPARAM)nextQuiz->quiz.spelling);
	      }
		  //3.3 '�ƴϿ�'��ư�� Ŭ������ ��
		  else if (ret == IDNO) {
			  //3.3.1 ���� �����츦 �����Ѵ�.
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

	//2. ���� ��ư�� Ŭ������ ��
	if (HIWORD(wParam) == BN_CLICKED) {
		//2.1 �ܾ��� �����츦 ã�´�.
		vocabularyForm = FindWindow("#32770", "�ܾ���");

		//2.2 �ܾ��� �������� �ܾ��忡�� ������ ��ġ�� ã�´�.
		indexBinder = (IndexBinder*)GetProp(vocabularyForm, "indexBinder");
		vocabulary = (Vocabulary*)GetWindowLong(vocabularyForm, GWL_USERDATA);
		correctQuiz = (CorrectQuiz*)GetProp(vocabularyForm, "correctQuiz");
		index=(WordCard*)GetProp(hWnd, "nextQuiz");

		//2.3 �ǹ̸� �д´�.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_GETTEXT, (WPARAM)32, (LPARAM)meaning);


		//2.3.1 ������ Ǭ��.
		Vocabulary_SpellingQuiz(vocabulary, meaning, index);


		//2.4.0 ���� ������ �̸� ���س��´�.
		nextQuiz = NextQuiz(vocabulary);
		SetProp(hWnd, "nextQuiz", nextQuiz);

		//2.4.1 ��� �¾��� ���, correctQuiz�� �ű��.

		if (strcmp(index->quiz.ox ,"O")==0) {
			
			//�ܾ��忡�� �̴´�.
			wordCard = Vocabulary_PutOut(vocabulary, index);
			//���ο��� �̴´�.
			if (wordCard.word.spelling[0] > 96 && wordCard.word.spelling[0] < 123) {
				IndexBinder_PutOut(indexBinder, wordCard.word.spelling[0] - 32, index);
			}
			else {
				IndexBinder_PutOut(indexBinder, wordCard.word.spelling[0], index);
			}

			//2.4.1.1 �ܾ��� �����쿡 ���� ��ġ�� �ܾ ����Ѵ�.
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

			//�ϱ�ܾ �ȴ´�.
			index_=CorrectQuiz_Put(correctQuiz, wordCard);
			correctQuizCount = correctQuiz->length;
			SetProp(vocabularyForm, "correctQuizCount", correctQuizCount);

			IndexBinder_Put(indexBinder, index_);
		}

	
		//2.5 ���� ������ ������
		if (nextQuiz != NULL) {
			
			//2.5.1 ���� ö�ڸ� ����Ѵ�.
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0, (LPARAM)nextQuiz->quiz.spelling);
			//2.5.2 �ǹ̸� ���� ó���Ѵ�.
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0, (LPARAM)"");
		}
		//2.6 ���� ������ ������
		else {
			//2.6.1 �޼��� �ڽ��� ����Ѵ�.
			ret = MessageBox(hWnd, "������ �ܾ��Դϴ�. ����� Ȯ���Ͻðڽ��ϱ�?", "Ȯ��â", MB_YESNOCANCEL);
			//2.6.2. '��'��ư�� Ŭ������ ��
			if (ret == IDYES) {
				//2.6.2.1 ���� �����츦 �����Ѵ�.
				EndDialog(hWnd, 0);
				//2.6.2.2 ���� �����츦 ����Ѵ�.
				DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), 
					MAKEINTRESOURCE(IDD_INCORRECTQUIZFORM), NULL, IncorrectQuizFormProc);
				
			}
			//2.6.3 '�ƴϿ�'��ư�� Ŭ������ ��
			else if (ret == IDNO) {
				//2.6.3.1 ���� �����츦 �����Ѵ�.
				EndDialog(hWnd, 0);
			}
		}
	}
	return TRUE;
}

BOOL QuizForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	int ret;

	//3. �ݱ� ��ư�� Ŭ������ ��
	if (HIWORD(wParam) == BN_CLICKED) {
		//3.1 �޼����ڽ��� ����Ѵ�.
		ret = MessageBox(hWnd, "������ Ȯ���Ͻðڽ��ϱ�?", "Ȯ��â", MB_YESNOCANCEL);
		//3.2 '��'��ư�� Ŭ������ ��
		if (ret == IDYES) {
			//3.2.1 ���� �����츦 ����Ѵ�.
			DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_INCORRECTQUIZFORM),
				NULL, IncorrectQuizFormProc);
			//3.2.2 ���� �����츦 �����Ѵ�.
			EndDialog(hWnd, 0);
		}
		//3.3 '�ƴϿ�'��ư�� Ŭ������ ��
		else if (ret == IDNO) {
			//3.3.1 ���� �����츦 �����Ѵ�.
			EndDialog(hWnd, 0);
		}
		RemoveProp(hWnd, "nextQuiz");
	}
	return TRUE;
}

