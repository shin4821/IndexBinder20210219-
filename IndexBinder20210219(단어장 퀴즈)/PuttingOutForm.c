//PuttingOutForm.c
#include"PuttingOutForm.h"
#include"Vocabulary.h"
#include"IndexBinder.h"
#include"Index.h"
#include"resource.h"
#include<Commctrl.h> 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#pragma warning(disable:4996)

BOOL CALLBACK PuttingOutFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	BOOL ret;
	switch (message) {
	case WM_INITDIALOG:
		ret = PuttingOutForm_OnInitDialog(hWnd, wParam, lParam);
		break;

	case WM_CLOSE:
		ret = PuttingOutForm_OnClose(hWnd, wParam, lParam);
		break;

	default:
		ret = FALSE;
		break;
	}
	return ret;
}

BOOL PuttingOutForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Vocabulary* vocabulary;
	HWND vocabularyForm;
	WordCard* index;
	IndexBinder* indexBinder;
	WordCard wordCard;
	Index* index_;
	Long i;
	Long j = 0;
	WordCard* current;
	HTREEITEM parent;
	HTREEITEM alphabet;
	HTREEITEM word;
	HTREEITEM wordClass;
	TVITEM item = { 0, };
	TCHAR textAlphabet[2];
	TCHAR alphabets[2];
	Long(*indexes);
	Long count;
	TCHAR textWord[32];


	//1. �����찡 ������ ��
	 //1.1 �ܾ��� �����츦 ã�´�.
	 vocabularyForm = FindWindow("#32770", "�ܾ���");

	 //1.2. �ܾ����������� �ܾ��忡�� ���� �ܾ�ī�带 ã�´�.
	 vocabulary = (Vocabulary*)GetWindowLong(vocabularyForm, GWL_USERDATA);
	 index = vocabulary->current;
	 current = vocabulary->current;

	 //1.3 ���� �ܾ�ī���� ������ ã�´�.
	 indexBinder = (IndexBinder*)GetProp(vocabularyForm, "indexBinder");
	 index_ = IndexBinder_Find(indexBinder, index->word.spelling[0]);

	 //1.4 �ش� ���ο��� ���� �ܾ�� ���� �ܾ �ִ��� Ȯ���Ѵ�.
	 Index_FindSpellings(index_, index, &indexes, &count);

	 //1.5. ���� �ܾ ���� ���, ���° ǰ������ Ȯ���Ѵ�.
	 if (count > 1) {
		 i = Index_Find(index_, index);
		 while (indexes[j] != i) {
			 j++; // j�� �ܾ��ڵ� ���� ���° ǰ������ �˷��ش�.
		 }
	 }

	 // indexes �Ҵ� �����Ѵ�.
	 if (indexes != NULL) {
		 free(indexes);
		 indexes = NULL;
	 }

	 //1.6. �ܾ����������� �ܾ��忡�� ���� �ܾ�ī�带 �̴´�.
	 vocabulary = (Vocabulary*)GetWindowLong(vocabularyForm, GWL_USERDATA);
	 wordCard = Vocabulary_PutOut(vocabulary, index);

	 //1.7 ���� �ܾ �̱� �����쿡 ����Ѵ�.
	 SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0, (LPARAM)wordCard.word.spelling);
	 SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0, (LPARAM)wordCard.word.wordClass);
	 SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0, (LPARAM)wordCard.word.meaning);
	 SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0, (LPARAM)wordCard.word.example);

	 //1.8 Ʈ����� alphabet�ڵ��� ã�´�.
	 parent = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT,
		 (LPARAM)NULL);
	 alphabet = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
		 (LPARAM)parent);

	 item.mask = TVIF_HANDLE | TVIF_TEXT;
	 item.hItem = alphabet;
	 textAlphabet[1] = '\0';
	 item.pszText = textAlphabet;
	 item.cchTextMax = 2;
	 SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

	 if (wordCard.word.spelling[0] > 96 && wordCard.word.spelling[0] < 123) {
		 alphabets[0] = wordCard.word.spelling[0] - 32;
		 alphabets[1] = '\0';
	 }
	 else {
		 alphabets[0] = wordCard.word.spelling[0];
		 alphabets[1] = '\0';
	 }

	 while (strcmp(textAlphabet, alphabets) != 0) {
		 alphabet = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
			 (WPARAM)TVGN_NEXT, (LPARAM)alphabet);
		 item.hItem = alphabet;
		 textAlphabet[1] = '\0';
		 item.pszText = textAlphabet;
		 item.cchTextMax = 2;
		 SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);
	 }

	 //1.9 Ʈ���� �� �ܾ��ڵ��� ã�´�.
	 word = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
		 (WPARAM)TVGN_CHILD, (LPARAM)alphabet);
	 item.hItem = word;
	 item.pszText = textWord;
	 item.cchTextMax = 32;
	 SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

	 while (strcmp(textWord, wordCard.word.spelling) != 0) {
		 word = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
			 (WPARAM)TVGN_NEXT, (LPARAM)word);
		 item.hItem = word;
		 item.pszText = textWord;
		 item.cchTextMax = 32;
		 SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);
	 }

	 //2.0. ���� �ܾ �־��� ���,
	 if (count > 1) {
		 //2.0.1. ���� �ܾ 3�� �̻��� ���,
		 if (count > 2) {
			 //2.0.1.1. �ܾ��ڵ� ���� ǰ���ڵ��� ã�´�.
			 wordClass = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
				 (WPARAM)TVGN_CHILD, (LPARAM)word);
			 i = 0;
			 while (i < j) {
				 wordClass = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
					 (WPARAM)TVGN_NEXT, (LPARAM)wordClass);
				 i++;
			 }
			 //2.0.2 �ش� ǰ���ڵ��� ���ش�.
			 SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_DELETEITEM, 0, (LPARAM)wordClass);
		 }
		 //2.0.2. ���� �ܾ 2���� ���,
		 else if (count < 3) {
			 //2.0.2.1. ǰ���ڵ� 2�� ��� ���ش�.
			 i = 0;
			 while (i < count) {
				 wordClass = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
					 (WPARAM)TVGN_CHILD, (LPARAM)word);
				 SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_DELETEITEM, 0, (LPARAM)wordClass);
				 i++;
			 }
		 }
	 }
	 //2.1. ���ϴܾ ������ ���, �ܾ��ڵ��� ���ش�.
	 else if(count<=1) {
		 SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_DELETEITEM, 0, (LPARAM)word);
	 }

	 //2.2. �ܾ����������� ����ö���� �ش� �ܾ �̴´�.
	 if (wordCard.word.spelling[0] > 96 && wordCard.word.spelling[0] < 123) {
		 index_ = IndexBinder_PutOut(indexBinder, wordCard.word.spelling[0]-32, current);
	 }
	 else {
		 index_ = IndexBinder_PutOut(indexBinder, wordCard.word.spelling[0], current);
	 }

	 //2.3. ����ö�� �ش� alphabet������ ���� ���, Ʈ���� �� alphabet�ڵ� ���ش�.
	 if (index_ == NULL) {
		 SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_DELETEITEM, 0, (LPARAM)alphabet);
	 }





	 //2.4. �ܾ��� �������� ���� �ܾ ã�´�.
	 index = vocabulary->current;

	 //2.5 �ܾ �ִ� ���,
	 if (index != NULL) {

		 //2.5.1. Ʈ���信�� alphabet�ڵ��� ã�´�.
		 parent = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
			 (WPARAM)TVGN_ROOT, (LPARAM)NULL);
		 alphabet = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
			 (WPARAM)TVGN_CHILD, (LPARAM)parent);

		 item.mask = TVIF_HANDLE | TVIF_TEXT;
		 item.hItem = alphabet;
		 textAlphabet[1] = '\0';
		 item.pszText = textAlphabet;
		 item.cchTextMax = 2;
		 SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

		 if (index->word.spelling[0] > 96 && index->word.spelling[0] < 123) {
			 alphabets[0] = index->word.spelling[0] - 32;
			 alphabets[1] = '\0';
		 }
		 else {
			 alphabets[0] = index->word.spelling[0];
			 alphabets[1] = '\0';
		 }

		 while (strcmp(textAlphabet, alphabets) != 0) {
			 alphabet = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
				 (WPARAM)TVGN_NEXT, (LPARAM)alphabet);
			 item.hItem = alphabet;
			 textAlphabet[1] = '\0';
			 item.pszText = textAlphabet;
			 item.cchTextMax = 2;
			 SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);
		 }
		 //2.5.2. �ܾ��ڵ� ã�´�. 
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
		 //2.5.3. ���ĺ� �ڵ� ��ģ��.
		 SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
			 (LPARAM)parent);

		 //2.5.4. alphabet�ڵ� ��ģ��.
		 SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
			 (LPARAM)alphabet);

		 //2.5.5. ����ö���� �ش� ������ ã�´�.
		 if (index->word.spelling[0] > 96 && index->word.spelling[0] < 123) {
			 index_ = IndexBinder_Find(indexBinder, index->word.spelling[0] - 32);
		 }
		 else {
			 index_ = IndexBinder_Find(indexBinder, index->word.spelling[0]);
		 }
		 //2.5.6. ���� �ܾ �ش� ���� �� ���ϴܾ �ִ��� Ȯ���Ѵ�.
		 Index_FindSpellings(index_, index, &indexes, &count);

		 //2.5.7. ���� �ܾ �ִ� ���,
		 if (count > 1) {
			 //2.5.7.1. �ش� �ܾ��� ǰ�簡 �� ��° �ִ��� Ȯ���Ѵ�.
			 i = Index_Find(index_, index);
			 j = 0;
			 while (indexes[j] != i) {
				 j++;
			 }
			 //2.5.7.2. �ܾ��ڵ� ���� ǰ���ڵ� ã�´�.
			 wordClass = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
				 (WPARAM)TVGN_CHILD, (LPARAM)word);
			 i = 0;
			 while (i < j) {
				 wordClass = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
					 (WPARAM)TVGN_NEXT, (LPARAM)wordClass);
				 i++;
			 }
			 //2.5.7.3. �ܾ��ڵ� ��ģ��.
			 SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
				 (LPARAM)word);
			 //2.5.7.4. ǰ���ڵ��� ���õ� ���·� ó���Ѵ�.
			 SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET,
				 (LPARAM)wordClass);
			
		 }
		 //2.5.8. ���ϴܾ ���� ���,
		 else if(count <= 1) {
			 //2.5.8.1. �ܾ��ڵ��� ���õ� ���·� ó���Ѵ�.
			 SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET,
				 (LPARAM)word);
		 }

		 //2.5.9. ���� �ܾ �ܾ ����Ѵ�.
		 SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_SPELLING), WM_SETTEXT, 0,
			 (LPARAM)index->word.spelling);
		 SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0,
			 (LPARAM)index->word.wordClass);
		 SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_MEANING), WM_SETTEXT, 0,
			 (LPARAM)index->word.meaning);
		 SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0,
			 (LPARAM)index->word.example);
	 }
	 //2.6 ���� �ܾ�ī�尡 ���� ���,
	 else {
		 //2.1.1 �ܾ��� �����츦 ���� ó���Ѵ�.
		 SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_SPELLING), WM_SETTEXT, 0, (LPARAM)"");
		 SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0, (LPARAM)"");
		 SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_MEANING), WM_SETTEXT, 0, (LPARAM)"");
		 SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0, (LPARAM)"");
	 } 
	 //2.7. indexes�� �Ҵ������Ѵ�.
	 if (indexes != NULL) {
		 free(indexes);
		 indexes = NULL;
	 }
	 return TRUE;
}



