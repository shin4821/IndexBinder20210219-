//VocabularyForm.c
#include"VocabularyForm.h"
#include"Vocabulary.h"
#include"IndexBinder.h"
#include"Index.h"
#include"PuttingForm.h"
#include"FindingForm.h"
#include"PuttingOutForm.h"
#include"QuizForm.h"
#include"ImportantWordsForm.h"
#include"CorrectQuizForm.h"
#include"resource.h"
#include<stdio.h>
#include<stdlib.h>
#include<Commctrl.h> // TVM의 헤더파일 추가
#pragma warning(disable:4996)

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	int response;
	response = DialogBox(hInstance, MAKEINTRESOURCE(IDD_VOCABULARYFORM), NULL, VocabularyFormProc);
	return response;
}

BOOL CALLBACK VocabularyFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	BOOL ret;
	switch (message) {
	case WM_INITDIALOG:
		ret = VocabularyForm_OnInitDialog(hWnd, wParam, lParam);
		break;

	case WM_COMMAND:
		ret = VocabularyForm_OnCommand(hWnd, wParam, lParam);
		break;

	case WM_NOTIFY:
		ret = VocabularyForm_OnNotify(hWnd, wParam, lParam);
		break;

	case WM_CLOSE:
		ret = VocabularyForm_OnClose(hWnd, wParam, lParam);
		break;

	default:
		ret = FALSE;
		break;
	}
	return ret;
}

BOOL VocabularyForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;
	switch (LOWORD(wParam)) {
	case IDC_BUTTON_PUT:
		ret = VocabularyForm_OnPutButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_FIND:
		ret = VocabularyForm_OnFindButtonClicked(hWnd, wParam, lParam);
		break;


	case IDC_BUTTON_ARRANGE:
		ret = VocabularyForm_OnArrangeButtonClicked(hWnd, wParam, lParam);
		break;


	case IDC_BUTTON_PUTOUT:
		ret = VocabularyForm_OnPutOutButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_FIRST:
		ret = VocabularyForm_OnFirstButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_PREVIOUS:
		ret = VocabularyForm_OnPreviousButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_NEXT:
		ret = VocabularyForm_OnNextButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_LAST:
		ret = VocabularyForm_OnLastButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_QUIZ:
		ret = VocabularyForm_OnQuizButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_PUTIMPORTANTWORD:
		ret = VocabularyForm_OnPutImportantWordButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_PUTOUTIMPORTANTWORD:
		ret = VocabularyForm_OnPutOutImportantWordButtonClicked(hWnd, wParam, lParam);
		break;


	case IDC_BUTTON_IMPORTANTWORDS:
		ret = VocabularyForm_OnImportantWordButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_CORRECTQUIZ:
		ret = VocabularyForm_OnCorrectQuizButtonClicked(hWnd, wParam, lParam);
		break;

	default:
		ret = FALSE;
		break;
	}
	return ret;
}

BOOL VocabularyForm_OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;
	switch (wParam) {
	case IDC_TREE_INDEXES:
		ret= VocabularyForm_OnTreeViewItemButtonClicked(hWnd, wParam, lParam);
		break;

	default:
		ret = FALSE;
		break;
	}
	return ret;
}

