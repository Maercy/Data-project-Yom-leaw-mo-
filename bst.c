#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Booking {
    int bookingID;
    char customerName[50];
    int tableNumber;

    struct Booking* left;
    struct Booking* right;
};

// create new node 
typedef struct Booking Booking;

Booking* createNode(int id, char name[], int table) {

    Booking* newNode = (Booking*)malloc(sizeof(Booking));

    newNode->bookingID = id;
    strcpy(newNode->customerName, name);
    newNode->tableNumber = table;

    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

// inserting bst 
Booking* insertHistory(Booking* root, int id, char name[], int table) {

    if (root == NULL) {
        return createNode(id, name, table);
    }

    if (id < root->bookingID) {
        root->left = insertHistory(root->left, id, name, table);
    }
    else if (id > root->bookingID) {
        root->right = insertHistory(root->right, id, name, table);
    }

    return root;
}
//search for booking 
Booking* searchHistory(Booking* root, int id) {

    if (root == NULL || root->bookingID == id) {
        return root;
    }

    if (id < root->bookingID) {
        return searchHistory(root->left, id);
    }

    return searchHistory(root->right, id);
}
// info didplay
void displayAll(Booking* root) {

    if (root == NULL) {
        return;
    }

    displayAll(root->left);

    printf("Booking ID: %d\n", root->bookingID);
    printf("Customer Name: %s\n", root->customerName);
    printf("Table Number: %d\n", root->tableNumber);
    printf("----------------------\n");

    displayAll(root->right);
}
// function 
int main() {

    Booking* root = NULL;

    root = insertHistory(root, 101, "Alice", 5);
    root = insertHistory(root, 103, "Bob", 2);
    root = insertHistory(root, 102, "Charlie", 7);

    printf("=== Reservation History ===\n");
    displayAll(root);

    printf("\n=== Search Booking ===\n");

    Booking* found = searchHistory(root, 102);


    if (found != NULL) {
        printf("Booking Found!\n");
        printf("Customer: %s\n", found->customerName);
        printf("Table: %d\n", found->tableNumber);
    }
    else {
        printf("Booking not found.\n");
    }

    return 0;
}
