#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <unordered_map>
#include <functional>
#define A 54059
#define B 76963
#define C 86969
#define FIRSTH 37
using namespace std;

unsigned hashString(const char* s){
    unsigned h = FIRSTH;
    while(*s){
        h = (h*A)^(s[0]*B);
        s++;
    }
    return h%C;
}

void collectWords(string fileName, unordered_map<int,size_t> &umap, int numWords){
    vector<string> sequence;
    ifstream file;
    file.open (fileName.c_str());
    if (!file.is_open()) return;
    int count = 0;
    int index = 0;
    string word;
    while (file >> word){
        if(count<numWords){
            sequence.push_back(word);
            count++;
        }
        else{
            string key = "";
            for(int i=0; i<sequence.size(); i++){
                key += sequence.at(i);
            }
            umap[index] = hashString(key.c_str());
            sequence.erase(sequence.begin());
            sequence.push_back(word);
            index++;
        }

    }
    file.close();
}

int getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

void checkHashTables(vector<string> files, vector<unordered_map<int,size_t>> umaps, int minCollisions){
    int count;
    for(int i=0; i<umaps.size(); i++){
        for(int j=i+1; j<umaps.size(); j++){
            count = 0;
            for(auto& k: umaps.at(i)){
                //unordered_map<int,size_t>::const_iterator itr = umaps.at(j).find(k.second);
                //if(itr != umaps.at(j).end()){
                for(auto& l: umaps.at(j)){        
                    //cout << k.second << " " << l.second << "\n";
                    if(k.second == l.second){
                        count++;
                    }
                }
            }
            if(count >= minCollisions)
                cout << count << ": " << files.at(i) << ", " << files.at(j) << "\n";
        }
    }
}

int main(int argc, char* argv[]){
    vector<string> files;
    int numWords = stoi(argv[2], NULL, 10);
    getdir(string(argv[1]), files);
    files.erase(files.begin());
    files.erase(files.begin());
    vector<unordered_map<int,size_t>> umaps(files.size());
    for(int i=0; i<files.size(); i++){
        string fullName = argv[1] + files[i];
        collectWords(fullName, umaps[i], numWords);
    }
    checkHashTables(files, umaps, stoi(argv[3], NULL, 10));
}
