#include <iostream>
#include <string>

using namespace std;

struct Enrollment {
    string courseID;
    double grade;
    Enrollment* next;
};

struct Student {
    int id;
    string name;
    Enrollment* enrollments;
    Student* next;
};

struct WaitlistNode {
    int studentID;
    WaitlistNode* next;
};

struct Course {
    string id;
    string name;
    int credits;
    int capacity;
    int enrolledCount;
    WaitlistNode* waitlist;
    Course* next;
    Course* prev;
};

struct Request {
    int studentID;
    string courseID;
    Request* next;
};

struct UndoAction {
    enum ActionType { ENROLL, DROP, GRADE };
    ActionType type;
    int studentID;
    string courseID;
    double oldGrade;
    UndoAction* next;
};

Student* studentListHead = nullptr;
Course* courseListHead = nullptr;
Course* courseListTail = nullptr;
Request* requestQueueHead = nullptr;
Request* requestQueueTail = nullptr;
UndoAction* undoStackTop = nullptr;

Student* findStudent(int id) {
    Student* curr = studentListHead;
    while (curr) {
        if (curr->id == id) return curr;
        curr = curr->next;
    }
    return nullptr;
}

Course* findCourse(string id) {
    Course* curr = courseListHead;
    while (curr) {
        if (curr->id == id) return curr;
        curr = curr->next;
    }
    return nullptr;
}

void addStudent(int id, string name) {
    if (findStudent(id)) {
        cout << "Error: Student ID already exists!\n";
        return;
    }
    Student* newStudent = new Student{id, name, nullptr, nullptr};
    
    if (!studentListHead) {
        studentListHead = newStudent;
    } else {
        Student* temp = studentListHead;
        while (temp->next) temp = temp->next;
        temp->next = newStudent;
    }
    cout << "Student added successfully.\n";
}

void addCourse(string id, string name, int credits, int cap) {
    if (findCourse(id)) {
        cout << "Error: Course ID already exists!\n";
        return;
    }
    Course* newCourse = new Course{id, name, credits, cap, 0, nullptr, nullptr, nullptr};
    
    if (!courseListHead) {
        courseListHead = courseListTail = newCourse;
    } else {
        courseListTail->next = newCourse;
        newCourse->prev = courseListTail;
        courseListTail = newCourse;
    }
    cout << "Course added successfully.\n";
}

void displayCoursesForward() {
    cout << "\n--- Courses List (Forward) ---\n";
    Course* temp = courseListHead;
    while (temp) {
        cout << "[" << temp->id << "] " << temp->name 
             << " (Cr: " << temp->credits << ") "
             << "Seats: " << temp->enrolledCount << "/" << temp->capacity << endl;
        temp = temp->next;
    }
}

void displayCoursesBackward() {
    cout << "\n--- Courses List (Backward) ---\n";
    Course* temp = courseListTail;
    while (temp) {
        cout << "[" << temp->id << "] " << temp->name << endl;
        temp = temp->prev;
    }
}

void enqueueRequest(int sID, string cID) {
    Request* newReq = new Request{sID, cID, nullptr};
    if (!requestQueueTail) {
        requestQueueHead = requestQueueTail = newReq;
    } else {
        requestQueueTail->next = newReq;
        requestQueueTail = newReq;
    }
    cout << "Request queued: Student " << sID << " -> Course " << cID << endl;
}

void pushUndo(UndoAction::ActionType type, int sID, string cID, double oldGrade = -1) {
    UndoAction* newAction = new UndoAction{type, sID, cID, oldGrade, undoStackTop};
    undoStackTop = newAction;
}

void addToWaitlist(Course* c, int sID) {
    WaitlistNode* newNode = new WaitlistNode{sID, nullptr};
    if (!c->waitlist) {
        c->waitlist = newNode;
        newNode->next = newNode;
    } else {
        WaitlistNode* tail = c->waitlist;
        while (tail->next != c->waitlist) {
            tail = tail->next;
        }
        tail->next = newNode;
        newNode->next = c->waitlist;
    }
    cout << "Course is FULL! Student " << sID << " added to WAITLIST (Circular).\n";
}

