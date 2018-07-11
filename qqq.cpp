#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <queue>
#include <cmath>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
using namespace std;

#define N 1000010

typedef unsigned int UI;
typedef unsigned long long UL;

struct Flow_Data{
    int Op_Val;
    bool Is_Num;
    int Rec, Rec2, a1, a2, a3;
    long long Result, Result2;
}Line[10000], Now;

struct Flow{
    int Line_num;
    int Process;
    Flow_Data x;
};

int Last_Lableline, Ex_Line, Next_Ex_Line, In_Pro, Wait, Total;
queue<Flow> Q;
map<string, int> To_Register;
int In[40], Out[40];
int Register[40];
bool Control_Hazard, Data_Hazard, Structue_Hazard, Access;
ifstream fin;

char S[10000];

int Space[N];
int C[40];
bool Data_now,Text_now;
string Str;
enum OpType{
    Align, Ascii, Asciiz, Byte, Half, Word, mySpace,     // 0 ~ 6
    Add, Addu, Addiu, Sub, Subu, Mul, Mulu, Div, Divu,
    // 7 ~ 15
    Xor, Xoru, Neg, Negu, Rem, Remu, Li,      // 16 ~ 22
    Seq, Sge, Sgt, Sle, Slt, Sne,         // 23 ~ 28
    B, Beq, Bne, Bge, Ble, Bgt, Blt, Beqz, Bnez, Blez, Bgez, Bgtz, Bltz,
    // 29 ~ 41
    J, Jr, Jal, Jalr,   // 42 ~ 45
    La, Lb, Lh, Lw,    // 46 ~ 49
    Sb, Sh, Sw,      // 50 ~ 52
    Move, Mfhi, Mflo,     // 53 ~ 55
    Nop, Syscall     // 56 ~ 57
};
map<string, int> Val;


struct Operator{
    vector<string> A;
    bool Transed;
}Line2[10000],Now2;


int Next[10000];
map<string, int> Start_line;
map<string, int> Start_pos;

int Stack_pos = 1000000, Static_pos = 0, Heap_pos;

void Init()
{
    Val[".align"] = 0;
    Val[".ascii"] = 1;
    Val[".asciiz"] = 2;
    Val[".byte"] = 3;
    Val[".half"] = 4;
    Val[".word"] = 5;
    Val[".space"] = 6;
    Val["add"] = 7;
    Val["addu"] = 8;
    Val["addiu"] = 9;
    Val["sub"] = 10;
    Val["subu"] = 11;
    Val["mul"] = 12;
    Val["mulu"] = 13;
    Val["div"] = 14;
    Val["divu"] = 15;
    Val["xor"] = 16;
    Val["xoru"] = 17;
    Val["neg"] = 18;
    Val["negu"] = 19;
    Val["rem"] = 20;
    Val["remu"] = 21;
    Val["li"] = 22;
    Val["seq"] = 23;
    Val["sge"] = 24;
    Val["sgt"] = 25;
    Val["sle"] = 26;
    Val["slt"] = 27;
    Val["sne"] = 28;
    Val["b"] = 29;
    Val["beq"] = 30;
    Val["bne"] = 31;
    Val["bge"] = 32;
    Val["ble"] = 33;
    Val["bgt"] = 34;
    Val["blt"] = 35;
    Val["beqz"] = 36;
    Val["bnez"] = 37;
    Val["blez"] = 38;
    Val["bgez"] = 39;
    Val["bgtz"] = 40;
    Val["bltz"] = 41;
    Val["j"] = 42;
    Val["jr"] = 43;
    Val["jal"] = 44;
    Val["jalr"] = 45;
    Val["la"] = 46;
    Val["lb"] = 47;
    Val["lh"] = 48;
    Val["lw"] = 49;
    Val["sb"] = 50;
    Val["sh"] = 51;
    Val["sw"] = 52;
    Val["move"] = 53;
    Val["mfhi"] = 54;
    Val["mflo"] = 55;
    Val["nop"] = 56;
    Val["syscall"] = 57;
    
    C[0]=1;
    for(int i = 1; i <= 30; i++)
        C[i] = C[i-1] << 1;
    
    Register[28] = Stack_pos;
    
    To_Register["$0"] = To_Register["$zero"] = 1;
    To_Register["$1"] = To_Register["$at"] = 2;
    To_Register["$2"] = To_Register["$v0"] = 3;
    To_Register["$3"] = To_Register["$v1"] = 4;
    To_Register["$4"] = To_Register["$a0"] = 5;
    To_Register["$5"] = To_Register["$a1"] = 6;
    To_Register["$6"] = To_Register["$a2"] = 7;
    To_Register["$7"] = To_Register["$a3"] = 8;
    To_Register["$8"] = To_Register["$t0"] = 9;
    To_Register["$9"] = To_Register["$t1"] = 10;
    To_Register["$10"] = To_Register["$t2"] = 11;
    To_Register["$11"] = To_Register["$t3"] = 12;
    To_Register["$12"] = To_Register["$t4"] = 13;
    To_Register["$13"] = To_Register["$t5"] = 14;
    To_Register["$14"] = To_Register["$t6"] = 15;
    To_Register["$15"] = To_Register["$t7"] = 16;
    To_Register["$16"] = To_Register["$s0"] = 17;
    To_Register["$17"] = To_Register["$s1"] = 18;
    To_Register["$18"] = To_Register["$s2"] = 19;
    To_Register["$19"] = To_Register["$s3"] = 20;
    To_Register["$20"] = To_Register["$s4"] = 21;
    To_Register["$21"] = To_Register["$s5"] = 22;
    To_Register["$22"] = To_Register["$s6"] = 23;
    To_Register["$23"] = To_Register["$s7"] = 24;
    To_Register["$24"] = To_Register["$t8"] = 25;
    To_Register["$25"] = To_Register["$t9"] = 26;
    To_Register["$28"] = To_Register["$gp"] = 27;
    To_Register["$29"] = To_Register["$sp"] = 28;
    To_Register["$30"] = To_Register["$fp"] = 29;
    To_Register["$31"] = To_Register["$ra"] = 30;
    To_Register["$hi"] = 31;
    To_Register["$lo"] = 32;
    To_Register["$pc"] = 33;
}

