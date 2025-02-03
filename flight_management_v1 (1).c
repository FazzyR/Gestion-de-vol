#include<stdio.h>
#include<stdlib.h>
#include<string.h>

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
    newFlight->reservations = NULL; 
    newFlight->waitlist = NULL;     
    newFlight->next = head;
    return newFlight;  
}

void freeAll(Flight *head) {
    while (head) {
        Flight *temp = head;
        head = head->next;
      
        Passenger *p = temp->reservations;
        while (p) {
            Passenger *tempP = p;
            p = p->next;
            free(tempP);
        }
        p = temp->waitlist;
        while (p) {
            Passenger *tempP = p;
            p = p->next;
            free(tempP);
        }
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
    
        Passenger *newPassenger = (Passenger *)malloc(sizeof(Passenger));
        if (!newPassenger) {
            printf("Memory allocation failed for new passenger.\n");
            return;
        }
        strncpy(newPassenger->name, name, MAX_NAME_LENGTH - 1);
        newPassenger->name[MAX_NAME_LENGTH - 1] = '\0';
        newPassenger->next = NULL;

        if (flight->waitlist == NULL) {
            flight->waitlist = newPassenger;
        } else {
            Passenger *current = flight->waitlist;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newPassenger;
        }
        printf("Flight %d is full. %s has been added to the waitlist.\n", flightNumber, name);
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
    printf("Passenger %s not found on flight %d.\n", name, flightNumber);
}
void searchAvailableFlights(Flight *head) {
    printf("Available flights:\n");
    while (head) {
        int waitlistCount = 0;
        Passenger *p = head->waitlist;
        while (p) {
            waitlistCount++;
            p = p->next;
        }
        printf("Flight %d: %d/%d seats booked, %d on waitlist.\n", 
               head->flightNumber, head->bookedSeats, head->capacity, waitlistCount);
        head = head->next;
    }
}
void viewReservationHistory(Flight *head) {
    while (head) {
        printf("Flight %d:\n", head->flightNumber);
        printf("  Confirmed Reservations: %d/%d\n", head->bookedSeats, head->capacity);
        int waitlistCount = 0;
        Passenger *p = head->waitlist;
        while (p) {
            waitlistCount++;
            p = p->next;
        }
        printf("  Waitlist: %d passengers\n", waitlistCount);
        head = head->next;
    }
}

int isValidName(char *name) {
    return (name && strlen(name) > 0 && strlen(name) < MAX_NAME_LENGTH);
}

int main() {
    Flight *head = NULL;
    int choice, flightNumber, capacity;
    char name[MAX_NAME_LENGTH];

    while (1) {
        printf("\nFlight Reservation System Menu:\n");
        printf("1. Add Flight\n");
        printf("2. Add Reservation\n");
        printf("3. Cancel Reservation\n");
        printf("4. View Available Flights\n");
        printf("5. View Reservation History\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  

        switch (choice) {
            case 1:
                // Adding a new flight
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
                printf("Enter flight number: ");
                scanf("%d", &flightNumber);
                getchar();  
                printf("Enter passenger name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = '\0'; 

                if (!isValidName(name)) {
                    printf("Invalid name. It must be non-empty and less than %d characters.\n", MAX_NAME_LENGTH);
                    break;
                }

                addReservation(head, name, flightNumber);
                break;

            case 3:
                printf("Enter flight number: ");
                scanf("%d", &flightNumber);
                getchar();  
                printf("Enter passenger name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = '\0'; 

                cancelReservation(head, name, flightNumber);
                break;

            case 4:
                searchAvailableFlights(head);
                break;

            case 5:
                viewReservationHistory(head);
                break;

            case 6:
                freeAll(head);
                printf("Exiting the program.\n");
                return 0;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

