#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cctype>
#include <assert.h>
#include <cmath>

#include "tree.h"


//-----------------------------------------------------------------------------------------

int constructTree (tree_t* rTree)
{
    tree_elem_t* firstElement = (tree_elem_t*) calloc (1, sizeof (tree_elem_t));
    assert (firstElement);

    rTree->root      = firstElement;
    rTree->nElements = 1;

    firstElement->parent = NULL;
    firstElement->left   = NULL;
    firstElement->right  = NULL;
    firstElement->type   = -1;
    firstElement->data   = POISON;

    //strcpy (firstElement->data, firstElemData);

    return 0;
}

//---------------------------------------------------------------------------------------

int addElemLeft (tree_t* rTree, tree_elem_t* parentElement, int type, double data)
{
    assert (type >= 0);
    tree_elem_t* addedElement = (tree_elem_t*) calloc (1, sizeof (tree_elem_t));
    assert (addedElement);

    parentElement->left = addedElement;

    addedElement->parent = parentElement;
    addedElement->left   = NULL;
    addedElement->right  = NULL;

    addedElement->type   = type;
    if (type == CONST)
        addedElement->data = data;
    else
    {
        printf ("\nwhat`s wrong with you?\n");
        printf ("it`s not a good idea to give data to this function\n");
        addedElement->data = POISON;
    }



    rTree->nElements++;
    return 0;
}



int addElemLeft (tree_t* rTree, tree_elem_t* parentElement, int type)
{
    /*printf ("\n type is %d\n", type);
    dumpWithDot(rTree);*/
    assert (type >= 0);
    tree_elem_t* addedElement = (tree_elem_t*) calloc (1, sizeof (tree_elem_t));
    assert (addedElement);

    parentElement->left = addedElement;

    addedElement->parent = parentElement;
    addedElement->left   = NULL;
    addedElement->right  = NULL;

    addedElement->type   = type;
    addedElement->data   = POISON;
    if (type == CONST)
    {
        printf ("\nI have no data!!!\n");
        assert (0);
    }


    rTree->nElements++;
    return 0;
}

//----------------------------------------------------------------------------------------

int addElemRight (tree_t* rTree, tree_elem_t* parentElement, int type, double data)
{
    assert (type >= 0);
    tree_elem_t* addedElement = (tree_elem_t*) calloc (1, sizeof (tree_elem_t));
    assert (addedElement);

    parentElement->right = addedElement;

    addedElement->parent = parentElement;
    addedElement->left   = NULL;
    addedElement->right  = NULL;

    addedElement->type   = type;
    if (type == CONST)
        addedElement->data = data;
    else
    {
        printf ("\nwhat`s wrong with you?\n");
        printf ("it`s not a good idea togive data to this function\n");
        addedElement->data   = POISON;
    }

    rTree->nElements++;
    return 0;
}


int addElemRight (tree_t* rTree, tree_elem_t* parentElement, int type)
{
    assert (type >= 0);
    tree_elem_t* addedElement = (tree_elem_t*) calloc (1, sizeof (tree_elem_t));
    assert (addedElement);

    parentElement->right = addedElement;

    addedElement->parent = parentElement;
    addedElement->left   = NULL;
    addedElement->right  = NULL;

    addedElement->type   = type;
    addedElement->data   = POISON;
    if (type == CONST)
    {
        printf ("\nI have no data!!!\n");
        assert (0);
    }

    rTree->nElements++;
    return 0;
}

//-----------------------------------------------------------------------------------------

int changeElemData (tree_t* rTree, tree_elem_t* element, int type, double data)
{
    assert (type >= 0);
    element->type   = type;
    if (type == CONST)
        element->data = data;
    else
    {
        printf ("\nwhat`s wrong with you?\n");
        printf ("it`s not a good idea togive data to this function\n");
        element->data   = POISON;
    }
    return 0;
}

