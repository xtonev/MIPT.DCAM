#include <TXLib.h>

enum Nodetype
{
    OP = 0,
    NUM,
    VAR
};

enum Operation
{
    FREE = 0,
    ADD = '+',
    SUB = '-',
    MUL = '*',
    DIV = '/',
    SIN = 's',
    COS = 'c',
    TG = 't',
    POWER = '^',
    SQRT = 'q',
    EXP = 'e',
    LN = 'l'
};

enum DeleteParam
{
    DELETE_NODE = 0,
    DELETE_ALL
};

enum Warning
{
    WRONG_NODE_TYPE = 1,
    WRONG_OPERATION = 2,
    WRONG_POINTER = 4,
    WRONG_PARENT = 8,
    NODE_COUNTER_ERROR = 16
}

typedef struct tree
{
    node* root;
    size_t nodeCounter;
} tree;

typedef struct node
{
    int nodeType;
    double data;
    node* parent;
    node* left;
    node* right;
} node;

tree* TreeCtor() //int Nodetype, double data, node* left, node* right
{
    tree* newTree = (tree*) calloc (1, sizeof(tree));
    newTree->root = NULL; //NodeCtor (newTree, OPERATOR, EQU, NULL, NULL, NULL);
    newTree->nodeCounter = 0;
    return newTree;
}

node* NodeCtor (tree* curTree, int nodeType, double data, node* parent, node* left, node* right)
{
    if ( (curTree->nodeCounter == 0) ^ (parent == NULL) )
        return NULL;
    node* newNode = (node*) calloc (1, sizeof (node));
    if (curTree->nodeCounter == 0)
        curTree->root = newNode;
    newNode->nodeType = nodeType;
    newNode->data = data;
    newNode->parent = parent;
    newNode->left = left;
    newNode->right = right;
    curTree->nodeCounter++;
    return newNode;
}

int DeleteNode (tree* curTree, node** curNodeP)
{
    if ((curTree == NULL) || (curNodeP == NULL) || (*curNodeP == NULL))
        return WRONG_POINTER;
    //if (*curNodeP->parent->left == *curNodeP)
    //    *curNodeP->parent->left = NULL;
    //else if (*curNodeP->parent->left == *curNodeP)
    //        *curNodeP->parent->right = NULL;
    //else return WRONG_PARENT;
    //*curNodeP->left->parent = NULL;
    //*curNodeP->right->parent = NULL;
    free (*curNode);
    *curNode = NULL;
    curTree->nodecounter--;
    if (curTree->nodeCounter < 0)
        return NODE_COUNTER_ERROR;
    return 0;
}

node* NodeDtor (tree* curTree, node* curNode, int DeleteParam, node** saveLeft, node** saveRight)
{
    if ((curNode == NULL) || (curTree == NULL))
        return NULL;
    else
    {
        if (DeleteParam)
        {
            NodeDtor (curNode->left, DELETE_ALL, NULL, NULL);
            NodeDtor (curNode->right, DELETE_ALL, NULL, NULL);
            DeleteNode (curTree, &curNode);
            return NULL;
        }
        else
        {
            if ((saveLeft == NULL) && (saveRight == NULL))
            {
                if (curNode->parent->left == curNode)
                    StickLeft (curTree, curNode->parent, curNode->left);
                else if (curNode->parent->right == curNode)
                    StickRight (curTree, curNode->parent, curNode->left);
                else return NULL;
                node* tempNode = curNode->right;
                tempNode->parent = NULL;
                DeleteNode (curTree, &curNode);
                return tempNode;
            }
            if ((saveLeft != NULL) && (saveRight == NULL))
            {
                &saveLeft = curNode->left;
                &saveLeft->parent = NULL;
                if (curNode->parent->left == curNode)
                    StickLeft (curTree, curNode->parent, curNode->right);
                else if (curNode->parent->right == curNode)
                    StickRight (curTree, curNode->parent, curNode->right);
                else return NULL;
                DeleteNode (curTree, &curNode);
                return NULL;
            }
            if ((saveLeft == NULL) && (saveRight != NULL))
            {
                if (curNode->parent->left == curNode)
                    StickLeft (curTree, curNode->parent, curNode->left);
                else if (curNode->parent->right == curNode)
                    StickRight (curTree, curNode->parent, curNode->left);
                else return NULL;
                &saveRight = curNode->right;
                &saveRight->parent = NULL;
                DeleteNode (curTree, &curNode);
                return NULL;
            }
            if ((saveLeft != NULL) && (saveRight != NULL))
            {
                if (curNode->parent->left == curNode)
                    curNode->parent->left = NULL;
                else if (curNode->parent->right == curNode)
                    curNode->parent->right = NULL;
                else return NULL;
                &saveLeft = curNode->left;
                &saveLeft->parent = NULL;
                &saveRight = curNode->right;
                &saveRight->parent = NULL;
                DeleteNode (curTree, &curNode);
                return NULL;
            }
        }
    }
}

