#include <iostream>
#include <pthread.h>
#include <sstream>
#include <algorithm>
#include <utility>
#include <vector>
#include <string>

using namespace std;

//will receive input from user and return it as a vector in order to access specific indexes for different purposes
vector<string> get_Inputs(){
    vector<string> inputs;
    string input;
    while(getline(cin, input)){
        if(!input.empty()){
          inputs.push_back(input);
        }
        else{
          return inputs;
        }
    }
    return inputs;
};

/*sparse the second input line containing the char and its ranges
is held within a vector that holds pairings of char and vector with vector containing
pairs of int and int where each pair of integers holds the start and end of each range*/
vector<pair<char, vector<pair<int, int>>>> create_Pairing(string sec_Input){
    char com_delimeter = ','; //used to sparse the string into each char and its ranges
    size_t pos_Comma = 0;
    int start, end;     //start and end of each range
    
    vector<pair<char, vector<pair<int, int>>>> pairings;
    char im_filler;             //to hold the char
    vector<pair<int, int>> ranges;
    string temp;            //to hold the sparsed string containing the char and ranges
    
    while((pos_Comma = sec_Input.find(com_delimeter)) != string::npos){
        temp = sec_Input.substr(0, pos_Comma);
        
        stringstream ss(temp);
        ss >> im_filler;
        
        while(ss >> start >> end){
            ranges.emplace_back(start, end);
        }
        
        pairings.emplace_back(im_filler, ranges);
        
        ranges.clear();     //clear vector to not waste memory space
    
        sec_Input.erase(0, pos_Comma + 1); //remove the section that was sparse in order for the next comma to be found
    }
    
    //for last char and range(s) since there are no more commas left
    if(!sec_Input.empty()){
        stringstream cc(sec_Input);
        cc >> im_filler;
        
        while(cc >> start >> end){
            ranges.emplace_back(start, end);
        }
        pairings.emplace_back(im_filler, ranges);
        ranges.clear();
    }
    return pairings; //returns all the found char and range pairings
};

//the Thread struct that holds the required variables for each thread to access
struct Thread{
    
    //all the variables that are pointers (except output) are done so that each struct can access the same info from the same address
    
    int rowIndex;   //Receives the line number to decode
    pair<int, int> *dimensions; //the image size
    vector<int> *headPos; //the headPos and dataPos arrays
    vector<int> *dataPos;
    
    vector<pair<char, vector<pair<int, int>>>> *pairings; //the information about the symbols to print from the main thread
    vector<char>* output; //the memory location to store the decoding process results
    
    Thread(int row, pair<int, int>* dim, vector<int>* head, vector<int>* data, vector<pair<char, vector<pair<int, int>>>>* p, vector<char>* out)
        : rowIndex(row), dimensions(dim), headPos(head), dataPos(data), pairings(p), output(out) {}
    
};

//uses x to go through the pairings and see in what range it falls in and then outputs the correct character accordingly
char what_char(int x, vector<pair<char, vector<pair<int, int>>>> pairings){
    for(const auto& p: pairings){       //vector<pair<char, vector<pair
        for(const auto& d: p.second){                       //vector<pair<int, int>>>
            if(d.first <= x && x <= d.second){
                return p.first;         //returns the char
            }
        }
    }
    return '4'; 
}

vector<char> this_row(int num_De, pair<int, int> size, vector<int> headPos, vector<int> dataPos, vector<pair<char, vector<pair<int, int>>>> pairings){
    /*num_De serves as y in the original pseudocode and since this is a thread per row, 
    the for loop for y is removed since the thread creation is already happening in a for loop
    but in main and the row it is at is being sent to the function*/
    bool defElement = false;
    int low, mid, high;
    vector<char> current_Row(size.second, ' ');
    
    for(int x = 0; x < size.second; x++){
        
        /*Specifically,
        for these positions where y == height, the binary search must 
        be performed from dataPos[height] to the end of the dataPos 
        (or when it reaches the last row so when y == width-1)*/
        
        if(num_De == size.first - 1){
            low = headPos[num_De];
            high = dataPos.size();
        }
        else{
            low = headPos[num_De];
            high = headPos[num_De + 1];
        }
        
        /*For an arbitrary query point  p(x,y), we first 
        calculate the row position using headPos[y]. Then we perform 
        a binary search within the corresponding scope from 
        dataPos[headPos[y]] to dataPos[head[y+1]]. if no equivalent 
        value of x is found, the position p(x,y) represents 
        an empty element. if an equivalent value of is
        found, the position p(x,y) is a defined element. */
        
        while(low <= high){
            mid = (low + high)/2;
            if(x == dataPos[mid]){
                defElement = true;
                current_Row[x] = what_char(x, pairings);    //places the correct char
                break;
            }
            else if(x < dataPos[mid]){
                high = mid -1;
            }
            else{
                low = mid+1;
            }
        }
        if(low > high){
            defElement = false;
        }
    }
    return current_Row;
}

