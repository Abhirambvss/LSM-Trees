#include<bits/stdc++.h>

using namespace std;

int globalTime = 0;

// structure to represent our key value data.
struct keyValueData{
    string K;
    int T,V;
    keyValueData(string x, int y, int z) : K(x), T(y), V(z){
    }
    bool operator < (keyValueData const&  other) const {
        return (K < other.K);
    }
};


// The below function searches the required key in memory
keyValueData searchInMemory(map<string,map<int,int>> &memory, string key){
    if(!memory.count(key))
        return keyValueData(key, -1, -1);
    keyValueData ans = keyValueData(key, -1, -1);
    int mx = 0;
    for(auto [v,t] : memory[key]){
        if(mx < t){
            mx = t;
            ans = keyValueData(key, t, v);
        }
    }
    return ans;
}

// The below function deletes the given key from the memory
void deleteInMemory(map<string,map<int,int>> &memory, string key){
    if(!memory.count(key))
        return;
    memory.erase(key);
    cout << "The value " << key << " is deleted successfully from memory." << endl;
}

// The below function is a comparator function used in sorting the key value data.
struct comparatorFunction{
    bool operator ()(keyValueData const& firstKeyValue, keyValueData const& secondKeyValue) const{
        if(firstKeyValue.K < secondKeyValue.K) { return true; }
        else if(firstKeyValue.K > secondKeyValue.K) { return false; }
        else if(secondKeyValue.K > firstKeyValue.K) { return true; }
        else if(secondKeyValue.K < firstKeyValue.K) { return false; }
        else if(firstKeyValue.K == secondKeyValue.K){
            if(firstKeyValue.T < secondKeyValue.T) { return true; }
            else { return false; }
        }
        else { return true; }
    }
};

// The below function "merge" is used to merge all the previous values of a key and update the key with its latest value.
vector<keyValueData> merge(vector<keyValueData> data1, vector<keyValueData> data2){
    vector<keyValueData> res;
    for(int i=0;i<data2.size(); ++i)
        data1.push_back(data2[i]);

    sort(data1.begin(), data1.end(), comparatorFunction());
    for(int i=0;i<data1.size(); ++i)
    {
        string present = data1[i].K;
        int time1 = 0;
        keyValueData bundles = keyValueData("GG", 0, 0);
        while(i < data1.size() && data1[i].K == present){
            if(time1 < data1[i].T){
                bundles = keyValueData(present, data1[i].T, data1[i].V);
                time1 = data1[i].T;
            }
            ++i;
        }
        --i;
        res.push_back(bundles);
    }
    return res;
}

// The below function will compress the segments (disks) into one and frees the space in the memory.
void compress(vector<vector<keyValueData>> &segments, int sizeOfMemory, map<string,map<int,int>> &memory, int sizeTillNow){
    vector<keyValueData> now = segments[1];
    segments[1].clear();
    for(int i = 2; i <= sizeOfMemory; ++i){
        now = merge(now, segments[i]);
        segments[i].clear();
    }
    segments[1].assign(now.begin(),now.end());
    int cnt = 0;
    bool flag = false;
    string last;
    for(auto [str, mp] : memory){
        for(auto [v, t] : memory[str]){
            if(cnt < sizeTillNow / 2){
                segments[2].push_back(keyValueData(str, t, v));
                cnt++;
            }
            else{
                last = str;
                flag=true;
                break;
            }
        }
        if(flag == true)
            break;
    }
    int ind = -1;
    for(int i=0;i<segments[2].size();++i){
        if(segments[2][i].K != last)
            memory.erase(segments[2][i].K);
        else{
            ind = i;
            break;
        }
    }
    for(int i=ind;i<segments[2].size();++i){
        memory[last].erase(segments[2][i].V);
    }
    if(memory[last].size() == 0)
        memory.erase(last);

}

//This function will search the key in the disk
keyValueData searchInDisk(vector<vector<keyValueData>> &segments, string k, int ava){
    int temp = 0;
    keyValueData res = keyValueData(k, -1, -1);
    int i=ava-1;
    while (i>=1){
        int p1 = lower_bound(segments[i].begin(), segments[i].end(), keyValueData(k, -1, -1)) - segments[i].begin();
        int j=p1;
        while(j<segments[i].size()){
            if(segments[i][j].K!=k)
                break;
            if(temp < segments[i][j].T){
                temp = segments[i][j].T;
                res = segments[i][j];
            }
            j++;
        }
        i--;
    }
    return res;
}

