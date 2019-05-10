#include "writepass.h"
#include <unordered_map>
#include <iomanip>
#include <stdint.h>
using namespace std;
string HexFromDecimal(int num);
bool flag = false;
writepass::writepass(std::vector<Instruction> ins, std::string file_name)
{
 file_name =file_name+" outputPhase2.txt";
    char const * fname = file_name.c_str ();
    freopen(fname,"w",stdout);
    string objectCode = "";
    cout << ">>   S t a r t   o f    P a s s   I I \n\n";
    cout << ">>   A s s e m b l e d    p r o g r a m     l i s t i n g \n\n";
    cout << "LC    Code         Source Statement \n\n";

      for(int i = 0; i < ins.size(); i++){

        printf("%.6x  ",ins[i].getAddress());
        cout<<"   ";
        if (!ins[i].ishasOpCode()){
            cout << "          ";
        }else {
            objectCode = HexFromDecimal(ins[i].getOpCode());
            if(ins[i].getFormat() == 3)
            {
                while(objectCode.size() < 6)
                {
                    objectCode = '0' + objectCode;
                }
            }
            if(ins[i].getFormat() == 4)
            {
                while(objectCode.size() < 8)
                {
                    objectCode = '0' + objectCode;
                }
            }
            cout << objectCode;
            int s = objectCode.length();
            for(int i = s; i < 10; i++){
                    cout<<" ";
            }
        }
        cout << ins[i].getLabel();
        int s = ins[i].getLabel().size();
            for(int i = s; i < 13; i++){
                    cout<<" ";
        }
        string ss = ins[i].getOperation();
        transform(ss.begin(), ss.end(), ss.begin(), ::tolower);
        if (ss.size() > 0){
            cout<<".";
        }
         cout << ss;
         int z= ss.size();
         for(int i = z; i < 13; i++){
                cout<<" ";
         }
        cout<<ins[i].getOperand();
        s = ins[i].getOperand().size();
        for(int i = s; i < 13; i++)
        {
            cout<<" ";
        }
        cout<<ins[i].getComment()<<"\n";

        if(ins[i].isPass2Err())
        {
            flag = true;
            cout<<ins[i].getPass2ErrMsg()<<"\n";
        }

      }

      if (flag){
        cout << "\n>>    u n s u c c e s s f u l    a s s e m b l y\n";
      } else {
          cout << "\n>>    s u c c e s s f u l    a s s e m b l y\n";
      }
}

writepass::~writepass()
{
    //dtor
}