void Read()
{
    getline(cin, Str);
    cout<<Str<<endl;
}

void ID_Skip_Space(string s, int &pos, int len)
{
    //cout<<"s "<<pos<<endl;
    while(pos < len && (s[pos] == ' ' || s[pos] == ',' || s[pos] == '\t'))
        pos++;
    //cout<<"t "<<pos<<endl;
}

string Scanner(string s, int &pos, int len)
{
    string tmp = "";
    if(s[pos] == '"')
    {
        while(1)
        {
            pos++;
            if(s[pos] == '\\')
            {
                pos++;
                if(s[pos] == 'n')
                    tmp += '\n';
                else if(s[pos] == '"')
                    tmp += '"';
                else if(s[pos] == '\\')
                    tmp += '\\';
                continue;
            }
            if(s[pos] == '"')
            {
                pos++;
                break;
            }
            else tmp += s[pos];
        }
        return tmp;
    }
    while(pos != len && s[pos] != ' ' && s[pos] != ',')
    {
        tmp += s[pos];
        pos++;
    }
    return tmp;
}

int To_num(string s)
{
    int len = (int)s.length();
    int sum = 0, f = 1;
    int i = 0;
    if(s[0] == '-')
        f = -1, i = 1;
    for( ; i < len; i++)
        sum = sum * 10 + (s[i] - '0');
    return sum * f;
}

/*void Add_Half()
{
    Space[Static_pos++].Type = 'B';
    Space[Static_pos++].Type = 'B';
}

void Add_Word()
{
    for(int i = 1; i <= 4; i++)
        Space[Static_pos++].Type = 'C';
}*/

void Ex_Align(int line_num)
{
    int num = To_num(Line2[line_num].A[1]);
    while(Static_pos % C[num] != 0)
        Static_pos++;
}

void Ex_Ascii(int line_num)
{
    string tmp = Line2[line_num].A[1];
    int len = (int)tmp.length();
    for(int i = 0; i < len ; i++)
    {
        if(tmp[i] == '\\')
        {
            if(tmp[i+1] == 'n')
                Space[Static_pos++] = 10;
            else if(tmp[i+1] == '"')
                Space[Static_pos++] = 34;
            else if(tmp[i+1] == '\\')
                Space[Static_pos++] = 92;
            i++;
        }
        else Space[Static_pos++] = (int)tmp[i];
    }
}

void Ex_Asciiz(int line_num)
{
    string tmp = Line2[line_num].A[1];
    int len = (int)tmp.length();
    for(int i = 0; i < len ; i++)
    {
        if(tmp[i] == '\\')
        {
            if(tmp[i+1] == 'n')
                Space[Static_pos++] = 10;
            else if(tmp[i+1] == '"')
                Space[Static_pos++] = 34;
            else if(tmp[i+1] == '\\')
                Space[Static_pos++] = 92;
            i++;
        }
        else Space[Static_pos++] = (int)tmp[i];
    }
    Space[Static_pos++] = 0;
}

void Ex_Byte(int line_num)
{
    int Size = (int)Line2[line_num].A.size();
    for(int i = 1; i < Size; i++)
    {
        Space[Static_pos] = (int)Line2[line_num].A[i][0];
        Static_pos+=1;
    }
}

