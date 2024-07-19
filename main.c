#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PASS_THRESHOLD 40
#define FILE_PATH "./student_records.txt"

struct Student {
    char firstName[50];
    char lastName[50];
    int rollNumber;
    float marks;
};


// Function to input student information
void addStudent(struct Student **students, int *numStudents, int *size) {
    char choice;

    do {
        if (*numStudents >= *size) {
            *size *= 2;
            *students = realloc(*students, (*size) * sizeof(struct Student));
            if (*students == NULL) {
                perror("Failed to reallocate memory");
                exit(EXIT_FAILURE);
            }
        }

        printf("\nEnter information for student %d:\n", *numStudents + 1);

        printf("Enter student's first name: ");
        fgets((*students)[*numStudents].firstName, sizeof((*students)[*numStudents].firstName), stdin);
        (*students)[*numStudents].firstName[strcspn((*students)[*numStudents].firstName, "\n")] = '\0';

        printf("Enter student's last name: ");
        fgets((*students)[*numStudents].lastName, sizeof((*students)[*numStudents].lastName), stdin);
        (*students)[*numStudents].lastName[strcspn((*students)[*numStudents].lastName, "\n")] = '\0';

        printf("Enter roll number: ");
        scanf("%d", &(*students)[*numStudents].rollNumber);

        printf("Enter marks: ");
        scanf("%f", &(*students)[*numStudents].marks);

        getchar();

        if ((*students)[*numStudents].marks > PASS_THRESHOLD) {
            printf("Student has passed.ミ☆\n");
        } else {
            printf("Student has failed.☹\n");
        }

        (*numStudents)++;

        printf("\nDo you want to enter information for another student? (y/n): ");
        scanf(" %c", &choice);
        getchar();

    } while (choice == 'y' || choice == 'Y');
}

// Function to remove student
void removeStudent(struct Student *students, int *numStudents, int rollNumber) {
    for (int i = 0; i < *numStudents; i++) {
        if (students[i].rollNumber == rollNumber) {
            students[i] = students[--(*numStudents)];
            printf("Student with roll number %d removed successfully.\n", rollNumber);
            return;
        }
    }
    printf("Student with roll number %d not found.\n", rollNumber);
}

void searchStudent(struct Student *students, int numStudents, int rollNumber) {
    for (int i = 0; i < numStudents; i++) {
        if (students[i].rollNumber == rollNumber) {
            printf("\nStudent found:\n");
            printf("First Name: %s\n", students[i].firstName);
            printf("Last Name: %s\n", students[i].lastName);
            printf("Roll Number: %d\n", students[i].rollNumber);
            printf("Marks: %.2f\n", students[i].marks);
            return;
        }
    }
    printf("Student with roll number %d not found.\n", rollNumber);
}

void modifyStudent(struct Student *students, int numStudents, int rollNumber) {
    for (int i = 0; i < numStudents; i++) {
        if (students[i].rollNumber == rollNumber) {
            struct Student *student = &students[i];

            printf("\nModifying student with roll number %d:\n", rollNumber);

            printf("Enter new first name: ");
            fgets(student->firstName, sizeof(student->firstName), stdin);
            student->firstName[strcspn(student->firstName, "\n")] = '\0';

            printf("Enter new last name: ");
            fgets(student->lastName, sizeof(student->lastName), stdin);
            student->lastName[strcspn(student->lastName, "\n")] = '\0';

            printf("Enter new marks: ");
            scanf("%f", &student->marks);
            getchar(); // Consume newline character

            printf("Student record updated successfully.\n");
            return;
        }
    }
    printf("Student with roll number %d not found.\n", rollNumber);
}

void displayStudents(struct Student *students, int numStudents) {
    if (numStudents == 0) {
        printf("No student records to display.\n");
        return;
    }

    printf("\nStudent Records:\n");
    for (int i = 0; i < numStudents; i++) {
        struct Student *student = &students[i];
        printf("\nStudent %d\n", i + 1);
        printf("First Name: %s\n", student->firstName);
        printf("Last Name: %s\n", student->lastName);
        printf("Roll Number: %d\n", student->rollNumber);
        printf("Marks: %.2f\n", student->marks);
    }
}

void calculateAverageMarks(struct Student *students, int numStudents) {
    if (numStudents == 0) {
        printf("No students to calculate average marks.\n");
        return;
    }

    float totalMarks = 0.0;
    for (int i = 0; i < numStudents; i++) {
        totalMarks += students[i].marks;
    }
    printf("Average marks: %.2f\n", totalMarks / numStudents);
}

// Comparison function for ascending order
int compareMarksAscending(const void *a, const void *b) {
    float markA = ((struct Student *)a)->marks;
    float markB = ((struct Student *)b)->marks;
    return (markA > markB) - (markA < markB); // Ascending order
}

// Comparison function for descending order
int compareMarksDescending(const void *a, const void *b) {
    float markA = ((struct Student *)a)->marks;
    float markB = ((struct Student *)b)->marks;
    return (markB > markA) - (markB < markA); // Descending order
}

