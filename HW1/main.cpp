#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <algorithm>
#include <time.h>

using namespace std;

int gofn = 0;
int num_Node_for_A_star = 0;
int num_Node_for_A_star_Manhattan = 0;
int num_Node_for_DFBnB_Manhattan = 0;
int num_Node_for_IDA_Manhattan = 0;


struct Node
{
    Node* parent;
    int matrix[3][3];
    int a, b;
    int cost;
    int level;
};

// This function prints optimal solution
void printMatrix(int matrix[3][3])
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
            printf("%d ", matrix[i][j]);
        printf("\n");
    }
}
// this function allocate the new node for DBnB
Node* newNode(int matrix[3][3], int a, int b, int newA,
    int newB, int level, Node* parent)
{
    Node* node = new Node;
    node->parent = parent;
    memcpy(node->matrix, matrix, sizeof node->matrix);
    swap(node->matrix[a][b], node->matrix[newA][newB]);
    node->cost = INT_MAX;
    node->level = level;
    node->a = newA;
    node->b = newB;
    return node;
}
// the four actions are moving the empty tile right, down, left, and up.
int row[] = { 0, 1, 0, -1 };
int col[] = { 1, 0, -1, 0 };

int isSafe(int a, int b)
{
    return (a >= 0 && a < 3 && b >= 0 && b < 3);
}

// print path from root node to destination node
void printPath(Node* root)
{
    if (root == NULL)
        return;
    printPath(root->parent);
    printMatrix(root->matrix);
    printf("\n");
}

//this function calculate the manhattan heuristic distance for DBnB
int for_DFS_manhattan(int initial[3][3], int end[3][3]){
    int dist = 0;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (initial[i][j] != 0)
                for (int k = 0; k < 3; k++)
                    for (int l = 0; l < 3; l++)
                        if (initial[i][j] == end[k][l])
                            dist += abs(i - k) + abs(j - l);
    
    // here we are counting the manhattan and adding gofn to it.
    // f(n)=g(n)+h(n);

    dist = dist + gofn;
    return dist;
}

struct comp
{
    bool operator()(const Node* lhs, const Node* rhs) const
    {
        return (lhs->cost + lhs->level) > (rhs->cost + rhs->level);
    }
};


// this function allocate the new node for A* node
class AstarNode
{
public:

    int* configuration;
    int gStar;
    int hStar;
    int fStar;
    AstarNode* parent;
    AstarNode* next;

    AstarNode() {
        configuration = new int[9];
        for (int i = 0; i < 9; i++) {
            configuration[i] = 0;
        }
        gStar = 0;
        hStar = 0;
        fStar = gStar + hStar;
        parent = NULL;
        next = NULL;
    }

    AstarNode(int* config) {
        configuration = new int[9];
        for (int i = 0; i < 9; i++) {
             configuration[i]=config[i];
        }
        gStar = 0;
        hStar = 0;
        fStar = gStar + hStar;
        parent = NULL;
        next = NULL;
    }

    AstarNode(int* config, AstarNode* p) {
        configuration = new int[9];
        for (int i = 0; i < 9; i++) {
            configuration[i] = config[i];
        }
        gStar = 0;
        hStar = 0;
        fStar = gStar + hStar;
        parent = p;
        next = NULL;
    }

    AstarNode(const AstarNode& object) {
        configuration = new int[9];
        for (int i = 0; i < 9; i++) {
            configuration[i] = object.configuration[i];
        }
        gStar = object.gStar;
        hStar = object.hStar;
        fStar = object.fStar;
        parent = object.parent;
        next = object.next;
    }
    
// define the & operator
    AstarNode& operator = (const AstarNode& rightSide) {
        delete [] configuration;
        configuration = new int[9];
        for (int i = 0; i < 9; i++) {
            configuration[i] = rightSide.configuration[i];
        }
        gStar = rightSide.gStar;
        hStar = rightSide.hStar;
        fStar = rightSide.fStar;
        delete parent;
        parent = rightSide.parent;
        delete next;
        next = rightSide.next;

        return *this;
    }

