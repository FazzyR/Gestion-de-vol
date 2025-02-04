#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50
#define MAX_LOCATION_LENGTH 100
#define MAX_DATE_LENGTH 20

typedef struct Passenger {
    char name[MAX_NAME_LENGTH];
    int seatNumber;
    struct Passenger *next;
} Passenger;

typedef struct Flight {
    int flightNumber;
    int capacity;
    int bookedSeats;
    int *seats;
    char departure[MAX_LOCATION_LENGTH];
    char destination[MAX_LOCATION_LENGTH];
    char departureDate[MAX_DATE_LENGTH];
    Passenger *reservations;
    Passenger *waitlist;
    struct Flight *next;
} Flight;

Flight *addFlight(Flight *head, int flightNumber, int capacity, char *departure, char *destination, char *departureDate) {
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
    newFlight->seats = (int *)calloc(capacity, sizeof(int));
    if (!newFlight->seats) {
        printf("Memory allocation failed for seats.\n");
        free(newFlight);
        exit(1);
    }
    strncpy(newFlight->departure, departure, MAX_LOCATION_LENGTH - 1);
    newFlight->departure[MAX_LOCATION_LENGTH - 1] = '\0';
    strncpy(newFlight->destination, destination, MAX_LOCATION_LENGTH - 1);
    newFlight->destination[MAX_LOCATION_LENGTH - 1] = '\0';
    strncpy(newFlight->departureDate, departureDate, MAX_DATE_LENGTH - 1);
    newFlight->departureDate[MAX_DATE_LENGTH - 1] = '\0';

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
        free(temp->seats);
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

void addReservation(Flight *head, char *name, int flightNumber, int seatNumber) {
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
        newPassenger->seatNumber = -1;
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

    if (seatNumber < 1 || seatNumber > flight->capacity) {
        printf("Invalid seat number.\n");
        return;
    }
    if (flight->seats[seatNumber - 1]) {
        printf("Seat %d is already taken.\n", seatNumber);
        return;
    }

    Passenger *newPassenger = (Passenger *)malloc(sizeof(Passenger));
    if (!newPassenger) {
        printf("Memory allocation failed for new passenger.\n");
        return;
    }
    strncpy(newPassenger->name, name, MAX_NAME_LENGTH - 1);
    newPassenger->name[MAX_NAME_LENGTH - 1] = '\0';
    newPassenger->seatNumber = seatNumber;
    newPassenger->next = flight->reservations;
    flight->reservations = newPassenger;
    flight->bookedSeats++;
    flight->seats[seatNumber - 1] = 1;
    printf("Reservation confirmed for %s on flight %d, seat number %d\n", name, flightNumber, seatNumber);
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

            flight->seats[curr->seatNumber - 1] = 0;
            free(curr);
            flight->bookedSeats--;
            printf("Reservation for %s on flight %d canceled.\n", name, flightNumber);

            if (flight->waitlist) {
                Passenger *promoted = flight->waitlist;
                flight->waitlist = promoted->next;
                promoted->seatNumber = curr->seatNumber;
                promoted->next = flight->reservations;
                flight->reservations = promoted;
                flight->bookedSeats++;
                flight->seats[promoted->seatNumber - 1] = 1;
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
        printf("Flight %d: %d/%d seats booked, %d on waitlist. Departure: %s, Destination: %s, Date: %s\n", head->flightNumber, head->bookedSeats, head->capacity, waitlistCount, head->departure, head->destination, head->departureDate);
        head = head->next;
    }
}

void viewReservationHistory(Flight *head) {
    while (head) {
        printf("Flight %d (Departure: %s, Destination: %s, Date: %s):\n", head->flightNumber, head->departure, head->destination, head->departureDate);
        printf("  Confirmed Reservations: %d/%d\n", head->bookedSeats, head->capacity);

        int pnum = 0;
        Passenger *x = head->reservations;
        while (x) {
            pnum++;
            printf("Passenger Name: %s, Seat Number: %d, Reservation Number: %d\n", x->name, x->seatNumber, pnum);
            x = x->next;
        }

        int waitlistCount = 0;
        Passenger *p = head->waitlist;
        printf("  Waitlist passengers:\n");
        while (p) {
            waitlistCount++;
            printf("Passenger Name: %s, Waitlist Position: %d\n", p->name, waitlistCount);
            p = p->next;
        }
        head = head->next;
    }
}

int isValidName(char *name) {
    return (name && strlen(name) > 0 && strlen(name) < MAX_NAME_LENGTH);
}

int main() {
    Flight *head = NULL;
    int choice, flightNumber, capacity;
    char name[MAX_NAME_LENGTH], departure[MAX_LOCATION_LENGTH], destination[MAX_LOCATION_LENGTH], departureDate[MAX_DATE_LENGTH];

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
                printf("Enter flight number: ");
                scanf("%d", &flightNumber);
                printf("Enter flight capacity: ");
                scanf("%d", &capacity);
                getchar();
                printf("Enter departure location: ");
                fgets(departure, sizeof(departure), stdin);
                departure[strcspn(departure, "\n")] = '\0';
                printf("Enter destination: ");
                                fgets(destination, sizeof(destination), stdin);
                destination[strcspn(destination, "\n")] = '\0';
                printf("Enter departure date (e.g., 2025-02-04): ");
                fgets(departureDate, sizeof(departureDate), stdin);
                departureDate[strcspn(departureDate, "\n")] = '\0';

                if (capacity <= 0) {
                    printf("Capacity must be greater than 0.\n");
                    break;
                }

                head = addFlight(head, flightNumber, capacity, departure, destination, departureDate);
                printf("Flight %d added with capacity %d. Departure: %s, Destination: %s, Date: %s\n", flightNumber, capacity, departure, destination, departureDate);
                break;

            case 2:
                printf("Enter flight number: ");
                scanf("%d", &flightNumber);
                getchar();
                printf("Enter passenger name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = '\0';
                printf("Enter seat number: ");
                int seatNumber;
                scanf("%d", &seatNumber);

                if (!isValidName(name)) {
                    printf("Invalid name. It must be non-empty and less than %d characters.\n", MAX_NAME_LENGTH);
                    break;
                }

                addReservation(head, name, flightNumber, seatNumber);
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
                exit(0);

            default:
                printf("Invalid choice. Please enter a number between 1 and 6.\n");
                break;
        }
    }
    return 0;
}
