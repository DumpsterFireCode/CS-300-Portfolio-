//============================================================================
// Name        : Project 2
// Author      : Andrei Kourouchin
// Version     : 1.0
//============================================================================

#include <algorithm>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>

using namespace std;

//Initialize course sturcture
struct Course {
    string courseId;
    string title;
    vector<string> prerequisites;

    Course() {

    }
};

//Initialize node structure 
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node() {
        left = right = nullptr;
    }

    Node(Course c_course) : Node() {
        this->course = c_course;
    }
};

//Binary Search Tree Class Definition

class BinarySearchTree {
private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    Course Search(string courseId);
    void Insert(Course course);
};
/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {

    //root is equal to nullptr
    root = nullptr;

}

BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node

}
/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    // call inOrder fuction and pass root 
    this->inOrder(root);
}


Course BinarySearchTree::Search(string courseId) {

    // set current node equal to root
    Node* current = root;

    // keep looping downwards until bottom reached or matching courseId found
    while (current != nullptr) {
        // if match found, return current course
        if (current->course.courseId.compare(courseId) == 0) {
            return current->course;
        }

        // if course is smaller than current node then traverse left
        if (courseId.compare(current->course.courseId) < 0) {
            current = current->left;
        }
        else {
            // else larger so traverse right
            current = current->right;
        }
    }



    Course course;
    return course;
}


void BinarySearchTree::Insert(Course course) {
    // if the root is equal to nullptr
    if (root == nullptr) {
        // root is equal to new node course
        root = new Node(course);
    }
    else {
        // add Node root and course
        this->addNode(root, course);
    }
}



void BinarySearchTree::addNode(Node* node, Course course) {
    // if node is larger then add to left
    if (node->course.courseId.compare(course.courseId) > 0) {

        // if no left node
        if (node->left == nullptr) {

            //this node becomes left
            node->left = new Node(course);
        }
        else {

            //else recurse down the left node
            this->addNode(node->left, course);
        }
    }
    else {

        //if no right node
        if (node->right == nullptr) {

            // this node becomes right
            node->right = new Node(course);
        }
        else {

            //recurse down the left node
            this->addNode(node->right, course);
        }
    }




}

void BinarySearchTree::inOrder(Node* node) {

    //if node is not equal to null ptr
    if (node != nullptr) {

        //InOrder not left
        inOrder(node->left);

        //output Course ID and Name
        cout << node->course.courseId << ", " << node->course.title << endl;
        

        //InOrder right
        inOrder(node->right);

    }

}

void loadCourses(string filePath, BinarySearchTree* bst) {
    int courseId = 1;
    ifstream inFile(filePath);
    string str;
    vector<string> courses;
    vector<string> results;

    //If inFIle is empty, exit because file was not loaded
    if (!inFile) {
        cout << "Unable to open file " << filePath;
        exit(1);
    }


    try {
        //For all lines in file add lines to courses vector
        for (string line; getline(inFile, line);) {
            // line =  CSCI400, Large Software Development, CSCI301, CSCI350
            courses.push_back(line);// add line to courses
        }
        //for the words in each vector entry
        for (string words : courses) {
            // words =  CSCI400, Large Software Development, CSCI301, CSCI350
            stringstream ss(words);
            string str;
            Course course;


            // push the first thing on the line
            getline(ss, str, ',');
            course.courseId = str;
            // push the second thing on the line
            getline(ss, str, ',');
            course.title = str;
            // push back anything else
            while (getline(ss, str, ',')) {
                course.prerequisites.push_back(str);

            }
            if (course.courseId.empty() || course.title.empty() ) {
                cout << "There is a problem with the entry for " << course.courseId <<
                    ". " << endl << "Please confirm there are at least two values on the line and the text is properly formatted." << endl;
                break;
            }

            bst->Insert(course);
        }
    }
    catch (int e) {
        cout << "Could not load courses.";
    }
}


int main() {

    //Initialize some housekeeping variables
    string searchCourseId;
    string filePath = "ABCU_Advising_Program_Input.txt";

    BinarySearchTree* bst;
    
    Course course;

    bst = new BinarySearchTree();

    cout << "Welcome to the course planner." << endl;
    int choice = 0;

    //Start while loop for menu loop
    while (choice != 9) {
        
        cout << endl << "  1. Load Data Structure" << endl;
        cout << "  2. Print Course List" << endl;
        cout << "  3. Print Course" << endl;
        cout << "  9. Exit" << endl;
        cout << endl << "What would you like to do? ";
        cin >> choice;
        string courseSearch;
        switch (choice) {

        case 1:
           
            loadCourses(filePath, bst);
            break;
        case 2:
            //Print a sample schedule InOrder
            cout << endl << "Here is a sample schedule:" << endl;
            bst->InOrder();
            cout << endl;
            break;
        case 3:
            //Get course info from user and print out results after search
            cout << endl << "Please enter the name of the course you'd like to search for." << endl;
            
            cin >> courseSearch;
            course = bst->Search(courseSearch);

            //If there are no prereqs, let user know, else print prereqs
            if (!course.courseId.empty()) {
                cout << course.courseId << ", " << course.title << endl;
                if (course.prerequisites.empty()) {
                    cout << "There are no prerequisites for this course.";
                }
                else {
                    cout << "Prerequisites: ";
                    for (int i = 0; i < course.prerequisites.size(); i++)
                        std::cout << course.prerequisites.at(i) << ' ' ;
                }

                }
            else {
                //If we do not find course, let user know
                cout << "Course Id " << courseSearch << " not found." << endl;
            }
            cout << endl;
            break;
        case 9:
            break;

        default:
            cout << choice << " is not a valid selection" << endl;

        }
    }

    cout << "Goodbye." << endl;
    return 0;
}
