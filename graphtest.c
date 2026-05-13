#include <stdio.h>
#include <string.h>

#define MAX_AREA 10
#define MAX_SUBNODE 20
#define MAX_PLACE 100
#define INF 9999

// ================= STRUCT =================

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

// ================= GLOBAL ARRAY =================

struct Area areas[MAX_AREA];
struct SubNode subnodes[MAX_SUBNODE];
struct Place places[MAX_PLACE];

// ================= COUNTERS =================

int areaCount = 0;
int subnodeCount = 0;
int placeCount = 0;

// ================= GRAPH =================

int graph[MAX_PLACE][MAX_PLACE];

// ================= FUNCTIONS =================

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

void addPlace(int id, int subnode_id, char place_name[], int isRestaurant, int restaurant_id, char restaurant_name[]) {
    places[placeCount].id = id;
    places[placeCount].subnode_id = subnode_id;
    strcpy(places[placeCount].place_name, place_name);
    places[placeCount].isRestaurant = isRestaurant;

    // restaurant data
    places[placeCount].restaurant.restaurant_id = restaurant_id;
    strcpy(places[placeCount].restaurant.restaurant_name, restaurant_name);

    placeCount++;
}

void connectPlaces(int from, int to, int distance) {
    graph[from][to] = distance;
    graph[to][from] = distance;
}

// ================= SHOW FUNCTIONS =================

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
    printf("\n===== PLACES =====\n");
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

// ================= INPUT FUNCTIONS =================

int selectArea() {
    int area;
    printf("\nSelect Area: ");
    scanf("%d", &area);
    return area;
}

int selectSubNode() {
    int subnode;
    printf("\nSelect Sub Location: ");
    scanf("%d", &subnode);
    return subnode;
}

int selectStartPlace() {
    int start;
    printf("\nSelect Current Place: ");
    scanf("%d", &start);
    return start;
}

int selectRestaurant() {
    int restaurant;
    printf("\nSelect Restaurant: ");
    scanf("%d", &restaurant);
    return restaurant;
}

// ================= DIJKSTRA =================

// ================= DIJKSTRA =================

void findShortestPath(int start, int destination) {
    int distance[MAX_PLACE];
    int visited[MAX_PLACE];
    int parent[MAX_PLACE]; // เพิ่มตัวแปรนี้เพื่อจำเส้นทาง

    // 1. Initialize (ตั้งค่าเริ่มต้น)
    for(int i = 0; i < placeCount; i++) {
        distance[i] = INF;
        visited[i] = 0;
        parent[i] = -1; // -1 แปลว่ายังไม่มีจุดเริ่มต้น
    }
    distance[start] = 0;

    // 2. Algorithm (คำนวณหาระยะทาง)
    for(int count = 0; count < placeCount - 1; count++) {
        int min = INF ;
        int u = -1;

        // หาจุดที่ระยะทางน้อยที่สุดที่ยังไม่ได้ไป
        for(int i = 0; i < placeCount; i++) {
            if(!visited[i] && distance[i] <= min) {
                min = distance[i];
                u = i;
            }
        }

        if (u == -1) break; // กันเหนียวกรณีทางขาด
        visited[u] = 1;

        // อัปเดตระยะทางของจุดรอบๆ
        for(int v = 0; v < placeCount; v++) {
            if(!visited[v] && graph[u][v] && distance[u] + graph[u][v] < distance[v]) {
                distance[v] = distance[u] + graph[u][v];
                parent[v] = u; // **หัวใจสำคัญอยู่ตรงนี้: จดไว้ว่าจุด v มาจากจุด u**
            }
        }
    }

    // 3. Output (แสดงผลเส้นทาง)
    printf("\n===== SHORTEST PATH =====\n");
    
    if (distance[destination] == INF) {
        printf("Error: Cannot find a path to this restaurant.\n");
        return;
    }

    // 3. แสดงผล (ปรับปรุงใหม่ให้ชัดเจน)
    printf("\n========================================\n");
    printf("   🗺️  PATHFINDING RESULTS\n");
    printf("========================================\n");

    if (distance[destination] >= INF) {
        printf(" No path found between these locations!\n");
        return;

    // สร้าง Array ย้อนรอยเส้นทางจากปลายทางกลับไปจุดเริ่มต้น
    int path[MAX_PLACE];
    int path_count = 0;
    int current = destination;

    // ไล่ย้อนกลับจาก parent
    while (current != -1) {
        path[path_count] = current;
        path_count++;
        current = parent[current];
    }
    // ต้องผ่านจุดไหนบ้าง
    printf("START FROM: %s\n", places[start].place_name);
    printf("----------------------------------------\n");

    // ปริ้นท์เส้นทางจากจุดเริ่มต้นไปปลายทาง(Print ถอยหลัง Array)
    printf("ROUTE : ");
    for (int i = path_count - 1; i >= 0; i--) {
        printf("Step %d: %s", (path_count - i), places[path[i]].place_name);
        
        // ถ้าไม่ใช่จุดสุดท้าย ให้ปริ้นท์ลูกศร
        if (i > 0) {
            printf("  ->  Next to [%s] (dist: %d)\n", 
                   places[path[i-1]].place_name, 
                   graph[path[i]][path[i-1]]);
        }else{
            printf("   [ARRIVED]\n");
        }
    }
    

    // output
    printf("\n===== SHORTEST PATH =====\n");
    printf("FROM : %s\n", places[start].place_name);
    printf("TO : %s\n", places[destination].place_name);
    printf("Total DISTANCE : %d\n", distance[destination]);
}
}
// ================= SEND RESTAURANT ID =================

