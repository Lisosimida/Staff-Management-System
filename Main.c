#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

#define MAX_STAFF 100
#define MAX_LENGTH 100
#define MAX_LEAVES 100
#define MAX_LINE_LENGTH 100
#define MAX_STAFFS 100

struct Staff
{

    char id[5];
    char department[MAX_LENGTH];
    char name[MAX_LENGTH];
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];

};

void staffMenu();
void superiorMenu();
void adminMenu();
int addStaff(struct Staff staff[], int size);
void saveStaff(struct Staff staff[], int size);
void apply_leave();
void cancel_leave();
void check_leave_status();
void monthly_report();
void leaveSummary();
void approve_or_reject_leave();
void search_staff_by_date();
void print_all_leave();

//---------------------------------------------------

int addStaff(struct Staff staff[], int size)
{

    int last_id = 0;
    char validID[4]; // to hold the first valid ID from the file
    FILE *fp = fopen("staffLogin.txt", "r");
    if (fp == NULL) {
        printf("Error opening file\n");
        return size;
    }
    fscanf(fp, "%s", validID);
    fclose(fp);

    if (size > 0) {
        last_id = atoi(staff[size-1].id+1); // extract the numeric portion of the last ID and convert it to an int
    }

    char id[4];
    while (1) {
        printf("Enter new staff ID (Ex.S%02d): ", last_id+1);
        scanf("%s", id);

        // check if the entered ID is already in use
        fp = fopen("staffLogin.txt", "r");
        if (fp == NULL) {
            printf("Error opening file\n");
            return size;
        }
        char line[100];
        int id_exists = 0; // flag to indicate whether the entered ID already exists
        while (fgets(line, sizeof(line), fp)) {
            char temp_id[4];
            sscanf(line, "%s", temp_id); // read the first field of the line, which is the staff ID
            if (strcmp(temp_id, id) == 0) {
                printf("ID already in use\n");
                id_exists = 1;
                break;
            }
        }

        fclose(fp);

        // check if the entered ID is in the correct format and is not already in use
        if (strlen(id) != 3 || id[0] != 'S' || !isdigit(id[1]) || !isdigit(id[2]) || id_exists) {
            printf("Invalid or duplicate ID. Please enter a unique ID in the format S00\n");
            continue;
        }

        break;
    }

    strcpy(staff[size].id, id);

    char department;
    while (1) {
        printf("Enter department (A for Academic, M for Management): ");
        scanf(" %c", &department);

        if (department == 'A' || department == 'a') {
            strcpy(staff[size].department, "Academic");
            break;
        } else if (department == 'M' || department == 'm') {
            strcpy(staff[size].department, "Management");
            break;
        } else {
            printf("Invalid department. Please enter A or M\n");
        }
    }

    printf("Enter you name: ");
    scanf("%s", staff[size].name);

    printf("Enter a new username: ");
    scanf("%s", staff[size].username);

    printf("Enter a new password: ");
    scanf("%s", staff[size].password);

    size++;

    return size;
}

void saveStaff(struct Staff staff[], int size)
{
    FILE *fp;
    fp = fopen("staffLogin.txt", "a");
    if (fp == NULL)
    {
        printf("Error opening staffLogin.txt file.\n");
        return;
    }

    FILE *fp_leave;
    fp_leave = fopen("leave_details.txt", "a");
    if (fp_leave == NULL)
    {
        printf("Error opening leave_details.txt file.\n");
        return;
    }

    for (int i = 0; i < size; i++)
    {
        fprintf(fp, "%s APU %s %s %s %s\n", staff[i].id, staff[i].department, staff[i].name, staff[i].username, staff[i].password);

        // Calculate leave balances
        int annual_leave = 8;
        int compassionate_leave = 5;
        int emergency_leave = 5;
        int maternity_leave = 60;
        int medical_leave = 14;

        // Write leave details to file
        fprintf(fp_leave, "%s %s %d %d %d %d %d\n", staff[i].id, staff[i].name, annual_leave, compassionate_leave, emergency_leave, maternity_leave, medical_leave);
    
    }

    fclose(fp);
    fclose(fp_leave);
}

typedef struct {

    char id[5];
    char name[20];
    int annualLeave;
    int compassionateLeave;
    int emergencyLeave;
    int maternityLeave;
    int medicalLeave;

} StaffLeave;

StaffLeave staffLeave[MAX_STAFF];

