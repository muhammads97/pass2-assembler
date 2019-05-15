#include "ReadFile.h"
#include<bits/stdc++.h>
#include<iostream>
ReadFile::ReadFile()
{
}
std::vector<std::string> ReadFile::read(std::string path){
    std::ifstream file(path);
    std::string line;
    std::vector<std:: string>vec;
    while(std::getline(file, line)){
        bool can = false;
        for(int i = 0; i < line.size(); i++){
            if(line[i] != ' '){
                can = true;
                break;
            }
        }
        if(can)
            vec.push_back(line);
    }
    return vec;
}