BOOL VocabularyForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Vocabulary* vocabulary;
	IndexBinder* indexBinder;
	CorrectQuiz* correctQuiz;
	WordCard* index;
	Index* index_ = NULL;
	WordCard* previous = NULL;
	HTREEITEM parent;
	HTREEITEM alphabet = NULL;
	HTREEITEM word = NULL;
	TVINSERTSTRUCT insertStruct = { 0, };
	TVITEM item = { 0, };
	Long i = 0;
	Long j = 0;
	Long count;
	Long vocabularyCount;
	Long spellingCount;
	TCHAR textAlphabet[2];
	Index(*indexes) = NULL;
	TCHAR alphabets[2];
	HTREEITEM wordClass;
	TCHAR textWord[32];
	HTREEITEM testWord;
	Long k = 0;
	Long l = 0;
	WordCard* current;
	Long(*spellingIndexes) = NULL;
	TCHAR testText[2];
	Long correctQuizCount;

	//1. 윈도우가 생성될때
	  //1.1 단어장을 만든다.(create)
	  vocabulary = (Vocabulary*)malloc(sizeof(Vocabulary));
	  Vocabulary_Create(vocabulary);
	  SetWindowLong(hWnd, GWL_USERDATA, (LONG)vocabulary);

	  // 암기단어를 만든다.
	  correctQuiz = (CorrectQuiz*)malloc(sizeof(CorrectQuiz));
	  CorrectQuiz_Create(correctQuiz);
	  SetProp(hWnd, "correctQuiz", correctQuiz);

	  //1.2 색인을 만든다.
	  indexBinder = (IndexBinder*)malloc(sizeof(IndexBinder));
	  IndexBinder_Create(indexBinder);
	  SetProp(hWnd, "indexBinder", indexBinder);

	  //1.3 트리뷰에 알파벳핸들을 만든다.
	  insertStruct.hParent = TVI_ROOT;
	  insertStruct.hInsertAfter = TVI_FIRST;
	  insertStruct.item.mask = TVIF_TEXT | TVIF_HANDLE;
	  insertStruct.item.pszText = "알파벳";
	  parent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_INSERTITEM, 0, (LPARAM)&insertStruct);

	  //1.4 적재한다.
	  vocabularyCount =Load(vocabulary);

	  //암기단어를 적재한다.
	  correctQuizCount = CorrectQuiz_Load(correctQuiz);
	  SetProp(hWnd, "correctQuizCount", correctQuizCount);


	  //1.5 적재한 단어카드 또는 암기단어가 1개 이상일 경우
	  if (vocabularyCount > 0 || correctQuizCount > 0) {

		  if (vocabularyCount > 0) {
			  //1.5.1 단어장의 처음부터 끝까지 반복한다.
			  index = Vocabulary_First(vocabulary);
			  while (index != previous) {
				  //1.5.1.1 색인철에 끼운다.
				  IndexBinder_Put(indexBinder, index);
				  previous = index;
				  index = Vocabulary_Next(vocabulary);
			  }
		  }
		  

		  if (correctQuizCount > 0) {
			  //1.5.1 암기단어의 처음부터 끝까지 반복한다.
			  previous = NULL;
			  index = CorrectQuiz_First(correctQuiz);
			  while (index != previous) {
				  //1.5.1.1 색인철에 끼운다.
				  IndexBinder_Put(indexBinder, index);
				  previous = index;
				  index = CorrectQuiz_Next(correctQuiz);
			  }
		  }
		  //1.5.2 색인리스트를 만든다.
		  MakeList(indexBinder, &indexes, &count);

		  //1.5.3 색인리스트의 개수(count)만큼 반복한다.
		  while (i < count) {

			  //1.5.3.1 알파벳 핸들 하위에 alphabet핸들을 추가한다.
			  insertStruct.hParent = parent;
			  insertStruct.hInsertAfter = TVI_LAST;

			  alphabets[0] = indexes[i].alphabet;
			  alphabets[1] = '\0';

			  insertStruct.item.pszText = alphabets;
			  alphabet = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
				  (LPARAM)&insertStruct);

			  //1.5.3.2 해당 alphabet색인의 length만큼 반복한다.
			  j = 0;
			  while (j < indexes[i].length) {

				  //1.5.3.2.1 해당 단어의 색인에 동일단어 유무 확인한다.
				  index = Index_GetAt(indexes + i, j);

				  if (index->word.spelling[0] > 96 && index->word.spelling[0] < 123) {
					  index_ = IndexBinder_Find(indexBinder, index->word.spelling[0] - 32);
				  }
				  else {
					  index_ = IndexBinder_Find(indexBinder, index->word.spelling[0]);
				  }

				  Index_FindSpellings(index_, index, &spellingIndexes, &spellingCount);

				  //1.5.3.2.2. 해당 색인에 동일 단어가 있으면, 
				  if (spellingCount > 1) {

					  //1. 단어핸들의 몇번째 위치의 품사인지 확인한다.
					  k = Index_Find(index_, index);

					  l = 0;
					  while (spellingIndexes[l] != k) {
						  l++;
					  }

					  //2. 첫번째 품사일 경우, 단어핸들 먼저 추가해준다.
					  if (l <= 0) {
						  insertStruct.hParent = alphabet;
						  insertStruct.hInsertAfter = TVI_LAST;
						  insertStruct.item.pszText = index->word.spelling;
						  word = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
							  (LPARAM)&insertStruct);
					  }
					  //3. 첫번째 품사가 아닐 경우, 단어핸들 찾는다.
					  else if (l > 0) {
						  word = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
							  (WPARAM)TVGN_CHILD, (LPARAM)alphabet);
						  item.hItem = word;
						  item.mask = TVIF_TEXT | TVIF_HANDLE;
						  item.pszText = textWord;
						  item.cchTextMax = 32;
						  SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

						  while (word != NULL && strcmp(textWord, index->word.spelling) != 0) {
							  word = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
								  (WPARAM)TVGN_NEXT, (LPARAM)word);
							  item.hItem = word;
							  item.pszText = textWord;
							  item.cchTextMax = 32;
							  SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);
						  }
					  }

					  //4. 단어핸들 하위에 품사핸들 추가해준다.
					  insertStruct.hParent = word;
					  insertStruct.hInsertAfter = TVI_LAST;
					  insertStruct.item.pszText = index->word.wordClass;
					  wordClass = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
						  (LPARAM)&insertStruct);

				  }

				  //1.5.3.2.3. 같은 철자가 없으면 alphabet핸들 하위에 단어핸들 추가한다.
				  else if (spellingCount <= 1) {
					  insertStruct.hParent = alphabet;
					  insertStruct.hInsertAfter = TVI_LAST;
					  insertStruct.item.pszText = index->word.spelling;
					  word = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
						  (LPARAM)&insertStruct);
				  }

				  if (spellingIndexes != NULL) {
					  free(spellingIndexes);
					  spellingIndexes = NULL;
				  }

				  j++;
			  }

			  i++;
		  }

		  if (vocabularyCount > 0) { //단어장의 단어가 1개 이상일 경우,

			  //1.5.4 단어장에서 첫번째 단어를 현재 단어로 옮긴다.
			  index = Vocabulary_First(vocabulary);

			  //1.5.5 트리뷰 상 현재 단어의 alphabet핸들을 찾는다.
			  parent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT,
				  (LPARAM)NULL);

			  alphabet = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
				  (LPARAM)parent);

			  item.hItem = alphabet;
			  item.mask = TVIF_HANDLE | TVIF_TEXT;
			  textAlphabet[1] = '\0';
			  item.pszText = textAlphabet;
			  item.cchTextMax = 2;
			  SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);


			  if (index->word.spelling[0] > 96 && index->word.spelling[0] < 123) {
				  alphabets[0] = index->word.spelling[0] - 32;
				  alphabets[1] = '\0';
			  }
			  else {
				  alphabets[0] = index->word.spelling[0];
				  alphabets[1] = '\0';
			  }


			  while (strcmp(textAlphabet, alphabets) != 0) {
				  alphabet = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
					  (LPARAM)alphabet);
				  item.hItem = alphabet;
				  textAlphabet[1] = '\0';
				  item.pszText = textAlphabet;
				  item.cchTextMax = 2;
				  SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);
			  }

			  //1.5.6 트리뷰상 알파벳 핸들 펼친다.
			  SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)parent);

			  //1.5.7 트리뷰상 alphabet 핸들 펼친다.
			  SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)alphabet);

			  //1.5.8 트리뷰 상 현재단어의 단어핸들을 찾는다.
			  word = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
				  (LPARAM)alphabet);
			  item.hItem = word;
			  item.pszText = textWord;
			  item.cchTextMax = 32;
			  SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

			  while (strcmp(textWord, index->word.spelling) != 0) {
				  word = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
					  (LPARAM)word);
				  item.hItem = word;
				  item.pszText = textWord;
				  item.cchTextMax = 32;
				  SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);
			  }

			  //1.5.9 해당 단어가 색인에 동일단어가 있는지 확인한다.
			  if (index->word.spelling[0] > 96 && index->word.spelling[0] < 123) {
				  index_ = IndexBinder_Find(indexBinder, index->word.spelling[0] - 32);
			  }
			  else {
				  index_ = IndexBinder_Find(indexBinder, index->word.spelling[0]);
			  }
			  Index_FindSpellings(index_, index, &spellingIndexes, &spellingCount);

			  //1.6.0. 해당 색인에 동일 단어가 있으면
			  if (spellingCount > 1) {
				  //1. 단어핸들의 몇번째 위치의 품사인지 확인한다.
				  k = Index_Find(index_, index);

				  l = 0;
				  while (spellingIndexes[l] != k) {
					  l++;
				  }

				  //2. 단어핸들을 펼쳐준다.
				  SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)word);

				  //3. 품사핸들을 찾는다.
				  wordClass = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
					  (LPARAM)word);
				  k = 0;
				  while (k < l) {
					  wordClass = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
						  (LPARAM)word);
					  k++;
				  }

				  //4. 품사핸들을 선택된 상태로 처리한다.
				  SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET,
					  (LPARAM)wordClass);

			  }
			  //1.6.1 해당 색인에 동일 단어가 없으면,
			  else {
				  //1.6.1.1 해당 단어 핸들을 선택된 상태로 처리한다.
				  SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)word);

			  }


			  //1.6.0 indexes를 할당 해제한다.
			  if (indexes != NULL) {
				  free(indexes);
				  indexes = NULL;
			  }

			  if (spellingIndexes != NULL) {
				  free(spellingIndexes);
				  spellingIndexes = NULL;
			  }


			  //1.6.1 현재 단어카드가 있을 경우 단어카드를 출력한다.
			  if (index != NULL) {
				  SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0,
					  (LPARAM)index->word.spelling);
				  SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0,
					  (LPARAM)index->word.wordClass);
				  SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0,
					  (LPARAM)index->word.meaning);
				  SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0,
					  (LPARAM)index->word.example);
			  }
		  }
		  else {
		  SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0,(LPARAM)"");
		  SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0,(LPARAM)"");
		  SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0,(LPARAM)"");
		  SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0,(LPARAM)"");
          }
	  }

	  return TRUE;

}

