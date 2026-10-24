//============================================================================
// Name        : CoursePlanner.cpp
// Author      : Joey Bruno
// Version     : 1.1
// Copyright   : Copyright � 2023 SNHU COCE
// Description : CS-499 Enhancement Algorithms & Data Structures
//============================================================================

#include <algorithm>
#include <climits>
#include <fstream>
#include <iostream>
#include <string> // atoi
#include <vector>

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// Initial size of table size
const unsigned int DEFAULT_SIZE = 179;

// forward declarations
double strToDouble(string str, char ch);

int main(int argc, char *argv[]);

// define a structure to hold Course information
struct Course
{
    string courseCode; // unique identifier
    string courseName;
    vector<string> prereqList;
};

// define a structure to hold Teacher information
struct Teacher
{
    string teacherName, teacherRating;
    string teacherCode; // unique identifier
    Course course;
};

// Initialize the displayCourse function for line 234
void displayCourse(Course course);

// Initialize the displayCourse function for line 246
void displayTeacher(Teacher teacher);

//============================================================================
// Hash Table class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a hash table with chaining with Teacher Information.
 */
class HashTableTeachers
{
private:
    // Define structures to hold teachers
    struct Node
    {
        Teacher teacher;
        unsigned int key;
        Node *next;

        Node()
        {
            key = UINT_MAX;
            next = nullptr;
        }

        Node(Teacher aTeacher) : Node()
        {
            teacher = aTeacher;
        }

        // initialize with a Course and a Key
        Node(Teacher aTeacher, unsigned int aKey) : Node(aTeacher)
        {
            key = aKey;
        }
    };

    std::vector<Node> teacherNodes;
    std::vector<Teacher> teacherList;

    unsigned int tableSize = DEFAULT_SIZE;

    unsigned int hash(int key);

public:
    HashTableTeachers();
    HashTableTeachers(unsigned int size);
    virtual ~HashTableTeachers();
    void InsertTeacher(Teacher teacher);
    void SortAllTeachers();
    void PrintAllTeachers();
    Teacher SearchTeacher(string teacherCode);
    size_t Size();
};

/**
 * Define a class containing data members and methods to
 * implement a hash table with chaining with Course Information.
 */
class HashTableCourses
{
private:
    // Define structures to hold courses
    struct Node
    {
        Course course;
        unsigned int key;
        Node *next;

        // default constructor
        Node()
        {
            key = UINT_MAX;
            next = nullptr;
        }

        // initialize with a Course
        Node(Course aCourse) : Node()
        {
            course = aCourse;
        }

        // initialize with a Course and a Key
        Node(Course aCourse, unsigned int aKey) : Node(aCourse)
        {
            key = aKey;
        }
    };

    std::vector<Node> courseNodes;
    std::vector<Course> courseList;

    unsigned int tableSize = DEFAULT_SIZE;

    unsigned int hash(int key);

public:
    HashTableCourses();
    HashTableCourses(unsigned int size);
    virtual ~HashTableCourses();
    void InsertCourse(Course course);
    void SortAllCourses();
    void PrintAllCourses();
    Course SearchCourse(string courseCode);
    size_t Size();
};

// Default constructor
HashTableCourses::HashTableCourses()
{
    // Initialize the Constructor towards the size of tableSize
    courseNodes.resize(tableSize);
}

/**
 * Constructor for specifying size of the table
 * Use to improve efficiency of hashing algorithm
 * by reducing collisions without wasting memory.
 *
 * @param size: the size of the HashTable
 */
HashTableCourses::HashTableCourses(unsigned int size)
{
    this->tableSize = size;
    courseNodes.resize(tableSize);
}

// Destructor
HashTableCourses::~HashTableCourses()
{
    // To Free All Storage When the Course Class is Destroyed
    courseNodes.erase(courseNodes.begin(), courseNodes.end());
}

/**
 * Calculate the hash value of a given key.
 * Note that key is specifically defined as
 * unsigned int to prevent undefined results
 * of a negative list index.
 *
 * @param key: The key to hash
 * @return: The calculated hash
 */
unsigned int HashTableCourses::hash(int key)
{
    // Hash Value of a given key
    return key % tableSize;
}

// Default constructor
HashTableTeachers::HashTableTeachers()
{
    // Initialize the Constructor towards the size of tableSize
    teacherNodes.resize(tableSize);
}

/**
 * Constructor for specifying size of the table
 * Use to improve efficiency of hashing algorithm
 * by reducing collisions without wasting memory.
 *
 * @param size: the size of the HashTable
 */
