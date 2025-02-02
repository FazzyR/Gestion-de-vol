#include<stdio.h>
#include<stdlib.h>

typedef struct Flight {
   int flightNumber;
   int capacity;
   int bookedSeats;
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
