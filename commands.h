
DEF_CMD (CONST, 0,  {                               \
                    diffNode->type  = CONST;        \
                    diffNode->data  = 0;            \
                    diffNode->left  = NULL;         \
                    diffNode->right = NULL;         \
                    }, 5)

DEF_CMD (VAR  , x,  {                               \
                    diffNode->type  = CONST;        \
                    diffNode->data  = 1;            \
                    diffNode->left  = NULL;         \
                    diffNode->right = NULL;         \
                    }, 5)

DEF_CMD (ADD  , +,  {                               \
                    diffNode->type  = ADD;          \
                    diffNode->data  = POISON;       \
                                                    \
                    addElemLeft(diffTree, diffNode, 0);                                 \
                    differenciateNode (diffTree, protNode->left, diffNode->left);       \
                                                                                        \
                    addElemRight(diffTree, diffNode, 0);                                \
                    differenciateNode (diffTree, protNode->right, diffNode->right);     \
                    }, 1)

DEF_CMD (SUB   , -, {                                                               \
                    diffNode->type  = SUB;                                          \
                    diffNode->data  = POISON;                                       \
                                                                                    \
                    addElemLeft(diffTree, diffNode, 0);                             \
                    differenciateNode (diffTree, protNode->left, diffNode->left);   \
                                                                                    \
                    addElemRight(diffTree, diffNode, 0);                            \
                    differenciateNode (diffTree, protNode->right, diffNode->right); \
                    }, 1)

DEF_CMD (MULT  , *, {                                                                       \
                    diffNode->type  = ADD;                                                  \
                    diffNode->data  = POISON;                                               \
                                                                                            \
                    addElemLeft      (diffTree, diffNode, MULT);                            \
                    addElemRight     (diffTree, diffNode, MULT);                            \
                                                                                            \
                    addElemLeft      (diffTree, diffNode->left , 0);                        \
                    addElemRight     (diffTree, diffNode->left , 0);                        \
                    addElemLeft      (diffTree, diffNode->right, 0);                        \
                    addElemRight     (diffTree, diffNode->right, 0);                        \
                                                                                            \
                    copyNode         (diffTree, protNode->left , diffNode->left ->left );   \
                    differenciateNode(diffTree, protNode->right, diffNode->left ->right);   \
                                                                                            \
                    copyNode         (diffTree, protNode->right, diffNode->right->left );   \
                    differenciateNode(diffTree, protNode->left , diffNode->right->right);   \
                    }, 2)

DEF_CMD (DIV , /    ,   {
                        diffNode->type = DIV;
                        diffNode->data = POISON;

                        addElemLeft  (diffTree, diffNode      , SUB );
                        addElemLeft  (diffTree, diffNode->left, MULT);
                        addElemRight (diffTree, diffNode->left, MULT);

                        addElemLeft  (diffTree, diffNode->left->left , NOTH);
                        addElemRight (diffTree, diffNode->left->left , NOTH);
                        addElemLeft  (diffTree, diffNode->left->right, NOTH);
                        addElemRight (diffTree, diffNode->left->right, NOTH);

                        differenciateNode (diffTree, protNode->left , diffNode->left->left ->left );
                        copyNode          (diffTree, protNode->right, diffNode->left->left ->right);

                        differenciateNode (diffTree, protNode->right, diffNode->left->right->left );
                        copyNode          (diffTree, protNode->left , diffNode->left->right->right);

                        addElemRight (diffTree, diffNode       , MULT);
                        addElemLeft  (diffTree, diffNode->right, NOTH);
                        addElemRight (diffTree, diffNode->right, NOTH);

                        copyNode (diffTree, protNode->right, diffNode->right->left );
                        copyNode (diffTree, protNode->right, diffNode->right->right);
                        }, 3)


DEF_CMD (SIN , sin  , {
                        diffNode->type = MULT;
                        diffNode->data = POISON;

                        addElemLeft  (diffTree, diffNode, NOTH);
                        addElemRight (diffTree, diffNode, COS);
                        addElemLeft  (diffTree, diffNode->right, NOTH);

                        differenciateNode (diffTree, protNode->left, diffNode->left);
                        copyNode          (diffTree, protNode->left, diffNode->right->left);
                        }, 3)


