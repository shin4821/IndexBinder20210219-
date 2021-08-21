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
#include<Commctrl.h> // TVM�� ������� �߰�
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

	//1. �����찡 �����ɶ�
	  //1.1 �ܾ����� �����.(create)
	  vocabulary = (Vocabulary*)malloc(sizeof(Vocabulary));
	  Vocabulary_Create(vocabulary);
	  SetWindowLong(hWnd, GWL_USERDATA, (LONG)vocabulary);

	  // �ϱ�ܾ �����.
	  correctQuiz = (CorrectQuiz*)malloc(sizeof(CorrectQuiz));
	  CorrectQuiz_Create(correctQuiz);
	  SetProp(hWnd, "correctQuiz", correctQuiz);

	  //1.2 ������ �����.
	  indexBinder = (IndexBinder*)malloc(sizeof(IndexBinder));
	  IndexBinder_Create(indexBinder);
	  SetProp(hWnd, "indexBinder", indexBinder);

	  //1.3 Ʈ���信 ���ĺ��ڵ��� �����.
	  insertStruct.hParent = TVI_ROOT;
	  insertStruct.hInsertAfter = TVI_FIRST;
	  insertStruct.item.mask = TVIF_TEXT | TVIF_HANDLE;
	  insertStruct.item.pszText = "���ĺ�";
	  parent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_INSERTITEM, 0, (LPARAM)&insertStruct);

	  //1.4 �����Ѵ�.
	  vocabularyCount =Load(vocabulary);

	  //�ϱ�ܾ �����Ѵ�.
	  correctQuizCount = CorrectQuiz_Load(correctQuiz);
	  SetProp(hWnd, "correctQuizCount", correctQuizCount);


	  //1.5 ������ �ܾ�ī�� �Ǵ� �ϱ�ܾ 1�� �̻��� ���
	  if (vocabularyCount > 0 || correctQuizCount > 0) {

		  if (vocabularyCount > 0) {
			  //1.5.1 �ܾ����� ó������ ������ �ݺ��Ѵ�.
			  index = Vocabulary_First(vocabulary);
			  while (index != previous) {
				  //1.5.1.1 ����ö�� �����.
				  IndexBinder_Put(indexBinder, index);
				  previous = index;
				  index = Vocabulary_Next(vocabulary);
			  }
		  }
		  

		  if (correctQuizCount > 0) {
			  //1.5.1 �ϱ�ܾ��� ó������ ������ �ݺ��Ѵ�.
			  previous = NULL;
			  index = CorrectQuiz_First(correctQuiz);
			  while (index != previous) {
				  //1.5.1.1 ����ö�� �����.
				  IndexBinder_Put(indexBinder, index);
				  previous = index;
				  index = CorrectQuiz_Next(correctQuiz);
			  }
		  }
		  //1.5.2 ���θ���Ʈ�� �����.
		  MakeList(indexBinder, &indexes, &count);

		  //1.5.3 ���θ���Ʈ�� ����(count)��ŭ �ݺ��Ѵ�.
		  while (i < count) {

			  //1.5.3.1 ���ĺ� �ڵ� ������ alphabet�ڵ��� �߰��Ѵ�.
			  insertStruct.hParent = parent;
			  insertStruct.hInsertAfter = TVI_LAST;

			  alphabets[0] = indexes[i].alphabet;
			  alphabets[1] = '\0';

			  insertStruct.item.pszText = alphabets;
			  alphabet = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
				  (LPARAM)&insertStruct);

			  //1.5.3.2 �ش� alphabet������ length��ŭ �ݺ��Ѵ�.
			  j = 0;
			  while (j < indexes[i].length) {

				  //1.5.3.2.1 �ش� �ܾ��� ���ο� ���ϴܾ� ���� Ȯ���Ѵ�.
				  index = Index_GetAt(indexes + i, j);

				  if (index->word.spelling[0] > 96 && index->word.spelling[0] < 123) {
					  index_ = IndexBinder_Find(indexBinder, index->word.spelling[0] - 32);
				  }
				  else {
					  index_ = IndexBinder_Find(indexBinder, index->word.spelling[0]);
				  }

				  Index_FindSpellings(index_, index, &spellingIndexes, &spellingCount);

				  //1.5.3.2.2. �ش� ���ο� ���� �ܾ ������, 
				  if (spellingCount > 1) {

					  //1. �ܾ��ڵ��� ���° ��ġ�� ǰ������ Ȯ���Ѵ�.
					  k = Index_Find(index_, index);

					  l = 0;
					  while (spellingIndexes[l] != k) {
						  l++;
					  }

					  //2. ù��° ǰ���� ���, �ܾ��ڵ� ���� �߰����ش�.
					  if (l <= 0) {
						  insertStruct.hParent = alphabet;
						  insertStruct.hInsertAfter = TVI_LAST;
						  insertStruct.item.pszText = index->word.spelling;
						  word = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
							  (LPARAM)&insertStruct);
					  }
					  //3. ù��° ǰ�簡 �ƴ� ���, �ܾ��ڵ� ã�´�.
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

					  //4. �ܾ��ڵ� ������ ǰ���ڵ� �߰����ش�.
					  insertStruct.hParent = word;
					  insertStruct.hInsertAfter = TVI_LAST;
					  insertStruct.item.pszText = index->word.wordClass;
					  wordClass = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
						  (LPARAM)&insertStruct);

				  }

				  //1.5.3.2.3. ���� ö�ڰ� ������ alphabet�ڵ� ������ �ܾ��ڵ� �߰��Ѵ�.
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

		  if (vocabularyCount > 0) { //�ܾ����� �ܾ 1�� �̻��� ���,

			  //1.5.4 �ܾ��忡�� ù��° �ܾ ���� �ܾ�� �ű��.
			  index = Vocabulary_First(vocabulary);

			  //1.5.5 Ʈ���� �� ���� �ܾ��� alphabet�ڵ��� ã�´�.
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

			  //1.5.6 Ʈ����� ���ĺ� �ڵ� ��ģ��.
			  SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)parent);

			  //1.5.7 Ʈ����� alphabet �ڵ� ��ģ��.
			  SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)alphabet);

			  //1.5.8 Ʈ���� �� ����ܾ��� �ܾ��ڵ��� ã�´�.
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

			  //1.5.9 �ش� �ܾ ���ο� ���ϴܾ �ִ��� Ȯ���Ѵ�.
			  if (index->word.spelling[0] > 96 && index->word.spelling[0] < 123) {
				  index_ = IndexBinder_Find(indexBinder, index->word.spelling[0] - 32);
			  }
			  else {
				  index_ = IndexBinder_Find(indexBinder, index->word.spelling[0]);
			  }
			  Index_FindSpellings(index_, index, &spellingIndexes, &spellingCount);

			  //1.6.0. �ش� ���ο� ���� �ܾ ������
			  if (spellingCount > 1) {
				  //1. �ܾ��ڵ��� ���° ��ġ�� ǰ������ Ȯ���Ѵ�.
				  k = Index_Find(index_, index);

				  l = 0;
				  while (spellingIndexes[l] != k) {
					  l++;
				  }

				  //2. �ܾ��ڵ��� �����ش�.
				  SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)word);

				  //3. ǰ���ڵ��� ã�´�.
				  wordClass = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
					  (LPARAM)word);
				  k = 0;
				  while (k < l) {
					  wordClass = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
						  (LPARAM)word);
					  k++;
				  }

				  //4. ǰ���ڵ��� ���õ� ���·� ó���Ѵ�.
				  SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET,
					  (LPARAM)wordClass);

			  }
			  //1.6.1 �ش� ���ο� ���� �ܾ ������,
			  else {
				  //1.6.1.1 �ش� �ܾ� �ڵ��� ���õ� ���·� ó���Ѵ�.
				  SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)word);

			  }


			  //1.6.0 indexes�� �Ҵ� �����Ѵ�.
			  if (indexes != NULL) {
				  free(indexes);
				  indexes = NULL;
			  }

			  if (spellingIndexes != NULL) {
				  free(spellingIndexes);
				  spellingIndexes = NULL;
			  }


			  //1.6.1 ���� �ܾ�ī�尡 ���� ��� �ܾ�ī�带 ����Ѵ�.
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
	//2. �ȱ� ��ư�� Ŭ������ ��
	if (HIWORD(wParam) == BN_CLICKED) {
		//2.1 �ȱ� �����츦 ����Ѵ�.
		DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_PUTTINGFORM),NULL,
			PuttingFormProc);
	}
	return TRUE;
}

