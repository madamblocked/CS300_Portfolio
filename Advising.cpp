//============================================================================
// Name        : Advising.cpp
// Author      : Adam Conger
// Version     : 1.0
// Copyright   : Copyright © 2022
// Description : Project Two CS300
//============================================================================

#include <algorithm>
#include <iostream>
#include <time.h>
#include <string>
#include <iterator>

#include "CSVparser.h"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Course {
    string courseId; // unique identifier
    string courseTitle;
    string prereq = "";
    string prereq_two = "";
    double amount;
    Course() {
        amount = 0.0;
    }
};

//============================================================================
// Linked-List class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a linked-list.
 */
class LinkedList {

private:
    //Internal structure for list entries, housekeeping variables
    struct Node {
        Course course;
        Node* next;

        // default constructor
        Node() {
            next = nullptr;
        }

        // initialize with a course
        Node(Course aCourse) {
            course = aCourse;
            next = nullptr;
        }
    };

    Node* head;
    Node* tail;
    int size = 0;


public:
    LinkedList();
    virtual ~LinkedList();
    void ListSort();
    void Append(Course course);
    void PrintList();
    Course Search(string courseId);
    int Size();
};

/**
 * Default constructor
 */
LinkedList::LinkedList() {
    //Initialize housekeeping variables
    head = nullptr;
    tail = nullptr;
}

/**
 * Destructor
 */
LinkedList::~LinkedList() {
    // start at the head
    Node* current = head;
    Node* temp;

    // loop over each node, detach from list then delete
    while (current != nullptr) {
        temp = current; // hang on to current node
        current = current->next; // make current the next node
        delete temp; // delete the orphan node
    }
}

/**
 * Sort the list
 
void LinkedList::ListSort() {
    //initialize linked list and array
    const int size = 1;
    Node* current = head;
    string array[size];
    array->resize(Size());
    //for each item in the linked list
    for (int i = 0; i < Size(); ++i) {
        //assign array element with current node
        array[i] = current->course.courseId;
        //loop over next node
        current = current->next;
    }
    //sort array
    std::sort(std::begin(array), std::end(array));
    //start at head and rewrite array back to linked list
    current = head;
    for (int i = 0; i < size; ++i) {
        current->course.courseId = array[i];
        current = current->next;
    }
}
**/

/**
 * Append a new bid to the end of the list
 */
void LinkedList::Append(Course course) {
    //Implement append logic
    //Create new node
    Node* newNode = new Node(course);

    if (head == nullptr) {
        head = newNode;
        tail = newNode;
    }
    else {
        //Make current tail node point to the next node
        tail->next = newNode;
        tail = newNode;
    }
    //increase size count
    ++size;
}


/**
 * Simple output of all bids in the list
 */
void LinkedList::PrintList() {
    // Implement print logic in alphanumeric order
    // start at the head
    Node* current = head;
    // while loop over each node looking for a match
    while (current != nullptr) {
        cout << current->course.courseId << ": " << current->course.courseTitle
            << " | Prerequisites | 1: " << current->course.prereq << " 2: " << current->course.prereq_two << endl;
        current = current->next;
    }
}

/**
 * Search for the specified courseId
 *
 * @paramcoursedId The course id to search for
 */
Course LinkedList::Search(string courseId) {
    // Implement search logic

    Course* course = new Course();
    course->courseId = "0";
    Node* current = head;

    // start at the head of the list
    current = head;
    while (current != nullptr) {
        if (courseId == current->course.courseId) {
            return current->course;
        }
        current = current->next;
    }

    // check for the case if current becomes nullptr
    if (current == nullptr) {
        return *course;
    }
}

/**
 * Returns the current size (number of elements) in the list
 */
int LinkedList::Size() {
    return size;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the course information
 *
 * @param course struct containing the course info
 */
void displayCourse(Course course) {
    cout << course.courseId << "; " << course.courseTitle << "; Prerequisites: " << course.prereq
        << "; " << course.prereq_two << endl;
    return;
}

/**
 * Prompt user for course information
 *
 * @return Course struct containing the course info
 */
Course getCourse() {
    Course course;

    cout << "Enter Course Id: ";
    cin.ignore();
    getline(cin, course.courseId);

    cout << "Enter Course title: ";
    getline(cin, course.courseTitle);

    cout << "Enter Course Prerequisite 1: ";
    cin >> course.prereq;

    cout << "Enter Course Prerequisite 2: ";
    cin >> course.prereq_two;
    return course;
}

/**
 * Load a CSV file containing courses into a LinkedList
 *
 * @return a LinkedList containing all the courses read
 */
void loadCourses(string csvPath, LinkedList* list) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser
    csv::Parser file = csv::Parser(csvPath);

    try {
        // loop to read rows of a CSV file
        for (int i = 0; i < file.rowCount(); i++) {

            // initialize a course using data from current row (i)
            Course course;
            course.courseId = file[i][0];
            course.courseTitle = file[i][1];
            course.prereq = file[i][2];
            course.prereq_two = file[i][3];


            // add this course to the end
            list->Append(course);
        }
    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 *
 * @param arg[1] path to CSV file to load from (optional)
 * @param arg[2] the bid Id to use when searching the list (optional)
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, courseKey;
    switch (argc) {
    case 3:
        csvPath = argv[1];
        courseKey = argv[2];
        break;
    default:
        csvPath = "ABCU_Courses_Feb_2022.csv";
        courseKey = "CSCI300";
    }

    clock_t ticks;

    LinkedList courseList;

    Course course;

    int choice = 0;
    while (choice != 9) {
        cout << "ABCU Course Advising Menu:" << endl;
        cout << "  1. Add a new Course" << endl;
        cout << "  2. Load CSV file" << endl;
        cout << "  3. Display Course Information" << endl;
        cout << "  4. Search for Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            course = getCourse();
            courseList.Append(course);
            displayCourse(course);

            break;

        case 2:
            ticks = clock();

            loadCourses(csvPath, &courseList);

            cout << courseList.Size() << " courses read." << endl;

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "That took: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds." << endl;

            break;

        case 3:
            courseList.PrintList();

            break;

        case 4:
            course = courseList.Search(courseKey);

            if (!course.courseId.empty()) {
                displayCourse(course);
            }
            else {
                cout << "Course Id " << courseKey << " not found." << endl;
            }

            break;

        }
    }

    cout << "Good bye." << endl;

    return 0;
}
