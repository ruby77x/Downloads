This C++ code implements a comprehensive University Management System that handles student enrollment, course management, grade tracking, waitlists, and undo functionality using various data structures.

Data Structures Used
1. Student Structure
Stores student information (ID, name)

Contains a linked list of Enrollment nodes for courses the student is enrolled in

Linked to other students in a singly linked list

2. Enrollment Structure
Tracks course enrollment for each student

Contains course ID, grade, and pointer to next enrollment

Forms a linked list per student

3. Course Structure (Doubly Linked List)
Stores course details (ID, name, credits, capacity)

Tracks current enrollment count

Contains a circular linked list waitlist for full courses

Uses next/prev pointers for bidirectional traversal

4. WaitlistNode Structure (Circular Linked List)
Represents students waiting for a full course

Circular linking allows continuous traversal

5. Request Structure (Queue)
Represents enrollment requests waiting to be processed

FIFO (First-In-First-Out) structure

6. UndoAction Structure (Stack)
Tracks actions for undo functionality

LIFO (Last-In-First-Out) structure

Stores action type, student ID, course ID, and previous grade

Key Features
1. Student Management
Add new students with unique IDs

Store student information in a linked list

2. Course Management
Add courses with specified capacity and credits

Courses stored in doubly linked list for bidirectional traversal

Display courses in forward and backward order

3. Enrollment System
Request Queue: Enrollment requests are queued and processed in order

Capacity Check: Automatically checks if course has available seats

Waitlist Management: Full courses use circular linked list waitlists

Duplicate Prevention: Prevents multiple enrollments in same course

4. Grade Management
Enter and update grades for enrolled students

Grade value -1.0 represents "no grade yet"

5. Transcript & GPA Calculation
Recursive Display: Prints transcript in reverse order using recursion

GPA Calculation: Computes weighted GPA using recursive traversal

Formula: GPA = Σ(grade × credits) / Σ(credits)

6. Undo System
Tracks ENROLL, DROP, and GRADE actions

Allows reverting to previous state

Maintains grade history for accurate undo

7. Memory Management
Proper cleanup of all dynamically allocated memory

Prevents memory leaks

Main Functions Explained
1. Core Operations
addStudent(): Adds new student to the system

addCourse(): Creates new course with specified parameters

enqueueRequest(): Adds enrollment request to processing queue

processNextRequest(): Processes the oldest request in queue

2. Enrollment Functions
enrollInternal(): Core enrollment logic with waitlist handling

dropCourse(): Removes student from course, updates enrollment count

addToWaitlist(): Adds student to circular waitlist for full course

3. Grade & Transcript Functions
enterGrade(): Updates student grade for specific course

printStudentDetails(): Displays transcript and calculates GPA

printTranscriptRecursive(): Recursive function to print enrollments

calculateGPARecursive(): Recursive GPA calculation

4. Utility Functions
findStudent() / findCourse(): Search functions using linear search

displayCoursesForward() / Backward(): Bidirectional course display

undoLastAction(): Reverts the most recent action

cleanUp(): Memory deallocation for all structures

User Interface
The main menu provides options for:

Adding students and courses

Submitting enrollment requests

Processing requests

Dropping courses

Managing grades

Viewing transcripts

Undoing actions

Displaying courses

Example Usage Flow
Admin adds courses (CS101, CS102)

Students are added to the system (Ali, Sara)

Students request enrollment via queue

System processes requests in order

If course is full, student goes to waitlist

Grades are entered for enrolled students

Transcripts show courses and calculated GPA

Mistakes can be undone using stack-based undo system

Technical Highlights
Multiple Data Structures: Linked lists, queues, stacks, circular lists

Recursive Algorithms: For GPA calculation and transcript display

Bidirectional Navigation: Course list can be traversed both ways

Circular Waitlist: Efficient waitlist management

Undo/Redo Pattern: Action tracking for system reliability

Memory Safety: Proper allocation and deallocation

Code Structure
Global pointers maintain the head/top of each data structure

Modular functions for each operation

Input validation and error handling

Clean separation of concerns between different system components

This system demonstrates practical application of data structures in managing complex real-world relationships between students, courses, and academic records.

