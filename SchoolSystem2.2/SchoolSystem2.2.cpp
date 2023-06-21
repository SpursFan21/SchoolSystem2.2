
// This library provides classes and functions for file input and output operations.
#include <fstream>
// This library provides the vector container class for dynamic arrays with automatic memory management.
#include <vector>
// This library provides the string class and associated functions for string operations.
#include <string>
// This library provides classes and functions for string stream operations.
#include <sstream>
// This library provides classes and functions for input and output operations.
#include <iostream>
// This library provides general-purpose functions and macros, including the exit() function.
#include <cstdlib>
//This library generates numbers for ID code logic
#include <random>
//This libary is used to generate random selection in the parent class
#include <ctime>
//This libary provides a collection of functions for performing various algorithms and operations on sequences of elements.
#include <algorithm>
//This libary provides a set of standard exception classes that can be used to handle runtime errors and exceptional conditions.
#include <stdexcept>


// Function to split a string into tokens
std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

void relogMenu();

class User {
protected:
    std::string username;
    std::string password;
    std::string id;
    std::string type;

public:
    User(const std::string& username, const std::string& password, const std::string& id = "")
        : username(username), password(password) {
        if (id.empty()) {
            // Generate a unique ID code
            this->id = generateUniqueId();
        }
        else {
            this->id = id;
        }
    }

    std::string getUsername() const { return username; }
    std::string getPassword() const { return password; }
    std::string getId() const { return id; }

    virtual void displayMenu() const = 0;

    void signOut() const {
        std::cout << "Signing out..." << std::endl;
        // Perform any necessary cleanup or tasks before signing out

        relogMenu();
    }

    std::string generateUniqueId() {
        static std::mt19937 randomEngine(std::random_device{}());
        static std::uniform_int_distribution<std::mt19937::result_type> distribution(1000, 9999);

        time_t uniqueId = static_cast<time_t>(distribution(randomEngine));
        return "ID" + std::to_string(uniqueId);
    }

    std::string getType() const {
        return type;
    }

};

// Admin class derived from User
class Admin : public User {
public:

    std::vector<std::string> classes;  // Declaration of the classes vector

    Admin(const std::string& username, const std::string& password, const std::string& id = "")
        : User(username, password) {
        std::string generatedId = ""; // Initialize the variable to an empty string
        if (id.empty()) {
            // Generate a unique ID code
            generatedId = generateUniqueId();
        }
        else {
            generatedId = id;
        }
        type = "Admin";
        this->id = generatedId;
    }

    void addClass(const std::string& className) {
        // Open the classes.txt file in append mode
        std::ofstream classesFile("classes.txt", std::ios::app);
        if (classesFile.is_open()) {
            // Write the class name followed by a newline character
            classesFile << className << "\n";
            classesFile.close();
            std::cout << "Class added successfully." << std::endl;

            // Add the class to the admin's class list
            classes.push_back(className);
        }
        else {
            std::cout << "Failed to open classes.txt" << std::endl;
        }
    }

    void displayMenu() const override {
        std::cout << "Admin Menu" << std::endl;
        std::cout << "User Type: " << getType() << std::endl;
        std::cout << "User ID: " << getId() << std::endl;
        std::cout << "1. Register User" << std::endl;
        std::cout << "2. Update User" << std::endl;
        std::cout << "3. Delete User" << std::endl;
        std::cout << "4. View User Information" << std::endl;
        std::cout << "5. Add Student" << std::endl;
        std::cout << "6. Add Teacher" << std::endl;
        std::cout << "7. Add Class" << std::endl;
        std::cout << "8. Add Grade for Student" << std::endl;
        std::cout << "9. Sign Out" << std::endl;
    }
};

// Teacher class derived from User
class Teacher : public User {
public:
    Teacher(const std::string& username, const std::string& password) : User(username, password) {
        type = "Teacher";
    }
    Teacher(const std::string& id, const std::string& username, const std::string& password)
        : User(username, password) {
        this->id = id;
        type = "Teacher";
    }
    Teacher(const std::string& teacherName) : User(teacherName, "") {
        type = "Teacher";
    }


