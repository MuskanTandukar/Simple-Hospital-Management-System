#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#define FILE_NAME "users.txt"
#define MAX_APPOINTMENTS 100

typedef struct {
    char name[50];
} Doctor;

//Patient History
typedef struct {
    char patientName[50];
    char phone[11];  
    char disease[50];
    char time[20];
    char status[20];  
} Appointment;

struct User {
    char id[10];         
    char userType[4];    
    char username[50];
    char password[50];
};

// Enhanced phone validation - must start with 97 or 98 and be exactly 10 digits
int isValidPhoneNumber(const char *phone) {
    if (strlen(phone) != 10) return 0;
    
    // Check if it starts with 97 or 98
    if (!(phone[0] == '9' && (phone[1] == '7' || phone[1] == '8'))) return 0;
    
    // Check if all characters are digits
    for (int i = 0; i < 10; i++) {
        if (!isdigit(phone[i])) return 0;
    }
    return 1;
}

// Home Screen
void displayWelcomeScreen() {
    system("cls");
    printf("========================================================\n");
    printf("           WELCOME TO HOSPITAL MANAGEMENT SYSTEM       \n");
    printf("========================================================\n");
    printf("                    Developed by: Your Team            \n");
    printf("                    Version: 1.0                       \n");
    printf("========================================================\n");
    printf("Features:\n");
    printf("- Patient Appointment Management\n");
    printf("- Doctor Panel with Full Control\n");
    printf("- Admin Panel with User Management\n");
    printf("- Staff Panel for General Operations\n");
    printf("- Enhanced Phone Validation\n");
    printf("- Automatic Login Tracking\n");
    printf("========================================================\n");
    printf("\nPress any key to continue to login...");
    getchar();
    getchar();
}

// Add a new appointment with enhanced phone validation
void addAppointment(Appointment appointments[], int *count) {
    if (*count >= MAX_APPOINTMENTS) {
        printf("Appointment list is full!\n");
        return;
    }

    printf("Enter patient name: ");
    fflush(stdin);
    scanf(" %[^\n]", appointments[*count].patientName);

    do {
        printf("Enter phone number (must start with 97 or 98 and be exactly 10 digits): ");
        fflush(stdin);
        scanf(" %[^\n]", appointments[*count].phone);
        if (!isValidPhoneNumber(appointments[*count].phone)) {
            printf("Invalid phone number! Must start with 97 or 98 and be exactly 10 digits. Try again.\n");
        }
    } while (!isValidPhoneNumber(appointments[*count].phone));

    printf("Enter disease: ");
    fflush(stdin);
    scanf(" %[^\n]", appointments[*count].disease);
    printf("Enter appointment time (e.g., 10:30AM): ");
    fflush(stdin);
    scanf(" %[^\n]", appointments[*count].time);
    strcpy(appointments[*count].status, "Pending");

    (*count)++;
    printf("Appointment added successfully!\n");
}

// Generate a filename based on doctor's name
void getFilename(char doctorName[], char filename[]) {
    strcpy(filename, "dr_");
    for (int i = 0; doctorName[i]; i++) {
        filename[3 + i] = (doctorName[i] == ' ') ? '_' : tolower(doctorName[i]);
    }
    filename[3 + strlen(doctorName)] = '\0';
    strcat(filename, ".txt");
}

// Save all appointments to the doctor's file
void saveAppointments(Appointment appointments[], int count, const char *filename) {
    FILE *fp = fopen(filename, "w");
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s,%s,%s,%s,%s\n", appointments[i].patientName, appointments[i].phone,
                appointments[i].disease, appointments[i].time, appointments[i].status);
    }
    fclose(fp);
}

// Load appointments from file
int loadAppointments(Appointment appointments[], const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;

    int count = 0;
    while (fscanf(fp, "%49[^,],%10[^,],%49[^,],%19[^,],%19[^\n]\n",
                  appointments[count].patientName,
                  appointments[count].phone,
                  appointments[count].disease,
                  appointments[count].time,
                  appointments[count].status) == 5) {
        count++;
    }
    fclose(fp);
    return count;
}

