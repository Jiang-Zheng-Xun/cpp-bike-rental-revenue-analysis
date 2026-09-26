#include <iostream>
#include <fstream>
#include <climits>
#include <string>
#include <cmath>

using namespace std;

//Global definition of fee policy
int* electric_fee = new int[2];
int* lady_fee = new int[2];
int* road_fee = new int[2];
int waiting_fee;
float discount_rate;
int transferring_fee;

//Graph begin
class Graph
{
private:
    int V;
    int** graph;

public:
    Graph(int V)
    {
        this->V = V;
        graph = new int*[V];
        for(int i=0; i<V; i++)
            graph[i]=new int[V];

        for(int i=0; i<V; i++)
            for(int j=0; j<V; j++)
                graph[i][j]=INT_MAX;
    }

    void addEdge(int src, int dest, int weight)
    {
        graph[src][dest]=weight;
        graph[dest][src]=weight;
    }

    void printSolution(int dist[]);
    void printSolution();

    int minDistance(int dist[], bool sptSet[]);

    int* dijkstra(int src);

};

void Graph::printSolution(int dist[])
{
    cout<<"Vertex \t\t  Distance"<<endl;
    for(int v=1; v<V; v++)
        cout<<v<<" \t\t "<<dist[v]<<endl;
}
void Graph::printSolution()
{
    cout<<"Vertex \t\t  Distance"<<endl;
    for(int v=1; v<V; v++)
        for(int i=1; i<V; i++)
            cout<<v<<" \t\t "<<i<<" \t\t "<<this->graph[v][i]<<endl;
}

int Graph::minDistance(int dist[], bool sptSet[])
{

    int min=INT_MAX;
    int min_index;

    for(int v=1; v<V; v++)
    {
        if(!sptSet[v] && dist[v]<=min)
        {
            min=dist[v];
            min_index=v;
        }
    }

    return min_index;
}

int* Graph::dijkstra(int src)
{
    int* dist=new int[V];
    bool* sptSet= new bool[V];
    dist[0]=INT_MAX;
    sptSet[0]=true;
    for(int v=1; v<V; v++)
        dist[v]=INT_MAX, sptSet[v]=false;

    dist[src]=0;

    for(int count=1; count<V-1; count++)
    {
        int u=minDistance(dist, sptSet);
        sptSet[u]=true;

        for(int v=1; v<V; v++)
        {
            if(!sptSet[v] && graph[u][v] && dist[u]!=INT_MAX && dist[u]+graph[u][v]<dist[v] && graph[u][v]!=INT_MAX)
                dist[v]=dist[u]+graph[u][v];
        }
    }

    return dist;
}
// Graph end

//Station begin
class Heap
{
private:
    int *harr;
    int capacity;
    int heap_size;
public:
    Heap(int cap)
    {
        harr=new int[cap];
        capacity=cap;
        heap_size=0;
        for(int i=0; i<cap; i++) harr[i]=INT_MAX;
    }

    int parent(int i){return (i-1)/2;}
    int left(int i) {return 2*i+1;}
    int right(int i) {return 2*i+2;}

    void insertKey(int k);
    void decreaseKey(int i, int new_val);
    int extractMin();
    void deleteKey(int i);
    void Heapify(int i);
    int getMin() {return harr[0];}
};

void swap(int *x, int *y)
{
    int temp = *x;
    *x=*y;
    *y=temp;
}

void Heap::insertKey(int k)
{
    if(heap_size==capacity)
    {
      cout<<"\n Overflow!\n";
      return;
    }

    heap_size++;
    int i=heap_size-1;
    harr[i]=k;
    while(i!=0 && harr[parent(i)]>harr[i])
    {
        swap(&harr[i], &harr[parent(i)]);
        i=parent(i);
    }
}

void Heap::decreaseKey(int i, int new_val)
{
    if(i>=heap_size)
    {
        cout<<"\n Error!\n";
        return;
    }

    harr[i]=new_val;
    while(i!=0 && harr[parent(i)]>harr[i])
    {
        swap(&harr[i], &harr[parent(i)]);
        i=parent(i);
    }
}

int Heap::extractMin()
{
    if(heap_size<=0) return INT_MAX;
    if(heap_size==1)
    {
        heap_size--;
        return harr[0];
    }

    int root = harr[0];
    harr[0]=harr[heap_size-1];
    heap_size--;
    Heapify(0);
    return root;
}

void Heap::deleteKey(int i)
{
    decreaseKey(i, INT_MIN);
    extractMin();
}