BOOL VocabularyForm_OnPutButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	//2. 꽂기 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//2.1 꽂기 윈도우를 출력한다.
		DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_PUTTINGFORM),NULL,
			PuttingFormProc);
	}
	return TRUE;
}

BOOL VocabularyForm_OnFindButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	//3. 찾기 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//2.1 찾기 윈도우를 출력한다.
		DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_FINDINGFORM), NULL,
			FindingFormProc);
	}
	return TRUE;
}

BOOL VocabularyForm_OnPutOutButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	//4. 뽑기 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//4.1 뽑기 윈도우를 출력한다.
		DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_PUTTINGOUTFORM), NULL,
			PuttingOutFormProc);
	}
	return TRUE;
}

BOOL VocabularyForm_OnFirstButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Vocabulary* vocabulary;
	IndexBinder* indexBinder;
	WordCard* index;
	Index* index_;
	HTREEITEM parent;
	HTREEITEM alphabet;
	HTREEITEM word;
	HTREEITEM wordClass;
	TVITEM item = { 0, };
	TCHAR textAlphabet[2];
	TCHAR alphabets[2];
	Long i;
	Long j = 0;
	Long k = 0;
	TCHAR textWord[32];
	TCHAR textWordClass[12];
	Long(*indexes);
	Long count;
	WordCard* testIndex;

	//6. 처음 버튼을 클릭했을때
	if (HIWORD(wParam) == BN_CLICKED) {
		//6.1 단어장에서 첫번째 단어를 현재 단어로 옮긴다.
		vocabulary = (Vocabulary*)GetWindowLong(hWnd, GWL_USERDATA);
		index = Vocabulary_First(vocabulary);

		//6.2 현재 단어의 alphabet핸들을 찾는다.
		parent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT,
			(LPARAM)NULL);
		alphabet = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
			(LPARAM)parent);
		item.mask = TVIF_HANDLE | TVIF_TEXT;
		item.hItem = alphabet;
		textAlphabet[1] = '\0';
		item.pszText = textAlphabet;
		item.cchTextMax = 2;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

		if (index->word.spelling[0] > 96 && index->word.spelling[0] < 123) {
			alphabets[0] = index->word.spelling[0]-32;
			alphabets[1] = '\0';
		}
		else {
			alphabets[0] = index->word.spelling[0];
			alphabets[1] = '\0';
		}

		while(strcmp(textAlphabet, alphabets)!=0) {
			alphabet = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
				(LPARAM)alphabet);
			item.hItem = alphabet;
			item.pszText = textAlphabet;
			item.cchTextMax = 2;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);
		}

		//6.3. 트리뷰 상 알파벳 핸들을 펼친다.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)parent);

		//6.4. 트리뷰 상 alphabet 핸들을 펼친다.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)alphabet);

		//6.5. 단어핸들 찾는다.
		word = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
			(LPARAM)alphabet);
		item.hItem = word;
		item.pszText = textWord;
		item.cchTextMax = 32;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

		while (strcmp(textWord, index->word.spelling) != 0) {
			word = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
				(LPARAM)word);
			item.hItem = word;
			item.pszText = textWord;
			item.cchTextMax = 32;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);
		}

		//6.6. 해당 색인에서 현재 단어의 동일단어 유무를 확인한다.
		indexBinder = (IndexBinder*)GetProp(hWnd, "indexBinder");

		if (index->word.spelling[0] > 96 && index->word.spelling[0] < 123) {
			index_ = IndexBinder_Find(indexBinder, index->word.spelling[0] - 32);
		}
		else {
			index_ = IndexBinder_Find(indexBinder, index->word.spelling[0]);
		}
		Index_FindSpellings(index_, index, &indexes, &count); // 동일 단어가 있을 경우, 배열에서 첨자 위치 확인.

		//6.5 동일 단어가 있으면,
		if (count > 1) {
			//6.5.0. 색인에서 해당 단어카드의 위치를 찾는다.
			i = Index_Find(index_, index); 

			//6.5.1. indexes에서 몇번째에 위치하는지 확인한다.
			j = 0;
			while (indexes[j] != i) {
				j++;
			}

			//6.5.2 품사핸들 찾는다.
			wordClass = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
				(LPARAM)word);
			i = 0;
			while (i < j) {
				wordClass = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
					(LPARAM)wordClass);
				i++;
			}

			//6.5.3. 단어핸들 펼쳐준다.
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)word);

			//6.5.4. 해당 품사를 선택된 상태로 처리한다.
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET, 
				(LPARAM)wordClass);
		}

		//6.6 동일 단어가 없으면,
		else {
			//6.6.1. 해당 단어 핸들을 선택된 상태로 처리한다.
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)word);
		}

		//7.0 현재 단어를 단어에 출력한다..
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0, (LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0, (LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0, (LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0, (LPARAM)index->word.example);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ISIMPORTANT), WM_SETTEXT, 0, (LPARAM)index->quiz.isImportant);

		if (indexes != NULL) {
			free(indexes);
			indexes = NULL;
		}

	}
	return TRUE;
}