    void displayMenu() const override {
        int choice;

        while (true) {
            std::cout << "Teacher User Menu" << std::endl;
            std::cout << "User Type: " << getType() << std::endl;
            std::cout << "User ID: " << getId() << std::endl;
            std::cout << "1. Add Grade for Student" << std::endl;
            std::cout << "2. View Grades" << std::endl;
            std::cout << "3. Sign Out" << std::endl;
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            switch (choice) {
            case 1: {
                std::string studentName;
                std::string grades;

                std::cout << "Enter student name: ";
                std::cin >> studentName;

                std::cout << "Enter grades (format: subject1:grade1 subject2:grade2 ...): ";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::getline(std::cin, grades);

                try {
                    std::ifstream file("grades.txt");
                    std::ofstream tempFile("temp_grades.txt");
                    bool found = false;

                    if (file.is_open() && tempFile.is_open()) {
                        std::string line;
                        while (std::getline(file, line)) {
                            std::istringstream iss(line);
                            std::string name;
                            std::string subjectsAndGrades;

                            if (iss >> name >> subjectsAndGrades) {
                                if (name == studentName) {
                                    found = true;
                                    tempFile << studentName << " " << grades << std::endl;
                                }
                                else {
                                    tempFile << name << " " << subjectsAndGrades << std::endl;
                                }
                            }
                        }

                        file.close();
                        tempFile.close();

                        if (found) {
                            // Replace the original grades file with the updated temporary file
                            if (std::remove("grades.txt") == 0 && std::rename("temp_grades.txt", "grades.txt") == 0) {
                                std::cout << "Grades updated successfully." << std::endl;
                            }
                            else {
                                std::cout << "Failed to update grades." << std::endl;
                            }
                        }
                        else {
                            // If the student name was not found, append the new grades to the end of the file
                            std::ofstream newGradeFile("grades.txt", std::ios::app);
                            if (newGradeFile.is_open()) {
                                newGradeFile << studentName << " " << grades << std::endl;
                                newGradeFile.close();
                                std::cout << "Grades added successfully." << std::endl;
                            }
                            else {
                                std::cout << "Failed to add grades." << std::endl;
                            }
                        }
                    }
                    else {
                        throw std::runtime_error("Unable to open file.");
                    }
                }
                catch (const std::exception& e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }
                break;
            }

            case 2: {
                try {
                    std::ifstream file("grades.txt");
                    if (file.is_open()) {
                        std::string line;
                        while (std::getline(file, line)) {
                            std::cout << line << std::endl;
                        }
                        file.close();
                    }
                    else {
                        throw std::runtime_error("Unable to open file.");
                    }
                }
                catch (const std::exception& e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }
                break;
            }

            case 3:
                signOut(); // Call the signOut function to sign out and return to the login menu
                return;

            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
            }
        }
    }

};

// Parent class derived from User
class Parent : public User {
public:
    Parent(const std::string& id, const std::string& username, const std::string& password)
        : User(username, password) {
        this->id = id;
        type = "Parent";
    }

    Parent(const std::string& username, const std::string& password)
        : User(username, password) {}

