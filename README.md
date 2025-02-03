# Gestion de Vol - Flight Reservation System

This project implements a flight reservation system in C. It allows users to manage flight reservations, cancellations, and view available flights and reservation histories. The system also supports a waitlist for flights that are fully booked.

## Features

- Add new flights with flight number and capacity.
- Reserve seats on flights (confirmed or waitlisted).
- Cancel reservations and promote waitlisted passengers if a seat becomes available.
- View available flights and the number of seats booked.
- View the history of reservations and waitlist for each flight.

## How to Use

1. **Add a Flight**  
   You can add a new flight by providing a flight number and capacity.

2. **Add a Reservation**  
   You can make a reservation by entering the flight number and passenger's name. If the flight is full, the passenger is added to the waitlist.

3. **Cancel a Reservation**  
   You can cancel a reservation by providing the flight number and passenger's name. If a seat becomes available, the first passenger on the waitlist is moved to the confirmed list.

4. **View Available Flights**  
   View the current flights along with the number of booked seats and available capacity.

5. **View Reservation History**  
   View all confirmed reservations and waitlisted passengers for each flight.

## Compilation and Execution

### Prerequisites
- A C compiler (e.g., GCC)

### Steps

1. Clone the repository or download the source code.
2. Compile the code:
   ```bash
   gcc -o gestion_de_vol gestion_de_vol.c
