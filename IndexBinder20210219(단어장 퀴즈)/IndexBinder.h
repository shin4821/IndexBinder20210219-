//IndexBinder.h
#ifndef _INDEXBINDER_H
#define _INDEXBINDER_H
#include"BinaryTree.h"
typedef struct _wordCard WordCard;
typedef struct _index Index;
typedef struct _indexBinder {
	BinaryTree indexes;
	Long length;
}IndexBinder;

void IndexBinder_Create(IndexBinder* indexBinder);
Index* IndexBinder_Put(IndexBinder* indexBinder, WordCard* index);
Index* IndexBinder_Find(IndexBinder* indexBinder, char alphabet);
Index* IndexBinder_PutOut(IndexBinder* indexBinder, char alphabet, WordCard* index);
void IndexBinder_Arrange(IndexBinder* indexBinder);
void MakeList(IndexBinder* indexBinder, Index*(*indexes), Long* count);
void IndexBinder_Destroy(IndexBinder* indexBinder);
int CompareAlphabets(void* one, void* other);
void Node_Arrange(BinaryNode* node); // IndexBinder_Arrange할때 각 색인의 배열마다 Index_Arrange위해 재귀만듦.

#endif//_INDEXBINDER_H