    void displayMenu() const override {
        try {
            std::cout << "Parent Menu" << std::endl;
            std::cout << "User Type: Parent" << std::endl;
            std::cout << "User ID: " << getId() << std::endl;
            std::cout << "1. View Student Report Card" << std::endl;
            std::cout << "2. Sign Out" << std::endl;

            int choice;
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            switch (choice) {
            case 1:
                viewStudentReportCard();
                break;
            case 2:
                User::signOut(); // Call the signOut function of the User base class
                return;
            default:
                std::cout << "Invalid choice." << std::endl;
                break;
            }

            // Prompt the parent user to continue
            char continueChoice;
            std::cout << "Do you want to continue? (Y/N): ";
            std::cin >> continueChoice;

            if (continueChoice == 'Y' || continueChoice == 'y') {
                displayMenu(); // Reload the parent menu
            }
        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }

    void viewStudentReportCard() const {
        try {
            std::ifstream file("grades.txt");
            if (!file.is_open()) {
                throw std::runtime_error("Unable to open grades file.");
            }

            std::string line;

            // Read the first line from the file
            if (std::getline(file, line)) {
                std::stringstream ss(line);
                std::string studentInfo, classGrade;

                std::getline(ss, studentInfo, ':');  // Extract the student information
                std::cout << "Student: " << studentInfo << std::endl;

                while (std::getline(ss, classGrade, ',')) {
                    std::stringstream gradeSS(classGrade);
                    std::string className, grade;

                    std::getline(gradeSS, className, ':');  // Extract the class name
                    std::getline(gradeSS, grade, ':');      // Extract the grade

                    std::cout << className << ": " << grade << std::endl;
                }
            }
            else {
                std::cout << "No student grades found." << std::endl;
            }

            file.close();
        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }


};

// StudentUser class derived from User
class StudentUser : public User {
public:
    StudentUser(const std::string& username, const std::string& password) : User("", username, password) {
        type = "Student";
    }

    StudentUser(const std::string& id, const std::string& username, const std::string& password)
        : User(id, username, password) {type = "Student"; }

    StudentUser(const std::string& name) : User("", name, "") { type = "Student"; }

    void updateGrade(int index, int grade) {
        if (index >= 0 && index < grades.size()) {
            grades[index] = grade;
        }
    }
    void viewGrades() const {
        std::ifstream file("grades.txt");
        std::string line;

        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string studentUsername;
            std::string studentName;
            std::string subjectGradePair;

            if (std::getline(ss, studentUsername, ':') && std::getline(ss, studentName, ':') && std::getline(ss, subjectGradePair)) {
                if (studentUsername == getUsername()) {
                    std::cout << "Grades for " << studentName << ": " << subjectGradePair << std::endl;
                    return;
                }
            }
        }

        std::cout << "No grades found for " << getUsername() << std::endl;
    }

    void displayMenu() const override {
        try {
            int choice;

            while (true) {
                std::cout << "Student User Menu" << std::endl;
                std::cout << "User Type: " << getType() << std::endl;
                std::cout << "User ID: " << getId() << std::endl;
                std::cout << "1. View Grades" << std::endl;
                std::cout << "2. Sign Out" << std::endl;
                std::cout << "Enter your choice: ";
                std::cin >> choice;

                if (std::cin.fail()) {
                    std::cin.clear();               // Clear the error state
                    std::cin.ignore(10000, '\n');   // Skip remaining input in the buffer
                    throw std::runtime_error("Invalid choice. Please enter a valid integer.");
                }

                switch (choice) {
                case 1:
                    viewGrades();
                    break;
                case 2:
                    signOut();
                    return;
                default:
                    std::cout << "Invalid choice. Please try again." << std::endl;
                    break;
                }
            }
        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }


    std::string getName() const { return getUsername(); }

    const std::vector<std::string>& getClasses() const { return classes; }

    std::vector<int> getGrades() const {
    return grades;
}

protected:
    std::vector<int> grades; // Vector to store grades
    std::vector<std::string> classes; // Vector to store class names

private:
    std::string studentId; // Student ID
};

class School
{
private:
    std::vector<User*> users;
    std::vector<StudentUser> students;

public:
    ~School() {
        // Clean up dynamically allocated user objects
        for (User* user : users) {
            delete user;
        }
    }

    std::vector<StudentUser>& getStudents() {
        return students;
    }

    void registerUser(User* user) {
        users.push_back(user);
    }

    User* findUser(const std::string& username) const {
        for (User* user : users) {
            if (user->getUsername() == username) {
                return user;
            }
        }
        return nullptr; // User not found
    }

    void deleteUser(const std::string& username) {
        try {
            // Find the user to delete
            auto it = std::find_if(users.begin(), users.end(), [&](const User* user) {
                return user->getUsername() == username;
                });

            if (it != users.end()) {
                delete* it;
                users.erase(it);
            }
            else {
                throw std::runtime_error("User not found.");
            }
        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }




    void addStudent(const std::string& name) {
        students.push_back(StudentUser(name));
    }

    StudentUser* findStudent(const std::string& name) {
        try {
            for (StudentUser& student : students) {
                if (student.getName() == name) {
                    return &student;
                }
            }
            throw std::runtime_error("Student not found.");
        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
            return nullptr;
        }
    }
};





    // Function to view user information from the file
    void viewUserInformation(const std::string& filename, const std::string& username) {
        try {
            std::ifstream inputFile(filename);
            if (!inputFile) {
                throw std::runtime_error("Failed to open the file");
            }

            std::string line;
            bool userFound = false;

            while (std::getline(inputFile, line)) {
                std::vector<std::string> userData = splitString(line, ',');
                if (userData.size() >= 3 && userData[1] == username) {
                    // Display the user information
                    std::cout << "Username: " << userData[1] << std::endl;
                    std::cout << "Password: " << userData[2] << std::endl;
                    userFound = true;
                    break;
                }
            }

            inputFile.close();

            if (!userFound) {
                std::cout << "User not found." << std::endl;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }


    // Function to update user information in the file
    void updateUserInformation(const std::string& username, const std::string& newUsername, const std::string& newPassword) {
        std::vector<std::string> userRecords;

        // Read the existing user information from the file
        std::ifstream inputFile("users.txt");
        if (inputFile.is_open()) {
            std::string line;
            while (std::getline(inputFile, line)) {
                userRecords.push_back(line);
            }
            inputFile.close();
        }
        else {
            std::cerr << "Error opening the file" << std::endl;
            return;
        }

        // Search for the username in the user records
        bool userFound = false;
        for (auto& record : userRecords) {
            std::vector<std::string> userData = splitString(record, ',');
            if (userData[1] == username) {
                // Update the user information
                userData[1] = newUsername;
                userData[2] = newPassword;

                // Modify the record in the user records
                record = userData[0] + "," + userData[1] + "," + userData[2];

                userFound = true;
                break;
            }
        }

        // Write the modified user information back to the file
        std::ofstream outputFile("users.txt");
        if (outputFile.is_open()) {
            for (const auto& record : userRecords) {
                outputFile << record << std::endl;
            }
            outputFile.close();
            std::cout << "User information updated successfully." << std::endl;
        }
        else {
            std::cerr << "Error opening the file" << std::endl;
        }

        if (!userFound) {
            std::cout << "User not found." << std::endl;
        }
    }

    // Function to delete a user from the users.txt file
    void deleteUserFromFile(const std::string& username) {
        std::ifstream inFile("users.txt");
        if (!inFile) {
            std::cout << "Error opening file." << std::endl;
            return;
        }

        std::ofstream outFile("users.txt");
        if (!outFile) {
            std::cout << "Error creating temporary file." << std::endl;
            inFile.close();
            return;
        }

        std::string line;
        bool userFound = false;

        while (std::getline(inFile, line)) {
            if (line.substr(0, username.length()) != username) {
                // Write the line to the temporary file if the username doesn't match
                outFile << line << std::endl;
            }
            else {
                // Set the flag to indicate that the user was found
                userFound = true;
            }
        }

        inFile.close();
        outFile.close();

        // Delete the original file and rename the temporary file to users.txt
        if (userFound) {
            if (std::remove("users.txt") != 0) {
                std::cout << "Error deleting file." << std::endl;
                return;
            }

            if (std::rename("temp.txt", "users.txt") != 0) {
                std::cout << "Error renaming file." << std::endl;
                return;
            }

            std::cout << "User deleted successfully." << std::endl;
        }
        else {
            std::cout << "User not found." << std::endl;
        }
    }

    bool isValidCredentials(const std::string& username, const std::string& password) {
        std::ifstream userFile("users.txt");
        if (userFile.is_open()) {
            std::string line;
            while (std::getline(userFile, line)) {
                std::vector<std::string> userData = splitString(line, ',');
                if (userData.size() >= 3 && userData[1] == username && userData[2] == password) {
                    userFile.close();
                    return true;
                }
                else {
                    std::cout << "Debug - Username: " << userData[1] << ", Password: " << userData[2] << std::endl;
                }
            }
            userFile.close();
        }
        return false;
    }

    void relogMenu() {
        School school; // Creating an instance of the School class

        User* currentUser = nullptr;
        bool loggedIn = false;
        bool exitProgram = false;

        int choice;

        while (!loggedIn && !exitProgram) {
            std::cout << "===== Start Menu =====" << std::endl;
            std::cout << "1. Log in" << std::endl;
            std::cout << "2. New Admin" << std::endl;
            std::cout << "3. Exit" << std::endl;
            std::cout << "======================" << std::endl;
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            switch (choice) {
            case 1: {
                std::string username, password;
                std::cout << "Enter username: ";
                try {
                    std::cin >> username;
                }
                catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                    break;
                }

                std::cout << "Enter password: ";
                try {
                    std::cin >> password;
                }
                catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                    break;
                }

                User* user = school.findUser(username);
                if (user && user->getPassword().compare(password) == 0) {
                    currentUser = user;
                    loggedIn = true;
                }
                else {
                    std::cout << "Invalid username or password" << std::endl;
                }
                break;
            }
            case 2: {
                // Create new admin logic
                std::string newAdminUsername, newAdminPassword;
                std::cout << "Enter new admin username: ";
                std::cin >> newAdminUsername;
                std::cout << "Enter new admin password: ";
                std::cin >> newAdminPassword;

                Admin* newAdmin = new Admin(newAdminUsername, newAdminPassword);
                school.registerUser(newAdmin);

                std::cout << "New admin created successfully!" << std::endl;
                break;
            }

            case 3: {
                std::cout << "Exiting..." << std::endl;
                exit(0);
            }
            default:
                std::cout << "Invalid choice" << std::endl;
                break;
            }
        }
    }


    void exitProgram()
    {
        std::cout << "Exiting..." << std::endl;
        exit(0);
    }

    int main()
    {
        School school;
        std::string line;
        // Creating Local user objects
        Admin admin("admin", "admin123", "adminID");  // Pass the username, password, and ID
        Teacher teacher("teacher", "teacher123");
        Parent parent("parent", "parent123");
        StudentUser student("student", "student123", "studentID");

        std::vector<User*> users;
        users.push_back(&admin);
        users.push_back(&teacher);
        users.push_back(&parent);
        users.push_back(&student);

        school.registerUser(&admin);
        school.registerUser(&teacher);
        school.registerUser(&parent);
        school.registerUser(&student);

        std::ifstream userFile("users.txt");
        if (userFile.is_open()) {
            std::string line;
            while (std::getline(userFile, line)) {
                std::vector<std::string> tokens = splitString(line, ',');

                // Check if the line has at least 4 tokens (userType, username, password, userID)
                if (tokens.size() >= 4) {
                    std::string userType = tokens[0];
                    std::string username = tokens[1];
                    std::string password = tokens[2];
                    std::string userID = tokens[3];

                    if (userType == "admin") {
                        Admin* admin = new Admin(userID, username, password);
                        school.registerUser(admin);
                    }
                    else if (userType == "teacher") {
                        Teacher* teacher = new Teacher(userID, username, password);
                        school.registerUser(teacher);
                    }
                    else if (userType == "parent") {
                        Parent* parent = new Parent(userID, username, password);
                        school.registerUser(parent);
                    }
                    else if (userType == "student") {
                        StudentUser* student = new StudentUser(userID, username, password);
                        school.registerUser(student);
                    }
                }
            }
            userFile.close();
        }
        else {
            std::cout << "Failed to open users.txt" << std::endl;
            return 1;
        }

        User* currentUser = nullptr;
        bool loggedIn = false;
        bool exitProgram = false;

        int choice;

        while (!loggedIn && !exitProgram) {
            std::cout << "===== Start Menu =====" << std::endl;
            std::cout << "1. Log in" << std::endl;
            std::cout << "2. New Admin" << std::endl;
            std::cout << "3. Exit" << std::endl;
            std::cout << "======================" << std::endl;
            std::cout << "Enter your choice: ";

            std::string input;
            std::cin >> input;

            try {
                choice = std::stoi(input);

                switch (choice) {
                case 1: {
                    std::string username, password;
                    std::cout << "Enter username: ";
                    std::cin >> username;

                    std::cout << "Enter password: ";
                    std::cin >> password;

                    User* user = school.findUser(username);
                    if (user && user->getPassword().compare(password) == 0) {
                        currentUser = user;
                        loggedIn = true;
                    }
                    else {
                        std::cout << "Invalid username or password" << std::endl;
                    }
                    break;
                }
                case 2: {
                    // Create new admin logic
                    std::string newAdminUsername, newAdminPassword;
                    std::cout << "Enter new admin username: ";
                    std::cin >> newAdminUsername;
                    std::cout << "Enter new admin password: ";
                    std::cin >> newAdminPassword;

                    Admin* newAdmin = new Admin(newAdminUsername, newAdminPassword);
                    school.registerUser(newAdmin);

                    std::cout << "New admin created successfully!" << std::endl;
                    break;
                }
                case 3: {
                    std::cout << "Exiting..." << std::endl;
                    exitProgram = true;
                    break;
                }
                default:
                    throw std::invalid_argument("Invalid choice");
                }
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << ". Please enter a valid choice." << std::endl;
                // Clear the input stream
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }


        while (loggedIn) {
            currentUser->displayMenu();
            bool validChoice = false;
            std::string continueChoice; // Declare continueChoice variable

            while (!validChoice) {
                // ...

                std::cout << "Enter your choice: ";
                std::string input;
                std::cin >> input;

                try {
                    choice = std::stoi(input);

                    switch (choice) {
                    case 1: {
                        // Register User
                        std::string userType, username, password;
                        std::cout << "Enter the user type (admin/teacher/parent/student): ";
                        std::cin >> userType;
                        std::cout << "Enter the username: ";
                        std::cin >> username;
                        std::cout << "Enter the password: ";
                        std::cin >> password;

                        std::cout << "User registered successfully." << std::endl;

                        // Save user information to file
                        std::ofstream outputFile("users.txt", std::ios::app);
                        if (outputFile.is_open()) {
                            outputFile << userType << "," << username << "," << password << std::endl;
                            outputFile.close();
                            std::cout << "User information saved to users.txt" << std::endl;
                        }
                        else {
                            throw std::runtime_error("Error opening the file");
                        }

                        if (continueChoice == "no") {
                            exitProgram = true;
                        }
                        else {
                            validChoice = true;
                        }
                        break;
                    }

                    case 2: {
                        // Update User
                        std::string username;
                        std::cout << "Enter the username of the user to update: ";
                        std::cin >> username;

                        std::string newUsername;
                        std::cout << "Enter the new username: ";
                        std::cin >> newUsername;

                        std::string newPassword;
                        std::cout << "Enter the new password: ";
                        std::cin >> newPassword;

                        updateUserInformation(username, newUsername, newPassword);
                        std::cout << "User updated successfully." << std::endl;

                        validChoice = true;
                        break;
                    }

                    case 3: {
                        // Delete User
                        std::string username;
                        std::cout << "Enter the username of the user to delete: ";
                        std::cin >> username;

                        school.deleteUser(username);

                        std::cout << "User deleted successfully." << std::endl;
                        validChoice = true;
                        break;
                    }

                    case 4: {
                        // View User Information
                        std::string username;
                        std::cout << "Enter the username of the user to view information: ";
                        std::cin >> username;

                        viewUserInformation("users.txt", username);
                        validChoice = true;
                        break;
                    }

                    case 5: {
                        // Add Student
                        std::string studentName;
                        std::cout << "Enter the name of the student to add: ";
                        std::cin >> studentName;

                        school.addStudent(studentName);
                        std::cout << "Student added successfully." << std::endl;
                        validChoice = true;
                        break;
                    }
                    case 6: {
                        // Add Teacher
                        std::string teacherName;
                        std::cout << "Enter the name of the teacher to add: ";
                        std::cin >> teacherName;

                        // Create a new Teacher object
                        Teacher newTeacher(teacherName);

                        // Save teacher information to file
                        std::ofstream staffFile;
                        staffFile.exceptions(std::ofstream::failbit | std::ofstream::badbit);
                        try {
                            staffFile.open("staff.txt", std::ios::app);
                            staffFile << "Teacher Name: " << newTeacher.getUsername() << "\n";
                            staffFile.close();
                            std::cout << "Teacher added successfully." << std::endl;
                        }
                        catch (const std::ofstream::failure& e) {
                            std::cerr << "Error writing to staff.txt: " << e.what() << std::endl;
                        }

                        validChoice = true;
                        break;
                    }


                    case 7: {
                        // Add Class
                        std::string className;
                        std::cout << "Enter the name of the class to add: ";
                        std::cin >> className;

                        // Assuming you have an Admin object for the current admin
                        if (currentUser->getUsername() == "admin") {
                            Admin* admin = dynamic_cast<Admin*>(currentUser);
                            admin->addClass(className);

                            // Update the classes.txt file with the new class
                            std::ofstream classesFile("classes.txt", std::ios::app);
                            if (classesFile.is_open()) {
                                classesFile << className << "\n";
                                classesFile.close();
                                std::cout << "Class added successfully." << std::endl;
                            }
                            else {
                                throw std::runtime_error("Failed to open classes.txt");
                            }
                        }
                        else {
                            throw std::invalid_argument("Invalid user type for adding a class.");
                        }

                        validChoice = true;
                        break;
                    }

                    case 8: {
                        // Add Grade for Student
                        std::string studentName;
                        std::cout << "Enter the name of the student: ";
                        std::cin >> studentName;

                        // Find the student in the school
                        StudentUser* student = school.findStudent(studentName);
                        if (student) {
                            // Display the student's classes
                            const std::vector<std::string>& classes = student->getClasses();
                            std::cout << "Classes: ";
                            for (const std::string& className : classes) {
                                std::cout << className << ", ";
                            }
                            std::cout << std::endl;

                            // Select a class to add the grade
                            std::string className;
                            std::cout << "Enter the class name to add the grade: ";
                            std::cin >> className;

                            // Find the index of the class in the student's classes vector
                            size_t index = -1;
                            for (size_t i = 0; i < classes.size(); i++) {
                                if (classes[i] == className) {
                                    index = i;
                                    break;
                                }
                            }

                            if (index != -1) {
                                // Prompt for the grade
                                int grade;
                                std::cout << "Enter the grade for " << className << ": ";
                                std::cin >> grade;

                                // Update the grade for the selected class
                                student->updateGrade(index, grade);

                                // Save the updated student information to the file
                                std::ofstream studentFile("students.txt");
                                if (studentFile.is_open()) {
                                    for (const StudentUser& s : school.getStudents()) {
                                        studentFile << s.getName() << ",";
                                        const std::vector<std::string>& studentClasses = s.getClasses();
                                        const std::vector<int>& studentGrades = s.getGrades();
                                        for (size_t i = 0; i < studentClasses.size(); i++) {
                                            studentFile << studentClasses[i] << ":" << studentGrades[i];
                                            if (i < studentClasses.size() - 1) {
                                                studentFile << ",";
                                            }
                                        }
                                        studentFile << "\n";
                                    }
                                    studentFile.close();
                                    std::cout << "Grade added successfully." << std::endl;
                                }
                                else {
                                    throw std::runtime_error("Failed to open students.txt");
                                }
                            }
                            else {
                                throw std::invalid_argument("Class not found for the student.");
                            }
                        }
                        else {
                            throw std::invalid_argument("Student not found.");
                        }

                        validChoice = true;
                        break;
                    }

                    case 9: {
                        currentUser->signOut();
                        break;

                    }
                    default:
                        throw std::invalid_argument("Invalid choice. Please try again.");
                    }
                }
                catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                }

            }

            std::cout << "Do you want to continue? (yes/no): ";
            std::cin >> continueChoice;

            if (continueChoice == "no") {
                exitProgram = true;
                exit(0);
            }
        }
    }
