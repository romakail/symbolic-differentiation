#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cctype>
#include <assert.h>
#include <cmath>

#include "diff.h"

int differenciate ()
{
     tree_t rTree = {};
    constructTree (&rTree);
    tree_t rDiff = {};
    constructTree (&rDiff);

    convertFileToTree (&rTree, "formula.txt");
    dumpWithDot    (&rTree);

    differenciateNode(&rDiff, rTree.root, rDiff.root);

    dumpWithDot   (&rDiff);

    simplifications (&rDiff);

    dumpWithDot   (&rDiff);

    dumpWithTex  (&rDiff);
    return 0;
}
//---------------------------------------------------------------------------------------------------------

int simplifications (tree_t* rTree)
{
    constantSimplification(rTree, rTree->root);
    printf ("\nI have simplificated\n");

    int changes = 1;
    while (changes != 0)
    {
        changes = 0;
        #define DEF_SIMP(name, func)\
        changes = changes + simplification##name   (rTree);

        #include "simplifications.h"
        #undef DEF_SIMP

    }
    constantSimplification (rTree, rTree->root);
    printf ("\nI have simplificated\n");

    return 0;
}

//---------------------------------------------------------------------------------------------------------

int convertFileToTree (tree_t* rTree, char* fileName)
{
    FILE* ptrFile = fopen (fileName, "r");
    assert (ptrFile);
    rewind (ptrFile);

    createTreeElement (rTree, rTree->root, ptrFile);

    fclose (ptrFile);
    printf ("\nI have read everything is ok\n");
    return 0;
}

//------------------------------------------------------------------------------------------------

int createTreeElement  (tree_t* rTree, tree_elem_t* element, FILE* ptrFile)   // S2 14.11.17 00:19
{
    char data [MAX_CHARS];
    char symbol;
    int    type;
    double digit;

    symbol = skipGetChar (ptrFile);

    if (symbol == '(')
    {
        //readBetweenQuots(ptrFile, data);
        //changeElemData(rTree, element, data);

        type = readBetweenQuots(ptrFile, data);
        printf ("\ntype is : %d\n", type);
        if (type == CONST)
        {
            fscanf (ptrFile, "%lg", &digit);
            printf ("\n%lg\n", digit);
            fscanf (ptrFile, "%c" , &symbol);

            printf ("\nsymbol is %c\n", symbol);
            assert (symbol == '"');

            changeElemData(rTree, element, type, digit);
        }
        else
        {
            changeElemData(rTree, element, type);
        }

        symbol = skipGetChar (ptrFile);
        fseek (ptrFile, -1, SEEK_CUR);
        if (symbol == '(')
        {
            addElemLeft   (rTree, element, 0);
            createTreeElement (rTree, element->left, ptrFile);

            addElemRight  (rTree, element, 0);
            createTreeElement(rTree, element->right, ptrFile);
        }
    }
    else
    {
        printf ("\nProblems with reading \"(\"\n");
        printf ("I`ve read \"%c\"", symbol);
        assert (0);
    }

    symbol = skipGetChar(ptrFile);
    //printf ("--%c\n", symbol);
    return 0;
}

//------------------------------------------------------------------------------------

char skipGetChar (FILE* ptrFile)
{
#define phil
#ifdef phil
    char symbol = '\0';
    fscanf(ptrFile, " %c", &symbol);
#else
    char symbol = '\0';
    fscanf (ptrFile, "%c", &symbol);

    while ((symbol == '\n') || (symbol == '\t') || (symbol == ' '))
    {
        fscanf (ptrFile, "%c", &symbol);
    }
#endif

    return symbol;
}

//---------------------------------------------------------------------------------------

