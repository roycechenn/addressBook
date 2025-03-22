#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct contact {
    char *firstName; // 4 byte ptr to fN theres no mem for the actual value
    char *familyName;
    long long phoneNum; /* 10-digit phone number stored as a 64-bit integer */
    char *address;
    int age;
} Contact;

int countContacts(Contact **contacts);
Contact *readNewContact();
Contact **appendContact(Contact **contacts, Contact *newContact);
Contact **insertContactAlphabetical(Contact **contacts, Contact *newContact);
Contact **removeContactByIndexHelper(Contact **contacts, int index);
Contact **removeContactByIndex(Contact **contacts);
int removeContactByFullName(Contact ***contacts);
void listContacts(Contact **contacts);
void saveContactsToFile(Contact **contacts, char *filename);
void printContactsToFile(Contact **contacts, char *filename);
Contact **loadContactsFromFile(Contact **addressBook, char *filename);
Contact **appendContactsFromFile(Contact **contacts, char *filename);
Contact **mergeContactsFromFile(Contact **contacts, char *filename);
Contact **editContact(Contact **contacts);
void freeContact(Contact *contact);
void freeContactList(Contact **contacts);

int main() {
    int choice = 0;
    char file[25];

    Contact **contacts = malloc(sizeof(Contact *)); // ptr to array of ptr to structs and function outputs ptr to struct

    if (contacts == NULL) {
        printf("Error: unable to allocation memory for addressBook");
        return 1;
    }
    contacts[0] = NULL;
    while (1) {
        printf("\nAddress Book Menu\n"
               "1.  Append Contact\n"
               "2.  Insert Contact in Alphabetical Order\n"
               "3.  Remove Contact by Index\n"
               "4.  Remove Contact by Full Name\n"
               "5.  Find and Edit Contact\n"
               "6.  List Contacts\n"
               "7.  Print Contacts to File with the format of an input file\n"
               "8.  Print Contacts to File (Human Readable)\n"
               "9.  Load Contacts from File Replacing Existing Contacts\n"
               "10. Append Contacts from File\n"
               "11. Merge Contacts from File\n"
               "12. Exit\n"
               "Choose an option: ");
        scanf("%d", &choice);
        if (choice > 12 || choice < 1) {
            printf("\nError: Invalid option\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');
        printf("\n");

        switch (choice) {
        case 1: {
            Contact *c = readNewContact();
            if (c != NULL) {
                contacts = appendContact(contacts, c);
            }
            break;
        }
        case 2: {
            Contact *c = readNewContact();
            if (c != NULL) {
                contacts = insertContactAlphabetical(contacts, c);
                break;
            }
            break;
        }
        case 3:
            contacts = removeContactByIndex(contacts);
            break;

        case 4:
            removeContactByFullName(&contacts);
            break;

        case 5:
            contacts = editContact(contacts);
            break;

        case 6:
            listContacts(contacts);
            break;

        case 7:
            printf("Enter filename: ");
            fgets(file, 25, stdin);
            file[strcspn(file, "\n")] = 0;
            printf("\n");
            saveContactsToFile(contacts, file);
            break;

        case 8:
            printf("Enter filename: ");
            fgets(file, 25, stdin);
            file[strcspn(file, "\n")] = 0;
            printf("\n");
            printContactsToFile(contacts, file);
            break;

        case 9:
            printf("Enter filename: ");
            fgets(file, 25, stdin);
            file[strcspn(file, "\n")] = 0;
            printf("\n");
            contacts = loadContactsFromFile(contacts, file);
            break;

        case 10:
            printf("Enter filename: ");
            fgets(file, 25, stdin);
            file[strcspn(file, "\n")] = 0;
            printf("\n");
            contacts = appendContactsFromFile(contacts, file);
            break;

        case 11:
            printf("Enter filename: ");
            fgets(file, 25, stdin);
            file[strcspn(file, "\n")] = 0;
            printf("\n");
            contacts = mergeContactsFromFile(contacts, file);
            break;

        case 12: // done
            freeContactList(contacts);
            printf("Exiting program. Goodbye!\n");
            return 0;
        default:
            printf("Invalid option\n");
            return 1;
        }
    }

    return 0;
}
// done
int countContacts(Contact **contacts) {
    if (contacts == NULL) {
        return 0;
    }
    int count = 0;
    while (contacts[count++] != NULL) {
    }
    return count - 1;
}

// done
Contact *readNewContact() { // option 1, returns a ptr to Contact

    Contact *c = malloc(sizeof(Contact)); // creates space for Contact
    if (c == NULL) {
        printf("Error: Memory allocation failed for Contact in readNewContact\n");
        return NULL;
    }

    char buffer[100];

    printf("Enter the first name: ");
    if (!fgets(buffer, 100, stdin)) {
        printf("Error: Unable to read first name\n");
        free(c);
        return NULL;
    }
    buffer[strcspn(buffer, "\n")] = 0;
    c->firstName = strdup(buffer);
    if (!c->firstName) {
        printf("Error: unable to allocate memory for the first name string\n");
        free(c);
        return NULL;
    }

    printf("Enter the family name: ");
    if (!fgets(buffer, 100, stdin)) {
        printf("Error: Unable to read family name\n");
        free(c->firstName);
        free(c);
        return NULL;
    }
    buffer[strcspn(buffer, "\n")] = 0;
    c->familyName = strdup(buffer);
    if (!c->familyName) {
        printf("Error: unable to allocate memory for the family name string\n");
        free(c->firstName);
        free(c);
        return NULL;
    }

    printf("Enter the address: ");
    if (!fgets(buffer, 100, stdin)) {
        printf("Error: Unable to read address\n");
        free(c->familyName);
        free(c->firstName);
        free(c);
        return NULL;
    }
    buffer[strcspn(buffer, "\n")] = 0;
    c->address = strdup(buffer);
    if (!c->address) {
        printf("Error: unable to allocate memory for the address string\n");
        free(c->familyName);
        free(c->firstName);
        free(c);
        return NULL;
    }

    int count = 0;
    printf("Enter 10-digit phone number that must not start with 0: ");
    while (count < 5) {
        if (scanf("%lld", &c->phoneNum) == 1 && c->phoneNum >= 1000000000 && c->phoneNum <= 9999999999) {
            break;
        } else {
            printf("Error: Invalid phone number. Try again: ");
            while (getchar() != '\n');
            count++;
        }
    }
    if (count == 5) {
        printf("Error: Could not read a valid phone number");
        c->phoneNum = 0;
    }
    while (getchar() != '\n');

    count = 0;
    printf("Enter the age: ");
    while (count < 5) {
        if (scanf("%d", &c->age) == 1 && c->age >= 1 && c->age <= 150) {
            break;
        } else {
            printf("\nError: Invalid age. Try again: ");
            while (getchar() != '\n');
            count++;
        }
    }
    if (count == 5) {
        printf("Error: Could not read a valid age");
        c->age = 0;
    }
    while (getchar() != '\n');

    return c;
}
// done
Contact **appendContact(Contact **contacts, Contact *newContact) {
    if (newContact == NULL) { // checks if newContact is not valid, eg. failed allocation in readNewContact
        return contacts;      // nothing happens when contacts is returned basically 1 = 1
    }

    int count = countContacts(contacts);
    Contact **updContact = realloc(contacts, sizeof(Contact *) * (count + 2)); // one for newContact and null
    if (updContact == NULL) {
        printf("Memory reallocation error in appendContact\n");
        return contacts; // if it fails, it just returns the original contacts, nothing changes
    }

    updContact[count] = newContact;
    updContact[count + 1] = NULL;
    printf("Contact appended successfully by appendContact\n");
    return updContact;
}
// done
Contact **insertContactAlphabetical(Contact **contacts, Contact *newContact) {
    if (newContact == NULL) {
        return contacts;
    }

    int count = countContacts(contacts);
    Contact **sortArr = realloc(contacts, sizeof(Contact *) * (count + 2));
    if (sortArr == NULL) {
        printf("Memory reallocation error in insertContactAlphabetical\n");
        return contacts;
    }

    int i;
    for (i = 0; i < count; i++) {
        int compare = strcmp(sortArr[i]->familyName, newContact->familyName);
        if (compare > 0 || (compare == 0 && strcmp(sortArr[i]->firstName, newContact->firstName) > 0)) {
            break; // stops at the correct spot, then append to sortArr
        }
    }

    for (int j = count; j > i; j--) { // shift everything after i left to make space for newContact
        sortArr[j] = sortArr[j - 1];
    }
    sortArr[i] = newContact;
    sortArr[count + 1] = NULL;
    printf("Contact was successfully added in alphabetical order\n");
    return sortArr;
}
// done
Contact **removeContactByIndexHelper(Contact **contacts, int index) {
    int count = countContacts(contacts);
    if (index < 0 || index >= count) {
        printf("Error: Index out of range in removeContactByIndexHelper\n");
        return contacts;
    }
    Contact **removeArr = realloc(contacts, sizeof(Contact *) * (count));
    if (removeArr == NULL) {
        printf("Error: Memory reallocation failed in removeContactByIndexHelper\n");
        return contacts;
    }

    free(contacts[index]->firstName);
    free(contacts[index]->familyName);
    free(contacts[index]->address);
    free(contacts[index]);

    for (int i = index; i < count - 1; i++) { // shift left
        removeArr[i] = removeArr[i + 1];
    }
    removeArr[count - 1] = NULL;
    return removeArr;
}
// done
Contact **removeContactByIndex(Contact **contacts) {

    int removeIndex = 0;
    if (!contacts) {
        printf("Error: value of addressBook received in removeContactByIndex was NULL\n");
        return NULL;
    }
    printf("Removing a Contact by index\nEnter index to remove (0 based): ");
    if (scanf("%d", &removeIndex) == 1) {
        printf("Contact removed successfully.\n");
        return removeContactByIndexHelper(contacts, removeIndex);
    } else {
        printf("Error: Value of index supplied could not be read.\n");
    }
    while (getchar() != '\n');
    return contacts;
}

// done
int removeContactByFullName(Contact ***contacts) {

    if (contacts == NULL) {
        printf("Error: value of contacts received in removeContactByFullName was NULL\n");
        return 0;
    }

    char first[25];
    char last[25];
    int count = countContacts(*contacts);

    printf("Enter first name: ");
    fgets(first, 24, stdin);
    first[strcspn(first, "\n")] = 0;

    printf("Enter family name: ");
    fgets(last, 24, stdin);
    last[strcspn(last, "\n")] = 0;

    for (int i = 0; i < count; i++) {
        if (strcmp((*contacts)[i]->firstName, first) == 0 && strcmp((*contacts)[i]->familyName, last) == 0) {
            // free memory then realloc

            freeContact((*contacts)[i]);

            // shift everyhing to the left, one free contact space
            for (int j = i; j < count - 1; j++) {
                (*contacts)[j] = (*contacts)[j + 1];
            }

            (*contacts)[count - 1] = NULL;

            Contact **temp = realloc(*contacts, count * sizeof(Contact *)); // what if the size is 1?
            if (temp == NULL) {
                printf("Error: Memory reallocation failed in removeContactByFullName\n");
                return 0;
            }

            *contacts = temp; // *contacts dereference into **contact (which is the main contact) = temp
            printf("Contact '%s %s' removed successfully\n", first, last);
            return 1;
        }
    }
    printf("Cobntact '%s %s' not found\n", first, last);
    return 2;
}

// done
void listContacts(Contact **contacts) {
    if (contacts == NULL) {
        return;
    }
    int count = countContacts(contacts);
    if (count == 0) {
        printf("Error: No contacts available.\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        printf("%d. %s %s\n", i + 1, contacts[i]->firstName, contacts[i]->familyName);
        printf("   Phone: %lld\n", contacts[i]->phoneNum);
        printf("   Address: %s\n", contacts[i]->address);
        printf("   Age:  %d\n", contacts[i]->age);
    }
}

// done
void saveContactsToFile(Contact **contacts, char *filename) {
    if (filename == NULL) {
        printf("Error: filename formal parameter passed value NULL in saveContactsToFile\n");
        return;
    }
    if (contacts == NULL) {
        printf("Error: contacts formal parameter passed value NULL in saveContactsToFile\n");
        return;
    }

    int count = countContacts(contacts);
    FILE *fileptr = fopen(filename, "w");
    if (fileptr == NULL) {
        printf("Error: file not opened in saveContactsToFile");
        return;
    }
    fprintf(fileptr, "%d\n", count);
    for (int i = 0; i < count; i++) {
        fprintf(fileptr, "%s\n", contacts[i]->firstName);
        fprintf(fileptr, "%s\n", contacts[i]->familyName);
        fprintf(fileptr, "%s\n", contacts[i]->address);
        fprintf(fileptr, "%lld\n", contacts[i]->phoneNum);
        fprintf(fileptr, "%d\n", contacts[i]->age);
    }
    fclose(fileptr);
}

// done
void printContactsToFile(Contact **contacts, char *filename) {
    if (filename == NULL) {
        printf("Error: filename formal parameter passed value NULL in printContactsToFile\n");
        return;
    }
    if (contacts == NULL) {
        printf("Error: contacts formal parameter passed value NULL in printContactsToFile\n");
        return;
    }

    int count = countContacts(contacts);
    FILE *fileptr = fopen(filename, "w");
    if (fileptr == NULL) {
        printf("Error: file not opened in printContactsToFile");
        return;
    }
    fprintf(fileptr, "Address Book report\n-------------------\n");
    for (int i = 0; i < count; i++) {
        fprintf(fileptr, "%d. %s %s\n", i + 1, contacts[i]->firstName, contacts[i]->familyName);
        fprintf(fileptr, "   Phone: %lld\n", contacts[i]->phoneNum);
        fprintf(fileptr, "   Address: %s\n", contacts[i]->address);
        fprintf(fileptr, "   Age:  %d\n", contacts[i]->age);
    }
    fprintf(fileptr, "------------------- \nTotal Contacts: %d\n", count);
    fclose(fileptr);
}

// done
Contact **loadContactsFromFile(Contact **addressBook, char *filename) {
    FILE *fileptr = fopen(filename, "r");
    if (fileptr == NULL) {
        printf("Error: File to load not found\n");
        return NULL;
    }

    int count = 0;

    if (fscanf(fileptr, "%d\n", &count) != 1) { // next read at name
        while (getchar() != '\n');
        fclose(fileptr);
        printf("Error: Memory allocation error, addressBook in loadContactsFromFile\n");
        return NULL;
    }

    // free old data
    if (addressBook) {
        freeContactList(addressBook);
    }

    // malloc space and insert data
    Contact **newArr = malloc(sizeof(Contact *) * (count + 1));
    if (newArr == NULL) {
        printf("Error: Memory allocation error, addressBook in loadContactsFromFile\n");
        fclose(fileptr);
        return NULL;
    }

    char buffer[100];
    for (int i = 0; i < count; i++) {
        Contact *contact = malloc(sizeof(Contact));
        if (contact == NULL) {
            printf("Error: Memory allocation error, Contact %d in loadContactsFromFile", i); // 0 index
            freeContactList(newArr);
            fclose(fileptr);
            return NULL;
        }
        fgets(buffer, 100, fileptr);
        buffer[strcspn(buffer, "\n")] = 0; // sets the last letter to /0
        contact->firstName = strdup(buffer);
        fgets(buffer, 100, fileptr);
        buffer[strcspn(buffer, "\n")] = 0;
        contact->familyName = strdup(buffer);
        fgets(buffer, 100, fileptr);
        buffer[strcspn(buffer, "\n")] = 0;
        contact->address = strdup(buffer);
        fscanf(fileptr, "%lld\n", &contact->phoneNum);
        fscanf(fileptr, "%d\n", &contact->age);

        newArr[i] = contact;
    }
    newArr[count] = NULL;
    fclose(fileptr);
    printf("Contacts loaded from file: %s\n", filename); // its not printing this, how do i debug
    return newArr;
}

// done
Contact **appendContactsFromFile(Contact **contacts, char *filename) {
    FILE *fileptr = fopen(filename, "r");
    if (fileptr == NULL) {
        printf("Error: File to load not found\n");
        return contacts;
    }

    int fileCount;
    if (fscanf(fileptr, "%d\n", &fileCount) != 1) { // next read at name
        fclose(fileptr);
        printf("Error: Could not read number of Contacts correctly\n");
        return contacts;
    }

    char buffer[100];
    for (int i = 0; i < fileCount; i++) {
        Contact *contact = malloc(sizeof(Contact));
        if (contact == NULL) {
            printf("Error: Memory allocation error, Contact %d in appendContactsFromFile", i); // 0 index
            continue;
        }

        // firstname
        if (!fgets(buffer, 100, fileptr)) {
            free(contact);
            continue;
        }
        buffer[strcspn(buffer, "\n")] = 0;
        contact->firstName = strdup(buffer);
        if (!contact->firstName) {
            printf("Error: Memory allocation error, memory for string in Contact %d not allocated\n", i);
            free(contact);
            continue;
        }

        // familyname
        if (!fgets(buffer, 100, fileptr)) {
            freeContact(contact);
            continue;
        }
        buffer[strcspn(buffer, "\n")] = 0;
        contact->familyName = strdup(buffer);
        if (!contact->familyName) {
            printf("Error: Memory allocation error, memory for string in Contact %d not allocated\n", i);
            freeContact(contact);
            continue;
        }
        // address
        if (!fgets(buffer, 100, fileptr)) {
            freeContact(contact);
            continue;
        }
        buffer[strcspn(buffer, "\n")] = 0;
        contact->address = strdup(buffer);
        if (!contact->address) {
            printf("Error: Memory allocation error, memory for string in Contact %d not allocated\n", i);
            freeContact(contact);
            continue;
        }

        if (fscanf(fileptr, "%lld\n", &contact->phoneNum) != 1) {
            freeContact(contact);
            continue;
        }
        if (fscanf(fileptr, "%d\n", &contact->age) != 1) {
            freeContact(contact);
            continue;
        }
        int duplicate = 0;
        int count = countContacts(contacts);
        for (int j = 0; j < count; j++) {
            if (strcmp(contacts[j]->firstName, contact->firstName) == 0 &&
                strcmp(contacts[j]->familyName, contact->familyName) == 0) {
                duplicate = 1;
                break;
            }
        }

        if (!duplicate) {
            contacts = appendContact(contacts, contact);
        } else {
            freeContact(contact);
        }
    }
    fclose(fileptr);
    printf("Appended contacts from %s\n", filename);
    return contacts;
}

// done
Contact **mergeContactsFromFile(Contact **contacts, char *filename) {
    FILE *fileptr = fopen(filename, "r");
    if (fileptr == NULL) {
        printf("Error: File to load not found\n");
        return contacts;
    }

    int fileCount;
    if (fscanf(fileptr, "%d\n", &fileCount) != 1) { // next read at name
        fclose(fileptr);
        printf("Error: Could not read number of Contacts correctly\n");
        return contacts;
    }

    char buffer[100];
    for (int i = 0; i < fileCount; i++) {
        Contact *contact = malloc(sizeof(Contact));
        if (contact == NULL) {
            printf("Error: Memory allocation error, Contact %d in mergeContactsFromFile", i); // 0 index
            continue;
        }

        // firstname
        if (!fgets(buffer, 100, fileptr)) {
            free(contact);
            continue;
        }
        buffer[strcspn(buffer, "\n")] = 0;
        contact->firstName = strdup(buffer);
        if (!contact->firstName) {
            printf("Error: Memory allocation error, memory for string in Contact %d not allocated\n", i);
            free(contact);
            continue;
        }

        // familyname
        if (!fgets(buffer, 100, fileptr)) {
            freeContact(contact);
            continue;
        }
        buffer[strcspn(buffer, "\n")] = 0;
        contact->familyName = strdup(buffer);
        if (!contact->familyName) {
            printf("Error: Memory allocation error, memory for string in Contact %d not allocated\n", i);
            freeContact(contact);
            continue;
        }
        // address
        if (!fgets(buffer, 100, fileptr)) {
            freeContact(contact);
            continue;
        }
        buffer[strcspn(buffer, "\n")] = 0;
        contact->address = strdup(buffer);
        if (!contact->address) {
            printf("Error: Memory allocation error, memory for string in Contact %d not allocated\n", i);
            freeContact(contact);
            continue;
        }

        if (fscanf(fileptr, "%lld\n", &contact->phoneNum) != 1) {
            freeContact(contact);
            continue;
        }
        if (fscanf(fileptr, "%d\n", &contact->age) != 1) {
            freeContact(contact);
            continue;
        }
        int duplicate = 0;
        int count = countContacts(contacts);
        for (int j = 0; j < count; j++) {
            if (strcmp(contacts[j]->firstName, contact->firstName) == 0 &&
                strcmp(contacts[j]->familyName, contact->familyName) == 0) {
                duplicate = 1;
                break;
            }
        }

        if (!duplicate) {
            contacts = insertContactAlphabetical(contacts, contact);
        } else {
            freeContact(contact);
        }
    }
    fclose(fileptr);
    printf("Appended contacts from %s\n", filename);
    return contacts;
}
// done
Contact **editContact(Contact **contacts) {

    int count = countContacts(contacts);
    int inputIndex = 0;
    int choice = 0;
    char buffer[100];

    if (count == 0) {
        printf("No contacts available to edit\n");
        return contacts;
    }

    printf("Enter index of contact to edit (0-%d): ", count - 1);
    if (scanf("%d", &inputIndex) != 1 || inputIndex >= count || inputIndex < 0) {
        printf("Error: Invalid Index\n");
        while (getchar() != '\n');
        return contacts;
    }

    printf("Editing contact: %s %s\n", contacts[inputIndex]->firstName, contacts[inputIndex]->familyName);
    printf("1. Edit First Name\n"
           "2. Edit Last Name\n"
           "3. Edit Address\n"
           "4. Edit Phone Number\n"
           "5. Edit Age\n"
           "6. Cancel\n"
           "Choose an option: ");
    if (scanf("%d", &choice) != 1) {
        printf("Error: Invalid option\n");
        while (getchar() != '\n');
        return contacts;
    }
    while (getchar() != '\n');
    printf("\n");

    switch (choice) {
    case 1:
        printf("Enter new first name: ");
        if (!fgets(buffer, 100, stdin)) {
            printf("Error: Failed to read input\n");
            return contacts;
        }
        buffer[strcspn(buffer, "\n")] = 0;
        printf("\n");
        free(contacts[inputIndex]->firstName);
        contacts[inputIndex]->firstName = strdup(buffer);
        if (contacts[inputIndex]->firstName == NULL) {
            printf("Memory allocation error for string in editContact\n");
            return contacts;
        }
        break;
    case 2:
        printf("Enter new family name: ");
        if (!fgets(buffer, 100, stdin)) {
            printf("Error: Failed to read input\n");
            return contacts;
        }
        buffer[strcspn(buffer, "\n")] = 0;
        printf("\n");
        free(contacts[inputIndex]->familyName);
        contacts[inputIndex]->familyName = strdup(buffer);
        if (contacts[inputIndex]->familyName == NULL) {
            printf("Memory allocation error for string in editContact\n");
            return contacts;
        }
        break;
    case 3:
        printf("Enter new address: ");
        if (!fgets(buffer, 100, stdin)) {
            printf("Error: Failed to read input\n");
            return contacts;
        }
        buffer[strcspn(buffer, "\n")] = 0;
        printf("\n");
        free(contacts[inputIndex]->address);
        contacts[inputIndex]->address = strdup(buffer);
        if (contacts[inputIndex]->address == NULL) {
            printf("Memory allocation error for string in editContact\n");
            return contacts;
        }
        break;
    case 4:
        printf("Enter new phone number: Enter 10-digit phone number that must not start with 0: ");
        long long tempPhone;
        if (scanf("%lld", &tempPhone) == 1 && tempPhone >= 1000000000 && tempPhone <= 9999999999) {
            contacts[inputIndex]->phoneNum = tempPhone;
            printf("\n");
            while (getchar() != '\n');
            break;
        } else {
            printf("\nError: Invalid phone number\n");
            while (getchar() != '\n');
            return contacts;
        }
        break;

    case 5:
        printf("Enter new age: ");
        int tempAge;
        if (scanf("%d", &tempAge) == 1 && tempAge >= 1 && tempAge <= 150) {
            contacts[inputIndex]->age = tempAge;
            printf("\n");
            while (getchar() != '\n');
            break;
        } else {
            printf("\nError: Invalid age\n");
            while (getchar() != '\n');
            return contacts;
        }
        break;
    case 6:
        printf("Edit cancelled\n");
        return contacts;
    }

    return contacts;
}

void freeContact(Contact *contact) {
    if (!contact) {
        return;
    }

    free(contact->firstName);
    free(contact->familyName);
    free(contact->address);
    free(contact);
}

void freeContactList(Contact **contacts) {
    if (!contacts) {
        return;
    }

    int count = countContacts(contacts);
    for (int i = 0; i < count; i++) {
        freeContact(contacts[i]);
    }
    free(contacts);
}
