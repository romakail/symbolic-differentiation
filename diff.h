#ifndef DIFF_H_INCLUDED
#define DIFF_H_INCLUDED

#include "tree.h"

//
int differenciate ();

// making a pervoobraznaya tree
int  convertFileToTree     (tree_t* rTree, char* fileName);
int  createTreeElement     (tree_t* rTree, tree_elem_t* element, FILE* ptrFile);
char skipGetChar           (FILE* ptrFile);
int  readBetweenQuots      (FILE* ptrFile, char* phrase);

//
int copyNode               (tree_t* copyTree, tree_elem_t* copiedNode, tree_elem_t* copyNode);
int differenciateNode      (tree_t* diffTree, tree_elem_t* protNode, tree_elem_t* diffNode);

// simplifications
int simplifications        (tree_t* rTree);
int constantSimplification (tree_t* rTree, tree_elem_t* element);

// Teh
int dumpWithTex            (tree_t* rTree);
int texPrintElement        (FILE* ptrFile, tree_elem_t* element);
int funcRate               (int func);
/*
int multZeroSimplification (tree_t* rTree);
int zeroMult               (tree_t* rTree, tree_elem_t* element);
int multOneSimplification  (tree_t* rTree);
int oneMult                (tree_t* rTree, tree_elem_t* element);
int zeroDivSimplification  (tree_t* rTree);
int zeroDiv                (tree_t* rTree, tree_elem_t* element);
int oneDivSimplification   (tree_t* rTree);
int oneDiv                 (tree_t* rTree, tree_elem_t* element);
*/

#define DEF_SIMP(name, func)                                    \
int name                 (tree_t* rTree, tree_elem_t* element); \
int simplification##name (tree_t* rTree);

#include "simplifications.h"

#undef DEF_SIMP

#endif