void Heap::Heapify(int i)
{
    int l=left(i);
    int r=right(i);
    int smallest=i;
    if(l<heap_size && harr[l]<harr[i]) smallest=l;
    if(r<heap_size && harr[r]<harr[smallest]) smallest=r;
    if(smallest!=i)
    {
        swap(&harr[i], &harr[smallest]);
        Heapify(smallest);
    }
}

class Station
{
public:
    int station_id;
    Heap* electric;
    Heap* lady;
    Heap* road;
};
// Station end

// User behavior
class User
{
public:
    int src_station_id;
    int dest_station_id;

    string biketype;
    int bike_id;

    string user_id;

    int timerent;
    int timereturn;

    int shortest_time;

    string request;

    int discount_flag=0;

    int settlement(int begin, int end, string biketype,int shortest);
};

int User::settlement(int begin, int end, string biketype,int shortest)
{
    int revenue=0;
    int fee;
    if(biketype=="electric")
    {
        fee = (end-begin == shortest)? electric_fee[0]:electric_fee[1];

    }
    else if(biketype=="lady")
    {
        fee = (end-begin == shortest)? lady_fee[0]:lady_fee[1];

    }
    else if(biketype=="road")
    {
        fee = (end-begin == shortest)? road_fee[0]:road_fee[1];

    }
    //cout<<"fee:"<<fee<<endl;
    revenue=fee*(end-begin);
    //cout<<"revenue:"<<revenue<<endl;
    return revenue;
}
// User end

//Userful Function Begin
int* selection_sort(int* array, int n)
{
    for (int i=1; i<n; i++)
    {
        // 從尚未排序的數字當中，找到第i小的數字。
        int min_index = i;
        for (int j=i+1; j<n; j++)
            if (array[j] < array[min_index])
                min_index = j;

        // 把第i小的數字，放在第i個位置。
        swap(&array[i], &array[min_index]);
    }
    return array;
}


//Userful Function END