void Ex_Half(int line_num)
{
    int Size = (int)Line2[line_num].A.size();
    for(int i = 1; i < Size; i++)
    {
        Space[Static_pos] = To_num(Line2[line_num].A[i]);
        Static_pos+=2;
    }
}

void Ex_Word(int line_num)
{
    int Size = (int)Line2[line_num].A.size();
    for(int i = 1; i < Size; i++)
    {
        Space[Static_pos] = To_num(Line2[line_num].A[i]);
        Static_pos+=4;
    }
}

void Ex_Space(int line_num)
{
    int num = To_num(Line2[line_num].A[1]);
    for(int i = 1; i <= num; i++)
    {
        Space[Static_pos++] = 0;
    }
}

void ID_Kind1(Flow_Data &x, int line_num, Flow_Data &y)
{
    if(!Line2[line_num].Transed)
    {
        Line2[line_num].Transed = true;
        x.Rec = x.a1 = To_Register[Line2[line_num].A[1]];
        x.a2 = To_Register[Line2[line_num].A[2]];
        x.a3 = To_Register[Line2[line_num].A[3]];
        if(x.a3 == 0)
            x.Is_Num = true, x.a3 = To_num(Line2[line_num].A[3]);
    }
    y = x;
    if(Out[y.a2] || (!y.Is_Num && Out[y.a3]))
    {
        Data_Hazard = true;
        return;
    }
    y.a2 = Register[y.a2];
    if(!y.Is_Num)
        y.a3 = Register[y.a3];
    Out[y.a1]++;
}

void ID_Kind2(Flow_Data &x, int line_num, Flow_Data &y)
{
    if(!Line2[line_num].Transed)
    {
        Line2[line_num].Transed = true;
        x.Rec = 31;
        x.Rec2 = 32;
        x.a1 = To_Register[Line2[line_num].A[1]];
        x.a2 = To_Register[Line2[line_num].A[2]];
        if(x.a2 == 0)
            x.Is_Num = true, x.a2 = To_num(Line2[line_num].A[2]);
    }
    y = x;
    if(Out[y.a1] || (!y.Is_Num && Out[y.a2]))
    {
        Data_Hazard = true;
        return;
    }
    y.a1 = Register[y.a1];
    if(!y.Is_Num)
        y.a2 = Register[y.a2];
    Out[31]++;
    Out[32]++;
}

void ID_Kind3(Flow_Data &x, int line_num, Flow_Data &y)
{
    if(!Line2[line_num].Transed)
    {
        Line2[line_num].Transed = true;
        x.Rec = x.a1 = To_Register[Line2[line_num].A[1]];
        x.a2 = To_Register[Line2[line_num].A[2]];
    }
    y = x;
    if(Out[y.a2])
    {
        Data_Hazard = true;
        return;
    }
    y.a2 = Register[y.a2];
    Out[y.a1]++;
}

void ID_Kind4(Flow_Data &x, int line_num, Flow_Data &y)
{
    if(!Line2[line_num].Transed)
    {
        Line2[line_num].Transed = true;
        x.Rec = x.a1 = To_Register[Line2[line_num].A[1]];
        x.a2 = To_num(Line2[line_num].A[2]);
    }
    y = x;
    Out[y.a1]++;
}

void ID_Kind5(Flow_Data &x, int line_num, Flow_Data &y)
{
    if(!Line2[line_num].Transed)
    {
        Line2[line_num].Transed = true;
        x.Rec = 34;
        x.a1 = To_Register[Line2[line_num].A[1]];
        x.a2 = To_Register[Line2[line_num].A[2]];
        if(x.a2 == 0)
            x.Is_Num = true, x.a2 = To_num(Line2[line_num].A[2]);
        x.a3 = Start_line[Line2[line_num].A[3]];
    }
    y = x;
    if(Out[y.a1] || (!y.Is_Num && Out[y.a2]))
    {
        Data_Hazard = true;
        return;
    }
    y.a1 = Register[y.a1];
    if(!y.Is_Num)
        y.a2 = Register[y.a2];
    Out[y.Rec]++;
}

void ID_Kind6(Flow_Data &x,int line_num, Flow_Data &y)
{
    if(!Line2[line_num].Transed)
    {
        Line2[line_num].Transed = true;
        x.Rec = 34;
        x.a1 = To_Register[Line2[line_num].A[1]];
        x.a2 = Start_line[Line2[line_num].A[2]];
    }
    y = x;
    if(Out[y.a1])
    {
        Data_Hazard = true;
        return;
    }
    y.a1 = Register[y.a1];
    Out[y.Rec]++;
}

