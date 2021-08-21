//Index.c
#include"Index.h"
#include"Vocabulary.h"
#include<stdlib.h>

void Index_Create(Index* index, Long capacity) {
	Create(&index->wordCards, capacity, sizeof(Long));
	index->capacity = capacity;
	index->length = 0;
}

Long Index_Put(Index* index, WordCard* key) {
	Long index_;

	//1. key(�ܾ�ī�帵ũ)�� �Է¹޴´�.
	//2. length�� capacity���� ������ Store�� ȣ���Ѵ�.
	if (index->length < index->capacity) {
		index_ = index->length;
		Store(&index->wordCards, index_, &key, sizeof(WordCard*));
	}
	//3. length�� capacity���� ũ�ų� ������ AppendForRear�� ȣ���Ѵ�.
	else {
		index_ = AppendForRear(&index->wordCards, &key, sizeof(WordCard*));
		index->capacity++;
	}
	//4. length�� 1�� ������Ų��.
	index->length++;
	//5. ÷��(index)�� ����Ѵ�.
	return index_;
}

Long Index_Find(Index* index, WordCard* key) {
	Long i;
	i = LinearSearchUnique(&index->wordCards, key, sizeof(WordCard*), CompareWordCards);
	return i;
}

Long Index_PutOut(Index* index, Long index_) {
	Long i;

	i = Delete(&index->wordCards, index_, sizeof(WordCard*));
	index->capacity--;
	index->length--;

	return i;
}

void Index_Arrange(Index* index) {
	SelectionSort(&index->wordCards, sizeof(WordCard*), CompareWordCardSpellings);
}

WordCard* Index_GetAt(Index* index, Long index_) {
	WordCard* key;

	GetAt(&index->wordCards, index_, &key, sizeof(WordCard*));
	return key;
}

void Index_FindSpellings(Index* index, WordCard* key, Long* (*indexes), Long* count) {
	//1. key�� �Է¹޴´�.
	//2. LinearSearchDuplicate�� ȣ���Ѵ�.
	LinearSearchDuplicate(&index->wordCards, key, indexes, count, sizeof(WordCard*), Index_CompareSpellings);
	//3. indexes, count�� ����Ѵ�.
	//4. ������.
}

void Index_Destroy(Index* index) {
	Destroy(&index->wordCards);
}

int CompareWordCards(void* one, void* other) {
	WordCard** one_ = (WordCard**)one;
	WordCard* other_ = (WordCard*)other;
	int ret;

	if (*one_ == other_) {
		ret = 0;
	}
	else {
		ret = -1;
	}
	return ret;
}

int CompareWordCardSpellings(void* one, void* other) {
	WordCard** one_ = (WordCard**)one;
	WordCard** other_ = (WordCard**)other;
	int ret;

	ret = strcmp((*one_)->word.spelling, (*other_)->word.spelling);
	return ret;
}

int Index_CompareSpellings(void* one, void* other) {
	WordCard** one_ = (WordCard**)one;
	WordCard* other_ = (WordCard*)other;
	int ret;

	ret = strcmp((*one_)->word.spelling, other_->word.spelling);
	return ret;
}








