#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_AREA 10
#define MAX_SUBNODE 20
#define MAX_PLACE 100
#define INF 9999

// ================= STRUCT (โครงสร้างข้อมูล) =================

// --- ระบบ BST (History) สำหรับเก็บประวัติ ---
struct Booking {
    int bookingID;
    char customerName[50];
    int tableNumber; // จำลองเลขโต๊ะ
    struct Booking* left;
    struct Booking* right;
};
typedef struct Booking Booking;

// QUEUE (gain) 
#define BRANCH_COUNT 3
const char *branchNames[BRANCH_COUNT] = {"Siam", "Ari", "Rama 2"};
const char branchPrefix[BRANCH_COUNT] = {'S', 'A', 'R'};

typedef struct {
    int bookingID;
    char queueCode[10];
    char name[50];
    char phone[20];
    int partySize;
    int branchID;
    char subNode[50];
    char restaurantName[50];
} Customer;

typedef struct QueueNode {
    Customer data;
    struct QueueNode *next;
} QueueNode;

typedef struct {
    QueueNode *front;
    QueueNode *rear;
    int size;
    int branchID;
    int totalBooked;
} Queue;

// ระบบ GRAPH (Cream) 
struct Area { 
    int id; 
    char name[50]; 
};

struct SubNode { 
    int id; 
    int area_id; 
    char name[50]; 
};

struct Restaurant { 
    int restaurant_id; 
    char restaurant_name[50]; 
};

struct Place {
    int id;
    int subnode_id;
    char place_name[50];
    int isRestaurant;
    struct Restaurant restaurant;
};

// ================= GLOBAL VARIABLES (ตัวแปรส่วนกลาง) =================

struct Area areas[MAX_AREA];
struct SubNode subnodes[MAX_SUBNODE];
struct Place places[MAX_PLACE];
Queue queues[BRANCH_COUNT]; 
Booking* historyRoot = NULL; // รากของ BST เก็บประวัติ

int areaCount = 0;
int subnodeCount = 0;
int placeCount = 0;
int graph[MAX_PLACE][MAX_PLACE];
int nextBookingID = 1001;

// ================= BST FUNCTIONS (ประวัติการจอง) =================

Booking* createBookingNode(int id, char name[], int table) {
    Booking* newNode = (Booking*)malloc(sizeof(Booking));
    newNode->bookingID = id;
    strcpy(newNode->customerName, name);
    newNode->tableNumber = table;
    newNode->left = newNode->right = NULL;
    return newNode;
}

Booking* insertHistory(Booking* root, int id, char name[], int table) {
    if (root == NULL) return createBookingNode(id, name, table);
    if (id < root->bookingID) root->left = insertHistory(root->left, id, name, table);
    else if (id > root->bookingID) root->right = insertHistory(root->right, id, name, table);
    return root;
}

Booking* searchHistory(Booking* root, int id) {
    if (root == NULL || root->bookingID == id) return root;
    if (id < root->bookingID) return searchHistory(root->left, id);
    return searchHistory(root->right, id);
}

void displayHistory(Booking* root) {
    if (root == NULL) return;
    displayHistory(root->left);
    printf("Booking ID: %d | Name: %s | Table: %d\n", root->bookingID, root->customerName, root->tableNumber);
    printf("----------------------------------------\n");
    displayHistory(root->right);
}

// ================= QUEUE FUNCTIONS (ระบบคิว) =================

void initQueue(Queue *q, int branchID) {
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
    q->branchID = branchID;
    q->totalBooked = 0;
}

void enqueue(Queue *q, Customer c) {
    QueueNode *newNode = (QueueNode *)malloc(sizeof(QueueNode));
    q->totalBooked++;
    sprintf(c.queueCode, "%c-%03d", branchPrefix[q->branchID - 1], q->totalBooked);
    newNode->data = c;
    newNode->next = NULL;
    
    if (q->rear == NULL) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
    q->size++;
    printf("\n>>> [QUEUED] Code: %s | Customer: %s | Restaurant: %s\n", c.queueCode, c.name, c.restaurantName);
}

// แก้ไขฟังก์ชัน dequeue ให้ลิ้งค์ข้อมูลไปเก็บใน BST History
void dequeue(Queue *q) {
    if (q->front == NULL) {
        printf("\n[EMPTY] No customers in %s branch.\n", branchNames[q->branchID-1]);
        return;
    }
    QueueNode *temp = q->front;
    int tableNum = (rand() % 20) + 1; // สุ่มเลขโต๊ะให้ลูกค้า

    printf("\n[SERVED] %s (%s) is now being seated at Table %d.\n", temp->data.name, temp->data.queueCode, tableNum);
    
    // --- LINK: ส่งข้อมูลจาก Queue ไปยัง BST History ---
    historyRoot = insertHistory(historyRoot, temp->data.bookingID, temp->data.name, tableNum);
    printf("[SYSTEM] Booking history updated for ID: %d\n", temp->data.bookingID);

    q->front = q->front->next;
    if (q->front == NULL) q->rear = NULL;
    free(temp);
    q->size--;
}