int readBetweenQuots (FILE* ptrFile, char* phrase)
{
    char symbol = '\0';
    symbol = skipGetChar (ptrFile);

    if (symbol == '"')
    {
        symbol = skipGetChar(ptrFile);
        fseek (ptrFile, -1, SEEK_CUR);
        printf ("\n---%c--\n", symbol);
        if ( isdigit (symbol) )
        {
            printf ("I am returning a digit");
            return 1;
        }
        else
        {
            int counter = 0;
            while (symbol != '"')
            {
                symbol = skipGetChar (ptrFile);
                //fscanf (ptrFile, "%c", &symbol);
                phrase [counter] = symbol;
                counter++;
            }
            phrase [counter-1] = '\0';

            if (strcmp (phrase, "noth") == 0)
                return NOTH;
            #define DEF_CMD(name, func, diff, rate) else if (strcmp (phrase, #func) == 0) \
                                                return name;

            #include "commands.h"
            #undef DEF_CMD


            /*if      (strcmp (phrase, "x")    == 0)
                return VAR;

            else if (strcmp (phrase, "+")    == 0)
                return ADD;

            else if (strcmp (phrase, "-")    == 0)
                return SUB;

            else if (strcmp (phrase, "*")    == 0)
                return MULT;

            else if (strcmp (phrase, "/")    == 0)
                return DIV;

            else if (strcmp (phrase, "sqrt") == 0)
                return SQRT;

            else if (strcmp (phrase, "sin")  == 0)
                return SIN;

            else if (strcmp (phrase, "cos")  == 0)
                return COS;

            else if (strcmp (phrase, "tan")  == 0)
                return TAN;

            else if (strcmp (phrase, "exp")  == 0)
                return EXP;

            else if (strcmp (phrase, "ln")   == 0)
                return LN;*/

            else
            {
                printf ("\nI am here\n");
                printf ("problem with \"%s\"\n", phrase);
                return -1;
            }
            //return counter;
        }
    }
    else
    {
        printf ("\nERROR, haven`t found '\"'\n");
        assert (0);
        return -1;
    }
}

//---------------------------------------------------------------------------------

int copyNode (tree_t* resultTree, tree_elem_t* copiedNode, tree_elem_t* resultNode)
{

    resultNode->type = copiedNode->type;
    resultNode->data = copiedNode->data;

    if (copiedNode->left != NULL)
    {
        addElemLeft  (resultTree, resultNode, 0);
        copyNode     (resultTree, copiedNode->left , resultNode->left );
    }
    else
        resultNode->left = NULL;

    if (copiedNode->right != NULL)
    {
        addElemRight (resultTree, resultNode, 0);
        copyNode     (resultTree, copiedNode->right, resultNode->right);
    }
    else
        resultNode->right = NULL;
    return 0;
}

//---------------------------------------------------------------------------------

int differenciateNode (tree_t* diffTree, tree_elem_t* protNode, tree_elem_t* diffNode)
{
    switch (protNode->type)
    {

    #define DEF_CMD(name, func, diff, rate)       \
        case name : diff                    \
                    break;

    #include "commands.h"
    #undef DEF_CMD

        default :    printf ("Problems with switch case");
                     assert (0);
    }
    return 0;
}






       /* case CONST : diffNode->type  = CONST;
                     diffNode->data  = 0;
                     diffNode->left  = NULL;
                     diffNode->right = NULL;
                     break;

        case VAR   : diffNode->type  = CONST;
                     diffNode->data  = 1;
                     diffNode->left  = NULL;
                     diffNode->right = NULL;
                     break;

        case ADD   : diffNode->type  = ADD;
                     diffNode->data  = POISON;

                     addElemLeft(diffTree, diffNode, 0);
                     differenciateNode (diffTree, protNode->left, diffNode->left);

                     addElemRight(diffTree, diffNode, 0);
                     differenciateNode (diffTree, protNode->right, diffNode->right);
                     break;

        case SUB   : diffNode->type  = SUB;
                     diffNode->data  = POISON;

                     addElemLeft(diffTree, diffNode, 0);
                     differenciateNode (diffTree, protNode->left, diffNode->left);

                     addElemRight(diffTree, diffNode, 0);
                     differenciateNode (diffTree, protNode->right, diffNode->right);
                     break;

        case MULT  : diffNode->type  = ADD;
                     diffNode->data  = POISON;

                     addElemLeft      (diffTree, diffNode, MULT);
                     addElemRight     (diffTree, diffNode, MULT);

                     addElemLeft      (diffTree, diffNode->left , 0);
                     addElemRight     (diffTree, diffNode->left , 0);
                     addElemLeft      (diffTree, diffNode->right, 0);
                     addElemRight     (diffTree, diffNode->right, 0);

                     copyNode         (diffTree, protNode->left , diffNode->left ->left );
                     differenciateNode(diffTree, protNode->right, diffNode->left ->right);

                     copyNode         (diffTree, protNode->right, diffNode->right->left );
                     differenciateNode(diffTree, protNode->left , diffNode->right->right);
                     break;*/

