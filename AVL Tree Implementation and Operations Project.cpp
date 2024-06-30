#include <iostream>
using namespace std;

class Node {
public:
    int key;
    Node* left;
    Node* right;
    int height;
};

int max(int a, int b) {
    return (a > b) ? a : b;
}

int height(Node* N) {
    if (N == NULL)
        return 0;
    return N->height;
}

Node* newNode(int key) {
    Node* node = new Node();
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return (node);
}

Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

Node* leftRightRotate(Node* node) {
    node->left = leftRotate(node->left);
    return rightRotate(node);
}

Node* rightLeftRotate(Node* node) {
    node->right = rightRotate(node->right);
    return leftRotate(node);
}

int getBalanceFactor(Node* N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

Node* insertNode(Node* node, int key) {
    if (node == NULL)
        return (newNode(key));
    if (key < node->key)
        node->left = insertNode(node->left, key);
    else if (key > node->key)
        node->right = insertNode(node->right, key);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));

    int balanceFactor = getBalanceFactor(node);

    if (balanceFactor > 1) {
        if (key < node->left->key)
            return rightRotate(node);
        else if (key > node->left->key)
            return leftRightRotate(node);
    }

    if (balanceFactor < -1) {
        if (key > node->right->key)
            return leftRotate(node);
        else if (key < node->right->key)
            return rightLeftRotate(node);
    }

    return node;
}

