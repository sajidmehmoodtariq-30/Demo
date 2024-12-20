// Header Files
#include <iostream>  // For input output operations
#include <windows.h> // For Sleep function
#include <string>    // For string operations
#include <fstream>   // For file operations
#include <ctime>  // For time
#include <cstdlib> // For rand and srand

using namespace std;

const int totalCourses = 5;  // Total number of courses
const int totalSections = 3; // Total number of sections

// Enumerations

enum Courses // Enum for Courses
{
    MATH = 0,
    PHYSICS,
    CHEMISTRY,
    BIOLOGY,
    CS
};

string courseToString(Courses course) // Function to convert Courses to string bcz the enum was giving index while printing
{
    switch (course)
    {
    case MATH:
        return "Math";
    case PHYSICS:
        return "Physics";
    case CHEMISTRY:
        return "Chemistry";
    case BIOLOGY:
        return "Biology";
    case CS:
        return "Computer Science";
    default:
        return "Unknown";
    }
}

enum Sections // Enum for Sections
{
    SECTION_A = 0,
    SECTION_B,
    SECTION_C
};

string sectionsToString(Sections section) // Function to convert Courses to string bcz enum was giving index while printing
{
    switch (section)
    {
    case SECTION_A:
        return "A";
    case SECTION_B:
        return "B";
    case SECTION_C:
        return "C";
    default:
        return "Unknown";
    }
}

// Structures

struct User
{
    string username;
    string password;
    string role;
    string name;
};

struct Student
{
    string rollNo;
    string name;
    char section;
    int quizAttempted = 0;
    int *makrsPtr = new int[quizAttempted];
    char Grade;
    int *coursesPtr = new int[5];
};

struct Teacher
{
    string teacherId;
    string name;
    bool *coursesPtr = new bool[totalCourses];
    bool *sectionsPtr = new bool[totalSections];
};

struct Question
{
    string questionText; // Question text
    string options[4];   // Four options for the question
    int correctOption;   // Index of the correct option (1 to 4)
};

struct Quiz
{
    string quizName;     // Name of the quiz
    string courseName;   // Course for which the quiz is created
    bool isRandom;       // Whether the questions are random or not
    int totalQuestions;  // Number of questions in the quiz
    Question *questions; // Dynamically allocated array of questions
};

// Function Prototypes
string toLowerCase(string str);
bool login(string role, User &loggedInUser);
bool Validation(const string &username, const string &password, const string &role, User &loggedInUser);
User *getData(const string &location, int &size);
void adminMenu(const User &loggedInUser);
Student *loadAllStudents(const string &location, int &studentCount);
Teacher *loadAllTeachers(const string &location, int &teacherCount);
void addUser(const string &location);
void searchUser(const string &location, const string &username);
void listAllUsers(const string &location);
void listSameName(const string &location);
void listSection();
void listStudentsByCourse(const string &location, Courses course);
void listTeachersBySection();
void listUsers(const string &location);
void deleteStudent(const string &username);
void deleteTeacher(const string &username);
void deleteUser(const string &location);
void editTeacher(const string &username);
void editStudent(const string &username);
void editUser(const string &location);
void teacherMenu(const User &loggedInUser);
void studentMenu(const User &loggedInUser);

int main()
{
    bool flag = true;        // To store user choice
    char choice;             // To store user choice
    bool passwordValidation; // To store password validation
    User loggedInUser;       // To store the logged-in user's information

    while (flag)
    {
        system("cls"); // To clear the screen
        // Menu
        cout << "To Enter as an Admin press 1" << endl;
        cout << "To Enter as a Teacher Press 2" << endl;
        cout << "To Enter as a Student Press 3" << endl;
        cout << "To Exit Press 4" << endl;
        cout << "Enter Your Choice: ";
        // User Choice
        cin >> choice;
        // Calling appropriate functions depending upon the choice of user
        switch (choice)
        {
        case '1':          // Admin
            system("cls"); // For clearing previous output
            cout << "Please Enter your Credentials or ";
            if (login("admin", loggedInUser))
            {
                cout << "Login Successful" << endl;
                Sleep(2000); // To pause the screen for 2 seconds
                adminMenu(loggedInUser);
            }
            break;
        case '2': // Teacher
            system("cls");
            cout << "Welcome to Teacher Panel Please Enter your Credentials or ";
            if (login("teacher", loggedInUser))
            {
                cout << "Login Successful" << endl;
                Sleep(2000); // To pause the screen for 2 seconds
                teacherMenu(loggedInUser);
            }
            break;
        case '3': // Student
            system("cls");
            cout << "Welcome to Student Panel Please Enter your Credentials or ";
            if (login("student", loggedInUser))
            {
                cout << "Login Successful" << endl;
                Sleep(2000); // To pause the screen for 2 seconds
                studentMenu(loggedInUser);
            }
            break;
        case '4': // Exit
            flag = false;
            break;
        default: // Invalid Case
            cout << "Invalid Choice" << endl;
            break;
        }
    }
    return 0;
}

string toLowerCase(string str)
{
    for (int i = 0; i < str.length(); i++)
        str[i] = tolower(str[i]);
    return str;
}

bool login(string role, User &loggedInUser)
{
    string username, password;
    bool flag = true;
    while (flag) // A while loop to get the username and password untill user enters correct credentials or exits by pressing 1
    {
        cout << "Press 1 to exit" << endl;
        cout << "Enter Username: ";
        cin >> username;
        cout << "Enter Password: ";
        cin >> password;
        username = toLowerCase(username); // Converting the username to lower case in order to avoid case sensitivity
        password = toLowerCase(password); // Converting the username to lower case in order to avoid case sensitivity
        if (username == "1" || password == "1")
            flag = false;
        else if (Validation(username, password, role, loggedInUser))
            return true;
        else
            cout << "Wrong Credentials" << endl;
        Sleep(2000); // To pause the screen for 2 seconds
    }

    return false;
}

// Function to Validate the entered credentials
bool Validation(const string &username, const string &password, const string &role, User &loggedInUser)
{
    bool isValid = false;
    int size = 0;
    // Getting data from file and storing it in a User pointer
    User *data = getData("Data/users.txt", size);

    // Checking if the entered credentials are correct
    for (int i = 0; i < size; i++)
    {
        if (data[i].username == username && data[i].password == password && data[i].role == role)
        {
            isValid = true;
            loggedInUser = data[i]; // Store the logged-in user's information
            break;
        }
    }
    // Deleting the dynamic data pointer
    delete[] data;
    return isValid;
}

