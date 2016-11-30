#include "a.h"
Document gen() {
    map<string,int> mp;
    for ( int i = 0 ; i < rand()%10+1 ; i++ ) {
        string t = "";
        t += 'A'+rand()%20;
        mp[t] = rand()%10+1;
    }
    Document ret(Point(rand()%100,rand()%100),mp);
    return ret;
}
void test() {
    srand((unsigned)time(NULL));
    IRTree* tree = new IRTree;
    for ( int i = 0 ; i < 120 ; i++ ) 
        tree->insert(gen());
    tree->bfs();
}

int main() {
    test();
    return 0;
}
