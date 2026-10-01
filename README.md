# Flight Reservation System (C)

Console application written in C to manage flights and passenger
reservations, with an automatic waitlist when a flight is full.

## Features
- Add flights (number, capacity, departure, destination, date)
- Book a specific seat on a flight
- Automatic waitlist when the flight is full
- Cancel a reservation, with automatic promotion of the first
  waitlisted passenger to the freed seat
- List flights with booked seats and waitlist size
- View confirmed passengers and waitlist for every flight

## Technical highlights
- Singly linked lists for flights, reservations and waitlists
- Dynamic memory management (`malloc`, `calloc`, `free`)
- Safe string handling with `strncpy`
- Seat availability tracked with a dynamically allocated array

## Build and run
    gcc -Wall -Wextra -o flight_system main.c
    ./flight_system

## Usage
    1. Add Flight
    2. Add Reservation
    3. Cancel Reservation
    4. View Available Flights
    5. View Reservation History
    6. Exit

## Possible improvements
- Save and load data from files
- Cancel waitlist entries
- Robust input validation
- Split the code into modules (`flight.c`, `flight.h`)