void updateLeaveBal()
{
    char id[5];
    char name[20];
    char leaveFile[] = "leave_details.txt";
    int found = 0;
    int idMatched = 0;
    int annualLeave = 8;
    int compassionateLeave = 5;
    int emergencyLeave = 5;
    int maternityLeave = 60;
    int medicalLeave = 14;

    printf("Enter staff ID or staff name: ");
    scanf("%s", id);
    fflush(stdin);

    // search for the staff member in the leave file
    FILE* fp = fopen(leaveFile, "r");
    if (fp == NULL) {
        printf("Error opening file\n");
        return;
    }

    int i = 0;
    char buffer[100];
    while (fgets(buffer, 100, fp)) {
        sscanf(buffer, "%s %s %d %d %d %d %d", staffLeave[i].id, staffLeave[i].name,
            &staffLeave[i].annualLeave, &staffLeave[i].compassionateLeave, &staffLeave[i].emergencyLeave,
            &staffLeave[i].maternityLeave, &staffLeave[i].medicalLeave);
        if (strcmp(id, staffLeave[i].id) == 0 || strcmp(id, staffLeave[i].name) == 0) {
            found = 1;
            strcpy(name, staffLeave[i].name);
            idMatched = 1;
            break;
        }
        i++;
    }

    fclose(fp);

    if (!found) {
        printf("Staff member not found.\n");
        return;
    }

    // prompt the user to enter the new leave balance
    printf("Update leave balance of %s (%s)\n", staffLeave[i].name, staffLeave[i].id);
    printf("Enter new annual leave balance (press enter to keep current balance): ");
    char input[10];
    fgets(input, 10, stdin);
    if (input[0] != '\n') {
        int value = atoi(input);
        if (value >= 0 && value <= 8) {
            annualLeave = value;
        } else {
            printf("Error: Invalid input. Annual leave balance must be between 0 and 8.\n");
            return;
        }
      }
    

    printf("Enter new compassionate leave balance (press enter to keep current balance): ");
    fgets(input, 10, stdin);
    if (input[0] != '\n') {
        int value = atoi(input);
        if (value >= 0 && value <= 5) {
            compassionateLeave = value;
        } else {
            printf("Error: Invalid input. Compassionate leave balance must be between 0 and 5.\n");
            return;
        }
    
    }

    printf("Enter new emergency leave balance (press enter to keep current balance): ");
    fgets(input, 10, stdin);
    if (input[0] != '\n') {
        int value = atoi(input);
        if (value >= 0 && value <= 5) {
            emergencyLeave = value;
        } else {
            printf("Error: Invalid input. Emergency leave balance must be between 0 and 5.\n");
            return;
        }
    }

    printf("Enter new maternity leave balance (press enter to keep current balance): ");
    fgets(input, 10, stdin);
    if (input[0] != '\n') {
        int value = atoi(input);
        if (value >= 0 && value <= 60) {
            maternityLeave = value;
        } else {
            printf("Error: Invalid input. Maternity leave balance must be between 0 and 60.\n");
            return;
        }
    }

    printf("Enter new medical leave balance (press enter to keep current balance): ");
    fgets(input, 10, stdin);
    if (input[0] != '\n') {
        int value = atoi(input);
        if (value >= 0 && value <= 14) {
            medicalLeave = value;
        } else {
            printf("Error: Invalid input. Medical leave balance must be between 0 and 14.\n");
            return;
        }
    }

    // update the leave balance for the staff member
    fp = fopen(leaveFile, "r");
    FILE* tmpFp = fopen("temp.txt", "w");
    if (fp == NULL || tmpFp == NULL) {
        printf("Failed to open file");
        return;
    }

    while (fgets(buffer, 100, fp)) {
        sscanf(buffer, "%s %s %d %d %d %d %d", staffLeave[i].id, staffLeave[i].name,
            &staffLeave[i].annualLeave, &staffLeave[i].compassionateLeave, &staffLeave[i].emergencyLeave,
            &staffLeave[i].maternityLeave, &staffLeave[i].medicalLeave);

        if (strcmp(id, staffLeave[i].id) == 0 || strcmp(name, staffLeave[i].name) == 0) {
            idMatched = 1;
            fprintf(tmpFp, "%s %s %d %d %d %d %d\n", staffLeave[i].id, staffLeave[i].name,
                annualLeave, compassionateLeave, emergencyLeave, maternityLeave, medicalLeave);
        } else {
            fprintf(tmpFp, "%s", buffer);
        }
    }

    fclose(fp);
    fclose(tmpFp);

    // replace the old file with the updated temp file
    if (idMatched) {
        remove(leaveFile);
        rename("temp.txt", leaveFile);
        printf("\nLeave balance updated successfully.");
    } else {
        remove("temp.txt");
        printf("\nError: Staff ID/Name not found.");
    }

}

