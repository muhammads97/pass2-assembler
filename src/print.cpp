#include "print.h"

using namespace std;
string HexFromDecimal(int num);
string getName(vector<Instruction>ins);
int getStartLoc(vector<Instruction>ins);
void printObjectCode(vector<Instruction>ins){
    //header
    string name = getName(ins);
    int startLoc = getStartLoc(ins);
    int startSize = 0;

    //record
    vector<Record>records;
    Record r;
    bool start = true;
    for(int i = 0; i < ins.size(); i++){
        if(!ins[i].ishasOpCode())
            continue;
        if(start){
            r.startAdd = ins[i].getAddress();
            start = false;
        }
        string objectCode = HexFromDecimal(ins[i].getOpCode());
        if(r.objRec.size() + objectCode.size() <= 62){
            r.objRec += objectCode;
        }else{
            r.obSize = HexFromDecimal(r.objRec.size()/2);
            records.push_back(r);
            startSize += r.objRec.size()/2;
            r.objRec = "";
            r.obSize = "";
            r.startAdd = ins[i].getAddress();
            i--;
        }
        if(i + 1 == ins.size()){
            r.obSize = HexFromDecimal(r.objRec.size()/2);
            records.push_back(r);
            startSize += r.objRec.size()/2;
            r.objRec = "";
            r.obSize = "";
            r.startAdd = ins[i].getAddress();
        }
    }
    // print
    string file_name = "objectcode.txt";
    char const * fname = file_name.c_str ();
    freopen(fname,"w",stdout);
    //header
    cout<<"H"<<name<<"^";
    printf("%.6x  ",startLoc);
    cout<<"^";
    printf("%.6x  ",startSize);
    cout<<"\n";
     //record
    for(Record i : records){
        cout<<"T";
        int x = stoi(i.startAdd, 0, 16);
        printf("%.6x  ",x);
        cout<<"^"<<i.obSize<<"^"<<i.objRec<<"\n";
    }

    //end
    cout<<"E";
    printf("%.6x  ",startLoc);


}
string getName(vector<Instruction>ins){
    for(Instruction i : ins){
        string s = i.getOperation();
        transform(s.begin() , s.end() ,s.begin() , ::toupper);
        if(s == "START"){
            if(i.getLabel().size())
                return i.getLabel();
        }
    }

    return "      ";
}
int getStartLoc(vector<Instruction>ins){
    for(Instruction i : ins){
        string s = i.getOperation();
        transform(s.begin() , s.end() ,s.begin() , ::toupper);
        if(s == "START"){
            if(i.getOperand().size())
                return  stoi (i.getOperand());
        }
    }
    return 0;
}
/*string HexFromDecimal(int num){
    int temp = num;
    int  r ;
    std::string hex = "";
    while (temp != 0){
        r = temp % 16;
        if (r < 10){
            hex += r + 48;
        } else {
            hex += r + 55;
        }
        temp = temp / 16 ;
    }
    reverse(hex.begin(), hex.end());
    return hex;
}*/
