#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Package {
    char code[20];
    float weight;
    char address[100];
    struct Package* next;
} Package;

Package* top = NULL;

void savePackages() {
    FILE *file = fopen("packages.dat", "wb");
    if (!file) {
        perror("Failed to open file");
        return;
    }
    Package *current = top;
    while (current) {
        fwrite(current, sizeof(Package), 1, file);
        current = current->next;
    }
    fclose(file);
}

void loadPackages() {
    FILE *file = fopen("packages.dat", "rb");
    if (!file) {
        perror("Failed to open file");
        return;
    }
    Package *current = NULL;
    while (1) {
        Package *newPackage = (Package*) malloc(sizeof(Package));
        if (fread(newPackage, sizeof(Package), 1, file) != 1) {
            free(newPackage);
            break;
        }
        newPackage->next = top;
        top = newPackage;
    }
    fclose(file);
}

void addPackage(char* code, float weight, char* address) {
    Package* newPackage = (Package*) malloc(sizeof(Package));
    strcpy(newPackage->code, code);
    newPackage->weight = weight;
    strcpy(newPackage->address, address);
    newPackage->next = top;
    top = newPackage;
    savePackages();
}

void deliverPackage() {
    if (!top) {
        printf("No packages to deliver.\n");
        return;
    }
    Package* temp = top;
    top = top->next;
    free(temp);
    savePackages();
}

void showUndeliveredPackages() {
    if (!top) {
        printf("No undelivered packages.\n");
        return;
    }
    Package* current = top;
    while (current) {
        printf("Package Code: %s, Weight: %.2f, Address: %s\n", current->code, current->weight, current->address);
        current = current->next;
    }
}

int main() {
    loadPackages();
    int choice;
    char code[20];
    float weight;
    char address[100];
    while (1) {
        printf("Press 1 to add package, 2 to deliver package, 3 to show undelivered packages, 0 to stop: ");
        scanf("%d", &choice);
        if (choice == 1) {
            printf("Enter package code, weight, and address: ");
            scanf("%s %f %[^\n]", code, &weight, address);
            addPackage(code, weight, address);
        } else if (choice == 2) {
            deliverPackage();
        } else if (choice == 3) {
            showUndeliveredPackages();
        } else if (choice == 0) {
            break;
        } else {
            printf("Invalid choice.\n");
        }
    }
    return 0;
}