// Getting User data from file
User *getData(const string &location, int &size)
{
    // Loading the File
    ifstream file(location);

    // Count the number of lines
    size = 0;
    // A temporary string created to calculate number of lines
    string temp;
    while (getline(file, temp))
        size++;

    // Reset the file stream to the beginning
    file.clear();
    file.seekg(0, ios::beg);

    // Allocate memory for User array
    User *data = new User[size];

    // Read the data from the file
    for (int i = 0; i < size; i++)
    {
        file >> data[i].username >> data[i].password >> data[i].role >> data[i].name; // Similar to using single cin ad getting multiple inputs in multiple variables
        data[i].username = toLowerCase(data[i].username);
        data[i].password = toLowerCase(data[i].password);
    }

    return data;
}

// admin menu
void adminMenu(const User &loggedInUser)
{
    bool flag = true;
    char choice;
    while (flag)
    {
        system("cls");
        // Admin Menu
        cout << "Hello " << loggedInUser.name << " Welcome to Admin Panel" << endl;
        cout << "Admin Menu:" << endl;
        cout << "1. Add User" << endl;
        cout << "2. Search User" << endl;
        cout << "3. List Users" << endl;
        cout << "4. Delete User" << endl;
        cout << "5. Update User" << endl;
        cout << "6. Logout" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case '1': // Add User
            addUser("Data/users.txt");
            break;
        case '2': // Search User
        {
            string username;
            cout << "Enter username to search: ";
            cin >> username;
            searchUser("Data/users.txt", username);
            system("pause");
            break;
        }
        case '3': // List Users
            listUsers("Data/users.txt");
            system("pause");
            break;
        case '4': // Delete User
        {
            deleteUser("Data/users.txt");
            system("pause");
            break;
        }
        case '5': // Update User
        {
            editUser("Data/users.txt");
            system("pause");
            break;
        }
        case '6':
            flag = false;
            break;
        default:
            cout << "Invalid choice!" << endl;
            break;
        }
    }
}

// Load all students from file
Student *loadAllStudents(const string &location, int &studentCount)
{
    ifstream file(location);

    if (!file.is_open())
    {
        cout << "Error: Could not open student file!" << endl;
        Sleep(2000);
        studentCount = 0;
        return nullptr;
    }

    // Count the number of students
    studentCount = 0;
    string temp;
    while (getline(file, temp))
        studentCount++;

    // Reset the file stream to read data
    file.clear();
    file.seekg(0, ios::beg);

    // Allocate memory for Student array
    Student *students = new Student[studentCount];

    // Read each student's data from the file
    for (int i = 0; i < studentCount; i++)
    {
        string username;
        char section;
        file >> username >> section;

        // Assign student data
        students[i].rollNo = username; // Assuming rollNo = username
        students[i].section = section;

        // Read course enrollment
        for (int j = 0; j < totalCourses; j++)
        {
            bool enrolled;
            file >> enrolled;
            students[i].coursesPtr[j] = enrolled;
        }
    }

    file.close();
    return students;
}

// Load all teachers from file
Teacher *loadAllTeachers(const string &location, int &teacherCount)
{
    ifstream file(location);

    if (!file.is_open())
    {
        cout << "Error: Could not open teacher file!" << endl;
        Sleep(2000);
        teacherCount = 0;
        return nullptr;
    }

    // Count the number of teachers
    teacherCount = 0;
    string temp;
    while (getline(file, temp))
        teacherCount++;

    // Reset the file stream to read data
    file.clear();
    file.seekg(0, ios::beg);

    // Allocate memory for Teacher array
    Teacher *teachers = new Teacher[teacherCount];

    // Read each teacher's data from the file
    for (int i = 0; i < teacherCount; i++)
    {
        // getting username from file
        string username;
        file >> username;

        // Assign teacher data
        teachers[i].teacherId = username;

        // Allocate memory for courses and sections
        teachers[i].coursesPtr = new bool[totalCourses];
        teachers[i].sectionsPtr = new bool[totalSections];

        // Read courses the teacher teaches
        for (int j = 0; j < totalCourses; j++)
            file >> teachers[i].coursesPtr[j];

        // Read sections the teacher is assigned to
        for (int j = 0; j < totalSections; j++)
            file >> teachers[i].sectionsPtr[j];
    }

    file.close();
    return teachers;
}

// Add User
void addUser(const string &location)
{
    int size = 0;
    User newUser;
    bool check = false;

    ofstream file(location, ios::app);
    system("cls");

    cout << "Enter User Details:" << endl;

    do
    {
        cout << "Enter Username: ";
        cin >> newUser.username;
        newUser.username = toLowerCase(newUser.username);

        // Check if username already exists in users
        User *data = getData("Data/users.txt", size);
        for (int i = 0; i < size; i++)
        {
            if (data[i].username == newUser.username)
            {
                cout << "Username already exists!" << endl;
                check = true;
                break;
            }
            else
                check = false;
        }
        delete[] data;
    } while (check);
    cout << "Enter Password: ";
    cin >> newUser.password;

    do
    {
        cout << "Enter Role (Admin/Teacher/Student): ";
        cin >> newUser.role;
        newUser.role = toLowerCase(newUser.role);
    } while (newUser.role != "admin" && newUser.role != "teacher" && newUser.role != "student");

    cout << "Enter Name: ";
    cin >> newUser.name;

    file << endl
         << newUser.username << " " << newUser.password << " " << newUser.role << " " << newUser.name;
    file.close();
    if (newUser.role == "teacher")
    {
        // Teacher-specific logic
        ofstream teacherFile("Data/teachers.txt", ios::app);
        teacherFile << endl
                    << newUser.username << " ";
        bool courses[totalCourses], sections[totalSections];
        cout << "Assign Courses to Teacher (1 for Yes, 0 for No):\n";
        for (int i = 0; i < totalCourses; i++)
        {
            do
            {
                cout << "Teach " << courseToString((Courses)i) << "? ";
                cin >> courses[i];
            } while (courses[i] != 0 && courses[i] != 1);
            teacherFile << courses[i] << " ";
        }
        cout << "Assign Sections to Teacher (1 for Yes, 0 for No):\n";
        for (int i = 0; i < totalSections; i++)
        {
            do
            {
                cout << "Teach " << sectionsToString((Sections)i) << "? ";
                cin >> sections[i];
            } while (sections[i] != 0 && sections[i] != 1);
            teacherFile << sections[i] << " ";
        }
        teacherFile << endl;
        teacherFile.close();
    }
    else if (newUser.role == "student")
    {
        // Student-specific logic
        ofstream studentFile("Data/students.txt", ios::app);
        char section;
        bool courses[totalCourses];
        cout << "Assign Section to Student (A/B/C): ";
        cin >> section;
        studentFile << endl
                    << newUser.username << " " << section << " ";
        cout << "Enroll Student in Courses (1 for Yes, 0 for No):\n";
        for (int i = 0; i < totalCourses; i++)
        {
            cout << "Enroll in " << courseToString((Courses)i) << "? ";
            cin >> courses[i];
            studentFile << courses[i] << " ";
        }
        studentFile << endl;
        studentFile.close();
    }

    cout << "User added successfully!" << endl;

    Sleep(1000);
}

