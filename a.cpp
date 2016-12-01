#include "a.h"
Document gen() {
    map<string,int> mp;
    for ( int i = 0 ; i < rand()%10+1 ; i++ ) {
        string t = "";
        t += 'A'+rand()%20;
        mp[t] = rand()%10+1;
    }
    Document ret(Point(rand()%10000,rand()%10000),mp);
    return ret;
}
void test() {
    srand((unsigned)time(NULL));
    IRTree* tree = new IRTree;
    for ( int i = 0 ; i < 12000 ; i++ ) {
        printf("%d add\n",i);
        tree->insert(gen());
    }
    tree->bfs();
}
/*
void test1() {
    vector<IRTree*> v;
    v.push_back(new IRTree);

    v[0]->insert(document);
}
*/
void test2(char* filename) {
    FILE* fp = fopen(filename,"r");
    IRTree* tree = new IRTree;
    double t1,t2;
    char buf1[128],buf2[128],buf3[128];
    int id = 0;
    while ( ~fscanf(fp,"%lf %lf %s %s %s",&t1,&t2,buf1,buf2,buf3) ) {
        printf("%d end\n",id++);
        map<string,int> mp;
        if ( mp.count(string(buf1)) == 0 )
            mp[buf1] = 1;
        else mp[buf1] ++;
        if ( mp.count(string(buf2)) == 0 )
            mp[buf2] = 1;
        else mp[buf2] ++;
        if ( mp.count(string(buf3)) == 0 )
            mp[buf3] = 1;
        else mp[buf3] ++;
        Document d(Point(t1,t2),mp);
        tree->insert(d);
    }
    tree->bfs();
    fclose(fp);
}

int main() {
//    test();
    test2("random.txt");
    return 0;
}