BOOL VocabularyForm_OnPreviousButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Vocabulary* vocabulary;
	IndexBinder* indexBinder;
	WordCard* index;
	Index* index_;
	HTREEITEM parent;
	HTREEITEM alphabet;
	HTREEITEM word;
	HTREEITEM wordClass;
	TVITEM item = { 0, };
	TCHAR textAlphabet[2];
	TCHAR alphabets[2];
	Long i;
	Long j = 0;
	TCHAR textWord[32];
	Long(*indexes);
	Long count;



	//7. 이전 버튼을 클릭했을때
	if (HIWORD(wParam) == BN_CLICKED) {
		//7.1 단어장에서 이전 단어를 현재 단어로 옮긴다.
		vocabulary = (Vocabulary*)GetWindowLong(hWnd, GWL_USERDATA);
		index = Vocabulary_Previous(vocabulary);

		//7.2 현재 단어의 alphabet핸들을 찾는다.
		parent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT,
			(LPARAM)NULL);
		alphabet = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
			(LPARAM)parent);
		item.mask = TVIF_HANDLE | TVIF_TEXT;
		item.hItem = alphabet;
		textAlphabet[1] = '\0';
		item.pszText = textAlphabet;
		item.cchTextMax = 2;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

		if (index->word.spelling[0] > 96 && index->word.spelling[0] < 123) {
			alphabets[0] = index->word.spelling[0] - 32;
			alphabets[1] = '\0';
		}
		else {
			alphabets[0] = index->word.spelling[0];
			alphabets[1] = '\0';
		}

		while (strcmp(textAlphabet, alphabets)!=0) {
			alphabet = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
				(LPARAM)alphabet);
			item.hItem = alphabet;
			item.pszText = textAlphabet;
			item.cchTextMax = 2;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);
		}

		//7.3 트리뷰 상 알파벳 핸들을 펼친다.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)parent);

		//7.4 트리뷰 상 alphabet 핸들을 펼친다.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)alphabet);

		//7.5 현재 단어의 단어핸들을 찾는다.
		word = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
			(LPARAM)alphabet);
		item.hItem = word;
		item.pszText = textWord;
		item.cchTextMax = 32;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

		while (strcmp(textWord, index->word.spelling) != 0) {
			word = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
				(LPARAM)word);
			item.hItem = word;
			item.pszText = textWord;
			item.cchTextMax = 32;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);
		}

		//7.6 해당 색인에 동일 단어가 있는지 확인한다.
		indexBinder = (IndexBinder*)GetProp(hWnd, "indexBinder");

		if (index->word.spelling[0] > 96 && index->word.spelling[0] < 123) {
			index_ = IndexBinder_Find(indexBinder, index->word.spelling[0] - 32);
		}
		else {
			index_ = IndexBinder_Find(indexBinder, index->word.spelling[0]);
		}
		Index_FindSpellings(index_, index, &indexes, &count);

		//7.7 동일 단어가 있을 경우,
		if (count > 1) {
			//7.7.1 단어핸들의 몇번째 위치의 품사인지 확인한다.
			i = Index_Find(index_, index); //색인에서 단어의 위치.
			while (indexes[j]!=i) {
				j++;
			}
			//7.7.2 품사핸들을 찾는다.
			wordClass = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
				(LPARAM)word);
			i = 0;
			while (i < j) {
				wordClass = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
					(LPARAM)wordClass);
				i++;
			}
			//7.7.3 단어핸들을 펼쳐준다.
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, 
				(LPARAM)word);
			//7.7.4 품사핸들을 선택된 상태로 처리한다.
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET, 
				(LPARAM)wordClass);
		}
		//7.8 동일 단어가 없을 경우, 해당 단어 핸들을 선택된 상태로 처리한다.
		else {
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)word);
		}
		
		//7.9 현재 단어를 단어에 출력한다..
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0, (LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0, (LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0, (LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0, (LPARAM)index->word.example);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ISIMPORTANT), WM_SETTEXT, 0, (LPARAM)index->quiz.isImportant);

		//8.0 indexes를 할당해제한다.
		if (indexes != NULL) {
			free(indexes);
			indexes = NULL;
		}
	}
	return TRUE;
}