void update(vector<vector<keyValueData>> &segments, int sizeOfMemory, map<string,map<int,int>> &h, int sizeTillNow, int ava){
    int cnt = 0;
    string last;
    bool flag = false;
    for(auto [str,mp] : h){
        for(auto [v, t] : h[str]){
            if(cnt < sizeTillNow / 2){
                segments[ava].push_back(keyValueData(str, t, v));
                cnt++;
            }
            else{
                last = str;
                flag = true;
                break;
            }
        }
        if(flag)
            break;
    }
    int ind = -1;
    for(int i=0;i<segments[ava].size();++i){
        if(segments[ava][i].K != last)
            h.erase(segments[ava][i].K);
        else{
            ind = i;
            break;
        }
    }
    for(int i=ind;i<segments[ava].size();++i){
        h[last].erase(segments[ava][i].V);
    }
    if(h[last].size() == 0)
        h.erase(last);
}


// The below function is used to delete the key and the value associated with it from the disk
void deleteInDisk(vector<vector<keyValueData>> &segments, string k, int ava){
    for(int i=ava-1;i>=1;--i){
        int p1 = lower_bound(segments[i].begin(), segments[i].end(), keyValueData(k, -1, -1)) - segments[i].begin();
        int p2 = upper_bound(segments[i].begin(), segments[i].end(), keyValueData(k, -1, -1)) - segments[i].begin();
        if(p1>=segments[i].size() || segments[i][p1].K!=k)
            continue;
        segments[i].erase(segments[i].begin() + p1, segments[i].begin() + p2);
    }
    cout<<"The value "<<k<<" is deleted successfully from the disk."<<endl;

}


// This is the main function.Program execution starts with this function
int main(){
    int numberOfOperations,value,sizeOfMemory;
    string operation;
    int ava = 1;
    int sizeTillNow = 0;
    string key;

    cout<<"Enter the size of memory"<<endl;
    cin >> sizeOfMemory ;
    cout<<"Enter the total number of operations to be performed"<<endl;

    cin>> numberOfOperations;
    map<string,map<int,int>> memory;
    vector<vector<keyValueData>> segments(2 * sizeOfMemory + 1);

    while (numberOfOperations--){

        cout<<"Enter the type of operation to perform ( PUT- Insert the value, GET- Search the value,  DEL - Delete the value )"<<endl;
        cin >> operation;
        if(operation == "PUT"||operation=="put"){

            cout<<"Enter key"<<endl;
            cin >> key;
            cout<<"Enter value"<<endl;
            cin>> value;

            if(sizeTillNow < sizeOfMemory){

                if(memory[key].count(value)) {
                    continue;
                }

                memory[key][value] = ++globalTime;
                ++sizeTillNow;

            }
            else{

                if(ava > sizeOfMemory){

                    compress(segments, sizeOfMemory, memory, sizeTillNow);
                    ava = 3;
                    sizeTillNow = sizeTillNow - sizeTillNow / 2;

                    //Now put current key,value in memory.
                    if(memory[key].count(value))
                        continue;
                    memory[key][value] = ++globalTime;
                    ++sizeTillNow;
                }
                else{

                    update(segments, sizeOfMemory, memory, sizeTillNow, ava);
                    ++ava;
                    sizeTillNow = sizeTillNow - sizeTillNow / 2;

                    if(memory[key].count(value))
                        continue;
                    memory[key][value] = ++globalTime;
                    ++sizeTillNow;
                }
            }
        }
        if(operation == "GET"||operation=="get"){
            cout<<"Enter key"<<endl;
            cin >> key;
            keyValueData kvd1 = searchInMemory(memory, key);
            keyValueData kvd2 = searchInDisk(segments, key, ava);
            if(kvd1.T == -1 && kvd2.T == -1)
                cout<<"Value not found in the memory and in disk hence returning "<<-1<<endl;
            else if(kvd1.T > kvd2.T)
                cout<<"The value found in the memory at "<<kvd1.K<<" is "<<kvd1.V<<endl;
            else
                cout<<"The value found in the disk at "<<kvd2.K<<" is "<<kvd2.V<<endl;

        }

        if(operation == "DEL"||operation=="del"){
            cout<<"Enter key"<<endl;
            cin >> key;
            deleteInMemory(memory, key);
            deleteInDisk(segments, key, ava);
        }

    }



}