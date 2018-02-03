#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

const int MAX_CHARS = 40;
const double POISON = NAN;

struct tree_elem_t
{
    tree_elem_t* parent;
    tree_elem_t* left;
    tree_elem_t* right;
    double data;
    int type;
    int number;
};

struct tree_t
{
    tree_elem_t* root;
    int nElements;
};

enum arithmeticType
{
    NOTH = 0,
    #define  DEF_CMD(name, func, diff, rate)  name ,
    #include "commands.h"
    #undef   DEF_CMD

};


/*enum arithmeticType
{
    NOTH = 0,
    CONST,
    VAR,
    ADD,
    SUB,
    MULT,
    DIV   ,
    SQRT  ,
    SIN   ,
    COS   ,
    TAN   ,
    EXP   ,
    LN    ,

};*/


//int constructTree  (tree_t* rTree, char firstElemData [MAX_CHARS]);
int constructTree  (tree_t* rTree);

//working with a tree
int addElemLeft    (tree_t* rTree, tree_elem_t* parentElement, int type, double data);
int addElemLeft    (tree_t* rTree, tree_elem_t* parentElement, int type);
int addElemRight   (tree_t* rTree, tree_elem_t* parentElement, int type, double data);
int addElemRight   (tree_t* rTree, tree_elem_t* parentElement, int type);
int changeElemData (tree_t* rTree, tree_elem_t* element,       int type, double data);
int changeElemData (tree_t* rTree, tree_elem_t* element,       int type);
int destructElement(tree_t* rTree, tree_elem_t* element);


//functions for dump
int dumpWithDot    (tree_t* rTree);
int fPrintfElem    (tree_elem_t* element, FILE* ptrFile);
int fPrintfArrow   (tree_elem_t* element, FILE* ptrFile);

//math functions
int isNull         (double number);
#endif

