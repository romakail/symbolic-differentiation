

DEF_SIMP (ZeroMult, if  ((element->type == MULT) &&
                        (((element->left ->type == CONST) && (isNull (element->left ->data) == 0))  ||
                        (( element->right->type == CONST) && (isNull (element->right->data) == 0))))
                    {
                        printf ("\nI am multiplying in zero\n");
                        element->type  = CONST;
                        element->data  = 0;
                        destructElement (rTree, element->left );
                        destructElement (rTree, element->right);
                        flag = 1;
                    })


DEF_SIMP (OneMult,  if  ((element->type == MULT) &&
                        ((element->left ->type == CONST) && (isNull (element->left ->data - 1) == 0)))
                    {
                        printf   ("\nI am multiplying on 1\n");
                        copyNode (rTree, element->right,  element);
                        flag = 1;
                    }

                    if  ((element->type == MULT) &&
                        ((element->right->type == CONST) && (isNull (element->right->data - 1) == 0)))
                    {
                        printf   ("\nI am multiplying on 1\n");
                        copyNode (rTree, element->left, element);
                        flag = 1;
                    })


DEF_SIMP (ZeroDiv,  if  ((element->type == DIV) && (element->left->type == CONST) && (isNull (element->left->data) == 0))
                    {
                        printf ("\nI am diversing zero on smt\n");
                        element->type  = CONST;
                        element->data  = 0;
                        destructElement (rTree, element->left );
                        destructElement (rTree, element->right);
                        flag = 1;
                    })


DEF_SIMP (OneDiv,   if ((element->type == DIV) && (element->right->type == CONST) && (isNull (element->right->data - 1) == 0))
                    {
                        printf ("\nI am diversing on 1\n");
                        copyNode (rTree, element->left, element);
                        flag = 1;
                    })

DEF_SIMP (ZeroAdd, if   ((element->type == ADD) &&
                        ((element->left ->type == CONST) && (isNull (element->left ->data) == 0)))
                    {
                        printf   ("\nI am adding zero\n");
                        copyNode (rTree, element->right,  element);
                        flag = 1;
                    }

                    if  ((element->type == ADD) &&
                        ((element->right->type == CONST) && (isNull (element->right->data) == 0)))
                    {
                        printf   ("\nI am adding zero\n");
                        copyNode (rTree, element->left, element);
                        flag = 1;
                    })

DEF_SIMP (ZeroSub,  if ((element->type == SUB) && (element->right->type == CONST) && (isNull (element->right->data) == 0))
                    {
                        printf ("\nI am sub 1\n");
                        copyNode (rTree, element->left, element);
                        flag = 1;
                    })

//DEF_SIMP