void enrollInternal(int sID, string cID, bool recordUndo = true) {
    Student* s = findStudent(sID);
    Course* c = findCourse(cID);
    
    if (!s || !c) {
        cout << "Error: Invalid Student or Course ID during processing.\n";
        return;
    }

    Enrollment* check = s->enrollments;
    while (check) {
        if (check->courseID == cID) {
            cout << "Already enrolled!\n";
            return;
        }
        check = check->next;
    }

    if (c->enrolledCount >= c->capacity) {
        addToWaitlist(c, sID);
        return;
    }

    Enrollment* newEnr = new Enrollment{cID, -1.0, s->enrollments};
    s->enrollments = newEnr;
    c->enrolledCount++;

    if (recordUndo) pushUndo(UndoAction::ENROLL, sID, cID);
    cout << "Success: Enrolled Student " << sID << " in " << cID << endl;
}

void processNextRequest() {
    if (!requestQueueHead) {
        cout << "Queue is empty. No requests to process.\n";
        return;
    }
    Request* temp = requestQueueHead;
    
    cout << "Processing request...\n";
    enrollInternal(temp->studentID, temp->courseID);
    
    requestQueueHead = requestQueueHead->next;
    if (!requestQueueHead) requestQueueTail = nullptr;
    
    delete temp;
}