BOOL VocabularyForm_OnNextButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Vocabulary* vocabulary;
	IndexBinder* indexBinder;
	WordCard* index;
	Index* index_;
	HTREEITEM parent;
	HTREEITEM alphabet;
	HTREEITEM word;
	HTREEITEM wordClass;
	TVITEM item = { 0, };
	TCHAR textAlphabet[2];
	TCHAR alphabets[2];
	Long i;
	Long j = 0;
	TCHAR textWord[32];
	Long(*indexes);
	Long count;

	//8. 다음 버튼을 클릭했을때
	if (HIWORD(wParam) == BN_CLICKED) {
		//8.1 단어장에서 다음 단어를 현재 단어로 옮긴다.
		vocabulary = (Vocabulary*)GetWindowLong(hWnd, GWL_USERDATA);
		index = Vocabulary_Next(vocabulary);

		//8.2 현재 단어의 alphabet핸들을 찾는다.
		parent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT,
			(LPARAM)NULL);
		alphabet = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
			(LPARAM)parent);
		item.mask = TVIF_HANDLE | TVIF_TEXT;
		item.hItem = alphabet;
		textAlphabet[1] = '\0';
		item.pszText = textAlphabet;
		item.cchTextMax = 2;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

		if (index->word.spelling[0] > 96 && index->word.spelling[0] < 123) {
			alphabets[0] = index->word.spelling[0] - 32;
			alphabets[1] = '\0';
		}
		else {
			alphabets[0] = index->word.spelling[0];
			alphabets[1] = '\0';
		}

		while (strcmp(textAlphabet, alphabets)!=0) {
			alphabet = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
				(LPARAM)alphabet);
			item.hItem = alphabet;
			item.pszText = textAlphabet;
			item.cchTextMax = 2;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);
		}

		//8.3 트리뷰 상 알파벳 핸들을 펼친다.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)parent);

		//8.4 트리뷰 상 alphabet 핸들을 펼친다.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)alphabet);

		//8.5 트리뷰 상 현재 단어의 단어핸들을 찾는다.
		word = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
			(LPARAM)alphabet);
		item.hItem = word;
		item.pszText = textWord;
		item.cchTextMax = 32;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

		while (strcmp(textWord, index->word.spelling) != 0) {
			word = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
				(LPARAM)word);
			item.hItem = word;
			item.pszText = textWord;
			item.cchTextMax = 32;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);
		}

		//8.6 해당 색인에 동일 단어가 있는지 확인한다.
		indexBinder = (IndexBinder*)GetProp(hWnd, "indexBinder");

		if (index->word.spelling[0] > 96 && index->word.spelling[0] < 123) {
			index_ = IndexBinder_Find(indexBinder, index->word.spelling[0] - 32);
		}
		else {
			index_ = IndexBinder_Find(indexBinder, index->word.spelling[0]);
		}
		Index_FindSpellings(index_, index, &indexes, &count);

		//8.7 동일 단어가 있으면,
		if (count > 1) {
			//8.7.1 단어핸들의 몇번째 위치의 품사인지 확인한다.
			i = Index_Find(index_, index);
			while (indexes[j] != i) {
				j++;
			}
			//8.7.2 품사핸들을 찾는다.
			wordClass = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD ,
				(LPARAM)word);
			i = 0;
			while (i < j) {
				wordClass = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
					(LPARAM)wordClass);
				i++;
			}
			//8.7.3. 단어핸들을 펼쳐준다.
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)word);

			//8.7.4 품사핸들을 선택된 상태로 처리한다.
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET,
				(LPARAM)wordClass);
		}
		//8.8 동일단어가 없으면, 단어핸들을 선택된 상태로 처리한다.
		else {
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)word);
		}
		//8.9 현재 단어를 단어에 출력한다..
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0, (LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0, (LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0, (LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0, (LPARAM)index->word.example);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ISIMPORTANT), WM_SETTEXT, 0, (LPARAM)index->quiz.isImportant);

		//9.0 indexes를 할당해제한다.
		if (indexes != NULL) {
			free(indexes);
			indexes = NULL;
		}
	}
	return TRUE;
}

BOOL VocabularyForm_OnLastButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Vocabulary* vocabulary;
	IndexBinder* indexBinder;
	WordCard* index;
	Index* index_;
	HTREEITEM parent;
	HTREEITEM alphabet;
	HTREEITEM word;
	HTREEITEM wordClass;
	TVITEM item = { 0, };
	TCHAR textAlphabet[2];
	TCHAR alphabets[2];
	TCHAR textWord[32];
	Long i;
	Long j = 0;
	Long(*indexes);
	Long count;

	//9. 마지막 버튼을 클릭했을때
	if (HIWORD(wParam) == BN_CLICKED) {
		//9.1 단어장에서 마지막 단어를 현재 단어로 옮긴다.
		vocabulary = (Vocabulary*)GetWindowLong(hWnd, GWL_USERDATA);
		index = Vocabulary_Last(vocabulary);

		//9.2 현재 단어의 alphabet핸들을 찾는다.
		parent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT,
			(LPARAM)NULL);
		alphabet = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
			(LPARAM)parent);
		item.mask = TVIF_HANDLE | TVIF_TEXT;
		item.hItem = alphabet;
		textAlphabet[1] = '\0';
		item.pszText = textAlphabet;
		item.cchTextMax = 2;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

		if (index->word.spelling[0] > 96 && index->word.spelling[0] < 123) {
			alphabets[0] = index->word.spelling[0] - 32;
			alphabets[1] = '\0';
		}
		else {
			alphabets[0] = index->word.spelling[0];
			alphabets[1] = '\0';
		}

		while (strcmp(textAlphabet,alphabets)!=0) {
			alphabet = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
				(LPARAM)alphabet);
			item.hItem = alphabet;
			item.pszText = textAlphabet;
			item.cchTextMax = 2;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);
		}

		//9.3 트리뷰 상 알파벳 핸들을 펼친다.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)parent);

		//9.4 트리뷰 상 alphabet 핸들을 펼친다.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)alphabet);

		//9.5 현재 단어의 단어핸들을 찾는다.
		word = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
			(LPARAM)alphabet);
		item.hItem = word;
		item.pszText = textWord;
		item.cchTextMax = 32;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

		while (strcmp(textWord, index->word.spelling) != 0) {
			word = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
				(LPARAM)word);
			item.hItem = word;
			item.pszText = textWord;
			item.cchTextMax = 32;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);
		}

		//9.6 해당 색인에 동일 단어가 있는지 확인한다.
		indexBinder = (IndexBinder*)GetProp(hWnd, "indexBinder");

		if (index->word.spelling[0] > 96 && index->word.spelling[0] < 123) {
			index_ = IndexBinder_Find(indexBinder, index->word.spelling[0] - 32);
		}
		else {
			index_ = IndexBinder_Find(indexBinder, index->word.spelling[0]);
		}
		Index_FindSpellings(index_, index, &indexes, &count);

		//9.7 동일단어가 있으면,
		if (count > 1) {
			//9.7.1 단어핸들의 몇번째 위치의 품사인지 확인한다.
			i = Index_Find(index_, index);
			while (indexes[j] != i) {
				j++;
			}
			//9.7.2 품사핸들을 찾는다.
			wordClass = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
				(LPARAM)word);
			i = 0;
			while (i < j) {
				wordClass = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
					(LPARAM)wordClass);
			}
			//9.7.3. 단어핸들을 펼쳐준다.
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)word);

			//9.7.4. 품사핸들을 선택된 상태로 처리한다.
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET, 
				(LPARAM)wordClass);
		}
		//9.8 동일단어가 없으면 단어핸들을 선택된 상태로 처리한다.
		else {
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET,(LPARAM)word);
		}
		//9.9 현재 단어를 단어에 출력한다..
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0, (LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0, (LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0, (LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0, (LPARAM)index->word.example);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ISIMPORTANT), WM_SETTEXT, 0, (LPARAM)index->quiz.isImportant);

		//10.0 indexes를 할당해제한다.
		if (indexes != NULL) {
			free(indexes);
			indexes = NULL;
		}
	}
	return TRUE;
}