// ================= GRAPH FUNCTIONS (ระบบนำทาง) =================

void addArea(int id, char name[]) {
    areas[areaCount].id = id; 
    strcpy(areas[areaCount].name, name); 
    areaCount++;
}

void addSubNode(int id, int area_id, char name[]) {
    subnodes[subnodeCount].id = id; 
    subnodes[subnodeCount].area_id = area_id;
    strcpy(subnodes[subnodeCount].name, name); 
    subnodeCount++;
}

void addPlace(int id, int subnode_id, char name[], int isRest, int restID, char restName[]) {
    places[placeCount].id = id; 
    places[placeCount].subnode_id = subnode_id;
    strcpy(places[placeCount].place_name, name); 
    places[placeCount].isRestaurant = isRest;
    places[placeCount].restaurant.restaurant_id = restID;
    strcpy(places[placeCount].restaurant.restaurant_name, restName);
    placeCount++;
}

void connectPlaces(int from, int to, int dist) {
    graph[from][to] = dist; 
    graph[to][from] = dist;
}

void showAreas() {
    printf("\n===== AREAS =====\n");
    for(int i = 0; i < areaCount; i++) {
        printf("%d. %s\n", areas[i].id, areas[i].name);
    }
}

void showSubNodes(int area_id) {
    printf("\n===== SUB LOCATIONS =====\n");
    for(int i = 0; i < subnodeCount; i++) {
        if(subnodes[i].area_id == area_id) {
            printf("%d. %s\n", subnodes[i].id, subnodes[i].name);
        }
    }
}

void showPlaces(int subnode_id) {
    printf("\n===== PLACES (current place) =====\n");
    for(int i = 0; i < placeCount; i++) {
        if(places[i].subnode_id == subnode_id) {
            printf("%d. %s\n", places[i].id, places[i].place_name);
        }
    }
}

void showRestaurants(int subnode_id) {
    printf("\n===== RESTAURANTS =====\n");
    for(int i = 0; i < placeCount; i++) {
        if(places[i].subnode_id == subnode_id && places[i].isRestaurant == 1) {
            printf("%d. %s\n", places[i].id, places[i].restaurant.restaurant_name);
        }
    }
}

// ================= INPUT FUNCTIONS (ฟังก์ชันรับค่าจาก User) =================

int selectArea() {
    int area;
    printf("\nSelect Area ID: ");
    scanf("%d", &area);
    return area;
}

int selectSubNode() {
    int subnode;
    printf("\nSelect Sub Location ID: ");
    scanf("%d", &subnode);
    return subnode;
}

int selectStartPlace() {
    int start;
    printf("\nSelect Your Current Place ID: ");
    scanf("%d", &start);
    return start;
}

int selectRestaurant() {
    int restaurant;
    printf("\nSelect Target Restaurant ID: ");
    scanf("%d", &restaurant);
    return restaurant;
}

void findShortestPath(int start, int destination) {
    int distance[MAX_PLACE], visited[MAX_PLACE], parent[MAX_PLACE];
    for (int i = 0; i < MAX_PLACE; i++) { 
        distance[i] = INF; 
        visited[i] = 0; 
        parent[i] = -1; 
    }
    distance[start] = 0;

    for (int count = 0; count < MAX_PLACE - 1; count++) {
        int min = INF, u = -1;
        for (int i = 0; i < MAX_PLACE; i++)
            if (!visited[i] && distance[i] <= min) { min = distance[i]; u = i; }
        if (u == -1) break;
        visited[u] = 1;
        for (int v = 0; v < MAX_PLACE; v++)
            if (!visited[v] && graph[u][v] > 0 && distance[u] + graph[u][v] < distance[v]) {
                distance[v] = distance[u] + graph[u][v]; 
                parent[v] = u;
            }
    }

    printf("\n========================================");
    printf("\n 🧭 NAVIGATION GUIDANCE");
    printf("\n========================================\n");

    if (distance[destination] >= INF) {
        printf("\nNo path found!\n");
        return;
    }

    int path[MAX_PLACE], path_count = 0, curr = destination;
    while (curr != -1) { 
        path[path_count++] = curr; 
        curr = parent[curr]; 
    }
    
    printf("START FROM: %s\n", places[start].place_name);
    printf("----------------------------------------\n");

    for (int i = path_count - 1; i >= 0; i--) {
        int nodeId = path[i];
        printf("Step %d: %s", (path_count - i), places[nodeId].place_name);
        
        if (i > 0) {
            int nextId = path[i-1];
            printf("  ->  Next: %s (dist: %d)\n", places[nextId].place_name, graph[nodeId][nextId]);
        } else {
            printf("   [ARRIVED] 🏁\n");
        }
    }
    
    printf("----------------------------------------\n");
    printf("TOTAL DISTANCE : %d units\n", distance[destination]);
    printf("========================================\n");
}