void ID_Kind7(Flow_Data &x, int line_num, Flow_Data &y)
{
    if(!Line2[line_num].Transed)
    {
        Line2[line_num].Transed = true;
        x.Rec = 34;
        x.a1 = Start_line[Line2[line_num].A[1]];
    }
    y = x;
    Out[y.Rec]++;
}

void ID_Kind8(Flow_Data &x, int line_num, Flow_Data &y)
{
    //cout<<"hehe"<<endl;
    if(!Line2[line_num].Transed)
    {
        Line2[line_num].Transed = true;
        x.Rec = 34;
        //cout<<"pppppp "<<Line2[line_num].A[1]<<endl;
        x.a1 = To_Register[Line2[line_num].A[1]];
    }
    y = x;
    if(Out[y.a1])
    {
        Data_Hazard = true;
        return;
    }
    y.a1 = Register[y.a1];
    Out[y.Rec]++;
}

void ID_Kind9(Flow_Data &x, int line_num, Flow_Data &y)
{
    if(!Line2[line_num].Transed)
    {
        Line2[line_num].Transed = true;
        x.Rec = 30;
        x.Rec2 = 34;
        x.a2 = Start_line[Line2[line_num].A[1]];
    }
    y = x;
    y.a1 = Register[34];
    Out[y.Rec]++;
    Out[y.Rec2]++;
}

void ID_Kind10(Flow_Data &x, int line_num, Flow_Data &y)
{
    if(!Line2[line_num].Transed)
    {
        Line2[line_num].Transed = true;
        x.Rec = 30;
        x.Rec2 = 34;
        x.a2 = To_Register[Line2[line_num].A[1]];
    }
    y = x;
    if(Out[y.a2])
    {
        Data_Hazard = true;
        return;
    }
    y.a1 = Register[34];
    y.a2 = Register[y.a2];
    Out[y.Rec]++;
    Out[y.Rec2]++;
}

void Trans(Flow_Data &x, int line_num)
{
    string s = Line2[line_num].A[2];
    if(s[0] == '_')
    {
        x.a1 = Start_pos[s];
        x.Is_Num = true;
        x.a2 = 0;
        return;
    }
    int pos = 0, f = 1, sum = 0;
    while(s[pos] != '(')
        pos++;
    int i = 0;
    if(s[0] == '-')
        f = -1, i = 1;
    for(; i < pos; i++)
        sum = sum * 10 + (s[i] - '0');
    pos ++;
    int pos1 = pos;
    while(s[pos1] != ')')
        pos1++;
    string tmp;
    tmp.assign(s, pos, pos1 - pos);
    x.a1 = To_Register[tmp];
    x.a2 = sum * f;
}

void ID_Kind11(Flow_Data &x, int line_num, Flow_Data &y)
{
    if(!Line2[line_num].Transed)
    {
        Line2[line_num].Transed = true;
        x.Rec = To_Register[Line2[line_num].A[1]];
        Trans(x, line_num);
    }
    y = x;
    if(!y.Is_Num && Out[y.a1])
    {
        Data_Hazard = true;
        return;
    }
    if(!y.Is_Num)
        y.a1 = Register[y.a1];
    Out[y.Rec]++;
}

void ID_Kind12(Flow_Data &x,int line_num, Flow_Data &y)
{
    if(!Line2[line_num].Transed)
    {
        Line2[line_num].Transed = true;
        x.a3 = To_Register[Line2[line_num].A[1]];
        Trans(x, line_num);
    }
    y = x;
    if(Out[y.a3] && Out[y.a1])
    {
        Data_Hazard = true;
        return;
    }
    if(!y.Is_Num)
        y.a1 = Register[y.a1];
    y.a3 = Register[y.a3];
}

void ID_Kind13(Flow_Data &x, int line_num, int src, Flow_Data &y)
{
    if(!Line2[line_num].Transed)
    {
        Line2[line_num].Transed = true;
        x.Rec = To_Register[Line2[line_num].A[1]];
        x.a1 = src;
    }
    y = x;
    if(Out[y.a1])
    {
        Data_Hazard = true;
        return;
    }
    y.a1 = Register[y.a1];
    Out[y.Rec]++;
}

void ID_Kind14(Flow_Data &x, Flow_Data &y)
{
    if(Out[3])
    {
        Data_Hazard = true;
        return;
    }
    else
    {
        if(Register[3] == 1)
        {
            if(Out[5])
            {
                Data_Hazard = true;
                return;
            }
        }
        else if(Register[3] == 4)
        {
            if(Out[5])
            {
                Data_Hazard = true;
                return;
            }
        }
        else if(Register[3] == 5)
        {
            x.Rec = 3;
            Out[3]++;
        }
        else if(Register[3] == 8)
        {
            if(Out[5])
            {
                Data_Hazard = true;
                return;
            }
            x.Rec = 6;
            Out[x.Rec]++;
        }
        else if(Register[3] == 9)
        {
            if(Out[5])
            {
                Data_Hazard = true;
                return;
            }
            x.Rec = 3;
            Out[3]++;
        }
        else if(Register[3] == 17)
        {
            if(Out[5])
            {
                Data_Hazard = true;
                return;
            }
        }
    }
    y = x;
}