// Search user
void searchUser(const string &location, const string &username)
{
    int size = 0;
    string role;
    User *data = getData(location, size);

    for (int i = 0; i < size; i++)
    {
        if (data[i].username == username)
        {
            role = data[i].role;
            cout << "User found!" << endl;
            cout << "Username: " << data[i].username << endl;
            cout << "Role: " << data[i].role << endl;
            cout << "Name: " << data[i].name << endl;
            break;
        }
    }
    if (role == "teacher")
    {
        int teacherCount = 0;
        Teacher *teachers = loadAllTeachers("Data/teachers.txt", teacherCount);
        for (int i = 0; i < teacherCount; i++)
        {
            if (teachers[i].teacherId == username)
            {
                cout << "Courses: ";
                for (int j = 0; j < totalCourses; j++)
                    if (teachers[i].coursesPtr[j])
                        cout << courseToString((Courses)j) << " ";

                cout << endl;
                cout << "Sections: ";
                for (int j = 0; j < totalSections; j++)
                    if (teachers[i].sectionsPtr[j])
                        cout << sectionsToString((Sections)j) << " ";

                cout << endl;
                break;
            }
        }
    }
    else if (role == "student")
    {
        int studentCount = 0;
        Student *students = loadAllStudents("Data/students.txt", studentCount);
        for (int i = 0; i < studentCount; i++)
        {
            if (students[i].rollNo == username)
            {
                cout << "Section: " << students[i].section << endl;
                cout << "Courses: ";
                for (int j = 0; j < totalCourses; j++)
                {
                    if (students[i].coursesPtr[j])
                    {
                        cout << courseToString((Courses)j) << " ";
                    }
                }
                cout << endl;
                break;
            }
        }
    }
}

// List all users
void listAllUsers(const string &location)
{
    int size = 0;
    User *data = getData(location, size);

    // Load all teachers and students once
    int teacherCount = 0, studentCount = 0;
    Teacher *teachers = loadAllTeachers("Data/teachers.txt", teacherCount);
    Student *students = loadAllStudents("Data/students.txt", studentCount);

    for (int i = 0; i < size; i++)
    {
        cout << "Username: " << data[i].username << endl;
        cout << "Role: " << data[i].role << endl;
        cout << "Name: " << data[i].name << endl;

        if (data[i].role == "teacher")
        {
            for (int t = 0; t < teacherCount; t++)
            {
                if (teachers[t].teacherId == data[i].username)
                {
                    cout << "Courses: ";
                    for (int j = 0; j < totalCourses; j++)
                        if (teachers[t].coursesPtr[j])
                            cout << courseToString((Courses)j) << " ";

                    cout << endl;

                    cout << "Sections: ";
                    for (int j = 0; j < totalSections; j++)
                        if (teachers[t].sectionsPtr[j])
                            cout << sectionsToString((Sections)j) << " ";

                    cout << endl;
                }
            }
        }
        else if (data[i].role == "student")
        {
            for (int s = 0; s < studentCount; s++)
            {
                if (students[s].rollNo == data[i].username)
                {
                    cout << "Section: " << students[s].section << endl;
                    cout << "Courses: ";
                    for (int j = 0; j < totalCourses; j++)
                        if (students[s].coursesPtr[j])
                            cout << courseToString((Courses)j) << " ";

                    cout << endl;
                }
            }
        }
        else if (data[i].role == "admin")
        {
            cout << "No additional data for admin role." << endl;
        }

        cout << endl;
    }

    // Clean up dynamic memory
    delete[] data;
    delete[] teachers;
    delete[] students;
}

// List Users with Same name
void listSameName(const string &location)
{
    int studentCount = 0;
    Student *students = loadAllStudents("Data/students.txt", studentCount);
    int teacherCount = 0;
    Teacher *teachers = loadAllTeachers("Data/teachers.txt", teacherCount);
    int size = 0;
    string role;
    User *data = getData(location, size);

    string name;
    cout << "Enter name to search: ";
    cin >> name;
    string username;

    system("cls");

    for (int i = 0; i < size; i++)
    {
        if (data[i].name == name)
        {
            role = data[i].role;
            cout << "User found!" << endl;
            cout << "Username: " << data[i].username << endl;
            username = data[i].username;
            cout << "Role: " << data[i].role << endl;
            cout << "Name: " << data[i].name << endl;
        }
        else
            username = " ";
        if (role == "teacher")
        {
            for (int i = 0; i < teacherCount; i++)
            {
                if (teachers[i].teacherId == username)
                {
                    cout << "Courses: ";
                    for (int j = 0; j < totalCourses; j++)
                        if (teachers[i].coursesPtr[j])
                            cout << courseToString((Courses)j) << " ";

                    cout << endl;
                    cout << "Sections: ";
                    for (int j = 0; j < totalSections; j++)
                        if (teachers[i].sectionsPtr[j])
                            cout << sectionsToString((Sections)j) << " ";

                    cout << endl;
                    break;
                }
            }
        }
        else if (role == "student")
        {
            for (int i = 0; i < studentCount; i++)
            {
                if (students[i].rollNo == username)
                {
                    cout << "Section: " << students[i].section << endl;
                    cout << "Courses: ";
                    for (int j = 0; j < totalCourses; j++)
                    {
                        if (students[i].coursesPtr[j])
                        {
                            cout << courseToString((Courses)j) << " ";
                        }
                    }
                    cout << endl;
                    break;
                }
            }
        }
    }

    delete[] data;
}

// List Users enrolled in a Specific Section
void listSection()
{
    int size = 0, studentCount = 0;
    User *data = getData("Data/users.txt", size);
    Student *students = loadAllStudents("Data/students.txt", studentCount);
    char section;
    string rollNo;
    cin >> section;
    for (int j = 0; j < studentCount; j++)
    {
        if (students[j].section == section)
        {
            rollNo = students[j].rollNo;
            cout << "Section: " << students[j].section << endl;
            cout << "Courses: ";
            for (int k = 0; k < totalCourses; k++)
                if (students[j].coursesPtr[k])
                    cout << courseToString((Courses)k) << " ";
            cout << endl;
            for (int i = 0; i < size; i++)
            {
                if (data[i].username == rollNo)
                {
                    cout << "Username: " << data[i].username << endl;
                    cout << "Role: " << data[i].role << endl;
                    cout << "Name: " << data[i].name << endl;
                }
            }
        }
    }
}