// ================= INTEGRATION (ส่วนเชื่อมต่อ) =================

void processBooking(int startID, int destID) {
    Customer c;
    
    // 1. นำทางหาเส้นทางใกล้ที่สุดก่อน
    findShortestPath(startID, destID);

    // 2. กรอกข้อมูลลูกค้า
    printf("\n--- Step: Registration ---\n");
    c.bookingID = nextBookingID++; // ลิ้งค์ ID อัตโนมัติ
    printf("Enter Name: "); scanf("%s", c.name);
    printf("Enter Phone: "); scanf("%s", c.phone);
    printf("Party Size: "); scanf("%d", &c.partySize);

    // 3. ดึงข้อมูลจาก Graph มาลงตัวแปร Customer
    int subID = places[destID].subnode_id;
    int areaID = -1;
    for(int i = 0; i < subnodeCount; i++) {
        if(subnodes[i].id == subID) {
            areaID = subnodes[i].area_id;
            strcpy(c.subNode, subnodes[i].name);
            break;
        }
    }
    c.branchID = areaID;
    strcpy(c.restaurantName, places[destID].restaurant.restaurant_name);

    // 4. จองคิวด้วย Queue
    enqueue(&queues[areaID - 1], c);
}

// ================= MAIN =================

int main() {
    SetConsoleOutputCP(65001);
    
    for (int i = 0; i < BRANCH_COUNT; i++) initQueue(&queues[i], i + 1);
    for (int i = 0; i < MAX_PLACE; i++) 
        for (int j = 0; j < MAX_PLACE; j++) graph[i][j] = 0;

    // --- SEED DATA ---
    addArea(1, "Siam");
    addArea(2, "Ari");
    addArea(3, "Pharram2");

    addSubNode(1, 1, "MBK");
    addSubNode(2, 1, "PARAGON");
    addSubNode(3, 2, "BTS Ari");
    addSubNode(4, 2, "Villa Ari");
    addSubNode(5, 3, "Bangmod");
    addSubNode(6, 3, "Central Pharam2");

    // SIAM - MBK
    addPlace(0, 1, "Entrance", 0, 0, "");
    addPlace(1, 1, "Escalator", 0, 0, "");
    addPlace(2, 1, "classroom tutor", 0, 0, "");
    addPlace(3, 1, "Toilet", 0, 0, "");
    addPlace(4, 1, "MK", 1, 1001, "MK");
    addPlace(5, 1, "Yayoi", 1, 1002, "Yayoi");
    addPlace(6, 1, "FoodCourt", 0, 0, "");
    addPlace(7, 1, "Bonchon", 1, 1003, "Bonchon");

    connectPlaces(0, 1, 2); connectPlaces(1, 2, 1);
    connectPlaces(1, 3, 3); connectPlaces(1, 6, 4);
    connectPlaces(6, 4, 2); connectPlaces(6, 5, 2);
    connectPlaces(6, 7, 2);

    // SIAM - PARAGON
    addPlace(8, 2, "Entrance", 0, 0, "");
    addPlace(9, 2, "Escalator", 0, 0, "");
    addPlace(10, 2, "Gourmet", 0, 0, "");
    addPlace(11, 2, "Tops", 0, 0, "");
    addPlace(12, 2, "Yayoi", 1, 2001, "Yayoi");
    addPlace(13, 2, "MK", 1, 2002, "MK");
    addPlace(14, 2, "Bonchon", 1, 2003, "Bonchon");

    connectPlaces(8, 9, 2); connectPlaces(9, 10, 3);
    connectPlaces(10, 11, 1); connectPlaces(11, 12, 4);
    connectPlaces(9, 13, 4); connectPlaces(13, 14, 4);

    // ARI - BTS
    addPlace(15, 3, "Entrance", 0, 0, "");
    addPlace(16, 3, "Escalator", 0, 0, "");
    addPlace(17, 3, "After You", 1, 3001, "After You");
    addPlace(18, 3, "Shabushi", 1, 3002, "Shabushi");
    addPlace(19, 3, "Somtum", 1, 3003, "Somtum");
    addPlace(20, 3, "exit 2", 0, 0, "");
    addPlace(21, 3, "Benz", 0, 0, "");
    addPlace(22, 3, "PTT", 0, 0, "");

    connectPlaces(15, 16, 2); connectPlaces(16, 17, 3);
    connectPlaces(16, 18, 3); connectPlaces(16, 19, 3);
    connectPlaces(16, 20, 2); connectPlaces(20, 21, 5);
    connectPlaces(20, 22, 5);

    // ARI - VILLA
    addPlace(23, 4, "Bts exit 1", 0, 0, "");
    addPlace(24, 4, "Thai restaurant", 0, 0, "");
    addPlace(25, 4, "starbucks", 0, 0, "");
    addPlace(26, 4, "escalator", 0, 0, "");
    addPlace(27, 4, "tops", 0, 0, "");
    addPlace(28, 4, "Somtum", 1, 4001, "Somtum");
    addPlace(29, 4, "After You", 1, 4002, "After You");
    addPlace(30, 4, "Shabushi", 1, 4003, "Shabushi");

    connectPlaces(23, 26, 2); connectPlaces(26, 24, 3);
    connectPlaces(26, 25, 1); connectPlaces(26, 27, 2);
    connectPlaces(27, 28, 2); connectPlaces(27, 29, 2);
    connectPlaces(27, 30, 2); connectPlaces(20, 23, 5);

    // RAMA 2 - BANGMOD
    addPlace(31, 5, "Entrance", 0, 0, "");
    addPlace(32, 5, "Escalator", 0, 0, "");
    addPlace(33, 5, "Red building", 0, 0, "");
    addPlace(34, 5, "CB2", 0, 0, "");
    addPlace(35, 5, "Food center", 0, 0, "");
    addPlace(36, 5, "Space dragon", 1, 5001, "Space dragon");
    addPlace(37, 5, "McDonald's", 1, 5002, "McDonald's");
    addPlace(38, 5, "KFC", 1, 5003, "KFC");

    connectPlaces(31, 32, 2); connectPlaces(32, 35, 3);
    connectPlaces(35, 33, 1); connectPlaces(35, 34, 1);
    connectPlaces(35, 36, 1); connectPlaces(35, 37, 1);
    connectPlaces(35, 38, 1);

    // RAMA 2 - CENTRAL
    addPlace(39, 6, "Entrance", 0, 0, "");
    addPlace(40, 6, "Escalator", 0, 0, "");
    addPlace(41, 6, "B2S", 0, 0, "");
    addPlace(42, 6, "Cinema", 0, 0, "");
    addPlace(43, 6, "Moshi Moshi", 0, 0, "");
    addPlace(44, 6, "Sushiro", 1, 6001, "Sushiro");
    addPlace(45, 6, "McDonald's", 1, 6002, "McDonald's");
    addPlace(46, 6, "KFC", 1, 6003, "KFC");

    connectPlaces(39, 40, 2); connectPlaces(40, 41, 3);
    connectPlaces(40, 42, 5); connectPlaces(40, 43, 3);
    connectPlaces(40, 44, 2); connectPlaces(40, 45, 2);
    connectPlaces(40, 46, 2);

    int choice;
    do {
        printf("\n======= RESTAURANT SYSTEM (Integrated) =======\n");
        printf("1. Reservation & Find Path (Graph -> Queue)\n");
        printf("2. Serve Next Customer (Queue -> BST History)\n");
        printf("3. Display Current Queues\n");
        printf("4. Search Booking History (BST Search)\n");
        printf("5. Display All History (BST Inorder)\n");
        printf("0. Exit\n");
        printf("Select: "); scanf("%d", &choice);

        if (choice == 1) {
            showAreas();
            int area = selectArea();
            showSubNodes(area);
            int sub = selectSubNode();
            showPlaces(sub);
            int start = selectStartPlace();
            showRestaurants(sub);
            int dest = selectRestaurant();
            processBooking(start, dest);
        } else if (choice == 2) {
            printf("\nSelect Branch (1:Siam, 2:Ari, 3:Rama 2): ");
            int b; scanf("%d", &b);
            dequeue(&queues[b-1]);
        } else if (choice == 3) {
            for(int i=0; i<BRANCH_COUNT; i++) {
                printf("\n--- %s Queue ---\n", branchNames[i]);
                QueueNode *curr = queues[i].front;
                if(!curr) printf(" Empty\n");
                while(curr) {
                    printf(" [%s] %s (ID: %d)\n", curr->data.queueCode, curr->data.name, curr->data.bookingID);
                    curr = curr->next;
                }
            }
        } else if (choice == 4) {
            printf("\nEnter Booking ID to search: ");
            int sid; scanf("%d", &searchID);
            Booking* found = searchHistory(historyRoot, sid);
            if (found) printf("Found! Name: %s, Table: %d\n", found->customerName, found->tableNumber);
            else printf("History not found.\n");
        } else if (choice == 5) {
            printf("\n=== Reservation History (BST) ===\n");
            if(!historyRoot) printf("No history yet.\n");
            else displayHistory(historyRoot);
        }
    } while (choice != 0);

    return 0;
}