void Ex_Store(int pos, int x, int len)
{
    //cout<<"st "<<pos<<" -> "<<x<<endl;
    Space[pos] = x;
}

void Save(string s)
{
    int line_num = Total;
    int len = (int)s.length();
    int now = 0;
    string Token, First_Token = "gzpp";
    while(1)
    {
        ID_Skip_Space(s, now, len);
        Token = Scanner(s, now, len);
        if(First_Token == "gzpp")
        {
            First_Token = Token;
            if(First_Token == ".text")
            {
                Text_now = true;
                Data_now = false;
                Total--;
                return;
            }
            else if(First_Token == ".data")
            {
                Text_now = false;
                Data_now = true;
                Total--;
                return;
            }
        }
        if(Token.length() != 0)
            Line2[line_num].A.push_back(Token);
        if(now == len)
            break;
    }
    if(First_Token[First_Token.length() - 1] == ':')
    {
        string tmp;
        tmp.assign(First_Token, 0, First_Token.length()-1);
        Start_line[tmp] = line_num + 1;
        Start_pos[tmp] = Static_pos;
    }
    else if(Data_now)
    {
        switch (Val[First_Token]) {
            case 0:
                Ex_Align(line_num);
                break;
            case 1:
                Ex_Ascii(line_num);
                break;
            case 2:
                //break;
                Ex_Asciiz(line_num);
                break;
            case 3:
                Ex_Byte(line_num);
                break;
            case 4:
                Ex_Half(line_num);
                break;
            case 5:
                Ex_Word(line_num);
                break;
            case 6:
                Ex_Space(line_num);
                break;
            default:
                break;
        }
    }
    else
        Line[line_num].Op_Val = Val[First_Token];
    /*cout<<line_num<<" ";
     for(int i = 0; i< Line2[line_num].A.size(); i++)
     cout<<Line2[line_num].A[i]<<" ";
     cout<<endl;*/
}

int IF()
{
    if(Register[34] > Total)
        return -1;
    while(Line2[Register[34]].A[0][0] == '_')
        Register[34]++;
    int tmp = Register[34];
    Register[34]++;
    if(Line[tmp].Op_Val >= 29 && Line[tmp].Op_Val <= 45)
        Wait = true;
    return tmp;
}

void ID_DP(Flow_Data &x, int line_num, Flow_Data &y)
{
    switch (x.Op_Val) {
        case 7:
            ID_Kind1(x, line_num, y);
            break;
        case 8:
            ID_Kind1(x, line_num, y);
            break;
        case 9:
            ID_Kind1(x, line_num, y);
            break;
        case 10:
            ID_Kind1(x, line_num, y);
            break;
        case 11:
            ID_Kind1(x, line_num, y);
            break;
        case 12:
            if(Line2[line_num].A.size() == 4)
                ID_Kind1(x, line_num, y);
            else ID_Kind2(x, line_num, y);
            break;
        case 13:
            if(Line2[line_num].A.size() == 4)
                ID_Kind1(x, line_num, y);
            else ID_Kind2(x, line_num, y);
            break;
        case 14:
            if(Line2[line_num].A.size() == 4)
                ID_Kind1(x, line_num, y);
            else ID_Kind2(x, line_num, y);
            break;
        case 15:
            if(Line2[line_num].A.size() == 4)
                ID_Kind1(x, line_num, y);
            else ID_Kind2(x, line_num, y);
            break;
        case 16:
            ID_Kind1(x, line_num, y);
            break;
        case 17:
            ID_Kind1(x, line_num, y);
            break;
        case 18:
            ID_Kind3(x, line_num, y);
            break;
        case 19:
            ID_Kind3(x, line_num, y);
            break;
        case 20:
            ID_Kind1(x, line_num, y);
            break;
        case 21:
            ID_Kind1(x, line_num, y);
            break;
        case 22:
            ID_Kind4(x, line_num, y);
            break;
        case 23:
            ID_Kind1(x, line_num, y);
            break;
        case 24:
            ID_Kind1(x, line_num, y);
            break;
        case 25:
            ID_Kind1(x, line_num, y);
            break;
        case 26:
            ID_Kind1(x, line_num, y);
            break;
        case 27:
            ID_Kind1(x, line_num, y);
            break;
        case 28:
            ID_Kind1(x, line_num, y);
            break;
        case 29:
            if(!Line2[line_num].Transed)
            {
                Line2[line_num].Transed = true;
                x.Rec = 34;
                x.a1 = Start_line[Line2[line_num].A[1]];
            }
            y = x;
            Out[y.Rec]++;
            break;
        case 30:
            ID_Kind5(x, line_num, y);
            break;
        case 31:
            ID_Kind5(x, line_num, y);
            break;
        case 32:
            ID_Kind5(x, line_num, y);
            break;
        case 33:
            ID_Kind5(x, line_num, y);
            break;
        case 34:
            ID_Kind5(x, line_num, y);
            break;
        case 35:
            ID_Kind5(x, line_num, y);
            break;
        case 36:
            ID_Kind6(x, line_num, y);
            break;
        case 37:
            ID_Kind6(x, line_num, y);
            break;
        case 38:
            ID_Kind6(x, line_num, y);
            break;
        case 39:
            ID_Kind6(x, line_num, y);
            break;
        case 40:
            ID_Kind6(x, line_num, y);
            break;
        case 41:
            ID_Kind6(x, line_num, y);
            break;
        case 42:
            ID_Kind7(x, line_num, y);
            break;
        case 43:
            ID_Kind8(x, line_num, y);
            break;
        case 44:
            ID_Kind9(x, line_num, y);
            break;
        case 45:
            ID_Kind10(x, line_num, y);
            break;
        case 46:
            ID_Kind11(x, line_num, y);
            break;
        case 47:
            ID_Kind11(x, line_num, y);
            break;
        case 48:
            ID_Kind11(x, line_num, y);
            break;
        case 49:
            ID_Kind11(x, line_num, y);
            break;
        case 50:
            ID_Kind12(x, line_num, y);
            break;
        case 51:
            ID_Kind12(x, line_num, y);
            break;
        case 52:
            ID_Kind12(x, line_num, y);
            break;
        case 53:
            ID_Kind3(x, line_num, y);
            break;
        case 54:
            ID_Kind13(x, line_num, 31, y);
            break;
        case 55:
            ID_Kind13(x, line_num, 32, y);
            break;
        case 56:
            break;
        case 57:
            ID_Kind14(x, y);
            break;
        default:
            break;
    }
}

