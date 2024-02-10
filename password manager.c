#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ACCOUNTS 100
#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50
#define MAX_WEBSITE_LENGTH 50
#define MAX_NOTES_LENGTH 100
#define ENCRYPTION_KEY 0x1F // Simple XOR encryption key

struct Account {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    char website[MAX_WEBSITE_LENGTH];
    char notes[MAX_NOTES_LENGTH];
};

struct Account accounts[MAX_ACCOUNTS];
int numAccounts = 0;
char masterPassword[MAX_PASSWORD_LENGTH] = ""; // Declaration for master password

// Function to encrypt or decrypt a string using XOR
void xorCrypt(char *input, size_t length) {
    for (size_t i = 0; i < length; i++) {
        input[i] ^= ENCRYPTION_KEY;
    }
}

// Function to save accounts to a file
void saveAccountsToFile() {
    FILE *file = fopen("passwords.dat", "wb");
    if (file == NULL) {
        perror("Error saving accounts to file");
        exit(EXIT_FAILURE);
    }
    fwrite(accounts, sizeof(struct Account), numAccounts, file);
    fclose(file);
}

// Function to load accounts from a file
void loadAccountsFromFile() {
    FILE *file = fopen("passwords.dat", "rb");
    if (file != NULL) {
        numAccounts = fread(accounts, sizeof(struct Account), MAX_ACCOUNTS, file);
        fclose(file);
    }
}

// Function to set the master password
void setMasterPassword() {
    printf("Set your master password: ");
    if (scanf("%49s", masterPassword) != 1) {
        fprintf(stderr, "Error reading master password\n");
        exit(EXIT_FAILURE);
    }

    // Encrypt the master password
    xorCrypt(masterPassword, strlen(masterPassword));
}

// Function to check if the entered master password is correct
bool checkMasterPassword() {
    printf("Enter Master Password: ");
    char enteredPassword[MAX_PASSWORD_LENGTH];
    if (scanf("%49s", enteredPassword) != 1) {
        fprintf(stderr, "Error reading master password\n");
        exit(EXIT_FAILURE);
    }

    // Encrypt the entered master password for comparison
    xorCrypt(enteredPassword, strlen(enteredPassword));

    return strcmp(enteredPassword, masterPassword) == 0;
}

void authenticate() {
    printf("Welcome to the Password Manager!\n");

    // If master password is not set, ask the user to set it
    if (strlen(masterPassword) == 0) {
        setMasterPassword();
    }

    while (!checkMasterPassword()) {
        printf("Incorrect master password. Please try again.\n");
    }
    printf("Authentication successful!\n");
}

void displayMenu() {
    printf("\nPassword Manager Menu:\n");
    printf("1. Add Account\n");
    printf("2. Retrieve Password\n");
    printf("3. Update Account\n");
    printf("4. Delete Account\n");
    printf("5. Display All Accounts\n");
    printf("6. Exit\n");
}

// Function to generate a simple password strength indicator
void passwordStrengthIndicator(const char *password) {
    int length = strlen(password);
    printf("Password Strength: ");

    if (length < 8) {
        printf("Weak\n");
    } else if (length < 12) {
        printf("Moderate\n");
    } else {
        printf("Strong\n");
    }
}

void addAccount() {
    if (numAccounts >= MAX_ACCOUNTS) {
        printf("Cannot add more accounts. Maximum limit reached.\n");
        return;
    }

    struct Account newAccount;

    printf("Enter Username: ");
    if (scanf("%49s", newAccount.username) != 1) {
        fprintf(stderr, "Error reading username\n");
        return;
    }

    printf("Enter Password: ");
    if (scanf("%49s", newAccount.password) != 1) {
        fprintf(stderr, "Error reading password\n");
        return;
    }
    xorCrypt(newAccount.password, strlen(newAccount.password));

    printf("Enter Website: ");
    if (scanf("%49s", newAccount.website) != 1) {
        fprintf(stderr, "Error reading website\n");
        return;
    }

    printf("Enter Notes: ");
    if (scanf("%99s", newAccount.notes) != 1) {
        fprintf(stderr, "Error reading notes\n");
        return;
    }

    passwordStrengthIndicator(newAccount.password);

    accounts[numAccounts++] = newAccount;
    printf("Account added successfully!\n");

    saveAccountsToFile();
}

void retrievePassword() {
    char searchUsername[MAX_USERNAME_LENGTH];
    printf("Enter Username for the account: ");
    if (scanf("%49s", searchUsername) != 1) {
        fprintf(stderr, "Error reading username\n");
        return;
    }

    for (int i = 0; i < numAccounts; i++) {
        if (strcmp(accounts[i].username, searchUsername) == 0) {
            // Decrypt the password before displaying it
            xorCrypt(accounts[i].password, strlen(accounts[i].password));
            printf("Password: %s\n", accounts[i].password);
            return;
        }
    }

    printf("Account not found.\n");
}

void updateAccount() {
    char updateUsername[MAX_USERNAME_LENGTH];
    printf("Enter Username for the account to update: ");
    if (scanf("%49s", updateUsername) != 1) {
        fprintf(stderr, "Error reading username\n");
        return;
    }

    for (int i = 0; i < numAccounts; i++) {
        if (strcmp(accounts[i].username, updateUsername) == 0) {
            printf("Enter new Password: ");
            if (scanf("%49s", accounts[i].password) != 1) {
                fprintf(stderr, "Error reading password\n");
                return;
            }
            xorCrypt(accounts[i].password, strlen(accounts[i].password));

            printf("Account updated successfully!\n");
            saveAccountsToFile();
            return;
        }
    }

    printf("Account not found.\n");
}

void deleteAccount() {
    char deleteUsername[MAX_USERNAME_LENGTH];
    printf("Enter Username for the account to delete: ");
    if (scanf("%49s", deleteUsername) != 1) {
        fprintf(stderr, "Error reading username\n");
        return;
    }

    for (int i = 0; i < numAccounts; i++) {
        if (strcmp(accounts[i].username, deleteUsername) == 0) {
            // Shift accounts to remove the deleted account
            for (int j = i; j < numAccounts - 1; j++) {
                accounts[j] = accounts[j + 1];
            }

            numAccounts--;

            printf("Account deleted successfully!\n");
            saveAccountsToFile();
            return;
        }
    }

    printf("Account not found.\n");
}

void displayAllAccounts() {
    if (numAccounts > 0) {
        printf("\nAll Accounts:\n");
        for (int i = 0; i < numAccounts; i++) {
            printf("Username: %s\n", accounts[i].username);
            printf("Password: %s\n", accounts[i].password); // Passwords are decrypted for display
            printf("Website: %s\n", accounts[i].website);
            printf("Notes: %s\n", accounts[i].notes);
            printf("------------------------\n");
        }
    } else {
        printf("No accounts to display.\n");
    }
}

int main() {
    loadAccountsFromFile();

    authenticate();

    int choice;
    do {
        displayMenu();
        printf("Enter your choice (1-6): ");
        if (scanf("%d", &choice) != 1) {
            fprintf(stderr, "Error reading choice\n");
            return EXIT_FAILURE;
        }

        switch (choice) {
            case 1:
                addAccount();
                break;
            case 2:
                retrievePassword();
                break;
            case 3:
                updateAccount();
                break;
            case 4:
                deleteAccount();
                break;
            case 5:
                displayAllAccounts();
                break;
            case 6:
                printf("Exiting the Password Manager. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 6.\n");
        }
    } while (choice != 6);

    return EXIT_SUCCESS;
}
