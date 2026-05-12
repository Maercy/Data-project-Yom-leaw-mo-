#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================
// LOCATION & RESTAURANT DATA
// ============================================================

// Main nodes (branches)
#define BRANCH_COUNT 3
const char *branchNames[BRANCH_COUNT] = {"Siam", "Ari", "Rama 2"};
const char *branchIDs[BRANCH_COUNT] = {"ID01", "ID02", "ID03"};
const char branchPrefix[BRANCH_COUNT] = {'S', 'A', 'R'};

// Sub nodes per branch (2 each)
const char *subNodes[BRANCH_COUNT][2] = {
    {"MBK", "Paragon"},          // Siam
    {"BTS Ari", "Villa"},        // Ari
    {"Bangmod", "Central Rama2"} // Rama 2
};

// Restaurants per branch (same for all sub nodes in same branch)
#define REST_COUNT 3
const char *restaurants[BRANCH_COUNT][REST_COUNT] = {
    {"MK", "Yayoi", "BonChon"},          // Siam
    {"After You", "Shabushi", "Somtum"}, // Ari
    {"Sushiro", "McDonald's", "KFC"}     // Rama 2
};

// Food type per restaurant (commented for now — Cream will use for Graph)
// const char* foodTypes[BRANCH_COUNT][REST_COUNT] = {
//     {"Japanese/Shabu", "Japanese",      "Korean"},    // Siam
//     {"Dessert/Cafe",   "Japanese/Shabu","Thai"},       // Ari
//     {"Japanese",       "Fast Food",     "Fast Food"}  // Rama 2
// };

// ============================================================
// STRUCT: Customer
// ============================================================
typedef struct
{
    int bookingID;
    char queueCode[10]; // e.g. S-001, A-002, R-003
    char name[50];
    char phone[20];
    int partySize;
    int branchID;             // 1=Siam, 2=Ari, 3=Rama2
    char subNode[20];         // e.g. "MBK", "Villa"
    char restaurantName[30];  // e.g. "MK", "Sushiro"
    char currentLocation[50]; // where customer is right now
} Customer;

// ============================================================
// QUEUE NODE
// ============================================================
typedef struct QueueNode
{
    Customer data;
    struct QueueNode *next;
} QueueNode;

// ============================================================
// QUEUE
// ============================================================
typedef struct
{
    QueueNode *front;
    QueueNode *rear;
    int size;
    int branchID;
    int totalBooked;
} Queue;

void initQueue(Queue *q, int branchID)
{
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
    q->branchID = branchID;
    q->totalBooked = 0;
}

// ============================================================
// autoNotify()
// Auto-notifies customers with <= 10 queues remaining
// At exactly 10 remaining: asks for confirmation
// ============================================================
void autoNotify(Queue *q)
{
    if (q->front == NULL)
        return;

    QueueNode *curr = q->front;
    int pos = 1;

    while (curr != NULL)
    {
        int remaining = pos - 1;

        if (remaining == 0)
        {
            printf("[NOTIFICATION] %s (%s) - Your table is ready! Please proceed to %s.\n",
                   curr->data.name, curr->data.queueCode, curr->data.restaurantName);
        }
        else if (remaining == 10)
        {
            printf("\n[NOTIFICATION] %s (%s) - 10 queues remaining at %s. Branch %s\n",
                   curr->data.name, curr->data.queueCode,
                   curr->data.restaurantName, branchNames[q->branchID - 1]);
            printf("  Confirm reservation? (1=Yes / 0=No): ");
            int confirm;
            scanf("%d", &confirm);
            if (confirm == 0)
            {
                printf("  Reservation cancelled for %s (%s)\n",
                       curr->data.name, curr->data.queueCode);
                curr->data.bookingID = -1; // mark for removal
            }
            else
            {
                printf("  Confirmed! Please head to %s soon.\n", curr->data.restaurantName);
            }
        }
        else if (remaining < 10)
        {
            printf("[NOTIFICATION] %s (%s) - %d queue(s) remaining. Branch %s\n",
                   curr->data.name, curr->data.queueCode,
                   remaining, branchNames[q->branchID - 1]);
        }

        curr = curr->next;
        pos++;
    }

    // Remove cancelled reservations
    while (q->front != NULL && q->front->data.bookingID == -1)
    {
        QueueNode *temp = q->front;
        q->front = q->front->next;
        if (q->front == NULL)
            q->rear = NULL;
        free(temp);
        q->size--;
    }
}

// ============================================================
// enqueue()
// Called by Cream's Graph after customer picks restaurant
// ============================================================
void enqueue(Queue *q, Customer c)
{
    QueueNode *newNode = (QueueNode *)malloc(sizeof(QueueNode));

    q->totalBooked++;
    sprintf(c.queueCode, "%c-%03d", branchPrefix[q->branchID - 1], q->totalBooked);

    newNode->data = c;
    newNode->next = NULL;

    if (q->rear == NULL)
    {
        q->front = q->rear = newNode;
    }
    else
    {
        q->rear->next = newNode;
        q->rear = newNode;
    }
    q->size++;

    printf("\n[Booked] Queue Code: %s | %s | %s > %s | Party: %d | Branch: %s\n",
           c.queueCode, c.name, c.subNode, c.restaurantName,
           c.partySize, branchNames[q->branchID - 1]);

    autoNotify(q);
}

