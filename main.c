/*
Veterinary Clinic Application
Main module
- Declares and populates main structs:
    - pets: array of Patient with the patient data.
    - appoints: array of appointments with the appointment data.
    - data: data structure that contains pets, MAX_PETS, appoints, and MAX_APPPOINTMENTS.
- Calls menuMain that controls the execution of the application.
*/

#include <stdio.h>

#include "clinic.h"

// Constants
#define MAX_PETS 20
#define MAX_APPOINTMENTS 50

int main(void)
{
    struct Patient pets[MAX_PETS] = { {0} };
    struct Appointment appoints[MAX_APPOINTMENTS] = { {0} };
    struct ClinicData data = { pets, MAX_PETS, appoints, MAX_APPOINTMENTS };

    int patientCount = importPatients("patientData.txt", pets, MAX_PETS);
    int appointmentCount = importAppointments("appointmentData.txt", appoints, MAX_APPOINTMENTS);

    printf("Imported %d patient records...\n", patientCount);
    printf("Imported %d appointment records...\n\n", appointmentCount);

    menuMain(&data);
    
    return 0;
}