//------------------------------------------------------------------------------------------------------------------

int constantSimplification (tree_t* rTree, tree_elem_t* element)
{
    if (element->left  != NULL)
        constantSimplification(rTree, element->left );

    if (element->right != NULL)
        constantSimplification(rTree, element->right);

    if (element->left != NULL)
    {
        if (element->right != NULL)
        {
            if ((element->left ->type == CONST) && (element->right->type == CONST))
            {
                printf ("\nI am starting to switch1\n");
                printf ("type is %d\n", element->type);
                printf ("element left  type is %d\n" , element->left ->type);
                printf ("element right type is %d\n" , element->right->type);
                printf ("element left  data is %lg\n", element->left ->data);
                printf ("element right data is %lg\n", element->right->data);

                switch (element->type)
                {
                    case ADD  : element->data = element->left->data + element->right->data;
                               break;

                    case SUB  : element->data = element->left->data - element->right->data;
                               break;

                    case MULT : element->data = (element->left->data) * (element->right->data);
                                printf ("I am multiplying");
                               break;

                    case DIV  : element->data = element->left->data / element->right->data;
                               break;

                    default   : printf ("\n problems, elemen->type is %d", element->type);
                               break;
                }
                element->type  = CONST;
                destructElement(rTree, element->left );
                destructElement(rTree, element->right);
            }
        }
        else if (element->right == NULL)
        {
            if (element->left ->type == CONST)
            {
                printf ("\nI am starting to switch1\n");
                switch (element->type)
                {
                    case COS : element->data = cos(element->left->data);
                              break;

                    case SIN : element->data = sin(element->left->data);
                              break;

                    case TAN : element->data = tan(element->left->data);
                              break;

                    case LN  : element->data = log(element->left->data);
                              break;

                    case EXP : element->data = exp(element->left->data);
                              break;

                    default   : printf ("\n problems, elemen->type is %d", element->type);
                               break;
                }
                element->type = CONST;
                destructElement(rTree, element->left);
            }
        }
        else
        {
            printf ("I don`t know what could happen");
            assert (0);
        }
    }
    printf ("\nI am doing something\n");
    return 0;
}

//----------------------------------------------------------------------------------------------------------

#define DEF_SIMP(name, func)                    \
int simplification##name (tree_t* rTree)        \
{                                               \
    int changes = 0;                            \
    while ( name (rTree, rTree->root) == 1)     \
        changes = 1;                            \
    return changes;                             \
}

#include "simplifications.h"
#undef DEF_SIMP

//---------------------------------------------------------------------------------------------------------

#define DEF_SIMP(name, func)                        \
int name (tree_t* rTree, tree_elem_t* element)      \
{                                                   \
    bool flag = 0;                                  \
                                                    \
    if (element->left  != NULL)                     \
    {                                               \
        if (name (rTree, element->left) == 1)       \
            flag = 1;                               \
    }                                               \
                                                    \
    if (element->right != NULL)                     \
    {                                               \
        if (name (rTree, element->right) == 1)      \
            flag = 1;                               \
    }                                               \
                                                    \
    func                                            \
                                                    \
    return flag;                                    \
}

#include "simplifications.h"
#undef  DEF_SIMP