BOOL VocabularyForm_OnFindButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	//3. ã�� ��ư�� Ŭ������ ��
	if (HIWORD(wParam) == BN_CLICKED) {
		//2.1 ã�� �����츦 ����Ѵ�.
		DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_FINDINGFORM), NULL,
			FindingFormProc);
	}
	return TRUE;
}

BOOL VocabularyForm_OnPutOutButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	//4. �̱� ��ư�� Ŭ������ ��
	if (HIWORD(wParam) == BN_CLICKED) {
		//4.1 �̱� �����츦 ����Ѵ�.
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

	//6. ó�� ��ư�� Ŭ��������
	if (HIWORD(wParam) == BN_CLICKED) {
		//6.1 �ܾ��忡�� ù��° �ܾ ���� �ܾ�� �ű��.
		vocabulary = (Vocabulary*)GetWindowLong(hWnd, GWL_USERDATA);
		index = Vocabulary_First(vocabulary);

		//6.2 ���� �ܾ��� alphabet�ڵ��� ã�´�.
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

		//6.3. Ʈ���� �� ���ĺ� �ڵ��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)parent);

		//6.4. Ʈ���� �� alphabet �ڵ��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)alphabet);

		//6.5. �ܾ��ڵ� ã�´�.
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

		//6.6. �ش� ���ο��� ���� �ܾ��� ���ϴܾ� ������ Ȯ���Ѵ�.
		indexBinder = (IndexBinder*)GetProp(hWnd, "indexBinder");

		if (index->word.spelling[0] > 96 && index->word.spelling[0] < 123) {
			index_ = IndexBinder_Find(indexBinder, index->word.spelling[0] - 32);
		}
		else {
			index_ = IndexBinder_Find(indexBinder, index->word.spelling[0]);
		}
		Index_FindSpellings(index_, index, &indexes, &count); // ���� �ܾ ���� ���, �迭���� ÷�� ��ġ Ȯ��.

		//6.5 ���� �ܾ ������,
		if (count > 1) {
			//6.5.0. ���ο��� �ش� �ܾ�ī���� ��ġ�� ã�´�.
			i = Index_Find(index_, index); 

			//6.5.1. indexes���� ���°�� ��ġ�ϴ��� Ȯ���Ѵ�.
			j = 0;
			while (indexes[j] != i) {
				j++;
			}

			//6.5.2 ǰ���ڵ� ã�´�.
			wordClass = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
				(LPARAM)word);
			i = 0;
			while (i < j) {
				wordClass = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
					(LPARAM)wordClass);
				i++;
			}

			//6.5.3. �ܾ��ڵ� �����ش�.
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)word);

			//6.5.4. �ش� ǰ�縦 ���õ� ���·� ó���Ѵ�.
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET, 
				(LPARAM)wordClass);
		}

		//6.6 ���� �ܾ ������,
		else {
			//6.6.1. �ش� �ܾ� �ڵ��� ���õ� ���·� ó���Ѵ�.
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)word);
		}

		//7.0 ���� �ܾ �ܾ ����Ѵ�..
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



	//7. ���� ��ư�� Ŭ��������
	if (HIWORD(wParam) == BN_CLICKED) {
		//7.1 �ܾ��忡�� ���� �ܾ ���� �ܾ�� �ű��.
		vocabulary = (Vocabulary*)GetWindowLong(hWnd, GWL_USERDATA);
		index = Vocabulary_Previous(vocabulary);

		//7.2 ���� �ܾ��� alphabet�ڵ��� ã�´�.
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

		//7.3 Ʈ���� �� ���ĺ� �ڵ��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)parent);

		//7.4 Ʈ���� �� alphabet �ڵ��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)alphabet);

		//7.5 ���� �ܾ��� �ܾ��ڵ��� ã�´�.
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

		//7.6 �ش� ���ο� ���� �ܾ �ִ��� Ȯ���Ѵ�.
		indexBinder = (IndexBinder*)GetProp(hWnd, "indexBinder");

		if (index->word.spelling[0] > 96 && index->word.spelling[0] < 123) {
			index_ = IndexBinder_Find(indexBinder, index->word.spelling[0] - 32);
		}
		else {
			index_ = IndexBinder_Find(indexBinder, index->word.spelling[0]);
		}
		Index_FindSpellings(index_, index, &indexes, &count);

		//7.7 ���� �ܾ ���� ���,
		if (count > 1) {
			//7.7.1 �ܾ��ڵ��� ���° ��ġ�� ǰ������ Ȯ���Ѵ�.
			i = Index_Find(index_, index); //���ο��� �ܾ��� ��ġ.
			while (indexes[j]!=i) {
				j++;
			}
			//7.7.2 ǰ���ڵ��� ã�´�.
			wordClass = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
				(LPARAM)word);
			i = 0;
			while (i < j) {
				wordClass = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
					(LPARAM)wordClass);
				i++;
			}
			//7.7.3 �ܾ��ڵ��� �����ش�.
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, 
				(LPARAM)word);
			//7.7.4 ǰ���ڵ��� ���õ� ���·� ó���Ѵ�.
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET, 
				(LPARAM)wordClass);
		}
		//7.8 ���� �ܾ ���� ���, �ش� �ܾ� �ڵ��� ���õ� ���·� ó���Ѵ�.
		else {
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)word);
		}
		
		//7.9 ���� �ܾ �ܾ ����Ѵ�..
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0, (LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0, (LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0, (LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0, (LPARAM)index->word.example);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ISIMPORTANT), WM_SETTEXT, 0, (LPARAM)index->quiz.isImportant);

		//8.0 indexes�� �Ҵ������Ѵ�.
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

	//8. ���� ��ư�� Ŭ��������
	if (HIWORD(wParam) == BN_CLICKED) {
		//8.1 �ܾ��忡�� ���� �ܾ ���� �ܾ�� �ű��.
		vocabulary = (Vocabulary*)GetWindowLong(hWnd, GWL_USERDATA);
		index = Vocabulary_Next(vocabulary);

		//8.2 ���� �ܾ��� alphabet�ڵ��� ã�´�.
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

		//8.3 Ʈ���� �� ���ĺ� �ڵ��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)parent);

		//8.4 Ʈ���� �� alphabet �ڵ��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)alphabet);

		//8.5 Ʈ���� �� ���� �ܾ��� �ܾ��ڵ��� ã�´�.
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

		//8.6 �ش� ���ο� ���� �ܾ �ִ��� Ȯ���Ѵ�.
		indexBinder = (IndexBinder*)GetProp(hWnd, "indexBinder");

		if (index->word.spelling[0] > 96 && index->word.spelling[0] < 123) {
			index_ = IndexBinder_Find(indexBinder, index->word.spelling[0] - 32);
		}
		else {
			index_ = IndexBinder_Find(indexBinder, index->word.spelling[0]);
		}
		Index_FindSpellings(index_, index, &indexes, &count);

		//8.7 ���� �ܾ ������,
		if (count > 1) {
			//8.7.1 �ܾ��ڵ��� ���° ��ġ�� ǰ������ Ȯ���Ѵ�.
			i = Index_Find(index_, index);
			while (indexes[j] != i) {
				j++;
			}
			//8.7.2 ǰ���ڵ��� ã�´�.
			wordClass = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD ,
				(LPARAM)word);
			i = 0;
			while (i < j) {
				wordClass = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
					(LPARAM)wordClass);
				i++;
			}
			//8.7.3. �ܾ��ڵ��� �����ش�.
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)word);

			//8.7.4 ǰ���ڵ��� ���õ� ���·� ó���Ѵ�.
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET,
				(LPARAM)wordClass);
		}
		//8.8 ���ϴܾ ������, �ܾ��ڵ��� ���õ� ���·� ó���Ѵ�.
		else {
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)word);
		}
		//8.9 ���� �ܾ �ܾ ����Ѵ�..
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0, (LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0, (LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0, (LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0, (LPARAM)index->word.example);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ISIMPORTANT), WM_SETTEXT, 0, (LPARAM)index->quiz.isImportant);

		//9.0 indexes�� �Ҵ������Ѵ�.
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

	//9. ������ ��ư�� Ŭ��������
	if (HIWORD(wParam) == BN_CLICKED) {
		//9.1 �ܾ��忡�� ������ �ܾ ���� �ܾ�� �ű��.
		vocabulary = (Vocabulary*)GetWindowLong(hWnd, GWL_USERDATA);
		index = Vocabulary_Last(vocabulary);

		//9.2 ���� �ܾ��� alphabet�ڵ��� ã�´�.
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

		//9.3 Ʈ���� �� ���ĺ� �ڵ��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)parent);

		//9.4 Ʈ���� �� alphabet �ڵ��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)alphabet);

		//9.5 ���� �ܾ��� �ܾ��ڵ��� ã�´�.
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

		//9.6 �ش� ���ο� ���� �ܾ �ִ��� Ȯ���Ѵ�.
		indexBinder = (IndexBinder*)GetProp(hWnd, "indexBinder");

		if (index->word.spelling[0] > 96 && index->word.spelling[0] < 123) {
			index_ = IndexBinder_Find(indexBinder, index->word.spelling[0] - 32);
		}
		else {
			index_ = IndexBinder_Find(indexBinder, index->word.spelling[0]);
		}
		Index_FindSpellings(index_, index, &indexes, &count);

		//9.7 ���ϴܾ ������,
		if (count > 1) {
			//9.7.1 �ܾ��ڵ��� ���° ��ġ�� ǰ������ Ȯ���Ѵ�.
			i = Index_Find(index_, index);
			while (indexes[j] != i) {
				j++;
			}
			//9.7.2 ǰ���ڵ��� ã�´�.
			wordClass = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
				(LPARAM)word);
			i = 0;
			while (i < j) {
				wordClass = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
					(LPARAM)wordClass);
			}
			//9.7.3. �ܾ��ڵ��� �����ش�.
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)word);

			//9.7.4. ǰ���ڵ��� ���õ� ���·� ó���Ѵ�.
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET, 
				(LPARAM)wordClass);
		}
		//9.8 ���ϴܾ ������ �ܾ��ڵ��� ���õ� ���·� ó���Ѵ�.
		else {
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET,(LPARAM)word);
		}
		//9.9 ���� �ܾ �ܾ ����Ѵ�..
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0, (LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0, (LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0, (LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0, (LPARAM)index->word.example);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ISIMPORTANT), WM_SETTEXT, 0, (LPARAM)index->quiz.isImportant);

		//10.0 indexes�� �Ҵ������Ѵ�.
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

	//5. �����ϱ� ��ư�� Ŭ������ ��
	if (HIWORD(wParam) == BN_CLICKED) {
		//5.1 ����ö���� �����Ѵ�.
		indexBinder = (IndexBinder*)GetProp(hWnd, "indexBinder");
		IndexBinder_Arrange(indexBinder);

		//5.2 ���θ���Ʈ�� �����.
		MakeList(indexBinder, &indexes, &count);

		//5.3 Ʈ���� �� ���ĺ� �ڵ��� ���ش�.
		parent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT,
			(LPARAM)NULL);
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_DELETEITEM, 0, (LPARAM)parent);

		//5.4 Ʈ���� �� ���ĺ� �ڵ��� �����.
		insertStruct.hParent = TVI_ROOT;
		insertStruct.hInsertAfter = TVI_FIRST;
		insertStruct.item.mask = TVIF_TEXT;
		insertStruct.item.pszText = "���ĺ�";
		parent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_INSERTITEM, 0, (LPARAM)&insertStruct);

		//5.5 ���θ���Ʈ�� ����(count)��ŭ �ݺ��Ѵ�.
		while (i < count) {
			//5.5.1 ���ĺ��ڵ� ������ alphabet�ڵ� �߰��Ѵ�.
			insertStruct.hParent = parent;
			insertStruct.hInsertAfter = TVI_LAST;
			insertStruct.item.mask = TVIF_TEXT;
			alphabets[0] = indexes[i].alphabet;
			alphabets[1] = '\0';
			insertStruct.item.pszText = alphabets;
			alphabet = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
				(LPARAM)&insertStruct);

			//5.5.2 �ش� alphabet�� length��ŭ �ݺ��Ѵ�.
			j = 0;
			while (j < indexes[i].length) {

				//5.5.2.1 �ش� �ܾ��� ���ο� ���ϴܾ� ���� Ȯ���Ѵ�.
				index = Index_GetAt(indexes+i, j);
				if (index->word.spelling[0] > 96 && index->word.spelling[0] < 123) {//�ҹ��ڸ� �빮�ڷ�
					index_ = IndexBinder_Find(indexBinder, index->word.spelling[0]-32);
				}
				else {
					index_ = IndexBinder_Find(indexBinder, index->word.spelling[0]);
				}

				Index_FindSpellings(index_, index, &spellingIndexes, &spellingCount);

				//5.5.2.2. ���ϴܾ ������,
				if (spellingCount > 1) {
					//5.5.2.2.1 �ܾ��ڵ��� �� ��° ��ġ�� ǰ������ Ȯ���Ѵ�.
					k = Index_Find(index_, index);
					l = 0;
					while (spellingIndexes[l] != k) {
						l++;
					}
					//5.5.2.2.2 ù��° ǰ���� ���, �ܾ��ڵ� ���� �߰����ش�.
					if (l <= 0) {
						insertStruct.hParent = alphabet;
						insertStruct.hInsertAfter = TVI_LAST;
						insertStruct.item.pszText = index->word.spelling;
						word = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
							(LPARAM)&insertStruct);
					}
					//5.5.2.2.3 ù��° ǰ�簡 �ƴҰ��, �ܾ��ڵ� ã�´�.
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
					//5.5.2.2.4. �ܾ��ڵ� ������ ǰ���ڵ� �߰��Ѵ�.
					insertStruct.hParent = word;
					insertStruct.hInsertAfter = TVI_LAST;
					insertStruct.item.pszText = index->word.wordClass;
					wordClass = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_INSERTITEM, 0, 
						(LPARAM)&insertStruct);
				}
				//5.5.2.3 ���ϴܾ� ������ alphabet�ڵ� ������ �ܾ��ڵ� �߰��Ѵ�.
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

		//5.6 ���� ��ġ�� ù��° �ܾ�īƮ�� �ű��.
		vocabulary = (Vocabulary*)GetWindowLong(hWnd, GWL_USERDATA);
		index = Vocabulary_First(vocabulary);

		//5.7 ���� �ܾ��� alphabet�ڵ��� ã�´�.
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
		//5.8. ���ĺ� �ڵ��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)parent);

		//5.9. alphabet �ڵ��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)alphabet);

		//6.0. �ܾ��ڵ��� ã�´�.
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
		//6.1. �ش� ���ο� ���� �ܾ �ִ��� Ȯ���Ѵ�.
		if (index->word.spelling[0] > 96 && index->word.spelling[0] < 123) {
			index_ = IndexBinder_Find(indexBinder, index->word.spelling[0] - 32);
		}
		else {
			index_ = IndexBinder_Find(indexBinder, index->word.spelling[0]);
		}
		Index_FindSpellings(index_, index, &spellingIndexes, &spellingCount);

		//6.2. ���� �ܾ ������,
		if (spellingCount > 1) {
			//6.2.1. �ܾ��ڵ��� ���° ��ġ�� ǰ������ Ȯ���Ѵ�.
			i = Index_Find(index_, index);
			j = 0;
			while (spellingIndexes[j] != i) {
				j++;
			}
			//6.2.2. ǰ���ڵ��� ã�´�.
			wordClass = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
				(LPARAM)word);
			i = 0;
			while (i < j) {
				wordClass = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
					(LPARAM)wordClass);
				i++;
			}
			//6.2.3. �ܾ��ڵ��� �����ش�.
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)word);

			//6.2.4. ǰ���ڵ��� ���õ� ���·� ó���Ѵ�.
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET, 
				(LPARAM)wordClass);
		}
		//6.3 ���ϴܾ ������ �ܾ��ڵ��� ���õ� ���·� ó���Ѵ�.
		else {
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET, 
				(LPARAM)word);
		}

		//6.4 ���� �ܾ�ī�带 ����Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0, index->word.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0, index->word.wordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0, index->word.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0, index->word.example);

		//6.5 indexes�� �Ҵ������Ѵ�.
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

	//7. Ʈ����� �ڵ��� ����Ŭ��������
	if (((LPNMHDR)lParam)->code == NM_DBLCLK) {
		//7.1 ���� Ŭ���� �ڵ��� alphabet �ڵ� ������ Ȯ���Ѵ�.
		parent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT,
			(LPARAM)NULL); // ROOT �ڵ�
		currentHandle = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CARET, 
			(LPARAM)NULL); // ����Ŭ���� �ڵ�
		childHandle = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
			(LPARAM)currentHandle); // ����Ŭ���� �ڵ��� �ڽ�.
		previousHandle = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, 
			(WPARAM)TVGN_PARENT, (LPARAM)currentHandle); // ����Ŭ���� �ڵ��� �θ�
		formerHandle = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
			(WPARAM)TVGN_PARENT, (LPARAM)previousHandle); // ����Ŭ���� �ڵ��� ���θ�.
		formerformer = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
			(WPARAM)TVGN_PARENT, (LPARAM)formerHandle);// ����Ŭ���� �ڵ��� �����θ�.
		
		//7.2 ����Ŭ���� �ڵ��� �ܾ� �ڵ��̰� ���� �ڵ��� ������,
		if (formerHandle == parent && childHandle == NULL) {
			//7.2.1 �ܾ��ڵ��� alphabet �ڵ��� ã�´�.
			item.mask = TVIF_TEXT | TVIF_HANDLE;
			item.hItem = previousHandle;
			textAlphabet[1] = '\0';
			item.pszText = textAlphabet;
			item.cchTextMax = 2;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

			//7.2.2 alphabet�ڵ��� ���� ��ġ�� ã�´�.
			indexBinder = (IndexBinder*)GetProp(hWnd, "indexBinder");
			index_ = IndexBinder_Find(indexBinder, textAlphabet[0]);

			//7.2.3 �ܾ��ڵ��� text�� ã�´�.
			item.hItem = currentHandle;
			item.pszText = textWord;
			item.cchTextMax = 32;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

			//7.2.4 �ش� �ܾ ���ο��� ���°�� ��ġ�ϴ��� ã�´�.
			index = Index_GetAt(index_, i);
			while (strcmp(index->word.spelling, textWord) != 0) {
				i++;
				index = Index_GetAt(index_, i);
			}
			//7.2.5 �ش� �ܾ ���� �ܾ�� �ű��.
			vocabulary = (Vocabulary*)GetWindowLong(hWnd, GWL_USERDATA);
			index = Move(vocabulary, index);

			//7.2.6 �ܾ ���� �ܾ ����Ѵ�.
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0, index->word.spelling);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0, index->word.wordClass);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0, index->word.meaning);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0, index->word.example);

		}
		//7.3 ����Ŭ���� �ڵ��� ǰ�� �ڵ��̸�,
		else if (formerformer == parent) {
			//7.3.1 ǰ���ڵ��� alphabet�ڵ��� ã�´�.
			item.mask = TVIF_TEXT | TVIF_HANDLE;
			item.hItem = formerHandle;
			textAlphabet[1] = '\0';
			item.pszText = textAlphabet;
			item.cchTextMax = 2;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

			//7.3.2 alphabet�ڵ��� ���� ��ġ�� ã�´�.
			indexBinder = (IndexBinder*)GetProp(hWnd, "indexBinder");
			index_ = IndexBinder_Find(indexBinder, textAlphabet[0]);

			//7.3.3 �ش� ǰ���ڵ��� �ܾ��ڵ��� ���� �� ��° ���� Ȯ���Ѵ�.
			testWordClass = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
				(LPARAM)previousHandle);
			i = 1;
			while (testWordClass != currentHandle) {
				testWordClass = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, 
					(WPARAM)TVGN_NEXT, (LPARAM)testWordClass);

				i++;
			}

			//7.3.4. ���õ� �ڵ��� �ܾ��ڵ� text�� �д´�.
			item.hItem = previousHandle;
			item.pszText = textWord;
			item.cchTextMax = 32;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

			//7.3.4 �ش� �ܾ�ī���� ��ġ�� ã�´�.
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

			//7.3.5 �ش� �ܾ ���� �ܾ�� �ű��.
			vocabulary = (Vocabulary*)GetWindowLong(hWnd, GWL_USERDATA);
			index = Move(vocabulary, index);

			//7.3.6 �ܾ ���� �ܾ ����Ѵ�.
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, 0, index->word.spelling);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, 0, index->word.wordClass);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, 0, index->word.meaning);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLE), WM_SETTEXT, 0, index->word.example);
		}

	}
	return TRUE;
}

