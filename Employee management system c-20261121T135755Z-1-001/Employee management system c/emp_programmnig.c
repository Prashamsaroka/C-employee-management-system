#include <stdio.h>
#include <string.h>

struct Employee {
    char id[10];
    char name[50];
    int age;
    char gender[10];
    char department[50];
    char post[50]; 
    char contact[15];
    float salary;
    char performanceReview[100];
    float basicSalary;
    float allowance;
    float deductions;
};

// Function prototypes
void createEmployee();
void viewEmployee();
void updateEmployee();
void deleteEmployee();
void showAllEmployees();
void updatePerformanceReview();  
void showEmployeesWithReviews(); 
void generatePaySlip();         

// Function to get the last employee ID from the file
int getLastEmployeeId() {
	FILE *fp = fopen("employees.txt", "r");
    char line[200];
    char lastId[10] = "1000";
    int lastIdInt = 1000;
    
    // Move the file pointer to the beginning
    fseek(fp, 0, SEEK_SET);
    
    // Read each line to find the last ID
    while (fgets(line, sizeof(line), fp) != NULL) {
        sscanf(line, "%[^|]", lastId); // Read the first part (ID) of the line
    }

    // Convert the lastId to integer
    lastIdInt = atoi(lastId);
    return lastIdInt;
}

// Create Employee
void createEmployee() {
    FILE *fp = fopen("employees.txt", "a+");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    struct Employee e;
    
    // Get last employee ID and increment it for new ID
    int newId = getLastEmployeeId(fp) + 1;
    sprintf(e.id, "%d", newId);
    
    printf("\nNew Employee ID: %s\n", e.id);

    // Input employee details
    printf("Enter Name: ");
    scanf(" %[^\n]", e.name);
    printf("Enter Age: ");
    scanf("%d", &e.age);
    printf("Enter Gender (M/m for Male, F/f for Female, O/o for Other): ");
    scanf("%s", e.gender);
    
    // Normalize gender input
    if (strcasecmp(e.gender, "M") == 0) {
        strcpy(e.gender, "Male");
    } else if (strcasecmp(e.gender, "F") == 0) {
        strcpy(e.gender, "Female");
    } else if (strcasecmp(e.gender, "O") == 0) {
        strcpy(e.gender, "Other");
    }

    printf("Enter Department: ");
    scanf(" %[^\n]", e.department);
    printf("Enter Post: ");
    scanf(" %[^\n]", e.post);
    printf("Enter Contact: ");
    scanf("%s", e.contact);
    
    // Ensure contact has country code prefix
    if (strncmp(e.contact, "+977-", 5) != 0) {
        char formattedContact[20];
        sprintf(formattedContact, "+977-%s", e.contact);
        strcpy(e.contact, formattedContact);
    }

    printf("Enter Salary: ");
    scanf("%f", &e.salary);
    
    // Write employee data to file
    fprintf(fp, "%s|%s|%d|%s|%s|%s|%s|%.2f\n", e.id, e.name, e.age, e.gender, e.department, e.post, e.contact, e.salary);
    fclose(fp);

    printf("\nEmployee record created successfully!\n");
}