int changeElemData (tree_t* rTree, tree_elem_t* element, int type)
{
    assert (type >= 0);
    element->type   = type;
    if (type != CONST)
    {
        element->type = type;
        element->data = POISON;
    }
    else
    {
        printf ("\nI have no type\n");
        assert (0);
    }
    return 0;
}

//-----------------------------------------------------------------------------------------

int dumpWithDot (tree_t* rTree)
{
    tree_elem_t* ptr = rTree->root;

    FILE* ptrFile = fopen ("Dump.txt", "w");
    assert (ptrFile);
    rewind (ptrFile);

    fprintf (ptrFile, "digraph tree{\n");
    fprintf (ptrFile, "rankdir=HR;\n");
    fprintf (ptrFile, "head [label=\"nElements : %d\"];\n", rTree->nElements);

    fPrintfElem  ( ptr, ptrFile);
    fPrintfArrow ( ptr, ptrFile);

    fprintf (ptrFile, "}");
    fclose (ptrFile);

    system ("dot -Tpng Dump.txt -o Dump.png");
    system ("xdot Dump.txt");

    return 0;
}

//------------------------------------------------------------------------------------------

int fPrintfElem (tree_elem_t* element, FILE* ptrFile)
{
    static int i = 0;

    if (element->left != NULL)
    {
        fPrintfElem (element->left, ptrFile);
        //printf ("I`m connecting %d\n", i);
        //fprintf (ptrFile, "elem%d:<l>->elem%d;\n", element->number, element->left->number);
    }

    i++;
    element->number = i;


    if (element->type == NOTH)
    {
        fprintf (ptrFile, "elem%d [label=\"NOTH\"]", i);
    }
    else if (element->type == CONST)
    {
        fprintf (ptrFile, "elem%d [label=\"%lg\"]", i, element->data);
    }
    #define DEF_CMD(name, func, diff, rate)                           \
        else if (element->type == name )                        \
        {\
            fprintf (ptrFile, "elem%d [label=\""#func"\"]", i); \
        }

    #include "commands.h"
    #undef DEF_CMD

    else
        assert (0);


    //fprintf (ptrFile, "elem%d [shape=record,label=\"{ parent\\n %p | ptr %d\\n %p | type\\n %d| data\\n %lg | { <l>left\\n %p | <r> right\\n %p }}\"];\n", i, element->parent, i, element, element->type, element->data, element->left, element->right);

    if (element->right != NULL)
    {
        fPrintfElem (element->right, ptrFile);
        //printf ("I`m connecting %d\n", i);
        //fprintf (ptrFile, "elem%d:<r>->elem%d;\n", element->number, element->right->number);
    }

    return 0;
}

//--------------------------------------------------------------------------------

int fPrintfArrow (tree_elem_t* element, FILE* ptrFile)
{
    if (element->left != 0)
    {
        fprintf (ptrFile, "elem%d:<l>->elem%d[color=\"red\"];\n", element->number, element->left->number);
        fPrintfArrow (element->left, ptrFile);
    }

    if (element->right != 0)
    {
        fprintf (ptrFile, "elem%d:<r>->elem%d[color=\"green\"];\n", element->number, element->right->number);
        fPrintfArrow (element->right, ptrFile);
    }
    return 0;
}

//-------------------------------------------------------------------------------------------------------

int destructElement (tree_t* rTree, tree_elem_t* element)
{
    if (element->left  != NULL)
        destructElement(rTree, element->left );

    if (element->right != NULL)
        destructElement(rTree, element->right);

    if (element->parent->left  == element)
        element->parent->left  = NULL;

    if (element->parent->right == element)
        element->parent->right = NULL;

    rTree->nElements--;
    free (element);
    return 0;
}

//--------------------------------------------------------------------------------------------------------

int isNull (double number)
{
    if (fabs (number) < 0.000000001)
        return 0;
    else
        return 1;
}

//---------------------------------------------------------------------------------------------------------






