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