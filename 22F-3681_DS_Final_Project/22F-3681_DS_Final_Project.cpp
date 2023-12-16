#include<iostream>
#include<iomanip>
#include<string>
#include<Windows.h>
#include<cstdlib>
#include<algorithm>
#include<fstream>
using namespace std;


string fileName = "Dictionary.txt";
class Node {
public:
    string word, meaning;
    Node* left;
    Node* right;
    int height;

    Node(string wo, string mean) : word(wo),meaning(mean), left(nullptr), right(nullptr), height(1) {}
};
class AVLTree
{
public:
    Node* root;
    AVLTree(): root(NULL){}

    // Return Height
    int getheight(Node* node) 
    {
         if (node == nullptr) 
            return 0;
 
         return node->height;
    }  
    // Return Get Balance  
    int getBalance(Node* node) 
    {
        if (node == nullptr)
            return 0;
  
        return getheight(node->left) - getheight(node->right);
    }
    // Right Rotate
    Node* rightRotate(Node* k1)
    {
        Node* k2 = k1->left;
        k1->left = k2->right;
        k2->right = k1;
        k1->height = max(getheight(k1->left), getheight(k1->right)) + 1;
        k2->height = max(getheight(k2->left), getheight(k2->right)) + 1;
        return k2;
    }
    //Left Rotate
    Node* leftRotate(Node* k1)
    {
        Node* k2 = k1->right;
        k1->right = k2->left;
        k2->left = k1;
        k1->height = max(getheight(k1->left), getheight(k1->right)) + 1;
        k2->height = max(getheight(k2->left), getheight(k2->right)) + 1;
        return k2;
    }
    // Insert Word in AVL
    Node* insert(Node* temp, string word, string mean, bool &flag)
    {
        if (temp == NULL)
        {
            Node* newNode = new Node(word, mean);
            temp = newNode;
            return temp;
        }
        else
        {
            if (word < temp->word)
                temp->left = insert(temp->left, word, mean, flag);
            else if (word > temp->word)
                temp->right = insert(temp->right, word, mean, flag);
            else
            {
                flag = false;
               // cout << "The Word is Already Exist in Dictionary" << endl;
            }
            
        }

        temp->height = 1 + max(getheight(temp->left), getheight(temp->right));

        int balance = getBalance(temp);

        // Left Heavy
        if (balance > 1 && word < temp->left->word) {
            return rightRotate(temp);
        }

        // Right Heavy
        if (balance < -1 && word > temp->right->word) {
            return leftRotate(temp);
        }

        // Left Right Heavy
        if (balance > 1 && word > temp->left->word) {
            temp->left = leftRotate(temp->left);
            return rightRotate(temp);
        }

        // Right Left Heavy
        if (balance < -1 && word < temp->right->word) {
            temp->right = rightRotate(temp->right);
            return leftRotate(temp);
        }

        return temp;
    }
    // Find Min Value For DEletion  
    Node* minValueNode(Node* node)
    {
        Node* current = node;
        while (current->left != nullptr) {
            current = current->left;
        }
        return current;
    }
    // Deletion of Word in AVL
    Node* deleteNode(Node* root, string word, bool& flag) {
        if (root == nullptr) {
            return root;
        }

        if (word < root->word) {
            root->left = deleteNode(root->left, word, flag);
        }
        else if (word > root->word) {
            root->right = deleteNode(root->right, word, flag);
        }
        else {
            flag = true;
            if (root->left == NULL)
            {
                Node* temp = root->right;
                delete root;
                return temp;
            }
            else if (root->right == NULL)
            {
                Node* temp = root->left;
                delete root;
                return temp;
            }
            else {
                Node* temp2 = minValueNode(root->right);
                root->word = temp2->word;
                root->meaning = temp2->meaning;
                root->right = deleteNode(root->right, root->word, flag);
            }
        }

        if (root == nullptr) {
            return root;
        }

        root->height = 1 + max(getheight(root->left), getheight(root->right));

        int balance = getBalance(root);

        // Left Left
        if (balance > 1 && getBalance(root->left) >= 0) {
            return rightRotate(root);
        }

        // Left Right
        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }

        // Right Right
        if (balance < -1 && getBalance(root->right) <= 0) {
            return leftRotate(root);
        }