// View Employee
void viewEmployee() {
    FILE *fp = fopen("employees.txt", "r"); 
    if (!fp) {
        printf("Error opening file.\n");
        return;
    }

    struct Employee e;
    char id[10], searchBy[50];
    int found = 0;

    printf("\nSearch by (1) ID or (2) Name: ");
    int option;
    scanf("%d", &option);

    if (option == 1) {
        printf("Enter Employee ID: ");
        scanf("%s", id);
    } else {
        printf("Enter Employee Name: ");
        scanf(" %[^\n]", searchBy);
    }

    // Reading data from file with optional performanceReview
    while (fscanf(fp, "%[^|]|%[^|]|%d|%[^|]|%[^|]|%[^|]|%[^|]|%f|%[^\n]\n",
                  e.id, e.name, &e.age, e.gender, e.department, e.post, e.contact, &e.salary, e.performanceReview) != EOF) {
        
        // Check if the performanceReview field is empty and set "No comments" if so
        if (strlen(e.performanceReview) == 0) {
            strcpy(e.performanceReview, "No comments");
        }

        if ((option == 1 && strcmp(e.id, id) == 0) || (option == 2 && strcmp(e.name, searchBy) == 0)) {
            printf("\nEmployee Found!\n");
            printf("\nID: %s\nName: %s\nAge: %d\nGender: %s\nDepartment: %s\nPost: %s\nContact: %s\nSalary: %.2f\nperformanceReview: %s\n",
                   e.id, e.name, e.age, e.gender, e.department, e.post, e.contact, e.salary, e.performanceReview);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Employee not found.\n");
    }

    fclose(fp);
}

// Update Employee
void updateEmployee() {
    FILE *fp = fopen("employees.txt", "r"); // Correct file name
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) {
        printf("Error opening file.\n");
        return;
    }

    struct Employee e;
    char id[10];
    int found = 0;

    printf("\nEnter Employee ID to Update: ");
    scanf("%s", id);

    // Reading data with optional review field
    while (fscanf(fp, "%[^|]|%[^|]|%d|%[^|]|%[^|]|%[^|]|%[^|]|%f|%[^\n]\n",
                  e.id, e.name, &e.age, e.gender, e.department, e.post, e.contact, &e.salary, e.performanceReview) != EOF) {
        if (strcmp(e.id, id) == 0) {
            found = 1;
            printf("\nEmployee Found!\n");
            printf("\nID: %s\nName: %s\nAge: %d\nGender: %s\nDepartment: %s\nPost: %s\nContact: %s\nSalary: %.2f\nperformanceReview: %s\n",
                   e.id, e.name, e.age, e.gender, e.department, e.post, e.contact, e.salary, *e.performanceReview ? e.performanceReview : "No comments");

            // Input for updated details
            printf("\nEnter New Name: ");
            scanf(" %[^\n]", e.name);
            printf("Enter New Age: ");
            scanf("%d", &e.age);
            printf("Enter New Gender (M/m for Male, F/f for Female, O/o for Other): ");
            scanf("%s", e.gender);

            if (strcasecmp(e.gender, "M") == 0) {
                strcpy(e.gender, "Male");
            } else if (strcasecmp(e.gender, "F") == 0) {
                strcpy(e.gender, "Female");
            } else if (strcasecmp(e.gender, "O") == 0) {
                strcpy(e.gender, "Other");
            }

            printf("Enter New Department: ");
            scanf(" %[^\n]", e.department);
            printf("Enter New Post: ");
            scanf(" %[^\n]", e.post);
            printf("Enter New Contact: ");
            scanf("%s", e.contact);

            if (strncmp(e.contact, "+977-", 5) != 0) {
                char formattedContact[20];
                sprintf(formattedContact, "+977-%s", e.contact);
                strcpy(e.contact, formattedContact);
            }

            printf("Enter New Salary: ");
            scanf("%f", &e.salary);

            // If there's no performanceReview input, set it to "No comments"
            strcpy(e.performanceReview, *e.performanceReview ? e.performanceReview : "No comments");
        }

        // Write the current record to temp file, with the updated information if found
        fprintf(temp, "%s|%s|%d|%s|%s|%s|%s|%.2f|%s\n", e.id, e.name, e.age, e.gender, e.department, e.post, e.contact, e.salary, e.performanceReview);
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove("employees.txt");
        rename("temp.txt", "employees.txt");
        printf("\nEmployee record updated successfully!\n");
    } else {
        printf("\nEmployee not found.\n");
        remove("temp.txt");
    }
}

