# University Management System (C++)

This C++ project implements a comprehensive **University Management System** designed to handle student enrollment, course management, grade tracking, waitlists, and undo functionality. It demonstrates the practical application of various fundamental data structures in a real-world scenario.

## 🏗️ Data Structures Used

The system leverages multiple data structures to manage relationships efficiently:

*   **Student Structure (Linked List):**
    *   Stores student info (ID, Name).
    *   Maintains a linked list of `Enrollment` nodes.
    *   Linked to other students via a singly linked list.
*   **Enrollment Structure (Linked List per Student):**
    *   Tracks course enrollments, grades, and course IDs.
*   **Course Structure (Doubly Linked List):**
    *   Stores details (ID, Name, Credits, Capacity).
    *   Uses `next`/`prev` pointers for bidirectional traversal.
    *   Contains a **Circular Linked List** for the waitlist.
*   **WaitlistNode (Circular Linked List):**
    *   Manages students waiting for full courses continuously.
*   **Request Structure (Queue - FIFO):**
    *   Handles enrollment requests in the order they are received.
*   **UndoAction Structure (Stack - LIFO):**
    *   Tracks actions (`ENROLL`, `DROP`, `GRADE`) to enable undo functionality.

## 🚀 Key Features

### 1. Student Management
*   Add new students with unique IDs.
*   Dynamic storage using linked lists.

### 2. Course Management
*   Create courses with specific capacity and credits.
*   Bidirectional display (Forward and Backward) using doubly linked lists.

### 3. Enrollment System
*   **Request Queue:** Process enrollment requests sequentially.
*   **Capacity Check:** Automatic validation of available seats.
*   **Waitlist:** Auto-adds students to a circular waitlist when courses are full.
*   **Duplicate Prevention:** Ensures unique enrollments.

### 4. Grade Management
*   Enter and update grades.
*   Default grade `-1.0` indicates "no grade yet".

### 5. Transcript & GPA
*   **Recursive Display:** Prints transcripts in reverse order.
*   **Weighted GPA:** Calculated recursively using the formula:
    $$ \text{GPA} = \frac{\sum (\text{grade} \times \text{credits})}{\sum \text{credits}} $$

### 6. Undo System
*   Revert `ENROLL`, `DROP`, and `GRADE` actions.
*   Maintains history to restore previous states accurately.

### 7. Memory Management
*   Full cleanup of dynamically allocated memory to prevent leaks.

---

## 🛠️ Main Functions

### Core Operations
*   `addStudent()`: Register a new student.
*   `addCourse()`: Create a new course.
*   `enqueueRequest()`: Queue an enrollment request.
*   `processNextRequest()`: Execute the oldest request in the queue.

### Enrollment Logic
*   `enrollInternal()`: Handles logic for open seats vs. waitlists.
*   `dropCourse()`: Removes a student and updates counts.
*   `addToWaitlist()`: Manages the circular waitlist.

### Grade & Reporting
*   `enterGrade()`: Assigns grades.
*   `printStudentDetails()`: Shows transcript and GPA.
*   `calculateGPARecursive()`: Computes GPA efficiently.

### Utilities
*   `findStudent()` / `findCourse()`: Linear search helpers.
*   `undoLastAction()`: Stack-based state reversion.
*   `cleanUp()`: Frees all memory.

---

## 💻 User Interface

The main menu provides an interactive console interface for:
1.  Adding Students/Courses
2.  Submitting & Processing Enrollment Requests
3.  Dropping Courses
4.  Managing Grades
5.  Viewing Transcripts
6.  Undoing Actions
7.  Displaying Course Lists

## 📝 Example Usage Flow

1.  **Setup:** Admin adds courses (e.g., *CS101*, *CS102*).
2.  **Registration:** Students (e.g., *Ali*, *Sara*) are added.
3.  **Request:** Students submit enrollment requests which go into the **Queue**.
4.  **Processing:** System processes requests. If *CS101* is full, the student is moved to the **Circular Waitlist**.
5.  **Grading:** Instructors enter grades.
6.  **Review:** Transcripts display courses and calculated GPA.
7.  **Correction:** Accidental drops or wrong grades can be reverted using **Undo**.

## 🌟 Technical Highlights

*   **Complex Data Structures:** Integration of Linked Lists, Doubly Linked Lists, Circular Lists, Stacks, and Queues.
*   **Recursion:** Used for elegant GPA calculation and transcript printing.
*   **Memory Safety:** Robust allocation and deallocation practices.
*   **Undo/Redo Pattern:** Implements a classic stack-based undo mechanism.