    ~AstarNode() {
        delete [] configuration;
        delete parent;
        delete next;
    }
};

class AStarSearch
{
public:

    AstarNode* startNode;
    AstarNode* goalNode;
    AstarNode* OpenList;
    AstarNode* CloseList;
    AstarNode* childList;
    AstarNode* temp;

    AStarSearch(int* initial,int* end) {
        startNode = new AstarNode(initial);
        goalNode = new AstarNode(end);
        OpenList = new AstarNode();
        CloseList = new AstarNode();
        childList = new AstarNode();
        temp = new AstarNode();
    }
//this function caculating the misplace of 8 puzzle for A* search using the heuristic function
    void computeStars(AstarNode* node) {
        if (node->parent != NULL) {
            node->gStar = node->parent->gStar + 1;
        }
        int misplaced = 0;
        for (int i = 0; i < 9; i++) {
            if (node->configuration[i] != goalNode->configuration[i]) {
                misplaced++;
            }
        }
        node->hStar = misplaced;
        node->fStar = node->gStar + node->hStar;
    }
//this function calculate the manhattan heuristic distance for A* search using the manhattan heuristic function
    void computemManhattan(AstarNode* node) {
        for (int i = 0; i < 9; i++)
            if(node->configuration[i] !=0)
                for (int j = 0; j < 9; j++)
                    if(node->configuration[i] == goalNode->configuration[j])
                         node->gStar+=abs(i-j);
        int misplaced = 0;
        for (int i = 0; i < 9; i++) {
            if (node->configuration[i] != goalNode->configuration[i]) {
                misplaced++;
            }
        }
        // here we are counting the manhattan and adding misplace puzzle to it.
        // f(n)=g(n)+h(n);
        node->hStar = misplaced;
        node->fStar = node->gStar + node->hStar;
    }

//Compare the two puzzle
    bool match(int* configuration1, int* configuration2) {
        for (int i = 0; i < 9; i++) {
            if (configuration1[i] != configuration2[i]) {
                return false;
            }
        }
        return true;
    }
//Check to see if the puzzle match to the goal puzzle
    bool isGoalNode(AstarNode* node) {
        return match(node->configuration, goalNode->configuration);
    }
//Check to see if the puzzle match to the start puzzle
    bool isStartNode(AstarNode* node) {
        return match(node->configuration, startNode->configuration);
    }
//Inset the node into the list
    void listInsert(AstarNode* node, AstarNode* list) {
        AstarNode* scan = list;
        while (scan->next != NULL && node->fStar > scan->next->fStar) {
            scan = scan->next;
        }
        node->next = scan->next;
        scan->next = node;
    }
//Remove the node into the list
    AstarNode* remove(AstarNode* list) {
        if (list->next != NULL) {
            AstarNode* temp = list->next;
            list->next = list->next->next;
            return temp;
        }
        else {
            return NULL;
        }
    }

    void push(AstarNode* node) {
        node->next = childList->next;
        childList->next = node;
    }