void search_staff()
{
    char search[100];
    fgets(search, sizeof(search), stdin);

    search[strcspn(search, "\n")] = '\0';
    
    // Validate the input
    while (strlen(search) == 0) {
        printf("Please enter staff ID or name: ");
        fgets(search, sizeof(search), stdin);
        search[strcspn(search, "\n")] = '\0';
    }

    FILE *file = fopen("leave.txt", "r");
    if (file == NULL) {
        printf("Error: Unable to open leave file.\n");
        return;
    }

    char buffer[100];
    int count = 1;
    bool found = false;

    while (!found && fgets(buffer, 100, file) != NULL) {
        char id_or_name[100], name[100];
        sscanf(buffer, "%s %s", id_or_name, name);

        if (strcmp(id_or_name, search) == 0 || strcmp(name, search) == 0) {
            found = true;
        }
    }

    if (found) {
        printf("Leave applications for staff %s:\n", search);
        printf("No.\tStaffID\t\tName\t\tLeave Type\tStart Date\tEnd Date\tStatus\n");
        printf("===============================================================================================================\n");

        rewind(file);  // Move the file pointer back to the beginning of the file

        while (fgets(buffer, 100, file) != NULL) {
            char id[100], name[100], type[100];
            int s_day, s_month, s_year, e_day, e_month, e_year, status;

            sscanf(buffer, "%s %s %s %d/%d/%d %d/%d/%d %d", id, name, type, &s_day, &s_month, &s_year, &e_day, &e_month, &e_year, &status);

            if (strcmp(id, search) == 0 || strcmp(name, search) == 0) {
                printf("%d\t%s\t\t%s\t\t%s\t\t%d/%d/%d\t%d/%d/%d\t", count, id, name, type, s_day, s_month, s_year, e_day, e_month, e_year);
                if (status == 0) {
                    printf("Pending\n");
                } else if (status == 1) {
                    printf("Approved\n");
                } else if (status == 2) {
                    printf("Rejected\n");
                } else {
                    printf("Unknown status\n");
                }

                count++;
            }
        }
    } else {
        printf("No leave applications found for staff %s.\n", search);
    }

    fclose(file);
    return;
}

typedef struct {
    char empId[4];
    char name[100];
    char type[100];
    int sday;
    int smonth;
    int syear;
    int eday;
    int emonth;
    int eyear;
    int status;
} LeaveApplication;

void monthly_report() {
    FILE *fp;
    char *filename = "leave.txt";
    char *output_file = "monthly_report.txt";
    char line[MAX_LINE_LENGTH];
    LeaveApplication leave[MAX_STAFFS];
    int i = 0, j = 0;
    int month = 0;
    int count = 0;

    while (1) {
        printf("Enter month (1-12): ");
        int result = scanf("%d", &month);
        if (result != 1) {
            printf("Error: Please enter an integer value.\n");
            // clear any remaining input from the buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        if (month < 1 || month > 12) {
            printf("Error: Please enter a value between 1 and 12.\n");
            continue;
        }
        break;
    }

    fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Could not open file %s", filename);
        return;
    }

    while (fgets(line, MAX_LINE_LENGTH, fp)) {
        sscanf(line, "%s %s %s %d/%d/%d %d/%d/%d %d", leave[i].empId, leave[i].name, leave[i].type, &leave[i].sday, &leave[i].smonth, &leave[i].syear, &leave[i].eday, &leave[i].emonth, 
        &leave[i].eyear, &leave[i].status);
        if (leave[i].smonth == month || leave[i].emonth == month) {
            count++;
        }
        i++;
    }

    fclose(fp);

    fp = fopen(output_file, "w");

    if (fp == NULL) {
        printf("Could not create file %s", output_file);
        return;
    }

    fprintf(fp, "Monthly Report for month %d:\n", month);
    fprintf(fp, "-----------------------------\n");
    fprintf(fp, "Total Leave Applications: %d\n", count);
    fprintf(fp, "\n");
    fprintf(fp, "Staff ID\tName\t\tDate\t\t\tStatus\n");
    fprintf(fp, "--------------------------------------------------\n");

    for (j = 0; j < i; j++) {
        if (leave[j].smonth == month || leave[j].emonth == month) {
            
            fprintf(fp, "%s\t\t\t%s\t\t%02d/%02d/%d\t\t", leave[j].empId, leave[j].name, leave[j].sday, leave[j].smonth, leave[j].syear, leave[j].status);

            if (leave[j].status == 0) {
                fprintf(fp,"Pending\n");
            } 
            else if (leave[j].status == 1) {
                fprintf(fp,"Approved\n");
            } 
            else if (leave[j].status == 2) {
                fprintf(fp,"Rejected\n");
            } 
            else {
                fprintf(fp,"Unknown status\n");
            }

        }

    }

    fclose(fp);

    printf("Monthly report created in file %s\n", output_file);

    return;
}

