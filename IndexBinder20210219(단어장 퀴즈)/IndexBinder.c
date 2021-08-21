//IndexBinder.c
#include"IndexBinder.h"
#include"Index.h"
#include"Vocabulary.h"
#include<stdlib.h>
#include<string.h>
#pragma warning(disable:4996)
 
void IndexBinder_Create(IndexBinder* indexBinder) {
	BinaryTree_Create(&indexBinder->indexes);
	indexBinder->length = 0;
}

Index* IndexBinder_Put(IndexBinder* indexBinder, WordCard* index) {
	Index indexCard;
	BinaryNode* node;
	Index* index_;

	//1. index(단어카드링크)를 입력받는다.
	//2. 입력받은 index로 indexCard를 만든다.
	if (index->word.spelling[0] > 96 && index->word.spelling[0] < 123) {
		indexCard.alphabet = index->word.spelling[0] - 32;
	}
	else {
		indexCard.alphabet = index->word.spelling[0];
	}
	//3. 색인철에 해당 알파벳 색인이 있는지 확인한다.
	node = Search(&indexBinder->indexes, &indexCard, CompareAlphabets);
	//4. 해당 알파벳 색인이 없는 경우, 색인철에 색인을 끼워준다.
	if (node == NULL) {
		Index_Create(&indexCard, 10);
		node = BinaryTree_Insert(&indexBinder->indexes, &indexCard, sizeof(Index), CompareAlphabets);
		indexBinder->length++;
	}
	//5. 해당 색인에 해당 단어를 끼운다.
	index_ = (Index*)(node + 1);
	Index_Put(index_, index);
	//6. index_를 출력한다.
	return index_;
}

Index* IndexBinder_Find(IndexBinder* indexBinder, char alphabet) {
	BinaryNode* node;
	Index indexCard;
	Index* index_;

	//1. 알파벳을 입력받는다.
	//2. indexCard를 만든다.
	if (alphabet > 96 && alphabet < 123) {
		indexCard.alphabet = alphabet - 32;
	}
	else {
		indexCard.alphabet = alphabet;
	}
	//3. Search 호출한다.
	node = Search(&indexBinder->indexes, &indexCard, CompareAlphabets);
	if (node != NULL) {
		index_ = (Index*)(node + 1);
	}
	else {
		index_ = NULL;
	}
	return index_;
}

Index* IndexBinder_PutOut(IndexBinder* indexBinder, char alphabet, WordCard* index) {
	Index indexCard;
	BinaryNode* node;
	Index* index_;
	Long i;

	//1. 알파벳, 뽑힌 단어카드 링크를 입력받는다.
	//2. indexCard를 만든다.
	indexCard.alphabet = alphabet;
	//3. Search를 호출하여 색인철 상, 색인의 위치를 확인한다.
	node = Search(&indexBinder->indexes, &indexCard, CompareAlphabets);
	//4. 해당 색인에서 뽑힌 단어카드 링크의 첨자가 몇번째인지 확인한다.
	index_ = (Index*)(node + 1);
	i = Index_Find(index_, index);
	//5. 해당번째의 단어카드를 해당 색인에서 뽑아준다.
	Index_PutOut(index_, i);
	//6. 단어카드를 뽑고난 후, length가 0이면 해당 색인도 색인철에서 뽑는다.
	if (index_->length <= 0) {
		BinaryTree_Delete(&indexBinder->indexes, &indexCard, sizeof(Index), CompareAlphabets);
		index_ = NULL;
		indexBinder->length--;
	}
	//7. index_를 출력한다.(색인의 위치. Delete되었으면 NULL)
	return index_;
}

void Node_Arrange(BinaryNode* node) {
	Index* index_;

	if (node != NULL) {
		Node_Arrange(node->left);
		index_ = (Index*)(node + 1);
		Index_Arrange(index_);
		Node_Arrange(node->right);
	}
}

void IndexBinder_Arrange(IndexBinder* indexBinder) {
	MakeBalance(&indexBinder->indexes, sizeof(Index));
	Node_Arrange(indexBinder->indexes.root);
}

void MakeList(IndexBinder* indexBinder, Index* (*indexes), Long* count){
	MakeKeys(&indexBinder->indexes, indexes, count, sizeof(Index));
	// indexes, count를 호출한다.
}

void IndexBinder_Destroy(IndexBinder* indexBinder) {
	BinaryTree_Destroy(&indexBinder->indexes);
}

int CompareAlphabets(void* one, void* other) {
	Index* one_ = (Index*)one;
	Index* other_ = (Index*)other;
	int ret;

	if (one_->alphabet == other_->alphabet) {
		ret = 0;
	}
	else if (one_->alphabet > other_->alphabet) {
		ret = 1;
	}
	else if (one_->alphabet < other_->alphabet) {
		ret = -1;
	}

	return ret;
}



