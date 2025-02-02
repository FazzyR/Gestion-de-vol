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