    AstarNode* pop() {
        AstarNode* temp = childList->next;
        childList->next = childList->next->next;
        return temp;
    }
//Check if the currentNode shows up before in p
    bool checkAncestors(AstarNode* currentNode, AstarNode* p) {
        if (currentNode->parent == NULL) {
            return false;
        }
        else {
            if (match(currentNode->configuration, p->configuration)) {
                return true;
            }
            if (p->parent == NULL) {
                return false;
            }
            return checkAncestors(currentNode, p->parent);
        }
    }
//Create the childList
    void constructChildList(AstarNode* currentNode) {
        childList = new AstarNode();
        int spot;

        // the four actions are moving the empty tile right, down, left, and up.
        int moveset1[9] = { 0 };
        int moveset2[9] = { 0 };
        int moveset3[9] = { 0 };
        int moveset4[9] = { 0 };
        
        int move1;
        int move2;
        int move3;
        int move4;

        for (int i = 0; i < 9; i++) {
            if (currentNode->configuration[i] == 0) {
                spot = i;
            }
            moveset1[i] = currentNode->configuration[i];
            moveset2[i] = currentNode->configuration[i];
            moveset3[i] = currentNode->configuration[i];
            moveset4[i] = currentNode->configuration[i];
        }

        //Sequence of If statement to saperate different move for blank in different place
        // 0 1 2
        // 3 4 5
        // 6 7 8
        
        if (spot == 0 || spot == 2 || spot == 6 || spot == 8) {
            if (spot == 0) {
                move1 = 1;
                move2 = 3;
            }
            else if (spot == 2) {
                move1 = 1;
                move2 = 5;
            }
            else if (spot == 6) {
                move1 = 7;
                move2 = 3;
            }
            else if (spot == 8) {
                move1 = 7;
                move2 = 5;
            }
            
            moveset1[spot] = currentNode->configuration[move1];
            moveset1[move1] = currentNode->configuration[spot];
            
            moveset2[spot] = currentNode->configuration[move2];
            moveset2[move2] = currentNode->configuration[spot];
            
            AstarNode* child1 = new AstarNode(moveset1, currentNode);
            AstarNode* child2 = new AstarNode(moveset2, currentNode);
            
            if (!checkAncestors(child1, child1->parent)) {
                push(child1);
            }
            if (!checkAncestors(child2, child2->parent)) {
                push(child2);
            }
        }

        if (spot == 1 || spot == 3 || spot == 5 || spot == 7) {

            if (spot == 1) {
                move1 = 0;
                move2 = 2;
                move3 = 4;
            }
            else if (spot == 3) {
                move1 = 0;
                move2 = 4;
                move3 = 6;
            }
            else if (spot == 5) {
                move1 = 2;
                move2 = 4;
                move3 = 8;
            }
            else if (spot == 7) {
                move1 = 6;
                move2 = 4;
                move3 = 8;
            }
           
            moveset1[spot] = currentNode->configuration[move1];
            moveset1[move1] = currentNode->configuration[spot];
           
            moveset2[spot] = currentNode->configuration[move2];
            moveset2[move2] = currentNode->configuration[spot];
           
            moveset3[spot] = currentNode->configuration[move3];
            moveset3[move3] = currentNode->configuration[spot];
            
            AstarNode* child1 = new AstarNode(moveset1, currentNode);
            AstarNode* child2 = new AstarNode(moveset2, currentNode);
            AstarNode* child3 = new AstarNode(moveset3, currentNode);
            
            if (!checkAncestors(child1, child1->parent)) {
                push(child1);
            }
            if (!checkAncestors(child2, child2->parent)) {
                push(child2);
            }
            if (!checkAncestors(child3, child3->parent)) {
                push(child3);
            }
        }

        if (spot == 4) {
            move1 = 1;
            move2 = 3;
            move3 = 5;
            move4 = 7;
            
            moveset1[spot] = currentNode->configuration[move1];
            moveset1[move1] = currentNode->configuration[spot];
            
            moveset2[spot] = currentNode->configuration[move2];
            moveset2[move2] = currentNode->configuration[spot];
            
            moveset3[spot] = currentNode->configuration[move3];
            moveset3[move3] = currentNode->configuration[spot];

            moveset4[spot] = currentNode->configuration[move4];
            moveset4[move4] = currentNode->configuration[spot];
            
            AstarNode* child1 = new AstarNode(moveset1, currentNode);
            AstarNode* child2 = new AstarNode(moveset2, currentNode);
            AstarNode* child3 = new AstarNode(moveset3, currentNode);
            AstarNode* child4 = new AstarNode(moveset4, currentNode);
            
            if (!checkAncestors(child1, child1->parent)) {
                push(child1);
            }
            if (!checkAncestors(child2, child2->parent)) {
                push(child2);
            }
            if (!checkAncestors(child3, child3->parent)) {
                push(child3);
            }
            if (!checkAncestors(child4, child4->parent)) {
                push(child4);
            }
        }

    }