HashTableTeachers::HashTableTeachers(unsigned int size)
{
    this->tableSize = size;
    teacherNodes.resize(tableSize);
}

// Destructor
HashTableTeachers::~HashTableTeachers()
{
    // To Free All Storage When the Teacher Class is Destroyed
    teacherNodes.erase(teacherNodes.begin(), teacherNodes.end());
}

/**
 * Calculate the hash value of a given key.
 * Note that key is specifically defined as
 * unsigned int to prevent undefined results
 * of a negative list index.
 *
 * @param key: The key to hash
 * @return: The calculated hash
 */
unsigned int HashTableTeachers::hash(int key)
{
    // Hash Value of a given key
    return key % tableSize;
}

// Sort the course list alphabetically
void HashTableCourses::SortAllCourses()
{

    for (unsigned int i = 0; i < courseNodes.size(); ++i)
    {
        Node *node = &courseNodes[i];

        // While Node is not null AND the Node Key is not Max
        while (node != nullptr && node->key != UINT_MAX)
        {
            courseList.push_back(node->course);

            // Move the current node to the next
            node = node->next;
        }
    }

    // sort based on the course list
    sort(courseList.begin(), courseList.end(), [](const Course &a, const Course &b)
         { return a.courseCode < b.courseCode; });
}

/**
 * Search for the specified courseCode
 *
 * @param courseCode The course Code to search for
 */
Course HashTableCourses::SearchCourse(string courseCode)
{
    // Goal: Search for a Course Code the User Searches For
    Course course;

    unsigned key = hash(atoi(courseCode.c_str()));
    Node *node = &(courseNodes.at(key));

    // while node not equal to nullptr
    while (node != nullptr)
    {
        // if the current node matches, return it
        if (node->key != UINT_MAX && node->course.courseCode.compare(courseCode) == 0)
        {
            return node->course;
        }
        // node is equal to next node
        node = node->next;
    }

    return course;
}

// Print all courses
void HashTableCourses::PrintAllCourses()
{
    // Goal: Print All Courses

    // iterate over the course list
    for (const auto &course : courseList)
    {
        // put each course iteration to displayCourse
        displayCourse(course);
    }
}

/**
 * Insert a course
 *
 * @param course The course to insert
 */
void HashTableCourses::InsertCourse(Course course)
{
    // Goal: Insert a Course into the hash table

    unsigned key = hash(atoi(course.courseCode.c_str()));
    // retrieve node using key
    Node *lastNode = &(courseNodes.at(key));

    // adding old node key to UNIT_MAX, set to key, set old node to course and old node next to null pointer
    if (lastNode->key == UINT_MAX)
    {
        lastNode->key = key;
        lastNode->course = course;
        lastNode->next = nullptr;
    }

    // else find the next open node
    else
    {
        while (lastNode->next != nullptr)
        {
            // add new newNode to end
            lastNode = lastNode->next;
        }
        // Initialize a new node for the next node
        lastNode->next = new Node(course, key);
    }
}

/**
 * Insert a teacher
 *
 * @param teacher The teacher to insert
 */
void HashTableTeachers::InsertTeacher(Teacher teacher)
{
    // Goal: Insert a Teacher into the hash table

    unsigned key = hash(atoi(teacher.teacherCode.c_str()));
    // retrieve node using key
    Node *lastNode = &(teacherNodes.at(key));

    // adding old node key to UNIT_MAX, set to key, set old node to course and old node next to null pointer
    if (lastNode->key == UINT_MAX)
    {
        lastNode->key = key;
        lastNode->teacher = teacher;
        lastNode->next = nullptr;
    }

    // else find the next open node
    else
    {
        while (lastNode->next != nullptr)
        {
            // add new newNode to end
            lastNode = lastNode->next;
        }
        // Initialize a new node for the next node
        lastNode->next = new Node(teacher, key);
    }
}

// Sort the teacher list alphabetically
void HashTableTeachers::SortAllTeachers()
{

    for (unsigned int i = 0; i < teacherNodes.size(); ++i)
    {
        Node *node = &teacherNodes[i];

        // While Node is not null AND the Node Key is not Max
        while (node != nullptr && node->key != UINT_MAX)
        {
            teacherList.push_back(node->teacher);

            // Move the current node to the next
            node = node->next;
        }
    }

    // sort based on the course list
    sort(teacherList.begin(), teacherList.end(), [](const Teacher &a, const Teacher &b)
         { return a.teacherCode < b.teacherCode; });
}