void approve_or_reject_leave() {
    // Open the leave file for reading
    FILE* file = fopen("leave.txt", "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return;
    }

    // Read all leave applications from the file into a buffer
    char buffer[100][100];
    int i = 0;
    while (fgets(buffer[i], 100, file) != NULL) {
        i++;
    }
    fclose(file);

    // Print out all leave applications
    print_all_leave();

    // Prompt the user to choose which leave application to approve/reject
    int choice;
    do {
        printf("Enter the row number of the leave application to approve or reject (0 to exit): ");
        scanf("%d", &choice);
        if (choice < 0 || choice > i) {
            printf("Invalid choice\n");
        }
    } while (choice < 0 || choice > i);

    // Exit if the user chooses 0
    if (choice == 0) {
        return;
    }

    // Parse the selected leave application
    char id[4];
    char name[100];
    char type[100];
    int s_day, s_month, s_year;
    int e_day, e_month, e_year;
    int status;
    sscanf(buffer[choice-1], "%s %s %s %d/%d/%d %d/%d/%d %d", id, name, type, &s_day, &s_month, &s_year, &e_day, &e_month, &e_year, &status);

    // Prompt the user to approve or reject the leave application
    char response;
    do {
        printf("Do you want to approve or reject the leave application? (a/r): ");
        scanf(" %c", &response);
        if (response != 'a' && response != 'A' && response != 'r' && response != 'R') {
            printf("Invalid choice\n");
        }
    } while (response != 'a' && response != 'A' && response != 'r' && response != 'R');

    // Update the status of the leave application in the file
    if (response == 'a' || response == 'A') {
        status=1;
        printf("Leave application approved\n");
    } else if (response=='r' || response=='R'){
        status=2;
        printf("Leave application rejected\n");
    }

    sprintf(buffer[choice-1], "%s %s %s %d/%d/%d %d/%d/%d %d\n", id, name, type, s_day, s_month, s_year, e_day, e_month, e_year, status);

    // Open the leave file for writing and write the updated leave applications to it
    file = fopen("leave.txt", "w");
    if (file == NULL) {
        printf("Error opening file\n");
        return;
    }
    for (int j = 0; j < i; j++) {
        fputs(buffer[j], file);
    }
    fclose(file);
}

void print_all_leave()
{
    FILE *file = fopen("leave.txt", "r");
    if (file == NULL) {
        printf("Error: Unable to open leave file.\n");
        return;
    }

    printf("List of all leave applications:\n");
    printf("No.\tStaff ID\tStaff Name\tLeave Type\tStart Date\tEnd Date\tStatus\n");
    printf("============================================================================================================\n");

    char buffer[100];
    int count = 1;

    while (fgets(buffer, 100, file) != NULL) {
        char id[4];
        char name[100];
        char type[100];
        int s_day, s_month, s_year;
        int e_day, e_month, e_year;
        int status;

        sscanf(buffer, "%s %s %s %d/%d/%d %d/%d/%d %d", id, name, type, &s_day, &s_month, &s_year, &e_day, &e_month, &e_year,  &status);

        printf("%d\t%s\t\t%s\t\t%s\t\t%d/%d/%d\t%d/%d/%d\t", count, id, name, type, s_day, s_month, s_year, e_day, e_month, e_year, status);
        if (status == 0) {
            printf("Pending\n");
        } else if (status == 1) {
            printf("Approved\n");
        } else if (status == 2) {
            printf("Rejected\n");
        } else {
            printf("Unknown status\n");
        }

        count++;
    }

    fclose(file);
}

