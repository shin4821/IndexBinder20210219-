//Vocabulary.h
#ifndef _VOCABULARY_H
#define _VOCABULARY_H
#include"LinkedList.h"
typedef signed long Long;
typedef struct _word {
	char spelling[32];
	char wordClass[16];
	char meaning[32];
	char example[128];
}Word;
typedef struct _quiz {
	char spelling[32];
	char answer[32];
	char ox[32];
	char meaning[32];
	char isImportant[5];
}Quiz;

typedef struct _wordCard {
	Word word;
	Quiz quiz;
}WordCard;

typedef struct _vocabulary { // 틀린 문제 또는 안푼 문제
	LinkedList wordCards;
	Long length;
	WordCard* current;
}Vocabulary;

typedef struct _correctQuiz { // 맞은 문제
	LinkedList wordCards;
	Long length;
	WordCard* current;
}CorrectQuiz;


void Vocabulary_Create(Vocabulary* vocabulary);
Long Load(Vocabulary* vocabulary);
WordCard* Vocabulary_Put(Vocabulary* vocabulary, WordCard wordCard);
void FindBySpelling(Vocabulary* vocabulary, char(*spelling), WordCard** (*indexes), Long* count);
void FindByMeaning(Vocabulary* vocabulary, char(*meaning), WordCard** (*indexes), Long* count);
WordCard* FindSpelling(Vocabulary* vocabulary, char(*spelling));
WordCard* Vocabulary_First(Vocabulary* vocabulary);
WordCard* Vocabulary_Next(Vocabulary* vocabulary);
WordCard* Vocabulary_Previous(Vocabulary* vocabulary);
WordCard* Vocabulary_Last(Vocabulary* vocabulary);
WordCard* Move(Vocabulary* vocabulary, WordCard* index);
WordCard Vocabulary_PutOut(Vocabulary* vocabulary, WordCard* index);
WordCard* PutImportantWord(Vocabulary* vocabulary, WordCard* index);
WordCard* PutOutImportantWord(Vocabulary* vocabulary, WordCard* index);
void FindImportantWords(Vocabulary* vocabulary, WordCard** (*indexes), Long* count);
WordCard* Vocabulary_SpellingQuiz(Vocabulary* vocabulary, char(*meaning), WordCard* index);
WordCard* NextQuiz(Vocabulary* vocabulary);
WordCard Vocabulary_GetAt(Vocabulary* vocabulary, WordCard* index);
Long Save(Vocabulary* vocabulary);
void Vocabulary_Destroy(Vocabulary* vocabulary);



void CorrectQuiz_Create(CorrectQuiz* correctQuiz);
Long CorrectQuiz_Load(CorrectQuiz* correctQuiz);
WordCard* CorrectQuiz_Put(CorrectQuiz* correctQuiz, WordCard wordCard);
void CorrectQuiz_FindBySpelling(CorrectQuiz* correctQuiz,char(*spelling), WordCard** (*indexes), 
	Long* count);
void CorrectQuiz_FindByMeaning(CorrectQuiz* correctQuiz, char(*meaning), WordCard** (*indexes),
	Long* count);
WordCard* CorrectQuiz_FindSpelling(CorrectQuiz* correctQuiz, char(*spelling));
WordCard* CorrectQuiz_First(CorrectQuiz* correctQuiz);
WordCard* CorrectQuiz_Next(CorrectQuiz* correctQuiz);
WordCard* CorrectQuiz_Previous(CorrectQuiz* correctQuiz);
WordCard* CorrectQuiz_Last(CorrectQuiz* correctQuiz);
WordCard* CorrectQuiz_Move(CorrectQuiz* correctQuiz, WordCard* index);
WordCard CorrectQuiz_PutOut(CorrectQuiz* correctQuiz, WordCard* index);
#if 0
void CorrectQuiz_FindSolvedQuiz(CorrectQuiz* correctQuiz, WordCard** (*indexes), Long* count);
void CorrectQuiz_FindIncorrectQuiz(CorrectQuiz* correctQuiz, WordCard** (*indexes), Long* count);
void CorrectQuiz_FindImportantWords(CorrectQuiz* correctQuiz, WordCard** (*indexes), Long* count);
WordCard* CorrectQuiz_PutImportantWord(CorrectQuiz* correctQuiz,WordCard* index);
WordCard* CorrectQuiz_PutOutImportantWord(CorrectQuiz* correctQuiz, WordCard* index);
#endif
void CorrectQuiz_Reset(CorrectQuiz* correctQuiz);
WordCard CorrectQuiz_GetAt(CorrectQuiz* correctQuiz, WordCard* index);
Long CorrectQuiz_Save(CorrectQuiz* correctQuiz);
void CorrectQuiz_Destroy(CorrectQuiz* correctQuiz);
int CompareSpellings(void* one, void* other);
int CompareMeanings(void* one, void* other);
int Vocabulary_CompareWordCards(void* one, void* other);
int CompareOx(void* one, void* other);
int CompareIsImportant(void* one, void* other);

#endif//_VOCABULARY_H










