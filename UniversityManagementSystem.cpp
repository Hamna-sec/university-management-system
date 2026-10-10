#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

class Student {
private:
    string rollNo;
    string name;
    string subject;
    string address;

public:
    // Setters & Getters
    void setRollNo(string r) { rollNo = r; }
    string getRollNo() const { return rollNo; }

    void inputStudent() {
        cout << "Enter Roll No: ";
        cin >> rollNo;
        cin.ignore(); // Clear buffer for getline
        cout << "Enter Name: ";
        getline(cin, name);
        cout << "Enter Subject: ";
        getline(cin, subject);
        cout << "Enter Address: ";
        getline(cin, address);
    }

    void displayStudent() const {
        cout << "----------------------------------------\n";
        cout << "Roll No : " << rollNo << "\n";
        cout << "Name    : " << name << "\n";
        cout << "Subject : " << subject << "\n";
        cout << "Address : " << address << "\n";
        cout << "----------------------------------------\n";
    }

    string toFileFormat() const {
        return rollNo + "," + name + "," + subject + "," + address + "\n";
    }

    void fromFileFormat(const string &data) {
        size_t pos1 = data.find(",");
        size_t pos2 = data.find(",", pos1 + 1);
        size_t pos3 = data.find(",", pos2 + 1);

        if (pos1 != string::npos && pos2 != string::npos && pos3 != string::npos) {
            rollNo = data.substr(0, pos1);
            name = data.substr(pos1 + 1, pos2 - pos1 - 1);
            subject = data.substr(pos2 + 1, pos3 - pos2 - 1);
            address = data.substr(pos3 + 1);
        }
    }
};

// File Operations Functions
void saveStudentToFile(Student &s) {
    ofstream file("students.txt", ios::app);
    if (file.is_open()) {
        file << s.toFileFormat();
        file.close();
        cout << "\n[Success] Student record saved successfully!\n";
    } else {
        cout << "\n[Error] Unable to open file for writing!\n";
    }
}

void displayAllStudents() {
    ifstream file("students.txt");
    string line;
    Student s;
    bool found = false;

    if (file.is_open()) {
        cout << "\n================ ALL STUDENT RECORDS ================\n";
        while (getline(file, line)) {
            if (!line.empty()) {
                s.fromFileFormat(line);
                s.displayStudent();
                found = true;
            }
        }
        file.close();
        if (!found) cout << "No records found in database.\n";
    } else {
        cout << "\n[Error] No record file found!\n";
    }
}

void searchStudent(const string &roll) {
    ifstream file("students.txt");
    string line;
    bool found = false;
    Student s;

    if (file.is_open()) {
        while (getline(file, line)) {
            if (!line.empty()) {
                s.fromFileFormat(line);
                if (s.getRollNo() == roll) {
                    cout << "\n[Record Found]:\n";
                    s.displayStudent();
                    found = true;
                    break;
                }
            }
        }
        file.close();
        if (!found) cout << "\n[Notice] Student with Roll No '" << roll << "' not found!\n";
    } else {
        cout << "\n[Error] Unable to open file!\n";
    }
}

void updateStudent(const string &roll) {
    ifstream file("students.txt");
    vector<Student> students;
    string line;
    bool found = false;
    Student s;

    if (file.is_open()) {
        while (getline(file, line)) {
            if (!line.empty()) {
                s.fromFileFormat(line);
                if (s.getRollNo() == roll) {
                    cout << "\n[Student Found] Enter new details:\n";
                    s.inputStudent();
                    found = true;
                }
                students.push_back(s);
            }
        }
        file.close();
    } else {
        cout << "\n[Error] Unable to open file!\n";
        return;
    }

    if (!found) {
        cout << "\n[Notice] Student with Roll No '" << roll << "' not found!\n";
        return;
    }

    ofstream outFile("students.txt");
    for (size_t i = 0; i < students.size(); i++) {
        outFile << students[i].toFileFormat();
    }
    outFile.close();

    cout << "\n[Success] Student details updated successfully!\n";
}

int main() {
    int choice;
    while (true) {
        cout << "\n============================================\n";
        cout << "      UNIVERSITY MANAGEMENT SYSTEM          \n";
        cout << "============================================\n";
        cout << "1. Add New Student\n";
        cout << "2. Search Student Record\n";
        cout << "3. Update Student Record\n";
        cout << "4. Display All Students\n";
        cout << "5. Exit System\n";
        cout << "--------------------------------------------\n";
        cout << "Enter your choice (1-5): ";
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        if (choice == 1) {
            Student s;
            s.inputStudent();
            saveStudentToFile(s);
        } else if (choice == 2) {
            string roll;
            cout << "Enter Roll No to Search: ";
            cin >> roll;
            searchStudent(roll);
        } else if (choice == 3) {
            string roll;
            cout << "Enter Roll No to Update: ";
            cin >> roll;
            updateStudent(roll);
        } else if (choice == 4) {
            displayAllStudents();
        } else if (choice == 5) {
            cout << "\nExiting System. Thank you!\n";
            break;
        } else {
            cout << "\n[Invalid] Please choose a valid option (1-5).\n";
        }
    }
    return 0;
}