    void printSolution(AstarNode* currentNode) {
        if (currentNode->parent != NULL) {
            printSolution(currentNode->parent);
        }
        
        for (int i = 0; i < 9; i++) {
            if (currentNode->configuration[i] == 0) {
                cout<< " ";
            }else {
                cout<< currentNode->configuration[i];
            }
            
            if ((i + 1) % 3 == 0) {
                cout<< endl;
            }else {
                cout<< " ";
            }
        }
        
        cout<<endl;
    }
    
};
//====================================================
//          Functions for each algorithms
//====================================================

//this function calculates the optimal solution for A* Heurstic, and caculate the number of node for this A* Heurstic.
void for_AStarSearch(int* initial,int* end){
        AStarSearch* search = new AStarSearch(initial, end);
        search->computeStars(search->startNode);
        search->listInsert(search->startNode, search->OpenList);

        int prntcnt = 0;

        AstarNode* currentNode = new AstarNode();

        while (search->OpenList->next != NULL && !search->isGoalNode(currentNode) ){
            currentNode = search->remove(search->OpenList);
            search->listInsert(currentNode, search->CloseList);
            num_Node_for_A_star++;
            if (search->isGoalNode(currentNode)) {
                search->printSolution(currentNode);
            }

            search->constructChildList(currentNode);
            
            while (search->childList->next != NULL) {
                AstarNode* child = search->pop();
                search->computeStars(child);

                AstarNode* OpenScan = search->OpenList;
                AstarNode* CloseScan = search->CloseList;
                int flag = 0;
                while (OpenScan->next != NULL){
                    if (search->match(child->configuration, OpenScan->next->configuration)) {
                        flag = 1;
                        if (child->fStar < OpenScan->fStar) {
                            search->remove(OpenScan);
                            search->listInsert(child, search->OpenList);
                            child->parent = currentNode;
                            break;
                        }
                    }
                    OpenScan = OpenScan->next;
                }
                if (flag == 0) {
                    while (CloseScan->next != NULL){
                        if (search->match(child->configuration, CloseScan->next->configuration)) {
                            flag = 1;
                            search->remove(CloseScan);
                            search->listInsert(child, search->OpenList);
                            child->parent = currentNode;
                            break;
                        }
                        CloseScan = CloseScan->next;
                    }
                }
                if (flag == 0) {
                    search->listInsert(child, search->OpenList);
                    child->parent = currentNode;
                }
            }
            }

}
//this function calculates the optimal solution for A* manhattan, and caculate the number of node for this A* search manhattan.
void for_AStarSearchManhattan(int* initial,int* end){
        AStarSearch* search = new AStarSearch(initial, end);
        search->computemManhattan(search->startNode);
        search->listInsert(search->startNode, search->OpenList);

        int prntcnt = 0;

        AstarNode* currentNode = new AstarNode();

        while (search->OpenList->next != NULL && !search->isGoalNode(currentNode) ){
            currentNode = search->remove(search->OpenList);
            search->listInsert(currentNode, search->CloseList);
            num_Node_for_A_star_Manhattan++;
            //Check if the currentNode puzzle is the goal puzzle
            if (search->isGoalNode(currentNode)) {
                search->printSolution(currentNode);

            }
            //Compute the child list
            search->constructChildList(currentNode);

            while (search->childList->next != NULL) {
                AstarNode* child = search->pop();
                search->computemManhattan(child);

                AstarNode* OpenScan = search->OpenList;
                AstarNode* CloseScan = search->CloseList;
                int flag = 0;
                while (OpenScan->next != NULL){
                    if (search->match(child->configuration, OpenScan->next->configuration)) {
                        flag = 1;
                        if (child->fStar < OpenScan->fStar) {
                            search->remove(OpenScan);
                            search->listInsert(child, search->OpenList);
                            child->parent = currentNode;
                            break;
                        }
                    }
                    OpenScan = OpenScan->next;
                }
                if (flag == 0) {
                    while (CloseScan->next != NULL){
                        if (search->match(child->configuration, CloseScan->next->configuration)) {
                            flag = 1;
                            search->remove(CloseScan);
                            search->listInsert(child, search->OpenList);
                            child->parent = currentNode;
                            break;
                        }
                        CloseScan = CloseScan->next;
                    }
                }
                if (flag == 0) {
                    search->listInsert(child, search->OpenList);
                    child->parent = currentNode;
                }
            }
            }

}

