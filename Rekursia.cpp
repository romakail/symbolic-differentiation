#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cctype>
#include <assert.h>
#include <cmath>

#include "Rekursia.h"

int p = 0;
char* s = 0;


int main ()
{
    int a = getG0 ("5");
    printf ("%d", a);
    return 0;
}

//---------------------------------------------------------------------------------------------------------

int getG0 (const char* what)
{
    char* s = what;
    int val = getN();
    assert (s[p] == '\0');
    return val;
}
/*
int getN ()
{
    if ((s[p] >= '0') && (s[p] <= '9'))
    {
        int val = s[p] - '0';
        p++;
        return val;
    }
    else
        return 0;
}
*/
//------------------------------------------------------------------------------------------------------------

int getN ()
{
    int val = 0;

    while ((s[p] >= '0') && (s[p] <= '9'))
    {
        val = val * 10 + s[p] - '0';
        p++;
    }
    return val;
}

//--------------------------------------------------------------------------------------------------------------

int getE ()
{
    int val = getT;

    while ((s[p] == '+') || (s[p] == '-'))
    {
        int op = s[p];
        p++;
        int val2 = getT();
        if      (op == '+') val +=val2;
        else if (op == '-') val -=val2;
        else    assert (0);
    }
    return val;
}

//--------------------------------------------------------------------------------------------------------------

int getT ()
{
    int val = getN;

    while ((s[p] == '*') || (s[p] == '/'))
    {
        int op = s[p];
        p++;
        int val2 = getN();
        if      (op == '*') val *=val2;
        else if (op == '/') val /=val2;
        else    assert (0);
    }
    return val;
}

//---------------------------------------------------------------------------------------------------------------

int getP ()
{
    if (s[p] == '(')
    {
        p++;
        int val = getE ();
        assert (s[p] == ')')
        p++;
        return val;
    }
    else
        return getN ();
}