void deleteEmployee() {
    FILE *fp = fopen("employees.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp || !temp) {
        printf("Error opening file.\n");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    struct Employee e;
    char id[10];
    int found = 0;

    printf("\nEnter Employee ID to Delete: ");
    scanf("%s", id);

    // Read and rewrite all records except the target one
    while (fscanf(fp, "%[^|]|%[^|]|%d|%[^|]|%[^|]|%[^|]|%[^|]|%f|%[^\n]\n",
                  e.id, e.name, &e.age, e.gender, e.department, e.post, e.contact, &e.salary, e.performanceReview) != EOF) {
        if (strcmp(e.id, id) != 0) {
            fprintf(temp, "%s|%s|%d|%s|%s|%s|%s|%.2f|%s\n",
                    e.id, e.name, e.age, e.gender, e.department, e.post, e.contact, e.salary, e.performanceReview);
        } else {
            found = 1;
            printf("\nDeleted Record:\n");
            printf("ID: %s, Name: %s, Age: %d, Department: %s, Salary: %.2f\n",
                   e.id, e.name, e.age, e.department, e.salary);
        }
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        if (remove("employees.txt") == 0 && rename("temp.txt", "employees.txt") == 0) {
            printf("\nEmployee record deleted successfully!\n");
        } else {
            printf("\nError updating file.\n");
        }
    } else {
        printf("\nEmployee ID not found.\n");
        remove("temp.txt");
    }
}

// Show All Employee
void showAllEmployees() {
    FILE *file = fopen("employees.txt", "r");
    struct Employee emp;
    char buffer[200];  // Temporary buffer to handle each line

    if (file == NULL) {
        printf("Error: Could not open file.\n");
        return;
    }

    printf("\nAll Employees:\n");
    printf("------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("| %-10s | %-20s | %-3s | %-6s | %-15s | %-20s | %-15s | %-10s | %-20s |\n", "ID", "Name", "Age", "Gender", "Department", "Post", "Contact", "Salary", "Performance Review");
    printf("------------------------------------------------------------------------------------------------------------------------------------\n");

    int recordFound = 0; // Track if any records are found

    // Start reading the file line-by-line
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        recordFound = 1;  // Mark that a record was found

        // Attempt to parse with or without performance review
        int fieldsParsed = sscanf(buffer, "%[^|]|%[^|]|%d|%[^|]|%[^|]|%[^|]|%[^|]|%f|%[^\n]",
                                  emp.id, emp.name, &emp.age, emp.gender, emp.department, emp.post, emp.contact, &emp.salary, emp.performanceReview);

        if (fieldsParsed < 8) {
            // If we have fewer than 8 fields, it’s an incomplete record
            printf("Error: Malformed employee record.\n");
            continue;
        }

        // Set performance review to "No comments" if it's missing
        if (fieldsParsed == 8) {
            strcpy(emp.performanceReview, "No comments");
        }

        // Print employee details in the formatted table
        printf("| %-10s | %-20s | %-3d | %-6s | %-15s | %-20s | %-15s | %-10.2f | %-20s |\n",
               emp.id, emp.name, emp.age, emp.gender, emp.department, emp.post, emp.contact, emp.salary, emp.performanceReview);
    }

    // Check if no records were found
    if (!recordFound) {
        printf("No employee records found.\n");
    }

    printf("------------------------------------------------------------------------------------------------------------------------------------\n");

    fclose(file);
}

// Update Performance Review for an employee
void updatePerformanceReview() {
    FILE *fp = fopen("employees.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    struct Employee e;
    char id[10];
    int found = 0;

    printf("\nEnter Employee ID to Update Performance Review: ");
    scanf("%s", id);

    while (fscanf(fp, "%[^|]|%[^|]|%d|%[^|]|%[^|]|%[^|]|%[^|]|%f|%[^\n]\n", e.id, e.name, &e.age, e.gender, e.department, e.post, e.contact, &e.salary, e.performanceReview) == 9) {
        if (strcmp(e.id, id) == 0) {
            found = 1;
            printf("\nEnter New Performance Review for %s: ", e.name);
            scanf(" %[^\n]", e.performanceReview);
            printf("Performance review updated for employee %s\n", e.name);
        }
        fprintf(temp, "%s|%s|%d|%s|%s|%s|%s|%.2f|%s\n", e.id, e.name, e.age, e.gender, e.department, e.post, e.contact, e.salary, e.performanceReview);
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove("employees.txt");
        rename("temp.txt", "employees.txt");
    } else {
        printf("\nEmployee not found.\n");
        remove("temp.txt");
    }

    printf("\nPress Enter to return to the main menu...");
    getchar(); // Capture the newline from previous input
    getchar(); // Wait for Enter key press
}

// Show Employees with Performance Reviews
void showEmployeesWithReviews() {
    FILE *fp = fopen("employees.txt", "r");
    struct Employee e;
    char line[256];  // Buffer to store each line from the file

    if (fp == NULL) {
        printf("Error: Could not open file.\n");
        return;
    }

    printf("\nEmployees with Performance Reviews:\n");
    printf("----------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("%-10s %-20s %-3s %-6s %-15s %-20s %-15s %-10s %-20s\n", 
           "ID", "Name", "Age", "Gender", "Department", "Post", "Contact", "Salary", "Performance Review");
    printf("----------------------------------------------------------------------------------------------------------------------------------------\n");

    // Read each line, print it for debugging, and parse it
    while (fgets(line, sizeof(line), fp)) {

        // Adjusted sscanf format string to match the expected input
        if (sscanf(line, "%[^|]|%[^|]|%d|%[^|]|%[^|]|%[^|]|%[^|]|%f|%[^\n]",
                   e.id, e.name, &e.age, e.gender, e.department, e.post, e.contact, &e.salary, e.performanceReview) >= EOF) {
            // Fill in "No comments" if performanceReview is empty
            if (strlen(e.performanceReview) == 0) {
                strcpy(e.performanceReview, "No comments");
            }
            
            // Print employee information including performance review
            printf("%-10s %-20s %-3d %-6s %-15s %-20s %-15s %-10.2f %-20s\n", 
                   e.id, e.name, e.age, e.gender, e.department, e.post, e.contact, e.salary, e.performanceReview);
        } else {
            // Debug: If parsing fails, print a message
            printf("Debug: Failed to parse line: %s\n", line);
        }
    }

    printf("----------------------------------------------------------------------------------------------------------------------------------------\n");
    fclose(fp);
}

// Generate Pay Slip
void generatePaySlip() {
    FILE *fp = fopen("employees.txt", "r");
    FILE *paySlipFile;
    struct Employee emp;
    char id[10], searchBy[50];
    int option, found = 0;

    if (fp == NULL) {
        printf("Error opening employee file.\n");
        return;
    }

    int choice;

    do {
        printf("\n1. Generate Payslip for Employee\n");
        printf("2. Print Payslip for the Employee\n");
        printf("3. Go Back to Menu\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            printf("\nGenerate Payslip by (1) ID or (2) Name: ");
            scanf("%d", &option);

            if (option == 1) {
                printf("Enter Employee ID: ");
                scanf("%s", id);
            } else {
                printf("Enter Employee Name: ");
                scanf(" %[^\n]", searchBy);
            }

            // Reading employee data without allowance/deduction fields
            while (fscanf(fp, "%[^|]|%[^|]|%d|%[^|]|%[^|]|%[^|]|%[^|]|%f|%[^\n]\n",
                    emp.id, emp.name, &emp.age, emp.gender, emp.department, emp.post, emp.contact, 
                    &emp.salary, emp.performanceReview) != EOF) {
                
                if ((option == 1 && strcmp(emp.id, id) == 0) || 
                    (option == 2 && strcmp(emp.name, searchBy) == 0)) {
                    found = 1;

                    // Calculate basic salary, allowance, and deductions
                    float basicSalary = emp.salary * 0.7f;
                    float allowance = emp.salary * 0.2f;
                    float deductions = emp.salary * 0.1f;
                    float netPay = basicSalary + allowance - deductions;

                    // Create filename and open pay slip file
                    char filename[100];
                    sprintf(filename, "PaySlips/%s_payslip.txt", emp.id); // Saves as PaySlips/ID_payslip.txt
                    paySlipFile = fopen(filename, "w");

                    if (paySlipFile == NULL) {
                        printf("Error creating payslip file.\n");
                        fclose(fp);
                        return;
                    }

                    // Write employee details to payslip
                    fprintf(paySlipFile, "=================== PAYSLIP ===================\n");
                    fprintf(paySlipFile, "Employee Name   : %s\n", emp.name);
                    fprintf(paySlipFile, "ID              : %s\n", emp.id);
                    fprintf(paySlipFile, "Age             : %d\n", emp.age);
                    fprintf(paySlipFile, "Gender          : %s\n", emp.gender);
                    fprintf(paySlipFile, "Department      : %s\n", emp.department);
                    fprintf(paySlipFile, "Post            : %s\n", emp.post);
                    fprintf(paySlipFile, "Contact         : %s\n", emp.contact);
                    fprintf(paySlipFile, "Performance     : %s\n", emp.performanceReview);
                    fprintf(paySlipFile, "-----------------------------------------------\n");
                    fprintf(paySlipFile, "Basic Salary    : %.2f\n", basicSalary);
                    fprintf(paySlipFile, "Allowance       : %.2f\n", allowance);
                    fprintf(paySlipFile, "Deductions      : %.2f\n", deductions);
                    fprintf(paySlipFile, "-----------------------------------------------\n");
                    fprintf(paySlipFile, "Total Salary    : %.2f\n", emp.salary);
                    fprintf(paySlipFile, "Net Pay         : %.2f\n", netPay);
                    fprintf(paySlipFile, "================================================\n");

                    fclose(paySlipFile);
                    printf("Payslip generated successfully and saved as %s!\n", filename);
                    break;
                }
            }
            if (!found) {
                printf("Employee not found.\n");
            }
            rewind(fp); // Reset file pointer for next iteration
        } else if (choice == 2) {
            printf("\nEnter Employee ID to print the existing payslip: ");
            scanf("%s", id);
            char filename[100];
            sprintf(filename, "PaySlips/%s_payslip.txt", id);
            paySlipFile = fopen(filename, "r");
            if (paySlipFile == NULL) {
                printf("Payslip not found for Employee ID: %s.\n", id);
            } else {
                char line[256];
                printf("\nPayslip for Employee ID: %s\n", id);
                while (fgets(line, sizeof(line), paySlipFile)) {
                    printf("%s", line);
                }
                fclose(paySlipFile);
            }
        }
    } while (choice != 3);
    fclose(fp);
}

int main() {
    int choice;

    do {
        printf("\n======================================\n");
        printf("Employee Management System\n");
        printf("======================================\n");
        printf("1. Create Employee Profile\n");
        printf("2. View Employee Details\n");
        printf("3. Update Employee Information\n");
        printf("4. Delete Inactive Profile\n");
        printf("5. Show All Employees\n");
        printf("6. Update Performance Review\n");
        printf("7. Show Employees with Performance Reviews\n");
        printf("8. Generate Pay Slip\n");
        printf("9. Exit\n");
        printf("======================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createEmployee();
                break;
            case 2:
                viewEmployee();
                break;
            case 3:
                updateEmployee();
                break;
            case 4:
                deleteEmployee();
                break;
            case 5:
                showAllEmployees();
                break;
            case 6:
                updatePerformanceReview();
                break;
            case 7:
                showEmployeesWithReviews();
                break;
            case 8:
                generatePaySlip();    
                break;
            case 9:
                printf("\nExiting...\n");
                break;
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    } while (choice != 9);

    return 0;
}