//this function calculates the optimal solution for Depth-first Branch and Bound with the Manhattan heuristic function , and caculate the number of node for this DBnB manhattan.
void for_Depth_first_Branch_Manhattan(int initial[3][3],int end[3][3]) {
    int a, b;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++)
        {
            if (initial[i][j] == 0) {
                a = i;
                b = j;
            }
        }
    }
    // priority queue to store live nodes
    priority_queue<Node*, std::vector<Node*>, comp> open;
    // create a Node and calculate its cost
    Node* node = newNode(initial, a, b, a, b, 0, NULL);
    node->cost = for_DFS_manhattan(initial, end);
    //Add Node to list of live nodes;
    open.push(node);
    
    while (!open.empty()){
        
        Node* mini = open.top();
        open.pop();
        if (mini->cost == 0){
            printPath(mini);
            return;
        }
        
        for (int i = 0; i < 4; i++){
            if (isSafe(mini->a + row[i], mini->b + col[i])){
                //create the child node and calculate the cost
                Node* child = newNode(mini->matrix, mini->a,mini->b, mini->a + row[i],mini->b + col[i],mini->level + 1, mini);
               child->cost = for_DFS_manhattan(child->matrix, end);

                // Add child to list of live nodes
                open.push(child);
                num_Node_for_DFBnB_Manhattan++;
            }
        }
    }
}
//this function calculates the optimal solution for Iterative deepening A* with the Manhattan heuristic function, and also caculate the number of node for this IDA*.
void for_AStarIterativedeepening(int* initial,int* end){
        AStarSearch* search = new AStarSearch(initial, end);
        
        search->computemManhattan(search->startNode);
        search->listInsert(search->startNode, search->OpenList);

        int prntcnt = 0;
        int limit=search->startNode->fStar;
        
    
        AstarNode* currentNode = new AstarNode();
    while(true){
        while (search->OpenList->next != NULL ){
            currentNode = search->remove(search->OpenList);
            search->listInsert(currentNode, search->CloseList);
            num_Node_for_IDA_Manhattan++;
            
            //Check if the currentNode puzzle is the goal puzzle
            if (search->isGoalNode(currentNode)) {
                search->printSolution(currentNode);
                return;
            }
            //Compute the child list
            search->constructChildList(currentNode);
            while (search->childList->next != NULL) {
                AstarNode* child = search->pop();
                search->computemManhattan(child);
                
                //compare child->fStar with limit
                if(child->fStar>limit){
                    search->listInsert(child, search->temp);
                    search->remove(child);
                }else{
                    search->listInsert(child, search->OpenList);
                    child->parent = currentNode;
                }
            }
            
            }
        // Adding the node with the smallest fStar to the openList
        AstarNode* smallNode = search->remove(search->temp);
        search->listInsert(smallNode, search->OpenList);
        
        //updating the limit to the smallest fStart in openList
        AstarNode* fistNode=search->remove(search->OpenList);
        search->computemManhattan(fistNode);
        limit=fistNode->fStar;
        search->listInsert(fistNode, search->OpenList);
    }

}

