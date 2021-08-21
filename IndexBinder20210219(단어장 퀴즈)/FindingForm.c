//FindingForm.c
#include"FindingForm.h"
#include"vocabulary.h"
#include"IndexBinder.h"
#include"Index.h"
#include<Commctrl.h> 
#include"resource.h"
#include<stdio.h>
#include<stdlib.h>
#pragma warning(disable:4996)

BOOL CALLBACK FindingFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	BOOL ret;
	switch (message) {
	case WM_INITDIALOG:
		ret = FindingForm_OnInitDialog(hWnd, wParam, lParam);
		break;

	case WM_COMMAND:
		ret = FindingForm_OnCommand(hWnd, wParam, lParam);
		break;

	case WM_CLOSE:
		ret = FindingForm_OnClose(hWnd, wParam, lParam);
		break;

	default:
		ret = FALSE;
		break;
	}
	return ret;
}

BOOL FindingForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;
	switch (LOWORD(wParam)) {
	case IDC_BUTTON_SPELLING:
		ret = FindingForm_OnSpellingButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_MEANING:
		ret = FindingForm_OnMeaningButtonClicked(hWnd, wParam, lParam);
		break;
		
	case IDC_BUTTON_FIND:
		ret = FindingForm_OnFindButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_FIRST:
		ret = FindingForm_OnFirstButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_PREVIOUS:
		ret = FindingForm_OnPreviousButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_NEXT:
		ret = FindingForm_OnNextButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_LAST:
		ret = FindingForm_OnLastButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_SELECT:
		ret = FindingForm_OnSelectButtonClicked(hWnd, wParam, lParam);
		break;

	default:
		ret = FALSE;
		break;
	}
	return ret;
}

BOOL FindingForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	//0. 윈도우가 생성될 때
	  //0.1 철자 라디오 박스에 체크한다.
	SendMessage(GetDlgItem(hWnd, IDC_BUTTON_SPELLING), BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
	return TRUE;
}

BOOL FindingForm_OnSpellingButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	//1. 철자 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//1.1 의미를 지운다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_CONDITIONMEANING), WM_SETTEXT, 0, (LPARAM)"");
	}
	return TRUE;
}

BOOL FindingForm_OnMeaningButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	//2. 의미 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//2.1 철자를 지운다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_CONDITIONSPELLING), WM_SETTEXT, 0, (LPARAM)"");
	}
	return TRUE;
}

BOOL FindingForm_OnFindButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Vocabulary* vocabulary;
	HWND vocabularyForm;
	TCHAR spelling[32];
	TCHAR meaning[32];
	WordCard* (*indexes);
	Long count;
	Long current;
	int retSpelling;
	int retMeaning;

	//3. 찾기 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//3.1 철자 버튼을 읽는다.
		retSpelling = SendMessage(GetDlgItem(hWnd, IDC_BUTTON_SPELLING), BM_GETCHECK, 0, 0);
		//3.2 철자를 읽는다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_CONDITIONSPELLING), WM_GETTEXT, (WPARAM)32, (LPARAM)spelling);
		//3.3 의미 버튼을 읽는다.
		retMeaning = SendMessage(GetDlgItem(hWnd, IDC_BUTTON_MEANING), BM_GETCHECK, 0, 0);
		//3.4 의미를 읽는다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_CONDITIONMEANING), WM_GETTEXT, (WPARAM)32, (LPARAM)meaning);
		//3.5 단어장 윈도우를 찾는다.
		vocabularyForm = FindWindow("#32770", "단어장");
		//3.6 단어장 윈도우의 단어장에서 조건에 따라 찾는다.
		vocabulary = (Vocabulary*)GetWindowLong(vocabularyForm, GWL_USERDATA);
		indexes = (WordCard * (*))GetWindowLong(hWnd, GWL_USERDATA);
		if (indexes != NULL) {
			free(indexes);
			indexes = NULL;
		}
		if (retSpelling == BST_CHECKED) {
			FindBySpelling(vocabulary, spelling, &indexes, &count);
		}
		else if (retMeaning == BST_CHECKED) {
			FindByMeaning(vocabulary, meaning, &indexes, &count);
		}
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)indexes);
		SetProp(hWnd, "count", (HANDLE)count);
		current = 0;
		SetProp(hWnd, "currnet", (HANDLE)current);

		//3.7 단어 카드가 있는 경우, 찾은 단어카드 중 첫번째 카드를 출력한다.
		indexes = (WordCard * (*))GetWindowLong(hWnd, GWL_USERDATA);
		if (indexes[current] != NULL) {
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0, 
				(LPARAM)indexes[current]->word.spelling);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0,
				(LPARAM)indexes[current]->word.wordClass);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0,
				(LPARAM)indexes[current]->word.meaning);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0,
				(LPARAM)indexes[current]->word.example);
		}
		//3.8 단어카드가 없는 경우, 단어카드를 공백 처리한다.
		else {
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0, (LPARAM)"");
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0, (LPARAM)"");
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0, (LPARAM)"");
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0, (LPARAM)"");
		}
	}
	return TRUE;
}

BOOL FindingForm_OnFirstButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordCard* (*indexes);
	Long current;

	//4. 처음 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//4.1 찾은 단어카드 중, 첫번째 단어카드를 출력한다.
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
	}
	return TRUE;
}

BOOL FindingForm_OnPreviousButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordCard* (*indexes);
	Long current;

	//5. 이전 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//5.1 찾은 단어카드 중, 이전 단어카드를 출력한다.
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
	}
	return TRUE;
}

