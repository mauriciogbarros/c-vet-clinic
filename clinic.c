/*
Clinic module
- Display functions
- Menu & Item selection functions
- User input functions
- File functions
- Utility functions
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>

#include "core.h"
#include "clinic.h"


//////////////////////////////////////
// DISPLAY FUNCTIONS
//////////////////////////////////////

// Display's the patient table header (table format)
void displayPatientTableHeader(void)
{
    printf("Pat.# Name            Phone#\n"
           "----- --------------- --------------------\n");
}

// Displays a single patient record in FMT_FORM | FMT_TABLE format
void displayPatientData(const struct Patient* patient, int fmt)
{
    if (fmt == FMT_FORM)
    {
        printf("Name  : %s\n"
               "Number: %05d\n"
               "Phone : ", patient->name, patient->patientNumber);
        displayFormattedPhone(patient->phone.number);
        printf(" (%s)\n", patient->phone.description);
    }
    else
    {
        printf("%05d %-15s ", patient->patientNumber,
               patient->name);
        displayFormattedPhone(patient->phone.number);
        printf(" (%s)\n", patient->phone.description);
    }
}

// Display's appointment schedule headers (date-specific or all records)
void displayScheduleTableHeader(const struct Date* date, int isAllRecords)
{
    printf("Clinic Appointments for the Date: ");

    if (isAllRecords)
    {
        printf("<ALL>\n\n");
        printf("Date       Time  Pat.# Name            Phone#\n"
               "---------- ----- ----- --------------- --------------------\n");
    }
    else
    {
        printf("%04d-%02d-%02d\n\n", date->year, date->month, date->day);
        printf("Time  Pat.# Name            Phone#\n"
               "----- ----- --------------- --------------------\n");
    }
}

// Display a single appointment record with patient info. in tabular format
void displayScheduleData(const struct Patient* patient,
                         const struct Appointment* appoint,
                         int includeDateField)
{
    if (includeDateField)
    {
        printf("%04d-%02d-%02d ", appoint->date.year, appoint->date.month,
               appoint->date.day);
    }
    printf("%02d:%02d %05d %-15s ", appoint->time.hour, appoint->time.min,
           patient->patientNumber, patient->name);

    displayFormattedPhone(patient->phone.number);

    printf(" (%s)\n", patient->phone.description);
}


//////////////////////////////////////
// MENU & ITEM SELECTION FUNCTIONS
//////////////////////////////////////

// main menu
void menuMain(struct ClinicData* data)
{
    int selection;

    do {
        printf("Veterinary Clinic System\n"
               "=========================\n"
               "1) PATIENT     Management\n"
               "2) APPOINTMENT Management\n"
               "-------------------------\n"
               "0) Exit System\n"
               "-------------------------\n"
               "Selection: ");
        selection = inputIntRange(0, 2);
        putchar('\n');
        switch (selection)
        {
        case 0:
            printf("Are you sure you want to exit? (y|n): ");
            selection = !(inputCharOption("yn") == 'y');
            putchar('\n');
            if (!selection)
            {
                printf("Exiting system... Goodbye.\n\n");
            }
            break;
        case 1:
            menuPatient(data->patients, data->maxPatient);
            break;
        case 2:
            menuAppointment(data);
            break;
        }
    } while (selection);
}

// Menu: Patient Management
void menuPatient(struct Patient patient[], int max)
{
    int selection;

    do {
        printf("Patient Management\n"
               "=========================\n"
               "1) VIEW   Patient Data\n"
               "2) SEARCH Patients\n"
               "3) ADD    Patient\n"
               "4) EDIT   Patient\n"
               "5) REMOVE Patient\n"
               "-------------------------\n"
               "0) Previous menu\n"
               "-------------------------\n"
               "Selection: ");
        selection = inputIntRange(0, 5);
        putchar('\n');
        switch (selection)
        {
        case 1:
            displayAllPatients(patient, max, FMT_TABLE);
            suspend();
            break;
        case 2:
            searchPatientData(patient, max);
            break;
        case 3:
            addPatient(patient, max);
            suspend();
            break;
        case 4:
            editPatient(patient, max);
            break;
        case 5:
            removePatient(patient, max);
            suspend();
            break;
        }
    } while (selection);
}

// Menu: Patient edit
void menuPatientEdit(struct Patient* patient)
{
    int selection;

    do {
        printf("Edit Patient (%05d)\n"
               "=========================\n"
               "1) NAME : %s\n"
               "2) PHONE: ", patient->patientNumber, patient->name);
        
        displayFormattedPhone(patient->phone.number);
        
        printf("\n"
               "-------------------------\n"
               "0) Previous menu\n"
               "-------------------------\n"
               "Selection: ");
        selection = inputIntRange(0, 2);
        putchar('\n');

        if (selection == 1)
        {
            printf("Name  : ");
            inputCString(patient->name, 1, NAME_LEN);
            putchar('\n');
            printf("Patient record updated!\n\n");
        }
        else if (selection == 2)
        {
            inputPhoneData(&patient->phone);
            printf("Patient record updated!\n\n");
        }

    } while (selection);
}

// Menu: Appointment Management
void menuAppointment(struct ClinicData* data)
{
    int selection;

    do {
        printf("Appointment Management\n"
               "==============================\n"
               "1) VIEW   ALL Appointments\n"
               "2) VIEW   Appointments by DATE\n"
               "3) ADD    Appointment\n"
               "4) REMOVE Appointment\n"
               "------------------------------\n"
               "0) Previous menu\n"
               "------------------------------\n"
               "Selection: ");
        selection = inputIntRange(0, 4);
        putchar('\n');
        switch (selection)
        {
        case 1:
            viewAllAppointments(data);
            suspend();
            break;
        case 2:
            viewAppointmentSchedule(data);
            suspend();
            break;
        case 3:
            addAppointment(data->appointments, data->maxAppointments,
                           data->patients, data->maxPatient);
            suspend();
            break;
        case 4:
            removeAppointment(data->appointments, data->maxAppointments,
                              data->patients, data->maxPatient);
            suspend();
            break;
        }
    } while (selection);
}

// Display's all patient data in the FMT_FORM | FMT_TABLE format
void displayAllPatients(const struct Patient patient[], int max, int fmt)
{
    int i = 0, noRecords = 0;

    displayPatientTableHeader();
    for (i = 0; i < max; i++)
        if (strcmp(patient[i].name, ""))
            displayPatientData(&patient[i], fmt);
        else
            noRecords++;

    if (noRecords == max)
        printf("*** No records found ***\n");

    printf("\n");
}

// Search for a patient record based on patient number or phone number
void searchPatientData(const struct Patient patient[], int max)
{
    int selection = 0;
    do
    {
        printf("Search Options\n");
        printf("==========================\n");
        printf("1) By patient number\n");
        printf("2) By phone number\n");
        printf("..........................\n");
        printf("0) Previous menu\n");
        printf("..........................\n");
        printf("Selection: ");
        selection = inputIntRange(0, 2);
        printf("\n");

        switch (selection)
        {
            case 1:
                searchPatientByPatientNumber(patient, max);
                break;

            case 2:
                searchPatientByPhoneNumber(patient, max);
                break;
        }
    } while (selection);
}

// Add a new patient record to the patient array
void addPatient(struct Patient patient[], int max)
{
    int i = 0, freeRecord = 0;

    while (i < max)
    {
        if (patient[i].patientNumber == 0)
        {
            freeRecord = i;
            i = max;
        }
        else
            i++;
    }

    if (freeRecord == 0)
        printf("ERROR: Patient listing is FULL!\n");
    else
    {
        patient[freeRecord].patientNumber = nextPatientNumber(patient, max);
        inputPatient(&patient[freeRecord]);
        printf("*** New patient record added ***\n");
    }

    printf("\n");
}

// Edit a patient record from the patient array
void editPatient(struct Patient patient[], int max)
{
    int number = 0, index = 0;

    printf("Enter the patient number: ");
    number = inputIntPositive();
    printf("\n");

    index = findPatientIndexByPatientNum(number, patient, max);

    index != -1 ? menuPatientEdit(&patient[index]) : printf("ERROR: Patient record not found!\n");
}

// Remove a patient record from the patient array
void removePatient(struct Patient patient[], int max)
{
    int number = 0, index = 0;
    char removeRecordInput = '\0';

    printf("Enter the patient number: ");
    number = inputIntPositive();
    printf("\n");

    index = findPatientIndexByPatientNum(number, patient, max);

    if (index != -1)
    {
        displayPatientData(&patient[index], FMT_FORM);
        printf("\n");
        printf("Are you sure you want to remove this patient record? (y/n): ");
        removeRecordInput = inputCharOption("yn");

        switch (removeRecordInput)
        {
        case 'y':
            patient[index].patientNumber = 0;
            strcpy(patient[index].name, "");
            strcpy(patient[index].phone.description, "");
            strcpy(patient[index].phone.number, "");
            printf("Patient record has been removed!\n");
            break;

        case 'n':
            printf("Operation aborted.\n");
            break;
        }
    }
    else
        printf("ERROR: Patient record not found!\n");
    printf("\n");
}

// View ALL scheduled appointments
void viewAllAppointments(struct ClinicData* data)
{
    int i = 0;
    int minPatientIndex = 0;
    int totalAppointments = 0;
    struct Appointment sortedAppointments[MAX_APPOINTMENTS] = { {0} };

    while (data->appointments[i].patientNumber != 0)
    {
        sortedAppointments[i].patientNumber = data->appointments[i].patientNumber;
        sortedAppointments[i].date.year = data->appointments[i].date.year;
        sortedAppointments[i].date.month = data->appointments[i].date.month;
        sortedAppointments[i].date.day = data->appointments[i].date.day;
        sortedAppointments[i].time.hour = data->appointments[i].time.hour;
        sortedAppointments[i].time.min = data->appointments[i].time.min;
        i++;
    }
    totalAppointments = i;

    sortAppointments(sortedAppointments, totalAppointments);

    displayScheduleTableHeader(&data->appointments->date, 1);
    for (i = 0; i < totalAppointments; i++)
    {
        minPatientIndex = findPatientIndexByPatientNum(sortedAppointments[i].patientNumber, data->patients, data->maxPatient);
        displayScheduleData(&data->patients[minPatientIndex], &sortedAppointments[i], 1);
    }

    printf("\n");
}


// View appointment schedule for the user input date
void viewAppointmentSchedule(struct ClinicData* data)
{
    int i = 0, scheduleTotal = 0, index = 0;

    struct Date schedule = { 0 };
    struct Appointment scheduleAppoints[MAX_APPOINTMENTS] = { {0} };

    inputDate(&schedule);

    printf("\n");

    for (i = 0; i < data->maxAppointments; i++)
    {
        if (data->appointments[i].date.year == schedule.year &&
            data->appointments[i].date.month == schedule.month &&
            data->appointments[i].date.day == schedule.day)
        {
            scheduleAppoints[scheduleTotal].date.year = data->appointments[i].date.year;
            scheduleAppoints[scheduleTotal].date.month = data->appointments[i].date.month;
            scheduleAppoints[scheduleTotal].date.day = data->appointments[i].date.day;
            scheduleAppoints[scheduleTotal].patientNumber = data->appointments[i].patientNumber;
            scheduleAppoints[scheduleTotal].time.hour = data->appointments[i].time.hour;
            scheduleAppoints[scheduleTotal].time.min = data->appointments[i].time.min;

            scheduleTotal++;
        }
    }

    displayScheduleTableHeader(&schedule, 0);

    if (scheduleTotal > 0)
    {
        sortAppointments(scheduleAppoints, scheduleTotal);
        for (i = 0; i < scheduleTotal; i++)
        {
            index = findPatientIndexByPatientNum(scheduleAppoints[i].patientNumber, data->patients, data->maxPatient);

            displayScheduleData(&data->patients[index], &scheduleAppoints[i], 0);
        }

    }
    else
        printf("No appointments\n");

    printf("\n");

}

// Add an appointment record to the appointment array
void addAppointment(struct Appointment* appointments, int maxAppointments, struct Patient* patients, int maxPatients)
{
    int i = 0, available = 0, isPatient = 0, totalPatients = 0, totalAppointments = 0;

    struct Appointment timeslot = { 0 };

    while (patients[totalPatients].patientNumber != 0)
        totalPatients++;

    while (appointments[totalAppointments].patientNumber != 0)
        totalAppointments++;

    while (!isPatient)
    {
        printf("Patient Number: ");
        timeslot.patientNumber = inputIntPositive();

        i = 0;
        while (!isPatient && i < totalPatients)
        {
            if (patients[i].patientNumber == timeslot.patientNumber)
            {
                isPatient = 1;
            }
            else
                i++;
        }

        if (isPatient)
        {
            while (!available)
            {
                if (timeslot.date.year == 0)
                    inputDate(&timeslot.date);
                printf("Hour (0-23)  : ");
                timeslot.time.hour = inputIntRange(0, 23);
                printf("Minute (0-59): ");
                timeslot.time.min = inputIntRange(0, 59);

                if (timeslot.time.hour >= MIN_HOUR && (timeslot.time.hour <= MAX_HOUR && timeslot.time.min == 0) && timeslot.time.min % APPOINTMENT_INTERVAL == 0)
                {
                    i = 0;
                    while (!available && i < totalAppointments)
                    {
                        if (appointments[i].date.year == timeslot.date.year &&
                            appointments[i].date.month == timeslot.date.month &&
                            appointments[i].date.day == timeslot.date.day)
                        {
                            if (appointments[i].time.hour != timeslot.time.hour)
                                available = 1;
                            else if (appointments[i].time.min != timeslot.time.min)
                                available = 1;
                            else
                                i++;
                        }
                        else
                            i++;
                    }

                    if (available)
                    {
                        appointments[totalAppointments].date.year = timeslot.date.year;
                        appointments[totalAppointments].date.month = timeslot.date.month;
                        appointments[totalAppointments].date.day = timeslot.date.day;
                        appointments[totalAppointments].patientNumber = timeslot.patientNumber;
                        appointments[totalAppointments].time.hour = timeslot.time.hour;
                        appointments[totalAppointments].time.min = timeslot.time.min;

                        printf("\n*** Appointment scheduled! ***\n");
                    }
                    else
                    {
                        printf("\nERROR: Appointment timeslot is not available!\n\n");
                        timeslot.date.year = 0;
                    }
                }
                else
                    printf("ERROR: Time must be between %d:00 and %d:00 in %d minute intervals.\n\n", MIN_HOUR, MAX_HOUR, APPOINTMENT_INTERVAL);
            }
        }
        else
            printf("ERROR: Patient record not found!\n\n");
    }

    printf("\n");
}


// Remove an appointment record from the appointment array
void removeAppointment(struct Appointment* appointments, int maxAppointments, struct Patient* patients, int maxPatients)
{
    int i = 0, isPatient = 0, isAppointment = 0, totalPatients = 0, totalAppointments = 0, patientIndex = -1, appointmentIndex = -1;
    char remove = '\0';
    char removeOptions[2] = { 'y','n' };

    struct Appointment timeslot = { 0 };

    while (patients[totalPatients].patientNumber != 0)
        totalPatients++;

    while (appointments[totalAppointments].patientNumber != 0)
        totalAppointments++;

    printf("Patient Number: ");
    timeslot.patientNumber = inputIntPositive();

    i = 0;
    while (!isPatient && i < totalPatients)
    {
        if (patients[i].patientNumber == timeslot.patientNumber)
        {
            isPatient = 1;
        }
        else
            i++;
    }

    if (isPatient)
    {
        inputDate(&timeslot.date);
        printf("\n");

        i = 0;
        while (!isAppointment && i < totalAppointments)
        {
            if (appointments[i].date.year == timeslot.date.year &&
                appointments[i].date.month == timeslot.date.month &&
                appointments[i].date.day == timeslot.date.day &&
                appointments[i].patientNumber == timeslot.patientNumber)
            {
                patientIndex = findPatientIndexByPatientNum(timeslot.patientNumber, patients, totalPatients);
                appointmentIndex = i;
                isAppointment = 1;
            }
            else
                i++;
        }

        if (isAppointment)
        {
            displayPatientData(&patients[patientIndex], 1);
            printf("Are you sure you want to remove this appointment (y,n): ");
            remove = inputCharOption(removeOptions);

            if (remove == 'y')
            {
                for (i = appointmentIndex; i < totalAppointments - 1; i++)
                {
                    appointments[i].date.year = appointments[i + 1].date.year;
                    appointments[i].date.month = appointments[i + 1].date.month;
                    appointments[i].date.day = appointments[i + 1].date.day;
                    appointments[i].patientNumber = appointments[i + 1].patientNumber;
                    appointments[i].time.hour = appointments[i + 1].time.hour;
                    appointments[i].time.min = appointments[i + 1].time.min;
                }
                appointments[totalAppointments - 1].date.year = 0;
                appointments[totalAppointments - 1].date.month = 0;
                appointments[totalAppointments - 1].date.day = 0;
                appointments[totalAppointments - 1].patientNumber = 0;
                appointments[totalAppointments - 1].time.hour = 0;
                appointments[totalAppointments - 1].time.min = 0;

                printf("\nAppointment record has been removed!\n");
            }
            else
                printf("\nOperation cancelled.\n");
        }
    }
    else
        printf("ERROR: Patient record not found!\n");

    printf("\n");
}

//////////////////////////////////////
// UTILITY FUNCTIONS
//////////////////////////////////////

// Search and display patient record by patient number (form)
void searchPatientByPatientNumber(const struct Patient patient[], int max)
{
    int number = 0, index = 0;

    printf("Search by patient number: ");

    number = inputIntPositive();
    index = findPatientIndexByPatientNum(number, patient, max);
    printf("\n");

    if (index != -1)
        displayPatientData(&patient[index], FMT_FORM);
    else
        printf("*** No records found ***\n");

    printf("\n");
    suspend();
}

// Search and display patient records by phone number (tabular)
void searchPatientByPhoneNumber(const struct Patient patient[], int max)
{
    char phoneNumber[PHONE_LEN + 1] = { 0 };
    int i = 0, found = 0;

    printf("Search by phone number: ");
    inputCString(phoneNumber, PHONE_LEN, PHONE_LEN);
    printf("\n");

    displayPatientTableHeader();
    for (i = 0; i < max; i++)
    {
        if (!strcmp(patient[i].phone.number, phoneNumber))
        {
            found += 1;
            displayPatientData(&patient[i], FMT_TABLE);
        }
    }

    if (!found)
    {
        printf("\n");
        printf("*** No records found ***\n");
    }

    printf("\n");
    suspend();
}

// Get the next highest patient number
int nextPatientNumber(const struct Patient patient[], int max)
{
    int highestNumber = 0;
    int i = 0;

    highestNumber = patient[0].patientNumber;
    for (i = 1; i < max; i++)
    {
        if (highestNumber < patient[i].patientNumber)
            highestNumber = patient[i].patientNumber;
    }
    highestNumber += 1;

    return (highestNumber);
}

// Find the patient array index by patient number (returns -1 if not found)
int findPatientIndexByPatientNum(int patientNumber,
    const struct Patient patient[], int max)
{
    int i = 0, index = -1, found = 0;

    while (!found && i < max)
    {
        if (patient[i].patientNumber == patientNumber)
        {
            index = i;
            found = 1;
        }
        else
            i++;
    }

    return index;
}

//////////////////////////////////////
// USER INPUT FUNCTIONS
//////////////////////////////////////

// Get user input for a new patient record
void inputPatient(struct Patient* patient)
{
    printf("Patient Data Input\n");
    printf("------------------\n");
    printf("Number: %05d\n", patient->patientNumber);
    printf("Name  : ");
    inputCString(patient->name, 1, NAME_LEN);
    printf("\n");

    inputPhoneData(&patient->phone);
}

// Get user input for phone contact information
void inputPhoneData(struct Phone* phone)
{
    int selection = 0, minSelection = 1, maxSelection = 4;

    printf("Phone Information\n");
    printf("-----------------\n");
    printf("How will the patient like to be contacted?\n");
    printf("1. Cell\n");
    printf("2. Home\n");
    printf("3. Work\n");
    printf("4. TBD\n");
    printf("Selection: ");
    selection = inputIntRange(minSelection, maxSelection);

    switch (selection)
    {
    case 1:
        strcpy(phone->description, "CELL");
        break;
    case 2:
        strcpy(phone->description, "HOME");
        break;
    case 3:
        strcpy(phone->description, "WORK");
        break;
    case 4:
        strcpy(phone->description, "TBD");
        break;
    }

    printf("\n");

    if (selection != 4)
    {
        printf("Contact: %s\n", phone->description);
        printf("Number : ");
        inputPhoneNumber(phone->number);

        printf("\n");
    }
    else
    {
        strcpy(phone->number, "");
    }

}

//////////////////////////////////////
// FILE FUNCTIONS
//////////////////////////////////////

// Import patient data from file into a Patient array (returns # of records read)
int importPatients(const char* datafile, struct Patient patients[], int max)
{
    int i = 0, isEOF = 0;
    FILE* patientData = NULL;
    patientData = fopen(datafile, "r");

    if (patientData != NULL)
    {
        while (!isEOF && i < max)
        {
            fscanf(patientData, "%d|%[^|]|%[^|]|%[^\n]",
                &patients[i].patientNumber,
                patients[i].name,
                patients[i].phone.description,
                patients[i].phone.number);

            isEOF = feof(patientData);
            if (!isEOF)
                i++;
        }
        fclose(patientData);
    }

    return i;
}

// Import appointment data from file into an Appointment array (returns # of records read)
int importAppointments(const char* datafile, struct Appointment appoints[], int max)
{
    int i = 0, isEOF = 0;
    FILE* appointmentData = NULL;
    appointmentData = fopen(datafile, "r");

    if (appointmentData != NULL)
    {
        while (!isEOF && i < max)
        {
            fscanf(appointmentData, "%d,%d,%d,%d,%d,%d",
                &appoints[i].patientNumber,
                &appoints[i].date.year,
                &appoints[i].date.month,
                &appoints[i].date.day,
                &appoints[i].time.hour,
                &appoints[i].time.min);

            isEOF = feof(appointmentData);
            if (!isEOF)
                i++;
        }
        fclose(appointmentData);
    }

    return i;
}

//////////////////////////////////////
// UTILITY FUNCTIONS
//////////////////////////////////////

// Sort appointment info
void  sortAppointments(struct Appointment* appointments, int totalAppointments)
{
    int i = 0, j = 0, minAppointmentIndex = 0;
    struct Appointment tempAppointment = { 0 };

    for (i = 0; i < totalAppointments; i++)
    {
        minAppointmentIndex = i;

        for (j = i + 1; j < totalAppointments; j++)
        {
            if (appointments[j].date.year < appointments[minAppointmentIndex].date.year)

                minAppointmentIndex = j;

            else if (appointments[j].date.year == appointments[minAppointmentIndex].date.year)
            {
                if (appointments[j].date.month < appointments[minAppointmentIndex].date.month)
                {
                    minAppointmentIndex = j;
                }
                else if (appointments[j].date.month == appointments[minAppointmentIndex].date.month)
                {
                    if (appointments[j].date.day < appointments[minAppointmentIndex].date.day)
                    {
                        minAppointmentIndex = j;
                    }
                    else if (appointments[j].date.day == appointments[minAppointmentIndex].date.day)
                    {
                        if (appointments[j].time.hour < appointments[minAppointmentIndex].time.hour)
                        {
                            minAppointmentIndex = j;
                        }
                        else if (appointments[j].time.hour == appointments[minAppointmentIndex].time.hour)
                        {
                            if (appointments[j].time.min < appointments[minAppointmentIndex].time.min)
                            {
                                minAppointmentIndex = j;
                            }
                        }
                    }
                }
            }
        }

        if (minAppointmentIndex != i)
        {
            tempAppointment.patientNumber = appointments[i].patientNumber;
            tempAppointment.date.year = appointments[i].date.year;
            tempAppointment.date.month = appointments[i].date.month;
            tempAppointment.date.day = appointments[i].date.day;
            tempAppointment.time.hour = appointments[i].time.hour;
            tempAppointment.time.min = appointments[i].time.min;

            appointments[i].patientNumber = appointments[minAppointmentIndex].patientNumber;
            appointments[i].date.year = appointments[minAppointmentIndex].date.year;
            appointments[i].date.month = appointments[minAppointmentIndex].date.month;
            appointments[i].date.day = appointments[minAppointmentIndex].date.day;
            appointments[i].time.hour = appointments[minAppointmentIndex].time.hour;
            appointments[i].time.min = appointments[minAppointmentIndex].time.min;

            appointments[minAppointmentIndex].patientNumber = tempAppointment.patientNumber;
            appointments[minAppointmentIndex].date.year = tempAppointment.date.year;
            appointments[minAppointmentIndex].date.month = tempAppointment.date.month;
            appointments[minAppointmentIndex].date.day = tempAppointment.date.day;
            appointments[minAppointmentIndex].time.hour = tempAppointment.time.hour;
            appointments[minAppointmentIndex].time.min = tempAppointment.time.min;
        }
    }
}

// Get user input for a date
void inputDate(struct Date* date)
{
    int isLeapYear = 0, maxDay = 0;

    printf("Year        : ");
    date->year = inputIntPositive();

    isLeapYear = (date->year % 4) == 0;

    printf("Month (1-12): ");
    date->month = inputIntRange(1, 12);

    switch (date->month)
    {
    case 1:
        maxDay = 31;
        break;

    case 2:
        maxDay = 28 + isLeapYear;
        break;

    case 3:
        maxDay = 31;
        break;

    case 4:
        maxDay = 30;
        break;

    case 5:
        maxDay = 31;
        break;

    case 6:
        maxDay = 30;
        break;

    case 7:
        maxDay = 31;
        break;

    case 8:
        maxDay = 31;
        break;

    case 9:
        maxDay = 30;
        break;

    case 10:
        maxDay = 31;
        break;

    case 11:
        maxDay = 30;
        break;

    case 12:
        maxDay = 31;
        break;
    }

    printf("Day (1-%d)  : ", maxDay);
    date->day = inputIntRange(1, maxDay);
}

// Get user input for a phone number and validate
void inputPhoneNumber(char* phoneString)
{
    int length = 0;
    int correct = 0, i = 0, minAscii = 48, maxAscii = 57;
    char inputString[100] = { "\0" };

    while (correct == 0 || length != PHONE_LEN)
    {
        scanf("%[^\n]", inputString);
        clearInputBuffer();

        length = strlen(inputString);

        if (length == PHONE_LEN)
        {
            i = 0;
            while (i < length)
            {
                if (inputString[i] >= minAscii || inputString[i] <= maxAscii)
                    correct = 1;
                else
                    correct = 0;
                i++;
            }

            if (correct)
            {
                strcpy(phoneString, inputString);
            }
            else
                printf("Invalid %d-digit number! Number: ", PHONE_LEN);
        }
        else
            printf("Invalid %d-digit number! Number: ", PHONE_LEN);
    }
}