void Exe(Flow_Data &x, int line_num)
{
    switch (x.Op_Val) {
        case 7:
            x.Result = x.a2 + x.a3;
            break;
        case 8:
            x.Result = ((UI)x.a2 + (UI)x.a3);
            break;
        case 9:
            x.Result = ((UI)x.a2 + (UI)x.a3);
            break;
        case 10:
            x.Result = x.a2 - x.a3;
            break;
        case 11:
            x.Result = ((UI)x.a2 - (UI)x.a3);
            break;
        case 12:
            if(Line2[line_num].A.size() == 4)
                x.Result = x.a2 * x.a3;
            else
            {
                long long tmp = (long long)x.a1 * (long long)x.a2;
                x.Result = tmp >> 32;
                x.Result2 = tmp - (x.Result << 32);
            }
            break;
        case 13:
            if(Line2[line_num].A.size() == 4)
                x.Result = ((UI)x.a2 * (UI)x.a3);
            else
            {
                long long tmp = ((UL)x.a1 * (UL)x.a2);
                x.Result = tmp >> 32;
                x.Result2 = tmp - (x.Result << 32);
            }
            break;
        case 14:
            if(Line2[line_num].A.size() == 4)
                x.Result = x.a2 / x.a3;
            else
            {
                x.Result = x.a1 % x.a2;
                x.Result2 = x.a1 / x.a2;
            }
            break;
        case 15:
            if(Line2[line_num].A.size() == 4)
                x.Result = ((UI)x.a2 / (UI)x.a3);
            else
            {
                x.Result = ((UI)x.a1 % (UI)x.a2);
                x.Result2 = ((UI)x.a1 / (UI)x.a2);
            }
            break;
        case 16:
            x.Result = x.a2 ^ x.a3;
            break;
        case 17:
            x.Result = ((UI)x.a2 ^ (UI)x.a3);
            break;
        case 18:
            x.Result = x.a2 * (-1);
            break;
        case 19:
            x.Result = abs(x.a2);
            break;
        case 20:
            x.Result = x.a2 % x.a3;
            break;
        case 21:
            x.Result = ((UI)x.a2 % (UI)x.a3);
            break;
        case 22:
            x.Result = x.a2;
            break;
        case 23:
            x.Result = (x.a2 == x.a3);
            break;
        case 24:
            x.Result = (x.a2 >= x.a3);
            break;
        case 25:
            x.Result = (x.a2 > x.a3);
            break;
        case 26:
            x.Result = (x.a2 <= x.a3);
            break;
        case 27:
            x.Result = (x.a2 < x.a3);
            break;
        case 28:
            x.Result = (x.a2 != x.a3);
            break;
        case 29:
            x.Result = x.a1;
            break;
        case 30:
            x.Result = (x.a1 == x.a2) ? x.a3 : Register[34];
            break;
        case 31:
            x.Result = (x.a1 != x.a2) ? x.a3 : Register[34];
            break;
        case 32:
            x.Result = (x.a1 >= x.a2) ? x.a3 : Register[34];
            break;
        case 33:
            x.Result = (x.a1 <= x.a2) ? x.a3 : Register[34];
            break;
        case 34:
            x.Result = (x.a1 > x.a2) ? x.a3 : Register[34];
            break;
        case 35:
            x.Result = (x.a1 < x.a2) ? x.a3 : Register[34];
            break;
        case 36:
            x.Result = (x.a1 == 0) ? x.a2 : Register[34];
            break;
        case 37:
            x.Result = (x.a1 != 0) ? x.a2 : Register[34];
            break;
        case 38:
            x.Result = (x.a1 <= 0) ? x.a2 : Register[34];
            break;
        case 39:
            x.Result = (x.a1 >= 0) ? x.a2 : Register[34];
            break;
        case 40:
            x.Result = (x.a1 > 0) ? x.a2 : Register[34];
            break;
        case 41:
            x.Result = (x.a1 < 0) ? x.a2 : Register[34];
            break;
        case 42:
            x.Result = x.a1;
            break;
        case 43:
            x.Result = x.a1;
            break;
        case 44:
            x.Result = x.a1;
            x.Result2 = x.a2;
            break;
        case 45:
            x.Result = x.a1;
            x.Result2 = x.a2;
            break;
        case 46:
            x.Result = x.a1 + x.a2;
            break;
        case 47:
            x.Result = x.a1 + x.a2;
            break;
        case 48:
            x.Result = x.a1 + x.a2;
            break;
        case 49:
            x.Result = x.a1 + x.a2;
            break;
        case 50:
            x.Result = x.a1 + x.a2;
            x.Result2 = x.a3;
            break;
        case 51:
            x.Result = x.a1 + x.a2;
            x.Result2 = x.a3;
            break;
        case 52:
            x.Result = x.a1 + x.a2;
            x.Result2 = x.a3;
            break;
        case 53:
            x.Result = x.a2;
            break;
        case 54:
            x.Result = x.a1;
            break;
        case 55:
            x.Result = x.a1;
            break;
        case 56:
            break;
        case 57:
            if(Register[3] == 1)
            {
                //cout<<"qgggggggggggg->";
                printf("%d",Register[5]);
            }
            else if(Register[3] == 4)
            {
                int pos = Register[5];
                //cout<<pos<<endl;
                //cout<<endl;
                while(1)
                {
                    if(Space[pos] == 0)
                        break;
                    char ch = (char)Space[pos];
                    printf("%c",ch);
                    pos++;
                }
            }
            else if(Register[3] == 5)
            {
                int tmp;
                scanf("%d",&tmp);
                //cout<<"s -> "<<tmp<<endl;
                x.Result = tmp;
            }
            else if(Register[3] == 8)
            {
                scanf("%s",S);
                //printf("s -> %s",S);
                x.Result = (int)strlen(S) + 1;
                for(int i = 1; i <= x.Result - 1; i++)
                {
                    Space[Register[5] + i - 1] = (int)S[i - 1];
                }
                Space[Register[5] + x.Result - 1] = 0;
            }
            else if(Register[3] == 9)
            {
                x.Result = Heap_pos;
                Heap_pos = Heap_pos + Register[5] + 1;
            }
            else if(Register[3] == 10)
            {
                exit(0);
            }
            else if(Register[3] == 17)
            {
                exit(Register[5]);
            }
            break;
        default:
            break;
    }
}

