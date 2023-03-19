#include <stdlib.h>
#include <string.h>
#include "bfs.h"
#define MAX_VALUE 1000

typedef struct QueueNode {
    Node* graphNode;
    QueueNode* next;
} QueueNode;

typedef struct Queue {
    int size;
    QueueNode* first;
    QueueNode* last;
}Queue;

int get_neighbors(const Grid *grid, Point p, Point neighb[])
{
    // TODO: fill the array neighb with the neighbors of the point p and return the number of neighbors
    // the point p will have at most 4 neighbors (up, down, left, right)
    // avoid the neighbors that are outside the grid limits or fall into a wall
    // note: the size of the array neighb is guaranteed to be at least 4

    int numberOfNeighbors = 0;
    
    //up
    if (p.row - 1 > 0) {
        if (grid->mat[p.row - 1][p.col] == 0) {
            neighb[numberOfNeighbors].row = p.row - 1;
            neighb[numberOfNeighbors].col = p.col;
            numberOfNeighbors++;
        }
    }

    //left
    if (p.col - 1 > 0) {
        if (grid->mat[p.row][p.col - 1] == 0) {
            neighb[numberOfNeighbors].row = p.row;
            neighb[numberOfNeighbors].col = p.col - 1;
            numberOfNeighbors++;
        }
    }

    //right
    if (p.col + 1 <= grid->cols) {
        if (grid->mat[p.row][p.col + 1] == 0) {
            neighb[numberOfNeighbors].row = p.row;
            neighb[numberOfNeighbors].col = p.col + 1;
            numberOfNeighbors++;
        }
    }

    //down
    if (p.row + 1 <= grid->rows) {
        if (grid->mat[p.row + 1][p.col] == 0) {
            neighb[numberOfNeighbors].row = p.row + 1;
            neighb[numberOfNeighbors].col = p.col;
            numberOfNeighbors++;
        }
    }

    return numberOfNeighbors;
}

void grid_to_graph(const Grid *grid, Graph *graph)
{
    //we need to keep the nodes in a matrix, so we can easily refer to a position in the grid
    Node *nodes[MAX_ROWS][MAX_COLS];
    int i, j, k;
    Point neighb[4];

    //compute how many nodes we have and allocate each node
    graph->nrNodes = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(grid->mat[i][j] == 0){
                nodes[i][j] = (Node*)malloc(sizeof(Node));
                memset(nodes[i][j], 0, sizeof(Node)); //initialize all fields with 0/NULL
                nodes[i][j]->position.row = i;
                nodes[i][j]->position.col = j;
                ++graph->nrNodes;
            }else{
                nodes[i][j] = NULL;
            }
        }
    }
    graph->v = (Node**)malloc(graph->nrNodes * sizeof(Node*));
    k = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(nodes[i][j] != NULL){
                graph->v[k++] = nodes[i][j];
            }
        }
    }

    //compute the adjacency list for each node
    for(i=0; i<graph->nrNodes; ++i){
        graph->v[i]->adjSize = get_neighbors(grid, graph->v[i]->position, neighb);
        if(graph->v[i]->adjSize != 0){
            graph->v[i]->adj = (Node**)malloc(graph->v[i]->adjSize * sizeof(Node*));
            k = 0;
            for(j=0; j<graph->v[i]->adjSize; ++j){
                if( neighb[j].row >= 0 && neighb[j].row < grid->rows &&
                    neighb[j].col >= 0 && neighb[j].col < grid->cols &&
                    grid->mat[neighb[j].row][neighb[j].col] == 0){
                        graph->v[i]->adj[k++] = nodes[neighb[j].row][neighb[j].col];
                }
            }
            if(k < graph->v[i]->adjSize){
                //get_neighbors returned some invalid neighbors
                graph->v[i]->adjSize = k;
                graph->v[i]->adj = (Node**)realloc(graph->v[i]->adj, k * sizeof(Node*));
            }
        }
    }
}