int main()
{
    /*part1_answer_file*/
    ofstream fout("part1_response.txt");
    ofstream fout1("part1_status.txt");

    /*part2_answer_file*/
    ofstream fout2("part2_response.txt");
    ofstream fout3("part2_status.txt");

    /*Build undirected Graph*/
    int V=102;
    Graph g(V);
    int src;
    int dest;
    int weight;

    string filename("./test_case/map.txt");
    int number;
    int count_graph=0; //graph counter

    ifstream input_file(filename);
    /*
    if(!input_file.is_open())
    {
        cerr << "Could not opent the file - '" << filename <<"'"<<endl;
        return EXIT_FAILURE;
    }
    */

    while(input_file >> number)
    {
        if(count_graph==0)
        {
            src=number;
            count_graph++;
        }
        else if(count_graph==1)
        {
            dest=number;
            count_graph++;
        }
        else
        {
            weight=number;
            g.addEdge(src, dest, weight);
            //cout<<src<<" "<<dest<<" "<<weight<<endl;
            count_graph=0;
        }
    }
    //cout<<endl;
    input_file.close();

    /* Shortest path testing
    int* arr=new int[V];
    arr=g.dijkstra(1);
    g.printSolution(arr);
    g.printSolution(); */

    /* Initialization of Station info. */
    string filename2("./test_case/station.txt");
    int id_count=0;
    int count_station=0; //station txt position counter
    int station_number=0; //the number of stations

    Station* station_arr = new Station[V]; //Stored each station info by array for the part1_answer.
    Station* station_arr1 = new Station[V]; //Stored each station info by array for the part2_answer.

    ifstream input_file2(filename2);

    while(input_file2 >> number)
    {
        if(count_station==0)
        {
            station_number++;
            id_count=number;
            station_arr[id_count].station_id=number;
            station_arr1[id_count].station_id=number;
            //cout<<"ID: "<<station_arr[id_count].station_id<<endl;
            count_station++;
        }
        else if(count_station==1)
        {
            station_arr[id_count].electric=new Heap(10000); //initial set number->overflow, so we reset 300.
            station_arr1[id_count].electric=new Heap(10000); //initial set number->overflow, so we reset 300.

            for(int i=0; i<number; i++)
            {
                int index;
                index=100*id_count+i;
                station_arr[id_count].electric->insertKey(index);
                station_arr1[id_count].electric->insertKey(index);
            }
            /*print electric
            cout<<"electric:";
            for(int i=0; i<number; i++) cout<<station_arr[id_count].electric->extractMin()<<" ";
            cout<<endl;
            */

            count_station++;
        }
        else if(count_station==2)
        {
            station_arr[id_count].lady=new Heap(10000); //initial set number->overflow, so we reset 300.
            station_arr1[id_count].lady=new Heap(10000); //initial set number->overflow, so we reset 300.

            for(int i=0; i<number; i++)
            {
                int index;
                index=100*id_count+i;
                station_arr[id_count].lady->insertKey(index);
                station_arr1[id_count].lady->insertKey(index);
            }
            /*
            cout<<"lady:";
            for(int i=0; i<number; i++) cout<<station_arr[id_count].lady->extractMin()<<" ";
            cout<<endl;
            */

            count_station++;
        }
        else
        {
            station_arr[id_count].road=new Heap(10000); //initial set number->overflow, so we reset 300.
            station_arr1[id_count].road=new Heap(10000); //initial set number->overflow, so we reset 300.
            // print road
            for(int i=0; i<number; i++)
            {
                int index;
                index=100*id_count+i;
                station_arr[id_count].road->insertKey(index);
                station_arr1[id_count].road->insertKey(index);
            }
            /*
            cout<<"road:";
            for(int i=0; i<number; i++) cout<<station_arr[id_count].road->extractMin()<<" ";
            cout<<endl;
            */
            // insert lady_heap

            count_station=0;
        }
    }
    //cout<<endl;
    input_file2.close();

    /*Define fee policy*/
    string filename3("./test_case/fee.txt");
    string str;
    int fee_count=0;

    ifstream input_file3(filename3);
    while(input_file3 >> str)
    {
        if(fee_count==1) electric_fee[0]=stoi(str);
        else if(fee_count==2) electric_fee[1]=stoi(str);
        else if(fee_count==4) lady_fee[0]=stoi(str);
        else if(fee_count==5) lady_fee[1]=stoi(str);
        else if(fee_count==7) road_fee[0]=stoi(str);
        else if(fee_count==8) road_fee[1]=stoi(str);
        else if(fee_count==9) waiting_fee=stoi(str);
        else if(fee_count==10) discount_rate=stof(str);
        else if(fee_count==11) transferring_fee=stoi(str);

        fee_count++;
        if(fee_count==12) fee_count=0;
    }
    //cout<<"electric "<<electric_fee[0]<<" "<<electric_fee[1]<<endl;
    //cout<<"lady "<<lady_fee[0]<<" "<<lady_fee[1]<<endl;
    //cout<<"road "<<road_fee[0]<<" "<<road_fee[1]<<endl;
    //cout<<waiting_fee<<endl;
    //cout<<discount_rate<<endl;
    //cout<<transferring_fee<<endl;
    //cout<<endl;
    input_file3.close();

    /*Part1_Answer1 Begin*/
    User* usr = new User[100000];
    string filename4("./test_case/user.txt");
    int usr_count;
    int usr_record=0;
    string state;
    int total_revenue=0;
    int temp_station_id;
    int position=0; //find the return user position

    ifstream input_file4(filename4);
    while(input_file4 >> str)
    {
        if(str=="rent")
        {
            fout<<str<<" ";
            //cout<<str<<" ";
            state="rent";
            usr_count=4;
        }

        if(str=="return")
        {
            fout<<str<<" ";
            //cout<<str<<" ";
            state="return";
            usr_count=3;
        }

        if(state=="rent" && usr_count>=0)
        {
            if(usr_count==3)
            {
                fout<<str<<" ";
                //cout<<str<<" ";
                usr[usr_record].src_station_id=stoi(str);
            }
            else if(usr_count==2)
            {
                fout<<str<<" ";
                //cout<<str<<" ";
                usr[usr_record].biketype=str;
                if(usr[usr_record].biketype=="electric")
                {
                    usr[usr_record].bike_id=station_arr[usr[usr_record].src_station_id].electric->extractMin();
                    if(usr[usr_record].bike_id!=INT_MAX)
                    {
                        //cout<<"rent_bikeid:"<<usr[usr_record].bike_id<<endl;
                        usr[usr_record].request="accept";
                    }
                    else
                    {
                        usr[usr_record].request="reject";
                    }
                }
                else if(usr[usr_record].biketype=="lady")
                {
                    usr[usr_record].bike_id=station_arr[usr[usr_record].src_station_id].lady->extractMin();
                    if(usr[usr_record].bike_id!=INT_MAX)
                    {
                        //cout<<"rent_bikeid:"<<usr[usr_record].bike_id<<endl;
                        usr[usr_record].request="accept";
                    }
                    else
                    {
                        usr[usr_record].request="reject";
                    }
                }
                else if(usr[usr_record].biketype=="road")
                {
                    usr[usr_record].bike_id=station_arr[usr[usr_record].src_station_id].road->extractMin();
                    if(usr[usr_record].bike_id!=INT_MAX)
                    {
                        //cout<<"rent_bikeid:"<<usr[usr_record].bike_id<<endl;
                        usr[usr_record].request="accept";
                    }
                    else
                    {
                        usr[usr_record].request="reject";
                    }
                }
            }
            else if(usr_count==1)
            {
                fout<<str<<" ";
                //cout<<str<<" ";
                usr[usr_record].user_id=str;
            }
            else if(usr_count==0)
            {
                fout<<str<<endl;
                //cout<<str<<endl;

                fout<<usr[usr_record].request<<endl;
                //cout<<usr[usr_record].request<<endl;

                usr[usr_record].timerent=stoi(str);
                usr_record++;
            }
        }

        if(state=="return" && usr_count>=0)
        {
            if(usr_count==2)
            {
               fout<<str<<" ";
               //cout<<str<<" ";
               temp_station_id=stoi(str);
            }
            else if(usr_count==1)
            {
                fout<<str<<" ";
                //cout<<str<<" ";
                while(usr[position].user_id!=str) position++;
                usr[position].dest_station_id=temp_station_id;
            }
            else if(usr_count==0)
            {
                fout<<str<<endl;
                //cout<<str<<endl;
                //cout<<"user_id:"<<usr[position].user_id<<endl;
                usr[position].timereturn=stoi(str);

                int* arr=new int[V];
                arr=g.dijkstra(usr[position].src_station_id);

                //cout<<"user_requeset:"<<usr[position].request<<endl;
                if(usr[position].request=="accept")
                {
                    //cout<<"begin:"<<usr[position].timerent<<endl;
                    //cout<<"end:"<<usr[position].timereturn<<endl;
                    //cout<<"biketype:"<<usr[position].biketype<<endl;
                    //cout<<"shortestpath:"<<arr[usr[position].dest_station_id]<<endl;
                    total_revenue += usr->settlement(usr[position].timerent, usr[position].timereturn, usr[position].biketype, arr[usr[position].dest_station_id]);
                    //cout<<"dest_station_id:"<<usr[position].dest_station_id<<endl;
                    //cout<<"return_bikeid:"<<usr[position].bike_id<<endl;
                    if(usr[position].biketype=="electric") station_arr[usr[position].dest_station_id].electric->insertKey(usr[position].bike_id);
                    else if(usr[position].biketype=="lady") station_arr[usr[position].dest_station_id].lady->insertKey(usr[position].bike_id);
                    else if(usr[position].biketype=="road") station_arr[usr[position].dest_station_id].road->insertKey(usr[position].bike_id);
                }

                position=0;

                //if(usr[position].timereturn>=1440) break;
            }
        }
        usr_count--;
    }
    fout.close();

    /*part1_status.txt*/
    for(int index=1; index<=station_number; index++)
    {
        fout1<<index<<":"<<endl;
        //cout<<index<<":"<<endl;

        fout1<<"electric:";
        //cout<<"electric:";

        int temp;
        for(int i=0; i<10000; i++)
        {
            temp=station_arr[index].electric->extractMin();
            if(temp!=INT_MAX)
            {
                fout1<<temp<<" ";
                //cout<<temp<<" ";
            }
            else break;
        }
        fout1<<endl;
        //cout<<endl;

        fout1<<"lady:";
        //cout<<"lady:";
        for(int i=0; i<10000; i++)
        {
            temp=station_arr[index].lady->extractMin();
            if(temp!=INT_MAX)
            {
               fout1<<temp<<" ";
               //cout<<temp<<" ";
            }
            else break;
        }
        fout1<<endl;
        //cout<<endl;

        fout1<<"road:";
        //cout<<"road:";
        for(int i=0; i<10000; i++)
        {
            temp=station_arr[index].road->extractMin();
            if(temp!=INT_MAX)
            {
               fout1<<temp<<" ";
               //cout<<temp<<" ";
            }
            else break;
        }
        fout1<<endl;
        //cout<<endl;
    }

    fout1<<total_revenue<<endl;
    //cout<<total_revenue<<endl;

    fout1.close();
    /*Part1_Answer END*/

    /*Part2_Answer Begin*/
    User* usr1 = new User[100000];
    string filename5("./test_case/user.txt");
    int usr1_count;
    int usr1_record=0;
    string state1;
    int total_revenue_1=0;
    int temp_station_id_1;
    int position_1=0; //find the return user position
    int transfer_fee=0;
    struct PendingTransfer { int station, bike_id, arrival; string type; };
    PendingTransfer* pending=new PendingTransfer[3*V];
    int pending_count=0;
    auto arrive = [&](int time) {
        for(int i=0; i<pending_count; ++i) {
            if(pending[i].arrival > time || pending[i].arrival < 0) continue;
            Station& target=station_arr1[pending[i].station];
            if(pending[i].type=="electric") target.electric->insertKey(pending[i].bike_id);
            else if(pending[i].type=="lady") target.lady->insertKey(pending[i].bike_id);
            else target.road->insertKey(pending[i].bike_id);
            pending[i].arrival=-1;
        }
    };
    // The old parser chooses a bike before reading the rent time. Read only the
    // event times in advance so transfers can arrive before that choice.
    int rent_times[100000];
    int rent_time_count=0;
    {
        ifstream schedule(filename5);
        string action, type, user_id;
        int station_id, event_time;
        while(schedule >> action >> station_id) {
            if(action=="rent") {
                schedule >> type >> user_id >> event_time;
                rent_times[rent_time_count++]=event_time;
            } else if(action=="return") {
                schedule >> user_id >> event_time;
            }
        }
    }

    //predict transfer
    for(int index=1; index<=station_number; index++)
    {
        int electric_temp=station_arr1[index].electric->extractMin();
        int lady_temp=station_arr1[index].lady->extractMin();
        int road_temp=station_arr1[index].road->extractMin();

        //si station is empty for electric
        if(electric_temp == INT_MAX)
        {
            // shortest policy transfer electric
            int* arr_electric=new int[V];
            int* sorted_electric=new int[V];
            arr_electric=g.dijkstra(index);
            for(int i=0; i<V; ++i) sorted_electric[i]=arr_electric[i];
            selection_sort(sorted_electric, V);

            for(int i=2; i<V; i++)
            {
                if(sorted_electric[i]==INT_MAX)
                {
                    break;
                }
                int sj=1; //transfer sj to si
                while(arr_electric[sj]!=sorted_electric[i]) sj++;
                //How many electric we can call
                int* callable_temp=new int[10000];
                for(int i=0; i<10000; i++) callable_temp[i]=INT_MAX;
                int callable_count=0;

                callable_temp[callable_count]=station_arr1[sj].electric->extractMin();
                while(callable_temp[callable_count]!=INT_MAX)
                {
                    callable_count++;
                    callable_temp[callable_count]=station_arr1[sj].electric->extractMin();
                }

                if(callable_count>=2)
                {
                    //int transfer_to_si = callable_count/2; // sj transfer half of bike to si
                    for(int i=0; i<1; i++) pending[pending_count++]=PendingTransfer{index, callable_temp[i], arr_electric[sj], "electric"}; //i<transfer_to_si
                    for(int j=1; j<callable_count; j++) station_arr1[sj].electric->insertKey(callable_temp[j]);
                    fout2<<"transfer "<<to_string(sj)<<" "<<to_string(index)<<" electric "<<to_string(1)<<" 0"<<endl; //to_string(transfer_to_si)
                    //cout<<"transfer"<<to_string(sj)<<" "<<to_string(index)<<" electric "<<to_string(1)<<" 0"<<endl;  //to_string(transfer_to_si)
                    transfer_fee+= (transferring_fee * arr_electric[sj]); //transfer_fee*shortest_path_time
                    break;
                }
                else
                {
                    for(int i=0; i<callable_count; i++)
                    {
                        if(callable_temp[i]!=INT_MAX)
                            station_arr1[sj].electric->insertKey(callable_temp[i]);
                    }

                }

            }
        }
        else
        {
            station_arr1[index].electric->insertKey(electric_temp);
        }

        //si station is empty for lady
        if(lady_temp == INT_MAX)
        {
            // shortest policy transfer lady
            int* arr_lady=new int[V];
            int* sorted_lady=new int[V];
            arr_lady=g.dijkstra(index);
            for(int i=0; i<V; ++i) sorted_lady[i]=arr_lady[i];
            selection_sort(sorted_lady, V);

            for(int i=2; i<V; i++)
            {
                if(sorted_lady[i]==INT_MAX)
                {
                    break;
                }
                int sj=1; //transfer sj to si
                while(arr_lady[sj]!=sorted_lady[i]) sj++;
                //How many lady we can call
                int* callable_temp=new int[10000];
                for(int i=0; i<10000; i++) callable_temp[i]=INT_MAX;
                int callable_count=0;

                callable_temp[callable_count]=station_arr1[sj].lady->extractMin();
                while(callable_temp[callable_count]!=INT_MAX)
                {
                    callable_count++;
                    callable_temp[callable_count]=station_arr1[sj].lady->extractMin();
                }

                if(callable_count>=2)
                {
                    //int transfer_to_si = callable_count/2; // sj transfer half of bike to si
                    for(int i=0; i<1; i++) pending[pending_count++]=PendingTransfer{index, callable_temp[i], arr_lady[sj], "lady"};
                    for(int j=1; j<callable_count; j++) station_arr1[sj].lady->insertKey(callable_temp[j]);
                    fout2<<"transfer "<<to_string(sj)<<" "<<to_string(index)<<" lady "<<to_string(1)<<" 0"<<endl;
                    //cout<<"transfer"<<to_string(sj)<<" "<<to_string(index)<<" lady "<<to_string(1)<<" 0"<<endl;
                    transfer_fee+= (transferring_fee * arr_lady[sj]); //transfer_fee*shortest_path_time
                    break;
                }
                else
                {
                    for(int i=0; i<callable_count; i++)
                    {
                        if(callable_temp[i]!=INT_MAX)
                            station_arr1[sj].lady->insertKey(callable_temp[i]);
                    }

                }

            }
        }
        else
        {
            station_arr1[index].lady->insertKey(lady_temp);
        }

        //si station is empty for road
        if(road_temp == INT_MAX)
        {
            // shortest policy transfer road
            int* arr_road=new int[V];
            int* sorted_road=new int[V];
            arr_road=g.dijkstra(index);
            for(int i=0; i<V; ++i) sorted_road[i]=arr_road[i];
            selection_sort(sorted_road, V);

            for(int i=2; i<V; i++)
            {
                if(sorted_road[i]==INT_MAX)
                {
                    break;
                }
                int sj=1; //transfer sj to si
                while(arr_road[sj]!=sorted_road[i]) sj++;
                //How many road we can call
                int* callable_temp=new int[10000];
                for(int i=0; i<10000; i++) callable_temp[i]=INT_MAX;
                int callable_count=0;

                callable_temp[callable_count]=station_arr1[sj].road->extractMin();
                while(callable_temp[callable_count]!=INT_MAX)
                {
                    callable_count++;
                    callable_temp[callable_count]=station_arr1[sj].road->extractMin();
                }

                if(callable_count>=2)
                {
                    //int transfer_to_si = callable_count/2; // sj transfer half of bike to si
                    for(int i=0; i<1; i++) pending[pending_count++]=PendingTransfer{index, callable_temp[i], arr_road[sj], "road"};
                    for(int j=1; j<callable_count; j++) station_arr1[sj].road->insertKey(callable_temp[j]);
                    fout2<<"transfer "<<to_string(sj)<<" "<<to_string(index)<<" road "<<to_string(1)<<" 0"<<endl;
                    //cout<<"transfer"<<to_string(sj)<<" "<<to_string(index)<<" road "<<to_string(1)<<" 0"<<endl;
                    transfer_fee+= (transferring_fee * arr_road[sj]); //transfer_fee*shortest_path_time
                    break;
                }
                else
                {
                    for(int i=0; i<callable_count; i++)
                    {
                        if(callable_temp[i]!=INT_MAX)
                            station_arr1[sj].road->insertKey(callable_temp[i]);
                    }

                }

            }
        }
        else
        {
            station_arr1[index].road->insertKey(road_temp);
        }

    }

    //int discount_flag=0;
    ifstream input_file5(filename4);
    while(input_file5 >> str)
    {
        if(str=="rent")
        {
            fout2<<str<<" ";
            //cout<<str<<" ";
            state1="rent";
            usr1_count=4;
        }

        if(str=="return")
        {
            fout2<<str<<" ";
            //cout<<str<<" ";
            state1="return";
            usr1_count=3;
        }

        if(state1=="rent" && usr1_count>=0)
        {
            if(usr1_count==3)
            {
                fout2<<str<<" ";
                //cout<<str<<" ";
                usr1[usr1_record].src_station_id=stoi(str);
            }
            else if(usr1_count==2)
            {
                fout2<<str<<" ";
                //cout<<str<<" ";
                usr1[usr1_record].biketype=str;
                arrive(rent_times[usr1_record]);
                if(usr1[usr1_record].biketype=="electric")
                {
                    usr1[usr1_record].bike_id=station_arr1[usr1[usr1_record].src_station_id].electric->extractMin(); //enough or not
                    if(usr1[usr1_record].bike_id!=INT_MAX)
                    {
                        //cout<<"rent_bikeid:"<<usr[usr_record].bike_id<<endl;
                        usr1[usr1_record].request="accept";
                    }
                    else
                    {
                       //electric discount policy
                       usr1[usr1_record].bike_id=station_arr1[usr1[usr1_record].src_station_id].lady->extractMin();
                       if(usr1[usr1_record].bike_id!=INT_MAX)
                       {
                           usr1[usr1_record].discount_flag=1;
                           usr1[usr1_record].biketype="lady";
                           usr1[usr1_record].request="accept";
                       }
                       else
                       {
                           usr1[usr1_record].bike_id=station_arr1[usr1[usr1_record].src_station_id].road->extractMin();
                           if(usr1[usr1_record].bike_id!=INT_MAX)
                           {
                               usr1[usr1_record].discount_flag=1;
                               usr1[usr1_record].biketype="road";
                               usr1[usr1_record].request="accept";
                           }
                           else
                           {
                               usr1[usr1_record].request="reject";
                           }
                       }

                    }
                }
                else if(usr1[usr1_record].biketype=="lady")
                {
                    usr1[usr1_record].bike_id=station_arr1[usr1[usr1_record].src_station_id].lady->extractMin();
                    if(usr1[usr1_record].bike_id!=INT_MAX)
                    {
                        //cout<<"rent_bikeid:"<<usr[usr_record].bike_id<<endl;
                        usr1[usr1_record].request="accept";
                    }
                    else
                    {
                       //lady discount policy
                       usr1[usr1_record].bike_id=station_arr1[usr1[usr1_record].src_station_id].electric->extractMin();
                       if(usr1[usr1_record].bike_id!=INT_MAX)
                       {
                           usr1[usr1_record].discount_flag=1;
                           usr1[usr1_record].biketype="electric";
                           usr1[usr1_record].request="accept";
                       }
                       else
                       {
                           usr1[usr1_record].bike_id=station_arr1[usr1[usr1_record].src_station_id].road->extractMin();
                           if(usr1[usr1_record].bike_id!=INT_MAX)
                           {
                               usr1[usr1_record].discount_flag=1;
                               usr1[usr1_record].biketype="road";
                               usr1[usr1_record].request="accept";
                           }
                           else
                           {
                               usr1[usr1_record].request="reject";
                           }
                       }

                    }
                }
                else if(usr1[usr1_record].biketype=="road")
                {
                    usr1[usr1_record].bike_id=station_arr1[usr1[usr1_record].src_station_id].road->extractMin();
                    if(usr1[usr1_record].bike_id!=INT_MAX)
                    {
                        //cout<<"rent_bikeid:"<<usr[usr_record].bike_id<<endl;
                        usr1[usr1_record].request="accept";
                    }
                    else
                    {
                       //road discount policy
                       usr1[usr1_record].bike_id=station_arr1[usr1[usr1_record].src_station_id].electric->extractMin();
                       if(usr1[usr1_record].bike_id!=INT_MAX)
                       {
                           usr1[usr1_record].discount_flag=1;
                           usr1[usr1_record].biketype="electric";
                           usr1[usr1_record].request="accept";
                       }
                       else
                       {
                           usr1[usr1_record].bike_id=station_arr1[usr1[usr1_record].src_station_id].lady->extractMin();
                           if(usr1[usr1_record].bike_id!=INT_MAX)
                           {
                               usr1[usr1_record].discount_flag=1;
                               usr1[usr1_record].biketype="lady";
                               usr1[usr1_record].request="accept";
                           }
                           else
                           {
                               usr1[usr1_record].request="reject";
                           }
                       };
                    }
                }
            }
            else if(usr1_count==1)
            {
                fout2<<str<<" ";
                //cout<<str<<" ";
                usr1[usr1_record].user_id=str;
            }
            else if(usr1_count==0)
            {
                fout2<<str<<endl;
                //cout<<str<<endl;

                if(usr1[usr1_record].discount_flag==1) fout2<<"discount "<<usr1[usr1_record].biketype<<endl;

                if(usr1[usr1_record].discount_flag==0)
                    fout2<<usr1[usr1_record].request<<endl;
                //cout<<usr1[usr1_record].request<<endl;

                usr1[usr1_record].timerent=stoi(str);
                usr1_record++;
            }
        }

        if(state1=="return" && usr1_count>=0)
        {
            if(usr1_count==2)
            {
               fout2<<str<<" ";
               //cout<<str<<" ";
               temp_station_id_1=stoi(str);
            }
            else if(usr1_count==1)
            {
                fout2<<str<<" ";
                //cout<<str<<" ";
                while(usr1[position_1].user_id!=str) position_1++;
                usr1[position_1].dest_station_id=temp_station_id_1;
            }
            else if(usr1_count==0)
            {
                fout2<<str<<endl;
                //cout<<str<<endl;
                //cout<<"user_id:"<<usr1[position].user_id<<endl;
                usr1[position_1].timereturn=stoi(str);
                arrive(usr1[position_1].timereturn);

                int* arr1=new int[V];
                arr1=g.dijkstra(usr1[position_1].src_station_id);
                //cout<<"user_requeset:"<<usr1[position].request<<endl;
                if(usr1[position_1].request=="accept")
                {
                    //cout<<"begin:"<<usr1[position].timerent<<endl;
                    //cout<<"end:"<<usr1[position].timereturn<<endl;
                    //cout<<"biketype:"<<usr1[position].biketype<<endl;
                    //cout<<"shortestpath:"<<arr[usr1[position].dest_station_id]<<endl;
                    if(usr1[position_1].discount_flag==0)
                    {
                        total_revenue_1 += usr1->settlement(usr1[position_1].timerent, usr1[position_1].timereturn, usr1[position_1].biketype, arr1[usr1[position_1].dest_station_id]);
                    }
                    else
                    {
                        int duration=usr1[position_1].timereturn-usr1[position_1].timerent;
                        int shortest=arr1[usr1[position_1].dest_station_id];
                        int* rates=usr1[position_1].biketype=="electric" ? electric_fee :
                                   usr1[position_1].biketype=="lady" ? lady_fee : road_fee;
                        int rate=(duration==shortest) ? rates[0] : rates[1];
                        total_revenue_1 += static_cast<int>(round(rate*discount_rate))*duration;
                    }

                    //cout<<"dest_station_id:"<<usr[position].dest_station_id<<endl;
                    //cout<<"return_bikeid:"<<usr[position].bike_id<<endl;
                    if(usr1[position_1].biketype=="electric") station_arr1[usr1[position_1].dest_station_id].electric->insertKey(usr1[position_1].bike_id);
                    else if(usr1[position_1].biketype=="lady") station_arr1[usr1[position_1].dest_station_id].lady->insertKey(usr1[position_1].bike_id);
                    else if(usr1[position_1].biketype=="road") station_arr1[usr1[position_1].dest_station_id].road->insertKey(usr1[position_1].bike_id);
                }
                //discount_flag=0;
                position_1=0;

                //if(usr1[position].timereturn>=1440) break;
            }
        }
        usr1_count--;
    }
    fout.close();

    /*part2_status.txt*/
    for(int index=1; index<=station_number; index++)
    {
        fout3<<index<<":"<<endl;
        //cout<<index<<":"<<endl;

        fout3<<"electric:";
        //cout<<"electric:";

        int temp;
        for(int i=0; i<10000; i++)
        {
            temp=station_arr1[index].electric->extractMin();
            if(temp!=INT_MAX)
            {
                fout3<<temp<<" ";
                //cout<<temp<<" ";
            }
            else break;
        }
        fout3<<endl;
        //cout<<endl;

        fout3<<"lady:";
        //cout<<"lady:";
        for(int i=0; i<10000; i++)
        {
            temp=station_arr1[index].lady->extractMin();
            if(temp!=INT_MAX)
            {
               fout3<<temp<<" ";
               //cout<<temp<<" ";
            }
            else break;
        }
        fout3<<endl;
        //cout<<endl;

        fout3<<"road:";
        //cout<<"road:";
        for(int i=0; i<10000; i++)
        {
            temp=station_arr1[index].road->extractMin();
            if(temp!=INT_MAX)
            {
               fout3<<temp<<" ";
               //cout<<temp<<" ";
            }
            else break;
        }
        fout3<<endl;
        //cout<<endl;
    }
    //cout<<"transfer_fee:"<<transfer_fee<<endl;
    total_revenue_1-=transfer_fee;
    if(total_revenue_1>=total_revenue) fout3<<total_revenue_1<<endl;
    else
    {
        ifstream source("part1_response.txt", ios::binary);
        ofstream dest("part2_response.txt", ios::binary);


        istreambuf_iterator<char> begin_source(source);
        istreambuf_iterator<char> end_source;
        ostreambuf_iterator<char> begin_dest(dest);
        copy(begin_source, end_source, begin_dest);

        source.close();
        dest.close();

        ifstream source1("part1_status.txt", ios::binary);
        ofstream dest1("part2_status.txt", ios::binary);

        istreambuf_iterator<char> begin_source1(source1);
        istreambuf_iterator<char> end_source1;
        ostreambuf_iterator<char> begin_dest1(dest1);
        copy(begin_source1, end_source1, begin_dest1);

        source1.close();
        dest1.close();
        //fout3<<total_revenue<<endl;
    }

    //cout<<total_revenue_1<<endl;

    fout3.close();
    /*Part2_Answer END*/



    return EXIT_SUCCESS;
}
