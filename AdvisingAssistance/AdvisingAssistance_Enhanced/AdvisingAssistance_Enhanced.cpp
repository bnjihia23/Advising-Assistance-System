//============================================================================
// Name        : AdvisingAssistance_Enhanced.cpp
// Author      : Brian Njihia
// Version     : 2.0
// Description : Advising Assistance with Binary Search Tree for CS 499
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include <cctype>

using namespace std;

// Holds course information
struct Course {
    string courseNumber;
    string courseName;
    vector<string> prerequisites;
};

// Removes leading and trailing whitespace from strings.
string Trim(const string& s) {
    size_t start = 0;
    while (start < s.size() && isspace(static_cast<unsigned char>(s[start]))) {
        ++start;
    }

    size_t end = s.size();
    while (end > start && isspace(static_cast<unsigned char>(s[end - 1]))) {
        --end;
    }

    return s.substr(start, end - start);
}

// Node structure for Binary Search Tree
struct BSTNode {
    Course course;
    BSTNode* left;
    BSTNode* right;

    BSTNode(const Course& courseData)
        : course(courseData), left(nullptr), right(nullptr) {
    }
};

// Binary Search Tree for storing and retrieving courses.
class CourseBST {
private:
    BSTNode* root;

    // Inserts a course into the tree using its course number as the key
    void insertNode(BSTNode*& node, const Course& course) {
        if (node == nullptr) {
            node = new BSTNode(course);
            return;
        }
        if (course.courseNumber < node->course.courseNumber) {
            insertNode(node->left, course);
        }
        else if (course.courseNumber > node->course.courseNumber) {
            insertNode(node->right, course);
        }
        // Duplicate course numbers are ignored.
    }

    // Performs in-order traversal to print courses alphabetically.
    void inOrderPrint(BSTNode* node) const {
        if (node == nullptr) return;

        inOrderPrint(node->left);
        cout << node->course.courseNumber << ", " << node->course.courseName << endl;
        inOrderPrint(node->right);
    }

    // Searches by courseNumber
    BSTNode* findNode(BSTNode* node, const string& courseNumber) const {
        if (!node) return nullptr;
        if (courseNumber == node->course.courseNumber) return node;
        if (courseNumber < node->course.courseNumber) return findNode(node->left, courseNumber);
        return findNode(node->right, courseNumber);
    }

    // Recursively frees all nodes to prevent memory leaks
    void destroyTree(BSTNode* node) {
        if (!node) return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

public:
    CourseBST() : root(nullptr) {}
    ~CourseBST() { destroyTree(root); }

    void InsertCourse(const Course& course) {
        insertNode(root, course);
    }

    // Prints all stored courses.
    void DisplaySortedCourses() const {
        if (!root) {
            cout << "No courses available. Please load a course file first." << endl;
            return;
        }
        inOrderPrint(root);
    }

    // Displays selected course and its prerequisites.
    void DisplayCourseDetails(string courseNumber) const {
        // Converts input to uppercase to avoid case-sensitivity
        transform(courseNumber.begin(), courseNumber.end(), courseNumber.begin(), ::toupper);

        BSTNode* node = findNode(root, courseNumber);
        if (!node) {
            cout << "Error: Course " << courseNumber << " not found." << endl;
            return;
        }

        cout << node->course.courseNumber << ", " << node->course.courseName << endl;

        if (node->course.prerequisites.empty()) {
            cout << "Prerequisites: None" << endl;
        }
        else {
            cout << "Prerequisites:" << endl;
            for (const string& prereq : node->course.prerequisites) {
                cout << "  - " << prereq << endl;
            }
        }
    }
};

// Loads and reads course data from CSV file
// Returns true on success and false if file could not be opened
bool LoadCourses(const string& filename, CourseBST& courseTree) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    string line;
    while (getline(file, line)) {
        // Makes sure the line isn't empty and has at least one comma
        if (line.empty() || line.find(',') == string::npos) {
            cout << "Warning: Malformed line in file. Skipping." << endl;
            continue;
        }

        // Extracts the course number
        Course course;
        size_t pos = line.find(',');
        course.courseNumber = Trim(line.substr(0, pos));
        line.erase(0, pos + 1);

        // Extracts the course name
        pos = line.find(',');
        if (pos == string::npos) {
            course.courseName = Trim(line);
            line.clear();
        }
        else {
            course.courseName = Trim(line.substr(0, pos));
            line.erase(0, pos + 1);
        }

        // Extracts the prerequisites
        while ((pos = line.find(',')) != string::npos) {
            string token = Trim(line.substr(0, pos));
            if (!token.empty()) course.prerequisites.push_back(token);
            line.erase(0, pos + 1);
        }

        if (!line.empty()) {
            string lastToken = Trim(line);
            if (!lastToken.empty()) course.prerequisites.push_back(lastToken);
        }

        // Adds the course to the BST
        courseTree.InsertCourse(course);
    }

    file.close();
    cout << "Courses successfully loaded from '" << filename << "'." << endl;
    return true;
}

// Displays the main menu options
void DisplayMenu() {
    cout << endl;
    cout << "1. Load Data Structure." << endl;
    cout << "2. Print Course List." << endl;
    cout << "3. Print Course." << endl;
    cout << "9. Exit." << endl;
    cout << "Please enter 1, 2, 3, or 9: ";
}

int main() {
    CourseBST courseTree;
    int choice = 0;

    cout << "Welcome to the course planner." << endl;

    // Main program loop
    while (true) {
        DisplayMenu();

        // Input validation
        if (!(cin >> choice)) {
            cout << "Error: Invalid input. Please enter a number (1, 2, 3, or 9)." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (choice == 9) {
            cout << "Thank you for using the course planner!" << endl;
            break;
        }

        switch (choice) {
        case 1: {
            // Keep asking for a filename until it loads or user cancels
            string filename;
            bool loaded = false;

            while (!loaded) {
                cout << "Enter the file name to load or Q to cancel: ";
                cin >> ws;         // Removes whitespace
                getline(cin, filename);

                if (filename == "q" || filename == "Q") {
                    cout << "File load canceled." << endl;
                    break;
                }

                loaded = LoadCourses(filename, courseTree);
                if (!loaded) {
                    cout << "Error: Unable to open '" << filename
                        << "'. Please check the name and try again." << endl;
                }
            }
            break;
        }
        case 2:
            courseTree.DisplaySortedCourses();
            break;
        case 3: {
            string courseNumber;
            cout << "What course do you want to know about? ";
            cin >> courseNumber;
            courseTree.DisplayCourseDetails(courseNumber);
            break;
        }
        default:
            cout << "Error: Invalid choice. Please enter 1, 2, 3, or 9." << endl;
        }
    }

    return 0;
}