        // Right Left
        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }
    Node* UpdateWord(Node* temp, string val, string newWord, bool &flag)
    {
        if (temp == nullptr) {
            return temp;
        }

        if (val < temp->word)
        {
            temp->left = UpdateWord(temp->left, val, newWord, flag);
        }
        else if (val > temp->word)
        {
            temp->right = UpdateWord(temp->right, val, newWord, flag);
        }
        else if (val == temp->word)
        {
            temp->word = newWord;
            flag = true;
        }
        return temp;
    }
    Node* UpdateMean(Node* temp, string val, string newMean, bool &flag)
    {
        if (temp == nullptr) {
            return temp;
        }

        if (val < temp->word)
        {
            temp->left = UpdateMean(temp->left, val, newMean, flag);
        }
        else if (val > temp->word)
        {
            temp->right = UpdateMean(temp->right, val, newMean, flag);
        }
        else if (val == temp->word)
        {
            temp->meaning = newMean;
            flag = true;
        }
        return temp;
    }
    Node* SearchWord(Node* temp, string val, bool& flag)
    {
        if (temp == NULL)
            return temp;

        if (val < temp->word)
        {
            temp->left = SearchWord(temp->left, val, flag);
        }
        else if (val > temp->word)
        {
            temp->right = SearchWord(temp->right, val, flag);
        }
        else
        {
            cout << "\n\nWord is : " << temp->word << " \n\nMeaning : " << temp->meaning << endl << endl << endl;
            flag = true;
            return temp;

        }
        
        return temp;
    } 
    void wordSuggestion(Node* node, const string& input, int& count) {
        if (node == nullptr || count >= 10) {
            return;
        }

        wordSuggestion(node->left, input, count);

        // Check if the current node's key contains the input string
        if ((node->word.find(input) != -1) && count<10) {
            cout <<count+1 <<"- " << node->word << endl;
            count++;
        }

        wordSuggestion(node->right, input, count);
    }
    void insertFile(Node*root, ofstream& write)
    {
        if (root == NULL)
            return;
        else
        {
            insertFile(root->left, write);
            write << root->word << "\t\t\t" << root->meaning << endl;
            insertFile(root->right, write);

        }
    }
    void insertWord(string word, string mean, const string& filename)
    {
        bool flag = true;
        root = insert(root, word, mean, flag);
        if (flag)
        {
            cout<< "\n\nInserting "; Sleep(750); cout << ". "; Sleep(750); cout << ". "; Sleep(750); cout << ". " << endl << endl; Sleep(750);
            
            ofstream writeData(filename);
            if (!writeData.is_open()) {
                cout << "Error opening file for writing: " << filename << endl;
                return;
            }
            insertFile(root, writeData);

            cout << "Word is Successfully inserted in Dictionary" << endl << endl << endl;
        }
        else
            cout << "\n\nWord Already Exist in Dictionary" << endl;

    }
    bool search(string word, const string& filename)
    {
        bool flag = false;
        SearchWord(root, word, flag);
        if (!flag)
        {

            cout << "\n\n\nWord Not Found in Dictionary" << endl;
            char ch;
            cout << "\n\n\nWant to Add (Y/N) : "; cin >> ch;
            if (ch == 'Y' || ch == 'y')
            {
                string mean;
                cout << "\n\nEnter the Meaning of the Word     :   "; cin >> mean;
                insertWord(word, mean, filename);
            }
            return false;
        }
        else
            return true;
    }
    void Delete(string word, const string& filename)
    {
        cout << "\n\nDeleting "; Sleep(750); cout << ". "; Sleep(750); cout << ". "; Sleep(750); cout << ". " << endl << endl; Sleep(750);

        ofstream writeData(filename);
        bool flag = false;
        deleteNode(root, word, flag);
        if (flag)
        {
            cout << "\n\nWord is Deleted Successfully" << endl << endl;
            insertFile(root, writeData);
        }
    }
    void Update(string word, string newWord,int opt,const string& filename)
    {
        Node* temp = root;
        cout << "\n\nUpdating "; Sleep(750); cout << ". "; Sleep(750); cout << ". "; Sleep(750); cout << ". " << endl << endl; Sleep(750);

        ofstream writeData(filename);
        bool flag = false;
        
        if (opt == 1)
            UpdateWord(temp, word, newWord, flag);
        else
            UpdateMean(temp, word, newWord, flag);

        cout << endl;
        if (flag)
        {
            cout << "Word is Updated Successfully" << endl;
            insertFile(temp, writeData);
        }
        else
        {
            cout << "\n\n\nWord Not Found in Dictionary" << endl;
            char ch;
            cout << "\n\n\nWant to Add (Y/N) : "; cin >> ch;
            if (ch == 'Y' || ch == 'y')
            {
                string mean;
                cout << "\n\nEnter the Meaning of the Word     :   "; cin >> mean;
                insertWord(word, mean, filename);
            }
        }
    }

};