// Print all Teachers
void HashTableTeachers::PrintAllTeachers()
{
    // iterate over the teacher list
    for (const auto &teacher : teacherList)
    {
        // put each teacher iteration to displayTeacher
        displayTeacher(teacher);
    }
}

/**
 * Search for the specified teacherCode
 *
 * @param teacherCode The Teacher Code to search for
 */
Teacher HashTableTeachers::SearchTeacher(string teacherCode)
{
    // Goal: Search for a Teacher Code the User Searches For
    Teacher teacher;

    unsigned key = hash(atoi(teacherCode.c_str()));
    Node *node = &(teacherNodes.at(key));

    // while node not equal to nullptr
    while (node != nullptr)
    {
        // if the current node matches, return it
        if (node->key != UINT_MAX && node->teacher.teacherCode.compare(teacherCode) == 0)
        {
            return node->teacher;
        }
        // node is equal to next node
        node = node->next;
    }

    return teacher;
}

//============================================================================
// Static methods used for testing
//============================================================================

/** Split Comma Delimitter
* @param lineFeed String line to split through
**/
vector<string> Split(string lineFeed)
{
    char delimiter = ',';
    vector<string> lineTokens;
    string temp = "";

    lineFeed += delimiter; // to read last word

    // need to check if the lineFeed is empty
    for (int i = 0; i < lineFeed.length(); i++)
    {
        if (lineFeed[i] == delimiter)
        {
            // store the word in the tokens vector
            lineTokens.push_back(temp);
            temp = "";
            i++;
        }
        temp += lineFeed[i];
    }
    return lineTokens;
}

/**
 * @param course struct containing the course info
 **/
void displayCourse(Course course)
{
    // Goal: Show course information

    cout << "Course Code: " << course.courseCode << ", Course Name: " << course.courseName << endl;

    cout << "Prerequisites: ";

    // If Prerequisite List is less than 3 -> Print None
    if (course.prereqList.size() < 3)
    {
        cout << "None" << endl;
    }
    // Else -> iterate over the list based on the size, and print each item
    else
    {
        for (unsigned int i = 1; i < course.prereqList.size(); i++)
        {
            cout << course.prereqList.at(i);

            // if the size is greater than 1 AND it is less than the size minus 1 -> insert a a comma for the second prerequisite
            if ((course.prereqList.size() > 1) && (i < course.prereqList.size() - 1))
            {
                cout << ", ";
            }
        }
        cout << endl;
    }
}

/**
 * @param teacher structure containing the teacher info
 **/

void displayTeacher(Teacher teacher)
{
    // Goal: Show Teacher information

    cout << "Teacher: " << teacher.teacherName << ", Teacher Code: " << teacher.teacherCode << ", Teacher Rating: " << teacher.teacherRating << endl;
    cout << "Course: " << teacher.course.courseName << endl;
}

/**
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the courses read
 **/

void loadCourses(string csvPath, HashTableCourses *hashTable)
{
    // Goal: Load Courses From the CSV - ABCU Advising Program Input
    string line;
    vector<string> courseTokens;

    // ifstream to open file
    ifstream file;

    file.open(csvPath);
    cout << "Opening File: " << csvPath << endl;

    // If File does not open throw error
    if (!file.is_open())
        throw runtime_error("Could not open file.");

    // while file is not at the end
    while (!file.eof())
    {
        Course course;

        getline(file, line);
        courseTokens = Split(line);

        // if course tokens size is less than 2 -> show error
        if (courseTokens.size() < 4)
        {
            cout << "Error occurred. Courses must have a course code and a course name." << endl;
            break;
        }
        else
        {
            // Else -> append the course tokens to respective spots in the course object
            course.courseCode = courseTokens.at(0);
            course.courseName = courseTokens.at(1);

            // Will Push Each PreReq to the list to put it inside the course strucutre
            for (unsigned int i = 4; i < courseTokens.size(); i++)
            {
                course.prereqList.push_back(courseTokens.at(i));
            }

            // insert course in the Hash Table
            hashTable->InsertCourse(course);
        }
    }
    cout << "All Courses Have Been Loaded" << endl;

    cout << "Closing file..." << endl;
    file.close();
}

/**
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the teachers read
 **/

