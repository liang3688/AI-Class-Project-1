#include <iostream>
#include <queue>
#include <vector>
#include <cmath>

using namespace std;

const int ROW = 3;
const int COL = 3;

enum ALGORITHM{UNIFORM, MISPLACED, EUCLIDEAN};

enum MOVES{UP, DOWN, LEFT, RIGHT};

class Node{
public:
    int rep[ROW][COL];
    int g;
    int h;
    int f;
    int operation;
    Node *parent;
    Node *operation_up;
    Node *operation_down;
    Node *operation_left;
    Node *operation_right;

    Node(){
        for(int i = 0; i < ROW; i++)
        {
            for(int j = 0; j < COL; j++)
                rep[i][j] = 0;
        }
        g = 0;
        h = 0;
        f = 0;
        operation = 0;
        parent = NULL;
        operation_up = NULL;
        operation_down = NULL;
        operation_left = NULL;
        operation_right = NULL;
    }

    Node(Node *temp, int op){
        for(int i = 0; i < ROW; i++)
        {
            for(int j = 0; j < COL; j++)
                rep[i][j] = temp->rep[i][j];
        }
        g = temp->g+1;
        h = 0;
        f = 0;
        this->parent = temp;
        this->operation_up = NULL;
        this->operation_down = NULL;
        this->operation_left = NULL;
        this->operation_right = NULL;
        if(op == UP){
            operation = UP;
            temp->operation_up = this;
        }
        else if(op == DOWN){
            operation = DOWN;
            temp->operation_down = this;
        }
        else if(op == LEFT){
            operation = LEFT;
            temp->operation_left = this;
        }
        else if(op == RIGHT){
            operation = RIGHT;
            temp->operation_right = this;
        }
    }

    void get_h(Node goal, int algorithm){
        h = 0;
        if(algorithm == MISPLACED){
            for(int i = 0; i < ROW; i++)
            {
                for(int j = 0; j < COL; j++)
                {
                    if(rep[i][j] != goal.rep[i][j] && goal.rep[i][j] != 0)
                        h++;
                }
            }
        }

        if(algorithm == EUCLIDEAN){
            int row = 0;
            int col = 0;
            int e = 0;
            int x = 0;
            int y = 0;
            for(int i = 0; i < ROW; i++)
            {
                for(int j = 0; j < COL; j++)
                {
                    if(rep[i][j] != goal.rep[i][j] && rep[i][j] != 0)
                    {
                        row = goal.find_row(rep[i][j]);
                        col = goal.find_col(rep[i][j]);
                        if(row != i){
                            if(row - i > 0)
                                x = x + (row-i);
                            else
                                x = x - (row-i);
                        }
                        if(col != j){
                            if(col - j > 0)
                                y = y + (col-j);
                            else
                                y = y - (col-j);
                        }
                        e = sqrt(pow(x, 2) + pow(y, 2));
                        h = h + e;
                        x = 0;
                        y = 0;
                    }
                }
            }
        }
    }

    void get_f(){
        f = g + h;
    }

    int find_row(const int num){
        for(int i = 0; i < ROW; i++)
        {
            for(int j = 0; j < COL; j++)
            {
                if(rep[i][j] == num)
                    return i;
            }
        }
    }

    int find_col(const int num){
        for(int i = 0; i < ROW; i++)
        {
            for(int j = 0; j < COL; j++)
            {
                if(rep[i][j] == num)
                    return j;
            }
        }
    }

    Node copy(Node a){
        for(int i = 0; i < ROW; i++)
        {
            for(int j = 0; j < COL; j++)
                rep[i][j] = a.rep[i][j];   
        }
        g = a.g;
        h = a.h;
        f = a.f;
        return *this;
    }

    bool is_same(const Node a) const {
        for(int i = 0; i < ROW; i++)
        {
            for(int j = 0; j < COL; j++)
            {
                if(rep[i][j] != a.rep[i][j])
                    return false;
            }
        }
        return true;
    }

    void print() const {
        for(int i = 0; i < ROW; i++)
        {
            for(int j = 0; j < COL; j++)
                cout << rep[i][j] << " ";
            cout << endl;
        }
        cout << endl;
    }

    void print_operations(){
        if(parent != NULL){
            parent->print_operations();
            if(operation == UP)
                cout << "Move up\n";
            else if(operation == DOWN)
                cout << "Move down\n";
            else if(operation == LEFT)
                cout << "Move left\n";
            else if(operation == RIGHT)
                cout << "Move right\n";
            print();
        }
        cout << endl;
    }
};

class Comparator{
public:
    bool operator()(const Node* n1, const Node* n2){
        return (n1->f > n2->f);
    }
};

bool check_unique(Node *state, priority_queue<Node*, vector<Node*>, Comparator>frontier, const vector<Node> record)
{
    bool is_unique = true;
    Node temp;
    while(!frontier.empty())
    {
        temp.copy(*frontier.top());
        frontier.pop();
        if(state->is_same(temp))
            is_unique = false;
    }
    for(unsigned int i = 0; i < record.size(); i++){
        if(state->is_same(record[i]))
            is_unique = false;
    }
    return is_unique;
}

