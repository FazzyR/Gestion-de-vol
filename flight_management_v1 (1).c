#include<stdio.h>
#include<stdlib.h>

#define MAX_NAME_LENGTH 50

typedef struct Passenger {
    char name[MAX_NAME_LENGTH];
    struct Passenger *next;
} Passenger;

typedef struct Flight {
   int flightNumber;
   int capacity;
   int bookedSeats;
   Passenger *reservations;
   Passenger *waitlist;
   struct Flight *next;
} Flight; 



Flight *addFlight(Flight *head, int flightNumber, int capacity) {
    Flight *newFlight = (Flight *)malloc(sizeof(Flight));
    if (!newFlight) {
        printf("Memory allocation failed for new flight.\n");
        exit(1);  
    }

    newFlight->flightNumber = flightNumber;
    newFlight->capacity = capacity;
    newFlight->bookedSeats = 0;
    newFlight->next = head;
    return newFlight;  
}

void freeAll(Flight *head) {
    while (head) {
        Flight *temp = head;
        head = head->next;
        free(temp);
    }
}
Flight *findFlight(Flight *head, int flightNumber) {
    while (head) {
        if (head->flightNumber == flightNumber) {
            return head;
        }
        head = head->next;  
    }
    return NULL;  
}

void addReservation(Flight *head, char *name, int flightNumber) {
    Flight *flight = findFlight(head, flightNumber);
    if (!flight) {
        printf("Flight %d not found.\n", flightNumber);
        return;  
    }

    if (flight->bookedSeats >= flight->capacity) {
        printf("Flight %d is full. Cannot add %s.\n", flightNumber, name);
        return;
    }

    Passenger *newPassenger = (Passenger *)malloc(sizeof(Passenger));
    if (!newPassenger) {
        printf("Memory allocation failed for new passenger.\n");
        return;  
    }
    strncpy(newPassenger->name, name, MAX_NAME_LENGTH - 1);
    newPassenger->name[MAX_NAME_LENGTH - 1] = '\0';
    newPassenger->next = flight->reservations;
    flight->reservations = newPassenger;
    flight->bookedSeats++;
    printf("Reservation confirmed for %s on flight %d.\n", name, flightNumber);
}

void cancelReservation(Flight *head, char *name, int flightNumber) {
    Flight *flight = findFlight(head, flightNumber);
    if (!flight) {
        printf("Flight %d not found.\n", flightNumber);
        return;
    }

    Passenger *prev = NULL, *curr = flight->reservations;

    while (curr) {
        if (strcmp(curr->name, name) == 0) {
            if (prev) prev->next = curr->next;
            else flight->reservations = curr->next;
            
            free(curr);
            flight->bookedSeats--;
            printf("Reservation for %s on flight %d canceled.\n", name, flightNumber);
            
            if (flight->waitlist) {
                Passenger *promoted = flight->waitlist;
                flight->waitlist = promoted->next;
                promoted->next = flight->reservations;
                flight->reservations = promoted;
                flight->bookedSeats++;
                printf("%s has been moved from the waitlist to confirmed reservations.\n", promoted->name);
            }
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}
void searchAvailableFlights(Flight *head) {
    printf("Available flights:\n");
    while (head) {
        printf("Flight %d: %d/%d seats booked\n", head->flightNumber, head->bookedSeats, head->capacity);
        head = head->next;
    }
}
void viewReservationHistory(Flight *head) {
    while (head) {
        printf("Flight %d:\n", head->flightNumber);
        printf("  Confirmed Reservations: %d/%d\n", head->bookedSeats, head->capacity);
        head = head->next;
    }
}
int main() {
    Flight *head = NULL;
    int choice, flightNumber, capacity;

    while (1) {
        printf("\nFlight Management System Menu:\n");
        printf("1. Add Flight\n");
        printf("2. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter flight number: ");
                scanf("%d", &flightNumber);
                printf("Enter flight capacity: ");
                scanf("%d", &capacity);

                if (capacity <= 0) {
                    printf("Capacity must be greater than 0.\n");
                    break;
                }
                head = addFlight(head, flightNumber, capacity);
                printf("Flight %d added with capacity %d.\n", flightNumber, capacity);
                break;

            case 2:
                freeAll(head);
                printf("Exiting the program.\n");
                return 0;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}