//-----------------------------------------------------------------------------------------------
/*
int multZeroSimplification(tree_t* rTree)
{
    int changes = 0;
    while (zeroMult(rTree, rTree->root) == 1)
        changes = 1;
    return changes;
}

//---------------------------------------------------------------------------------------------

int zeroMult (tree_t* rTree, tree_elem_t* element)
{
    bool flag = 0;

    if (element->left  != NULL)
    {
        if (zeroMult (rTree, element->left) == 1)
            flag = 1;
    }

    if (element->right != NULL)
    {
        if (zeroMult (rTree, element->right) == 1)
            flag = 1;
    }

    if ((element->type == MULT) &&
       (((element->left ->type == CONST) && (isNull (element->left ->data) == 0))  ||
       (( element->right->type == CONST) && (isNull (element->right->data) == 0))))
    {
        printf ("\nI am multiplying in zero\n");
        element->type  = CONST;
        element->data  = 0;
        destructElement (rTree, element->left );
        destructElement (rTree, element->right);
        flag = 1;
    }
    return flag;
}

//--------------------------------------------------------------------------------------------------------------------

int multOneSimplification(tree_t* rTree)
{
    int changes = 0;
    while (oneMult(rTree, rTree->root) == 1)
        changes = 1;
    return changes;
}

//------------------------------------------ -----------------------------------------------------------------------------

int oneMult (tree_t* rTree, tree_elem_t* element)
{
    bool flag = 0;

    if (element->left  != NULL)
    {
        if (oneMult (rTree, element->left ) == 1)
            flag = 1;
    }

    if (element->right != NULL)
    {
        if (oneMult (rTree, element->right) == 1)
            flag = 1;
    }

    if ((element->type == MULT) &&
       ((element->left ->type == CONST) && (isNull (element->left ->data - 1) == 0)))
    {
        printf   ("\nI am multiplying on 1\n");
        copyNode (rTree, element->right,  element);
        flag = 1;
    }

    if ((element->type == MULT) &&
       ((element->right->type == CONST) && (isNull (element->right->data - 1) == 0)))
    {
        printf   ("\nI am multiplying on 1\n");
        copyNode (rTree, element->left, element);
        flag = 1;
    }
    return flag;
}

//---------------------------------------------------------------------------------------------------------------------

int zeroDivSimplification (tree_t* rTree)
{
    int changes = 0;
    while (zeroDiv (rTree, rTree->root) == 1)
        changes = 1;
    return changes;
}

//--------------------------------------------------------------------------------------------------------------------

int zeroDiv (tree_t* rTree, tree_elem_t* element)
{
    bool flag = 0;

    if (element->left  != NULL)
    {
        if (zeroDiv (rTree, element->left) == 1)
            flag = 1;
    }

    if (element->right != NULL)
    {
        if (zeroDiv (rTree, element->right) == 1)
            flag = 1;
    }

    if ((element->type == DIV) && (element->left->type == CONST) && (isNull (element->left->data) == 0))
    {
        printf ("\nI am diversing zero on smt\n");
        element->type  = CONST;
        element->data  = 0;
        destructElement (rTree, element->left );
        destructElement (rTree, element->right);
        flag = 1;
    }
    return flag;
}

//------------------------------------------------------------------------------------------------------------------

int oneDivSimplification (tree_t* rTree)
{
    int changes = 0;
    while (oneDiv (rTree, rTree->root) == 1)
        changes = 1;
    return changes;
}

//--------------------------------------------------------------------------------------------------------------------

int oneDiv (tree_t* rTree, tree_elem_t* element)
{
    bool flag = 0;

    if (element->left  != NULL)
    {
        if (oneDiv (rTree, element->left) == 1)
            flag = 1;
    }

    if (element->right != NULL)
    {
        if (oneDiv (rTree, element->right) == 1)
            flag = 1;
    }

    if ((element->type == DIV) && (element->right->type == CONST) && (isNull (element->right->data - 1) == 0))
    {
        printf ("\nI am diversing on 1\n");
        copyNode (rTree, element->left, element);
        flag = 1;
        element->type  = CONST;
        element->data  = 0;
        destructElement (rTree, element->left );
        destructElement (rTree, element->right);
        flag = 1;
    }
    return flag;
}
*/