// Display all appointments
void displayAppointments(Appointment appointments[], int count) {
    printf("================================================================================================================\n");
    printf("| %-15s | %-15s | %-15s | %-10s | %-10s |\n", "Patient Name", "Phone", "Disease", "Time", "Status");
    printf("----------------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("| %-15s | %-15s | %-15s | %-10s | %-10s |\n",
               appointments[i].patientName,
               appointments[i].phone,
               appointments[i].disease,
               appointments[i].time,
               appointments[i].status);
    }
    printf("================================================================================================================\n");
}

// Delete an appointment by patient name
void deleteAppointment(Appointment appointments[], int *count) {
    char name[50];
    printf("Enter patient name to delete: ");
    fflush(stdin);
    scanf(" %[^\n]", name);

    for (int i = 0; i < *count; i++) {
        if (strcmp(appointments[i].patientName, name) == 0) {
            for (int j = i; j < *count - 1; j++) {
                appointments[j] = appointments[j + 1];
            }
            (*count)--;
            printf("Appointment deleted.\n");
            return;
        }
    }
    printf("Appointment not found.\n");
}

// Edit an appointment with enhanced phone validation
void editAppointment(Appointment appointments[], int count) {
    char name[50];
    printf("Enter patient name to edit: ");
    fflush(stdin);
    scanf(" %[^\n]", name);

    for (int i = 0; i < count; i++) {
        if (strcmp(appointments[i].patientName, name) == 0) {
            printf("Editing appointment for: %s\n", name);

            do {
                printf("Enter new phone (must start with 97 or 98 and be exactly 10 digits): ");
                fflush(stdin);
                scanf(" %[^\n]", appointments[i].phone);
                if (!isValidPhoneNumber(appointments[i].phone)) {
                    printf("Invalid phone number! Must start with 97 or 98 and be exactly 10 digits. Try again.\n");
                }
            } while (!isValidPhoneNumber(appointments[i].phone));

            printf("Enter new disease: ");
            fflush(stdin);
            scanf(" %[^\n]", appointments[i].disease);
            printf("Enter new time: ");
            fflush(stdin);
            scanf(" %[^\n]", appointments[i].time);
            printf("Enter new status (Pending/Completed/Cancelled): ");
            fflush(stdin);
            scanf(" %[^\n]", appointments[i].status);
            printf("Appointment updated successfully.\n");
            return;
        }
    }
    printf("Appointment not found.\n");
}

// Search for appointments by patient name
void searchAppointment(Appointment appointments[], int count) {
    char name[50];
    printf("Enter patient name to search: ");
    fflush(stdin);
    scanf(" %[^\n]", name);

    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strstr(appointments[i].patientName, name)) {
            printf("Match found: %s, Phone: %s, Disease: %s, Time: %s, Status: %s\n",
                   appointments[i].patientName, appointments[i].phone,
                   appointments[i].disease, appointments[i].time,
                   appointments[i].status);
            found = 1;
        }
    }
    if (!found) printf("No matching appointment found.\n");
}

// Sort appointments by patient name or time
void sortAppointments(Appointment appointments[], int count) {
    int choice;
    printf("Sort by:\n1. Patient Name\n2. Appointment Time\nChoose: ");
    fflush(stdin);
    scanf("%d", &choice);

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            int cmp = (choice == 1) ? strcmp(appointments[j].patientName, appointments[j + 1].patientName)
                                    : strcmp(appointments[j].time, appointments[j + 1].time);
            if (cmp > 0) {
                Appointment temp = appointments[j];
                appointments[j] = appointments[j + 1];
                appointments[j + 1] = temp;
            }
        }
    }
    printf("Appointments sorted.\n");
}