void dropCourse(int sID, string cID, bool recordUndo = true) {
    Student* s = findStudent(sID);
    Course* c = findCourse(cID);
    if (!s) return;

    Enrollment* curr = s->enrollments;
    Enrollment* prev = nullptr;

    while (curr) {
        if (curr->courseID == cID) {
            if (prev) prev->next = curr->next;
            else s->enrollments = curr->next;
            
            double oldG = curr->grade;
            delete curr;
            if (c) c->enrolledCount--;
            
            if (recordUndo) pushUndo(UndoAction::DROP, sID, cID, oldG);
            cout << "Course " << cID << " dropped for Student " << sID << endl;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    cout << "Enrollment not found to drop.\n";
}

void enterGrade(int sID, string cID, double grade, bool recordUndo = true) {
    Student* s = findStudent(sID);
    if (!s) { cout << "Student not found.\n"; return; }
    
    Enrollment* curr = s->enrollments;
    while (curr) {
        if (curr->courseID == cID) {
            double oldG = curr->grade;
            curr->grade = grade;
            if (recordUndo) pushUndo(UndoAction::GRADE, sID, cID, oldG);
            cout << "Grade updated successfully.\n";
            return;
        }
        curr = curr->next;
    }
    cout << "Student is not enrolled in this course.\n";
}

void printTranscriptRecursive(Enrollment* e) {
    if (!e) return;
    printTranscriptRecursive(e->next);
    cout << " - Course: " << e->courseID << " | Grade: ";
    if (e->grade == -1) cout << "N/A"; else cout << e->grade;
    cout << endl;
}

void calculateGPARecursive(Enrollment* e, double& totalPoints, int& totalCredits) {
    if (!e) return;
    
    if (e->grade != -1) {
        Course* c = findCourse(e->courseID);
        if (c) {
            totalPoints += e->grade * c->credits;
            totalCredits += c->credits;
        }
    }
    calculateGPARecursive(e->next, totalPoints, totalCredits);
}

void printStudentDetails(int sID) {
    Student* s = findStudent(sID);
    if (!s) { cout << "Student not found.\n"; return; }
    
    cout << "\n=== Transcript for " << s->name << " (" << s->id << ") ===\n";
    printTranscriptRecursive(s->enrollments);
    
    double tPoints = 0;
    int tCredits = 0;
    calculateGPARecursive(s->enrollments, tPoints, tCredits);
    
    if (tCredits > 0)
        cout << ">>> Cumulative GPA: " << tPoints / tCredits << endl;
    else
        cout << ">>> GPA: 0.00 (No graded credits)\n";
}

void undoLastAction() {
    if (!undoStackTop) {
        cout << "Undo Stack is empty.\n";
        return;
    }
    UndoAction* u = undoStackTop;
    undoStackTop = undoStackTop->next;

    cout << "Undoing last action... ";
    if (u->type == UndoAction::ENROLL) {
        dropCourse(u->studentID, u->courseID, false); 
    } else if (u->type == UndoAction::DROP) {
        enrollInternal(u->studentID, u->courseID, false);
        if (u->oldGrade != -1) enterGrade(u->studentID, u->courseID, u->oldGrade, false);
    } else if (u->type == UndoAction::GRADE) {
        enterGrade(u->studentID, u->courseID, u->oldGrade, false);
    }
    
    delete u;
}

void cleanUp() {
    // Clean up all allocated memory (students, courses, requests, undo stack)
    while (studentListHead) {
        Student* temp = studentListHead;
        // حذف enrollments الطالب أولاً
        while (temp->enrollments) {
            Enrollment* e = temp->enrollments;
            temp->enrollments = temp->enrollments->next;
            delete e;
        }
        studentListHead = studentListHead->next;
        delete temp;
    }
    while (courseListHead) {
        Course* temp = courseListHead;
        // Delete circular waitlist
        if (temp->waitlist) {
            WaitlistNode* head = temp->waitlist;
            WaitlistNode* curr = head;
            do {
                WaitlistNode* next = curr->next;
                delete curr;
                curr = next;
            } while (curr != head);
        }
        courseListHead = courseListHead->next;
        delete temp;
    }
    while (requestQueueHead) {
        Request* temp = requestQueueHead;
        requestQueueHead = requestQueueHead->next;
        delete temp;
    }
    while (undoStackTop) {
        UndoAction* temp = undoStackTop;
        undoStackTop = undoStackTop->next;
        delete temp;
    }
}

int main() {
    addCourse("CS101", "Introduction to CS", 3, 5);
    addCourse("CS102", "Data Structures", 4, 1);
    addStudent(2023001, "Ali");
    addStudent(2023002, "Sara");

    int choice;
    do {
        cout << "\n========================================\n";
        cout << "      University Management System      \n";
        cout << "========================================\n";
        cout << "1. Add Student\n";
        cout << "2. Add Course\n";
        cout << "3. Request Enrollment (Enqueue)\n";
        cout << "4. Process Next Request (Dequeue)\n";
        cout << "5. Drop Course\n";
        cout << "6. Enter/Update Grade\n";
        cout << "7. Print Transcript & GPA (Recursive)\n";
        cout << "8. Undo Last Action (Stack)\n";
        cout << "9. Display Courses (Doubly List)\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        int sid, cred, cap;
        string name, cid;
        double grd;

        switch(choice) {
            case 1:
                cout << "Enter ID: "; cin >> sid;
                cout << "Enter Name: "; cin.ignore(); getline(cin, name);
                addStudent(sid, name);
                break;
            case 2:
                cout << "Enter Course ID: "; cin >> cid;
                cout << "Enter Name: "; cin.ignore(); getline(cin, name);
                cout << "Enter Credits: "; cin >> cred;
                cout << "Enter Capacity: "; cin >> cap;
                addCourse(cid, name, cred, cap);
                break;
            case 3:
                cout << "Student ID: "; cin >> sid;
                cout << "Course ID: "; cin >> cid;
                enqueueRequest(sid, cid);
                break;
            case 4:
                processNextRequest();
                break;
            case 5:
                cout << "Student ID: "; cin >> sid;
                cout << "Course ID: "; cin >> cid;
                dropCourse(sid, cid);
                break;
            case 6:
                cout << "Student ID: "; cin >> sid;
                cout << "Course ID: "; cin >> cid;
                cout << "New Grade: "; cin >> grd;
                enterGrade(sid, cid, grd);
                break;
            case 7:
                cout << "Student ID: "; cin >> sid;
                printStudentDetails(sid);
                break;
            case 8:
                undoLastAction();
                break;
            case 9:
                displayCoursesForward();
                displayCoursesBackward();
                break;
            case 0: 
                cout << "Exiting system...\n"; 
                cleanUp();
                break;
            default: 
                cout << "Invalid choice, please try again.\n";
        }
    } while (choice != 0);

    return 0;
}