/*int zeroMult (tree_elem_t* element)
{
    if (element->left  != NULL)
        zeroMult (element->left );

    if (element->right != NULL)
        zeroMult (element->right);


}*/

//--------------------------------------------------------------------------------------------------------------------

int dumpWithTex (tree_t* rTree)
{
    FILE* ptrFile = fopen ("texDump", "w");
    assert (ptrFile);
    rewind (ptrFile);

    fprintf (ptrFile, "\\documentclass{article}\n");
    fprintf (ptrFile, "\\begin{document}\n\n$\n\t");

    texPrintElement(ptrFile, rTree->root);

    fprintf (ptrFile, "\n$\n");
    fprintf (ptrFile, "\\end{document}");

    fclose (ptrFile);

    system ("latex texDump");
    system ("evince texDump.dvi");
    return 0;

}

//--------------------------------------------------------------------------------------------------------------------

int texPrintElement (FILE* ptrFile, tree_elem_t* element)
{
    if (element->type == CONST)
    {
        fprintf (ptrFile, "%lg", element->data);
    }
    else if (element->type == VAR)
    {
        fprintf (ptrFile, "x");
    }
    else if (   (element->type == ADD)  ||
                (element->type == SUB)  ||
                (element->type == MULT)
            )
    {
        if (element->left != NULL)
        {
            if (funcRate (element->type) > funcRate(element->left->type))
                fprintf (ptrFile, "(");

            texPrintElement (ptrFile, element->left);

            if (funcRate (element->type) > funcRate(element->left->type))
                fprintf (ptrFile, ")");
        }
        if (0);
        #define DEF_CMD(name, func, diff, rate)\
        else if (element->type == name)\
        {\
            fprintf (ptrFile, #func);\
        }

        #include "commands.h"
        #undef DEF_CMD
        {
            if (funcRate (element->type) > funcRate(element->right->type))
                fprintf (ptrFile, "(");

            texPrintElement (ptrFile, element->right);

            if (funcRate (element->type) > funcRate(element->right->type))
                fprintf (ptrFile, ")");
        }
    }
    else if (element->type == DIV)
    {
        fprintf (ptrFile, "\\frac{");
        texPrintElement (ptrFile, element->left);
        fprintf (ptrFile, "} {");
        texPrintElement (ptrFile, element->right);
        fprintf (ptrFile, "}");
    }
    else if (element->type == COS)
    {
        fprintf (ptrFile, "cos(");
        texPrintElement (ptrFile, element->left);
        fprintf (ptrFile, ")");
    }
    else if (element->type == SIN)
    {
        fprintf (ptrFile, "sin(");
        texPrintElement (ptrFile, element->left);
        fprintf (ptrFile, ")");
    }
    else if (element->type == SQRT)
    {
        fprintf (ptrFile, "\\sqrt{");
        texPrintElement (ptrFile, element->left);
        fprintf (ptrFile, "}");
    }
    else if (element->type == EXP)
    {
        fprintf (ptrFile, "e^{");
        texPrintElement (ptrFile, element->left);
        fprintf (ptrFile, "}");
    }
    else if (element->type == POW)
    {
        fprintf (ptrFile, "{");
        texPrintElement (ptrFile, element->left );
        fprintf (ptrFile, "}^{");
        texPrintElement (ptrFile, element->right);
        fprintf (ptrFile, "}");
    }
    else if (element->type == LN)
    {
        fprintf (ptrFile, "ln(");
        texPrintElement (ptrFile, element->left );
        fprintf (ptrFile, ")");
    }
    return 0;
}

//------------------------------------------------------------------------------------------------------------------

int funcRate (int func)
{
    switch (func)
    {
    #define DEF_CMD(name, func, diff, rate)     \
    case name : return rate;                    \
        break;

    #include "commands.h"
    #undef DEF_CMD
    }
}














