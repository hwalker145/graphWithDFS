#include <iostream> // for console io

/**
 * @brief Edge struct
 * has integer values for source node and destination
 */
struct Edge{
    int source, destination;
};

/**
 * @brief Adjacency list node struct
 * holds next node as ptr and data as int
 */
struct AdjNode{
    int data;
    AdjNode* ptr;
};

/**
 * @brief Holds info for the DFS stacked vertices
 *  no need to hold data, it's indexed as an array
 *  so the index is equal to the data value...
 */
struct DFSVertex{
    int pop;
    int push;
};

/**
 * @brief graph class
 * 
 */
class Graph{
    int _N; // number of nodes, can be changed
    AdjNode* listHead; // array of AdjNodes to represent all the
                        // sources in the adjacency list

    void setN(int N) { _N = N; } // mutator for node count, not totally necessary
    int getN() const { return _N; }

    /**
     * @brief Adds a vertex to the adj list as a node...
     *      works like so: the array-indexed Adjnode is the source
     *      and the stack LL of Adjnodes it points to represents
     *      the destinations
     * 
     * @param source int for the source end of the edge to be 
     *              represented by this list
     * @param destination int for the end of the edge.
     * @return AdjNode* used as an indirect way to point the
     *          array-indexed part of the list to most recent dest.
     */
    AdjNode* addAdjNode(int source, int destination) {
        // below checks if the stack is empty
        if(listHead[source].ptr != nullptr) {
            // if it is NOT empty, more complex steps here.
            // the standard stack insertion, I believe
            AdjNode* tempPtr = listHead[source].ptr;
            AdjNode* newNode = new AdjNode;

            newNode->data = destination;
            newNode->ptr = tempPtr;
            tempPtr = nullptr;

            return newNode;
        }
        else {
            // empty stack, much simpler
            listHead[source].ptr = new AdjNode;
            listHead[source].ptr->data = destination;
            listHead[source].ptr->ptr = nullptr;

            return listHead[source].ptr;
        }
        // this function always returns a ptr to the new node
    }

    /**
     * @brief This is a recursive function to search the graph starting
     *      with one node. This function does not completely search a 
     *      disconnected graph. The container will take care of that
     * 
     * @param _pushNo indirect reference to the push number
     * @param _popNo indirect reference to the pop number
     * @param _node indirect reference to the dfs'd node index
     * @param _arr the array of dfs values
     */
    void dfs(int &_pushNo, int &_popNo, int _node, DFSVertex* _arr) {
        // makes a traversal pointer out of the head of the 
        // _node's adjacency list LL
        AdjNode* travPtr = listHead[_node].ptr;
        // mark the node
        _arr[_node].push = _pushNo++;
        // checks if the list is empty or not
        while(travPtr != nullptr) {
            if(!_arr[travPtr->data].push) {
                
                // search from that node
                dfs(_pushNo, _popNo, travPtr->data, _arr);
            }
            // move to the next one in the adj list and then check condition above
            travPtr = travPtr->ptr;
        }
        // if list is exhausted, pop the node.
        // The recursive ascent will go to the last call's travPtr = travPtr->ptr next
        _arr[_node].pop = _popNo++;
    }

public:
    /**
     * @brief Construct a new Graph object
     * 
     * @param N count of nodes/vertices
     * @param n edge count
     * @param edges array holding edge structs
     */
    Graph(int N, int n, Edge* edges) {
        setN(N);
        // initializes the adjlist's head array
        listHead = new AdjNode[getN()];
        for(int i = 0; i < N; i++) {
            // nodes as sources in the adjlist are in array structure
            listHead[i].data = i;
            listHead[i].ptr = nullptr;
        }

        // so they are easy to index ^^ 
        // i.e. no searching in the list with O(n^2) complexity.
        int nodeIndex;
        for(int i = 0; i < n; i++) {
            // takes the edges array and builds the adjlist using its data
            nodeIndex = edges[i].source;
            // the above line is used for aesthetics
            listHead[nodeIndex].ptr = addAdjNode(listHead[nodeIndex].data, edges[i].destination);
        }
    }

    /**
     * @brief Reports the adjlist
     * 
     */
    void displayList() const {
        AdjNode* travPtr;
        std::cout << "The adjacency list:\n";
        for(int i = 0; i < getN(); i++) {
            travPtr = (listHead + i);
            std::cout << (char)(travPtr->data + 'A') << "  -->  ";
            while(travPtr->ptr != nullptr) {
                std::cout << (char)(travPtr->ptr->data + 'A') << "  ";
                travPtr = travPtr->ptr;
            }
            std::cout << "\n\n";
        }
    }

    /**
     * @brief Wrapper function which starts the dfs process
     * and makes the topmost recursive call
     * 
     */
    void dfsWrapper() {
        // initializesn the dfs array
        DFSVertex arr[getN()];
        int pushNo = 1;
        int popNo = 1;

        // makes all the vertices have zero at push and pop
        for(int i = 0; i < getN(); i++)
        {
            arr[i].push = 0;
            arr[i].pop = 0;
        }

        // for each vertex if it hasn't been visited
        for(int i = 0; i < getN(); i++)
        {
            if(!arr[i].push)
            {
                // this function populates the dfs array
                dfs(pushNo, popNo, i, arr);
            }
        }

        std::cout << "The dfs results:\n";
        // spits it all out
        for(int i = 0; i < getN(); i++)
        {
            std::cout << arr[i].push << "  " << (char)(i + 'A') << "  " << arr[i].pop << "\n\n";
        }
    }

    
};

int main() {
    int GRAPH_VERTICES = 6;
    int GRAPH_EDGES = 10;

    Edge edges[GRAPH_EDGES];
    edges[0] = (struct Edge){0, 1};
    edges[1] = (struct Edge){1, 2};
    edges[2] = (struct Edge){2, 3};
    edges[3] = (struct Edge){2, 5};
    edges[4] = (struct Edge){0, 2};
    edges[5] = (struct Edge){5, 3};
    edges[6] = (struct Edge){3, 5};
    edges[7] = (struct Edge){3, 4};
    edges[8] = (struct Edge){5, 1};
    edges[9] = (struct Edge){1, 4};

    Graph G(GRAPH_VERTICES, GRAPH_EDGES, edges);
    G.displayList();
    G.dfsWrapper();

    return 0;
}