#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct Note {
    string lastName;
    string firstName;
    string phoneNumber;
    int birthDate[3];
};

struct TreeNode {
    Note data;
    TreeNode* left;
    TreeNode* right;
};

TreeNode* createNode(const Note& note);
TreeNode* deleteNode(TreeNode* root, const string& lastName);
TreeNode* minValueNode(TreeNode* node);
TreeNode* searchByLastName(TreeNode* root, const string& lastName);
void inOrderTraversal(TreeNode* root);
void freeMemory(TreeNode* root);
void saveToFile(TreeNode* root, ofstream& outFile);
TreeNode* loadFromFile(TreeNode* root, ifstream& inFile);
TreeNode* insertNode(TreeNode* root, const Note& note);
int getHeight(TreeNode* node);
TreeNode* rotateRight(TreeNode* root);
TreeNode* rotateLeft(TreeNode* root);
TreeNode* balanceTree(TreeNode* root);
void sortAndPrintByPhoneNumber(TreeNode* root);
void storeNodesInOrder(TreeNode* root, vector<Note>& notes);
bool isValidDate(int day, int month, int year);

TreeNode* createNode(const Note& note) {
    TreeNode* newNode = new TreeNode();
    newNode->data = note;
    newNode->left = nullptr;
    newNode->right = nullptr;
    return newNode;
}

TreeNode* insertNode(TreeNode* root, const Note& note) {
    if (root == nullptr) {
        return createNode(note);
    }

    TreeNode* parent = nullptr;
    TreeNode* current = root;

    while (current != nullptr) {
        parent = current;
        current = current->right;
    }

    parent->right = createNode(note);

    return root;
}

TreeNode* deleteNode(TreeNode* root, const string& lastName) {
    if (root == nullptr) {
        return root;
    }
    if (lastName < root->data.lastName) {
        root->left = deleteNode(root->left, lastName);
    }
    else if (lastName > root->data.lastName) {
        root->right = deleteNode(root->right, lastName);
    }
    else {
        if (root->left == nullptr) {
            TreeNode* temp = root->right;
            delete root;
            return temp;
        }
        else if (root->right == nullptr) {
            TreeNode* temp = root->left;
            delete root;
            return temp;
        }
        TreeNode* temp = minValueNode(root->right);
        root->data = temp->data;
        root->right = deleteNode(root->right, temp->data.lastName);
    }
    return root;
}

TreeNode* minValueNode(TreeNode* node) {
    TreeNode* current = node;
    while (current && current->left != nullptr) {
        current = current->left;
    }
    return current;
}

TreeNode* searchByLastName(TreeNode* root, const string& lastName) {
    if (root == nullptr || root->data.lastName == lastName) {
        return root;
    }
    if (lastName < root->data.lastName) {
        return searchByLastName(root->left, lastName);
    }
    else {
        return searchByLastName(root->right, lastName);
    }
}

void inOrderTraversal(TreeNode* root) {
    if (root == nullptr) {
        return;
    }
    inOrderTraversal(root->left);
    cout << "Last Name: " << root->data.lastName << endl;
    cout << "First Name: " << root->data.firstName << endl;
    cout << "Phone Number: " << root->data.phoneNumber << endl;
    cout << "Birth Date: " << root->data.birthDate[0] << "/" << root->data.birthDate[1] << "/" << root->data.birthDate[2] << endl;
    cout << "--------------------------" << endl;
    inOrderTraversal(root->right);
}

void freeMemory(TreeNode* root) {
    if (root == nullptr) {
        return;
    }
    freeMemory(root->left);
    freeMemory(root->right);
    delete root;
}

void saveToFile(TreeNode* root, ofstream& outFile) {
    if (root == nullptr) {
        return;
    }
    outFile << root->data.lastName << endl;
    outFile << root->data.firstName << endl;
    outFile << root->data.phoneNumber << endl;
    outFile << root->data.birthDate[0] << " " << root->data.birthDate[1] << " " << root->data.birthDate[2] << endl;
    saveToFile(root->left, outFile);
    saveToFile(root->right, outFile);
}

TreeNode* loadFromFile(TreeNode* root, ifstream& inFile) {
    freeMemory(root);
    root = nullptr;

    Note note;
    while (inFile >> note.lastName >> note.firstName >> note.phoneNumber >> note.birthDate[0] >> note.birthDate[1] >> note.birthDate[2]) {
        root = insertNode(root, note);
        root = balanceTree(root); 
    }
    return root;
}

int getHeight(TreeNode* node) {
    if (node == nullptr)
        return 0;
    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);
    return 1 + max(leftHeight, rightHeight);
}

TreeNode* rotateRight(TreeNode* root) {
    TreeNode* newRoot = root->left;
    root->left = newRoot->right;
    newRoot->right = root;
    return newRoot;
}

TreeNode* rotateLeft(TreeNode* root) {
    TreeNode* newRoot = root->right;
    root->right = newRoot->left;
    newRoot->left = root;
    return newRoot;
}

