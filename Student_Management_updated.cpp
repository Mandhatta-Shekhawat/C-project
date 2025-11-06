#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// ======================= Base class =======================
class Person {
protected:
    int age;
    string name;
public:
    Person() {
        name = "";
        age = 0;
    }

    Person(string n, int a) {
        name = n;
        age = a;
    }

    virtual void display() {
        cout << "Name: " << name << endl;
        cout << "Age: " << age << endl;
    }
};

// ======================= Derived class =======================
class Student : public Person {
private:
    int rollNo;
    float marks;
    static int count;
public:
    Student() {
        name = "";
        age = 0;
        rollNo = 0;
        marks = 0.0;
        count++;
    }

    Student(string n, int a, int r, float m) {
        name = n;
        age = a;
        rollNo = r;
        marks = m;
        count++;
    }

    Student(const Student &other) {
        name = other.name;
        age = other.age;
        rollNo = other.rollNo;
        marks = other.marks;
        count++;
    }

    ~Student() {
        count--;
    }

    void setDetails(string n, int a, int r, float m) {
        this->name = n;
        this->age = a;
        this->rollNo = r;
        this->marks = m;
    }

    void display() override {
        cout << "Roll No: " << rollNo << endl;
        cout << "Name: " << name << endl;
        cout << "Age: " << age << endl;
        cout << "Marks: " << marks << endl;
    }

    friend void updateMarks(Student &s, float newMarks);

    bool searchByRoll(int r) {
        return rollNo == r;
    }

    bool searchByName(string n) {
        return name == n;
    }

    void saveToFile(ofstream &out) {
        out << rollNo << "," << name << "," << age << "," << marks << endl;
    }

    static int getCount() {
        return count;
    }
};

// Initialize static member
int Student::count = 0;

// Friend function
void updateMarks(Student &s, float newMarks) {
    s.marks = newMarks;
}

// ======================= Main Program =======================
int main() {
    const int MAX_STUDENTS = 100;  // fixed array size
    Student students[MAX_STUDENTS];
    int studentCount = 0;

    int choice;

    do {
        cout << "\n===== Student Management System =====\n";
        cout << "1. Add Student\n";
        cout << "2. Display All Students\n";
        cout << "3. Search Student\n";
        cout << "4. Update Marks\n";
        cout << "5. Save to File\n";
        cout << "6. Show Total Students\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            if (studentCount >= MAX_STUDENTS) {
                cout << "Cannot add more students (limit reached).\n";
                continue;
            }

            string name;
            int age, roll;
            float marks;

            cout << "Enter name: ";
            cin.ignore();
            getline(cin, name);

            cout << "Enter Age: ";
            cin >> age;
            cout << "Enter Roll Number: ";
            cin >> roll;
            cout << "Enter Marks: ";
            cin >> marks;

            students[studentCount].setDetails(name, age, roll, marks);
            studentCount++;

            cout << "Student added successfully!\n";
        }

        else if (choice == 2) {
            cout << "\n--- Student List ---\n";
            for (int i = 0; i < studentCount; i++) {
                students[i].display();
                cout << "-------------------\n";
            }
        }

        else if (choice == 3) {
            int searchType;
            cout << "Search by 1. Roll No  2. Name: ";
            cin >> searchType;

            bool found = false;

            if (searchType == 1) {
                int roll;
                cout << "Enter Roll No: ";
                cin >> roll;

                for (int i = 0; i < studentCount; i++) {
                    if (students[i].searchByRoll(roll)) {
                        students[i].display();
                        found = true;
                        break;
                    }
                }
            } else {
                string name;
                cout << "Enter Name: ";
                cin.ignore();
                getline(cin, name);

                for (int i = 0; i < studentCount; i++) {
                    if (students[i].searchByName(name)) {
                        students[i].display();
                        found = true;
                        break;
                    }
                }
            }

            if (!found) {
                cout << "Student not found.\n";
            }
        }

        else if (choice == 4) {
            int roll;
            float newMarks;
            cout << "Enter Roll No to update marks: ";
            cin >> roll;
            cout << "Enter new marks: ";
            cin >> newMarks;

            bool updated = false;
            for (int i = 0; i < studentCount; i++) {
                if (students[i].searchByRoll(roll)) {
                    updateMarks(students[i], newMarks);
                    cout << "Marks updated successfully!\n";
                    updated = true;
                    break;
                }
            }
            if (!updated) cout << "Roll No not found.\n";
        }

        else if (choice == 5) {
            try {
                ofstream fout("students.txt");
                if (!fout) throw runtime_error("File could not be opened!");

                for (int i = 0; i < studentCount; i++) {
                    students[i].saveToFile(fout);
                }

                fout.close();
                cout << "Data saved to file successfully!\n";
            } catch (exception &e) {
                cout << "Error: " << e.what() << endl;
            }
        }

        else if (choice == 6) {
            cout << "Total students currently in memory: " << Student::getCount() << endl;
        }

    } while (choice != 0);

    cout << "Exiting program...\n";
    return 0;
}
