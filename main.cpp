#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <cmath>
#include <fstream>
#include <time.h>  

using namespace std;

static int level_total=0;
static int node_count = 0;
static int total_node = 0;
static clock_t t;

class Node
{
public:
    string S;
    int node_number;
    int symbol_number;
    int level;
    Node* next;
    Node(){
        node_number = ++node_count;
        next = NULL;
    }
};

class Queue{
    Node* front;
    Node* back;
public:
    Queue(){
        front = NULL;
        back = NULL;
    }
    void push_back(Node&);
    bool empty();
    Node front_val();
    void pop_front();
    ~Queue();
};

Queue::~Queue(){
    Node* temp = front;
    while(temp != NULL)
    {
        front = temp->next;
        delete temp;
    }
}

Node Queue::front_val()
{
    return *front;
}

void Queue::pop_front()
{
    Node* temp = front->next;
    delete front;
    front = temp;
}

bool Queue::empty() {
    return (front == NULL);
}

void Queue::push_back(Node &v) {
    if (front == NULL){
        front = new Node;
        front->symbol_number = v.symbol_number;
        front->node_number = v.node_number;
        front->level = v.level;
        front->S = v.S;
        back = front;
        return;
    }

    Node* new_node = new Node;
    new_node->symbol_number = v.symbol_number;
    new_node->node_number = v.node_number;
    new_node->level = v.level;
    new_node->S = v.S;
    back->next = new_node;
    back = new_node;
}

class Graph
{
public:
    map<int, bool> visited;
    map<int, vector<Node> > adj;
    string first_input;
    string second_input;
    string output;
    string concatted_string;
    string algorithm;
    string output_file_name;
    int visited_node_amount;
    void check_cond(Node& candidate);
    void addEdge(int v, Node &w);
    void write_to_file(map<char,int>);
    void DFS(Node& v);
    void BFS(vector <Node> v);
    Graph(){
        visited_node_amount = 0;
    }
};

void Graph::write_to_file(map<char,int> letters_map)
{
    t = clock() - t; // calculate running time
    ofstream myfile;
    myfile.open(output_file_name + ".txt");
    myfile << "Algorithm: " << algorithm << endl;
    myfile << "Number of the visited nodes: " << visited_node_amount << endl;
    myfile << "Maximum number of nodes kept in the memory: " << total_node << endl;
    myfile << "Running time: " << to_string(((float)t)/CLOCKS_PER_SEC) + " seconds" << endl;
    
    string result = "";

    int len_of_str = concatted_string.length();

    for(int i=0;i<len_of_str;i++){
        if (i == len_of_str - 1) {
            result = result + concatted_string[i] + ": " + to_string(letters_map[concatted_string[i]]);
        } else {
            result = result + concatted_string[i] + ": " + to_string(letters_map[concatted_string[i]]) + ", ";
        }
    }
    myfile << "Solution: " << result << endl;
    myfile.close();
}

void Graph::addEdge(int v, Node &w)
{
    adj[v].push_back(w); 
}

void Graph::BFS(vector <Node> v)
{
    Queue queue;
    int v_size = v.size();

    for(int a=0; a < v_size; a++)
    {
        visited[v[a].node_number] = true;
        queue.push_back(v[a]);
    }

    while(!queue.empty())
    {
        Node x = queue.front_val();
        queue.pop_front();
        visited_node_amount++;
        if(x.level == level_total)
        {
            check_cond(x);
        }

        for (vector<Node>::iterator i = adj[x.node_number].begin(); i != adj[x.node_number].end(); ++i)
        {
            queue.push_back(*i);

            if (!visited[i->node_number])
            {
                visited[i->node_number] = true;
            }
        }
    }
}

void Graph::DFS(Node& v)
{
    visited_node_amount++;
    visited[v.node_number] = true;
    vector<Node>::iterator i;
    for (i = adj[v.node_number].begin(); i != adj[v.node_number].end(); i++)
    {
        if(i->level == level_total)
        {
            check_cond(*i);
        }
        if (!visited[(i->node_number)])
        {
            DFS(*i);
        }
    }
}