void update_pq_size(priority_queue<Node*, vector<Node*>, Comparator> pq, unsigned int &pq_max)
{
    if(pq.size() > pq_max)
        pq_max = pq.size();
}

class Problem{
public:
    Node init_state;
    Node goal_state;

    void option_1(){
        init_state = Node();
        goal_state = Node();
        int k = 1;
        for(int i = 0; i < ROW; i++)
        {
            for(int j = 0; j < COL; j++)
            {
                init_state.rep[i][j] = k;
                goal_state.rep[i][j] = k;
                k++;
            }
        }
        init_state.rep[1][0] = 4;
        init_state.rep[1][1] = 8;
        init_state.rep[1][2] = 0;
        init_state.rep[2][0] = 7;
        init_state.rep[2][1] = 6;
        init_state.rep[2][2] = 5;
        goal_state.rep[2][2] = 0;
        init_state.parent = nullptr;
    }

    void option_2(int a[][COL]){
        init_state = Node();
        goal_state = Node();
        int k = 1;
        for(int i = 0; i < ROW; i++)
        {
            for(int j = 0; j < COL; j++)
            {
                init_state.rep[i][j] = a[i][j];
                goal_state.rep[i][j] = k;
                k++;
            }
        }
        goal_state.rep[ROW-1][COL-1] = 0;
        init_state.parent = nullptr;
    }

    void op_up(Node *state, priority_queue<Node*, vector<Node*>, Comparator>&frontier, const vector<Node> record, int algorithm){

        if(state->find_row(0) != 0)
        {
            Node *newnode = new Node(state, UP);
            int row = state->find_row(0);
            int col = state->find_col(0);
            newnode->rep[row][col] = state->rep[row-1][col];
            newnode->rep[row-1][col] = 0;
            if(algorithm == UNIFORM)
                newnode->h = 0;
            else if(algorithm == MISPLACED)
                newnode->get_h(goal_state, MISPLACED);
            else if(algorithm == EUCLIDEAN)
                newnode->get_h(goal_state, EUCLIDEAN);
            newnode->get_f();
            if(check_unique(newnode, frontier, record) && !state->is_same(*newnode))
                frontier.push(newnode);
        }
    }

    void op_down(Node *state, priority_queue<Node*, vector<Node*>, Comparator>&frontier, const vector<Node> record, int algorithm){

        if(state->find_row(0) != (ROW-1))
        {
            Node *newnode = new Node(state, DOWN);
            int row = state->find_row(0);
            int col = state->find_col(0);
            newnode->rep[row][col] = state->rep[row+1][col];
            newnode->rep[row+1][col] = 0;
            if(algorithm == UNIFORM)
                newnode->h = 0;
            else if(algorithm == MISPLACED)
                newnode->get_h(goal_state, MISPLACED);
            else if(algorithm == EUCLIDEAN)
                newnode->get_h(goal_state, EUCLIDEAN);
            newnode->get_f();
            if(check_unique(newnode, frontier, record) && !state->is_same(*newnode))
                frontier.push(newnode);
        }
    }

    void op_left(Node *state, priority_queue<Node*, vector<Node*>, Comparator>&frontier, const vector<Node> record, int algorithm){

        if(state->find_col(0) != 0)
        {
            Node *newnode = new Node(state, LEFT);
            int row = state->find_row(0);
            int col = state->find_col(0);
            newnode->rep[row][col] = state->rep[row][col-1];
            newnode->rep[row][col-1] = 0;
            if(algorithm == UNIFORM)
                newnode->h = 0;
            else if(algorithm == MISPLACED)
                newnode->get_h(goal_state, MISPLACED);
            else if(algorithm == EUCLIDEAN)
                newnode->get_h(goal_state, EUCLIDEAN);
            newnode->get_f();
            if(check_unique(newnode, frontier, record) && !state->is_same(*newnode))
                frontier.push(newnode);
        }
    }
    void op_right(Node *state, priority_queue<Node*, vector<Node*>, Comparator>&frontier, const vector<Node> record, int algorithm){

        if(state->find_col(0) != (COL-1))
        {
            Node *newnode = new Node(state, RIGHT);
            int row = state->find_row(0);
            int col = state->find_col(0);
            newnode->rep[row][col] = state->rep[row][col+1];
            newnode->rep[row][col+1] = 0;
            if(algorithm == UNIFORM)
                newnode->h = 0;
            else if(algorithm == MISPLACED)
                newnode->get_h(goal_state, MISPLACED);
            else if(algorithm == EUCLIDEAN)
                newnode->get_h(goal_state, EUCLIDEAN);
            newnode->get_f();
            if(check_unique(newnode, frontier, record) && !state->is_same(*newnode))
                frontier.push(newnode);
        }
    }

    void print_state(Node state){
        state.print();
    }

};