//================================================================
//       Functions to out put all case for each algorithms
//================================================================
void AStarSearch_for_all_case(int* easy,int* medium,int* hard,int* worst,int* goal){
    cout<<"For Easy case: "<<endl;
    clock_t tStart1 = clock();
    for_AStarSearch(easy,goal);
    cout<<"Number of node: "<<num_Node_for_A_star<<endl;
    printf("Time taken: %.6fs\n", (double)(clock() - tStart1)/CLOCKS_PER_SEC);
    num_Node_for_A_star=0;
    cout<<endl;

    cout<<"For Medium case: "<<endl<<endl;
    clock_t tStart2 = clock();
    for_AStarSearch(medium,goal);
    cout<<"Number of node: "<<num_Node_for_A_star<<endl;
    printf("Time taken: %.6fs\n", (double)(clock() - tStart2)/CLOCKS_PER_SEC);
    num_Node_for_A_star=0;
    cout<<endl;

    cout<<"For Hard case: "<<endl<<endl;
    clock_t tStart3 = clock();
    for_AStarSearch(hard,goal);
    cout<<"Number of node: "<<num_Node_for_A_star<<endl;
    printf("Time taken: %.6fs\n", (double)(clock() - tStart3)/CLOCKS_PER_SEC);
    num_Node_for_A_star=0;
    cout<<endl;
    
    cout<<"For Worst case: "<<endl<<endl;
    clock_t tStart4 = clock();
    for_AStarSearch(worst,goal);
    cout<<"Number of node: "<<num_Node_for_A_star<<endl;
    printf("Time taken: %.6fs\n", (double)(clock() - tStart4)/CLOCKS_PER_SEC);
    num_Node_for_A_star=0;
    cout<<endl;

}
void AStarSearchManhattan_for_all_case(int* easy,int* medium,int* hard,int* worst,int* goal){
    cout<<"For Easy case: "<<endl;
    clock_t tStart1 = clock();
    for_AStarSearchManhattan(easy,goal);
    cout<<"Number of node: "<<num_Node_for_A_star_Manhattan<<endl;
    printf("Time taken: %.6fs\n", (double)(clock() - tStart1)/CLOCKS_PER_SEC);
    num_Node_for_A_star_Manhattan=0;
    cout<<endl;

    cout<<"For Medium case: "<<endl<<endl;
    clock_t tStart2 = clock();
    for_AStarSearchManhattan(medium,goal);
    cout<<"Number of node: "<<num_Node_for_A_star_Manhattan<<endl;
    printf("Time taken: %.6fs\n", (double)(clock() - tStart2)/CLOCKS_PER_SEC);
    num_Node_for_A_star_Manhattan=0;
    cout<<endl;

    cout<<"For Hard case: "<<endl<<endl;
    clock_t tStart3 = clock();
    for_AStarSearchManhattan(hard,goal);
    cout<<"Number of node: "<<num_Node_for_A_star_Manhattan<<endl;
    printf("Time taken: %.6fs\n", (double)(clock() - tStart3)/CLOCKS_PER_SEC);
    num_Node_for_A_star_Manhattan=0;
    cout<<endl;
    
    cout<<"For Worst case: "<<endl<<endl;
    clock_t tStart4 = clock();
    for_AStarSearchManhattan(worst,goal);
    cout<<"Number of node: "<<num_Node_for_A_star_Manhattan<<endl;
    printf("Time taken: %.6fs\n", (double)(clock() - tStart4)/CLOCKS_PER_SEC);
    cout<<endl;

}

void AStarIterativedeepening_for_all_case(int* easy,int* medium,int* hard,int* worst,int* goal){
    cout<<"For Easy case: "<<endl;
    clock_t tStart1 = clock();
    for_AStarIterativedeepening(easy,goal);
    cout<<"Number of node: "<<num_Node_for_IDA_Manhattan<<endl;
    printf("Time taken: %.6fs\n", (double)(clock() - tStart1)/CLOCKS_PER_SEC);
    num_Node_for_A_star_Manhattan=0;
    cout<<endl;

    cout<<"For Medium case: "<<endl<<endl;
    clock_t tStart2 = clock();
    for_AStarIterativedeepening(medium,goal);
    cout<<"Number of node: "<<num_Node_for_IDA_Manhattan<<endl;
    printf("Time taken: %.6fs\n", (double)(clock() - tStart2)/CLOCKS_PER_SEC);
    num_Node_for_A_star_Manhattan=0;
    cout<<endl;

    cout<<"For Hard case: "<<endl<<endl;
    clock_t tStart3 = clock();
    for_AStarIterativedeepening(hard,goal);
    cout<<"Number of node: "<<num_Node_for_IDA_Manhattan<<endl;
    printf("Time taken: %.6fs\n", (double)(clock() - tStart3)/CLOCKS_PER_SEC);
    num_Node_for_A_star_Manhattan=0;
    cout<<endl;
    
    cout<<"For Worst case: "<<endl<<endl;
    clock_t tStart4 = clock();
    for_AStarIterativedeepening(worst,goal);
    cout<<"Number of node: "<<num_Node_for_IDA_Manhattan<<endl;
    printf("Time taken: %.6fs\n", (double)(clock() - tStart4)/CLOCKS_PER_SEC);
    cout<<endl;

}

