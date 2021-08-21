//ImportantWordsForm.c
#include"ImportantWordsForm.h"
#include"Vocabulary.h"
#include"resource.h"
#include<stdio.h>
#include<stdlib.h>
#pragma warning(disable:4996)

BOOL CALLBACK ImportantWordsFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	int ret;
	switch (message) {
	case WM_INITDIALOG:
		ret = ImportantWordsForm_OnInitDialog(hWnd, wParam, lParam);
		break;

	case WM_COMMAND:
		ret = ImportantWordsForm_OnCommand(hWnd, wParam, lParam);
		break;

	case WM_CLOSE:
		ret = ImportantWordsForm_OnClose(hWnd, wParam, lParam);
		break;

	default:
		ret = FALSE;
		break;
	}
	return ret;
}

BOOL ImportantWordsForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	int ret;
	switch (LOWORD(wParam)) {
	case IDC_BUTTON_FIRST:
		ret = ImportantWordsForm_OnFirstButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_PREVIOUS:
		ret = ImportantWordsForm_OnPreviousButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_NEXT:
		ret = ImportantWordsForm_OnNextButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_LAST:
		ret = ImportantWordsForm_OnLastButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_PUTOUTIMPORTANTWORD:
		ret = ImportantWordsForm_OnPutOutImportantWordButtonClicked(hWnd, wParam, lParam);
		break;

	default:
		ret = FALSE;
		break;
	}
	return ret;
}

BOOL ImportantWordsForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Vocabulary* vocabulary;
	HWND vocabularyForm;
	WordCard* (*indexes);
	Long count;
	Long current;

	//1. 윈도우가 생성될 때
	  //1.1 단어장 윈도우를 찾는다.
	  vocabularyForm = FindWindow("#32770", "단어장");
	  //1.2 단어장 윈도우의 단어장에서 중요단어를 찾는다.
	  vocabulary = (Vocabulary*)GetWindowLong(vocabularyForm, GWL_USERDATA);
	  FindImportantWords(vocabulary, &indexes, &count);
	  SetWindowLong(hWnd, GWL_USERDATA, (LONG)indexes);
	  SetProp(hWnd, "count", (HANDLE)count);
	  current = 0;
	  SetProp(hWnd, "current", (HANDLE)current);
	  //1.3 중요단어가 있을 경우, 중요단어의 첫번째 단어를 출력한다.
	  if (indexes[current] != NULL) {
		  SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0,
			  (LPARAM)indexes[current]->word.spelling);
		  SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0,
			  (LPARAM)indexes[current]->word.wordClass);
		  SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0,
			  (LPARAM)indexes[current]->word.meaning);
		  SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0,
			  (LPARAM)indexes[current]->word.example);
		  SendMessage(GetDlgItem(hWnd, IDC_EDIT_ISIMPORTANT), WM_SETTEXT, 0,
			  (LPARAM)indexes[current]->quiz.isImportant);
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

BOOL ImportantWordsForm_OnFirstButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordCard* (*indexes);
	Long current;

	//2. 처음 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//2.1 중요 단어의 첫번째 단어를 출력한다.
		indexes = (WordCard * (*))GetWindowLong(hWnd, GWL_USERDATA);
		current = 0;
		SetProp(hWnd, "current", (HANDLE)current);

		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->word.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->word.wordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->word.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->word.example);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ISIMPORTANT), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->quiz.isImportant);
	}
	return TRUE;
}

BOOL ImportantWordsForm_OnPreviousButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordCard* (*indexes);
	Long current;

	//3. 이전 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//3.1 중요단어의 이전 단어를 출력한다.
		indexes = (WordCard * (*))GetWindowLong(hWnd, GWL_USERDATA);
		current = (Long)GetProp(hWnd, "current");
		current--;
		if (current < 0) {
			current = 0;
		}
		SetProp(hWnd, "current", current);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->word.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->word.wordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->word.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->word.example);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ISIMPORTANT), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->quiz.isImportant);
	}
	return TRUE;
}