struct Leave {
    char id[4];
    char staffID[4];
    char name[20];
    char type[20];
    char start_date[11];
    char end_date[11];
    int start_day;
    int start_month;
    int start_year;
    int end_day;
    int end_month;
    int end_year;
    int status;
};

void search_staff_by_date() {
    // Prompt user to enter a date
    char input_date[11];
    int day, month, year;

    do {
        printf("Enter a date (dd/mm/yyyy): ");
        scanf("%s", input_date);

        if (sscanf(input_date, "%d/%d/%d", &day, &month, &year) != 3) {
            printf("Invalid date format. Please enter the date in the format dd/mm/yyyy.\n");
        }
    } while (sscanf(input_date, "%d/%d/%d", &day, &month, &year) != 3);

    // Read leave data from file
    struct Leave leaves[MAX_LEAVES];
    FILE *fp = fopen("leave.txt", "r");
    if (fp == NULL) {
        printf("Error opening file.");
        exit(1);
    }
    int num_leaves = 0;
    char line[100];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%s %s %s %s %s %d", leaves[num_leaves].id, leaves[num_leaves].name, leaves[num_leaves].type, leaves[num_leaves].start_date, leaves[num_leaves].end_date, &leaves[num_leaves].status);
        num_leaves++;
    }
    fclose(fp);

    // Search for leave applications on input date
    int found = 0;
    int count = 0;
    for (int i = 0; i < num_leaves; i++) {
        if (strcmp(input_date, leaves[i].start_date) >= 0 && strcmp(input_date, leaves[i].end_date) <= 0) {
            if (leaves[i].status == 1) {
                printf("%s %s applied for %s leave\n", leaves[i].id, leaves[i].name, leaves[i].type);
                count++; // Increment count_staff when a valid leave application is found
            } else {
                printf("%s %s's %s leave application is pending or has been rejected by superior\n", leaves[i].id, leaves[i].name, leaves[i].type);
            }
            found = 1;
        } 
    }

    if (found) {
        printf("%d staff(s) applied for leave on %s.\n", count, input_date);
    } else {
        printf("No leave applications found on %s\n", input_date);
    }
    return;
} 

void adminMenu(){
    int adminOption, size=0;
    struct Staff staff[100];

    do{
        printf("==[Admin Menu]==\n");
        printf("[1] Add new staff info\n");
        printf("[2] Update the leave balance of staff\n");
        printf("[3] Search a staff\n");
        printf("[4] Generate monthly report\n");
        printf("[5] Exit\n");
        printf("Please enter your choice: ");
        scanf("%d", &adminOption);
        
        switch(adminOption){
            case 1:
                size = addStaff(staff, size);
                saveStaff(staff, size);
                printf("Staff added successfully.\n");
                break;
            case 2: 
                updateLeaveBal();
                break;
            case 3:
                search_staff();
                break;
            case 4:
                monthly_report();
                break;
            case 5:
                return;
            default:
                printf("Invalid input!\n");
                adminMenu();
                break;

        }

    }
    while(
        printf("Back to main menu\n"));
}

void staffMenu(){
    int staffOption;
    do{
        printf("==[Staff Menu]==\n");
        printf("[1] Apply Leave\n");
        printf("[2] Cancel Leave\n");
        printf("[3] Check leave status\n");
        printf("[4] Check the summary of leave information\n");
        printf("[5] Exit\n");
        printf("Please select one action to perform: ");
        scanf("%d", &staffOption);
                    
        switch(staffOption){
            case 1:
                apply_leave();
                break;
            case 2:
                cancel_leave();
                break;
            case 3:
                check_leave_status();
                break;
            case 4:
                leaveSummary();
                break;
            case 5:
                return;
            default:
                printf("Invalid option!\n");
                staffMenu();
                break;
        }
    }
    while(
      printf("Back to main menu\n"));
}

void superiorMenu(){
    int supOption;
    do{
        printf("==[Superior Menu]==\n");
        printf("[1] Approve or reject leave application\n");
        printf("[2] View staff's leave by date\n");
        printf("[3] Exit\n");
        printf("Please select 1 option: ");
        scanf("%d", &supOption);

        switch(supOption){
            case 1:
                approve_or_reject_leave();
                break;
            case 2:
                search_staff_by_date();
                break;
            case 3:
                return;
            default:
                printf("Invalid Option!");
                superiorMenu();
                break;
        }
  }

  while(("Back to main menu"));
}