void MA(Flow_Data &x)
{
    if(x.Op_Val >= 47 && x.Op_Val <= 52)
    {
        /*if(Access)
         {/Users/mac/Desktop/MipsTest/123/234/5.in
         Structue_Hazard = true;
         return;
         }*/
        //if((int)x.Result < 0)
        //    cout<<"fuck "<<x.Op_Val<<" "<<x.Result<<endl;
        switch (x.Op_Val) {
            case 47:
                x.Result = Space[(int)x.Result];
                break;
            case 48:
                x.Result = Space[(int)x.Result];
                break;
            case 49:
                x.Result = Space[(int)x.Result];
                break;
            case 50:
                Ex_Store((int)x.Result, (int)x.Result2, 1);
                break;
            case 51:
                Ex_Store((int)x.Result, (int)x.Result2, 2);
                break;
            case 52:
                Ex_Store((int)x.Result, (int)x.Result2, 4);
                break;
            default:
                break;
        }
        //Access = true;
    }
}

void WB(Flow_Data x)
{
    if(x.Rec)
        Register[x.Rec] = (int)x.Result, Out[x.Rec]--;
    if(x.Rec2)
        Register[x.Rec2] = (int)x.Result2, Out[x.Rec2]--;
}

int main(int argv,char *argc[])
{
    Init();
    fin.open(argc[1]);
    //freopen("1.s","r",stdin);
    //freopen("4.out","w",stdout);
    Last_Lableline = -1;
    while(getline(fin, Str))
    {
        if(Str.length() == 0)
            continue;
        bool Skip = true;
        int qwer = Str.length();
        for(int i = 0 ; i < qwer; i++)
            if(Str[i] != ' ' && Str[i] != '\t')
                Skip = false;
        if(Skip)
            continue;
        Total++;
        Save(Str);
    }
    fin.close();
    //return 0;
    //freopen("1.in","r",stdin);
    Heap_pos = Static_pos + 1;
    //cout<<"haha "<<endl;
    Register[34] = Start_line["main"];
    int nowline;
    while(1)
    {
        nowline = IF();
        if(nowline == -1)
            break;
        //cout<<nowline<<" "<<Register[29]<<endl;
        //cout<<nowline<<" "<<Line[nowline].Op_Val<<" "<<Register[30]<<" "<<Register[34]<<endl;
        ID_DP(Line[nowline], nowline, Now);
        //cout<<Now.Rec<<" "<<Now.a1<<endl;
        Exe(Now, nowline);
        MA(Now);
        WB(Now);
        //cout<<nowline<<" "<<Register[30]<<" "<<Register[34]<<endl;
    }
    return 0;
}