BOOL ImportantWordsForm_OnNextButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordCard* (*indexes);
	Long current;
	Long count;

	//4. 다음 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//4.1 중요단어의 다음 단어를 출력한다.
		indexes = (WordCard * (*))GetWindowLong(hWnd, GWL_USERDATA);
		current = (Long)GetProp(hWnd, "current");
		count = (Long)GetProp(hWnd, "count");
		current++;
		if (current >= count) {
			current = count - 1;
		}
		SetProp(hWnd, "current", current);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->word.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->word.wordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->word.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->word.example);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ISIMPORTANT), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->quiz.isImportant);
	}
	return TRUE;
}

BOOL ImportantWordsForm_OnLastButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordCard* (*indexes);
	Long current;
	Long count;

	//5. 마지막 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//5.1 중요단어의 마지막 단어를 출력한다.
		indexes = (WordCard * (*))GetWindowLong(hWnd, GWL_USERDATA);
		count = (Long)GetProp(hWnd, "count");
		current = count - 1;
		SetProp(hWnd, "current", (HANDLE)current);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->word.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->word.wordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->word.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->word.example);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ISIMPORTANT), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->quiz.isImportant);
	}
	return TRUE;
}

BOOL ImportantWordsForm_OnPutOutImportantWordButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Vocabulary* vocabulary;
	HWND vocabularyForm;
	WordCard* (*indexes);
	Long count;
	Long current;
	WordCard* index;

	//6. 중요단어 취소 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//6.1 해당 단어카드의 위치를 읽는다.
		indexes = (WordCard * (*))GetWindowLong(hWnd, GWL_USERDATA);
		current = (Long)GetProp(hWnd, "current");
		index = indexes[current];
		//6.2 단어장 윈도우를 찾는다.
		vocabularyForm = FindWindow("#32770", "단어장");
		//6.3 단어장 윈도우의 단어장에서 중요단어 취소한다.
		vocabulary = (Vocabulary*)GetWindowLong(vocabularyForm, GWL_USERDATA);
		index = PutOutImportantWord(vocabulary, index);
		//6.4 단어장 윈도우의 단어장에서 중요단어를 찾는다.
		if (indexes != NULL) {
			free(indexes);
			indexes = NULL;
		}
		FindImportantWords(vocabulary, &indexes, &count);
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)indexes);
		SetProp(hWnd, "count", count);
		//6.5 중요단어가 있을 경우, 취소한 단어의 위치에 따라, 단어를 선택하여 출력한다.
		if (indexes[0] != NULL) {
			if (current < count) {
				SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0,
					(LPARAM)indexes[current]->word.spelling);
				SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0,
					(LPARAM)indexes[current]->word.wordClass);
				SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0,
					(LPARAM)indexes[current]->word.meaning);
				SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0,
					(LPARAM)indexes[current]->word.example);
				SendMessage(GetDlgItem(hWnd, IDC_EDIT_ISIMPORTANT), WM_SETTEXT, 0,
					(LPARAM)indexes[current]->quiz.isImportant);
			}
			else if (current >= count) {
				current = count - 1;
				SetProp(hWnd, "current", current);
				SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0,
					(LPARAM)indexes[current]->word.spelling);
				SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0,
					(LPARAM)indexes[current]->word.wordClass);
				SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0,
					(LPARAM)indexes[current]->word.meaning);
				SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0,
					(LPARAM)indexes[current]->word.example);
				SendMessage(GetDlgItem(hWnd, IDC_EDIT_ISIMPORTANT), WM_SETTEXT, 0,
					(LPARAM)indexes[current]->quiz.isImportant);
			}
		}
		else {
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0, "");
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0, "");
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0, "");
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0, "");
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_ISIMPORTANT), WM_SETTEXT, 0, "");
		}
	}
	return TRUE;
}


BOOL ImportantWordsForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordCard* (*indexes);

	//7. 닫기 버튼을 클릭했을 때
	  //7.1 중요단어 윈도우를 종료한다.
	  indexes = (WordCard * (*))GetWindowLong(hWnd, GWL_USERDATA);
	  if (indexes != NULL) {
		  free(indexes);
		  indexes = NULL;
	  }
	  RemoveProp(hWnd, "current");
	  RemoveProp(hWnd, "count");
	  EndDialog(hWnd, 0);

	  return TRUE;
}