BOOL VocabularyForm_OnArrangeButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Vocabulary* vocabulary;
	IndexBinder* indexBinder;
	WordCard* index;
	Index* index_;
	HTREEITEM parent;
	HTREEITEM alphabet;
	HTREEITEM word;
	HTREEITEM wordClass;
	TCHAR textAlphabet[2];
	TCHAR alphabets[2];
	TCHAR textWord[32];
	Index(*indexes);
	Long count;
	Long i = 0;
	Long j = 0;
	Long k;
	Long l;
	TVINSERTSTRUCT insertStruct = { 0, };
	TVITEM item = { 0, };
	Long(*spellingIndexes);
	Long spellingCount;

	//5. 정리하기 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//5.1 색인철에서 정리한다.
		indexBinder = (IndexBinder*)GetProp(hWnd, "indexBinder");
		IndexBinder_Arrange(indexBinder);

		//5.2 색인리스트를 만든다.
		MakeList(indexBinder, &indexes, &count);

		//5.3 트리뷰 상 알파벳 핸들을 없앤다.
		parent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT,
			(LPARAM)NULL);
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_DELETEITEM, 0, (LPARAM)parent);

		//5.4 트리뷰 상 알파벳 핸들을 만든다.
		insertStruct.hParent = TVI_ROOT;
		insertStruct.hInsertAfter = TVI_FIRST;
		insertStruct.item.mask = TVIF_TEXT;
		insertStruct.item.pszText = "알파벳";
		parent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_INSERTITEM, 0, (LPARAM)&insertStruct);

		//5.5 색인리스트의 개수(count)만큼 반복한다.
		while (i < count) {
			//5.5.1 알파벳핸들 하위에 alphabet핸들 추가한다.
			insertStruct.hParent = parent;
			insertStruct.hInsertAfter = TVI_LAST;
			insertStruct.item.mask = TVIF_TEXT;
			alphabets[0] = indexes[i].alphabet;
			alphabets[1] = '\0';
			insertStruct.item.pszText = alphabets;
			alphabet = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
				(LPARAM)&insertStruct);

			//5.5.2 해당 alphabet의 length만큼 반복한다.
			j = 0;
			while (j < indexes[i].length) {

				//5.5.2.1 해당 단어의 색인에 동일단어 유무 확인한다.
				index = Index_GetAt(indexes+i, j);
				if (index->word.spelling[0] > 96 && index->word.spelling[0] < 123) {//소문자면 대문자로
					index_ = IndexBinder_Find(indexBinder, index->word.spelling[0]-32);
				}
				else {
					index_ = IndexBinder_Find(indexBinder, index->word.spelling[0]);
				}

				Index_FindSpellings(index_, index, &spellingIndexes, &spellingCount);

				//5.5.2.2. 동일단어가 있으면,
				if (spellingCount > 1) {
					//5.5.2.2.1 단어핸들의 몇 번째 위치의 품사인지 확인한다.
					k = Index_Find(index_, index);
					l = 0;
					while (spellingIndexes[l] != k) {
						l++;
					}
					//5.5.2.2.2 첫번째 품사일 경우, 단어핸들 먼저 추가해준다.
					if (l <= 0) {
						insertStruct.hParent = alphabet;
						insertStruct.hInsertAfter = TVI_LAST;
						insertStruct.item.pszText = index->word.spelling;
						word = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
							(LPARAM)&insertStruct);
					}
					//5.5.2.2.3 첫번째 품사가 아닐경우, 단어핸들 찾는다.
					else {
						word = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
							(WPARAM)TVGN_CHILD, (LPARAM)alphabet);
						item.hItem = word;
						item.mask = TVIF_TEXT | TVIF_HANDLE;
						item.pszText = textWord;
						item.cchTextMax = 32;
						SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

						while (word != NULL && strcmp(textWord, index->word.spelling) != 0) {
							word = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
								(WPARAM)TVGN_NEXT, (LPARAM)word);
							item.hItem = word;
							item.pszText = textWord;
							item.cchTextMax = 32;
							SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);
						}
					}
					//5.5.2.2.4. 단어핸들 하위에 품사핸들 추가한다.
					insertStruct.hParent = word;
					insertStruct.hInsertAfter = TVI_LAST;
					insertStruct.item.pszText = index->word.wordClass;
					wordClass = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_INSERTITEM, 0, 
						(LPARAM)&insertStruct);
				}
				//5.5.2.3 동일단어 없으면 alphabet핸들 하위에 단어핸들 추가한다.
				else {
					insertStruct.hParent = alphabet;
					insertStruct.hInsertAfter = TVI_LAST;
					insertStruct.item.pszText = index->word.spelling;
					SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_INSERTITEM, 0, (LPARAM)&insertStruct);
				}

				if (spellingIndexes != NULL) {
					free(spellingIndexes);
					spellingIndexes = NULL;
				}

				j++;
			}
			i++;
		}

		//5.6 현재 위치를 첫번째 단어카트로 옮긴다.
		vocabulary = (Vocabulary*)GetWindowLong(hWnd, GWL_USERDATA);
		index = Vocabulary_First(vocabulary);

		//5.7 현재 단어의 alphabet핸들을 찾는다.
		alphabet = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
			(LPARAM)parent);

		item.mask = TVIF_HANDLE | TVIF_TEXT;
		item.hItem = alphabet;
		textAlphabet[1] = '\0';
		item.pszText = textAlphabet;
		item.cchTextMax = 2;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

		if (index->word.spelling[0] > 96 && index->word.spelling[0] < 123) {
			alphabets[0] = index->word.spelling[0] - 32;
			alphabets[1] = '\0';
		}
		else {
			alphabets[0] = index->word.spelling[0];
			alphabets[1] = '\0';
		}

		while (strcmp(textAlphabet, alphabets) != 0) {
			alphabet = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
				(LPARAM)alphabet);
			item.hItem = alphabet;
			textAlphabet[1] = '\0';
			item.pszText = textAlphabet;
			item.cchTextMax = 2;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);
		}
		//5.8. 알파벳 핸들을 펼친다.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)parent);

		//5.9. alphabet 핸들을 펼친다.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)alphabet);

		//6.0. 단어핸들을 찾는다.
		word = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
			(LPARAM)alphabet);
		item.hItem = word;
		item.pszText = textWord;
		item.cchTextMax = 32;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

		while (strcmp(textWord, index->word.spelling) != 0) {
			word = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
				(LPARAM)word);
			item.hItem = word;
			item.pszText = textWord;
			item.cchTextMax = 32;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);
		}
		//6.1. 해당 색인에 동일 단어가 있는지 확인한다.
		if (index->word.spelling[0] > 96 && index->word.spelling[0] < 123) {
			index_ = IndexBinder_Find(indexBinder, index->word.spelling[0] - 32);
		}
		else {
			index_ = IndexBinder_Find(indexBinder, index->word.spelling[0]);
		}
		Index_FindSpellings(index_, index, &spellingIndexes, &spellingCount);

		//6.2. 동일 단어가 있으면,
		if (spellingCount > 1) {
			//6.2.1. 단어핸들의 몇번째 위치의 품사인지 확인한다.
			i = Index_Find(index_, index);
			j = 0;
			while (spellingIndexes[j] != i) {
				j++;
			}
			//6.2.2. 품사핸들을 찾는다.
			wordClass = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
				(LPARAM)word);
			i = 0;
			while (i < j) {
				wordClass = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
					(LPARAM)wordClass);
				i++;
			}
			//6.2.3. 단어핸들을 펼쳐준다.
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)word);

			//6.2.4. 품사핸들을 선택된 상태로 처리한다.
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET, 
				(LPARAM)wordClass);
		}
		//6.3 동일단어가 없으면 단어핸들을 선택된 상태로 처리한다.
		else {
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET, 
				(LPARAM)word);
		}

		//6.4 현재 단어카드를 출력한다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0, index->word.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0, index->word.wordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0, index->word.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0, index->word.example);

		//6.5 indexes를 할당해제한다.
		if (indexes != NULL) {
			free(indexes);
			indexes = NULL;
		}	

		if (spellingIndexes != NULL) {
			free(spellingIndexes);
			spellingIndexes = NULL;
		}
	}
	return TRUE;
}
	
