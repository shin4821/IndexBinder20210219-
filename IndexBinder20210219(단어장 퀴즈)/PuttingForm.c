//PuttingForm.c
#include"PuttingForm.h"
#include"Vocabulary.h"
#include"IndexBinder.h"
#include"Index.h"
#include"resource.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<Commctrl.h> // TVM의 헤더파일 추가
#pragma warning(disable:4996)

BOOL CALLBACK PuttingFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	BOOL ret;
	switch (message) {
	case WM_INITDIALOG:
		ret = PuttingForm_OnInitDialog(hWnd, wParam, lParam);
		break;
		
	case WM_COMMAND:
		ret = PuttingForm_OnCommand(hWnd, wParam, lParam);
		break;

	case WM_CLOSE:
		ret = PuttingForm_OnClose(hWnd, wParam, lParam);
		break;

	default:
		ret = FALSE;
		break;
	}
	return ret;
}

BOOL PuttingForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;

	switch (LOWORD(wParam)) {
	case IDC_EDIT_SPELLING:
		ret = PuttingForm_OnSpellingKillFocus(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_PUT:
		ret = PuttingForm_OnPutButtonClicked(hWnd, wParam, lParam);
		break;

	default:
		ret = FALSE;
		break;
	}
	return ret;
}

BOOL PuttingForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	TCHAR wordClasses[][8] = { "명사", "대명사", "동사", "형용사", "부사", "전치사","접속사","감탄사" };
	Long i;

	//1. 꽂기 윈도우가 생성될 때
	  //1.1 콤보박스 목록에 품사를 추가한다.
	for (i = 0; i < sizeof(wordClasses) / sizeof(wordClasses[0]); i++) {
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), CB_ADDSTRING, 0, (LPARAM)wordClasses[i]);
	}
	return TRUE;
}

BOOL PuttingForm_OnSpellingKillFocus(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	IndexBinder* indexBinder;
	HWND vocabularyForm;
	WordCard* index;
	TCHAR spelling[32];
	Long i = 0;
	Index* index_;

	//2. 철자에서 포커스를 잃을때
	if (HIWORD(wParam) == EN_KILLFOCUS) {
		//2.0 철자를 읽는다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_GETTEXT, (WPARAM)32, (LPARAM)spelling);
		//2.1 색인철에서 해당 철자의 첫번째 스펠링으로 철자를 찾는다.
		vocabularyForm = FindWindow("#32770", "단어장");
		indexBinder = (IndexBinder*)GetProp(vocabularyForm, "indexBinder");

		if (spelling[0] > 96 && spelling[0] < 123) {
			index_ = IndexBinder_Find(indexBinder, spelling[0] - 32);
		}
		else {
			index_ = IndexBinder_Find(indexBinder, spelling[0]);
		}
		
		//2.2 색인이 있는경우,
		if (index_ != NULL) {
			//2.2.1 index가 NULL이 아니고 key의 철자와 같지 않은 동안 반복한다.
			index = Index_GetAt(index_, i);
			while (index != NULL && strcmp(spelling, index->word.spelling) != 0) {
				i++;
				index = Index_GetAt(index_, i);
			}
			//2.2.2 해당 색인의 length보다 적은경우,꽂기 윈도우에 꽂는다.
			if (index != NULL) {
				SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0, (LPARAM)index->word.wordClass);
				SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0, (LPARAM)index->word.meaning);
				SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0, (LPARAM)index->word.example);
			}
		}
	}
	return TRUE;
}