/*int main(int argc, char *argv[])
{
    Init();
    fin.open(argv[1]);
    //freopen("1.s","r",stdin);
    //freopen("4.out","w",stdout);
    Last_Lableline = -1;
    while(getline(fin, Str))
    {
        if(Str.length() == 0)
            continue;
        bool Skip = true;
        int qwer = Str.length();
        for(int i = 0 ; i < qwer; i++)
            if(Str[i] != ' ' && Str[i] != '\t')
                Skip = false;
        if(Skip)
            continue;
        Total++;
        Save(Str);
    }
    fin.close();
    //freopen("1.in","r",stdin);
    Heap_pos = Static_pos + 1;
    //cout<<"haha "<<endl;
    Register[34] = Start_line["main"];
    int nowline;
    while(1)
    {
        nowline = IF();
        if(nowline == -1)
            break;
        //cout<<nowline<<" "<<Register[29]<<endl;
        //cout<<nowline<<" "<<Line[nowline].Op_Val<<" "<<Register[30]<<" "<<Register[34]<<endl;
        ID_DP(Line[nowline], nowline, Now);
        //cout<<Now.Rec<<" "<<Now.a1<<endl;
        Exe(Now, nowline);
        MA(Now);
        WB(Now);
        //cout<<nowline<<" "<<Register[30]<<" "<<Register[34]<<endl;
    }
    return 0;
    Ex_Line = Start_line["main"];
     //cout<<"ssss -> "<<Ex_Line<<endl;
     Register[34] = Ex_Line;
     Q.push((Flow){0, 1, Now});
     In_Pro++;
     int RR = 0;
     while(1)
     {
     int cnt = min(5, In_Pro);
     Data_Hazard = false;
     //cout<<Register[34]<<" "<<Register[30]<<" "<<endl;
     for(int i = 1; i <= cnt; i++)
     {
     Flow now = Q.front();
     Q.pop();
     cout<<now.Line_num<<" "<<Register[30]<<" "<<Register[28]<<endl;
     if(Data_Hazard)
     {
     Q.push(now);
     continue;
     }
     if(now.Line_num == -1)
     {
     In_Pro--;
     continue;
     }
     switch (now.Process) {
     case 1:
     now.Line_num = IF();
     break;
     case 2:
     ID_DP(Line[now.Line_num], now.Line_num, now.x);
     break;
     case 3:
     Exe(now.x, now.Line_num);
     break;
     case 4:
     MA(now.x);
     break;
     case 5:
     WB(now.x);
     In_Pro--;
     break;
     default:
     break;
     }
     //cout<<"mao  "<<Data_Hazard<<endl;
     if(Data_Hazard)
     {
     Q.push(now);
     continue;
     }
     now.Process++;
     if(now.Process <= 5)
     Q.push(now);
     }
     if(!Wait)
     {
     if(In_Pro < 5)
     {
     Q.push((Flow){0, 1, Now});
     In_Pro++;
     }
     }
     else
     {
     if(!In_Pro)
     {
     Q.push((Flow){0, 1, Now});
     In_Pro++;
     Wait = false;
     }
     }
     }*/
//    return 0;
//}