bool uniqueCharacters(string str)
{
    int str_len = str.length();
    for (int i = 0; i < str_len - 1; i++) {
        for (int j = i + 1; j < str_len; j++) {
            if (str[i] == str[j]) {
                return false;
            }
        }
    }
    return true;
}

void Graph::check_cond(Node& candidate)
{
    string num = to_string(candidate.symbol_number);
    int uu = num.length();
    for(int i=0;i < level_total - uu; i++)
    {
        num = '0' + num;
    }
    bool is_diff = uniqueCharacters(num);

    if(is_diff)
    {
        map<char,int> letters_map;
        for(int i=0;i<level_total;i++)
        {
            letters_map[concatted_string[i]] = num[i] - '0';
        }

        int let_1 = 0;
        int let_2 = 0;
        int out_let = 0;

        int first_len = first_input.length();
        int second_len = second_input.length();
        int output_len = output.length();

        for(int i=0; i<first_len ;i++)
        {
            int let_val = letters_map[first_input[first_input.length()-i-1]];
            let_1 += (int) pow(10,i) * let_val;
        }

        for(int i=0;i<second_len;i++)
        {
            int let_val = letters_map[second_input[second_input.length()-1-i]];
            let_2 += (int) pow(10,i) * let_val;
        }

        for(int i=0;i<output_len;i++)
        {
            int let_val = letters_map[output[output.length()-1-i]];
            out_let += (int) pow(10,i) * let_val;
        }

        string let_1_s = to_string(let_1);
        string let_2_s = to_string(let_2);
        string output_s = to_string(out_let);

        int let_1_s_count = let_1_s.length();
        int let_2_s_count = let_2_s.length();
        int output_s_count = output_s.length();
        
        if((let_1 + let_2 == out_let) && (let_1_s_count == first_len) && (let_2_s_count == second_len) && (output_s_count == output_len))
        {
            write_to_file(letters_map);
            exit(1);
        }
    }
}

string removeDuplicates(string str)
{
    string s = str;
    for ( std::string::size_type i = 0; i < s.size(); i++ )
    {
        string::size_type j = i + 1;
        while ( j < s.size() )
        {
            if ( s[i] == s[j] )
            {
                s.erase( j, 1 );
            }
            else
            {
                ++j;
            }
        }
    }
    return s;
}

int main(int argc, char** argv) {
    int curr_Val_Asl = 0;

    if (argc != 6)
    {
        cout << "Not typed well!" << endl;
        exit(1);
    }

    string algorithm = argv[1];
    string first_input = argv[2];
    string second_input = argv[3];
    string output = argv[4];
    string output_file = argv[5];

    string concatted_string = first_input + second_input + output;
    concatted_string = removeDuplicates(concatted_string);
    int total_letters_amount = concatted_string.length();
    Graph g;
    g.first_input = first_input;
    g.second_input = second_input;
    g.output = output;
    g.concatted_string = concatted_string;
    g.algorithm = algorithm;
    g.output_file_name = output_file;

    int current_node_number = 0;

    level_total = total_letters_amount;

    for(int i=0; i<total_letters_amount; i++)
    {
        int number_of_nodes_in_layer = pow(10,i);
        for (int j=0; j < number_of_nodes_in_layer;j++)
        {
            bool isUnique = false;
            for(int z=0; z<10; z++)
            {
                string val = to_string(curr_Val_Asl);
                isUnique = uniqueCharacters(val);
                if(isUnique){
                    Node new_node;
                    new_node.S = concatted_string[i];
                    new_node.symbol_number = curr_Val_Asl;
                    new_node.level = i+1;
                    g.addEdge(current_node_number,new_node);
                }
                curr_Val_Asl++;
            }
            if(isUnique)
            {
                current_node_number++;
            }
        }
        curr_Val_Asl = 0;
    }

    total_node = node_count;

    if(algorithm == "BFS")
    {
        t = clock();
        g.BFS(g.adj[0]);
    }
    else if (algorithm == "DFS")
    {
        Node root; //this is the root node
        root.S = "";
        root.symbol_number = 0;
        root.level = 0;
        root.node_number = 0;
        t = clock(); // start time of the searching
        g.DFS(root);
    } 
    else 
    {
        cout << "There is no algorithm called: " << algorithm << endl;
        exit(1);
    }
    return 0;
}