// Doctor Panel
void doctorPanel() {
    Doctor doctor;
    Appointment appointments[MAX_APPOINTMENTS];
    char filename[100];

    printf("==== Doctor Login ====\n");
    printf("Enter your full name: ");
    fflush(stdin);
    scanf(" %[^\n]", doctor.name);

    getFilename(doctor.name, filename);
    int count = loadAppointments(appointments, filename);

    int choice;
    do {
        printf("\n===== Doctor Panel: %s =====\n", doctor.name);
        printf("1. View All Appointments\n");
        printf("2. Add Appointment\n");
        printf("3. Edit Appointment\n");
        printf("4. Delete Appointment\n");
        printf("5. Search Appointment\n");
        printf("6. Sort Appointments\n");
        printf("7. Save & Exit\n");
        printf("Choose an option: ");
        fflush(stdin);
        scanf("%d", &choice);

        switch (choice) {
            case 1: displayAppointments(appointments, count); break;
            case 2: addAppointment(appointments, &count); break;
            case 3: editAppointment(appointments, count); break;
            case 4: deleteAppointment(appointments, &count); break;
            case 5: searchAppointment(appointments, count); break;
            case 6: sortAppointments(appointments, count); break;
            case 7:
                saveAppointments(appointments, count, filename);
                printf("Data saved to '%s'. Exiting...\n", filename);
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 7);
}

// Valid user types
int isValidPrefix(const char *prefix) {
    return (strcmp(prefix, "ADM") == 0 || strcmp(prefix, "DOC") == 0 || strcmp(prefix, "STF") == 0);
}

// Check if ID already exists
int idExists(const char *id) {
    struct User temp;
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) return 0;
    while (fread(&temp, sizeof(struct User), 1, fp)) {
        if (strcmp(temp.id, id) == 0) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

// Get last number for ID generation
int getLastIDNumber(const char *prefix) {
    struct User temp;
    FILE *fp = fopen(FILE_NAME, "rb");
    int max = 0;
    if (!fp) return 0;
    while (fread(&temp, sizeof(struct User), 1, fp)) {
        if (strncmp(temp.id, prefix, 3) == 0) {
            int num = atoi(&temp.id[3]);
            if (num > max)
                max = num;
        }
    }
    fclose(fp);
    return max;
}

// Generate new unique ID
void createID(char *prefix, char *id) {
    int last;
    for (int i = 0; prefix[i]; i++)
        prefix[i] = toupper(prefix[i]);

    if (!isValidPrefix(prefix)) {
        printf("Invalid prefix. Use only ADM, DOC, or STF.\n");
        strcpy(id, "");
        return;
    }

    do {
        last = getLastIDNumber(prefix) + 1;
        sprintf(id, "%s%03d", prefix, last);
    } while (idExists(id));
}

// Generate automatic username and password
void generateCredentials(const char *userType, const char *id, char *username, char *password) {
    // Generate username: usertype + last 3 digits of ID
    sprintf(username, "%s%s", userType, &id[3]);
    
    // Generate password: ID + "2024"
    sprintf(password, "%s2024", id);
}

// Automatically register users (for admin use)
void autoRegisterUser() {
    struct User user;
    FILE *fp = fopen(FILE_NAME, "ab");

    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    printf("\n--- Automatic User Registration ---\n");
    printf("Select user type:\n");
    printf("1. Admin (ADM)\n");
    printf("2. Doctor (DOC)\n");
    printf("3. Staff (STF)\n");
    printf("Choose: ");
    
    int choice;
    scanf("%d", &choice);
    
    switch(choice) {
        case 1: strcpy(user.userType, "ADM"); break;
        case 2: strcpy(user.userType, "DOC"); break;
        case 3: strcpy(user.userType, "STF"); break;
        default: 
            printf("Invalid choice!\n");
            fclose(fp);
            return;
    }

    createID(user.userType, user.id);
    if (strlen(user.id) == 0) {
        fclose(fp);
        return;
    }
    
    generateCredentials(user.userType, user.id, user.username, user.password);

    printf("User created successfully!\n");
    printf("ID: %s\n", user.id);
    printf("Username: %s\n", user.username);
    printf("Password: %s\n", user.password);
    printf("Please note down these credentials!\n");

    fwrite(&user, sizeof(struct User), 1, fp);
    fclose(fp);
}

// Manual register (old method)
void registerUser() {
    struct User user;
    FILE *fp = fopen(FILE_NAME, "ab");

    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    printf("\n--- Manual User Registration ---\n");
    printf("Enter user type prefix (ADM / DOC / STF): ");
    scanf("%s", user.userType);

    createID(user.userType, user.id);
    if (strlen(user.id) == 0) {
        fclose(fp);
        return;
    }

    printf("Assigned ID: %s\n", user.id);
    printf("Enter username: ");
    scanf("%s", user.username);
    printf("Enter password: ");
    scanf("%s", user.password);

    fwrite(&user, sizeof(struct User), 1, fp);
    fclose(fp);
    printf("Registration successful!\n");
}

// Display all users
void displayUsers() {
    struct User user;
    FILE *fp = fopen(FILE_NAME, "rb");

    if (!fp) {
        printf("No users found.\n");
        return;
    }

    printf("\n--- Registered Users ---\n");
    printf("%-10s %-5s %-15s %-15s\n", "User ID", "Type", "Username", "Password");
    printf("--------------------------------------------------------\n");

    while (fread(&user, sizeof(struct User), 1, fp)) {
        printf("%-10s %-5s %-15s %-15s\n", user.id, user.userType, user.username, user.password);
    }

    fclose(fp);
}

// Delete user by ID
void deleteUser() {
    char targetID[10];
    struct User user;
    int found = 0;

    printf("\n--- Delete User ---\n");
    printf("Enter the User ID to delete: ");
    scanf("%s", targetID);

    FILE *fp = fopen(FILE_NAME, "rb");
    FILE *temp = fopen("temp.txt", "wb");

    if (!fp || !temp) {
        printf("File error!\n");
        return;
    }

    while (fread(&user, sizeof(struct User), 1, fp)) {
        if (strcmp(user.id, targetID) == 0) {
            found = 1;
            continue;
        }
        fwrite(&user, sizeof(struct User), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);

    if (found)
        printf("User with ID %s deleted successfully.\n", targetID);
    else
        printf("User ID not found.\n");
}

// Login with time logging
struct User loginUser() {
    struct User input, temp;
    int found = 0;

    FILE *fp = fopen(FILE_NAME, "rb");

    if (!fp) {
        printf("No registered users found. Creating default admin...\n");
        // Create default admin
        struct User defaultAdmin = {"ADM001", "ADM", "admin", "admin123"};
        FILE *newFile = fopen(FILE_NAME, "wb");
        fwrite(&defaultAdmin, sizeof(struct User), 1, newFile);
        fclose(newFile);
        printf("Default admin created - ID: ADM001, Username: admin, Password: admin123\n");
        fp = fopen(FILE_NAME, "rb");
    }

    printf("\n--- User Login ---\n");
    printf("Enter User ID: ");
    scanf("%s", input.id);
    printf("Enter password: ");
    scanf("%s", input.password);

    while (fread(&temp, sizeof(struct User), 1, fp)) {
        if (strcmp(temp.id, input.id) == 0 && strcmp(temp.password, input.password) == 0) {
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (found) {
        system("cls");
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        char loginTime[30];
        strftime(loginTime, sizeof(loginTime), "%Y-%m-%d %H:%M:%S", t);

        printf("\nLogin successful! Welcome, %s (ID: %s)\n", temp.username, temp.id);
        printf("Logged in at: %s\n", loginTime);

        FILE *log = fopen("login_log.txt", "a");
        if (log) {
            fprintf(log, "%s (%s) logged in at %s\n", temp.id, temp.username, loginTime);
            fclose(log);
        }

        return temp;
    } else {
        printf("Invalid ID or password.\n");
        exit(0);
    }
}

// User Management Menu
void userManagementMenu() {
    int choice;
    char c;

    do {
        system("cls");
        printf("\n=== User Management System ===\n");
        printf("1. Auto Register User (Recommended)\n");
        printf("2. Manual Register User\n");
        printf("3. Show Registered Users\n");
        printf("4. Delete User\n");
        printf("5. Exit to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: autoRegisterUser(); break;
            case 2: registerUser(); break;
            case 3: displayUsers(); break;
            case 4: deleteUser(); break;
            case 5: return;
            default: printf("Invalid choice. Try again.\n"); break;
        }

        if (choice != 5) {
            printf("\nDo you want to go back to User Management Menu? (y/n): ");
            scanf(" %c", &c);
        }

    } while (choice != 5 && (c == 'y' || c == 'Y'));
}

// Patient Management for Admin
void appointmentManager(char doctorName[]) {
    Appointment appointments[MAX_APPOINTMENTS];
    char filename[100];
    getFilename(doctorName, filename);
    int count = loadAppointments(appointments, filename);
    int choice;

    do {
        printf("\n===== Patient Management: %s =====\n", doctorName);
        printf("1. View All Appointments\n");
        printf("2. Add Appointment\n");
        printf("3. Edit Appointment\n");
        printf("4. Delete Appointment\n");
        printf("5. Search Appointment\n");
        printf("6. Sort Appointments\n");
        printf("7. Save & Exit\n");
        printf("Choose an option: ");
        fflush(stdin);
        scanf("%d", &choice);

        switch (choice) {
            case 1: displayAppointments(appointments, count); break;
            case 2: addAppointment(appointments, &count); break;
            case 3: editAppointment(appointments, count); break;
            case 4: deleteAppointment(appointments, &count); break;
            case 5: searchAppointment(appointments, count); break;
            case 6: sortAppointments(appointments, count); break;
            case 7:
                saveAppointments(appointments, count, filename);
                printf("Data saved to '%s'. Exiting...\n", filename);
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 7);
}

// Staff Panel
void staffPanel(struct User user) {
    int choice;
    
    do {
        printf("\n=== Staff Panel: %s ===\n", user.username);
        printf("1. View Appointment Records\n");
        printf("2. Search Patient Records\n");
        printf("3. View Login Times\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                char drName[50];
                printf("Enter Doctor Name to view records: ");
                fflush(stdin);
                scanf(" %[^\n]", drName);
                
                char filename[100];
                getFilename(drName, filename);
                Appointment appointments[MAX_APPOINTMENTS];
                int count = loadAppointments(appointments, filename);
                displayAppointments(appointments, count);
                break;
            }
            case 2: {
                char drName[50];
                printf("Enter Doctor Name for patient search: ");
                fflush(stdin);
                scanf(" %[^\n]", drName);
                
                char filename[100];
                getFilename(drName, filename);
                Appointment appointments[MAX_APPOINTMENTS];
                int count = loadAppointments(appointments, filename);
                searchAppointment(appointments, count);
                break;
            }
            case 3:
                printf("\nOpening login records...\n");
                system("notepad login_log.txt");
                break;
            case 4:
                printf("Exiting staff panel...\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 4);
}

// Main menu
int main() {
    displayWelcomeScreen();
    
    struct User loggedInUser = loginUser();  // Login at the start

    if (strncmp(loggedInUser.id, "DOC", 3) == 0) {
        doctorPanel();  // Doctor Panel
    } else if (strncmp(loggedInUser.id, "ADM", 3) == 0) {
        int choice;
        while (1) {
            printf("\n=== Admin Management Panel ===\n");
            printf("1. User Management System\n");
            printf("2. Patient Record Management\n");
            printf("3. View Login Times\n");
            printf("4. System Statistics\n");
            printf("5. Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1: userManagementMenu(); break;
                case 2: {
                    char drName[50];
                    printf("Enter Doctor Name to access Patient Records: ");
                    fflush(stdin);
                    scanf(" %[^\n]", drName);
                    appointmentManager(drName);
                    break;
                }
                case 3: 
                    printf("\nOpening login records...\n");
                    system("notepad login_log.txt");
                    break;
                case 4:
                    displayUsers();
                    break;
                case 5: 
                    printf("Exiting program. Goodbye!\n"); 
                    return 0;
                default: 
                    printf("Invalid choice. Try again.\n"); 
                    break;
            }
        }
    } else if (strncmp(loggedInUser.id, "STF", 3) == 0) {
        staffPanel(loggedInUser);  // Staff Panel
    } else {
        printf("Unauthorized user type. Contact admin.\n");
    }

    return 0;
}