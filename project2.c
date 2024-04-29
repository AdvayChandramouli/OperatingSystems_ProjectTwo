/*
 * Advay Chandramouli
 * CS4348.501 - Prof. Greg Ozbirn
 * Project Two: Threads and Semaphores
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define MAXCAPACITY 3

// Semaphore Declaration
sem_t receptionist; // Semaphore for the receptionist
sem_t leave_reception; // Semaphore for patients leaving reception
sem_t registration; // Semaphore for patient registration
sem_t patient_entry; // Semaphore for patient entry
sem_t room; // Semaphore for room assignment

sem_t patient_waiting[MAXCAPACITY]; // Semaphores for patients waiting
sem_t patient_seated[MAXCAPACITY]; // Semaphores for patients seated in clinic room
sem_t patient_exit[MAXCAPACITY]; // Semaphores for patients leaving clinic
sem_t patient_symptoms[MAXCAPACITY]; // Semaphores for patients communicating symptoms to doctor
sem_t room_nurse[MAXCAPACITY]; // Semaphores for nurses in different rooms
sem_t nurse_informed[MAXCAPACITY]; // Semaphores for nurses informed of patient arrival
sem_t nurse_prompted[MAXCAPACITY]; // Semaphores for nurses prompting patients
sem_t doctor_avail[MAXCAPACITY]; // Semaphores for doctors available
sem_t doctor_informed[MAXCAPACITY]; // Semaphores for doctors being informed of patient waiting
sem_t doctor_prescribes[MAXCAPACITY]; // Semaphores for doctors prescribing treatment

// System Resources
int reg_id; // ID for patient registration
int room_id; // ID for room assignment
int num_doctors, num_nurses, num_patients; // Number of doctors, nurses, and patients
int waiting_queue[3]; // Queue of patients waiting in different rooms
int currently_attending[3]; // IDs of patients currently attended by doctors in different rooms
int doctors[3]; // IDs of doctors
int nurses[3]; // IDs of nurses

struct _patient
{
    int id; // ID of the patient
    int room_num; // Room number assigned to the patient
};

struct _patient patient[15]; // Array of patients

// Thread function for doctors
void *th_doctor(void *arg)
{
    int docID = *(int*)arg;
    while(1)
    {
        sem_post(&doctor_avail[docID]); // Doctor becomes available
        sem_wait(&doctor_informed[docID]); // Wait for doctor to be informed
        sem_post(&patient_seated[docID]); // Signal patient seated
        sem_wait(&patient_symptoms[docID]); // Wait for patient to indicate symptoms
        printf("Doctor %d listens to symptoms from patient\n", currently_attending[docID]); // Doctor listens to patient's symptoms
        sem_post(&doctor_prescribes[docID]); // Doctor prescribes treatment
    }
    return NULL;
}

// Thread function for nurses
void *th_nurse(void *arg)
{
    int id = *(int*)arg;
    while(1)
    {
        sem_post(&room_nurse[id]); // Nurse becomes available in the room
        sem_wait(&patient_waiting[id]); // Wait for patient to be waiting
        sem_wait(&nurse_informed[id]); // Wait for nurse to be informed
        sem_wait(&patient_exit[id]); // Wait for patient to exit
        printf("Nurse %d takes patient %d to doctor's office\n", id, waiting_queue[id]); // Nurse escorts patient to doctor's office
        sem_post(&nurse_prompted[id]); // Nurse prompts
        sem_post(&doctor_informed[id]); // Inform doctor
    }
}

// Thread function for receptionist
void *th_receptionist(void *arg) {
    srand(time(NULL)); // Seed for random number generation

    while (1) {
        sem_post(&receptionist); // Signal receptionist
        sem_wait(&patient_entry); // Wait for patient entry

        sem_wait(&registration); // Wait for registration
        printf("Receptionist registers patient %d\n", reg_id); // Receptionist registers patient
        room_id = rand() % num_doctors; // Randomly assign room
        sem_post(&room); // Signal room assignment
        sem_post(&nurse_informed[room_id]); // Inform nurse
        sem_wait(&leave_reception); // Wait for patient to leave reception
    }

    return NULL;
}

// Thread function for patients
void *th_patient(void *arg)
{
    struct _patient p = *(struct _patient *)arg;

    sem_wait(&receptionist); // Wait for receptionist
    sem_post(&patient_entry); // Signal patient entry
    printf("Patient %d enters waiting room, waits for receptionist\n", p.id); // Patient enters waiting room

    reg_id = p.id; // Set registration ID
    sem_post(&registration); // Signal registration

    sem_wait(&room); // Wait for room assignment
    p.room_num = room_id; // Assign room number
    printf("Patient %d leaves receptionist and sits in waiting room\n", p.id); // Patient leaves receptionist
    sem_post(&leave_reception); // Signal leaving reception

    sem_post(&patient_waiting[p.room_num]); // Signal patient waiting
    sem_wait(&room_nurse[p.room_num]); // Wait for nurse in room
    waiting_queue[p.room_num] = p.id; // Add patient to waiting queue
    sem_wait(&nurse_prompted[p.room_num]); // Wait for nurse prompt

    printf("Patient %d enters doctor %d's office\n", p.id, p.room_num); // Patient enters doctor's office
    currently_attending[p.room_num] = p.id; // Set currently attending patient
    sem_post(&patient_seated[p.room_num]); // Signal patient seated
    sem_wait(&doctor_avail[p.room_num]); // Wait for doctor availability

    sem_post(&patient_symptoms[p.room_num]); // Signal patient symptoms
    sem_wait(&doctor_prescribes[p.room_num]); // Wait for doctor prescription
    printf("Patient %d receives advice from doctor %d\n", p.id, p.room_num); // Patient receives advice from doctor

    printf("Patient %d leaves\n", p.id); // Patient leaves

    sem_post(&patient_exit[p.room_num]); // Signal patient exit

    return NULL;
}


int main(int argc, char *argv[])
{
    //Input Validation
    //Check if less than 2 arguments provided
    if (argc < 2) {
        printf("Invalid number of arguments! ");
        exit(-1);
    }
    //Resource initialization
    sscanf(argv[1], "%d", &num_doctors);
    sscanf(argv[1], "%d", &num_nurses); //Same # of nurses as doctors on call
    sscanf(argv[2], "%d", &num_patients); //Patients = argv[1]

    //check for command line arguments out of bounds
    if(num_doctors > 3)
    {
        printf("Invalid number of doctors/nurses! ");
        exit(-1);
    }
    if(num_patients > 15)
    {
        printf("Invalid number of patients! ");
        exit(-1);
    }

    printf("Run with %d patients, %d nurses, %d doctors\n", num_patients, num_nurses, num_doctors);
    printf("\n");


    //Semaphore initialization
    sem_init(&receptionist, 0, 0); //Initial value for receptionist semaphore is 1
    sem_init(&patient_entry, 0, 0); //Initial value for patient entry is 1
    sem_init(&room, 0, 0);
    sem_init(&registration, 0, 0);
    sem_init(&leave_reception, 0, 0);

    for(int i = 0; i < num_doctors; i++)
    {
        sem_init(&patient_waiting[i], 0, 0);
        sem_init(&room_nurse[i], 0, 0);
        sem_init(&nurse_prompted[i], 0, 0);
        sem_init(&patient_seated[i], 0, 0);
        sem_init(&patient_symptoms[i], 0, 0);
        sem_init(&doctor_prescribes[i], 0, 0);
        sem_init(&doctor_avail[i], 0, 0);
        sem_init(&doctor_informed[i], 0, 0);
        sem_init(&nurse_informed[i], 0, 0);
        sem_init(&patient_exit[i], 0, 1);
    }


    pthread_t recID; //thread ID for receptionist
    pthread_t pid[num_patients]; //thread ID for patients
    pthread_t did[num_doctors]; //thread ID for doctor
    pthread_t nid[num_nurses]; //thread ID for nurses

    //Create receptionist thread
    pthread_create(&recID, 0, th_receptionist, 0);

    //Create doctor & nurse thread
    for(int i = 0 ; i < num_doctors; i++)
    {
        doctors[i] = i;
        nurses[i] = i;
        pthread_create(&did[i], 0, th_doctor, &doctors[i]);
        pthread_create(&nid[i], 0, th_nurse, &nurses[i]);
    }
    //Create patient thread
    for(int i = 0; i < num_patients; i++)
    {
        patient[i].id = i;
        patient[i].room_num = 0;
        pthread_create(&pid[i], 0, th_patient, &patient[i]);
    }
    for(int i = 0; i < num_patients; i++)
    {
        pthread_join(pid[i], 0);
    }

    printf("Simulation complete\n");

    return 0;
}