// ============================================================
// dequeue()
// ============================================================
void dequeue(Queue *q)
{
    if (q->front == NULL)
    {
        printf("[Empty] Branch %s has no waiting customers.\n", branchNames[q->branchID - 1]);
        return;
    }
    QueueNode *temp = q->front;
    printf("\n[Served] %s (%s) is now being seated at %s. Branch %s\n",
           temp->data.name, temp->data.queueCode,
           temp->data.restaurantName, branchNames[q->branchID - 1]);

    q->front = q->front->next;
    if (q->front == NULL)
        q->rear = NULL;
    free(temp);
    q->size--;

    autoNotify(q);
}

// ============================================================
// displayQueue()
// ============================================================
void displayQueue(Queue *q)
{
    if (q->front == NULL)
    {
        printf("[Branch %s] Queue is empty.\n", branchNames[q->branchID - 1]);
        return;
    }
    printf("\n--- Branch %s Queue (%d waiting) ---\n",
           branchNames[q->branchID - 1], q->size);
    QueueNode *curr = q->front;
    int pos = 1;
    while (curr != NULL)
    {
        printf("%d. %s | %s | %s > %s | Party: %d | Remaining: %d\n",
               pos, curr->data.queueCode, curr->data.name,
               curr->data.subNode, curr->data.restaurantName,
               curr->data.partySize, pos - 1);
        curr = curr->next;
        pos++;
    }
}

// ============================================================
// getPosition()
// ============================================================
int getPosition(Queue *q, char *queueCode)
{
    QueueNode *curr = q->front;
    int pos = 1;
    while (curr != NULL)
    {
        if (strcmp(curr->data.queueCode, queueCode) == 0)
            return pos;
        curr = curr->next;
        pos++;
    }
    return -1;
}

// ============================================================
// selectRestaurant()
// For testing — in final system Cream's Graph handles this
// Flow: current location -> branch -> sub node -> restaurant
// ============================================================
Customer selectRestaurant(int bookingID)
{
    Customer c;
    c.bookingID = bookingID;

    // Step 1: where are you now?
    printf("\nWhere are you currently located? ");
    scanf(" %[^\n]", c.currentLocation);

    // Step 2: pick branch
    printf("\nSelect area:\n");
    for (int i = 0; i < BRANCH_COUNT; i++)
        printf("  %d. %s (%s)\n", i + 1, branchNames[i], branchIDs[i]);
    printf("Choice: ");
    scanf("%d", &c.branchID);

    int b = c.branchID - 1;

    // Step 3: pick sub node
    printf("\nSelect location in %s:\n", branchNames[b]);
    for (int i = 0; i < 2; i++)
        printf("  %d. %s\n", i + 1, subNodes[b][i]);
    printf("Choice: ");
    int subChoice;
    scanf("%d", &subChoice);
    strcpy(c.subNode, subNodes[b][subChoice - 1]);

    // Step 4: pick restaurant
    printf("\nRestaurants in %s:\n", c.subNode);
    for (int i = 0; i < REST_COUNT; i++)
        printf("  %d. %s\n", i + 1, restaurants[b][i]);
    printf("Choice: ");
    int restChoice;
    scanf("%d", &restChoice);
    strcpy(c.restaurantName, restaurants[b][restChoice - 1]);

    // Step 5: customer info
    printf("\nName: ");
    scanf("%s", c.name);
    printf("Phone: ");
    scanf("%s", c.phone);
    printf("Party size: ");
    scanf("%d", &c.partySize);

    return c;
}

// ============================================================
// MAIN MENU
// ============================================================
int main()
{
    Queue branch1, branch2, branch3;
    initQueue(&branch1, 1); // Siam
    initQueue(&branch2, 2); // Ari
    initQueue(&branch3, 3); // Rama 2

    int nextID = 1;
    int choice;

    do
    {
        printf("\n=== Restaurant Reservation System ===\n");
        printf("1. New reservation\n");
        printf("2. Serve next customer\n");
        printf("3. Display all queues\n");
        printf("4. Check my position\n");
        printf("0. Exit\n");
        printf("Select: ");
        scanf("%d", &choice);

        if (choice == 1)
        {
            Customer c = selectRestaurant(nextID++);
            if (c.branchID == 1)
                enqueue(&branch1, c);
            else if (c.branchID == 2)
                enqueue(&branch2, c);
            else if (c.branchID == 3)
                enqueue(&branch3, c);
            else
                printf("Invalid branch.\n");
        }
        else if (choice == 2)
        {
            printf("Branch (1=Siam / 2=Ari / 3=Rama 2): ");
            int bid;
            scanf("%d", &bid);
            if (bid == 1)
                dequeue(&branch1);
            else if (bid == 2)
                dequeue(&branch2);
            else if (bid == 3)
                dequeue(&branch3);
        }
        else if (choice == 3)
        {
            displayQueue(&branch1);
            displayQueue(&branch2);
            displayQueue(&branch3);
        }
        else if (choice == 4)
        {
            char code[10];
            printf("Enter queue code (e.g. S-001): ");
            scanf("%s", code);
            int p1 = getPosition(&branch1, code);
            int p2 = getPosition(&branch2, code);
            int p3 = getPosition(&branch3, code);
            if (p1 != -1)
                printf("Position %d in Siam. %d queue(s) remaining.\n", p1, p1 - 1);
            else if (p2 != -1)
                printf("Position %d in Ari. %d queue(s) remaining.\n", p2, p2 - 1);
            else if (p3 != -1)
                printf("Position %d in Rama 2. %d queue(s) remaining.\n", p3, p3 - 1);
            else
                printf("Queue code not found.\n");
        }

    } while (choice != 0);

    return 0;
}