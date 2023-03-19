/* Nilvan Roxana Maria 
* grupa 30228
* In aceasta tema am avut de implementat algoritmul DFS de pargurege al grafurilor, sortarea topologica si algoritmul lui Tarjan
* 
* DFS: 
* Parcurgerea DFS este realizata prin doua functii DFS_VISIT si DFS
* Functia DFS primeste un graf, transmis ca si parametru si pargure toate nodurile grafului colorandu-le in alb, mai apoi pentru fiecare nod al
* grafului pargure lista de copii si daca gaseste un nod colorat alb apeleaza functia DFS_VISIT
* Functia DFS_Visit primeste ca parametru graful, un nod de start si o variabila time in care se stoca timpul de descoperire. Nodul trimis ca si 
* parametru va fi actualizat : incrementam timpul si setam timpul de descoperire pentru nodul curent, coloram nodul cu gri. 
* Pargurgem lista de vecini ai nodului si pentru fiecare vecin gasit alb setam nodul curent ca fiind nodul parite si reapelam alogoritmul DFS_Visit. 
* In final, dupa ce am parcurs toti vecinii setam timpul de final si coloram nodul cu negru.
* Complexitatea algoritmului este O(V + E)
* 
* Sortarea topologica:
* Se apeleaza functia DFS pentru a afla timpul de descoperire al fiecarui nod.
* Transformam graful intr-un array de noduri pentru a putea aplica algoritmul de sortare prin selectie.
* Soratm nodurile in functie de timpul final de descoperire
* Complexitatea algoritmului O(V + E)
* 
* Algoritmul lui Tarjan:
* Pentru realizarea algoritmului am creat un nou tip de nod care mai are doua variabile "low" si "onStack"
* Functia tarjanSCC apeleaza pentru fiecare nod gasit alb functia Tarjan
* Functia Tarjan functioneaza in felul urmator : setam timpul de descoperire, valoarea actuala a variabilei globale "time stamp", culoarea gri,
* adaugam nodul pe stack si ii modificam variabila onStack la 1. Parcurgem lista de copii a nodului si pentru fiecare nod gasit alb reapelam algoritmul
* si actualizam valoarea low sa fie minimul dintre nodul curent si nodul alb gasit. Daca nodul nu este alb si se afla pe stiva actualizam valoarea
* low a nodului curent sa fie minimul dintre low-ul curent si timpul de descoperire al nodului pe care l-am gasit.
* 
* Daca low-ul nodului este egal cu timpul de descoperire am gasit o componenta conexa si o afisam.
* 
* 
*/
#include<stdio.h>
#include "Profiler.h"

enum {
    COLOR_WHITE = 0,
    COLOR_GRAY,
    COLOR_BLACK
};

typedef struct _Node {
    int key;
    int adjSize;
    struct _Node** adj;
    int color;
    int discoveredTime;
    int finishTime;
    struct _Node* parent;
    struct _Node* kid;
}Node;

typedef struct {
    int nrNodes;
    Node** v;
}Graph;

typedef struct _TarjanNode {
    int key;
    int adjSize;
    struct _TarjanNode** adj;
    int color;
    int discoveredTime;
    int finishTime;
    struct _TarjanNode* parent;
    struct _TarjanNode* kid;
    int low;
    int onStack;
}TarjanNode;

typedef struct TarjanGraph {
    int nrNodes;
    TarjanNode** v;
}TarjanGraph;

Node* createNode(int key) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode != NULL) {
        newNode->key = key;
        newNode->color = COLOR_WHITE;
        newNode->adjSize = 0;
        newNode->discoveredTime = -1;
        newNode->finishTime = -1;
        newNode->parent = NULL;
        newNode->kid = NULL;
        newNode->adj = (Node**)malloc(sizeof(Node*));
    }
    return newNode;
}

Graph* createGraph(int number) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->nrNodes = number;
    graph->v = (Node**)malloc(number * sizeof(Node*));

    int i;
    for (i = 0; i < graph->nrNodes; i++) {
        graph->v[i] = createNode(i);
    }
    return graph;
}

TarjanNode* createTarjanNode(int key) {
    TarjanNode* newNode = (TarjanNode*)malloc(sizeof(Node));
    if (newNode != NULL) {
        newNode->key = key;
        newNode->color = COLOR_WHITE;
        newNode->adjSize = 0;
        newNode->discoveredTime = -1;
        newNode->finishTime = -1;
        newNode->parent = NULL;
        newNode->kid = NULL;
        newNode->adj = (TarjanNode**)malloc(sizeof(TarjanNode*));
        newNode->low = 0;
        newNode->onStack = 0;
    }
    return newNode;
}