bool UCS(Problem p){
    Node *state = new Node();
    bool print = false;
    unsigned int max = 0;
    unsigned int total = 0;
    state->copy(p.init_state);
    vector<Node> explored;
    priority_queue<Node*, vector<Node*>, Comparator>frontier;
    frontier.push(state);
    while(!frontier.empty()){
        state = frontier.top();
        if(print)
            cout << "The best state to expand with g(n) = " << state->g << " and h(n) = " << state->h << " is...\n";
        state->print();
        frontier.pop();
        if(state->is_same(p.goal_state)){
            cout << "Goal!!!\n\n\nInitial State:\n";
            p.init_state.print();
            state->print_operations();
            cout << "The search algorithm expanded a total of " << total << " nodes to solve this problem.\n"
                    "The maximum number of nodes in the queue at any one time: " << max << endl;
            return true;
        }
        cout << "Expanding this node...\n";
        p.op_up(state, frontier, explored, UNIFORM);
        p.op_down(state, frontier, explored, UNIFORM);
        p.op_left(state, frontier, explored, UNIFORM);
        p.op_right(state, frontier, explored, UNIFORM);
        total++;
        update_pq_size(frontier, max);
        explored.push_back(*state);
        print = true;
    }
    return false;
}

bool MT(Problem p){
    Node *state = new Node();
    bool print = false;
    unsigned int max = 0;
    unsigned int total = 0;
    state->copy(p.init_state);
    vector<Node> explored;
    priority_queue<Node*, vector<Node*>, Comparator>frontier;
    frontier.push(state);
    while(!frontier.empty()){
        state = frontier.top();
        if(print)
            cout << "The best state to expand with g(n) = " << state->g << " and h(n) = " << state->h << " is...\n";
        state->print();
        frontier.pop();
        if(state->is_same(p.goal_state)){
            cout << "Goal!!!\n\n\nInitial State:\n";
            p.init_state.print();
            state->print_operations();
            cout << "The search algorithm expanded a total of " << total << " nodes to solve this problem.\n"
                    "The maximum number of nodes in the queue at any one time: " << max << endl;
            return true;
        }
        cout << "Expanding this node...\n";
        p.op_up(state, frontier, explored, MISPLACED);
        p.op_down(state, frontier, explored, MISPLACED);
        p.op_left(state, frontier, explored, MISPLACED);
        p.op_right(state, frontier, explored, MISPLACED);
        total++;
        update_pq_size(frontier, max);
        explored.push_back(*state);
        print = true;
    }
    return false;
}

bool ED(Problem p){
    Node *state = new Node();
    bool print = false;
    unsigned int max = 0;
    unsigned int total = 0;
    state->copy(p.init_state);
    vector<Node> explored;
    priority_queue<Node*, vector<Node*>, Comparator>frontier;
    frontier.push(state);
    while(!frontier.empty()){
        state = frontier.top();
        if(print)
            cout << "The best state to expand with g(n) = " << state->g << " and h(n) = " << state->h << " is...\n";
        state->print();
        frontier.pop();
        if(state->is_same(p.goal_state)){
            cout << "Goal!!!\n\n\nInitial State:\n";
            p.init_state.print();
            state->print_operations();
            cout << "The search algorithm expanded a total of " << total << " nodes to solve this problem.\n"
                    "The maximum number of nodes in the queue at any one time: " << max << endl;
            return true;
        }
        cout << "Expanding this node...\n";
        p.op_up(state, frontier, explored, EUCLIDEAN);
        p.op_down(state, frontier, explored, EUCLIDEAN);
        p.op_left(state, frontier, explored, EUCLIDEAN);
        p.op_right(state, frontier, explored, EUCLIDEAN);
        total++;
        update_pq_size(frontier, max);
        explored.push_back(*state);
        print = true;
    }
    return false;
}

int main()
{
    int input;
    Problem puzzle;
    cout << "Welcome to 862128237 8 puzzle solver.\n"
            "\nType \"1\" to use a default puzzle, or \"2\" to enter your own puzzle.\n";
    cin >> input;
    if(input == 1)
        puzzle.option_1();
    else if(input == 2)
    {
        cout << "\nEnter your puzzle, use a zero to represent a blank.\n";
        int a[ROW][COL];
        cout << "\nEnter the first row, use space or tabs between numbers\t";
        cin >> a[0][0] >> a[0][1] >> a[0][2];
        cout << "\nEnter the second row, use space or tabs between numbers\t";
        cin >> a[1][0] >> a[1][1] >> a[1][2];
        cout << "\nEnter the third row, use space or tabs between numbers\t";
        cin >> a[2][0] >> a[2][1] >> a[2][2];
        puzzle.option_2(a);
    }
    else{
        cout << "Invalid input, program terminated.\n";
        exit(0);
    }
    cout << "\nEnter the choice of algorithm:\n"
            "\n1)Uniform Cost Search\n"
            "2)A* with Misplaced Tile heuristic\n"
            "3)A* with Eucledian distance heuristic\n";
    cin >> input;
    cout << endl;
    if(input == 1)
        UCS(puzzle);
    else if(input == 2)
        MT(puzzle);
    else if(input == 3)
        ED(puzzle);
    else
        cout << "Invalid input, program terminated.\n";

    return 0;
}
