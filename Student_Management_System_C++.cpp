#include<iostream>
#include<fstream>
#include<string>
using namespace std;
// Base class which is person
class Person{
    protected:
    int age;
    string name;
    public:
    // Default constructor
    Person() {
    name = "";
    age = 0;
}
// Paramatrized constructor
Person(string n, int a) {
    name = n;
    age = a;
}
    virtual void display() {
    cout << "Name: " << name << endl;
    cout << "Age: " << age << endl;
}

};
// Derived Class i.e. Student
class Student : public Person{
    protected:
    int rollNo;
    float marks;
   static int count;
    public:
    // Default constructor
    Student() {
    name = "";
    age = 0;
    rollNo = 0;
    marks = 0.0;
    count++;
}
// Paramatrized Constructor
Student(string n, int a, int r, float m) {
    name = n;
    age = a;
    rollNo = r;
    marks = m;
    count++;
}
// Copy Constructor
Student(const Student &other) {
    name = other.name;
    age = other.age;
    rollNo = other.rollNo;
    marks = other.marks;
    count++;
}
// Destructor
  ~Student(){
    count--;
  }

// Member Functions
void setDetails(string n, int a, int r, float m) {
    this->name = n; // using this pointer
    this->age = a;
    this->rollNo = r;
    this->marks = m;
}
void display()override {
    cout << "Roll No: " << rollNo << endl;
    cout << "Name: " << name << endl;
    cout << "Age: " << age << endl;
    cout << "Marks: " << marks << endl;
}
 // Friend function to access private data of the base class
    friend void updateMarks(Student &s, float newMarks);
bool searchByRoll(int r) {
    if (rollNo == r)
        return true;
    else
        return false;
}

bool searchByName(string n) {
    if (name == n)
        return true;
    else
        return false;
}
// saves the data to file
void saveToFile(ofstream &out) {
    out << rollNo << ",";   // write roll number
    out << name << ",";     // write name
    out << age << ",";      // write age
    out << marks << endl;   // write marks and move to next line
}
string getName() const {
    return name;
}

int getAge() const {
    return age;
}

int getRollNo() const {
    return rollNo;
}

float getMarks() const {
    return marks;
}
 static int getCount(){
    return count;
 }
};
// Initialize static member
int Student::count = 0;

// Friend function definition
void updateMarks(Student &s, float newMarks) {
    s.marks = newMarks;
}
// Derived class for Weak Students (inherits from Student)(jMultilevel Inheritance)
class WeakStudent : public Student {
private:
    static const int PASS_MARKS = 40; // Passing marks threshold

public:
    // Default constructor
    WeakStudent() {
    name = "";
    age = 0;
    rollNo = 0;
    marks = 0.0;
}

    // Parametrized constructor
    WeakStudent(string n, int a, int r, float m) {// it receives these from the student class
    name = n;
    age = a;
    rollNo = r;
    marks = m;
}

    // Function to check if student is weak
    bool isWeak() const {
        return getMarks() < PASS_MARKS;
    }

    // Function to save weak student data
    void saveWeakStudent(ofstream &out) {
        if (isWeak()) {
            saveToFile(out);
        }
    }
};

// main programme

int main(){
    const int MAX_STUDENTS = 100;  // Maximum students allowed
	Student students[MAX_STUDENTS]; // Fixed-size array instead of vector as array can have unnumbered inputs
	int studentCount = 0;           // Number of students currently in array
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
            }else{
            string name;
            int age, roll;
            float marks;

            cout<<"Enter name: ";
            cin.ignore(); //clear the buffer i.e. \n which causes the skip of next line
            getline(cin,name);

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
        }
        else if (choice == 2) {
            cout << "\n Student List \n";
            for (int i=0; i<studentCount;i++) {
                students[i].display();
                cout << "\n";
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
        ofstream weakOut("weak_students.txt");
        if (!fout || !weakOut) throw runtime_error("File could not be opened!");

        for (int i = 0; i < studentCount; i++) {
            // Save all students
            students[i].saveToFile(fout);

            // Check if weak
            WeakStudent w(
                students[i].getName(),
                students[i].getAge(),
                students[i].getRollNo(),
                students[i].getMarks()
            );
            if (w.isWeak()) {
                w.saveToFile(weakOut);
            }
        }

        fout.close();
        weakOut.close();
        cout << "Data saved to files successfully!\n";
        cout << "Weak students saved to weak_students.txt successfully!\n";
    }
    catch (exception &e) {
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
