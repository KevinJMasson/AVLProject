#include <iostream>
#include <algorithm>

using namespace std;

class Node {
    public:
    int value;
    int height;
    Node *left;
    Node *right;
};

Node* createNode(int);
int findHeight(Node*);
int balance(Node*);
Node* RR(Node*);
Node* LR(Node*);
Node* insert(Node*, int);
Node* smallestNode(Node*);
Node* deleteNode(Node*, int);
void findValue(Node*, int);
void output(Node*);


//Creates a new node within the tree
Node* createNode(int value) {
    Node* n = new Node();
    n->value = value;
    n->left = nullptr;
    n->right = nullptr;
    n->height = 1;
    return n;
}

//finds the height of the inputted node, if it is not a nullptr
int findHeight(Node* n) {
    if (n == nullptr)
      return 0;
    else
      return n->height;
}

//calculates the balance factor of the node to see if it needs to be rotated
int balance(Node *n) {
    if (n == nullptr)
        return 0;
    return (findHeight(n->left) - findHeight(n->right));
}

//Right rotation
//rotates node values and recalculates the height based on the highest node
//updates input node to new height based on its new rotated position
//updates new nodes height based on its new children
Node *RR(Node *n) {
    Node *t1 = n->left;
    Node *t2 = t1->right;

    t1->right = n;
    n->left = t2;

    n->height = max(findHeight(n->left), findHeight(n->right)) + 1;
    t1->height = max(findHeight(t1->left), findHeight(t1->right)) + 1;

    return t1;
}

//Left rotation
//rotates node values and recalculates the height based on the highest node
//updates input node to new height based on its new rotated position
//updates new nodes height based on its new children
Node *LR(Node* n) {
    Node *t1 = n->right;
    Node *t2 = t1->left;

    t1->left = n;
    n->right = t2;

    n->height = max(findHeight(n->left), findHeight(n->right)) + 1;
    t1->height = max(findHeight(t1->left), findHeight(t1->right)) + 1;

    return t1;
}

//Inserts a new node into the tree based on if each node value is greater or
//lower than input, and shifts node right if new value is greater, and left if
//lower. Then calculates how the tree needs to be re-balanced to maintain no
//height difference greater than 1.
Node* insert(Node* n, int value) {
//if the node is a nullpointer, it will create a new node with the value
  if (n == nullptr)
    return(createNode(value));

//if the value is greater than the current value it will check the right side
//as left is no longer relevant, and if it is less than current it will check
//the left as the right is no longer relevant
  if (value < n->value)
    n->left = insert(n->left, value);
    else if (value > n->value)
      n->right = insert(n->right, value);
      else
        return n;

  n->height = 1 + max(findHeight(n->left), findHeight(n->right));

//once inserted it will check to make sure that all nodes are within 1 height difference
  int b = balance(n);

  if (b > 1 && value < n->left->value)
    return RR(n);

  if (b < -1 && value > n->right->value)
    return LR(n);

  if (b > 1 && value > n->left->value) {
    n->left = LR(n->left);
    return RR(n);
  }

  if (b < -1 && value < n->right->value) {
    n->right = RR(n->right);
    return LR(n);
  }

return n;
}

//used to find the smallest node in the tree, as the smaller node will always
//be the left position node
Node* smallestNode(Node* n)
{
    Node* s = n;

    while (s->left != nullptr)
        s = s->left;

    return s;
}

//IDeletes node with input value based on if each node value is greater or
//lower than input, and shifts node right if new value is greater, and left if
//lower. Then calculates how the tree needs to be re-balanced to maintain no
//height difference greater than 1.
Node* deleteNode(Node* n, int value)
{
//if value is nullptr then we have hit the bottom
  if (n == nullptr)
    return n;

//if the input value is less than the value, it will move to the left since
//right is no longer relevant, otherwise it will move to the right as left is
//no longer relevant.
  if (value < n->value)
    n->left = deleteNode(n->left, value);
  else if(value > n->value )
      n->right = deleteNode(n->right, value);
      else {
//if one of the children node is empty it will move the nodes into correct
//alignment
        if(n->left == nullptr || n->right == nullptr) {
          Node* temp;
          if (n->left != nullptr)
            temp = n->left;
          else
            temp = n->right;

          if (temp == nullptr) {
            temp = n;
            n = nullptr;
          }
          else
            n = temp;

          if (temp != nullptr)
            delete temp;
        }
      else {
//if the node's children are not nullpointers, it will move the the lowest value
//on the right side up
      Node* temp = smallestNode(n->right);

      n->value = temp->value;

      n->right = deleteNode(n->right, temp->value);
    }
  }

  if (n == nullptr)
    return n;

  n->height = 1 + max(findHeight(n->left), findHeight(n->right));

//checking for height balance to ensure that all nodes are within 1 height difference
  int b = balance(n);

  if (b > 1 && balance(n->left) >= 0)
    return RR(n);

  if (b > 1 && balance(n->left) < 0) {
    n->left = LR(n->left);
    return RR(n);
  }

  if (b < -1 && balance(n->right) <= 0)
    return LR(n);

  if (b < -1 && balance(n->right) > 0) {
      n->right = RR(n->right);
      return LR(n);
  }

  return n;
}

//function to find the node with input value, mostly for demonstration
//purposes, but uses the AVL values of if the input is greater than the current
//node it will check the right side as left will only be smaller, and vice versa
//for lower. function assumes that the value is known to be present in the tree
void findValue(Node* n, int value) {
    Node* c = n;
    int i=1;

    while (c->value != value) {
        if (c->value > value)
          c = c->left;
        else
          c = c->right;
    }
}

//outputs the tree based on the preorder of the tree. Will output the value
//before moving on to the next node. Will output the height value of the node
//to show when it is at the root and when it hits the bottom to check validity
void output(Node* r) {
  if(r != nullptr){
    cout << r->value << "(" << r->height << ") ";
    output(r->left);
    output(r->right);
  }
}

int main()
{
    char in = 'y';
    int i = 2;
    Node *root = nullptr;

    while (i<20) {
  		root = insert(root, i);
      i = i+2;
  	}
    cout << "Before Delete:" << endl;
    output(root);
    cout << endl;
    cout << "After Delete:" << endl;
    root = deleteNode(root, 8);
    output(root);
    cout << endl;

    while (in == 'y') {
      root = insert(root, i);
      output(root);
      cout << endl;
      cout << "Again?: ";
      cin >> in;
      i=i+2;
    }

    return 0;
}