BOOL VocabularyForm_OnTreeViewItemButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Vocabulary* vocabulary;
	IndexBinder* indexBinder;
	Index* index_;
	WordCard* index = NULL;
	HTREEITEM parent;
	HTREEITEM testWord;
	HTREEITEM testWordClass;
	HTREEITEM wordClass;
	HTREEITEM currentHandle;
	HTREEITEM previousHandle;
	HTREEITEM formerHandle;
	HTREEITEM childHandle;
	HTREEITEM formerformer;
	TVITEM item = { 0, };
	TCHAR textAlphabet[2];
	TCHAR textWord[32];
	Long i = 0;
	Long j = 0;
	Long k = 0;

	//7. 트리뷰상 핸들을 더블클릭했을때
	if (((LPNMHDR)lParam)->code == NM_DBLCLK) {
		//7.1 더블 클릭한 핸들의 alphabet 핸들 유무를 확인한다.
		parent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT,
			(LPARAM)NULL); // ROOT 핸들
		currentHandle = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CARET, 
			(LPARAM)NULL); // 더블클릭한 핸들
		childHandle = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
			(LPARAM)currentHandle); // 더블클릭한 핸들의 자식.
		previousHandle = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, 
			(WPARAM)TVGN_PARENT, (LPARAM)currentHandle); // 더블클릭한 핸들의 부모
		formerHandle = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
			(WPARAM)TVGN_PARENT, (LPARAM)previousHandle); // 더블클릭한 핸들의 조부모.
		formerformer = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
			(WPARAM)TVGN_PARENT, (LPARAM)formerHandle);// 더블클릭한 핸들의 조조부모.
		
		//7.2 더블클릭한 핸들이 단어 핸들이고 하위 핸들이 없으면,
		if (formerHandle == parent && childHandle == NULL) {
			//7.2.1 단어핸들의 alphabet 핸들을 찾는다.
			item.mask = TVIF_TEXT | TVIF_HANDLE;
			item.hItem = previousHandle;
			textAlphabet[1] = '\0';
			item.pszText = textAlphabet;
			item.cchTextMax = 2;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

			//7.2.2 alphabet핸들의 색인 위치를 찾는다.
			indexBinder = (IndexBinder*)GetProp(hWnd, "indexBinder");
			index_ = IndexBinder_Find(indexBinder, textAlphabet[0]);

			//7.2.3 단어핸들의 text를 찾는다.
			item.hItem = currentHandle;
			item.pszText = textWord;
			item.cchTextMax = 32;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

			//7.2.4 해당 단어가 색인에서 몇번째에 위치하는지 찾는다.
			index = Index_GetAt(index_, i);
			while (strcmp(index->word.spelling, textWord) != 0) {
				i++;
				index = Index_GetAt(index_, i);
			}
			//7.2.5 해당 단어를 현재 단어로 옮긴다.
			vocabulary = (Vocabulary*)GetWindowLong(hWnd, GWL_USERDATA);
			index = Move(vocabulary, index);

			//7.2.6 단어에 현재 단어를 출력한다.
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0, index->word.spelling);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0, index->word.wordClass);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0, index->word.meaning);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0, index->word.example);

		}
		//7.3 더블클릭한 핸들이 품사 핸들이면,
		else if (formerformer == parent) {
			//7.3.1 품사핸들의 alphabet핸들을 찾는다.
			item.mask = TVIF_TEXT | TVIF_HANDLE;
			item.hItem = formerHandle;
			textAlphabet[1] = '\0';
			item.pszText = textAlphabet;
			item.cchTextMax = 2;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

			//7.3.2 alphabet핸들의 색인 위치를 찾는다.
			indexBinder = (IndexBinder*)GetProp(hWnd, "indexBinder");
			index_ = IndexBinder_Find(indexBinder, textAlphabet[0]);

			//7.3.3 해당 품사핸들이 단어핸들의 하위 몇 번째 인지 확인한다.
			testWordClass = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
				(LPARAM)previousHandle);
			i = 1;
			while (testWordClass != currentHandle) {
				testWordClass = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, 
					(WPARAM)TVGN_NEXT, (LPARAM)testWordClass);

				i++;
			}

			//7.3.4. 선택된 핸들의 단어핸들 text를 읽는다.
			item.hItem = previousHandle;
			item.pszText = textWord;
			item.cchTextMax = 32;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

			//7.3.4 해당 단어카드의 위치를 찾는다.
			index = Index_GetAt(index_, j);
			while (k < i) {
				if (strcmp(index->word.spelling, textWord) == 0) {
					k++;
				}
				j++;
				index = Index_GetAt(index_, j);
			}
			j--;
			index = Index_GetAt(index_, j);

			//7.3.5 해당 단어를 현재 단어로 옮긴다.
			vocabulary = (Vocabulary*)GetWindowLong(hWnd, GWL_USERDATA);
			index = Move(vocabulary, index);

			//7.3.6 단어에 현재 단어를 출력한다.
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0, index->word.spelling);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0, index->word.wordClass);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0, index->word.meaning);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0, index->word.example);
		}

	}
	return TRUE;
}

