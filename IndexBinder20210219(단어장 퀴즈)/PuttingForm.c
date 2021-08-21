//PuttingForm.c
#include"PuttingForm.h"
#include"Vocabulary.h"
#include"IndexBinder.h"
#include"Index.h"
#include"resource.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<Commctrl.h> // TVM�� ������� �߰�
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
	TCHAR wordClasses[][8] = { "���", "����", "����", "�����", "�λ�", "��ġ��","���ӻ�","��ź��" };
	Long i;

	//1. �ȱ� �����찡 ������ ��
	  //1.1 �޺��ڽ� ��Ͽ� ǰ�縦 �߰��Ѵ�.
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

	//2. ö�ڿ��� ��Ŀ���� ������
	if (HIWORD(wParam) == EN_KILLFOCUS) {
		//2.0 ö�ڸ� �д´�.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_GETTEXT, (WPARAM)32, (LPARAM)spelling);
		//2.1 ����ö���� �ش� ö���� ù��° ���縵���� ö�ڸ� ã�´�.
		vocabularyForm = FindWindow("#32770", "�ܾ���");
		indexBinder = (IndexBinder*)GetProp(vocabularyForm, "indexBinder");

		if (spelling[0] > 96 && spelling[0] < 123) {
			index_ = IndexBinder_Find(indexBinder, spelling[0] - 32);
		}
		else {
			index_ = IndexBinder_Find(indexBinder, spelling[0]);
		}
		
		//2.2 ������ �ִ°��,
		if (index_ != NULL) {
			//2.2.1 index�� NULL�� �ƴϰ� key�� ö�ڿ� ���� ���� ���� �ݺ��Ѵ�.
			index = Index_GetAt(index_, i);
			while (index != NULL && strcmp(spelling, index->word.spelling) != 0) {
				i++;
				index = Index_GetAt(index_, i);
			}
			//2.2.2 �ش� ������ length���� �������,�ȱ� �����쿡 �ȴ´�.
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


	//3. �ȱ� ��ư�� Ŭ������ ��
	if (HIWORD(wParam) == BN_CLICKED) {
		//3.1 �ܾ�ī�带 �д´�.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_GETTEXT, (WPARAM)32, (LPARAM)spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_GETTEXT, (WPARAM)8, (LPARAM)wordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_GETTEXT, (WPARAM)32, (LPARAM)meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_GETTEXT, (WPARAM)32, (LPARAM)example);

		//3.2 �ܾ��� �����츦 ã�´�.
		vocabularyForm = FindWindow("#32770", "�ܾ���");

		//3.3 �ܾ��� �������� �ܾ��忡 �ش� �ܾ�ī�带 �ȴ´�.
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

		//3.4 �ܾ��� �������� ����ö�� �ش� �ܾ�ī�带 �ȴ´�.
		indexBinder = (IndexBinder*)GetProp(vocabularyForm, "indexBinder");
		IndexBinder_Put(indexBinder, index);

		//3.5 Ʈ���� �� ���� �ܾ��� alphabet�ڵ��� ã�´�.
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

		// ���ĺ� �ڵ��� �����ش�.
		SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)parent);

		//3.6 alphabet �ڵ��� �ִ°��,
		if (alphabet != NULL) {
			//alphabet �ڵ��� ��ģ��.
			SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, 
				(LPARAM)alphabet);

			//3.6.1 ���ο� ���ϴܾ �ִ��� Ȯ���Ѵ�.
			if (index->word.spelling[0] > 96 && index->word.spelling[0] < 123) {
				index_ = IndexBinder_Find(indexBinder, index->word.spelling[0] - 32);
			}
			else {
				index_ = IndexBinder_Find(indexBinder, index->word.spelling[0]);
			}
			Index_FindSpellings(index_, index, &indexes, &count);

			//3.6.2. ���ϴܾ ������(count == 2)
			if (count > 1 && count < 3) {
				//3.6.2.1. �ܾ��ڵ� ã�´�.
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
				//3.6.2.2. �ܾ��ڵ� ������ �̹� �����ִ� ù��° �ܾ��� ǰ�� �߰��Ѵ�.
				indexes[0] = i;
				first = Index_GetAt(index_, i);

				insertStruct.hParent = word;
				insertStruct.hInsertAfter = TVI_LAST;
				insertStruct.item.mask = TVIF_TEXT;
				insertStruct.item.pszText = first->word.wordClass;
				SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
					(LPARAM)&insertStruct);

				//3.6.2.3. �ι�° �ܾ��� ǰ�� ���� �ܾ��ڵ� ������ �߰��Ѵ�.
				insertStruct.hParent = word;
				insertStruct.hInsertAfter = TVI_LAST;
				insertStruct.item.mask = TVIF_TEXT;
				insertStruct.item.pszText = index->word.wordClass;
				wordClassHandle = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
					(LPARAM)&insertStruct);
				
				//3.6.2.4.  �ܾ��ڵ��� �����ش�.
				SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, 
					(LPARAM)word);

				//3.6.2.5. �ش� ǰ�縦 ���õ� ���·� ó���Ѵ�.
				SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET,
					(LPARAM)wordClass);
			}
			//3.6.2. ���ϴܾ ������(count > 2)
			else if (count > 2) {
				//3.6.2.1.  �ܾ��ڵ� ã�´�.
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

				//3.6.2.2. �ܾ��ڵ� ������ ǰ���ڵ� �߰��Ѵ�.
				insertStruct.hParent = word;
				insertStruct.hInsertAfter = TVI_LAST;
				insertStruct.item.mask = TVIF_TEXT;
				insertStruct.item.pszText = index->word.wordClass;
				wordClassHandle = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
					(LPARAM)&insertStruct);

				//3.6.2.3.  �ܾ��ڵ��� �����ش�.
				SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
					(LPARAM)word);

				//3.6.2.4. �ش� ǰ�縦 ���õ� ���·� ó���Ѵ�.
				SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET,
					(LPARAM)wordClass);
			}
			//3.6.3 ���ϴܾ ������ 
			else if (count <= 1) {
				//3.6.3.1. alphabet�ڵ� ������ �ܾ��ڵ� �߰��Ѵ�.
				insertStruct.hParent = alphabet;
				insertStruct.hInsertAfter = TVI_LAST;
				insertStruct.item.mask = TVIF_TEXT;
				insertStruct.item.pszText = index->word.spelling;
				word = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
					(LPARAM)&insertStruct);
				//3.6.3.2. �ش� �ܾ ���õ� ���·� ó���Ѵ�.
				SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET,
					(LPARAM)word);
			}
		}
		//3.7 alphabet �ڵ��� ���°��, 
		else {
			//3.7.1. ���ĺ� �ڵ� ������ alphabet�ڵ� �߰��Ѵ�.
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

			//3.7.2. alphabet�ڵ� ������ �ܾ� �ڵ��� �߰��Ѵ�.
			insertStruct.hParent = alphabet;
			insertStruct.hInsertAfter = TVI_LAST;
			insertStruct.item.mask = TVIF_TEXT;
			insertStruct.item.pszText = index->word.spelling;
			word = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
				(LPARAM)&insertStruct);

			//3.7.3. alphabet �ڵ��� ��ģ��.
			SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
				(LPARAM)alphabet);

			//3.7.4. �ش� �ܾ��ڵ��� ���õ� ���·� ó���Ѵ�.
			SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET,
				(LPARAM)word);
		}
		//3.8 ���� �ܾ�ī�带 �ܾ��� �����쿡 ����Ѵ�.
		SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_SPELLING), WM_SETTEXT, 0, (LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0, (LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_MEANING), WM_SETTEXT, 0, (LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0, (LPARAM)index->word.example);
		SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_ISIMPORTANT), WM_SETTEXT, 0, 
			(LPARAM)index->quiz.isImportant);

		//3.9. indexes �Ҵ������Ѵ�.
		if (indexes != NULL) {
			free(indexes);
			indexes = NULL;
		}
		//4.0. �ȱ� �����츦 �����Ѵ�.
		EndDialog(hWnd, 0);
	}
	return TRUE;
}

BOOL PuttingForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	//4. �ݱ� ��ư�� Ŭ������ ��
	  //4.1 �ȱ� �����츦 �����Ѵ�.
	EndDialog(hWnd, 0);

	return TRUE;
}