TarjanGraph* createTarjanGraph(int number) {
    TarjanGraph* graph = (TarjanGraph*)malloc(sizeof(TarjanGraph));
    graph->nrNodes = number;
    graph->v = (TarjanNode**)malloc(number * sizeof(TarjanNode*));

    int i;
    for (i = 0; i < graph->nrNodes; i++) {
        graph->v[i] = createTarjanNode(i);
    }
    return graph;
}

void addTarjanEdge(TarjanGraph* graph, int src, int dest) {
    TarjanNode* u = graph->v[src];
    TarjanNode* v = graph->v[dest];
    TarjanNode** newAdj = (TarjanNode**)malloc((u->adjSize + 1) * sizeof(TarjanNode*));

    int i;
    for (i = 0; i < u->adjSize; i++) {
        newAdj[i] = u->adj[i];
    }
    newAdj[u->adjSize] = v;
    u->adjSize++;
    free(u->adj);
    u->adj = newAdj;
}

void addEdge(Graph* graph, int src, int dest) {
    Node* u = graph->v[src];
    Node* v = graph->v[dest];

    Node** newAdj = (Node**)malloc((u->adjSize + 1) * sizeof(Node*));

    int i;
    for (i = 0; i < u->adjSize; i++) {
        newAdj[i] = u->adj[i];
    }

    newAdj[u->adjSize] = v;
    u->adjSize++;
    free(u->adj);
    u->adj = newAdj;
}

void DFS_VISIT(Graph* G, Node* u, int* time, Operation *operation) {
    *time = *time + 1;
    u->discoveredTime = *time;
    u->color = COLOR_GRAY;
    if (operation != NULL) {
        operation->count(2);
    }

    int i;
    for (i = 0; i < u->adjSize; i++) {
        Node* v = u->adj[i];
        if (operation != NULL) {
            operation->count();
        }
        if (v->color == COLOR_WHITE) {
            v->parent = u;
            DFS_VISIT(G, v, time, operation);
        }
    }

    u->color = COLOR_BLACK;
    *time = *time + 1;
    u->finishTime = *time;
    if (operation != NULL) {
        operation->count(2);
    }
}

void DFS(Graph* graph, Operation *operation) {
    int i;
    for (i = 0; i < graph->nrNodes; i++) {
        if (operation != NULL) {
            operation->count(2);
        }
        graph->v[i]->color = COLOR_WHITE;
        graph->v[i]->parent = NULL;
    }

    int time = 0;
    for (i = 0; i < graph->nrNodes; i++) {
        if (graph->v[i]->color == COLOR_WHITE) {
            if (operation != NULL) {
                operation->count();
            }
            DFS_VISIT(graph, graph->v[i], &time, operation);
        }
    }
}

void selection_sort(Node** vector, int lenght) {
    int min_poz;
    for (int i = 0; i < lenght - 1; i++) {
        min_poz = i;
        for (int j = i + 1; j < lenght; j++) {
            if (vector[j]->finishTime < vector[min_poz]->finishTime) {
                min_poz = j;
            }
        }
        if (i != min_poz) {
            Node* aux = vector[i];
            vector[i] = vector[min_poz];
            vector[min_poz] = aux;
        }
    }
}

Node** convertGraphToArray(Graph* graph) {
    Node** array = (Node**)malloc(sizeof(Node*) * graph->nrNodes);
    for (int i = 0; i < graph->nrNodes; i++) {
        array[i] = graph->v[i];
    }
    return array;
}

void topologicalSort(Graph* graph) {
    DFS(graph, NULL);

    Node** arrayOfnodes = convertGraphToArray(graph);
    selection_sort(arrayOfnodes, graph->nrNodes);

    int i;
    for (i = 0; i < graph->nrNodes; i++) {
        printf("%d ", arrayOfnodes[i]->key);
    }
    printf("\n");
}

int timeStamp;
int index;
TarjanNode* stack[5000];
int stackSize;

void Tarjan(TarjanGraph* grah, TarjanNode* node) {
    node->discoveredTime = ++timeStamp;
    node->low = node->discoveredTime;
    node->color = COLOR_GRAY;
    stack[stackSize++] = node;
    node->onStack = 1;

    for (int i = 0; i < node->adjSize; i++) {
        TarjanNode* v = node->adj[i];
        if (v->color == COLOR_WHITE) {
            Tarjan(grah, v);
            if (node->low < v->low) {
                node->low = node->low;
            }
            else {
                node->low = v->low;
            }
        }
        else {
            if (v->onStack == 1) {
                if (node->low < v->discoveredTime) {
                    node->low = node->low;
                }
                else {
                    node->low = v->discoveredTime;
                }
            }
        }
    }

    if (node->low == node->discoveredTime) {
        printf("The strongly connected component : ");
            while (stack[stackSize] != node) {
                TarjanNode* aux = stack[--stackSize];
                aux->onStack = 0;
                printf("%d ", aux->key);
            }
            printf("\n");
    }
}