// Comparison function for sorting by roll number
int compareRollNumber(const void *a, const void *b) {
    int rollA = ((struct Student *)a)->rollNumber;
    int rollB = ((struct Student *)b)->rollNumber;
    return rollA - rollB; // Ascending order by roll number
}

// Function to sort students in ascending order by marks
void sortMarksAscending(struct Student *students, int numStudents) {
    qsort(students, numStudents, sizeof(struct Student), compareMarksAscending);
    printf("Students sorted in ascending order by marks.\n");
}

// Function to sort students in descending order by marks
void sortMarksDescending(struct Student *students, int numStudents) {
    qsort(students, numStudents, sizeof(struct Student), compareMarksDescending);
    printf("Students sorted in descending order by marks.\n");
}

// Function to sort by roll number
void sortByRollNumber(struct Student *students, int numStudents) {
    qsort(students, numStudents, sizeof(struct Student), compareRollNumber);
    printf("Students sorted by roll number.\n");
}

void writeToFile(struct Student *students, int numStudents) {
    FILE *file = fopen(FILE_PATH, "w");
    if (file == NULL) {
        perror("Failed to open file for writing");
        return;
    }

    for (int i = 0; i < numStudents; i++) {
        fprintf(file, "%s %s %d %.2f\n", students[i].firstName, students[i].lastName, students[i].rollNumber, students[i].marks);
    }

    fclose(file);
    printf("Student records written to file successfully.\n");
}

void readFromFile(struct Student **students, int *numStudents, int *size) {
    FILE *file = fopen(FILE_PATH, "r");
    if (file == NULL) {
        perror("Failed to open file for reading");
        return;
    }

    *numStudents = 0;
    while (!feof(file)) {
        if (*numStudents >= *size) {
            *size *= 2;
            *students = realloc(*students, (*size) * sizeof(struct Student));
            if (*students == NULL) {
                perror("Failed to reallocate memory");
                fclose(file);
                return;
            }
        }

        fscanf(file, "%s %s %d %f", (*students)[*numStudents].firstName, (*students)[*numStudents].lastName, &(*students)[*numStudents].rollNumber, &(*students)[*numStudents].marks);
        (*numStudents)++;
    }

    fclose(file);
    printf("Student records read from file successfully.\n");
}

void displayMenu() {
    printf("\n✿❀⊱⊱⊱ TASK MENU ⊰⊰⊰❀✿\n");
    printf("1. Add Student\n");
    printf("2. Remove Student\n");
    printf("3. Search Student by Roll Number\n");
    printf("4. Modify Student\n");
    printf("5. Display All Students\n");
    printf("6. Calculate Average Marks\n");
    printf("7. Sort Students by Marks (Ascending)\n");
    printf("8. Sort Students by Marks (Descending)\n");
    printf("9. Sort Students by Roll Number\n");
    printf("10. Write to File\n");
    printf("11. Read from File\n");
    printf("12. Exit\n");
    printf("Enter your choice (1-12): ");
}

int main() {
char userName[20];

    printf("˚ ༘ ⋆｡˚ ✧ ˚ STUDENT RECORD MANAGEMENT SYSTEM ˚ ✧ ˚ ༘ ⋆｡˚\n");
    printf("             ˖⁺‧₊˚ ˚₊‧⁺˖✮ WELCOME ✮˖⁺‧₊˚ ˚₊‧⁺˖");
    printf("\n\nPlease enter your username: ");
   scanf("%s", &userName);
   printf("\n                    ˚₊✧⋆ Hello, %s ⋆✧₊˚\n", userName);

    int initialSize = 2;
    struct Student *students = malloc(initialSize * sizeof(struct Student));
    if (students == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    int numStudents = 0;
    int size = initialSize;
    int choice;
    int rollNumber;

    while (1) {
        displayMenu();
        scanf("%d", &choice);
        getchar(); // Consume newline character

        switch (choice) {
            case 1:
                addStudent(&students, &numStudents, &size);
                break;
            case 2:
                printf("Enter roll number to remove: ");
                scanf("%d", &rollNumber);
                getchar();
                removeStudent(students, &numStudents, rollNumber);
                break;
            case 3:
                printf("Enter roll number to search: ");
                scanf("%d", &rollNumber);
                getchar();
                searchStudent(students, numStudents, rollNumber);
                break;
            case 4:
                printf("Enter roll number to modify: ");
                scanf("%d", &rollNumber);
                getchar();
                modifyStudent(students, numStudents, rollNumber);
                break;
            case 5:
                displayStudents(students, numStudents);
                break;
            case 6:
                calculateAverageMarks(students, numStudents);
                break;
            case 7:
                sortMarksAscending(students, numStudents);
                break;
            case 8:
                sortMarksDescending(students, numStudents);
                break;
            case 9:
                sortByRollNumber(students, numStudents);
                break;
            case 10:
                writeToFile(students, numStudents);
                break;
            case 11:
                readFromFile(&students, &numStudents, &size);
                break;
            case 12:
                free(students);
                printf("\n˖⁺‧₊˚ ˚₊‧⁺˖ Goodbye, %s ˖⁺‧₊˚ ˚₊‧⁺˖\n",userName);
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}