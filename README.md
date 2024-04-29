# OperatingSystems_ProjectTwo
## Threads and Semaphores
### Project #2

### I. Project Organization

This project studies the coordination of multiple threads using semaphores. Each aspect of the project is detailed below:

- Design 40 points
- Code 25 points
- Output 25 points
- Summary 10 points

#### Design

The design should include a list of every semaphore, its purpose, and initial value, along with pseudocode for each function, similar to the barbershop problem in the textbook.

#### Code

Your code should be well-formatted with plenty of comments, adhering to good naming standards and structure. It should correctly implement the design, matching the provided pseudocode.

#### Output

Output will be graded by running your program.

#### Summary

The summary section should discuss your simulation, any difficulties encountered, lessons learned, and results. It should be at least one page in length.

### II. Project Description

**Language/Platform**

This project must target a Unix platform and execute properly on our cs1 or csgrads1 Linux server. The project must be written in C, C++, or Java. If using C or C++, you must use POSIX pthreads and semaphores (no mutexes, locks, etc.). If using Java, you must use Java Threads and Java Semaphores (java.util.concurrent.Semaphore). You should not use the “synchronized” keyword in Java. You should not use any Java classes that have built-in mutual exclusion. Any mechanisms for thread coordination other than the semaphore are not allowed.

**Doctor’s Office Simulation**

This project simulates a visit to the doctor’s office, similar to the “barbershop” example in the textbook.

**Overview**

The clinic to be simulated has doctors, each with their own nurse. Each doctor has an office in which to visit patients. Patients will enter the clinic to see a doctor, randomly assigned. Patients wait in the waiting room until registered by the receptionist, then sit until called by the nurse. The nurse directs the patient to the doctor's office, and after the visit, the patient leaves.

**Threads**

- Receptionist – one thread
- Doctor – one thread each, maximum of 3 doctors
- Nurse – one per doctor thread, identifier of doctor and corresponding nurse should match
- Patient – one thread each, up to 15 patients

**Inputs**

The program should receive the number of doctors and patients as command-line inputs.

**Output**

- Each step of each task of each thread should be printed to the screen with identifying numbers.
- Begin by printing the number of patients, nurses, and doctors in this run.
- Sample output is provided in the project description.

### III. Project Guidelines

**Submitting**

Your final project should work correctly on cs1 or csgrads1. Submit your project on eLearning, including:

1. `design.xxx` where xxx is doc, docx, or pdf.
2. `summary.xxx` where xxx is doc, docx, or pdf.
3. `project2.c`, `project2.cpp`, or `Project2.java` along with any other source files.

**Partial or Missing Submissions**

It is your responsibility to upload all of the correct files on time. Once the deadline passes, changes to the submission are not accepted without a late penalty.

**Academic Honesty**

This is an individual project. All work must be your own. Comparison software may be used to compare the work of all students. Similar work will be reported to the Office of Community Standards and Conduct for investigation.

**Resources**

The web has many articles on threads and there are books available on threads. The course website also contains example source code.

**Grading**

The written portions will be graded subjectively based on completeness and quality. The code will be graded based on points allocated for each key part of the processing as determined by the instructor.

The output will be graded based on expected results for the input programs. A script will be used to automatically compile your program, execute it, and compare the output to the expected output. For this reason, it is very important that your program compiles and executes successfully on the cs1 server (or csgrads1) as shown below:

- Java:
  - compile: `javac Project2.java`
  - run: `java Project2`

- C/C++:
  - compile: `gcc project2.c -o project2` or `g++ project2.cpp -o project2`
  - run: `project2`