void free_graph(Graph *graph)
{
    if(graph->v != NULL){
        for(int i=0; i<graph->nrNodes; ++i){
            if(graph->v[i] != NULL){
                if(graph->v[i]->adj != NULL){
                    free(graph->v[i]->adj);
                    graph->v[i]->adj = NULL;
                }
                graph->v[i]->adjSize = 0;
                free(graph->v[i]);
                graph->v[i] = NULL;
            }
        }
        free(graph->v);
        graph->v = NULL;
    }
    graph->nrNodes = 0;
}

Queue* QueueNew() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->size = 0;
    queue->first = NULL;
    queue->last = NULL;
    return queue;
}

void Enqueue(Queue* queue, Node *newNode) {
    QueueNode* toInsert = (QueueNode*)malloc(sizeof(Node));
    toInsert->graphNode = newNode;
    toInsert->next = NULL;

    if (queue) {
        if (queue->first == NULL) {
            queue->first = toInsert;
            queue->last = toInsert;
        }
        else {
            queue->last->next = toInsert;
            queue->last = toInsert;
        }
        queue->size = queue->size + 1;
    }
}

Node* Dequeue(Queue* queue) {
    Node* toDelete = (Node*)malloc(sizeof(Node));
    toDelete = NULL;
    if (queue) {
        if (queue->first != NULL) {
            toDelete = queue->first->graphNode;
            queue->first = queue->first->next;

            if (queue->first == NULL) {
                queue->last = NULL;
            }
            queue->size = queue->size - 1;
        }
    }

    return toDelete;
    
}

void bfs(Graph *graph, Node *s, Operation *op)
{
    // TOOD: implement the BFS algorithm on the graph, starting from the node s
    // at the end of the algorithm, every node reachable from s should have the color BLACK
    // for all the visited nodes, the minimum distance from s (dist) and the parent in the BFS tree should be set
    // for counting the number of operations, the optional op parameter is received
    // since op can be NULL (when we are calling the bfs for display purposes), you should check it before counting:
    // if(op != NULL) op->count();

    for (int i = 0; i < graph->nrNodes; i++) {
        graph->v[i]->color = COLOR_WHITE;
        graph->v[i]->dist = 0;
        graph->v[i]->parent = NULL;

        if (op != NULL) op->count(3);
    }

    s->color = COLOR_GRAY;
    s->dist = 0;
    s->parent = NULL;
    if (op != NULL) op->count(3);

    Queue *queue = QueueNew();
    if (op != NULL) op->count();
    Enqueue(queue, s);
    while (queue->size > 0) {
        Node* u = Dequeue(queue);

        for (int i = 0; i < u->adjSize; i++) {
            if (op != NULL) op->count();
            if (u->adj[i]->color == COLOR_WHITE) {
                u->adj[i]->color = COLOR_GRAY;
                u->adj[i]->dist = u->adj[i]->dist + 1;
                u->adj[i]->parent = u;
                Enqueue(queue, u->adj[i]);
                if (op != NULL) op->count(3);
            }
        }

        if (op != NULL) op->count();
        u->color = COLOR_BLACK;
    }
}

void printSpace(int depth) {
    for (int i = 0; i < depth; i++) {
        printf("     ");
    }
}

void pretyPrintParentReprezentation(int arrayOfPerents[], int size, int root, int depth, Point repr[]) {
    printSpace(depth);
    printf("(%d, %d)\n", repr[root].row, repr[root].col);
    for (int i = 0; i < size; i++)
    {
        if (arrayOfPerents[i] == root)
        {
            pretyPrintParentReprezentation(arrayOfPerents, size, i, depth + 1, repr);
        }
    }
}

int findRootPostion(int arrayOfPerents[], int size) {
    for (int i = 0; i < size; i++) {
        if (arrayOfPerents[i] == -1) {
            return i;
        }
    }
}