BOOL VocabularyForm_OnQuizButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	// ���� ��ư�� Ŭ������ ��
	if (HIWORD(wParam) == BN_CLICKED) {
		//���� �����츦 ����Ѵ�.
		DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_QUIZFORM), NULL,
			QuizFormProc);
	}
	return TRUE;

}

BOOL VocabularyForm_OnCorrectQuizButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	// �ϱ�ܾ� ��ư�� Ŭ������ ��
	if (HIWORD(wParam) == BN_CLICKED) {
		//�ϱ�ܾ� �����츦 ����Ѵ�.
		DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_CORRECTQUIZFORM), NULL,
			CorrectQuizFormProc);
	}
	return TRUE;

}

BOOL VocabularyForm_OnPutImportantWordButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Vocabulary* vocabulary;
	WordCard* index;

	//1. �߿�ܾ� �߰� ��ư�� Ŭ������ ��
	if (HIWORD(wParam) == BN_CLICKED) {
		//1.1 ���� �ܾ�ī���� ��ġ�� �д´�.
		vocabulary = (Vocabulary*)GetWindowLong(hWnd, GWL_USERDATA);
		index = vocabulary->current;
		//1.2 �ܾ��忡�� �߿�ܾ�� �߰��Ѵ�.
		index = PutImportantWord(vocabulary, index);
		//1.3 ������������ �ܾ�ī�忡 '�߿�'�� ����Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ISIMPORTANT), WM_SETTEXT, 0, (LPARAM)index->quiz.isImportant);
	}
	return TRUE;
}

