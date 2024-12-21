#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Task {
    int id;
    char description[100];
    struct Task* next;
};

// Function prototypes
void addTask(struct Task** head, int* idCounter);
void deleteTask(struct Task** head);
void displayTasks(struct Task* head);
void saveTasksToFile(struct Task* head);
void loadTasksFromFile(struct Task** head, int* idCounter);

int main() {
    struct Task* head = NULL;  // Initialize linked list
    int idCounter = 1;         // Task ID counter
    int choice;

    // Load tasks from file at program start
    loadTasksFromFile(&head, &idCounter);

    // Menu-driven program
    do {
        printf("\nTo-Do List Application\n");
        printf("1. Add Task\n");
        printf("2. Delete Task\n");
        printf("3. Display Tasks\n");
        printf("4. Save and Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // To consume newline character

        switch (choice) {
            case 1:
                addTask(&head, &idCounter);
                break;
            case 2:
                deleteTask(&head);
                break;
            case 3:
                displayTasks(head);
                break;
            case 4:
                saveTasksToFile(head);
                printf("Tasks saved. Exiting...\n");
                break;
            default:
                printf("Invalid choice. Try again!\n");
        }
    } while (choice != 4);

    return 0;
}

void addTask(struct Task** head, int* idCounter) {
    struct Task* newTask = (struct Task*)malloc(sizeof(struct Task));
    if (newTask == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    newTask->id = (*idCounter)++;
    printf("Enter task description: ");
    fgets(newTask->description, sizeof(newTask->description), stdin);
    newTask->description[strcspn(newTask->description, "\n")] = '\0'; // Remove newline

    newTask->next = *head;
    *head = newTask;

    printf("Task added successfully!\n");
}

void deleteTask(struct Task** head) {
    if (*head == NULL) {
        printf("No tasks to delete.\n");
        return;
    }

    int taskId;
    printf("Enter Task ID to delete: ");
    scanf("%d", &taskId);

    struct Task *temp = *head, *prev = NULL;

    while (temp != NULL && temp->id != taskId) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Task ID not found.\n");
        return;
    }

    if (prev == NULL) { // Task to be deleted is the head
        *head = temp->next;
    } else {
        prev->next = temp->next;
    }

    free(temp);
    printf("Task deleted successfully!\n");
}

void displayTasks(struct Task* head) {
    if (head == NULL) {
        printf("No tasks to display.\n");
        return;
    }

    printf("\nTo-Do List:\n");
    struct Task* temp = head;
    while (temp != NULL) {
        printf("ID: %d, Description: %s\n", temp->id, temp->description);
        temp = temp->next;
    }
}

void saveTasksToFile(struct Task* head) {
    FILE* file = fopen("tasks.txt", "w");
    if (file == NULL) {
        printf("Error opening file for saving tasks.\n");
        return;
    }

    struct Task* temp = head;
    while (temp != NULL) {
        fprintf(file, "%d\n%s\n", temp->id, temp->description);
        temp = temp->next;
    }

    fclose(file);
}

void loadTasksFromFile(struct Task** head, int* idCounter) {
    FILE* file = fopen("tasks.txt", "r");
    if (file == NULL) {
        printf("No saved tasks found.\n");
        return;
    }

    while (1) {
        struct Task* newTask = (struct Task*)malloc(sizeof(struct Task));
        if (fscanf(file, "%d\n", &newTask->id) != 1) {
            free(newTask);
            break;
        }

        fgets(newTask->description, sizeof(newTask->description), file);
        newTask->description[strcspn(newTask->description, "\n")] = '\0'; // Remove newline

        newTask->next = *head;
        *head = newTask;

        if (newTask->id >= *idCounter) {
            *idCounter = newTask->id + 1;
        }
    }

    fclose(file);
    printf("Tasks loaded successfully!\n");
}