int TreeDtor (tree** curTreeP)
{
    if ((curTreeP == NULL) || (*curTreeP == NULL))
        return WRONG_POINTER;
    NodeDtor (*curTreeP, *curTreeP->root, DELETE_ALL, NULL, NULL);
    free (*curTreeP);
    *curTreeP = NULL;
    return 0;
}

int NodeOK (node* curNode) //make list for pointers
{
    if (!curNode)
    {
        switch (curNode->nodeType)
        {
            case OP:
            {
                switch ((int)curNode->data)
                {
                    case FREE:
                        break;
                    case ADD:
                        break;
                    case SUB:
                        break;
                    case MUL:
                        break;
                    case DIV:
                        break;
                    case SIN:
                        break;
                    case COS:
                        break;
                    case TG:
                        break;
                    case POWER:
                        break;
                    case SQRT:
                        break;
                    case EXP:
                        break;
                    case LN:
                        break;
                    default:
                        return WRONG_OPERATION;
                }
                break;
            }
            case NUM:
                break;
            case VAR
                break;
            default :
                return WRONG_NODE_TYPE;
        }
        if ((curNode->left->parent != curNode) || (curNode->right->parent != curNode))
                return WRONG_POINTER;
        return ( NodeOK (curNode->left) + NodeOK (curNode->right) );
    }
    else return 0;
}

void DumpTree (tree* curTree)
{
}

int CreateLeft (tree* curTree, node* curNode, int nodeType, double data, node* left, node* right)
{
    if ((curNode->left) || (curTree == NULL))
        return WRONG_POINTER;
    curNode->left = NodeCtor (curTree, nodeType, data, curNode, left, right);
    curNode->left->parent = curNode;
    return 0;
}

int StickLeft (tree* curTree, node* curNode, node* stickNode)
{
    if ((curNode->left != NULL) || (curTree == NULL))
        return WRONG_POINTER;
    curNode->left = stickNode;
    stickNode->parent = curNode;
    return 0;
}

int CreateRight (tree* curTree, node* curNode, int nodeType, double data, node* left, node* right)
{
    if ((curNode->right) || (curTree == NULL))
        return WRONG_POINTER;
    curNode->right = NodeCtor (curTree, nodeType, data, curNode, left, right);
    curNode->right->parent = curNode;
    return 0;
}

int StickRight (tree* CurTree, node* curNode, node* stickNode)
{
    if ((curNode->right) || (curTree == NULL))
        return WRONG_POINTER;
    curNode->right = stickNode;
    stickNode->parent = curNode;
    return 0;
}

int InsertNode (node* curNode, int nodeType, double data, node* left, node* right)
{
    if (!curNode)
        return WRONG_POINTER;
    curNode->nodeType = nodeType;
    curNode->data = data;
    curNode->left = left;
    curNode->right = right;
    return 0;
}