// List all students enrolled in a specific Course
void listStudentsByCourse(const string &location, Courses course)
{
    int studentCount = 0, size = 0;                              // Variable to hold the count of students
    Student *students = loadAllStudents(location, studentCount); // Load all student data
    User *data = getData("Data/users.txt", size);
    bool found = false;

    for (int i = 0; i < studentCount; i++)
    {
        if (students[i].coursesPtr[course]) // Check if the student is enrolled in the specified course
        {
            found = true;
            cout << "Roll No: " << students[i].rollNo << endl;
            cout << "Section: " << sectionsToString((Sections)(students[i].section - 'A')) << endl;

            // Search for the student's name in user data
            for (int j = 0; j < size; j++)
            {
                if (data[j].username == students[i].rollNo)
                {
                    cout << "Name: " << data[j].name << endl;
                    break;
                }
            }
            cout << endl; // Print a blank line between students
        }
    }

    // If no students were found in the course
    if (!found)
    {
        cout << "No students are enrolled in " << courseToString(course) << "." << endl;
    }

    // Clean up dynamically allocated memory
    delete[] students;
    delete[] data;
}

// List teacher teaching a section
void listTeachersBySection()
{
    int size = 0, teacherCount = 0;
    User *data = getData("Data/users.txt", size);                           // Load all users
    Teacher *teachers = loadAllTeachers("Data/teachers.txt", teacherCount); // Load all teachers
    char section;

    cout << "Enter the section (A, B, C): ";
    cin >> section;

    bool found = false;

    for (int i = 0; i < teacherCount; i++)
    {
        if (teachers[i].sectionsPtr[section - 'A']) // Check if the teacher teaches the specified section
        {
            found = true;

            // Print teacher-specific data
            cout << "Section: " << sectionsToString((Sections)(section - 'A')) << endl;
            cout << "Courses: ";
            for (int j = 0; j < totalCourses; j++)
            {
                if (teachers[i].coursesPtr[j])
                    cout << courseToString((Courses)j) << " ";
            }
            cout << endl;

            // Find and display the teacher's user information
            for (int k = 0; k < size; k++)
            {
                if (data[k].username == teachers[i].teacherId)
                {
                    cout << "Username: " << data[k].username << endl;
                    cout << "Role: " << data[k].role << endl;
                    cout << "Name: " << data[k].name << endl;
                }
            }

            cout << "-----------------" << endl;
        }
    }

    if (!found)
        cout << "No teachers are teaching section " << section << "." << endl;

    // Clean up dynamically allocated memory
    delete[] data;
    delete[] teachers;
}

// List Users
void listUsers(const string &location)
{
    char choice;
    bool flag = true;

    while (flag)
    {
        system("cls");
        cout << "Choose an option:\n";
        cout << "1. List All Users\n";
        cout << "2. List Users with the Same Name\n";
        cout << "3. List Students Enrolled in a Specific Course\n";
        cout << "4. List Students in a Specific Section\n";
        cout << "5. List Teachers in a Specific Section\n";
        cout << "6. Exit \n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice)
        {
        case '1':
            listAllUsers(location);
            // system("pause");
            break;
        case '2':
            listSameName(location);
            system("pause");
            break;
        case '3':
            int courseIndex;
            do
            {
                cout << "Enter the course index \n0 for Math \n1 for Physics \n2. for Chemistry \n3. for Biology \n4.for CS: ";
                cout << "\nEnter your choice:";
                cin >> courseIndex;
            } while (courseIndex < 0 || courseIndex > 4);
            listStudentsByCourse("Data/students.txt", (Courses)courseIndex);
            system("pause");
            break;
        case '4':
            listSection();
            system("pause");
            break;
        case '5':
            listTeachersBySection();
            system("pause");
            break;
        case '6':
            flag = false;
            break;
        default:
            break;
        }
    }
}

// Delete Student
void deleteStudent(const string &username)
{
    int studentCount = 0;
    Student *students = loadAllStudents("Data/students.txt", studentCount);

    ofstream tempFile("Data/temp_students.txt");
    if (!tempFile.is_open())
    {
        cout << "Error: Unable to open temporary file for writing!" << endl;
        delete[] students;
        return;
    }

    for (int i = 0; i < studentCount; i++)
    {
        if (students[i].rollNo != username)
        {
            tempFile << students[i].rollNo << " " << students[i].section << " ";
            for (int j = 0; j < totalCourses; j++)
                tempFile << students[i].coursesPtr[j] << " ";
            tempFile << endl;
        }
    }
    tempFile.close();

    remove("Data/students.txt");
    rename("Data/temp_students.txt", "Data/students.txt");

    delete[] students;
    cout << "Student-specific data deleted successfully!" << endl;
}

// To delete teacher
void deleteTeacher(const string &username)
{
    int teacherCount = 0;
    Teacher *teachers = loadAllTeachers("Data/teachers.txt", teacherCount);

    ofstream tempFile("Data/temp_teachers.txt");
    if (!tempFile.is_open())
    {
        cout << "Error: Unable to open temporary file for writing!" << endl;
        delete[] teachers;
        return;
    }

    for (int i = 0; i < teacherCount; i++)
    {
        if (teachers[i].teacherId != username)
        {
            tempFile << teachers[i].teacherId << " ";
            for (int j = 0; j < totalCourses; j++)
                tempFile << teachers[i].coursesPtr[j] << " ";
            for (int j = 0; j < totalSections; j++)
                tempFile << teachers[i].sectionsPtr[j] << " ";
            tempFile << endl;
        }
    }
    tempFile.close();

    remove("Data/teachers.txt");
    rename("Data/temp_teachers.txt", "Data/teachers.txt");

    delete[] teachers;
    cout << "Teacher-specific data deleted successfully!" << endl;
}

// Delete User
void deleteUser(const string &location)
{
    int size = 0;
    User *data = getData(location, size); // Load all users

    string username;
    cout << "Enter the username to delete: ";
    cin >> username;

    bool found = false;
    string role;

    // Open a temporary file to write updated data
    ofstream tempFile("Data/temp_users.txt");
    if (!tempFile.is_open())
    {
        cout << "Error: Unable to open temporary file for writing!" << endl;
        delete[] data;
        return;
    }

    for (int i = 0; i < size; i++)
    {
        if (data[i].username == username)
        {
            found = true;
            role = data[i].role;
            cout << "Deleting user: " << data[i].username << endl;
        }
        else
        {
            // Write all other users to the temporary file
            tempFile << data[i].username << " " << data[i].password << " " << data[i].role << " " << data[i].name << endl;
        }
    }
    tempFile.close();

    if (found)
    {
        // Replace the original users.txt with the updated temp file
        remove(location.c_str());
        rename("Data/temp_users.txt", location.c_str());

        if (role == "teacher")
        {
            deleteTeacher(username);
        }
        else if (role == "student")
        {
            deleteStudent(username);
        }

        cout << "User deleted successfully!" << endl;
    }
    else
    {
        cout << "Error: User not found!" << endl;
        remove("Data/temp_users.txt"); // Clean up temp file if not used
    }

    delete[] data;
}