void apply_leave()
{
    struct Leave leave;
    printf("Enter staff ID: ");
    scanf("%s", leave.staffID);
    printf("Enter staff name: ");
    scanf("%s", leave.name);
    // Prompt the user to select a leave type
    printf("Select leave type:\n");
    printf("1. Annual Leave\n");
    printf("2. Compassionate Leave\n");
    printf("3. Emergency Leave\n");
    printf("4. Maternity Leave\n");
    printf("5. Medical Leave\n");
    printf("Enter selection (1-5): ");
    int selection;
    scanf("%d", &selection);

    // Validate leave type selection
    if (selection < 1 || selection > 5) {
        printf("Error: Invalid selection.\n");
        return;
    }

    // Set leave type based on selection
    switch (selection) {
    case 1:
        strcpy(leave.type, "Annual");
        break;
    case 2:
        strcpy(leave.type, "Compassionate");
        break;
    case 3:
        strcpy(leave.type, "Emergency");
        break;
    case 4:
        strcpy(leave.type, "Maternity");
        break;
    case 5:
        strcpy(leave.type, "Medical");
        break;
    }

    printf("Enter start date (dd/mm/yyyy): ");
    scanf("%d/%d/%d", &leave.start_day, &leave.start_month, &leave.start_year);

  time_t now = time(NULL);
  struct tm *current_time = localtime(&now);
  int current_day = current_time->tm_mday;
  int current_month = current_time->tm_mon + 1;
  int current_year = current_time->tm_year + 1900;

  // Validate start date
  if (leave.start_year < current_year || 
     (leave.start_year == current_year && (leave.start_month < current_month ||
     (leave.start_month == current_month && leave.start_day < current_day)))) {
    printf("Error: Start date cannot be in the past.\n");
    return;
  }

  printf("Enter end date (dd/mm/yyyy): ");
  scanf("%d/%d/%d", &leave.end_day, &leave.end_month, &leave.end_year);

  // Validate end date
  if (leave.end_year < leave.start_year || 
     (leave.end_year == leave.start_year && (leave.end_month < leave.start_month || 
     (leave.end_month == leave.start_month && leave.end_day < leave.start_day + 1 )))) {
    printf("Error: End date should be after start date.\n");
    return;
  }

  leave.status = 0;  // Pending

  FILE *file = fopen("leave.txt", "a");
  fprintf(file, "%s %s %s %d/%d/%d %d/%d/%d %d\n", leave.staffID, leave.name, leave.type, leave.start_day, leave.start_month, leave.start_year, leave.end_day, leave.end_month, leave.end_year, leave.status);
  fclose(file);

  printf("Leave applied successfully.\n");
}

void cancel_leave()
{
  char staff_ID[100];
  printf("Enter staff ID: ");
  scanf("%s", staff_ID);

  FILE *file = fopen("leave.txt", "r");
  FILE *temp = fopen("temp.txt", "w");

  char buffer[100];
  int found = 0;

  while (fgets(buffer, 100, file) != NULL) {
    char id[4], name[100], type[100];
    int s_day, s_month, s_year, e_day, e_month, e_year, status;
    
    sscanf(buffer, "%s %s %s %d/%d/%d %d/%d/%d %d", id, name, type, &s_day, &s_month, &s_year, &e_day, &e_month, &e_year, &status);

    if (strcmp(id, staff_ID) == 0) {
      printf("Leave record found:\n");
      printf("Staff ID\tStaff Name\tLeave Type\tStart Date\tEnd Date\tStatus\n");
      printf("======================================================================================\n");
      printf("%s\t\t%s\t\t%s\t\t%d/%d/%d\t%d/%d/%d\t", id, name, type, s_day, s_month, s_year, e_day, e_month, e_year, status);
      if (status == 0) {
          printf("Pending\n");
      } 
      else if (status == 1) {
          printf("Approved\n");
      } 
      else if (status == 2) {
          printf("Rejected\n");
      } else {
          printf("Unknown status\n");
      }

      // Check if leave date has passed or leave is rejected
      time_t now = time(NULL);
      struct tm *t = localtime(&now);
      int current_day = t->tm_mday;
      int current_month = t->tm_mon + 1;
      int current_year = t->tm_year + 1900;
      
      if (status == 2 || (e_year < current_year || (e_year == current_year && e_month < current_month) || (e_year == current_year && e_month == current_month && e_day < current_day))) {
        printf("Leave cannot be cancelled as (Leave is rejected) or (Leave date is passed). Please Try Again!\n");

        // Return to prompt user to enter staff ID again
        cancel_leave();
        return;
      }

      printf("Are you sure you want to cancel this leave? (y/n)\n");

      char confirmation;
      scanf(" %c", &confirmation);

      if (confirmation == 'y' || confirmation == 'Y') {
        printf("Leave cancelled successfully.\n");
        found = 1;
      } else {
        fprintf(temp, "%s", buffer);
      }
    } else {
      fprintf(temp, "%s", buffer);
    }
  }
  
  fclose(file);
  fclose(temp);

  if (found == 0) {
    printf("Leave record not found.\n");
  } else {
    remove("leave.txt");
    rename("temp.txt", "leave.txt");
  }
}


