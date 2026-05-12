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

void findShortestPath(int start, int destination) {
    int distance[MAX_PLACE];
    int visited[MAX_PLACE];

    // initialize
    for(int i = 0; i < placeCount; i++) {
        distance[i] = INF;
        visited[i] = 0;
    }
    distance[start] = 0;

    // algorithm
    for(int count = 0; count < placeCount - 1; count++) {
        int min = INF;
        int u = -1;

        // find minimum distance
        for(int i = 0; i < placeCount; i++) {
            if(!visited[i] && distance[i] <= min) {
                min = distance[i];
                u = i;
            }
        }

        visited[u] = 1;

        // update distances
        for(int v = 0; v < placeCount; v++) {
            if(!visited[v] && graph[u][v] && distance[u] + graph[u][v] < distance[v]) {
                distance[v] = distance[u] + graph[u][v];
            }
        }
    }

    // output
    printf("\n===== SHORTEST PATH =====\n");
    printf("FROM : %s\n", places[start].place_name);
    printf("TO : %s\n", places[destination].place_name);
    printf("DISTANCE : %d\n", distance[destination]);
}

// ================= SEND RESTAURANT ID =================

void sendRestaurantID(int place_id) {
    for(int i = 0; i < placeCount; i++) {
        if(places[i].id == place_id) {
            printf("\nSending restaurant_id = %d to Queue System...\n", places[i].restaurant.restaurant_id);
        }
    }
}

// ================= MAIN =================

int main() {

    // ================= ADD DATA =================
    //siam*******
    addArea(1, "Siam");
    //mbk
    addSubNode(1, 1, "MBK");
    addPlace(0, 1, "Entrance", 0, 0, "");
    addPlace(1, 1, "Escalator", 0, 0, "");
    addPlace(2, 1, "MK", 1, 1001, "MK");
    addPlace(3, 1, "Yayoi", 1, 1002, "Yayoi");
    addPlace(4, 1, "FoodCourt", 0, 0, "");

    connectPlaces(0, 1, 2);
    connectPlaces(1, 2, 1);
    connectPlaces(1, 3, 3);
    connectPlaces(0, 4, 4);
    connectPlaces(4, 3, 2);
    
    //paragon
    addSubNode(2, 1, "PARAGON");
    addPlace(5, 2, "FoodCourt", 0, 0, "");
    addPlace(6, 2, "FoodCourt", 0, 0, "");
    addPlace(7, 2, "FoodCourt", 0, 0, "");
    addPlace(8, 2, "FoodCourt", 0, 0, "");
    addPlace(9, 2, "FoodCourt", 0, 0, "");

    // ================= CONNECT GRAPH =================
    
    connectPlaces(0, 1, 2);
    connectPlaces(1, 2, 1);
    connectPlaces(1, 3, 3);
    connectPlaces(0, 4, 4);
    connectPlaces(4, 3, 2);

        // ================= ADD DATA =================
    //Ari******
    addArea(2, "Ari");
    //bts
    addSubNode(3, 2, "BTS Ari");

    addPlace(10, 3, "Entrance", 0, 0, "");
    addPlace(11, 3, "Escalator", 0, 0, "");
    addPlace(12, 3, "MK", 1, 1001, "MK");
    addPlace(13, 3, "Yayoi", 1, 1002, "Yayoi");
    addPlace(14, 3, "FoodCourt", 0, 0, "");

    connectPlaces(0, 1, 2);
    connectPlaces(1, 2, 1);
    connectPlaces(1, 3, 3);
    connectPlaces(0, 4, 4);
    connectPlaces(4, 3, 2);

    //villa

    addSubNode(4, 2, "Villa Ari");
    addPlace(15, 4, "Entrance", 0, 0, "");
    addPlace(16, 4, "Escalator", 0, 0, "");
    addPlace(17, 4, "MK", 1, 1001, "MK");
    addPlace(18, 4, "Yayoi", 1, 1002, "Yayoi");
    addPlace(19, 4, "FoodCourt", 0, 0, "");

    // ================= CONNECT GRAPH =================

    connectPlaces(0, 1, 2);
    connectPlaces(1, 2, 1);
    connectPlaces(1, 3, 3);
    connectPlaces(0, 4, 4);
    connectPlaces(4, 3, 2);

    //พระราม2******
    addArea(3, "Pharram2");
    //bangmod
    addSubNode(5, 3, "bangmod");

    addPlace(20, 5, "Entrance", 0, 0, "");
    addPlace(21, 5, "Escalator", 0, 0, "");
    addPlace(22, 5, "MK", 1, 1001, "MK");
    addPlace(23, 5, "Yayoi", 1, 1002, "Yayoi");
    addPlace(24, 5, "FoodCourt", 0, 0, "");

    connectPlaces(0, 1, 2);
    connectPlaces(1, 2, 1);
    connectPlaces(1, 3, 3);
    connectPlaces(0, 4, 4);
    connectPlaces(4, 3, 2);

    //central phraram2

    addSubNode(6, 2, "Central Pharam2");
    addPlace(25, 6, "Entrance", 0, 0, "");
    addPlace(26, 6, "Escalator", 0, 0, "");
    addPlace(27, 6, "MK", 1, 1001, "MK");
    addPlace(28, 6, "Yayoi", 1, 1002, "Yayoi");
    addPlace(29, 6, "FoodCourt", 0, 0, "");

    // ================= CONNECT GRAPH =================

    connectPlaces(0, 1, 2);
    connectPlaces(1, 2, 1);
    connectPlaces(1, 3, 3);
    connectPlaces(0, 4, 4);
    connectPlaces(4, 3, 2);
    

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