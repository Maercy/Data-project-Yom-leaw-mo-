#include <stdio.h>
#include <string.h>
#include <windows.h>

#define MAX_AREA 10
#define MAX_SUBNODE 20
#define MAX_PLACE 100
#define INF 9999


// ================= STRUCT (โครงสร้างข้อมูล) =================

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

// ================= GLOBAL ARRAY (ตัวแปรส่วนกลาง) =================

struct Area areas[MAX_AREA];
struct SubNode subnodes[MAX_SUBNODE];
struct Place places[MAX_PLACE];

// ================= COUNTERS (ตัวนับจำนวนข้อมูล) =================

int areaCount = 0;
int subnodeCount = 0;
int placeCount = 0;

// ================= GRAPH (เมทริกซ์เก็บเส้นเชื่อม) =================

int graph[MAX_PLACE][MAX_PLACE];

// ================= FUNCTIONS (ฟังก์ชันจัดการข้อมูล) =================

// ฟังก์ชันเพิ่มพื้นที่หลัก (เช่น Siam, Ari)
void addArea(int id, char name[]) {
    areas[areaCount].id = id;
    strcpy(areas[areaCount].name, name);
    areaCount++;
}

// ฟังก์ชันเพิ่มสถานที่ย่อย (เช่น MBK, Paragon)
void addSubNode(int id, int area_id, char name[]) {
    subnodes[subnodeCount].id = id;
    subnodes[subnodeCount].area_id = area_id;
    strcpy(subnodes[subnodeCount].name, name);
    subnodeCount++;
}

// ฟังก์ชันเพิ่มจุดพิกัด (เช่น ประตูทางเข้า, ร้านอาหาร)
void addPlace(int id, int subnode_id, char place_name[], int isRestaurant, int restaurant_id, char restaurant_name[]) {
    places[placeCount].id = id;
    places[placeCount].subnode_id = subnode_id;
    strcpy(places[placeCount].place_name, place_name);
    places[placeCount].isRestaurant = isRestaurant;

    // เก็บข้อมูลร้านอาหาร (ถ้ามี)
    places[placeCount].restaurant.restaurant_id = restaurant_id;
    strcpy(places[placeCount].restaurant.restaurant_name, restaurant_name);

    placeCount++;
}

// ฟังก์ชันเชื่อมจุดสองจุดเข้าด้วยกันพร้อมระบุระยะทาง
void connectPlaces(int from, int to, int distance) {
    graph[from][to] = distance;
    graph[to][from] = distance;
}

// ================= SHOW FUNCTIONS (ฟังก์ชันแสดงผลเมนู) =================

void showAreas() {
    printf("\n===== AREAS  =====\n");
    for(int i = 0; i < areaCount; i++) {
        printf("%d. %s\n", areas[i].id, areas[i].name);
    }
}

