# Password-Manager-Project-in-C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    if (file != NULL) {
        fwrite(accounts, sizeof(struct Account), numAccounts, file);
        fclose(file);
    } else {
        printf("Error saving accounts to file.\n");
        exit(1);
    }
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
    scanf("%s", masterPassword);

    // Encrypt the master password
    xorCrypt(masterPassword, strlen(masterPassword));
}

// Function to check if the entered master password is correct
int checkMasterPassword() {
    printf("Enter Master Password: ");
    char enteredPassword[MAX_PASSWORD_LENGTH];
    scanf("%s", enteredPassword);

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

    int authenticated = 0;

    while (!authenticated) {
        if (checkMasterPassword()) {
            printf("Authentication successful!\n");
            authenticated = 1;
        } else {
            printf("Incorrect master password. Please try again.\n");
        }
    }
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
    struct Account newAccount;

    printf("Enter Username: ");
    scanf("%s", newAccount.username);

    printf("Enter Password: ");
    scanf("%s", newAccount.password);
    xorCrypt(newAccount.password, strlen(newAccount.password));

    printf("Enter Website: ");
    scanf("%s", newAccount.website);

    printf("Enter Notes: ");
    scanf("%s", newAccount.notes);

    passwordStrengthIndicator(newAccount.password);

    accounts[numAccounts++] = newAccount;
    printf("Account added successfully!\n");

    saveAccountsToFile();
}

void retrievePassword() {
    char searchUsername[MAX_USERNAME_LENGTH];
    printf("Enter Username for the account: ");
    scanf("%s", searchUsername);

    int found = 0;
    for (int i = 0; i < numAccounts; i++) {
        if (strcmp(accounts[i].username, searchUsername) == 0) {
            // Decrypt the password before displaying it
            xorCrypt(accounts[i].password, strlen(accounts[i].password));

            printf("Password: %s\n", accounts[i].password);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Account not found.\n");
    }
}

void updateAccount() {
    char updateUsername[MAX_USERNAME_LENGTH];
    printf("Enter Username for the account to update: ");
    scanf("%s", updateUsername);

    int found = 0;
    for (int i = 0; i < numAccounts; i++) {
        if (strcmp(accounts[i].username, updateUsername) == 0) {
            printf("Enter new Password: ");
            scanf("%s", accounts[i].password);
            xorCrypt(accounts[i].password, strlen(accounts[i].password));

            printf("Account updated successfully!\n");
            saveAccountsToFile();
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Account not found.\n");
    }
}

void deleteAccount() {
    char deleteUsername[MAX_USERNAME_LENGTH];
    printf("Enter Username for the account to delete: ");
    scanf("%s", deleteUsername);

    int found = 0;
    for (int i = 0; i < numAccounts; i++) {
        if (strcmp(accounts[i].username, deleteUsername) == 0) {
            // Shift accounts to remove the deleted account
            for (int j = i; j < numAccounts - 1; j++) {
                accounts[j] = accounts[j + 1];
            }

            numAccounts--;

            printf("Account deleted successfully!\n");
            saveAccountsToFile();
            found = 1;
                  }
              }

              if (!found) {
                  printf("Account not found.\n");
              }
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
                  scanf("%d", &choice);

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
                  }
              } while (choice != 6);

              printf("Exiting the Password Manager. Goodbye!\n");

              return 0;
          }