BOOL VocabularyForm_OnPutOutImportantWordButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Vocabulary* vocabulary;
	WordCard* index;

	//2. �߿�ܾ� ��� ��ư�� Ŭ������ ��
	if (HIWORD(wParam) == BN_CLICKED) {
		//2.1 �߿�ܾ� ��� ��ư�� Ŭ������ ��
		vocabulary = (Vocabulary*)GetWindowLong(hWnd, GWL_USERDATA);
		index = vocabulary->current;
		//2.2  �ܾ��忡�� �߿�ܾ� ����Ѵ�.
		index = PutOutImportantWord(vocabulary, index);
		//2.3 ���� �������� �ܾ�ī�忡 '�߿�'�� �����.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ISIMPORTANT), WM_SETTEXT, 0, (LPARAM)index->quiz.isImportant);
	}
	return TRUE;
}

BOOL VocabularyForm_OnImportantWordButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	// �߿�ܾ� ��ư�� Ŭ������ ��
	if (HIWORD(wParam) == BN_CLICKED) {
		// �߿�ܾ� �����츦 ����Ѵ�.
		DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_IMPORTANTWORDSFORM),
			NULL, ImportantWordsFormProc);
	}
	return TRUE;
}


BOOL VocabularyForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Vocabulary* vocabulary;
	IndexBinder* indexBinder;
	CorrectQuiz* correctQuiz;

	//10. �ݱ� ��ư�� Ŭ������ ��
	vocabulary = (Vocabulary*)GetWindowLong(hWnd, GWL_USERDATA);
	correctQuiz = (CorrectQuiz*)GetProp(hWnd, "correctQuiz");
	indexBinder = (IndexBinder*)GetProp(hWnd, "indexBinder");

	if (vocabulary != NULL) {

		//10.1 �����Ѵ�.
		Save(vocabulary);

		//10.2 �ܾ����� ���ش�.
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
		//10.1 �����Ѵ�.
		CorrectQuiz_Save(correctQuiz);

		//10.2 �ܾ����� ���ش�.
		CorrectQuiz_Destroy(correctQuiz);
		free(correctQuiz);
		correctQuiz = NULL;
	}

	RemoveProp(hWnd, "indexBinder");
	RemoveProp(hWnd, "correctQuiz");

	//10.3 �����츦 �����Ѵ�.
	EndDialog(hWnd, 0);
	return TRUE;
}
