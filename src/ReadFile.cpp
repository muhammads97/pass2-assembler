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
        vec.push_back(line);
    }
    return vec;
}