// Edit Teacher
void editTeacher(const string &username)
{
    int teacherCount = 0;
    Teacher *teachers = loadAllTeachers("Data/teachers.txt", teacherCount);

    ofstream tempFile("Data/temp_teachers.txt");
    if (!tempFile.is_open())
    {
        cout << "Error: Unable to open temporary file for writing!" << endl;
        delete[] teachers;
        return;
    }

    for (int i = 0; i < teacherCount; i++)
    {
        if (teachers[i].teacherId == username)
        {
            cout << "Editing teacher-specific data for: " << username << endl;

            // Update courses
            cout << "Update courses taught (1 for Yes, 0 for No):" << endl;
            for (int j = 0; j < totalCourses; j++)
            {
                cout << "Teaches " << courseToString((Courses)j) << "? ";
                cin >> teachers[i].coursesPtr[j];
            }

            // Update sections
            cout << "Update sections taught (1 for Yes, 0 for No):" << endl;
            for (int j = 0; j < totalSections; j++)
            {
                cout << "Teaches Section " << sectionsToString((Sections)j) << "? ";
                cin >> teachers[i].sectionsPtr[j];
            }
        }

        // Write updated or unmodified teacher data to temp file
        tempFile << teachers[i].teacherId << " ";
        for (int j = 0; j < totalCourses; j++)
            tempFile << teachers[i].coursesPtr[j] << " ";
        for (int j = 0; j < totalSections; j++)
            tempFile << teachers[i].sectionsPtr[j] << " ";
        tempFile << endl;
    }
    tempFile.close();

    remove("Data/teachers.txt");
    rename("Data/temp_teachers.txt", "Data/teachers.txt");

    delete[] teachers;
    cout << "Teacher-specific data updated successfully!" << endl;
}

// Edit Student
void editStudent(const string &username)
{
    int studentCount = 0;
    Student *students = loadAllStudents("Data/students.txt", studentCount);

    ofstream tempFile("Data/temp_students.txt");
    if (!tempFile.is_open())
    {
        cout << "Error: Unable to open temporary file for writing!" << endl;
        delete[] students;
        return;
    }

    for (int i = 0; i < studentCount; i++)
    {
        if (students[i].rollNo == username)
        {
            cout << "Editing student-specific data for: " << username << endl;

            // Update section
            cout << "Update section (current: " << students[i].section << "): ";
            cin >> students[i].section;

            // Update courses
            cout << "Update courses enrolled (1 for Yes, 0 for No):" << endl;
            for (int j = 0; j < totalCourses; j++)
            {
                cout << "Enrolled in " << courseToString((Courses)j) << "? ";
                cin >> students[i].coursesPtr[j];
            }
        }

        // Write updated or unmodified student data to temp file
        tempFile << students[i].rollNo << " " << students[i].section << " ";
        for (int j = 0; j < totalCourses; j++)
            tempFile << students[i].coursesPtr[j] << " ";
        tempFile << endl;
    }
    tempFile.close();

    remove("Data/students.txt");
    rename("Data/temp_students.txt", "Data/students.txt");

    delete[] students;
    cout << "Student-specific data updated successfully!" << endl;
}

// Edit User
void editUser(const string &location)
{
    int size = 0;
    User *data = getData(location, size); // Load all users

    string username;
    cout << "Enter the username to edit: ";
    cin >> username;

    bool found = false;
    string role;

    // Open a temporary file to write updated data
    ofstream tempFile("Data/temp_users.txt");
    if (!tempFile.is_open())
    {
        cout << "Error: Unable to open temporary file for writing!" << endl;
        delete[] data;
        return;
    }

    for (int i = 0; i < size; i++)
    {
        if (data[i].username == username)
        {
            found = true;
            role = data[i].role;

            cout << "Editing user: " << data[i].username << endl;

            // Update common user fields
            cout << "Enter new name (current: " << data[i].name << "): ";
            cin >> data[i].name;
            cout << "Enter new password (current: " << data[i].password << "): ";
            cin >> data[i].password;

            // Save updated user details to temp file
            tempFile << data[i].username << " " << data[i].password << " " << data[i].role << " " << data[i].name << endl;
        }
        else
        {
            // Write unmodified user data to the temp file
            tempFile << data[i].username << " " << data[i].password << " " << data[i].role << " " << data[i].name << endl;
        }
    }
    tempFile.close();

    if (found)
    {
        // Replace the original users file with the updated temp file
        remove(location.c_str());
        rename("Data/temp_users.txt", location.c_str());

        // Update role-specific data
        if (role == "teacher")
        {
            editTeacher(username);
        }
        else if (role == "student")
        {
            editStudent(username);
        }

        cout << "User updated successfully!" << endl;
    }
    else
    {
        cout << "Error: User not found!" << endl;
        remove("Data/temp_users.txt"); // Clean up temp file if not used
    }

    delete[] data;
}

// Writing quiz
void createQuiz(const string &location)
{
    Quiz quiz;

    // Collecting quiz details
    cout << "Enter Quiz Name: ";
    cin >> quiz.quizName;
    cout << "Enter Course Name: ";
    cin >> quiz.courseName;
    cout << "Enter Total Number of Questions: ";
    cin >> quiz.totalQuestions;

    quiz.questions = new Question[quiz.totalQuestions];

    // Collecting questions for the quiz
    for (int i = 0; i < quiz.totalQuestions; i++)
    {
        cout << "Enter Question " << i + 1 << ": ";
        cin.ignore(); // To clear the input buffer
        getline(cin, quiz.questions[i].questionText);

        for (int j = 0; j < 4; j++)
        {
            cout << "Enter Option " << j + 1 << ": ";
            getline(cin, quiz.questions[i].options[j]);
        }

        do
        {
            cout << "Enter the Correct Option (1-4): ";
            cin >> quiz.questions[i].correctOption;
            if (quiz.questions[i].correctOption < 1 || quiz.questions[i].correctOption > 4)
                cout << "Invalid input! Please enter a number between 1 and 4." << endl;
        } while (quiz.questions[i].correctOption < 1 || quiz.questions[i].correctOption > 4);
    }

    // Asking if the questions should be randomized
    char randomizeChoice;
    cout << "Do you want to randomize the questions? (y/n): ";
    cin >> randomizeChoice;
    quiz.isRandom = (randomizeChoice == 'y' || randomizeChoice == 'Y');

    // Asking for the time limit
    int timeLimit;
    cout << "Enter the time limit for the quiz in minutes: ";
    cin >> timeLimit;

    // Writing quiz data to file
    ofstream file(location, ios::app);
    if (!file.is_open())
    {
        cout << "Error: Unable to save the quiz to file!" << endl;
        delete[] quiz.questions;
        return;
    }

    file << quiz.quizName << " " << quiz.courseName << " " << quiz.totalQuestions << " " << quiz.isRandom << " " << timeLimit << endl;
    for (int i = 0; i < quiz.totalQuestions; i++)
    {
        file << quiz.questions[i].questionText << endl;
        for (int j = 0; j < 4; j++)
            file << quiz.questions[i].options[j] << endl;
        file << quiz.questions[i].correctOption << endl;
    }
    file.close();

    cout << "Quiz created and saved successfully!" << endl;

    // Free allocated memory
    delete[] quiz.questions;
}

