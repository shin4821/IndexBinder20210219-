//index.h
#ifndef _INDEX_H
#define _INDEX_H
#include"array.h"
typedef struct _wordCard WordCard;
typedef struct _index {
	char alphabet;
	Array wordCards;
	Long capacity;
	Long length;
}Index;

void Index_Create(Index* index, Long capacity);
Long Index_Put(Index* index, WordCard* key);
Long Index_Find(Index* index, WordCard* key);
Long Index_PutOut(Index* index, Long index_);
void Index_Arrange(Index* index);
WordCard* Index_GetAt(Index* index, Long index_);
void Index_FindSpellings(Index* index, WordCard* key, Long* (*indexes), Long* count);
void Index_Destroy(Index* index);
int CompareWordCards(void* one, void* other);
int CompareWordCardSpellings(void* one, void* other);
int Index_CompareSpellings(void* one, void* other);

#endif//_INDEX_H