void check_leave_status()
{
  char staff_name[100];

  printf("Enter staff name: ");
  scanf("%s", staff_name);

  FILE *file = fopen("leave.txt", "r");

  char buffer[100];
  int found = 0;

  printf("Leave status for %s:\n", staff_name);
  printf("Leave Type\tStart Date\tEnd Date\tStatus\n");

  while (fgets(buffer, 100, file) != NULL) {
    char id[4];
    char name[100];
    char type[100];
    int s_day, s_month, s_year;
    int e_day, e_month, e_year;
    int status;

    sscanf(buffer, "%s %s %s %d/%d/%d %d/%d/%d %d", id, name, type, &s_day, &s_month, &s_year, &e_day, &e_month, &e_year, &status);

    if (strcmp(name, staff_name) == 0) {
      found = 1;
      printf("%s\t%02d/%02d/%04d\t%02d/%02d/%04d\t%s\n", type, s_day, s_month, s_year, e_day, e_month, e_year, (status == 0) ? "Pending" : ((status == 1) ? "Approved" : "Rejected"));
    }
  }

  if (found == 0) {
    printf("No leave information found for %s.\n", staff_name);
  }

  fclose(file);
}

struct staff {
    char id[4];
    char name[20];
    int annual_leave;
    int compassionate_leave;
    int emergency_leave;
    int maternity_leave;
    int medical_leave;
};

void leaveSummary() {
    struct staff staff_list[MAX_STAFF];
    FILE *fp;
    int i, j, total_leaves;
    char name[20];

    // Open file
    fp = fopen("leave_details.txt", "r");
    if (fp == NULL) {
        printf("Error opening file\n");
        return;
    }

    // Read staff data from file
    for (i = 0; i < MAX_STAFF; i++) {
        fscanf(fp, "%s %s %d %d %d %d %d",
               staff_list[i].id,
               staff_list[i].name,
               &staff_list[i].annual_leave,
               &staff_list[i].compassionate_leave,
               &staff_list[i].emergency_leave,
               &staff_list[i].maternity_leave,
               &staff_list[i].medical_leave);
    }

    // Close file
    fclose(fp);

    // Input staff name
    printf("Please enter staff name: ");
    scanf("%s", name);

    // Search staff by name
    for (i = 0; i < MAX_STAFF; i++) {
        if (strcmp(staff_list[i].name, name) == 0) {
            // Calculate total leaves for staff
            total_leaves = staff_list[i].annual_leave +
                          staff_list[i].compassionate_leave +
                          staff_list[i].emergency_leave +
                          staff_list[i].maternity_leave +
                          staff_list[i].medical_leave;

            // Print summary
            printf("Name: %s\n", staff_list[i].name);
            printf("Total Leaves: %d\n", total_leaves);
            printf("Annual Leave: %d\n", staff_list[i].annual_leave);
            printf("Compassionate Leave: %d\n", staff_list[i].compassionate_leave);
            printf("Emergency Leave: %d\n", staff_list[i].emergency_leave);
            printf("Maternity Leave: %d\n", staff_list[i].maternity_leave);
            printf("Medical Leave: %d\n", staff_list[i].medical_leave);

            return;
        }
    }

    // Staff not found
    printf("Staff not found\n");
    return;
}