void showSubNodes(int area_id) {
    printf("\n===== SUB LOCATIONS  =====\n");
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
    printf("\n===== RESTAURANTS  =====\n");
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

// ================= DIJKSTRA (อัลกอริทึมหาเส้นทางสั้นที่สุด) =================

void findShortestPath(int start, int destination) {
    int distance[MAX_PLACE];
    int visited[MAX_PLACE];
    int parent[MAX_PLACE]; 

    // 1. Initialize (ตั้งค่าเริ่มต้น)
    for(int i = 0; i < MAX_PLACE; i++) {
        distance[i] = INF;
        visited[i] = 0;
        parent[i] = -1; 
    }
    distance[start] = 0;

    // 2. Algorithm (คำนวณ)
    for(int count = 0; count < placeCount - 1; count++) {
        int min = INF;
        int u = -1;

        for(int i = 0; i < placeCount; i++) {
            if(!visited[i] && distance[i] <= min) {
                min = distance[i];
                u = i;
            }
        }

        if (u == -1) break; 
        visited[u] = 1;

        for(int v = 0; v < placeCount; v++) {
            // เช็คว่ามีการเชื่อมต่อกัน (graph[u][v] > 0) และระยะทางใหม่สั้นกว่าเดิมไหม
            if(!visited[v] && graph[u][v] > 0 && distance[u] + graph[u][v] < distance[v]) {
                distance[v] = distance[u] + graph[u][v];
                parent[v] = u; 
            }
        }
    }

    // 3. Output (แสดงผลขั้นตอนการเดิน)
    printf("\n========================================");
    printf("\n 🧭 NAVIGATION GUIDANCE");
    printf("\n========================================\n");
    
    if (distance[destination] >= INF) {
        printf("Error: Cannot find a path to this destination.\n");
        return;
    }

    // สร้าง Array ย้อนรอยเส้นทางจาก parent
    int path[MAX_PLACE];
    int path_count = 0;
    int current = destination;

    while (current != -1) {
        path[path_count++] = current;
        current = parent[current];
    }

    printf("START FROM: %s\n", places[start].place_name);
    printf("----------------------------------------\n");

    // ปริ้นท์เส้นทางไล่ลำดับ Step จากเริ่มไปจบ
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

// ================= SEND DATA (ส่งข้อมูลไปยังระบบอื่น) =================

void sendRestaurantID(int place_id){
    for(int i = 0; i < placeCount; i++) {
        if(places[i].id == place_id) {
            printf("\n[SYSTEM] Sending restaurant_id = %d to Queue System...\n", places[i].restaurant.restaurant_id);
        }
    }
}

// ================= MAIN (ส่วนการทำงานหลัก) =================

int main() {
    SetConsoleOutputCP(65001);
    // 1. ล้างค่า Graph ให้เป็น 0 ก่อนเริ่มใส่ข้อมูล
    for(int i = 0; i < MAX_PLACE; i++) {
        for(int j = 0; j < MAX_PLACE; j++) {
            graph[i][j] = 0;
        }
    }

    // 2. แอดข้อมูลย่านต่างๆ (Area)
    addArea(1, "Siam");
    addArea(2, "Ari");
    addArea(3, "Pharram2");

    // 3. แอดสถานที่ย่อย (SubNode)
    addSubNode(1, 1, "MBK");
    addSubNode(2, 1, "PARAGON");
    addSubNode(3, 2, "BTS Ari");
    addSubNode(4, 2, "Villa Ari");
    addSubNode(5, 3, "Bangmod");
    addSubNode(6, 3, "Central Pharam2");

    // 4. แอดพิกัดจุดต่างๆ (Places)
    // --- MBK ---
    addPlace(0, 1, "Entrance", 0, 0, "");
    addPlace(1, 1, "Escalator", 0, 0, "");
    addPlace(2, 1, "classroom tutor", 0, 0, "");
    addPlace(3, 1, "Toilet", 0, 0, "");
    addPlace(4, 1, "MK", 1, 1001, "MK");
    addPlace(5, 1, "Yayoi", 1, 1002, "Yayoi");
    addPlace(6, 1, "FoodCourt", 0, 0, "");
    addPlace(7, 1, "Bonchon", 1, 1003, "Bonchon");

    // --- PARAGON ---
    addPlace(8, 2, "Entrance", 0, 0, "");
    addPlace(9, 2, "Escalator", 0, 0, "");
    addPlace(10, 2, "Gourmet", 0, 0, "");
    addPlace(11, 2, "Tops", 0, 0, "");
    addPlace(12, 2, "Yayoi", 1, 2001, "Yayoi");
    addPlace(13, 2, "MK", 1, 2002, "MK");
    addPlace(14, 2, "Bonchon", 1, 2003, "Bonchon");

    // --- BTS Ari ---
    addPlace(15, 3, "Entrance", 0, 0, "");
    addPlace(16, 3, "Escalator", 0, 0, "");
    addPlace(17, 3, "After You", 1, 3001, "After You");
    addPlace(18, 3, "Shabushi", 1, 3002, "Shabushi");
    addPlace(19, 3, "Somtum", 1, 3003, "Somtum");
    addPlace(20, 3, "exit 2", 0, 0, "");
    addPlace(21, 3, "Benz", 0, 0, "");
    addPlace(22, 3, "PTT", 0, 0, "");

    // --- Villa Ari ---
    addPlace(23, 4, "Bts exit 1", 0, 0, "");
    addPlace(24, 4, "Thai restaurant", 0, 0, "");
    addPlace(25, 4, "starbucks", 0, 0, "");
    addPlace(26, 4, "escalator", 0, 0, "");
    addPlace(27, 4, "tops", 0, 0, "");
    addPlace(28, 4, "Somtum", 1, 4001, "Somtum");
    addPlace(29, 4, "After You", 1, 4002, "After You");
    addPlace(30, 4, "Shabushi", 1, 4003, "Shabushi");

    // --- Bangmod ---
    addPlace(31, 5, "Entrance", 0, 0, "");
    addPlace(32, 5, "Escalator", 0, 0, "");
    addPlace(33, 5, "Red building", 0, 0, "");
    addPlace(34, 5, "CB2", 0, 0, "");
    addPlace(35, 5, "Food center", 0, 0, "");
    addPlace(36, 5, "Space dragon", 1, 5001, "Space dragon");
    addPlace(37, 5, "McDonald's", 1, 5002, "McDonald's");
    addPlace(38, 5, "KFC", 1, 5003, "KFC");

    // --- Central Pharam2 ---
    addPlace(39, 6, "Entrance", 0, 0, "");
    addPlace(40, 6, "Escalator", 0, 0, "");
    addPlace(41, 6, "B2S", 0, 0, "");
    addPlace(42, 6, "Cinema", 0, 0, "");
    addPlace(43, 6, "Moshi Moshi", 0, 0, "");
    addPlace(44, 6, "Sushiro", 1, 6001, "Sushiro");
    addPlace(45, 6, "McDonald's", 1, 6002, "McDonald's");
    addPlace(46, 6, "KFC", 1, 6003, "KFC");

    // 5. สร้างเส้นเชื่อม (Graph Connections)
    // MBK
    connectPlaces(0, 1, 2); connectPlaces(1, 2, 1);
    connectPlaces(1, 3, 3); connectPlaces(1, 6, 4);
    connectPlaces(6, 4, 2); connectPlaces(6, 5, 2);
    connectPlaces(6, 7, 2);

    // PARAGON
    connectPlaces(8, 9, 2); connectPlaces(9, 10, 3);
    connectPlaces(10, 11, 1); connectPlaces(11, 12, 4);
    connectPlaces(9, 13, 4); connectPlaces(13, 14, 4);

    // BTS Ari
    connectPlaces(15, 16, 2); connectPlaces(16, 17, 3);
    connectPlaces(16, 18, 3); connectPlaces(16, 19, 3);
    connectPlaces(16, 20, 2); connectPlaces(20, 21, 5);
    connectPlaces(20, 22, 5);

    // Villa Ari
    connectPlaces(23, 26, 2); connectPlaces(26, 24, 3);
    connectPlaces(26, 25, 1); connectPlaces(26, 27, 2);
    connectPlaces(27, 28, 2); connectPlaces(27, 29, 2);
    connectPlaces(27, 30, 2);
    connectPlaces(20, 23, 5); // ทางเชื่อมข้ามจาก BTS Ari ไป Villa Ari

    // Bangmod
    connectPlaces(31, 32, 2); connectPlaces(32, 35, 3);
    connectPlaces(35, 33, 1); connectPlaces(35, 34, 1);
    connectPlaces(35, 36, 1); connectPlaces(35, 37, 1);
    connectPlaces(35, 38, 1);

    // Central Pharam2
    connectPlaces(39, 40, 2); connectPlaces(40, 41, 3);
    connectPlaces(40, 42, 5); connectPlaces(40, 43, 3);
    connectPlaces(40, 44, 2); connectPlaces(40, 45, 2);
    connectPlaces(40, 46, 2);

    // 6. ส่วนการรันระบบ (System Flow)
    showAreas();
    int area = selectArea();

    showSubNodes(area);
    int sub = selectSubNode();

    showPlaces(sub);
    int start = selectStartPlace();

    showRestaurants(sub);
    int dest = selectRestaurant();

    findShortestPath(start, dest);
    sendRestaurantID(dest);

    return 0;
}