//function will pass in pthread_join, returns a void pointer into a Thread pointer 
void* threadInstructions(void* ptr){
    //Thread* thread = (Thread*) ptr; is C-style cast with no type safety
    Thread* thread = static_cast<Thread*> (ptr);
    vector<char> *output = thread->output;
    
    //creates what needs to be outputted, modifying the contents inside the memory by dereferencing output
    *output = this_row(thread->rowIndex, *(thread->dimensions), *(thread->headPos), *(thread->dataPos), *(thread->pairings));
    return nullptr;
}
 
int main(){
    
    vector<string> inputs = get_Inputs(); //holds all the inputs

    //parsing string to find length and width
    int length = stoi(inputs[0].substr(0, inputs[0].find(' '))); 
    int width = stoi(inputs[0].substr(inputs[0].find(' ') + 1, inputs[0].length()));
    pair<int, int> size = make_pair(width, length);
    
    /*was able to extract the width and length of the image*/
    //cout << length << " " << width << endl;
    
    vector<pair<char, vector<pair<int, int>>>> pairings = create_Pairing(inputs[1]);
    
    /*was able to sparse the second input line containing the char and its ranges
    is held within a vector that holds pairings of char and vector with vector containing
    pairs of int and int where each pair of integers holds the start and end of each range the character has
    to avoid repetition, the character and vector pairing was done and placed in a vector*/
    /*for(const auto& p : pairings){
        for(const auto& d: p.second){
            cout << p.first << " " << d.first << " " << d.second << endl;
        }
    }*/
    
    string temp = inputs[2];
    int thee_pos;
    stringstream bb(temp);
    vector<int> headPos;
    
    while(bb >> thee_pos){
        headPos.emplace_back(thee_pos);
    }
    
    /*was able to sparse the values for the headPos array^^^^^^^*/
    /*for(int i = 0; i < headPos.size(); i++){
        cout << headPos[i] << endl;
    }*/
    
    temp = inputs[3];
    stringstream pp(temp);
    vector<int> dataPos;
    
    while(pp >> thee_pos){
        dataPos.emplace_back(thee_pos);
    }
    
    /*was able to successufully sparse the values for the dataPos array^^^^^^*/
    /*for(int i = 0; i < dataPos.size(); i++){
        cout << dataPos[i] << endl;
    }*/
    
    vector<vector<char>> image(width, vector<char>(length, ' ')); //the 2d image
    vector<pthread_t> tid; //holds n number of threads
    //vector<Thread*> pointerVector; 
    
    //create the n threads needed to create the image
    //passing row/line to decode, size, the arrays, and output of the thread by pass by reference on that particular row
    for(int y = 0; y < width; y++){
        Thread* newThread = new Thread(y, &size, &headPos, &dataPos, &pairings, &image[y]);
        pthread_t myThread;
        if(pthread_create(&myThread, NULL, threadInstructions, static_cast<void*> (newThread))){
            
            //static_cast<void*> (newThread) ensures that Thread* pointer is converted to a void* before passing to pthread_create
            cout << "Error creating thread." << endl;
            return -1;
        }
        tid.push_back(myThread);
        //pointerVector.push_back(newThread);
    }
    
    //call on pthread join
    for(int y = 0; y < width; y++){
        pthread_join(tid[y], NULL);
    }
    
    //outputting what the threads did
    for(int i = 0; i < width; i++){
        for(int j = 0; j < length; j++){
            cout << image[i][j];
        }
        cout << endl;
    }
    
    
   return 0; 
}