Node* nodeWithMinimumValue(Node* node) {
    Node* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

Node* deleteNode(Node* root, int key) {
    if (root == NULL)
        return root;

    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else {
        if ((root->left == NULL) || (root->right == NULL)) {
            Node* temp = root->left ? root->left : root->right;

            if (temp == NULL) {
                temp = root;
                root = NULL;
            }
            else
                *root = *temp;

            delete temp;
        }
        else {
            Node* temp = nodeWithMinimumValue(root->right);

            root->key = temp->key;

            root->right = deleteNode(root->right, temp->key);
        }
    }

    if (root == NULL)
        return root;

    root->height = 1 + max(height(root->left), height(root->right));

    int balanceFactor = getBalanceFactor(root);

    if (balanceFactor > 1 && getBalanceFactor(root->left) >= 0)
        return rightRotate(root);

    if (balanceFactor > 1 && getBalanceFactor(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balanceFactor < -1 && getBalanceFactor(root->right) <= 0)
        return leftRotate(root);

    if (balanceFactor < -1 && getBalanceFactor(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

Node* searchNode(Node* root, int key) {
    if (root == NULL || root->key == key)
        return root;

    if (key < root->key)
        return searchNode(root->left, key);

    return searchNode(root->right, key);
}

int diameter(Node* root) {
    if (root == NULL)
        return 0;

    int leftHeight = height(root->left);
    int rightHeight = height(root->right);

    int leftDiameter = diameter(root->left);
    int rightDiameter = diameter(root->right);

    return max(leftHeight + rightHeight + 1, max(leftDiameter, rightDiameter));
}

Node* findMin(Node* root) {
    if (root == NULL || root->left == NULL)
        return root;
    return findMin(root->left);
}

Node* findMax(Node* root) {
    if (root == NULL || root->right == NULL)
        return root;
    return findMax(root->right);
}

Node* findSuccessor(Node* root, int key) {
    Node* current = searchNode(root, key);
    if (current == NULL)
        return NULL;

    if (current->right != NULL)
        return findMin(current->right);

    Node* successor = NULL;
    Node* ancestor = root;
    while (ancestor != current) {
        if (current->key < ancestor->key) {
            successor = ancestor;
            ancestor = ancestor->left;
        }
        else {
            ancestor = ancestor->right;
        }
    }

    return successor;
}

Node* findPredecessor(Node* root, int key) {
    Node* current = searchNode(root, key);
    if (current == NULL)
        return NULL;

    if (current->left != NULL)
        return findMax(current->left);

    Node* predecessor = NULL;
    Node* ancestor = root;
    while (ancestor != current) {
        if (current->key > ancestor->key) {
            predecessor = ancestor; 
            ancestor = ancestor->right;
        }
        else {
            ancestor = ancestor->left;
        }
    }

    return predecessor;
}

void input(Node*& root) {
    int data;
    cout << "Enter data (-1 to end): ";
    cin >> data;
    while (data != -1) {
        root = insertNode(root, data);
        cin >> data;
    }
}

void printTree(Node* root, string indent, bool last, bool showBalance) {
    if (root != nullptr) {
        cout << indent;
        if (last) {
            cout << "R----";
            indent += "   ";
        }
        else {
            cout << "L----";
            indent += "|  ";
        }
        cout << root->key;
        if (showBalance) {
            cout << " [Balance Factor: " << getBalanceFactor(root) << "]";
        }
        cout << endl;
        printTree(root->left, indent, false, showBalance);
        printTree(root->right, indent, true, showBalance);
    }
}

void inorderTraversal(Node* root) {
    if (root != NULL) {
        inorderTraversal(root->left);
        cout << root->key << " ";
        inorderTraversal(root->right);
    }
}

void preorderTraversal(Node* root) {
    if (root != NULL) {
        cout << root->key << " ";
        preorderTraversal(root->left);
        preorderTraversal(root->right);
    }
}

void postorderTraversal(Node* root) {
    if (root != NULL) {
        postorderTraversal(root->left);
        postorderTraversal(root->right);
        cout << root->key << " ";
    }
}

int main() {
    Node* root = NULL;
    int choice;
    int key;
    Node* result = NULL; 

    do {
        cout << "\n---------------------------" << endl;
        cout << "AVL Tree Operations Menu" << endl;
        cout << "---------------------------" << endl;
        cout << "1. Insert a node" << endl;
        cout << "2. Delete a node" << endl;
        cout << "3. Search for a key" << endl;
        cout << "4. Inorder Traversal" << endl;
        cout << "5. Preorder Traversal" << endl;
        cout << "6. Postorder Traversal" << endl;
        cout << "7. Print AVL Tree" << endl;
        cout << "8. Show Balance Factor of a Node" << endl;
        cout << "9. Calculate Diameter of the AVL Tree" << endl;
        cout << "10. Find Minimum Key" << endl;
        cout << "11. Find Maximum Key" << endl;
        cout << "12. Find Successor" << endl;
        cout << "13. Find Predecessor" << endl;
        cout << "14. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            do {
                cout << "Enter key to insert (-1 to end insertion): ";
                cin >> key;
                if (key != -1)
                    root = insertNode(root, key);
            } while (key != -1);
            break;
        case 2:
            cout << "Enter key to delete: ";
            cin >> key;
            root = deleteNode(root, key);
            break;
        case 3:
            cout << "Enter key to search: ";
            cin >> key;
            result = searchNode(root, key); 
            if (result != NULL) {
                cout << "Key " << key << " found in AVL tree." << endl;
            }
            else {
                cout << "Key " << key << " not found in AVL tree." << endl;
            }
            break;
        case 4:
            cout << "Inorder Traversal: ";
            inorderTraversal(root);
            cout << endl;
            break;
        case 5:
            cout << "Preorder Traversal: ";
            preorderTraversal(root);
            cout << endl;
            break;
        case 6:
            cout << "Postorder Traversal: ";
            postorderTraversal(root);
            cout << endl;
            break;
        case 7:
            cout << "AVL Tree Structure:" << endl;
            printTree(root, "", true, false);
            break;
        case 8:
            cout << "Enter key to show balance factor: ";
            cin >> key;
            result = searchNode(root, key); 
            if (result != NULL) {
                cout << "Balance Factor of Node " << key << " is " << getBalanceFactor(result) << endl;
            }
            else {
                cout << "Node with key " << key << " not found." << endl;
            }
            break;
        case 9:
            cout << "Diameter of the AVL Tree is: " << diameter(root) << endl;
            break;
        case 10:
            result = findMin(root);
            if (result != NULL)
                cout << "Minimum key in AVL tree is: " << result->key << endl;
            else
                cout << "AVL tree is empty." << endl;
            break;
        case 11:
            result = findMax(root);
            if (result != NULL)
                cout << "Maximum key in AVL tree is: " << result->key << endl;
            else
                cout << "AVL tree is empty." << endl;
            break;
        case 12:
            cout << "Enter key to find successor: ";
            cin >> key;
            result = findSuccessor(root, key);
            if (result != NULL)
                cout << "Successor of key " << key << " is: " << result->key << endl;
            else
                cout << "Key " << key << " not found or no successor exists." << endl;
            break;
        case 13:
            cout << "Enter key to find predecessor: ";
            cin >> key;
            result = findPredecessor(root, key);
            if (result != NULL)
                cout << "Predecessor of key " << key << " is: " << result->key << endl;
            else
                cout << "Key " << key << " not found or no predecessor exists." << endl;
            break;
        case 14:
            cout << "Exiting program." << endl;
            break;
        default:
            cout << "Invalid choice. Please enter a valid option." << endl;
            break;
        }
    } while (choice != 14);

    return 0;
}