tree* TreeCtorByExpr
{
    FILE* expression = fopen ("expression.txt", "r");
    fseek (expression, 0, SEEK_END);
    size_t exprSize = ftell (expression);
    rewind (expression);
    if (!exprSize)
        return NULL;
    char* buffer = (char*) calloc (fsize + 1, sizeof (char) );
    fread (buffer, sizeof (char), exprSize, expression);
    fclose (expression);

    tree* newTree = TreeCtor();
    node* tempNode = NULL;

    size_t counter = 0;
    char* endPtr = NULL;
    while (counter <= exprSize)
    {
        if ( isspace(buffer[counter] )
            counter++;
        else if ( isdigit(buffer[counter] )
        {
            if (tempNode->left == NULL)
                CreateLeft (newtree, tempNode, NUM, strtod (buffer[counter], &endPtr), NULL, NULL);
            else CreateRight (newTree, tempNode, NUM, strtod (buffer[counter], &endPtr), NULL, NULL);
            counter = counter + ( endPtr - &buffer[counter] ) / sizeof (char);
        }
        else if ( isalpha(buffer[counter]) )
        {
            if (tempNode->left == NULL)
                CreateLeft (newtree, tempNode, VAR, 'buffer[counter]', NULL, NULL);
            else CreateRight (newTree, tempNode, VAR, 'buffer[counter]', NULL, NULL);
            counter++;
        }
        else
        {
            switch (buffer[counter])
            {
                case '(':
                {
                    if (tempNode->data == FREE)
                    {
                        CreateLeft (newTree, tempNode, OP, FREE, NULL, NULL);
                        tempNode = tempNode->left;
                    }
                    else
                    {
                        CreateRight (newtree, tempNode, OP, FREE, NULL, NULL);
                        tempNode = tempNode->right;
                    }
                    break;
                }
                case ')':
                {
                    tempNode = tempNode->parent;
                    break;
                }
                case ADD:
                {
                    if (tempNode->data != FREE)
                        tempNode = tempNode->parent;
                    tempNode->data = ADD;
                    break;
                }
                case SUB:
                {
                    if (tempNode->data != FREE)
                        tempNode = tempNode->parent;
                    tempNode->data = SUB;
                    break;
                }
                case MUL:
                {
                    if (tempNode->data != FREE)
                        tempNode = tempNode->parent;
                    tempNode->data = MUL;
                    break;
                }
                case DIV:
                {
                    if (tempNode->data != FREE)
                        tempNode = tempNode->parent;
                    tempNode->data = DIV;
                    break;
                }
                case POWER:
                {
                    if (tempNode->data != FREE)
                        tempNode = tempNode->parent;
                    tempNode->data = POWER;
                    break;
                }
                case SQRT:
                {
                    tempNode->data = SQRT;
                    break;
                }
                default:
                {
                    TreeDtor (&newTree);
                    return NULL;
                }
            }
            counter++;
        }
    }

    free (buffer);
    return newTree;
}

node* CopySubtree (tree* newTree, node* curNode) //node = (answ == 'y')? node->left :node->right
{
    if (curNode == NULL)
        return NULL;
    else return NodeCtor (newTree, curNode->nodetype, curNode->data, NULL, CopySubtree (curNode->left), CopySubtree (curNode->right) );
}

void RenewParent (node* renewingSubtree)
{
    if (renewingSubtree != NULL)
    {
        renewingSubtree->left->parent = renewingSubtree;
        renewingSubtree->right->parent = renewingSubtree;
        RenewParent (renewingSubtree->left);
        RenewParent (renewingSubtree->right);
    }
}

tree* CopyTree (tree* curTree)
{
    if (curTree == NULL)
        return NULL;
    tree* newTree = (tree*) calloc (1, sizeof (tree));
    newTree->nodeCounter = curTree->nodeCounter;
    if (newTree->nodeCounter)
    {
       newTree->root = CopySubtree (newTree, curTree->root);
       RenewParent (newTree->root);
    }
    else newTree->root = NULL;
    return newTree;
}

node* NodeD (tree* newTree, node* curNode)
{
    switch (curNode->nodeType)
    {
        case OP:
        {
            switch ((int)curNode->data)
            {
                case ADD:
                    return NodeCtor (newTree, OP, ADD, NULL, NodeD (newTree, curNode->left), NodeD (newTree, curNode->right) )
                case SUB:
                    return NodeCtor (newTree, OP, SUB, NULL, NodeD (newTree, curNode->left), NodeD (newTree, curNode->right) )
                case MUL:
                    return NodeCtor (newTree, OP, ADD, NULL,
                        NodeCtor (newTree, OP, MUL, NULL, NodeD (newTree, curNode->left), CopySubtree (newTree, curNode->right),
                            NodeCtor (newTree, OP, MUL, NULL, CopySubtree (newTree, curNode->left), NodeD (newTree, curNode->right) );
                case DIV:
                    return NodeCtor (newTree, OP, DIV, NULL,
                        NodeCtor (newTree, OP, SUB, NULL, NodeCtor (newTree, OP, MUL, NULL, NodeD (newTree, curNode->left), CopySubtree (newTree, curNode->right) ), NodeCtor (newTree, OP, MUL, NULL, CopySubtree (newTree, curNode->left), NodeD (newTree, curNode->right) ) ),
                            NodeCtor (newTree, OP, POWER, NULL, CopySubtree (newTree, curNode->right), NodeCtor (newTree, NUM, 2, NULL, NULL, NULL) ) );
                case SIN:
                    return NodeCtor (newTree, OP, MUL, NULL, NodeCtor (newTree, OP, COS, NULL, CopySubtree (newTree, curNode->left), NULL), NodeD (newTree, curNode->left) );
                case COS:
                    return NodeCtor (newTree, OP, SUB, NULL, NodeCtor (newTree, NUM, 0, NULL, NULL, NULL), NodeCtor (newTree, OP, MUL, NULL, NodeCtor (newTree, OP, SIN, NULL, CopySubtree (newTree, curNode->left), NULL), NodeD (newTree, curNode->left) )
                case TG:
                    return NodeCtor (newTree, OP, DIV, NULL, NodeCtor (newTree, NUM, 1, NULL, NULL, NULL), NodeCtor (newTree, OP, POWER, NULL, NodeCtor (newTree, OP, COS, NULL, CopySubtree (newTree, curNode->left), NULL), NodeCtor (newTree, NUM, 2, NULL, NULL, NULL) ) );
                case POWER:
                    return NodeCtor (newTree, OP, MUL, NULL,
                        NodeCtor (newTree, OP, POWER, NULL, CopySubtree (newTree, curNode->left), CopySubtree (newTree, curNode->right) ),
                        NodeCtor (newTree, OP, ADD, NULL,
                            NodeCtor (newTree, OP, MUL, NULL,
                                NodeCtor (newTree, OP, DIV, NULL, CopySubtree (newTree, curNode->right), CopySubtree (newTree, curNode->right) ),
                                NodeD (newTree, curNode->left) ),
                            NodeCtor (newTree, OP, MUL, NULL, NodeCtor (newTree, OP, LN, NULL, CopySubtree (newTree, curNode->left), NULL), NodeD (newTree, curNode->right) ) ) );
                case SQRT:
                    return NodeCtor (newTree, OP, DIV, NULL,
                        NodeCtor (newTree, NUM, 1, NULL, NULL, NULL),
                        NodeCtor (newTree, OP, MUL, NULL,
                            NodeCtor (newTree, NUM, 2, NULL, NULL, NULL),
                            NodeCtor (newTree, OP, SQRT, NULL, CopySubtree (newTree, curNode->left), NULL) ) );
                case EXP:
                    return NodeCtor (newTree, OP, MUL, NULL, NodeD (newTree, curNode->left), NodeCtor (newTree, OP, EXP, NULL, CopySubtree (newTree, curNode->left), NULL) );
                case LN:
                    return NodeCtor (newTree, OP, DIV, NULL, NodeCtor (newTree, NUM, 1, NULL, NULL, NULL), CopySubtree (newTree, curNode->left) );
                default:
                    return NULL;
            }
        }
        case NUM:
            return NodeCtor (newTree, NUM, 0, NULL, NULL, NULL);
        case VAR:
            return NodeCtor (newTree, NUM, 1, NULL, NULL, NULL);
        default:
            return NULL;
    }
}

tree* TreeD (tree* curTree)
{
    if (curTree == NULL)
        return NULL;
    tree* newTree = (tree*) calloc (1, sizeof (tree));
    newTree->root = NodeD (newTree, curTree->root);
    return newTree;
}