void print_bfs_tree(Graph *graph)
{
    //first, we will represent the BFS tree as a parent array
    int n = 0; //the number of nodes
    int *p = NULL; //the parent array
    Point *repr = NULL; //the representation for each element in p

    //some of the nodes in graph->v may not have been reached by BFS
    //p and repr will contain only the reachable nodes
    int *transf = (int*)malloc(graph->nrNodes * sizeof(int));
    for(int i=0; i<graph->nrNodes; ++i){
        if(graph->v[i]->color == COLOR_BLACK){
            transf[i] = n;
            ++n;
        }else{
            transf[i] = -1;
        }
    }
    if(n == 0){
        //no BFS tree
        free(transf);
        return;
    }

    int err = 0;
    p = (int*)malloc(n * sizeof(int));
    repr = (Point*)malloc(n * sizeof(Node));
    for(int i=0; i<graph->nrNodes && !err; ++i){
        if(graph->v[i]->color == COLOR_BLACK){
            if(transf[i] < 0 || transf[i] >= n){
                err = 1;
            }else{
                repr[transf[i]] = graph->v[i]->position;
                if(graph->v[i]->parent == NULL){
                    p[transf[i]] = -1;
                }else{
                    err = 1;
                    for(int j=0; j<graph->nrNodes; ++j){
                        if(graph->v[i]->parent == graph->v[j]){
                            if(transf[j] >= 0 && transf[j] < n){
                                p[transf[i]] = transf[j];
                                err = 0;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    free(transf);
    transf = NULL;

    if(!err){
        // TODO: pretty print the BFS tree
        // the parrent array is p (p[k] is the parent for node k or -1 if k is the root)
        // when printing the node k, print repr[k] (it contains the row and column for that point)
        // you can adapt the code for transforming and printing multi-way trees from the previous labs
        int rootPosition = findRootPostion(p, n);
        pretyPrintParentReprezentation(p, n, rootPosition, 0, repr);

    }

    if(p != NULL){
        free(p);
        p = NULL;
    }
    if(repr != NULL){
        free(repr);
        repr = NULL;
    }
}

int shortest_path(Graph *graph, Node *start, Node *end, Node *path[])
{
    // TODO: compute the shortest path between the nodes start and end in the given graph
    // the nodes from the path, should be filled, in order, in the array path
    // the number of nodes filled in the path array should be returned
    // if end is not reachable from start, return -1
    // note: the size of the array path is guaranteed to be at least 1000
    bfs(graph, start);

    if (end->color != COLOR_BLACK) {
        return -1;
    }

    int pathLength = 0;
    Node* current = end;
    while (current != start) {
        path[pathLength] = current;
        pathLength++;
        current = current->parent;
    }

    path[pathLength] = start;
    pathLength++;

    for (int i = 0; i < pathLength / 2; i++) {
        Node* temp = path[i];
        path[i] = path[pathLength - i - 1];
        path[pathLength - i - 1] = temp;
    }

    return pathLength;
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

void generateConnectedGraph(Graph *graph, int numberOfEdges) {
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
            graph->v[firstNode[i]]->adj[graph->v[firstNode[i]]->adjSize] = graph->v[secondNode[i]];
            graph->v[firstNode[i]]->adjSize++;
        }
        else {
            findDiferentValue(firstNode, secondNode, i, graph->nrNodes);
            graph->v[firstNode[i]]->adj[graph->v[firstNode[i]]->adjSize] = graph->v[secondNode[i]];
            graph->v[firstNode[i]]->adjSize++;
        }
    }
}

void performance()
{
    int n, i;
    Profiler p("bfs");

    // vary the number of edges
    for(n=1000; n<=4500; n+=100){
        Operation op = p.createOperation("bfs-edges", n);
        Graph graph;
        graph.nrNodes = 100;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for(i=0; i<graph.nrNodes; ++i){
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
        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    // vary the number of vertices
    for(n=100; n<=200; n+=10){
        Operation op = p.createOperation("bfs-vertices", n);
        Graph graph;
        graph.nrNodes = n;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for(i=0; i<graph.nrNodes; ++i){
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
        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    p.showReport();
}