BOOL VocabularyForm_OnQuizButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	// 퀴즈 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//퀴즈 윈도우를 출력한다.
		DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_QUIZFORM), NULL,
			QuizFormProc);
	}
	return TRUE;

}

BOOL VocabularyForm_OnCorrectQuizButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	// 암기단어 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//암기단어 윈도우를 출력한다.
		DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_CORRECTQUIZFORM), NULL,
			CorrectQuizFormProc);
	}
	return TRUE;

}

BOOL VocabularyForm_OnPutImportantWordButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Vocabulary* vocabulary;
	WordCard* index;

	//1. 중요단어 추가 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//1.1 현재 단어카드의 위치를 읽는다.
		vocabulary = (Vocabulary*)GetWindowLong(hWnd, GWL_USERDATA);
		index = vocabulary->current;
		//1.2 단어장에서 중요단어로 추가한다.
		index = PutImportantWord(vocabulary, index);
		//1.3 메인윈도우의 단어카드에 '중요'를 출력한다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ISIMPORTANT), WM_SETTEXT, 0, (LPARAM)index->quiz.isImportant);
	}
	return TRUE;
}

BOOL VocabularyForm_OnPutOutImportantWordButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Vocabulary* vocabulary;
	WordCard* index;

	//2. 중요단어 취소 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//2.1 중요단어 취소 버튼을 클릭했을 때
		vocabulary = (Vocabulary*)GetWindowLong(hWnd, GWL_USERDATA);
		index = vocabulary->current;
		//2.2  단어장에서 중요단어 취소한다.
		index = PutOutImportantWord(vocabulary, index);
		//2.3 메인 윈도우의 단어카드에 '중요'를 지운다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ISIMPORTANT), WM_SETTEXT, 0, (LPARAM)index->quiz.isImportant);
	}
	return TRUE;
}

BOOL VocabularyForm_OnImportantWordButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	// 중요단어 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		// 중요단어 윈도우를 출력한다.
		DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_IMPORTANTWORDSFORM),
			NULL, ImportantWordsFormProc);
	}
	return TRUE;
}


BOOL VocabularyForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Vocabulary* vocabulary;
	IndexBinder* indexBinder;
	CorrectQuiz* correctQuiz;

	//10. 닫기 버튼을 클릭했을 때
	vocabulary = (Vocabulary*)GetWindowLong(hWnd, GWL_USERDATA);
	correctQuiz = (CorrectQuiz*)GetProp(hWnd, "correctQuiz");
	indexBinder = (IndexBinder*)GetProp(hWnd, "indexBinder");

	if (vocabulary != NULL) {

		//10.1 저장한다.
		Save(vocabulary);

		//10.2 단어장을 없앤다.
		Vocabulary_Destroy(vocabulary);
		free(vocabulary);
		vocabulary = NULL;
	}
	
	if (indexBinder != NULL) {
		IndexBinder_Destroy(indexBinder);
		free(indexBinder);
		indexBinder = NULL;
	}

	if (correctQuiz != NULL) {
		//10.1 저장한다.
		CorrectQuiz_Save(correctQuiz);

		//10.2 단어장을 없앤다.
		CorrectQuiz_Destroy(correctQuiz);
		free(correctQuiz);
		correctQuiz = NULL;
	}

	RemoveProp(hWnd, "indexBinder");
	RemoveProp(hWnd, "correctQuiz");

	//10.3 윈도우를 종료한다.
	EndDialog(hWnd, 0);
	return TRUE;
}
