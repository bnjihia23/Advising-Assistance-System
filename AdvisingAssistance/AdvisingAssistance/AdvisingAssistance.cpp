//============================================================================
// Name        : AdvisingAssistance.cpp
// Author      : Brian Njihia
// Version     : 1.1
// Copyright   : Copyright © 2024 SNHU COCE
// Description : CS 300 Project Two Advising Assistance Code
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// Struct to store course details
struct Course {
    string courseNumber;
    string courseName;
    vector<string> prerequisites;
};

// Node structure for LinkedList
struct Node {
    Course course;
    Node* next;
    Node(Course courseData) : course(courseData), next(nullptr) {}
};

// LinkedList class for storing courses
class LinkedList {
private:
    Node* head;

public:
    LinkedList() : head(nullptr) {}

    void Append(Course course) {
        Node* newNode = new Node(course);
        if (!head) {
            head = newNode;
        }
        else {
            Node* current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode;
        }
    }

    void DisplaySortedCourses() {
        if (!head) {
            cout << "No courses available. Please load a course file first." << endl;
            return;
        }

        vector<Course> sortedCourses;
        Node* current = head;

        // Collect courses into a vector
        while (current) {
            sortedCourses.push_back(current->course);
            current = current->next;
        }

        // Sort the vector by course number
        sort(sortedCourses.begin(), sortedCourses.end(), [](const Course& a, const Course& b) {
            return a.courseNumber < b.courseNumber;
            });

        // Display sorted courses
        for (const Course& course : sortedCourses) {
            cout << course.courseNumber << ", " << course.courseName << endl;
        }
    }

    void DisplayCourseDetails(string courseNumber) {
        transform(courseNumber.begin(), courseNumber.end(), courseNumber.begin(), ::toupper);
        Node* current = head;
        while (current) {
            if (current->course.courseNumber == courseNumber) {
                cout << current->course.courseNumber << ", " << current->course.courseName << endl;
                if (current->course.prerequisites.empty()) {
                    cout << "Prerequisites: None" << endl;
                }
                else {
                    cout << "Prerequisites: " << endl;
                    for (const string& prereq : current->course.prerequisites) {
                        cout << "  - " << prereq << endl;
                    }
                }
                return;
            }
            current = current->next;
        }
        cout << "Error: Course " << courseNumber << " not found." << endl;
    }
};

// Function to load courses from a CSV file
void LoadCourses(const string& filename, LinkedList& courseList) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Unable to open file. Please check if the file exists and you have read permissions." << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        size_t pos = 0;
        string token;
        Course course;

        // Validate line format
        if (line.empty() || line.find(',') == string::npos) {
            cout << "Error: Malformed line in file. Skipping." << endl;
            continue;
        }

        // Extract course number
        pos = line.find(',');
        course.courseNumber = line.substr(0, pos);
        line.erase(0, pos + 1);

        // Extract course name
        pos = line.find(',');
        course.courseName = line.substr(0, pos);
        line.erase(0, pos + 1);

        // Extract prerequisites
        while ((pos = line.find(',')) != string::npos) {
            token = line.substr(0, pos);
            course.prerequisites.push_back(token);
            line.erase(0, pos + 1);
        }
        if (!line.empty()) {
            course.prerequisites.push_back(line);
        }

        courseList.Append(course);
    }
    file.close();
    cout << "Courses successfully loaded." << endl;
}

// Main menu function
void DisplayMenu() {
    cout << "1. Load Data Structure." << endl;
    cout << "2. Print Course List." << endl;
    cout << "3. Print Course." << endl;
    cout << "9. Exit." << endl;
    cout << "Please enter 1, 2, 3, or 9." << endl;
}

int main() {
    LinkedList courseList;
    int choice = 0;

    cout << "Welcome to the course planner." << endl;

    while (choice != 9) {
        DisplayMenu();

        // Input validation for menu choice
        if (!(cin >> choice)) {
            cout << "Error: Invalid input. Please enter 1, 2, 3, or 9." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
        }

        if (choice < 1 || choice > 9) {
            cout << "Error: Please enter a valid option (1, 2, 3, or 9)." << endl;
            continue;
        }

        switch (choice) {
        case 1: {
            string filename;
            cout << "Enter the file name to load: ";
            cin >> filename;
            LoadCourses(filename, courseList);
            break;
        }
        case 2:
            courseList.DisplaySortedCourses();
            break;
        case 3: {
            string courseNumber;
            cout << "What course do you want to know about? ";
            cin >> courseNumber;
            courseList.DisplayCourseDetails(courseNumber);
            break;
        }
        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;
        default:
            cout << "Error: " << choice << " is not a valid option." << endl;
        }
    }

    return 0;
}
