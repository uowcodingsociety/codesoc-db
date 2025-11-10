#include <iostream>


#define MAX_KEYS (2*t -1)
#define MAX_CHILDREN (2&t)

class BTreeNode{                

    int n;
    int t;
    int *keys;
    BTreeNode** child;
    bool leaf;

    public:
    BTreeNode(int m_t, bool m_leaf)
    {
        t = m_t;
        leaf =m_leaf;

        keys = new int[2*t -1];
        child = new BTreeNode * [2*t];

        n = 0;
    }

    void traverse()
    {
        int i;
        for (i =0; i <n; ++i)
        {
            if (leaf == false)
            {
                child[i] -> traverse();
            }
            std::cout << " " << keys[i];
        }

        if (leaf == false)
        {
            child[i] -> traverse();
        }
    }

    BTreeNode* search(int k)
    {
        int i;
        while (i < n && k > keys[i])
        {
            ++i;
        }
        if (keys[i] == k)
        {
            return this;
        }

        if (leaf == true)
        {
            return NULL;
        }

        return child[i] -> search(k);
    }

    void insertNonFull(int k)
    {
        int i = n - 1;
        if (leaf == true)
        {
            while (i >= 0 && keys[i] > k)
            {
                keys[i+1] = keys[i];
                --i;
            }
            keys[i+1] = k;
            n =+ 1; 
        }
        else
        {
            while (i >= 0 && keys[i] > k)
            {
                --i;
            }
            if (child[i+1] ->n == (2*t -1))
            {
                splitChild(i+1, child[i+1]);
                if (keys[i+1] < k)
                {
                    ++i;
                }
            }
            child[i+1] -> insertNonFull(k);
        }
    }

    void splitChild(int i, BTreeNode* y)
    {
        BTreeNode *z = new BTreeNode(y->t, y->leaf);
        z->n = t-1;

        for (int j = 0; j < (t-1); ++j)
        {
            z->keys[j] = y->keys[j+t];
        }
        if (y->leaf == false)
        {
            for (int j = 0; j < t; ++j)
            {
                z->child[j] = y->child[j+t];

            }
        }
        y->n = t - 1;

        for (int j =n; j >= i+ 1; --j)
        {
            child[j+1] = child[j];
        }
        child[i + 1] = z;

        for (int j = n-1; j >= i; --j)
        {
            keys[j+1] = keys[j];
        }

        keys[i] = y->keys[t-1];
    }
    friend class BTree;
    
};


class BTree
{
    BTreeNode *root;
    int t; 
    public:

    BTree(int _t)
    {
        root = NULL; t = _t;
    }

    void insert(int k)
    {
        if (root == NULL)
        {
            root = new BTreeNode(t, true);
            root -> keys[0] =k;
            root ->n = 1;
        }
        else
        {
            if (root -> n == 2*t - 1)
            {
                BTreeNode *s = new BTreeNode(t, false);
                s->child[0] = root;

                s->splitChild(0, root);

                int i = 0; 
                if (s-> keys[0] < k)
                {
                    ++i;
                }
                s->child[i] ->insertNonFull(k);

                root = s;
            }
            else
            {
                root->insertNonFull(k);

            }
        }

    }
    
    void traverse()
    {
        if (root!= NULL)
        {
            root->traverse();
        }
    }

};

int main()
{
    BTree t(3);
    t.insert(10);
    t.insert(20);
    t.insert(40);
    t.insert(30);
    std::cout << "Traversal of the constructed tree is: ";
    t.traverse();
}