void Depth_first_Branch_Manhattan_for_all_case(){
    
    int easy[3][3]={{1, 3, 4},{8, 6, 2},{7, 0, 5}};
    int medium[3][3] ={{2, 8, 1},{0, 4, 3},{7, 6, 5}};
    int hard[3][3] ={{2, 8, 1},{4, 6, 3},{0, 7, 5}};
    int worst[3][3] ={{5, 6, 7},{4, 0, 8},{3, 2, 1}};
    int goal[3][3] ={{1, 2, 3},{8, 0, 4},{7, 6, 5}};

    cout<<"For Easy case: "<<endl;
    clock_t tStart1 = clock();
    for_Depth_first_Branch_Manhattan(easy,goal);
    cout<<"Number of node: "<<num_Node_for_DFBnB_Manhattan<<endl;
    printf("Time taken: %.6fs\n", (double)(clock() - tStart1)/CLOCKS_PER_SEC);
    cout<<endl;

    cout<<"For Medium case: "<<endl<<endl;
    clock_t tStart2 = clock();
    for_Depth_first_Branch_Manhattan(medium,goal);
    cout<<"Number of node: "<<num_Node_for_DFBnB_Manhattan<<endl;
    printf("Time taken: %.6fs\n", (double)(clock() - tStart2)/CLOCKS_PER_SEC);
    cout<<endl;

    cout<<"For Hard case: "<<endl<<endl;
    clock_t tStart3 = clock();
    for_Depth_first_Branch_Manhattan(hard,goal);
    cout<<"Number of node: "<<num_Node_for_DFBnB_Manhattan<<endl;
    printf("Time taken: %.6fs\n", (double)(clock() - tStart3)/CLOCKS_PER_SEC);
    cout<<endl;

    cout<<"For Worst case: "<<endl<<endl;
    clock_t tStart4 = clock();
    for_Depth_first_Branch_Manhattan(worst,goal);
    cout<<"Number of node: "<<num_Node_for_DFBnB_Manhattan<<endl;
    printf("Time taken: %.6fs\n", (double)(clock() - tStart4)/CLOCKS_PER_SEC);
    cout<<endl;
}

int main(){
    int easy[]={1,3,4,8,6,2,7,0,5};
    int medium[]={2,8,1,0,4,3,7,6,5};
    int hard[]={2,8,1,4,6,3,0,7,5};
    int worst[]={5,6,7,4,0,8,3,2,1};
    
    int goal[]={1,2,3,8,0,4,7,6,5};

    cout<<"============= For A* search using heuristic function ============="<<endl;
    AStarSearch_for_all_case(easy,medium,hard,worst,goal);

//    cout<<"============= For A* search using the Manhattan heuristic function ============="<<endl;
//    AStarSearchManhattan_for_all_case(easy,medium,hard,worst,goal);
//
//    cout<<"============= For Iterative deepening A* with the Manhattan heuristic function ============="<<endl;
//    AStarIterativedeepening_for_all_case(easy,medium,hard,worst,goal);
//
//    cout<<"============= For Depth-first Branch and Bound with the Manhattan heuristic function ============="<<endl;
//    Depth_first_Branch_Manhattan_for_all_case();

    
    return 0;

}