DEF_CMD (COS , cos  , {
                        diffNode->type = MULT;
                        diffNode->data = POISON;

                        addElemLeft  (diffTree, diffNode, SUB);
                        addElemRight (diffTree, diffNode, SIN);

                        addElemLeft  (diffTree, diffNode->left, CONST, 0);
                        addElemRight (diffTree, diffNode->left, NOTH);

                        differenciateNode (diffTree, protNode->left, diffNode->left->right);

                        addElemLeft  (diffTree, diffNode->right, NOTH);
                        copyNode     (diffTree, protNode->left, diffNode->right->left);
                        }, 3)


DEF_CMD (SQRT, sqrt , {
                        diffNode->type = MULT;
                        diffNode->data = POISON;

                        addElemRight (diffTree, diffNode, NOTH);
                        differenciateNode (diffTree, protNode->left, diffNode->right);

                        addElemLeft  (diffTree, diffNode, DIV);

                        addElemLeft  (diffTree, diffNode->left, CONST, 1);
                        addElemRight (diffTree, diffNode->left, MULT);

                        addElemLeft  (diffTree, diffNode->left->right, CONST, 2);
                        addElemRight (diffTree, diffNode->left->right, SQRT);

                        addElemLeft  (diffTree, diffNode->left->right->right, NOTH);
                        copyNode     (diffTree, protNode->left, diffNode->left->right->right->left);
                        }, 5)

DEF_CMD (TAN , tan  , {
                        diffNode->type = DIV;
                        diffNode->data = POISON;

                        addElemLeft  (diffTree, diffNode, NOTH);
                        differenciateNode     (diffTree, protNode->left, diffNode->left);
                        addElemRight (diffTree, diffNode, MULT);

                        addElemLeft  (diffTree, diffNode->right, COS);
                        addElemRight (diffTree, diffNode->right, COS);

                        addElemLeft  (diffTree, diffNode->right->left , NOTH);
                        addElemLeft  (diffTree, diffNode->right->right, NOTH);

                        copyNode     (diffTree, protNode->left, diffNode->right->left ->left);
                        copyNode     (diffTree, protNode->left, diffNode->right->right->left);
                        }, 5)

DEF_CMD (EXP , exp  , {
                        diffNode->type = MULT;
                        diffNode->data = POISON;

                        addElemRight (diffTree, diffNode, NOTH);
                        differenciateNode (diffTree, protNode->left, diffNode->right);

                        addElemLeft  (diffTree, diffNode, NOTH);
                        copyNode     (diffTree, protNode, diffNode->left);

                        }, 5)


DEF_CMD (POW , pow  , {
                        diffNode->type = MULT;
                        diffNode->data = POISON;

                        addElemLeft  (diffTree, diffNode, NOTH);
                        copyNode     (diffTree, protNode, diffNode->left);

                        addElemRight (diffTree, diffNode, ADD);

                        addElemLeft  (diffTree, diffNode->right, MULT);
                        addElemLeft  (diffTree, diffNode->right->left, DIV);
                        addElemRight (diffTree, diffNode->right->left, NOTH);
                        differenciateNode (diffTree, protNode->left, diffNode->right->left->right);

                        addElemLeft  (diffTree, diffNode->right->left->left, NOTH);
                        addElemRight (diffTree, diffNode->right->left->left, NOTH);

                        copyNode     (diffTree, protNode->right, diffNode->right->left->left->left );
                        copyNode     (diffTree, protNode->left , diffNode->right->left->left->right);

                        addElemRight (diffTree, diffNode->right, MULT);
                        addElemLeft  (diffTree, diffNode->right->right, LN);
                        addElemLeft  (diffTree, diffNode->right->right->left, NOTH);
                        copyNode     (diffTree, protNode->left, diffNode->right->right->left->left);
                        addElemRight (diffTree, diffNode->right->right, NOTH);
                        differenciateNode(diffTree, protNode->right, diffNode->right->right->right);

                        }, 5)
DEF_CMD (LN  , ln   , {
                        diffNode->type = DIV;
                        diffNode->data = POISON;

                        addElemLeft  (diffTree, diffNode, NOTH);
                        addElemRight (diffTree, diffNode, NOTH);

                        differenciateNode (diffTree, protNode->left, diffNode->left );
                        copyNode          (diffTree, protNode->left, diffNode->right);

                        }, 5)