void tarjanSCC(TarjanGraph* graph) {
    for (int i = 0; i < graph->nrNodes; i++) {
        TarjanNode* aux = graph->v[i];
        if (aux->color == COLOR_WHITE) {
            Tarjan(graph, aux);
        }
    }
}

void printGraph(Graph* graph) {
    int i;
    for (i = 0; i < graph->nrNodes; i++) {
        Node* u = graph->v[i];
        printf("Adjacency list of node %d: ", u->key);
        int j;
        for (j = 0; j < u->adjSize; j++) {
            printf("%d ", u->adj[j]->key);
        }
        printf("\n");
    }
}

void printDFSResult(Graph* G) {
    int i;
    for (i = 0; i < G->nrNodes; i++) {
        Node* u = G->v[i];
        printf("Node %d: discovered at %d, finished at %d\n", u->key, u->discoveredTime, u->finishTime);
    }
}

void demo() {
    Graph* graph = createGraph(7);
    addEdge(graph, 0, 1);
    addEdge(graph, 1, 3);
    addEdge(graph, 2, 3);
    addEdge(graph, 2, 4);
    addEdge(graph, 4, 5);
    addEdge(graph, 5, 6);
    printGraph(graph);
    DFS(graph, NULL);
    printf("\n");
    printf("The graph after DFS : \n");
    printDFSResult(graph);
    printf("\n");
    topologicalSort(graph);
    printf("\n");
    TarjanGraph* graphTarjan = createTarjanGraph(6);
    addTarjanEdge(graphTarjan, 0, 1);
    addTarjanEdge(graphTarjan, 1, 2);
    addTarjanEdge(graphTarjan, 2, 3);
    addTarjanEdge(graphTarjan, 3, 0);
    addTarjanEdge(graphTarjan, 0, 5);
    addTarjanEdge(graphTarjan, 4, 3);
    addTarjanEdge(graphTarjan, 4, 5);
    tarjanSCC(graphTarjan);
}

void findDiferentValue(int array1[], int array2[], int poz, int size) {
    for (int i = poz; i < size; i++) {
        if (array1[poz] != array2[i]) {
            int aux = array2[poz];
            array2[poz] = array2[i];
            array2[i] = aux;
        }
    }
}

void generateConnectedGraph(Graph* graph, int numberOfEdges) {
    for (int i = 0; i < graph->nrNodes - 1; i++) {
        graph->v[i]->adj[0] = graph->v[i + 1];
        graph->v[i]->adjSize++;
    }
    int firstNode[5000];
    int secondNode[5000];
    FillRandomArray(firstNode, numberOfEdges, 0, graph->nrNodes - 1, false, UNSORTED);
    FillRandomArray(secondNode, numberOfEdges, 0, graph->nrNodes - 1, false, UNSORTED);

    for (int i = 0; i < numberOfEdges; i++) {
        if (firstNode[i] != secondNode[i]) {
            addEdge(graph, firstNode[i], secondNode[i]);
        }
        else {
            findDiferentValue(firstNode, secondNode, i, graph->nrNodes);
            addEdge(graph, firstNode[i], secondNode[i]);
        }
    }
}
void performance()
{
    int n, i;
    Profiler p("bfs");

    // vary the number of edges
    for (n = 1000; n <= 4500; n += 100) {
        Operation op = p.createOperation("dfs-edges", n);
        Graph graph;
        graph.nrNodes = 100;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for (i = 0; i < graph.nrNodes; ++i) {
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate n random edges
        // make sure the generated graph is connected
        for (int i = 0; i < graph.nrNodes; i++) {
            graph.v[i]->adj = (Node**)malloc(graph.nrNodes * sizeof(Node));

            for (int j = 0; j < graph.nrNodes; j++) {
                graph.v[i]->adj[j] = (Node*)malloc(sizeof(Node));
            }
        }
        generateConnectedGraph(&graph, n);
        DFS(&graph, &op);
    }

    // vary the number of vertices
    for (n = 100; n <= 200; n += 10) {
        Operation op = p.createOperation("dfs-vertices", n);
        Graph graph;
        graph.nrNodes = n;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for (i = 0; i < graph.nrNodes; ++i) {
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate 4500 random edges
        // make sure the generated graph is connected
        for (int i = 0; i < graph.nrNodes; i++) {
            graph.v[i]->adj = (Node**)malloc(graph.nrNodes * sizeof(Node));

            for (int j = 0; j < graph.nrNodes; j++) {
                graph.v[i]->adj[j] = (Node*)malloc(sizeof(Node));
            }
        }
        generateConnectedGraph(&graph, 4500);
        DFS(&graph, &op);
    }

    p.showReport();
}

int main() {
    demo();
    //performance();
	return 0;
}