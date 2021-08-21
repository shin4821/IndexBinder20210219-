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


	//1. 윈도우가 생성될 때
	 //1.1 단어장 윈도우를 찾는다.
	 vocabularyForm = FindWindow("#32770", "단어장");

	 //1.2. 단어장윈도우의 단어장에서 현재 단어카드를 찾는다.
	 vocabulary = (Vocabulary*)GetWindowLong(vocabularyForm, GWL_USERDATA);
	 index = vocabulary->current;
	 current = vocabulary->current;

	 //1.3 현재 단어카드의 색인을 찾는다.
	 indexBinder = (IndexBinder*)GetProp(vocabularyForm, "indexBinder");
	 index_ = IndexBinder_Find(indexBinder, index->word.spelling[0]);

	 //1.4 해당 색인에서 현재 단어와 동일 단어가 있는지 확인한다.
	 Index_FindSpellings(index_, index, &indexes, &count);

	 //1.5. 동일 단어가 있을 경우, 몇번째 품사인지 확인한다.
	 if (count > 1) {
		 i = Index_Find(index_, index);
		 while (indexes[j] != i) {
			 j++; // j가 단어핸들 기준 몇번째 품사인지 알려준다.
		 }
	 }

	 // indexes 할당 해제한다.
	 if (indexes != NULL) {
		 free(indexes);
		 indexes = NULL;
	 }

	 //1.6. 단어장윈도우의 단어장에서 현재 단어카드를 뽑는다.
	 vocabulary = (Vocabulary*)GetWindowLong(vocabularyForm, GWL_USERDATA);
	 wordCard = Vocabulary_PutOut(vocabulary, index);

	 //1.7 뽑은 단어를 뽑기 윈도우에 출력한다.
	 SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0, (LPARAM)wordCard.word.spelling);
	 SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0, (LPARAM)wordCard.word.wordClass);
	 SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0, (LPARAM)wordCard.word.meaning);
	 SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0, (LPARAM)wordCard.word.example);

	 //1.8 트리뷰상 alphabet핸들을 찾는다.
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

	 //1.9 트리뷰 상 단어핸들을 찾는다.
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

	 //2.0. 동일 단어가 있었는 경우,
	 if (count > 1) {
		 //2.0.1. 동일 단어가 3개 이상인 경우,
		 if (count > 2) {
			 //2.0.1.1. 단어핸들 하위 품사핸들을 찾는다.
			 wordClass = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
				 (WPARAM)TVGN_CHILD, (LPARAM)word);
			 i = 0;
			 while (i < j) {
				 wordClass = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
					 (WPARAM)TVGN_NEXT, (LPARAM)wordClass);
				 i++;
			 }
			 //2.0.2 해당 품사핸들을 없앤다.
			 SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_DELETEITEM, 0, (LPARAM)wordClass);
		 }
		 //2.0.2. 동일 단어가 2개인 경우,
		 else if (count < 3) {
			 //2.0.2.1. 품사핸들 2개 모두 없앤다.
			 i = 0;
			 while (i < count) {
				 wordClass = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
					 (WPARAM)TVGN_CHILD, (LPARAM)word);
				 SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_DELETEITEM, 0, (LPARAM)wordClass);
				 i++;
			 }
		 }
	 }
	 //2.1. 동일단어가 없었는 경우, 단어핸들을 없앤다.
	 else if(count<=1) {
		 SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_DELETEITEM, 0, (LPARAM)word);
	 }

	 //2.2. 단어장윈도우의 색인철에서 해당 단어를 뽑는다.
	 if (wordCard.word.spelling[0] > 96 && wordCard.word.spelling[0] < 123) {
		 index_ = IndexBinder_PutOut(indexBinder, wordCard.word.spelling[0]-32, current);
	 }
	 else {
		 index_ = IndexBinder_PutOut(indexBinder, wordCard.word.spelling[0], current);
	 }

	 //2.3. 색인철에 해당 alphabet색인이 없는 경우, 트리뷰 상 alphabet핸들 없앤다.
	 if (index_ == NULL) {
		 SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_DELETEITEM, 0, (LPARAM)alphabet);
	 }





	 //2.4. 단어장 윈도우의 현재 단어를 찾는다.
	 index = vocabulary->current;

	 //2.5 단어가 있는 경우,
	 if (index != NULL) {

		 //2.5.1. 트리뷰에서 alphabet핸들을 찾는다.
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
		 //2.5.2. 단어핸들 찾는다. 
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
		 //2.5.3. 알파벳 핸들 펼친다.
		 SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
			 (LPARAM)parent);

		 //2.5.4. alphabet핸들 펼친다.
		 SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
			 (LPARAM)alphabet);

		 //2.5.5. 색인철에서 해당 색인을 찾는다.
		 if (index->word.spelling[0] > 96 && index->word.spelling[0] < 123) {
			 index_ = IndexBinder_Find(indexBinder, index->word.spelling[0] - 32);
		 }
		 else {
			 index_ = IndexBinder_Find(indexBinder, index->word.spelling[0]);
		 }
		 //2.5.6. 현재 단어가 해당 색인 내 동일단어가 있는지 확인한다.
		 Index_FindSpellings(index_, index, &indexes, &count);

		 //2.5.7. 동일 단어가 있는 경우,
		 if (count > 1) {
			 //2.5.7.1. 해당 단어의 품사가 몇 번째 있는지 확인한다.
			 i = Index_Find(index_, index);
			 j = 0;
			 while (indexes[j] != i) {
				 j++;
			 }
			 //2.5.7.2. 단어핸들 하위 품사핸들 찾는다.
			 wordClass = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
				 (WPARAM)TVGN_CHILD, (LPARAM)word);
			 i = 0;
			 while (i < j) {
				 wordClass = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
					 (WPARAM)TVGN_NEXT, (LPARAM)wordClass);
				 i++;
			 }
			 //2.5.7.3. 단어핸들 펼친다.
			 SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
				 (LPARAM)word);
			 //2.5.7.4. 품사핸들을 선택된 상태로 처리한다.
			 SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET,
				 (LPARAM)wordClass);
			
		 }
		 //2.5.8. 동일단어가 없는 경우,
		 else if(count <= 1) {
			 //2.5.8.1. 단어핸들을 선택된 상태로 처리한다.
			 SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET,
				 (LPARAM)word);
		 }

		 //2.5.9. 현재 단어를 단어에 출력한다.
		 SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_SPELLING), WM_SETTEXT, 0,
			 (LPARAM)index->word.spelling);
		 SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0,
			 (LPARAM)index->word.wordClass);
		 SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_MEANING), WM_SETTEXT, 0,
			 (LPARAM)index->word.meaning);
		 SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0,
			 (LPARAM)index->word.example);
	 }
	 //2.6 현재 단어카드가 없는 경우,
	 else {
		 //2.1.1 단어장 윈도우를 공백 처리한다.
		 SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_SPELLING), WM_SETTEXT, 0, (LPARAM)"");
		 SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0, (LPARAM)"");
		 SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_MEANING), WM_SETTEXT, 0, (LPARAM)"");
		 SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0, (LPARAM)"");
	 } 
	 //2.7. indexes를 할당해제한다.
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


	//2. 닫기 버튼을 클릭했을 때
	  //2.1 메세지 박스를 출력한다.
	  ret = MessageBox(hWnd, "꽂으시겠습니까?", "확인창", MB_YESNOCANCEL);
	  //2.2 '예'버튼을 클릭했을 때
	  if (ret == IDYES) {
		  //2.2.1 단어카드를 읽는다.
		  SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_GETTEXT, (WPARAM)32, (LPARAM)spelling);
		  SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_GETTEXT, (WPARAM)8, (LPARAM)wordClass);
		  SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_GETTEXT, (WPARAM)32, (LPARAM)meaning);
		  SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_GETTEXT, (WPARAM)64, (LPARAM)example);
		  
		  //2.2.2 단어장 윈도우를 찾는다.
		  vocabularyForm = FindWindow("#32770", "단어장");
		  
		  //2.2.3 단어장 윈도우의 단어장에 꽂는다.
		  strcpy(wordCard.word.spelling, spelling);
		  strcpy(wordCard.word.wordClass, wordClass);
		  strcpy(wordCard.word.meaning, meaning);
		  strcpy(wordCard.word.example, example);

		  vocabulary = (Vocabulary*)GetWindowLong(vocabularyForm, GWL_USERDATA);
		  index = Vocabulary_Put(vocabulary, wordCard);

		  //2.2.4 단어장 윈도우의 색인철에 꽂는다.
		  indexBinder = (IndexBinder*)GetProp(vocabularyForm, "indexBinder");
		  index_ = IndexBinder_Put(indexBinder, index);

		  //2.2.5 트리뷰에서 alphabet핸들을 찾는다.
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
		  //2.2.6 알파벳핸들 펼친다.
		  SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
			  (LPARAM)parent);

		  //2.2.7. alphabet핸들 있을 경우,
		  if (alphabet != NULL) {
			  //2.2.7.1. alphabet핸들 펼친다.
			  SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
				  (LPARAM)alphabet);

			  //2.2.7.2. 동일단어 있는지 확인한다.
			  if (index->word.spelling[0] > 96 && index->word.spelling[0] < 123) {
				  index_ = IndexBinder_Find(indexBinder, index->word.spelling[0] - 32);
			  }
			  else {
				  index_ = IndexBinder_Find(indexBinder, index->word.spelling[0]);
			  }
			  Index_FindSpellings(index_, index, &indexes, &count);

			  //2.2.7.3. 동일단어가 있으면(count=2)
			  if (count > 1 && count < 3) {
				  //2.2.7.3.1. 단어핸들 찾는다.
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
				  //2.2.7.3.2. 단어핸들 하위에 이미 꽂혀있던 첫번째 단어 품사를 추가한다.
				  indexes[0] = i;
				  first = Index_GetAt(index_, i);

				  insertStruct.hParent = word;
				  insertStruct.hInsertAfter = TVI_LAST;
				  insertStruct.item.mask = TVIF_TEXT;
				  insertStruct.item.pszText = first->word.wordClass;
				  SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
					  (LPARAM)&insertStruct);

				  //2.2.7.3.3. 두번째 단어의 품사 역시 하위에 추가한다.
				  insertStruct.hParent = word;
				  insertStruct.hInsertAfter = TVI_LAST;
				  insertStruct.item.mask = TVIF_TEXT;
				  insertStruct.item.pszText = index->word.wordClass;
				  wordClassHandle = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
					  (LPARAM)&insertStruct);

				  //2.2.7.3.4. 단어핸들 펼친다.
				  SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
					  (LPARAM)word);

				  //2.2.7.3.5. 해당 품사를 선택된 상태로 처리한다.
				  SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET,
					  (LPARAM)wordClassHandle);
			  }
			  //2.2.7.4. 동일단어가 있으면(count>2)
			  if (count > 2) {
				  //2.2.7.4.1. 단어핸들 찾는다.
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

				  //2.2.7.4.2. 단어핸들 하위에 품사 추가한다.
				  insertStruct.hParent = word;
				  insertStruct.hInsertAfter = TVI_LAST;
				  insertStruct.item.mask = TVIF_TEXT;
				  insertStruct.item.pszText = index->word.wordClass;
				  wordClassHandle = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
					  (LPARAM)&insertStruct);

				  //2.2.7.4.3. 단어핸들 펼친다.
				  SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
					  (LPARAM)word);

				  //2.2.7.4.4. 해당 품사를 선택된 상태로 처리한다.
				  SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET,
					  (LPARAM)wordClassHandle);
			  }
			  //2.2.7.5. 동일단어가 없으면(count<=1)
			  if (count <= 1) {
				  //2.2.7.5.1. alphabet핸들 하위에 단어핸들 추가한다.
				  insertStruct.hParent = alphabet;
				  insertStruct.hInsertAfter = TVI_LAST;
				  insertStruct.item.mask = TVIF_TEXT;
				  insertStruct.item.pszText = index->word.spelling;
				  word = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
					  (LPARAM)&insertStruct);

				  //2.2.7.5.2. 단어핸들을 선택된 상태로 처리한다.
				  SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET,
					  (LPARAM)word);
			  }
		  }
		  //2.2.8. alphabet핸들 없는 경우,
		  else {
		     //2.2.8.1. alphabet핸들 추가한다.
			 insertStruct.hParent = parent;
			 insertStruct.hInsertAfter = TVI_SORT;
			 insertStruct.item.mask = TVIF_TEXT;
			 insertStruct.item.pszText = alphabets;
			 alphabet = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
				 (LPARAM)&insertStruct);

			 //2.2.8.2. alphabet핸들 펼친다.
			 SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
				 (LPARAM)alphabet);

			 //2.2.8.3. alphabet핸들 하위에 단어핸들 추가한다.
			 insertStruct.hParent = alphabet;
			 insertStruct.hInsertAfter = TVI_LAST;
			 insertStruct.item.mask = TVIF_TEXT;
			 insertStruct.item.pszText = index->word.spelling;
			 word = SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
				 (LPARAM)&insertStruct);

			 //2.2.8.4. 단어핸들을 선택된 상태로 처리한다.
			 SendMessage(GetDlgItem(vocabularyForm, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET,
				 (LPARAM)word);
          }
		  //2.2.9. 꽂은 단어카드를 단어장 윈도우에 출력한다.
		  SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_SPELLING), WM_SETTEXT, 0,
			  (LPARAM)index->word.spelling);
		  SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0,
			  (LPARAM)index->word.wordClass);
		  SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_MEANING), WM_SETTEXT, 0,
			  (LPARAM)index->word.meaning);
		  SendMessage(GetDlgItem(vocabularyForm, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0,
			  (LPARAM)index->word.example);

		  //2.3.0. indexes 할당해제한다.
		  if (indexes != NULL) {
			  free(indexes);
			  indexes = NULL;
		  }

		  //2.3.1 뽑기 윈도우를 종료한다.
		  EndDialog(hWnd, 0);

      }

	  //2.3 '아니오' 버튼을 클릭했을 때
	  else if (ret == IDNO) {

	  //2.3.1 뽑기 윈도우를 종료한다.
	  EndDialog(hWnd, 0);
	  }

	  return TRUE;
}


