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

	//1. key(단어카드링크)를 입력받는다.
	//2. length가 capacity보다 작으면 Store를 호출한다.
	if (index->length < index->capacity) {
		index_ = index->length;
		Store(&index->wordCards, index_, &key, sizeof(WordCard*));
	}
	//3. length가 capacity보다 크거나 같으면 AppendForRear를 호출한다.
	else {
		index_ = AppendForRear(&index->wordCards, &key, sizeof(WordCard*));
		index->capacity++;
	}
	//4. length를 1씩 증가시킨다.
	index->length++;
	//5. 첨자(index)를 출력한다.
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
	//1. key를 입력받는다.
	//2. LinearSearchDuplicate를 호출한다.
	LinearSearchDuplicate(&index->wordCards, key, indexes, count, sizeof(WordCard*), Index_CompareSpellings);
	//3. indexes, count를 출력한다.
	//4. 끝낸다.
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








