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

	//1. index(�ܾ�ī�帵ũ)�� �Է¹޴´�.
	//2. �Է¹��� index�� indexCard�� �����.
	if (index->word.spelling[0] > 96 && index->word.spelling[0] < 123) {
		indexCard.alphabet = index->word.spelling[0] - 32;
	}
	else {
		indexCard.alphabet = index->word.spelling[0];
	}
	//3. ����ö�� �ش� ���ĺ� ������ �ִ��� Ȯ���Ѵ�.
	node = Search(&indexBinder->indexes, &indexCard, CompareAlphabets);
	//4. �ش� ���ĺ� ������ ���� ���, ����ö�� ������ �����ش�.
	if (node == NULL) {
		Index_Create(&indexCard, 10);
		node = BinaryTree_Insert(&indexBinder->indexes, &indexCard, sizeof(Index), CompareAlphabets);
		indexBinder->length++;
	}
	//5. �ش� ���ο� �ش� �ܾ �����.
	index_ = (Index*)(node + 1);
	Index_Put(index_, index);
	//6. index_�� ����Ѵ�.
	return index_;
}

Index* IndexBinder_Find(IndexBinder* indexBinder, char alphabet) {
	BinaryNode* node;
	Index indexCard;
	Index* index_;

	//1. ���ĺ��� �Է¹޴´�.
	//2. indexCard�� �����.
	if (alphabet > 96 && alphabet < 123) {
		indexCard.alphabet = alphabet - 32;
	}
	else {
		indexCard.alphabet = alphabet;
	}
	//3. Search ȣ���Ѵ�.
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

	//1. ���ĺ�, ���� �ܾ�ī�� ��ũ�� �Է¹޴´�.
	//2. indexCard�� �����.
	indexCard.alphabet = alphabet;
	//3. Search�� ȣ���Ͽ� ����ö ��, ������ ��ġ�� Ȯ���Ѵ�.
	node = Search(&indexBinder->indexes, &indexCard, CompareAlphabets);
	//4. �ش� ���ο��� ���� �ܾ�ī�� ��ũ�� ÷�ڰ� ���°���� Ȯ���Ѵ�.
	index_ = (Index*)(node + 1);
	i = Index_Find(index_, index);
	//5. �ش��°�� �ܾ�ī�带 �ش� ���ο��� �̾��ش�.
	Index_PutOut(index_, i);
	//6. �ܾ�ī�带 �̰� ��, length�� 0�̸� �ش� ���ε� ����ö���� �̴´�.
	if (index_->length <= 0) {
		BinaryTree_Delete(&indexBinder->indexes, &indexCard, sizeof(Index), CompareAlphabets);
		index_ = NULL;
		indexBinder->length--;
	}
	//7. index_�� ����Ѵ�.(������ ��ġ. Delete�Ǿ����� NULL)
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
	// indexes, count�� ȣ���Ѵ�.
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