// Editing quiz
void editQuiz(const string &location)
{
    string quizName, newQuizName, newCourseName;
    bool quizFound = false;

    cout << "Enter the name of the quiz you want to edit: ";
    cin >> quizName;

    ifstream inFile(location);
    if (!inFile.is_open())
    {
        cout << "Error: Unable to open the file for reading!" << endl;
        return;
    }

    ofstream tempFile("Data/temp_quizzes.txt");
    if (!tempFile.is_open())
    {
        cout << "Error: Unable to open the temporary file for writing!" << endl;
        inFile.close();
        return;
    }

    string fileQuizName, fileCourseName, questionText, options[4];
    int totalQuestions, correctOption, timeLimit;
    bool isRandom;

    while (inFile >> fileQuizName >> fileCourseName >> totalQuestions >> isRandom >> timeLimit)
    {
        inFile.ignore(); // Ignore the newline after timeLimit

        if (fileQuizName == quizName)
        {
            quizFound = true;

            cout << "Editing Quiz: " << fileQuizName << endl;
            cout << "Enter new quiz name (or press Enter to keep it the same): ";
            cin.ignore(); // To clear the input buffer
            getline(cin, newQuizName);
            if (newQuizName.empty())
                newQuizName = fileQuizName;

            cout << "Enter new course name (or press Enter to keep it the same): ";
            getline(cin, newCourseName);
            if (newCourseName.empty())
                newCourseName = fileCourseName;

            char randomizeChoice;
            cout << "Current randomization: " << (isRandom ? "Yes" : "No") << endl;
            cout << "Do you want to randomize the questions? (y/n, or press Enter to keep it the same): ";
            cin.get(randomizeChoice);
            cin.ignore(); // Ignore the newline after input
            if (randomizeChoice == 'y' || randomizeChoice == 'Y')
                isRandom = true;
            else if (randomizeChoice == 'n' || randomizeChoice == 'N')
                isRandom = false;

            cout << "Current time limit: " << timeLimit << " minutes" << endl;
            cout << "Enter new time limit (in minutes, or press 0 to keep it the same): ";
            int newTimeLimit;
            cin >> newTimeLimit;
            cin.ignore(); // Ignore the newline after input
            if (newTimeLimit == 0)
                newTimeLimit = timeLimit;

            tempFile << newQuizName << " " << newCourseName << " " << totalQuestions << " " << isRandom << " " << newTimeLimit << endl;

            for (int i = 0; i < totalQuestions; i++)
            {
                cout << "Question " << i + 1 << ":" << endl;

                getline(inFile, questionText);
                cout << "Current Question: " << questionText << endl;
                cout << "Enter new question text (or press Enter to keep it the same): ";
                string newQuestionText;
                getline(cin, newQuestionText);
                if (newQuestionText.empty())
                    newQuestionText = questionText;

                for (int j = 0; j < 4; j++)
                {
                    getline(inFile, options[j]);
                    cout << "Current Option " << j + 1 << ": " << options[j] << endl;
                    cout << "Enter new option text (or press Enter to keep it the same): ";
                    string newOption;
                    getline(cin, newOption);
                    if (!newOption.empty())
                        options[j] = newOption;
                }

                inFile >> correctOption;
                inFile.ignore(); // Ignore the newline after correctOption
                cout << "Current Correct Option: " << correctOption << endl;
                cout << "Enter new correct option (1-4, or press 0 to keep it the same): ";
                int newCorrectOption;
                cin >> newCorrectOption;
                cin.ignore(); // Ignore the newline after input
                if (newCorrectOption == 0)
                    newCorrectOption = correctOption;

                // Save updated question to temporary file
                tempFile << newQuestionText << endl;
                for (int j = 0; j < 4; j++)
                    tempFile << options[j] << endl;
                tempFile << newCorrectOption << endl;
            }
        }
        else
        {
            // Copy quiz data as-is for quizzes not being edited
            tempFile << fileQuizName << " " << fileCourseName << " " << totalQuestions << " " << isRandom << " " << timeLimit << endl;
            for (int i = 0; i < totalQuestions; i++)
            {
                getline(inFile, questionText);
                tempFile << questionText << endl;
                for (int j = 0; j < 4; j++)
                {
                    getline(inFile, options[j]);
                    tempFile << options[j] << endl;
                }
                inFile >> correctOption;
                inFile.ignore(); // Ignore the newline
                tempFile << correctOption << endl;
            }
        }
    }

    inFile.close();
    tempFile.close();

    if (quizFound)
    {
        remove(location.c_str());
        rename("Data/temp_quizzes.txt", location.c_str());
        cout << "Quiz edited successfully!" << endl;
    }
    else
    {
        remove("Data/temp_quizzes.txt");
        cout << "Error: Quiz not found!" << endl;
    }
}

// Preview quiz
void previewQuiz(const string &location)
{
    string quizName;
    cout << "Enter the name of the quiz to preview: ";
    cin >> quizName;

    ifstream inFile(location);
    if (!inFile.is_open())
    {
        cout << "Error: Unable to open the quiz file!" << endl;
        return;
    }

    string fileQuizName, fileCourseName, questionText, options[4];
    int totalQuestions, correctOption, timeLimit;
    bool quizFound = false, isRandom;

    while (inFile >> fileQuizName >> fileCourseName >> totalQuestions >> isRandom >> timeLimit)
    {
        inFile.ignore(); // Ignore newline after timeLimit

        if (fileQuizName == quizName)
        {
            quizFound = true;

            cout << "\n--- Quiz Preview ---" << endl;
            cout << "Quiz Name: " << fileQuizName << endl;
            cout << "Course: " << fileCourseName << endl;
            cout << "Total Questions: " << totalQuestions << endl;
            cout << "Randomized: " << (isRandom ? "Yes" : "No") << endl;
            cout << "Time Limit: " << timeLimit << " minutes" << endl;

            for (int i = 0; i < totalQuestions; i++)
            {
                cout << "\nQuestion " << i + 1 << ": ";
                getline(inFile, questionText); // Read question text
                cout << questionText << endl;

                for (int j = 0; j < 4; j++)
                {
                    getline(inFile, options[j]); // Read each option
                    cout << "Option " << j + 1 << ": " << options[j] << endl;
                }

                inFile >> correctOption; // Read correct option
                inFile.ignore();         // Ignore newline after correctOption
                cout << "Correct Option: " << correctOption << endl;
            }
            cout << "\n--- End of Quiz Preview ---\n"
                 << endl;
            break;
        }
        else
        {
            // Skip over quiz questions and answers for other quizzes
            for (int i = 0; i < totalQuestions; i++)
            {
                getline(inFile, questionText); // Skip question text
                for (int j = 0; j < 4; j++)
                    getline(inFile, options[j]); // Skip options
                inFile >> correctOption;         // Skip correct option
                inFile.ignore();                 // Ignore newline after correctOption
            }
        }
    }

    if (!quizFound)
    {
        cout << "Error: Quiz not found!" << endl;
    }

    inFile.close();
}