void title()
{
    cout << "************************************" << endl;
    cout << "*           FAST Dictionary        *" << endl;
    cout << "************************************" << endl;
}
void displayMenu()
{ 
    cout << "************************************" << endl;
    cout << "*           FAST Dictionary        *" << endl;
    cout << "************************************" << endl;

    cout << setw(24) << "1. Add Word" << endl;
    cout << setw(27) << "2. Search Word" << endl;
    cout << setw(27) << "3. Delete Word" << endl;
    cout << setw(27) << "4. Update Word" << endl;
    cout << setw(32) << "5. Word Suggestions" << endl;
    cout << setw(20) << "6. Exit" << endl;

    cout << "************************************" << endl;
}
void readFile(const string& filename, AVLTree& obj)
{
    // Open the file
    ifstream file(filename);
    bool flag = true;
    // Check if the file is open
    if (!file.is_open()) {
        cout << "Error opening file for reading: " << filename << endl;
        return;
    }

    // Read and output each line from the file
    string word, mean;
    srand(time(0));
    int count = 0;

    while (file >> word >> mean) 
    {
        obj.root = obj.insert(obj.root, word, mean, flag);

        /*int a = word[0];
        int b = rand() % (102 - 97 + 1) + 97;

        if (a == b)
        {
            count++;
            cout <<count<<" - " << word << endl;
        }*/
        
    }

    file.close();
}
//void check(AVLTree objDic)
//{
//    cout << "Number of Words " << ::count << endl;
//    objDic.inOrder(objDic.root);
//    cout << "Duplicate Number of Words " << ::count2 << endl;
//    cout << "Number of Nodes " << ::count3 << endl;
//}
int main()
{
LoadDictionary:
    system("cls");
    AVLTree objDic;
    title();
    cout << endl << endl;
    cout << "Dictionary is Loading "; Sleep(750); cout << ". "; Sleep(750); cout << ". "; Sleep(750); cout << ". " << endl << endl; Sleep(750);
   
    // Reading Data From File
    readFile(fileName, objDic);
   //  
   //check(objDic);



    cout << "Dictionary Loaded Successfully." << endl; Sleep(1000); 
    cout << endl;

    //system("cls");

    // System Start
    char ch;
    int opt;
    bool chk = true;
    do
    {
        string word, mean;
        
        do
        {
            displayMenu();
            cout << "Enter your Choice : "; cin >> opt;
            Sleep(750);
            system("cls");
            title();
            if (opt == 1)
            {
                cout << "\n\n<------------Add Word------------>" << endl << endl;
                cout << "Enter Word    :  "; cin >> word;
                cout << "Enter Meaning :  "; cin >> mean;
                objDic.insertWord(word, mean, fileName);
            }
            else if (opt == 2)
            {
                int ch;
                cout << "\n\n<-----------Search Word---------->" << endl << endl;
                cout << "1- Search Word" << endl;
                cout << "2- Delete Word" << endl;
                cout << "3- Update Word" << endl;
                cout << "4- Word Suggestions" << endl;
                cout << "\n\nEnter Choice  :      "; cin >> ch;
                cout << endl;
                if (ch == 1)
                {
                    cout << "\n\n<-----------Search Word---------->" << endl << endl;
                    cout << "Enter Word    :      "; cin >> word;
                    objDic.search(word, fileName);
                }
                else if (ch == 2)
                {
                    char ch;
                    cout << "\n\n<-----------Delete Word---------->" << endl << endl;
                    cout << "Enter Word    :  "; cin >> word;
                    objDic.search(word, fileName);

                    cout << "\n\n<-------------------------------->" << endl << endl;
                    cout << "Are You Sure (Y/N)   :   "; cin >> ch;
                    if (ch == 'Y' || ch == 'y')
                        objDic.Delete(word, fileName);
                }
                else if (ch == 3)
                {
                    string word2, mean2;
                    int opt=0;
                    cout << "\n\n<-----------Update Word---------->" << endl << endl;
                    cout << "Enter Word    :  "; cin >> word;
                    if (objDic.search(word, fileName))
                    {
                        string word2, mean2;
                        cout << "<-------------------------------->" << endl << endl;
                        cout << "1- Update Word" << endl;
                        cout << "2- Update Meaning" << endl;
                        cout << "\n\nEnter Choice  :    "; cin >> opt;
                        if (opt == 1)
                        {
                            cout << "\n\n<-------------------------------->" << endl << endl;
                            cout << "Enter New Word   :   "; cin >> word2;
                            objDic.Update(word, word2, 1, fileName);
                        }
                        else if (opt == 2)
                        {
                            cout << "\n\n<-------------------------------->" << endl << endl;
                            cout << "Enter New Meaning  :   "; cin >> mean2;
                            objDic.Update(word, mean2, opt, fileName);
                        }
                    }
                    else
                        cout << "Invalid Input" << endl;
                }

            }
            else if (opt == 3)
            {
                char ch;
                cout << "\n\n<-----------Delete Word---------->" << endl << endl;
                cout << "Enter Word    :  "; cin >> word;
                objDic.search(word, fileName);

                cout << "\n\n<-------------------------------->" << endl << endl;
                cout << "Are You Sure (Y/N)   :   "; cin >> ch;
                if (ch == 'Y' || ch == 'y')
                    objDic.Delete(word, fileName);
            }
            else if (opt == 4)
            {
                string word2, mean2;
                int opt;
                cout << "\n\n<-----------Update Word---------->" << endl << endl;
                cout << "Enter Word    :  "; cin >> word;
                if (objDic.search(word, fileName))
                {
                    string word2, mean2;
                    cout << "<-------------------------------->" << endl << endl;
                    cout << "1- Update Word" << endl;
                    cout << "2- Update Meaning" << endl;
                    cout << "\n\nEnter Choice  :    "; cin >> opt;
                    if (opt == 1)
                    {
                        cout << "\n\n<-------------------------------->" << endl << endl;
                        cout << "Enter New Word   :   "; cin >> word2;
                        objDic.Update(word, word2, 1, fileName);
                    }
                    else if (opt == 2)
                    {
                        cout << "\n\n<-------------------------------->" << endl << endl;
                        cout << "Enter New Meaning  :   "; cin >> mean2;
                        objDic.Update(word, mean2, opt, fileName);
                    }
                }


            }
            else if (opt == 5)
            {
                int Wordcount = 0;
                cout << "\n\n<-----------Word Suggestions---------->" << endl << endl;
                cout << "Enter Word    :  "; cin >> word;
                cout << endl << endl;
                cout << "<-----------Suggestions---------->" << endl << endl;

                objDic.wordSuggestion(objDic.root, word, Wordcount);
                char ch;
                cout << "\n\nWant to Perform Some Actions (Y/N) : "; cin >> ch;
                if (ch == 'Y' || ch == 'y')
                {

                    int opt;
                    cout << "<-------------------------------->" << endl << endl;
                    cout << "1- Update Word" << endl;
                    cout << "2- Delete Word" << endl;
                    cout << "\n\nEnter Choice  :    "; cin >> opt;

                    if (opt == 1)
                    {
                        string word, word2, mean2;
                        int opt;
                        cout << "\n\n<-----------Update Word---------->" << endl << endl;
                        cout << "Enter Word    :  "; cin >> word;
                        system("cls");
                        title();
                        if (objDic.search(word, fileName))
                        {
                            string word2, mean2;
                            cout << "<-------------------------------->" << endl << endl;
                            cout << "1- Update Word" << endl;
                            cout << "2- Update Meaning" << endl;
                            cout << "\n\nEnter Choice  :    "; cin >> opt;
                            if (opt == 1)
                            {
                                cout << "\n\n<-------------------------------->" << endl << endl;
                                cout << "Enter New Word   :   "; cin >> word2;
                                objDic.Update(word, word2, 1, fileName);
                            }
                            else if (opt == 2)
                            {
                                cout << "\n\n<-------------------------------->" << endl << endl;
                                cout << "Enter New Meaning  :   "; cin >> mean2;
                                objDic.Update(word, mean2, opt, fileName);
                            }
                        }
                    }
                    if (opt == 2)
                    {
                        char ch;
                        cout << "\n\n<-----------Delete Word---------->" << endl << endl;
                        cout << "Enter Word    :  "; cin >> word;
                        system("cls");
                        title();
                        objDic.search(word, fileName);

                        cout << "\n\n<-------------------------------->" << endl << endl;
                        cout << "Are You Sure (Y/N)   :   "; cin >> ch;
                        if (ch == 'Y' || ch == 'y')
                            objDic.Delete(word, fileName);
                    }

                }
            }
            else if (opt == 6)
            {
                char ch;
                cout << "\n\n<-----------Exit Program---------->" << endl << endl;
                cout << "Are You Sure (Y/N)   :   "; cin >> ch;
                cout << "\n\n<-------------------------------->" << endl << endl;
                
                if (ch == 'Y' || ch == 'y')
                {
                    cout << "Thank you for Using Dictionary..." << endl;
                    cout << "\nStay Connected Stay Uptodated..." << endl;
                    cout << "\n\nExiting "; Sleep(750); cout << ". "; Sleep(750); cout << ". "; Sleep(750); cout << ". " << endl << endl; Sleep(750);
                    
                    chk = false;
                }
            }
            else
            {
                cout << "Wrong Selection. Please Select Again" << endl;
                Sleep(1000);
                system("cls");
            }
               
        }while (opt > 6);
        
       
        cout << endl;
        if (chk)
        {
            cout << "Do You Want to Continue. (Y/N) : "; cin >> ch; system("cls");
        }
        else
            ch = 'n';
            
    } while ((ch == 'Y' || ch == 'y') && chk);

    cout << "Exited Successfully" << endl << endl;

    cout << "Want to Open Dictionary Again (Y/N)   :   "; cin >> ch;
    if (ch == 'Y' || ch == 'y')
        goto LoadDictionary;
    
    cout << endl << endl;
    system("Pause");
    return 0;
}