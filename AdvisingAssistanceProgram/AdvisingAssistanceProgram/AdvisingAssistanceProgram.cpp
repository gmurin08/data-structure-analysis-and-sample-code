//============================================================================
// Name        : BinarySearchTree.cpp
// Author      : Gino Murin
//============================================================================

#include <iostream>
#include <time.h>
//used to store prerequisite data in a string vector
#include <vector>
//used to parse prerequisite string data into vector
#include <sstream>
//handles standardization of user input to uppercase
#include <string>
#include <algorithm>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

//define a structure to hold course information
struct Course {
    string course_number;
    string course_name;
    vector<string> prerequisites;
};

// Internal structure for tree node
struct Node {
    Course course;
    //Points to current node's left child
    Node* left;
    //points to current node's right child
    Node* right;

    //Default constructor
    Node() {
        //initializes nodes left and right children to nullptr
        left = nullptr;
        right = nullptr;
    }

    //Overload constructor with course object
    Node(Course aCourse) : Node() {
        this->course = aCourse;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    Course Search(string course_number);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    //root is equal to nullptr
    root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
    delete root;
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    // call inOrder fuction and pass root 
    this->inOrder(root);
}

/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {
    // Implements inserting a course into the tree
    // if root equarl to null ptr
    if (root == nullptr) {
        // root is equal to new node course
        root = new Node(course);
    }
    // else
    else {
        // add Node root and course
        this->addNode(root, course);
    }


}


/**
 * Search for a course
 */
Course BinarySearchTree::Search(string course_number) {
    // Searches the tree for a coourse number
    // set current node equal to root
    Node* current = root;
    // keep looping downwards until bottom reached or matching course number found
    while (current != nullptr) {
        // if match found, return current course number
        if (current->course.course_number.compare(course_number) == 0) {
            return current->course;
        }

        // if course number is smaller than current node then traverse left
        if (course_number.compare(current->course.course_number) < 0) {
            current = current->left;
        }
        // else larger so traverse right
        else {
            current = current->right;
        }
    }

    //Initialize and return empty course object when while loop breaks
    Course course;
    return course;
}

/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param course Course to be added
 */
void BinarySearchTree::addNode(Node* node, Course course) {
    // Inserts a course into the tree
    // if node is larger then add to left
    if (node->course.course_number.compare(course.course_number) > 0) {
        // if no left node
        if (node->left == nullptr) {
            // this node becomes left
            node->left = new Node(course);
        }
        else {
            // else recurse down the left node
            this->addNode(node->left, course);
        }
    }
    // else 
    else {
        // if no right node
        if (node->right == nullptr) {
            // this node becomes right
            node->right = new Node(course);
        }
        //else
        else {
            // recurse down the left node
            this->addNode(node->right, course);
        }
    }

}

/**
 * Inorder Traversal of BST
 *
 * @param node Current node in tree
 */
void BinarySearchTree::inOrder(Node* node) {
    // Inorder Traversal
    //if node is not equal to null ptr
    if (node != nullptr) {
        //InOrder to left
        inOrder(node->left);
        //output course name and number
        cout << node->course.course_number << ", " << node->course.course_name << endl;
        //InOder to right
        inOrder(node->right);
    }



}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the course information to the console (std::out)
 *
 * @param course struct containing the course info
 */
void displayCourse(Course course) {
    //output course number and name
    cout << course.course_number << ", " << course.course_name << endl;
    cout << "Prerequisites: ";
    //FOR EACH prerequisite in prerequistes vector
    for (int i = 0; i < course.prerequisites.size(); i++) {
        //IF index has reached the last element
        if (i == (course.prerequisites.size() - 1)) {
            //print prerequisite with no comma delimiter. endline
            cout << course.prerequisites.at(i) << "\n" << endl;
        }
        //Else print prerequisite with comma
        else {
            cout << course.prerequisites.at(i) << ", ";
        }
    }

    return;
}

/**
 * Load a CSV file containing courses into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the courses read
 */
void loadCourses(string csvPath, BinarySearchTree* bst) {

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    try {
        // for each line in csv file
        for (unsigned int i = 0; i < file.rowCount(); i++) {
            //Parse each line into a course struct
            Course course;
            course.course_number = file[i][0];
            course.course_name = file[i][1];
            //parses each comma separated course into course vector
            stringstream s_stream(file[i][2]);
            while (s_stream.good()) {
                string substr;
                getline(s_stream, substr, ',');
                course.prerequisites.push_back(substr);
            }

            // push the course to bst
            bst->Insert(course);
        }

    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
}


/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {
    // Define a binary search tree to hold all courses
    BinarySearchTree* bst{};

    //Define a course object to hold course data
    Course course;

    //Define a string to hold the course file path 
    string courseKey;

    //Define an integer to hold the user's menu choice
    int choice = 0;

    cout << "Welcome to the course planner. \n" << endl;

    while (choice != 9) {

        cout << "  1. Load Data Structure." << endl;
        cout << "  2. Print Course List." << endl;
        cout << "  3. Print Course." << endl;
        cout << "  9. Exit \n" << endl;
        cout << "What would you like to do? ";

        cin >> choice;

        switch (choice) {

        case 1:
            //create a binary search tree object to load courses into
            bst = new BinarySearchTree();

            //input the file path from user
            cout << "Enter the file path. - Default is './courses.csv': " << endl;
            cin >> courseKey;

            // Complete the method call to load courses
            loadCourses(courseKey, bst);

            cout << "CSV file " << courseKey << " loaded successfully.\n" << endl;

            break;

        case 2:
            cout << "Here is a sample schedule: \n" << endl;

            bst->InOrder();

            cout << "\n";

            break;

        case 3:
            
            cout << "What course do you want to know about?";
            cin >> courseKey;

            //standardize input to uppercase
            transform(courseKey.begin(), courseKey.end(), courseKey.begin(), ::toupper);

            course = bst->Search(courseKey);

            //IF the course object returned is not empty, print course info
            if (!course.course_number.empty()) {
                displayCourse(course);
            }
            else {
                cout << "Course number " << courseKey << " not found." << endl;
            }

            break;

        case 9:

            break;

        default:
            //Default case for input validation
            cout << choice << " is not a valid option.\n " << endl;
            break;
        }
    }

    cout << "Thank you for using course planner!" << endl;

    return 0;
}