BOOL PuttingForm_OnPutButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Vocabulary* vocabulary;
	IndexBinder* indexBinder;
	HWND vocabularyForm;
	TCHAR spelling[32];
	TCHAR wordClass[8];
	TCHAR meaning[32];
	TCHAR example[64];
	WordCard wordCard;
	WordCard* index;
	WordCard* first;
	Index* index_;
	HTREEITEM parent;
	HTREEITEM alphabet;
	HTREEITEM word;
	HTREEITEM wordClassHandle;
	TCHAR textAlphabet[2];
	TCHAR alphabets[2];
	TVITEM item = { 0, };
	TVINSERTSTRUCT insertStruct = { 0, };
	Long(*indexes) = NULL;
	Long count;
	TCHAR textWord[32];
	Long i = 0;
	Long j = 0;


	//3. 꽂기 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//3.1 단어카드를 읽는다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_GETTEXT, (WPARAM)32, (LPARAM)spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_GETTEXT, (WPARAM)8, (LPARAM)wordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_GETTEXT, (WPARAM)32, (LPARAM)meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_GETTEXT, (WPARAM)32, (LPARAM)example);

		//3.2 단어장 윈도우를 찾는다.
		vocabularyForm = FindWindow("#32770", "단어장");

		//3.3 단어장 윈도우의 단어장에 해당 단어카드를 꽂는다.
		strcpy(wordCard.word.spelling, spelling);
		strcpy(wordCard.word.wordClass, wordClass);
		strcpy(wordCard.word.meaning, meaning);
		strcpy(wordCard.word.example, example);
		strcpy(wordCard.quiz.spelling, spelling);
		strcpy(wordCard.quiz.answer, "");
		strcpy(wordCard.quiz.ox, "");
		strcpy(wordCard.quiz.meaning, meaning);
		strcpy(wordCard.quiz.isImportant, "");



		vocabulary = (Vocabulary*)GetWindowLong(vocabularyForm, GWL_USERDATA);
		index = Vocabulary_Put(vocabulary, wordCard);

		//3.4 단어장 윈도우의 색인철에 해당 단어카드를 꽂는다.
		indexBinder = (IndexBinder*)GetProp(vocabularyForm, "indexBinder");
		IndexBinder_Put(indexBinder, index);

		//3.5 트리뷰 상 현재 단어의 alphabet핸들을 찾는다.
		parent = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT,
			(LPARAM)NULL);
		alphabet= SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
			(LPARAM)parent);

		item.mask = TVIF_HANDLE | TVIF_TEXT;
		item.hItem = alphabet;
		textAlphabet[1] = '\0';
		item.pszText = textAlphabet;
		item.cchTextMax = 2;
		SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);
		
		if (wordCard.word.spelling[0] > 96 && wordCard.word.spelling[0] < 123) {
			alphabets[0] = index->word.spelling[0]-32;
			alphabets[1] = '\0';
		}
		else {
			alphabets[0] = index->word.spelling[0];
			alphabets[1] = '\0';
		}

		while (alphabet != NULL && strcmp(textAlphabet, alphabets) != 0) {
			alphabet = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
				(WPARAM)TVGN_NEXT, (LPARAM)alphabet);
			item.hItem = alphabet;
			item.pszText = textAlphabet;
			item.cchTextMax = 2;
			SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);
		}

		// 알파벳 핸들을 펼쳐준다.
		SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)parent);

		//3.6 alphabet 핸들이 있는경우,
		if (alphabet != NULL) {
			//alphabet 핸들을 펼친다.
			SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, 
				(LPARAM)alphabet);

			//3.6.1 색인에 동일단어가 있는지 확인한다.
			if (index->word.spelling[0] > 96 && index->word.spelling[0] < 123) {
				index_ = IndexBinder_Find(indexBinder, index->word.spelling[0] - 32);
			}
			else {
				index_ = IndexBinder_Find(indexBinder, index->word.spelling[0]);
			}
			Index_FindSpellings(index_, index, &indexes, &count);

			//3.6.2. 동일단어가 있으면(count == 2)
			if (count > 1 && count < 3) {
				//3.6.2.1. 단어핸들 찾는다.
				word = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM, 
					(WPARAM)TVGN_CHILD, (LPARAM)alphabet);
				item.hItem = word;
				item.pszText = textWord;
				item.cchTextMax = 32;
				SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

				while (strcmp(textWord, index->word.spelling) != 0) {
					word = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
						(WPARAM)TVGN_NEXT, (LPARAM)word);
					item.hItem = word;
					item.pszText = textWord;
					item.cchTextMax = 32;
					SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);
				}
				//3.6.2.2. 단어핸들 하위에 이미 꽂혀있던 첫번째 단어의 품사 추가한다.
				indexes[0] = i;
				first = Index_GetAt(index_, i);

				insertStruct.hParent = word;
				insertStruct.hInsertAfter = TVI_LAST;
				insertStruct.item.mask = TVIF_TEXT;
				insertStruct.item.pszText = first->word.wordClass;
				SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
					(LPARAM)&insertStruct);

				//3.6.2.3. 두번째 단어의 품사 역시 단어핸들 하위에 추가한다.
				insertStruct.hParent = word;
				insertStruct.hInsertAfter = TVI_LAST;
				insertStruct.item.mask = TVIF_TEXT;
				insertStruct.item.pszText = index->word.wordClass;
				wordClassHandle = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
					(LPARAM)&insertStruct);
				
				//3.6.2.4.  단어핸들을 펼쳐준다.
				SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, 
					(LPARAM)word);

				//3.6.2.5. 해당 품사를 선택된 상태로 처리한다.
				SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET,
					(LPARAM)wordClass);
			}
			//3.6.2. 동일단어가 있으면(count > 2)
			else if (count > 2) {
				//3.6.2.1.  단어핸들 찾는다.
				word = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM, 
					(WPARAM)TVGN_CHILD, (LPARAM)alphabet);
				item.hItem = word;
				item.pszText = textWord;
				item.cchTextMax = 32;
				SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

				while (strcmp(textWord, index->word.spelling) != 0) {
					word = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM, 
						(WPARAM)TVGN_NEXT, (LPARAM)word);
					item.hItem = word;
					item.pszText = textWord;
					item.cchTextMax = 32;
					SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);
				}

				//3.6.2.2. 단어핸들 하위에 품사핸들 추가한다.
				insertStruct.hParent = word;
				insertStruct.hInsertAfter = TVI_LAST;
				insertStruct.item.mask = TVIF_TEXT;
				insertStruct.item.pszText = index->word.wordClass;
				wordClassHandle = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
					(LPARAM)&insertStruct);

				//3.6.2.3.  단어핸들을 펼쳐준다.
				SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
					(LPARAM)word);

				//3.6.2.4. 해당 품사를 선택된 상태로 처리한다.
				SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET,
					(LPARAM)wordClass);
			}
			//3.6.3 동일단어가 없으면 
			else if (count <= 1) {
				//3.6.3.1. alphabet핸들 하위에 단어핸들 추가한다.
				insertStruct.hParent = alphabet;
				insertStruct.hInsertAfter = TVI_LAST;
				insertStruct.item.mask = TVIF_TEXT;
				insertStruct.item.pszText = index->word.spelling;
				word = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
					(LPARAM)&insertStruct);
				//3.6.3.2. 해당 단어를 선택된 상태로 처리한다.
				SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET,
					(LPARAM)word);
			}
		}
		//3.7 alphabet 핸들이 없는경우, 
		else {
			//3.7.1. 알파벳 핸들 하위에 alphabet핸들 추가한다.
			insertStruct.hParent = parent;
			insertStruct.hInsertAfter = TVI_SORT;
			insertStruct.item.mask = TVIF_TEXT;
			
			if (index->word.spelling[0] > 96 && index->word.spelling[0] < 123) {
				alphabets[0] = index->word.spelling[0]-32;
				alphabets[1] = '\0';
			}
			else {
				alphabets[0] = index->word.spelling[0];
				alphabets[1] = '\0';
			}
			insertStruct.item.pszText = alphabets;
			alphabet = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
				(LPARAM)&insertStruct);

			//3.7.2. alphabet핸들 하위에 단어 핸들을 추가한다.
			insertStruct.hParent = alphabet;
			insertStruct.hInsertAfter = TVI_LAST;
			insertStruct.item.mask = TVIF_TEXT;
			insertStruct.item.pszText = index->word.spelling;
			word = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
				(LPARAM)&insertStruct);

			//3.7.3. alphabet 핸들을 펼친다.
			SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
				(LPARAM)alphabet);

			//3.7.4. 해당 단어핸들을 선택된 상태로 처리한다.
			SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET,
				(LPARAM)word);
		}
		//3.8 꽂은 단어카드를 단어장 윈도우에 출력한다.
		SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_SPELLING), WM_SETTEXT, 0, (LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0, (LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_MEANING), WM_SETTEXT, 0, (LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0, (LPARAM)index->word.example);
		SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_ISIMPORTANT), WM_SETTEXT, 0, 
			(LPARAM)index->quiz.isImportant);

		//3.9. indexes 할당해제한다.
		if (indexes != NULL) {
			free(indexes);
			indexes = NULL;
		}
		//4.0. 꽂기 윈도우를 종료한다.
		EndDialog(hWnd, 0);
	}
	return TRUE;
}

BOOL PuttingForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	//4. 닫기 버튼을 클릭했을 때
	  //4.1 꽂기 윈도우를 종료한다.
	EndDialog(hWnd, 0);

	return TRUE;
}