// Helper function to convert course name to index
int courseToIndex(const string &courseName)
{
    for (int i = 0; i < totalCourses; i++)
    {
        if (courseToString((Courses)i) == courseName)
            return i;
    }
    return -1; // Return -1 if the course is not found
}

// Assign quiz
void assignQuiz(const string &quizLocation, const string &studentsLocation, const string &assignedLocation)
{
    string quizName, courseName;
    char section;
    bool quizFound = false;

    // Input quiz name and section
    cout << "Enter the name of the quiz to assign: ";
    cin >> quizName;
    cout << "Enter the section to assign the quiz to (A, B, C): ";
    cin >> section;

    // Open quiz file to check for the quiz and retrieve course name
    ifstream quizFile(quizLocation);
    if (!quizFile.is_open())
    {
        cout << "Error: Unable to open the quiz file!" << endl;
        return;
    }

    string fileQuizName, fileCourseName;
    int totalQuestions;

    while (quizFile >> fileQuizName >> fileCourseName >> totalQuestions)
    {
        if (fileQuizName == quizName)
        {
            quizFound = true;
            courseName = fileCourseName;
            break;
        }
        // Skip over questions for quizzes not being searched
        string temp;
        for (int i = 0; i < totalQuestions * 5; i++)
        {
            getline(quizFile, temp); // Skip question and options
        }
    }
    quizFile.close();

    if (!quizFound)
    {
        cout << "Error: Quiz not found!" << endl;
        return;
    }

    // Load students and assign quiz
    int studentCount = 0;
    Student *students = loadAllStudents(studentsLocation, studentCount);
    ofstream assignedFile(assignedLocation, ios::app);
    if (!assignedFile.is_open())
    {
        cout << "Error: Unable to open the assigned quiz file!" << endl;
        delete[] students;
        return;
    }

    bool sectionFound = false;

    for (int i = 0; i < studentCount; i++)
    {
        // Check if the student is in the correct section and enrolled in the course
        if (students[i].section == section && students[i].coursesPtr[courseToIndex(courseName)])
        {
            sectionFound = true;
            assignedFile << students[i].rollNo << " " << quizName << endl;
        }
    }

    if (!sectionFound)
    {
        cout << "No students in section " << section << " are enrolled in the course " << courseName << "." << endl;
    }
    else
    {
        cout << "Quiz \"" << quizName << "\" assigned successfully to section " << section << "." << endl;
    }

    assignedFile.close();
    delete[] students;
}

// Assign Grades
void assignGrades(const string &resultsLocation, const string &gradesLocation)
{
    string studentUsername, quizName;
    int obtainedMarks, totalMarks;
    bool resultsFound = false;

    ifstream resultsFile(resultsLocation);
    if (!resultsFile.is_open())
    {
        cout << "Error: Unable to open the results file!" << endl;
        return;
    }

    // Display results and percentages for each student
    cout << "Student Quiz Results:\n";
    while (resultsFile >> studentUsername >> quizName >> obtainedMarks >> totalMarks)
    {
        resultsFound = true;
        double percentage = (double)obtainedMarks / totalMarks * 100.0;

        cout << "Student Username: " << studentUsername << endl;
        cout << "Quiz: " << quizName << endl;
        cout << "Obtained Marks: " << obtainedMarks << " / " << totalMarks << endl;
        cout << "Percentage: " << percentage << "%" << endl;

        // Allow the teacher to assign a grade
        cout << "Assign Grade (A/B/C/D/F) for " << studentUsername << ": ";
        char grade;
        cin >> grade;

        // Save the grade to the grades file
        ofstream gradesFile(gradesLocation, ios::app);
        if (!gradesFile.is_open())
        {
            cout << "Error: Unable to save the grades!" << endl;
            resultsFile.close();
            return;
        }
        gradesFile << studentUsername << " " << quizName << " " << grade << endl;
        gradesFile.close();
    }
    resultsFile.close();

    if (!resultsFound)
    {
        cout << "No quiz results found!" << endl;
    }
    else
    {
        cout << "Grades assigned successfully!" << endl;
    }
}

// View Results
void viewResults(const string &resultsLocation, const string &studentsLocation)
{
    char choice;
    cout << "Choose an option to view results:\n";
    cout << "1. View results of a specific section\n";
    cout << "2. View results of a specific student\n";
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice == '1')
    {
        char section;
        cout << "Enter the section (A, B, C): ";
        cin >> section;

        // Load students data
        int studentCount = 0;
        Student *students = loadAllStudents(studentsLocation, studentCount);

        // Open results file
        ifstream resultsFile(resultsLocation);
        if (!resultsFile.is_open())
        {
            cout << "Error: Unable to open the results file!" << endl;
            delete[] students;
            return;
        }

        cout << "Results for Section " << section << ":\n";
        bool found = false;
        string studentUsername, quizName;
        int obtainedMarks, totalMarks;

        while (resultsFile >> studentUsername >> quizName >> obtainedMarks >> totalMarks)
        {
            // Match section for each student
            for (int i = 0; i < studentCount; i++)
            {
                if (students[i].rollNo == studentUsername && students[i].section == section)
                {
                    found = true;
                    cout << "Student Roll No: " << studentUsername << endl;
                    cout << "Quiz: " << quizName << endl;
                    cout << "Score: " << obtainedMarks << "/" << totalMarks << endl;
                    cout << "---------------------\n";
                }
            }
        }

        if (!found)
        {
            cout << "No results found for Section " << section << "." << endl;
        }

        resultsFile.close();
        delete[] students;
    }
    else if (choice == '2')
    {
        string studentRollNo;
        cout << "Enter the roll number of the student: ";
        cin >> studentRollNo;

        // Open results file
        ifstream resultsFile(resultsLocation);
        if (!resultsFile.is_open())
        {
            cout << "Error: Unable to open the results file!" << endl;
            return;
        }

        cout << "Results for Student Roll No: " << studentRollNo << ":\n";
        bool found = false;
        string quizName;
        int obtainedMarks, totalMarks;

        while (resultsFile >> studentRollNo >> quizName >> obtainedMarks >> totalMarks)
        {
            if (studentRollNo == studentRollNo)
            {
                found = true;
                cout << "Quiz: " << quizName << endl;
                cout << "Score: " << obtainedMarks << "/" << totalMarks << endl;
                cout << "---------------------\n";
            }
        }

        if (!found)
        {
            cout << "No results found for Student Roll No: " << studentRollNo << "." << endl;
        }

        resultsFile.close();
    }
    else
    {
        cout << "Invalid choice! Returning to the menu.\n";
    }
}

