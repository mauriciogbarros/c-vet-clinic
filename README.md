# Veterinary Clinic Application project

Written in C.
- Patient and Appointment data are loaded when the app begins.
- Max pets: 20
- Max appointments: 50

## Main module: `main.c`
- Declares and populates main structs:
    - pets: array of Patient with the patient data.
    - appoints: array of appointments with the appointment data.
    - data: data structure that contains pets, MAX_PETS, appoints, and MAX_APPPOINTMENTS.
- Calls menuMain that controls the execution of the application.

## Clinic module: `clinic.c`
- Display functions
- Menu & Item selection functions
- User input functions
- File functions
- Utility functions

## Core Module: `core.c`
- User interface functions
- User input functions