BOOL PuttingOutForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Vocabulary* vocabulary;
	HWND vocabularyForm;
	WordCard* index;
	IndexBinder* indexBinder;
	Index* index_;
	WordCard wordCard;
	int ret;
	TCHAR spelling[32];
	TCHAR wordClass[8];
	TCHAR meaning[32];
	TCHAR example[64];
	HTREEITEM parent;
	HTREEITEM alphabet;
	HTREEITEM word;
	HTREEITEM wordClassHandle;
	TVITEM item = { 0, };
	TVINSERTSTRUCT insertStruct = { 0, };
	Long i = 0;
	Long j = 0;
	TCHAR textAlphabet[2];
	TCHAR alphabets[2];
	Long(*indexes) = NULL;
	Long count;
	TCHAR textWord[32];
	WordCard* first;


	//2. �ݱ� ��ư�� Ŭ������ ��
	  //2.1 �޼��� �ڽ��� ����Ѵ�.
	  ret = MessageBox(hWnd, "�����ðڽ��ϱ�?", "Ȯ��â", MB_YESNOCANCEL);
	  //2.2 '��'��ư�� Ŭ������ ��
	  if (ret == IDYES) {
		  //2.2.1 �ܾ�ī�带 �д´�.
		  SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_GETTEXT, (WPARAM)32, (LPARAM)spelling);
		  SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_GETTEXT, (WPARAM)8, (LPARAM)wordClass);
		  SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_GETTEXT, (WPARAM)32, (LPARAM)meaning);
		  SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_GETTEXT, (WPARAM)64, (LPARAM)example);
		  
		  //2.2.2 �ܾ��� �����츦 ã�´�.
		  vocabularyForm = FindWindow("#32770", "�ܾ���");
		  
		  //2.2.3 �ܾ��� �������� �ܾ��忡 �ȴ´�.
		  strcpy(wordCard.word.spelling, spelling);
		  strcpy(wordCard.word.wordClass, wordClass);
		  strcpy(wordCard.word.meaning, meaning);
		  strcpy(wordCard.word.example, example);

		  vocabulary = (Vocabulary*)GetWindowLong(vocabularyForm, GWL_USERDATA);
		  index = Vocabulary_Put(vocabulary, wordCard);

		  //2.2.4 �ܾ��� �������� ����ö�� �ȴ´�.
		  indexBinder = (IndexBinder*)GetProp(vocabularyForm, "indexBinder");
		  index_ = IndexBinder_Put(indexBinder, index);

		  //2.2.5 Ʈ���信�� alphabet�ڵ��� ã�´�.
		  parent = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
			  (WPARAM)TVGN_ROOT, (LPARAM)NULL);
		  alphabet = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
			  (WPARAM)TVGN_CHILD, (LPARAM)parent);

		  item.mask = TVIF_HANDLE | TVIF_TEXT;
		  item.hItem = alphabet;
		  textAlphabet[1] = '\0';
		  item.pszText = textAlphabet;
		  item.cchTextMax = 2;
		  SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

		  if (wordCard.word.spelling[0] > 96 && wordCard.word.spelling[0] < 123) {
			  alphabets[0] = index->word.spelling[0] - 32;
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
		  //2.2.6 ���ĺ��ڵ� ��ģ��.
		  SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
			  (LPARAM)parent);

		  //2.2.7. alphabet�ڵ� ���� ���,
		  if (alphabet != NULL) {
			  //2.2.7.1. alphabet�ڵ� ��ģ��.
			  SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
				  (LPARAM)alphabet);

			  //2.2.7.2. ���ϴܾ� �ִ��� Ȯ���Ѵ�.
			  if (index->word.spelling[0] > 96 && index->word.spelling[0] < 123) {
				  index_ = IndexBinder_Find(indexBinder, index->word.spelling[0] - 32);
			  }
			  else {
				  index_ = IndexBinder_Find(indexBinder, index->word.spelling[0]);
			  }
			  Index_FindSpellings(index_, index, &indexes, &count);

			  //2.2.7.3. ���ϴܾ ������(count=2)
			  if (count > 1 && count < 3) {
				  //2.2.7.3.1. �ܾ��ڵ� ã�´�.
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
				  //2.2.7.3.2. �ܾ��ڵ� ������ �̹� �����ִ� ù��° �ܾ� ǰ�縦 �߰��Ѵ�.
				  indexes[0] = i;
				  first = Index_GetAt(index_, i);

				  insertStruct.hParent = word;
				  insertStruct.hInsertAfter = TVI_LAST;
				  insertStruct.item.mask = TVIF_TEXT;
				  insertStruct.item.pszText = first->word.wordClass;
				  SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
					  (LPARAM)&insertStruct);

				  //2.2.7.3.3. �ι�° �ܾ��� ǰ�� ���� ������ �߰��Ѵ�.
				  insertStruct.hParent = word;
				  insertStruct.hInsertAfter = TVI_LAST;
				  insertStruct.item.mask = TVIF_TEXT;
				  insertStruct.item.pszText = index->word.wordClass;
				  wordClassHandle = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
					  (LPARAM)&insertStruct);

				  //2.2.7.3.4. �ܾ��ڵ� ��ģ��.
				  SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
					  (LPARAM)word);

				  //2.2.7.3.5. �ش� ǰ�縦 ���õ� ���·� ó���Ѵ�.
				  SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET,
					  (LPARAM)wordClassHandle);
			  }
			  //2.2.7.4. ���ϴܾ ������(count>2)
			  if (count > 2) {
				  //2.2.7.4.1. �ܾ��ڵ� ã�´�.
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

				  //2.2.7.4.2. �ܾ��ڵ� ������ ǰ�� �߰��Ѵ�.
				  insertStruct.hParent = word;
				  insertStruct.hInsertAfter = TVI_LAST;
				  insertStruct.item.mask = TVIF_TEXT;
				  insertStruct.item.pszText = index->word.wordClass;
				  wordClassHandle = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
					  (LPARAM)&insertStruct);

				  //2.2.7.4.3. �ܾ��ڵ� ��ģ��.
				  SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
					  (LPARAM)word);

				  //2.2.7.4.4. �ش� ǰ�縦 ���õ� ���·� ó���Ѵ�.
				  SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET,
					  (LPARAM)wordClassHandle);
			  }
			  //2.2.7.5. ���ϴܾ ������(count<=1)
			  if (count <= 1) {
				  //2.2.7.5.1. alphabet�ڵ� ������ �ܾ��ڵ� �߰��Ѵ�.
				  insertStruct.hParent = alphabet;
				  insertStruct.hInsertAfter = TVI_LAST;
				  insertStruct.item.mask = TVIF_TEXT;
				  insertStruct.item.pszText = index->word.spelling;
				  word = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
					  (LPARAM)&insertStruct);

				  //2.2.7.5.2. �ܾ��ڵ��� ���õ� ���·� ó���Ѵ�.
				  SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET,
					  (LPARAM)word);
			  }
		  }
		  //2.2.8. alphabet�ڵ� ���� ���,
		  else {
		     //2.2.8.1. alphabet�ڵ� �߰��Ѵ�.
			 insertStruct.hParent = parent;
			 insertStruct.hInsertAfter = TVI_SORT;
			 insertStruct.item.mask = TVIF_TEXT;
			 insertStruct.item.pszText = alphabets;
			 alphabet = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
				 (LPARAM)&insertStruct);

			 //2.2.8.2. alphabet�ڵ� ��ģ��.
			 SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
				 (LPARAM)alphabet);

			 //2.2.8.3. alphabet�ڵ� ������ �ܾ��ڵ� �߰��Ѵ�.
			 insertStruct.hParent = alphabet;
			 insertStruct.hInsertAfter = TVI_LAST;
			 insertStruct.item.mask = TVIF_TEXT;
			 insertStruct.item.pszText = index->word.spelling;
			 word = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
				 (LPARAM)&insertStruct);

			 //2.2.8.4. �ܾ��ڵ��� ���õ� ���·� ó���Ѵ�.
			 SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET,
				 (LPARAM)word);
          }
		  //2.2.9. ���� �ܾ�ī�带 �ܾ��� �����쿡 ����Ѵ�.
		  SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_SPELLING), WM_SETTEXT, 0,
			  (LPARAM)index->word.spelling);
		  SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0,
			  (LPARAM)index->word.wordClass);
		  SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_MEANING), WM_SETTEXT, 0,
			  (LPARAM)index->word.meaning);
		  SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0,
			  (LPARAM)index->word.example);

		  //2.3.0. indexes �Ҵ������Ѵ�.
		  if (indexes != NULL) {
			  free(indexes);
			  indexes = NULL;
		  }

		  //2.3.1 �̱� �����츦 �����Ѵ�.
		  EndDialog(hWnd, 0);

      }

	  //2.3 '�ƴϿ�' ��ư�� Ŭ������ ��
	  else if (ret == IDNO) {

	  //2.3.1 �̱� �����츦 �����Ѵ�.
	  EndDialog(hWnd, 0);
	  }

	  return TRUE;
}