TreeNode* balanceTree(TreeNode* root) {
    if (root == nullptr)
        return root;

    int balanceFactor = getHeight(root->right) - getHeight(root->left);

    if (balanceFactor > 1) {
        if (getHeight(root->right->right) < getHeight(root->right->left)) {
            root->right = rotateRight(root->right);
        }
        root = rotateLeft(root);
    }
    else if (balanceFactor < -1) {
        if (getHeight(root->left->left) < getHeight(root->left->right)) {
            root->left = rotateLeft(root->left);
        }
        root = rotateRight(root);
    }
    root->left = balanceTree(root->left);
    root->right = balanceTree(root->right);
    return root;
}

void storeNodesInOrder(TreeNode* root, vector<Note>& notes) {
    if (root == nullptr) return;
    storeNodesInOrder(root->left, notes);
    notes.push_back(root->data);
    storeNodesInOrder(root->right, notes);
}

void sortAndPrintByPhoneNumber(TreeNode* root) {
    vector<Note> notes;
    storeNodesInOrder(root, notes);
    sort(notes.begin(), notes.end(), [](const Note& a, const Note& b) {
        return a.phoneNumber < b.phoneNumber;
        });
    for (const Note& note : notes) {
        cout << "Last Name: " << note.lastName << endl;
        cout << "First Name: " << note.firstName << endl;
        cout << "Phone Number: " << note.phoneNumber << endl;
        cout << "Birth Date: " << note.birthDate[0] << "/" << note.birthDate[1] << "/" << note.birthDate[2] << endl;
        cout << "--------------------------" << endl;
    }
}

bool isValidDate(int day, int month, int year) {
    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1) return false;
    int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) {
        daysInMonth[1] = 29;
    }
    if (day > daysInMonth[month - 1]) return false;
    return true;
}

int main() {
    TreeNode* root = nullptr;
    TreeNode* foundNode = nullptr;
    int choice;
    string filename;
    Note note;
    string lastName;

    while (true) {
        cout << "Menu:" << endl;
        cout << "1. Add note" << endl;
        cout << "2. Delete note" << endl;
        cout << "3. Display note by last name" << endl;
        cout << "4. Save to file" << endl;
        cout << "5. Load from file" << endl;
        cout << "6. Display all notes" << endl;
        cout << "7. Balance tree" << endl;
        cout << "8. Sort by phone number" << endl;
        cout << "9. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            cout << "Enter last name: ";
            getline(cin, note.lastName);
            cout << "Enter first name: ";
            getline(cin, note.firstName);
            cout << "Enter phone number: ";
            getline(cin, note.phoneNumber);
            do {
                cout << "Enter birth date (day month year): ";
                cin >> note.birthDate[0] >> note.birthDate[1] >> note.birthDate[2];
                if (!isValidDate(note.birthDate[0], note.birthDate[1], note.birthDate[2])) {
                    cout << "Invalid date. Please enter again." << endl;
                }
            } while (!isValidDate(note.birthDate[0], note.birthDate[1], note.birthDate[2]));
            cin.ignore();
            root = insertNode(root, note);
            break;
        case 2:
            cout << "Enter last name to delete: ";
            getline(cin, lastName);
            root = deleteNode(root, lastName);
            break;
        case 3:
            cout << "Enter last name to search: ";
            getline(cin, lastName);
            foundNode = searchByLastName(root, lastName);
            if (foundNode != nullptr) {
                cout << "Person found:" << endl;
                cout << "Last Name: " << foundNode->data.lastName << endl;
                cout << "First Name: " << foundNode->data.firstName << endl;
                cout << "Phone Number: " << foundNode->data.phoneNumber << endl;
                cout << "Birth Date: " << foundNode->data.birthDate[0] << "/" << foundNode->data.birthDate[1] << "/" << foundNode->data.birthDate[2] << endl;
            }
            else {
                cout << "Person with last name '" << lastName << "' not found." << endl;
            }
            break;
        case 4:
            cout << "Enter filename to save: ";
            getline(cin, filename);
            {
                ofstream outFile(filename);
                if (!outFile) {
                    cout << "Error opening file for writing!" << endl;
                }
                else {
                    saveToFile(root, outFile);
                    cout << "Data saved to file successfully." << endl;
                }
            }
            break;
        case 5:
            cout << "Enter filename to load: ";
            getline(cin, filename);
            {
                ifstream inFile(filename);
                if (!inFile) {
                    cout << "Error opening file for reading!" << endl;
                }
                else {
                    root = loadFromFile(root, inFile);
                    cout << "Data loaded from file successfully." << endl;
                }
            }
            break;
        case 6:
            cout << "Displaying all notes:" << endl;
            inOrderTraversal(root);
            break;
        case 7:
            root = balanceTree(root);
            cout << "Tree balanced successfully." << endl;
            break;
        case 8:
            sortAndPrintByPhoneNumber(root);
            cout << "Tree sorted by phone number successfully." << endl;
            break;
        case 9:
            cout << "Exiting..." << endl;
            freeMemory(root);
            return 0;
        default:
            cout << "Invalid choice, please try again." << endl;
        }
    }
    return 0;
}