void loadTeachers(string csvPath, HashTableTeachers *hashTable)
{
    // Goal: Load Teachers From the CSV - ABCU Advising Program Input
    string line;
    vector<string> teacherTokens;

    // ifstream to open file
    ifstream file;

    file.open(csvPath);
    cout << "Opening File: " << csvPath << endl;

    // If File does not open throw error
    if (!file.is_open())
        throw runtime_error("Could not open file.");

    // while file is not at the end
    while (!file.eof())
    {
        Teacher teacher;

        getline(file, line);
        teacherTokens = Split(line);

        // if teacher tokens size is less than 2 -> show error
        if (teacherTokens.size() < 2)
        {
            cout << "Error occurred. Teachers must have a teacher code, a teacher name, and teacher rating." << endl;
            break;
        }
        // Else -> append the teacher tokens to respective spots in the teacher object
        else
        {
            teacher.course.courseName = teacherTokens.at(1);
            teacher.teacherName = teacherTokens.at(2);
            teacher.teacherCode = teacherTokens.at(3);
            teacher.teacherRating = teacherTokens.at(4);

            // insert teacher in the Hash Table
            hashTable->InsertTeacher(teacher);
        }
    }

    cout << "All Teachers Have Been Loaded" << endl;
    cout << "Closing file..." << endl;
    file.close();
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * @param str String being converted to a double
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch)
{
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/** 
* Convert Case to Uppcase when it is the first letter
* @param strToConvert the string to convert 
**/
void convertCase(string &strToConvert)
{
    for (unsigned int i = 0; i < strToConvert.length(); i++)
    {
        if (isalpha(strToConvert.at(i)))
        {
            strToConvert.at(i) = toupper(strToConvert.at(i));
        }
    }
}

int main(int argc, char *argv[])
{

    // process command line arguments
    string csvPath, courseKey, teacherKey;

    switch (argc)
    {
    case 2:
        csvPath = argv[1];
        break;
    case 3:
        csvPath = argv[1];
        courseKey = argv[2];
        break;
    case 4:
        csvPath = argv[1];
        break;
    case 5:
        csvPath = argv[1];
        break;
    case 6:
        csvPath = argv[1];
        teacherKey = argv[2];
        break;
    default:
        csvPath = "CS 300 ABCU_Advising_Program_Input.csv";
    };

    // Define a hash table to hold all the courses
    HashTableCourses *courseTable;

    Course course;
    courseTable = new HashTableCourses();

    // Define a new hash table to hold all the teachers
    HashTableTeachers *teacherTable;

    Teacher teacher;
    teacherTable = new HashTableTeachers();

    // choice the user will make in the menu
    int choice = 0;

    // While Choice IS NOT 9, display the menu
    while (choice != 9)
    {
        cout << "Menu:" << endl;
        cout << "  1. Load Courses" << endl;
        cout << "  2. Display All Courses Alphabetically" << endl;
        cout << "  3. Find Course" << endl;
        cout << "  4. Load Teachers" << endl;
        cout << "  5. Display All Teachers Alphabetically" << endl;
        cout << "  6. Find Teacher" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        // Switch to each case based on the User's entered choice
        switch (choice)
        {

        case 1:
            // Complete the method call to load the courses
            loadCourses(csvPath, courseTable);
            cout << course.courseName << endl;
            break;

        case 2:
            courseTable->SortAllCourses();
            courseTable->PrintAllCourses();

            break;

        case 3:
            cout << "Please Enter a Course Code to Search For: " << endl;
            cin >> courseKey;

            convertCase(courseKey);

            course = courseTable->SearchCourse(courseKey);

            if (!course.courseCode.empty())
            {
                cout << "Course Information" << endl;
                displayCourse(course);
            }
            else
            {
                cout << "Course Code: " << courseKey << " not found." << endl;
            }

            break;

        case 4:
            loadTeachers(csvPath, teacherTable);
            break;

        case 5:
            teacherTable->SortAllTeachers();
            teacherTable->PrintAllTeachers();

            break;

        case 6:
            cout << "Please Enter a Teacher Code to Search For: " << endl;
            cin >> teacherKey;

            convertCase(teacherKey);

            teacher = teacherTable->SearchTeacher(teacherKey);

            if (!teacher.teacherCode.empty())
            {
                cout << "Teacher Information" << endl;
                displayTeacher(teacher);
            }
            else
            {
                cout << "Teacher Code: " << teacherKey << " not found." << endl;
            }

            break;
        }
        // if user enters a non-numerical Number, clear choice and restart menu
        if (cin.fail())
        {
            cout << "Invalid Input. Please Enter a Number From the Menu" << endl;

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            continue;
        }
    }
    cout << "Thanks for using the course planner, goodbye!" << endl;

    return 0;
}
