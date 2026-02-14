#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NAME_LEN 50

// Employee structure
struct Employee {
    char name[NAME_LEN];
    int id;
    float salary;
};

void Employee_details(void);
void view_details(void);
void delete_employee(void);

// Append a new employee to employee.csv
void Employee_details(void)
{
    FILE *p = fopen("employee.csv", "a");
    if (!p) {
        perror("Unable to open employee.csv for appending");
        return;
    }

    struct Employee e;

    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);

    printf("Enter employee name: ");
    if (fgets(e.name, sizeof e.name, stdin) == NULL) {
        printf("No name entered. Aborting entry.\n");
        fclose(p);
        return;
    }

    size_t ln = strlen(e.name);
    if (ln && e.name[ln - 1] == '\n') e.name[ln - 1] = '\0';

    printf("Enter employee ID: ");
    if (scanf("%d", &e.id) != 1) {
        printf("Invalid ID. Aborting entry.\n");
        fclose(p);
        return;
    }

    printf("Enter employee salary: ");
    if (scanf("%f", &e.salary) != 1) {
        printf("Invalid salary. Aborting entry.\n");
        fclose(p);
        return;
    }

    fprintf(p, "%s,%d,%.2f\n", e.name, e.id, e.salary);
    fclose(p);
    printf("Employee saved.\n");
}

// Read and display employee.csv
void view_details(void)
{
    printf("-------Show The Employee Details-------\n");
    printf("Name\tID\tSalary\n");

    FILE *ptr = fopen("employee.csv", "r");
    if (!ptr) {
        printf("No records found (employee.csv not present).\n");
        return;
    }

    char line[256];
    int any = 0;
    while (fgets(line, sizeof line, ptr)) {
        char name[NAME_LEN];
        int id;
        float sal;

        if (sscanf(line, " %49[^,],%d,%f", name, &id, &sal) == 3) {
            printf("%s\t%d\t%.2f\n", name, id, sal);
        } else {
            fputs(line, stdout);
        }
        any = 1;
    }

    if (!any) printf("No records found.\n");
    fclose(ptr);
}

// Delete employee by ID
void delete_employee()
{
    int idToDelete;
    printf("Enter Employee ID to delete: ");
    scanf("%d", &idToDelete);

    FILE *fp = fopen("employee.csv", "r");
    if (!fp) {
        printf("No records found (employee.csv missing).\n");
        return;
    }

    FILE *temp = fopen("temp.csv", "w");
    if (!temp) {
        printf("Error creating temporary file.\n");
        fclose(fp);
        return;
    }

    char line[256];
    int found = 0;

    while (fgets(line, sizeof line, fp)) {
        char name[NAME_LEN];
        int id;
        float salary;

        if (sscanf(line, "%49[^,],%d,%f", name, &id, &salary) == 3) {
            if (id == idToDelete) {
                found = 1;
                continue;   // skip writing the deleted employee
            }
        }
        fputs(line, temp);
    }

    fclose(fp);
    fclose(temp);

    // Replace original file with updated file
    remove("employee.csv");
    rename("temp.csv", "employee.csv");

    if (found)
        printf("Employee with ID %d deleted successfully.\n", idToDelete);
    else
        printf("Employee ID %d not found.\n", idToDelete);
}

int main(void)
{
    int n;

    while (1) {
        printf("-----------Employee Details Entry----------\n");
        printf("1 - Enter details of Employee\n");
        printf("2 - Display Employee Details\n");
        printf("3 - Exit\n");
        printf("4 - Delete Employee\n");
        printf("Enter your choice: ");

        if (scanf("%d", &n) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Invalid input, please try again.\n");
            continue;
        }

        switch (n) {
            case 1:
                Employee_details();
                break;
            case 2:
                view_details();
                printf("\n");
                break;
            case 3:
                printf("Exiting the program.\n");
                return 0;
            case 4:
                delete_employee();
                break;
            default:
                printf("This choice does not exist, please try again!\n");
        }
    }
    return 0;
}
