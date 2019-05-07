#include "WriteFile.h"
#include<unordered_map>
#include <iomanip>
#include <stdint.h>
using namespace std;
WriteFile::WriteFile(std::vector<Instruction> ins,std::vector < std::pair <int ,std::string> > literals_data,
                     int pc,std::unordered_map<std::string, Symbol> symbols,std::string file_name)
{

    file_name =file_name+" output.txt";
    char const * fname = file_name.c_str ();
    freopen(fname,"w",stdout);
     std::cout<<" Source Program Statements With Value of LC indicated \n\n";
    for(int i = 0; i < ins.size(); i++){
        //if(ins[i].getOperand() != "")

            printf("%.6x  ",ins[i].getAddress());
            std::cout<<"     "<<ins[i].getLabel();
            int s = ins[i].getLabel().size();
            for(int i = s; i < 18; i++){
                    cout<<" ";
            }
            cout<<ins[i].getOperation();
            s = ins[i].getOperation().size();
            for(int i = s; i < 18; i++){
                cout<<" ";
            }
            cout<<ins[i].getOperand();
            s = ins[i].getOperand().size();
            for(int i = s; i < 18; i++){
                cout<<" ";
            }
            cout<<ins[i].getComment()<<"\n";
        if(ins[i].isWrong()){
            std::cout<<ins[i].getErrorMsg()<<"\n";
        }
    }
    cout << "\n";
    for(int i = 0; i < literals_data.size() ; i++){
            std::pair<int , std::string> temp = literals_data[i];
            printf("%.6x",pc);
            std::cout<< "       " << temp.second ;
            int s = temp.second.size();
            for(int i = s; i < 18; i++){
                cout<<" ";
            }
            cout<< "Literal" << "\n";
            pc += temp.first;
            literals_data[i].first = pc;
    }
    std::cout<<"\n>>    END    Of    Pass    One \n";
    std::cout<<"     ******************************************************************** \n";
    std::cout<<">>    s y m b o l     t a b l e   (values in decimal) \n\n";
    std::cout<< "         NAME              VALUE               ABSOL/RELOC \n";
    std::cout<< "         ---------------------------------------------------------\n";
    std::unordered_map<std::string,Symbol>::iterator it = symbols.begin();
    while(it != symbols.end()){
        if(symbols[it->first].relocatable){
            std::cout<<"         " << it->first;
            int s = it->first.size();
            for(int i = s; i < 18; i++){
                cout<<" ";
            }
            cout<<symbols[it->first].address;
            s = sizeof(symbols[it->first].address)/24;
            for(int i = s; i < 18; i++){
                cout<<" ";
            }
            std::cout<<"relocatable\n";
        }

        else{

            std::cout<<"         " << it->first;
            int s = it->first.size();
            for(int i = s; i < 18; i++){
                cout<<" ";
            }
            cout<<symbols[it->first].address;
            s = sizeof(symbols[it->first].address)/24;
            for(int i = s; i < 18; i++){
                cout<<" ";
            }
            std::cout<<"absolute\n";
        }
        it++;

    }

    std::cout<<"     ******************************************************************** \n";

}

WriteFile::~WriteFile()
{
    //dtor
}
