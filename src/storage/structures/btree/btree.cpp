#include <iostream>
#include <memory>

/* 
B-Tree of minimum degree t can have up to 2t children
and up to (2t - 1) keys.
*/

class BTreeNode{                
    int n{0}; // number of keys stored in THIS node
    int t; // minimum degree, each node can have at max 2*t children, and max (2*t -1) keys.
    int* keys; // used to search and sort within the btree
    BTreeNode** child; // Child is an array of pointers, therefore child[i] is of type BTreeNode* .
    bool leaf;  

    public:
    BTreeNode(int m_t, bool m_leaf)
    {
        t = m_t;
        leaf = m_leaf;
        /* 
        Applying rules in relation to the minimum degree t. 
        Meaning allocating array size in relation to the max possible size.
        Maximum number of children is maximum number of keys + 1 
        */
        keys = new int[2*t -1];
        child = new BTreeNode * [2*t];
    }

    void traverse()
    {
        int i {0};
        while (i < n) // All the way up until the last node
        {
            if (!leaf)
            {
                child[i] -> traverse(); // Go as deep down as possible if not a leaf.
                std::cout << keys[i] << ' '; 
            }
            else
            {
                std::cout << keys[i] << ' ';
            }
            i++;
        }
        if (!leaf)
        {
            child[n] -> traverse();
        }
    }

    bool search(int k)
    {
        int i{0};
        while (i < n && k > keys[i]) 
        {
            ++i;
        }
        if (keys[i] == k)
        {
            return true;
        }

        if (leaf == true)
        {
            return false;
        }

        return child[i] -> search(k); // If neither of those conditions are true, then k must fall within the range of child[i]'s keys.
    }


    void splitChild(int i, BTreeNode* y)
    {    
        BTreeNode *z = new BTreeNode(y->t, y->leaf); 
        z->n = t-1; // Number of keys is 1 less than the minimum degree.
         
        for (int j{0}; j < z->n; j++)
        {
            z->keys[j] = y->keys[j+t]; 
            //t is half of the max size, -> z takes on the right half of y's keys.
        }
        if (y->leaf == false)
        {
            for (int j = 0; j < t; j++)
            {
                z->child[j] = y->child[j+t]; //moving right half of y's children to z
            }
        }
        y->n = t - 1; // shrinking y to its left half

        for (int j =n; j >= i+ 1; j--) // make space in the parent's children array
        {
            child[j+1] = child[j]; // shift child pointers right
        }
        child[i + 1] = z; //place z next to y in the child pointers
         
        for (int j = n-1; j >= i; j--)
        {
            keys[j+1] = keys[j]; //shift parent keys right
        }
        keys[i] = y->keys[t-1]; //insert median key into parent keys
    }

    void insertNonFull(int k) // Insert into the node when its not full.
    {
        int i = n - 1; // one less than the amount of keys stored in the node
        if (leaf == true)
        {
            while (i >= 0 && keys[i] > k) 
            {
                keys[i+1] = keys[i]; // shift keys to the right
                --i;
            }
            keys[i+1] = k; // insert key k into where it fits (ascending order)
            n += 1; //increase number of keys stored
        }
        else
        {
            while (i >= 0 && keys[i] > k)
            {
                --i;
            }
            if (child[i+1] ->n == (2*t -1)) // if the child has the maximum number of keys
            {
                splitChild(i+1, child[i+1]);
                if (keys[i+1] < k) 
                // if k is greater than the median, it belongs to z, (child[i+2])
                {
                    ++i; // increment once
                }
            }
            child[i+1] -> insertNonFull(k); //insert k into child[i+2] 
        }
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
        root = nullptr; t = _t;
    }

    void insert(int k)
    {
        if (!root)
        {
            root = new BTreeNode(t, true);
            root -> keys[0] =k; 
            root ->n = 1;
        }
        else
        {
            if (root -> n == 2*t - 1) //max number of keys
            {
                BTreeNode *s = new BTreeNode(t, false);
                s->child[0] = root; // s becomes the new parent and then splits the old root

                s->splitChild(0, root);

                int i{0}; 
                if (s-> keys[0] < k)
                {
                    ++i;
                }
                s->child[i] ->insertNonFull(k);
                root = s; //updating the root pointer 
            }
            else
            {
                root->insertNonFull(k);
            }
        }

    }
    
    void traverse()
    {
        if (root)
        {
            root->traverse();
        }
        else
        {
            std::cout << "BTree empty. \n";
        }
    }

};

int main()
{
    BTree t(3);
    t.traverse(); //BTree Empty

    t.insert(40);
    t.insert(30);
    t.insert(6);
    t.traverse();
}