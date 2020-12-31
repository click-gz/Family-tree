#include <iostream>
#include<fstream>
#include<queue>
#include<string>
#include<sstream>
using namespace std;
//姓名、出生日期、婚否、地址、健在否、死亡日期（若其已死亡），也可附加其它信息、但不是必需的。
typedef struct Date {
    int y, m, d;
}Date;

typedef struct Person {
    Person* parent;
    string Name;
    Date *Birthday;
    bool Ifmarry;
    string Place;
    bool Alive;
    Date *Deadday;
    Person *child[20];
    int Familylevel;
}Person,*PList;
void Save(PList FT) {
    fstream f;
    f.open("family tree.txt", ios::binary | ios::out);
    if (!f) {
        cout << "open error" << endl;
        exit(0);
    }
    queue<Person*>Q;
    Q.push(FT);
    while (Q.size() != 0) {
        Person* t = Q.front();
        
        f << t->Name << " ";
        if (t->parent == NULL) {
            f << "0" << " ";
        }
        else {
            f << t->parent->Name << " ";
        }
        f << t->Birthday->y << "-" << t->Birthday->m << "-" << t->Birthday->d << " "<<t->Ifmarry<<" ";
        f << t->Place << " "<<t->Alive<<" ";
        f << t->Deadday->y << "-" << t->Deadday->m << "-" << t->Deadday->d << " ";
        if (t->child[0] == NULL) {
            f << "0";
        }
        else {
            for (int i = 0; t->child[i] != NULL; i++) {
                if (t->child[i]->Name != "0")
                    Q.push(t->child[i]);
                f << t->child[i]->Name << " ";
            }
        }
        Q.pop();
        if (!Q.empty()) {
            f << endl;
        }
    }
}
void get_parent(PList FT, string s,Person* p) {
    queue<Person*>Q;
    Q.push(FT);
    while (Q.size()!=0) {
        Person* t = Q.front();
        if (t->Name == s) {
            for (int i = 0; t->child[i] != NULL; i++) {
                if (t->child[i]->Name == p->Name) {
                    p->parent = t;
                    t->child[i] = p;
                    p->Familylevel = t->Familylevel + 1;
                    return;
                }
                Q.push(t->child[i]);
            }

        }
        else {
            for (int i = 0; t->child[i] != NULL && t->child[i]->Name!="0"; i++) {

                Q.push(t->child[i]);
            }
        }
        Q.pop();
    }
}
void Init(PList& FT) {
    fstream f;
    f.open("family tree.txt", ios::in);
    if (!f) {
        cout << "open error" << endl;
        exit(0);
    }
    string content;
    
    while (!f.eof()) {
        getline(f, content);
        int count = 0;
        if (FT == NULL) {
            FT = new Person;
            FT->Familylevel = 1;
            for(int i=0;i<20;i++)
                FT->child[i] = { NULL };
            string s = "";
            istringstream ss(content);
            while (ss >> s) {
                if (count == 0) {
                    FT->Name = s;
                }
                else if (count == 1) {
                    FT->parent = NULL;
                }
                else if (count == 2) {
                    Date* date = new Date;
                    date->y = 0;
                    date->m = 0;
                    date->d = 0;
                    int k = 0;
                    for (int t = 0; t < 3; t++) {
                        int num = 0;
                        while (s[k] != '-'&&s[k]!='\0') {
                            num *= 10;
                            num += (s[k] - '0');
                            k++;
                        }
                        if (date->y == 0) {
                            date->y = num;
                        }
                        else if (date->m == 0) {
                            date->m = num;
                        }
                        else {
                            date->d = num;
                        }
                        k++;
                    }
                    FT->Birthday = date;
                }
                else if (count == 3) {
                    if (s == "0") {
                        FT->Ifmarry = 0;
                    }
                    else {
                        FT->Ifmarry = 1;
                    }
                }
                else if (count == 4) {
                    FT->Place = s;
                }
                else if (count == 5) {
                    if (s == "0") {
                        FT->Alive = 0;
                    }
                    else {
                        FT->Alive = 1;
                    }
                }
                else if (count == 6) {
                    Date* date = new Date;
                    date->y = 0;
                    date->m = 0;
                    date->d = 0;
                    int k = 0;
                    for (int t = 0; t < 3; t++) {
                        int num = 0;
                        while (s[k] != '-' && s[k] != '\0') {
                            num *= 10;
                            num += (s[k] - '0');
                            k++;
                        }
                        if (date->y == 0) {
                            date->y = num;
                        }
                        else if (date->m == 0) {
                            date->m = num;
                        }
                        else {
                            date->d = num;
                        }
                        k++;
                    }
                    FT->Deadday = date;
                }
                else {
                    Person* temp = new Person;
                    FT->child[count - 7] = temp;
                    temp->Name = s;
                }
                count++;
            }
        }
        else {
            Person* temp = new Person;
            for (int i = 0; i < 20; i++) {
                temp->child[i] = { NULL };
            }
            string s = "";
            istringstream ss(content);
            while (ss >> s) {
                if (count == 0) {
                    temp->Name = s;
                }
                else if (count == 1) {
                    get_parent(FT, s, temp);
                }
                else if (count == 2) {
                    Date* date = new Date;
                    date->y = 0;
                    date->m = 0;
                    date->d = 0;
                    int k = 0;
                    for (int t = 0; t < 3; t++) {
                        int num = 0;
                        while (s[k] != '-' && s[k] != '\0') {
                            num *= 10;
                            num += (s[k] - '0');
                            k++;
                        }
                        if (date->y == 0) {
                            date->y = num;
                        }
                        else if (date->m == 0) {
                            date->m = num;
                        }
                        else {
                            date->d = num;
                        }
                        k++;
                    }
                    temp->Birthday = date;
                }
                else if (count == 3) {
                    if (s == "0") {
                        temp->Ifmarry = 0;
                    }
                    else {
                        temp->Ifmarry = 1;
                    }
                }
                else if (count == 4) {
                    temp->Place = s;
                }
                else if (count == 5) {
                    if (s == "0") {
                        temp->Alive = 0;
                    }
                    else {
                        temp->Alive = 1;
                    }
                }
                else if (count == 6) {
                    Date* date = new Date;
                    date->y = 0;
                    date->m = 0;
                    date->d = 0;
                    int k = 0;
                    for (int t = 0; t < 3; t++) {
                        int num = 0;
                        while (s[k] != '-' && s[k] != '\0') {
                            num *= 10;
                            num += (s[k] - '0');
                            k++;
                        }
                        if (date->y == 0) {
                            date->y = num;
                        }
                        else if (date->m == 0) {
                            date->m = num;
                        }
                        else {
                            date->d = num;
                        }
                        k++;
                    }
                    temp->Deadday = date;
                }
                else {
                    if (s != "0") {
                        Person* ttemp = new Person;
                        temp->child[count - 7] = ttemp;
                        ttemp->Name = s;
                    }
                }
                count++;
            }
        }
        cout << content << endl;
    }
    f.close();
}
void show_single(Person* p) {
    cout << "姓名：" << p->Name <<" ";
    cout << "生平居住在：" << p->Place << endl;
    if (p->parent) {
        cout << "父亲姓名：" << p->parent->Name<<endl;
    }
    cout << "出生日期：" << p->Birthday->y << "-" << p->Birthday->m << "-" << p->Birthday->d << endl;
    if (p->Alive) {
        cout << "仍健在" << endl;
    }
    else {
        cout << "去世日期为：" << p->Deadday->y << "-" << p->Deadday->m << "-" << p->Deadday->d << endl;
    }
    if (p->Ifmarry) {
        cout << "已婚配" << endl;
        if (p->child[0] == NULL) {
            cout << "无子嗣" << endl;
        }
        else {
            cout << "孩子有：";
            for (int i = 0; p->child[i] != NULL; i++) {
                cout << p->child[i]->Name<<" ";
            }
        }

    }
    

}
void show_level(PList FT,int i) {//显示某一代所有人信息
    queue<Person*>Q;
    Q.push(FT);
    while (Q.size() != 0) {
        cout << endl;
        Person* t = Q.front();
        if (t->Familylevel == i) {
            show_single(t);
            cout << endl;
        }
        if (t->Familylevel == i + 1) {
            break;
        }
        for (int i = 0; t->child[i] != NULL && t->child[i]->Name != "0"; i++) {

            Q.push(t->child[i]);
        }
        Q.pop();
    }
}
void Query_Name(PList FT) {
    string name;
    cin >> name;
    queue<Person*>Q;
    Q.push(FT);
    while (Q.size() != 0) {
        cout << endl;
        Person* t = Q.front();
        if (t->Name == name) {
            show_single(t);
            if (t->parent)
                show_single(t->parent);
            else
                cout << "NO parent";
            cout << endl;
            for (int i = 0; t->child[i] != NULL; i++) {
                show_single(t->child[i]);
                cout << " ";
            }
            break;
        }
       
        for (int i = 0; t->child[i] != NULL && t->child[i]->Name != "0"; i++) {

            Q.push(t->child[i]);
        }
        Q.pop();
    }
}
void Query_Birthday(PList FT) {
    Date d;
    cin >> d.y >> d.m >> d.d;
    queue<Person*>Q;
    Q.push(FT);
    while (Q.size() != 0) {
        cout << endl;
        Person* t = Q.front();
        if (t->Birthday->y == d.y && t->Birthday->m == d.m && t->Birthday->d == d.d) {
            show_single(t);
            if (t->parent)
                show_single(t->parent);
            else
                cout << "NO parent";
            cout << endl;
            for (int i = 0; t->child[i] != NULL; i++) {
                show_single(t->child[i]);
                cout << " ";
            }
        }

        for (int i = 0; t->child[i] != NULL && t->child[i]->Name != "0"; i++) {

            Q.push(t->child[i]);
        }
        Q.pop();
    }
}
void Comfirm_Relation(PList FT) {

}
void Add_child(PList FT) {
    string name;
    cin >> name;
    queue<Person*>Q;
    Q.push(FT);
    while (Q.size() != 0) {
        cout << endl;
        Person* t = Q.front();
        if (t->Name==name) {
            int i = 0;
            while(t->child[i] != NULL){
                i++;
            }
            cout << "name:";
            Person* temp = new Person;
            cin >> temp->Name;
            temp->parent = t;
            t->child[i] = temp;
        }

        for (int i = 0; t->child[i] != NULL && t->child[i]->Name != "0"; i++) {

            Q.push(t->child[i]);
        }
        Q.pop();
    }
}
void Change_Information(PList FT) {

}
void Delete(PList& FT) {
    string name;
    cin >> name;
    queue<Person*>Q;
    Q.push(FT);
    while (Q.size() != 0) {
        cout << endl;
        Person* t = Q.front();
        if (t->Name == name) {
            int i = 0;
            while (t->child[i] != NULL) {
                free(t->child[i]);
                t->child[i] = NULL;
                i++;
            }

            Person* temp = t->parent;
            i = 0;
            while (temp->child[i]->Name != name) {
                i++;
            }
            free(temp->child[i]);
            temp->child[i] = NULL;
            while (temp->child[i+1] != NULL) {
                temp->child[i] = temp->child[i + 1];
                i++;
            }
            temp->child[i] = NULL;
            break;
        }

        for (int i = 0; t->child[i] != NULL && t->child[i]->Name != "0"; i++) {

            Q.push(t->child[i]);
        }
        Q.pop();
    }
}
int main()
{
    PList FT=NULL;
    Init(FT);
    
    show_level(FT,2);/*
    Query_Name(FT);
    Query_Birthday(FT);*/
    Delete(FT);
    show_level(FT, 2);
    Save(FT);
}