int main() {
    char school[MAX_LENGTH];
    char department[MAX_LENGTH];
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];
    int detectAdmin = 0;
    int detectSuperior = 0;
    int detectStaff = 0;

    printf("Enter your school : ");
    scanf("%s", school);

    printf("Enter the department of your school: ");
    scanf("%s", department);

    printf("Enter username: ");
    scanf("%s", username);

    printf("Enter password: ");
    scanf("%s", password);

    // Convert the school and department input to lowercase
    for (int i = 0; i < strlen(school); i++) {
        school[i] = tolower(school[i]);
    }
    for (int i = 0; i < strlen(department); i++) {
        department[i] = tolower(department[i]);
    }

    // Open the file in the "adminLogin" folder
    FILE *fp = fopen("adminLogin.txt", "r");
    if (fp == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    char validSchool[MAX_LENGTH];
    char validDepartment[MAX_LENGTH];
    char validUsername[MAX_LENGTH];
    char validPassword[MAX_LENGTH];

    // Read all the valid credentials from the file
    while (fscanf(fp, "%s %s %s %s", validSchool, validDepartment, validUsername, validPassword) != EOF) {
        // Convert the valid credentials from the file to lowercase
        for (int i = 0; i < strlen(validSchool); i++) {
            validSchool[i] = tolower(validSchool[i]);
        }
        
        for (int i = 0; i < strlen(validDepartment); i++) {
            validDepartment[i] = tolower(validDepartment[i]);
        }

        // Compare the user's input with the current valid credentials from the file
        // strcasecmp function is used to compare the user's input with the current valid credentials from the file
        if (strcasecmp(school, validSchool) == 0 &&
            strcasecmp(department, validDepartment) == 0 &&
            strcmp(username, validUsername) == 0 &&
            strcmp(password, validPassword) == 0) {
            if (strcmp(validUsername, "admin")==0){
                detectAdmin = 1;
                break;
            }
        }
    }

    if (detectAdmin){
        printf("Logged in as an admin.\n");
        adminMenu();
        main();
    }

    else if (!detectAdmin){
        fclose(fp);
        FILE *fp = fopen("staffLogin.txt", "r");
        if (fp == NULL) {
            printf("Error opening file\n");
            return 1;
        }

        char validSchool[MAX_LENGTH];
        char validDepartment[MAX_LENGTH];
        char validUsername[MAX_LENGTH];
        char validPassword[MAX_LENGTH];

        while (fscanf(fp, "%*s %s %s %*s %s %s", validSchool, validDepartment, validUsername, validPassword) != EOF) {
            // Convert the valid credentials from the file to lowercase
            for (int i = 0; i < strlen(validSchool); i++) {
                validSchool[i] = tolower(validSchool[i]);
            }
            for (int i = 0; i < strlen(validDepartment); i++) {
                validDepartment[i] = tolower(validDepartment[i]);
            }
            if (strcasecmp(school, validSchool) == 0 &&
                strcasecmp(department, validDepartment) == 0 &&
                strcasecmp(username, validUsername) == 0 &&
                strcasecmp(password, validPassword) == 0) {
                detectStaff = 1;
                break;
            }
        }

        if (detectStaff){
            printf("Welcome to the main screen\n");
            printf("You are logged in as a staff. \n");
            staffMenu();
            main();
        }
        else if (!detectStaff){
            fclose(fp);
            // Open the superior login file
            FILE *fp = fopen("superiorLogin.txt", "r");
            if (fp == NULL) {
              printf("Error opening file\n");
              return 1;
            }

            char validSchool[MAX_LENGTH];
            char validDepartment[MAX_LENGTH];
            char validUsername[MAX_LENGTH];
            char validPassword[MAX_LENGTH];

            while (fscanf(fp, "%s %s %s %s", validSchool, validDepartment, validUsername, validPassword) != EOF) {
                // Convert the valid credentials from the file to lowercase
                for (int i = 0; i < strlen(validSchool); i++) {
                    validSchool[i] = tolower(validSchool[i]);
                }
                for (int i = 0; i < strlen(validDepartment); i++) {
                    validDepartment[i] = tolower(validDepartment[i]);
                }
                if (strcasecmp(school, validSchool) == 0 &&
                    strcasecmp(department, validDepartment) == 0 &&
                    strcasecmp(username, validUsername) == 0 &&
                    strcasecmp(password, validPassword) == 0){
                    detectSuperior = 1;
                    break;
                }
            }
            
            if (detectSuperior){
              printf("Welcome to the main screen\n");
              printf("Your are logged in as a superior. \n");
              superiorMenu();
              main();
              
            }
            
            else{
              printf("Wrong information! Please try again!\n");
              main(); 
            }
        }
    }
    
    // Close the file
    fclose(fp);
    return 0;
}