// teacher menu
void teacherMenu(const User &loggedInUser)
{
    bool flag = true;
    char choice;
    while (flag)
    {
        system("cls");
        cout << "Hello " << loggedInUser.name << " Welcome to Teacher Panel" << endl;
        cout << "Teacher Menu:" << endl;
        cout << "1. Create Quiz" << endl;
        cout << "2. Edit Quiz" << endl;
        cout << "3. Preview Quiz" << endl;
        cout << "4. Assign Quiz" << endl;
        cout << "5. Assign Grades" << endl;
        cout << "6. View Grades" << endl;
        cout << "7. Logout" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case '1': // Create Quiz
            createQuiz("Data/quizez.txt");
            system("pause");
            break;
        case '2':
            editQuiz("Data/quizez.txt");
            system("pause");
            break;
        case '3':
            previewQuiz("Data/quizez.txt");
            system("pause");
            break;
        case '4': // Assign Quiz
            assignQuiz("Data/quizez.txt", "Data/students.txt", "Data/assigned.txt");
            Sleep(2000);
            break;
        case '5':
            assignGrades("Data/results.txt", "Data/grades.txt");
            Sleep(2000);
            break;
        case '6':
            viewResults("Data/results.txt", "Data/students.txt");
            Sleep(2000);
            break;
        case '7':
            flag = false;
            break;
        default:
            cout << "Invalid choice!" << endl;
            Sleep(1000);
            break;
        }
    }
}

// Attempt quiz
void attemptQuiz(const string &quizLocation, const string &assignedLocation, const string &studentUsername, const string &resultsLocation)
{
    bool isQuizAssigned = false;
    string assignedQuizName;

    // Check if the quiz is assigned to the student
    ifstream assignedFile(assignedLocation);
    if (!assignedFile.is_open())
    {
        cout << "Error: Unable to open the assigned quiz file!" << endl;
        return;
    }

    while (assignedFile >> assignedQuizName)
    {
        string assignedStudent;
        assignedFile >> assignedStudent;
        if (assignedStudent == studentUsername)
        {
            isQuizAssigned = true;
            break;
        }
    }
    assignedFile.close();

    if (!isQuizAssigned)
    {
        cout << "No quiz is assigned to you!" << endl;
        return;
    }

    // Open quiz file to locate the quiz details
    ifstream quizFile(quizLocation);
    if (!quizFile.is_open())
    {
        cout << "Error: Unable to open the quiz file!" << endl;
        return;
    }

    string quizName, courseName, question, options[4];
    int totalQuestions, correctOption, answer, obtainedMarks = 0;
    bool quizFound = false, isRandom;

    while (quizFile >> quizName >> courseName >> totalQuestions >> isRandom)
    {
        if (quizName == assignedQuizName)
        {
            quizFound = true;
            break;
        }

        // Skip over questions for unrelated quizzes
        for (int i = 0; i < totalQuestions * 5; i++)
        {
            getline(quizFile, question);
        }
    }

    if (!quizFound)
    {
        cout << "Error: Quiz not found in the quiz file!" << endl;
        quizFile.close();
        return;
    }

    // Quiz attempt begins
    Question *questions = new Question[totalQuestions];
    quizFile.ignore(); // Ignore the newline after the last integer read

    for (int i = 0; i < totalQuestions; i++)
    {
        getline(quizFile, questions[i].questionText);
        for (int j = 0; j < 4; j++)
        {
            getline(quizFile, questions[i].options[j]);
        }
        quizFile >> questions[i].correctOption;
        quizFile.ignore(); // Ignore the newline after the correct option
    }

    quizFile.close();

    // Randomize questions if needed
    if (isRandom)
    {
        srand(time(0));
        for (int i = 0; i < totalQuestions; i++)
        {
            int randomIndex = rand() % totalQuestions;
            Question temp = questions[i];
            questions[i] = questions[randomIndex];
            questions[randomIndex] = temp;
        }
    }

    // Display questions and get answers
    for (int i = 0; i < totalQuestions; i++)
    {
        cout << "Question " << i + 1 << ": " << questions[i].questionText << endl;
        for (int j = 0; j < 4; j++)
        {
            cout << j + 1 << ". " << questions[i].options[j] << endl;
        }
        cout << "Enter your answer (1-4): ";
        cin >> answer;

        if (answer == questions[i].correctOption)
        {
            obtainedMarks++;
        }
    }

    // Display result
    cout << "You scored " << obtainedMarks << " out of " << totalQuestions << "!" << endl;

    // Save the result to the results file
    ofstream resultsFile(resultsLocation, ios::app);
    if (!resultsFile.is_open())
    {
        cout << "Error: Unable to save the quiz results!" << endl;
        delete[] questions;
        return;
    }

    resultsFile << studentUsername << " " << quizName << " " << obtainedMarks << " " << totalQuestions << endl;
    resultsFile.close();

    cout << "Your result has been saved successfully!" << endl;

    // Free allocated memory
    delete[] questions;
}

// View Grades
void viewGrades(const string &gradesLocation, const string &studentUsername)
{
    ifstream gradesFile(gradesLocation);
    if (!gradesFile.is_open())
    {
        cout << "Error: Unable to open the grades file!" << endl;
        return;
    }

    bool found = false;
    string studentRollNo, quizName;
    char grade;

    cout << "Grades for Student Roll No: " << studentUsername << ":\n";

    while (gradesFile >> studentRollNo >> quizName >> grade)
    {
        if (studentRollNo == studentUsername)
        {
            found = true;
            cout << "Quiz: " << quizName << endl;
            cout << "Grade: " << grade << endl;
            cout << "---------------------\n";
        }
    }

    if (!found)
    {
        cout << "No grades found for your account." << endl;
    }

    gradesFile.close();
}


// student menu
void studentMenu(const User &loggedInUser)
{
    bool flag = true;
    char choice;
    while (flag)
    {
        system("cls");
        cout << "Hello " << loggedInUser.name << " Welcome to Admin Panel" << endl;
        cout << "Student Menu:" << endl;
        cout << "1. Attempt Quiz" << endl;
        cout << "2. View Grades" << endl;
        cout << "3. Logout" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case '1':
            attemptQuiz("Data/quizez.txt", "Data/assigned.txt", loggedInUser.username, "Data/results.txt");
            break;
        case '2':
            viewGrades("Data/grades.txt", loggedInUser.username);
            Sleep(2000); // To pause the screen for 2 seconds
            break;
        case '3':
            flag = false;
            break;
        default:
            cout << "Invalid choice!" << endl;
            Sleep(1000);
            break;
        }
    }
}