void sendRestaurantID(int place_id){
    for(int i = 0; i < placeCount; i++) {
        if(places[i].id == place_id) {
            printf("\nSending restaurant_id = %d to Queue System...\n", places[i].restaurant.restaurant_id);
        }
    }
}

// ================= MAIN =================

int main() {
    // --- เพิ่มส่วนนี้เพื่อล้างค่า Graph ก่อนเริ่ม ---
    for(int i = 0; i < MAX_PLACE; i++) {
        for(int j = 0; j < MAX_PLACE; j++) {
            graph[i][j] = 0; // 0 หมายถึงไม่มีทางเชื่อม
        }
    }
    // ================= ADD DATA =================
    // Area 1: Siam
    addArea(1, "Siam");
    
    // SubNode 1: MBK
    addSubNode(1, 1, "MBK");
    addPlace(0, 1, "Entrance", 0, 0, "");
    addPlace(1, 1, "Escalator", 0, 0, "");
    addPlace(2, 1, "classroom tutor", 0, 0, "");
    addPlace(3, 1, "Toilet", 0, 0, "");
    addPlace(4, 1, "MK", 1, 1001, "MK");
    addPlace(5, 1, "Yayoi", 1, 1002, "Yayoi");
    addPlace(6, 1, "FoodCourt", 0, 0, "");
    addPlace(7, 1, "Bonchon", 1, 0, "Bonchon");

    // CONNECT GRAPH: MBK
    connectPlaces(0, 1, 2);  // Entrance -> Escalator
    connectPlaces(1, 2, 1);  // Escalator -> Tutor
    connectPlaces(1, 3, 3);  // Escalator -> Toilet
    connectPlaces(1, 6, 4);  // Escalator -> FoodCourt
    connectPlaces(6, 4, 2);  // FoodCourt -> MK
    connectPlaces(6, 5, 2);  // FoodCourt -> Yayoi
    connectPlaces(6, 7, 2);  // FoodCourt -> Bonchon

    // SubNode 2: PARAGON
    addSubNode(2, 1, "PARAGON");
    addPlace(8, 2, "Entrance", 0, 0, "");
    addPlace(9, 2, "Escalator", 0, 0, "");
    addPlace(10, 2, "Gourmet", 0, 0, "");
    addPlace(11, 2, "Tops", 0, 0, "");
    addPlace(12, 2, "Yayoi", 1, 0, "Yayoi");
    addPlace(13, 2, "MK", 1, 0, "MK");
    addPlace(14, 2, "Bonchon", 1, 0, "Bonchon");

    // CONNECT GRAPH: PARAGON
    connectPlaces(8, 9, 2);   // Entrance -> Escalator
    connectPlaces(9, 10, 3);  // Escalator -> Gourmet
    connectPlaces(10, 11, 1); // Gourmet -> Tops
    connectPlaces(11, 12, 4);  // top -> Yayoi
    connectPlaces(9, 13, 4);  // Escalator -> MK
    connectPlaces(13, 14, 4);  // MK -> Bonchon
    
    // ทางเชื่อมระหว่างห้าง (Siam Inter-connection)
    //connectPlaces(0, 8, 15); // MBK -> Paragon 

    // ================= ADD DATA =================
    // Area 2: Ari
    addArea(2, "Ari");
    
    // SubNode 3: BTS Ari
    addSubNode(3, 2, "BTS Ari");
    addPlace(15, 3, "Entrance", 0, 0, "");
    addPlace(16, 3, "Escalator", 0, 0, "");
    addPlace(17, 3, "After You", 1, 1001, "After You");
    addPlace(18, 3, "Shabushi", 1, 1002, "Shabushi");
    addPlace(19, 3, "Somtum", 1, 0, "Somtum");
    addPlace(20, 3, "exit 2", 0, 0, "");
    addPlace(21, 3, "Benz", 0, 0, "");
    addPlace(22, 3, "PTT", 0, 0, "");

    // CONNECT GRAPH: BTS Ari
    connectPlaces(15, 16, 2);
    connectPlaces(16, 17, 3);
    connectPlaces(16, 18, 3);
    connectPlaces(16, 19, 3);
    connectPlaces(16, 20, 2);
    connectPlaces(20, 21, 5);
    connectPlaces(20, 22, 5);

    // SubNode 4: Villa Ari
    addSubNode(4, 2, "Villa Ari");
    addPlace(23, 4, "Bts exit 1", 0, 0, "");
    addPlace(24, 4, "Thia restarunt", 0, 0, "");
    addPlace(25, 4, "starbucks", 0, 0, "");
    addPlace(26, 4, "esculator", 0, 0, "");
    addPlace(27, 4, "tops", 0, 0, "");
    addPlace(28, 4, "Somtum", 1, 0, "Somtum");
    addPlace(29, 4, "After You", 1, 0, "After You");
    addPlace(30, 4, "Shabushi", 1, 0, "Shabushi");

    // CONNECT GRAPH: Villa Ari
    connectPlaces(23, 26, 2);
    connectPlaces(26, 24, 3);
    connectPlaces(26, 25, 1);
    connectPlaces(26, 27, 2);
    connectPlaces(27, 28, 2);
    connectPlaces(27, 29, 2);
    connectPlaces(27, 30, 2);
    
    // ทางเชื่อม BTS -> Villa
    connectPlaces(20, 23, 5);

    // ================= ADD DATA =================
    // Area 3: Pharram2
    addArea(3, "Pharram2");
    
    // SubNode 5: Bangmod
    addSubNode(5, 3, "bangmod");
    addPlace(31, 5, "Entrance", 0, 0, "");
    addPlace(32, 5, "Escalator", 0, 0, "");
    addPlace(33, 5, "Red building", 0, 0, "  ");
    addPlace(34, 5, "CB2", 0, 0, "");
    addPlace(35, 5, "King monkut food center", 0, 0, "");
    addPlace(36, 5, "space dragon", 1, 0, "space dragon");
    addPlace(37, 5, "McDonald's", 1, 0, "McDonald's");
    addPlace(38, 5, "KFC", 1, 0, "KFC");

    // CONNECT GRAPH: Bangmod
    connectPlaces(31, 32, 2);
    connectPlaces(32, 35, 3);
    connectPlaces(35, 33, 1);
    connectPlaces(35, 34, 1);
    connectPlaces(35, 36, 1);
    connectPlaces(35, 37, 1);
    connectPlaces(35, 38, 1);

    // SubNode 6: Central Pharam2
    addSubNode(6, 3, "Central Pharam2");
    addPlace(39, 6, "Entrance", 0, 0, "");
    addPlace(40, 6, "Escalator", 0, 0, "");
    addPlace(41, 6, "B2S", 0, 0, "");
    addPlace(42, 6, "Cinema", 0, 0, "");
    addPlace(43, 6, "Moshi Moshi", 0, 0, "");
    addPlace(44, 6, "Sushiro", 1, 0, "Sushiro");
    addPlace(45, 6, "McDonald's", 1, 0, "McDonald's");
    addPlace(46, 6, "KFC", 1, 0, "KFC");

    // CONNECT GRAPH: Central Pharam2
    connectPlaces(39, 40, 2);
    connectPlaces(40, 41, 3);
    connectPlaces(40, 42, 5);
    connectPlaces(40, 43, 3);
    connectPlaces(40, 44, 2);
    connectPlaces(40, 45, 2);
    connectPlaces(40, 46, 2);

    // ================= SYSTEM FLOW =================
    showAreas();
    int selectedArea = selectArea();

    showSubNodes(selectedArea);
    int selectedSubNode = selectSubNode();

    showPlaces(selectedSubNode);
    int start = selectStartPlace();

    showRestaurants(selectedSubNode);
    int destination = selectRestaurant();

    findShortestPath(start, destination);

    sendRestaurantID(destination);

    return 0;
}