BOOL FindingForm_OnNextButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordCard* (*indexes);
	Long count;
	Long current;

	//6. 다음 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//6.1 찾은 단어카드 중, 다음 단어카드를 출력한다.
		indexes = (WordCard * (*))GetWindowLong(hWnd, GWL_USERDATA);
		count = (Long)GetProp(hWnd, "count");
		current = (Long)GetProp(hWnd, "current");
		current++;
		if (current >= count) {
			current = count - 1;
		}
		SetProp(hWnd, "current", (HANDLE)current);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->word.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->word.wordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->word.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->word.example);
	}
	return TRUE;
}

BOOL FindingForm_OnLastButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordCard* (*indexes);
	Long count;
	Long current;

	//7. 마지막 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//7.1 찾은 단어카드 중, 마지막 단어카드를 출력한다.
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
	}
	return TRUE;
}

BOOL FindingForm_OnSelectButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	IndexBinder* indexBinder;
	Vocabulary* vocabulary;
	HWND vocabularyForm;
	WordCard* index;
	WordCard* (*indexes);
	Long current;
	HTREEITEM parent;
	HTREEITEM alphabet;
	HTREEITEM word;
	HTREEITEM wordClass;
	Long i;
	Long j = 0;
	TVITEM item = { 0, };
	TCHAR textAlphabet[2];
	TCHAR alphabets[2];
	Index* index_;
	Long(*spellingIndexes);
	Long count;
	TCHAR textWord[32];

	//8. 선택 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//8.1 현재 단어카드의 위치를 읽는다.
		indexes = (WordCard * (*))GetWindowLong(hWnd, GWL_USERDATA);
		current = (Long)GetProp(hWnd, "current");
		index = indexes[current];

		//8.2 단어장 윈도우를 찾는다.
		vocabularyForm = FindWindow("#32770", "단어장");
		vocabulary = (Vocabulary*)GetWindowLong(vocabularyForm, GWL_USERDATA);

		//8.3 단어장 윈도우의 단어장에서 현재 단어카드를 읽은 위치로 옮긴다.
		index = Move(vocabulary, index);

		//8.4 트리뷰에서 해당 alphabet 찾는다.
		parent = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT,
			(LPARAM)NULL);
		alphabet = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM, 
			(WPARAM)TVGN_CHILD, (LPARAM)parent);
		item.mask = TVIF_TEXT | TVIF_HANDLE;
		textAlphabet[1] = '\0';
		item.pszText = textAlphabet;
		item.cchTextMax = 2;
		item.hItem = alphabet;
		SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

		
		if (index->word.spelling[0] > 96 && index->word.spelling[0] < 123) {
			alphabets[0] = index->word.spelling[0]-32;
			alphabets[1] = '\0';
		}
		else {
			alphabets[0] = index->word.spelling[0];
			alphabets[1] = '\0';
		}

		while (strcmp(textAlphabet, alphabets) != 0) {
			alphabet = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
				(WPARAM)TVGN_NEXT, (LPARAM)alphabet);
			item.pszText = textAlphabet;
			item.cchTextMax = 2;
			item.hItem = alphabet;
			SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);
		}

		//8.5 트리뷰에서 해당 단어핸들 찾는다.
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

		//8.6 트리뷰 상 알파벳 핸들을 펼친다.
		SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
			(LPARAM)parent);

		//8.7 트리뷰 상 alphabet핸들을 펼친다.
		SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
			(LPARAM)alphabet);

		//8.8. 해당 단어가 색인에서 동일 단어가 있는지 확인한다.
		indexBinder = (IndexBinder*)GetProp(vocabularyForm, "indexBinder");

		if (index->word.spelling[0] > 96 && index->word.spelling[0] < 123) {
			index_ = IndexBinder_Find(indexBinder, index->word.spelling[0] - 32);
		}
		else {
			index_ = IndexBinder_Find(indexBinder, index->word.spelling[0]);
		}
		Index_FindSpellings(index_, index, &spellingIndexes, &count);

		//8.9. 동일단어가 있는 경우,
		if (count > 1) {
			//8.9.1. 품사의 위치 확인한다.
			i = Index_Find(index_, index);
			while (spellingIndexes[j] != i) {
				j++;
			}
			//8.9.2. 품사핸들 찾는다.
			wordClass = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
				(WPARAM)TVGN_CHILD, (LPARAM)word);
			i = 0;
			while (i < j) {
				wordClass = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
					(WPARAM)TVGN_NEXT, (LPARAM)wordClass);
				i++;
			}
			//8.9.3. 단어핸들 펼친다.
			SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
				(LPARAM)word);

			//8.9.4. 해당 품사핸들을 선택된 상태로 처리한다.
			SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET,
				(LPARAM)wordClass);
		}
		//9.0 동일단어가 없는 경우,
		if (count <= 1) {
			//9.0.1. 해당 단어핸들을 선택된 상태로 처리한다.
			SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET,
				(LPARAM)word);
		}

		//9.1. 현재 단어카드를 단어장 모니터에 출력한다.
		SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_SPELLING), WM_SETTEXT, 0,
			(LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0,
			(LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_MEANING), WM_SETTEXT, 0,
			(LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0,
			(LPARAM)index->word.example);

		//9.2. spellingIndexes 할당해제한다.
		if (spellingIndexes != NULL) {
			free(spellingIndexes);
			spellingIndexes = NULL;
		}


		//9.0 찾기 윈도우를 종료한다.
		if (indexes != NULL) {
			free(indexes);
			indexes = NULL;
		}
		RemoveProp(hWnd, "current");
		RemoveProp(hWnd, "count");
		EndDialog(hWnd, 0);
	}
	return TRUE;
}

BOOL FindingForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordCard* (*indexes);

	//9. 닫기 버튼을 클릭했을때
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
