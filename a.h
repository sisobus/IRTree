#include <cstdio>
#include <list>
#include <cstring>
#include <string>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <map>
using namespace std;
#define MIN_SIZE_OF_MBR 4
#define MAX_SIZE_OF_MBR 8
#define inf 2e+9

struct Keyword {
    string name;
    double frequency;
    Keyword() {}
    Keyword(string _name,double _frequency):
        name(_name), frequency(_frequency){}
};
struct Point {
    double x,y;
    Point() {}
    Point(double _x,double _y):
        x(_x), y(_y){}
};
struct Rectangle {
    Point left;
    Point right;
    Rectangle() {}
    Rectangle(Point _left, Point _right):
        left(_left), right(_right) {}
    
    double area(Rectangle r) {
        return (r.right.x - r.left.x)*(r.right.y - r.left.y);
    }
    double area() {
        return (right.x - left.x)*(right.y - left.y);
    }
    bool isInside(Point p) {
        return left.x <= p.x && p.x <= right.x && left.y <= p.y && p.y <= right.y;
    }
    void update(Rectangle r) {
        right.x = max(right.x,r.right.x);
        right.y = max(right.y,r.right.y);
        left.x = min(left.x,r.left.x);
        left.y = min(left.y,r.left.y);
    }
    void update(Point p) {
        double a = area();
        if ( p.x > right.x ) {
            if ( p.y > right.y ) {
                right.x = p.x;
                right.y = p.y;
            } else if ( p.y < left.y ) {
                left.y = p.y;
                right.x = p.x;
            } else {
                right.x = p.x;
            }
        } else if ( p.x < left.x ) {
            if ( p.y > right.y ) {
                left.x = p.x;
                right.y = p.y;
            } else if ( p.y < left.y ) {
                left.x = p.x;
                left.y = p.y;
            } else {
                left.x = p.x;
            }
        } else {
            if ( p.y > right.y ) {
                right.y = p.y;
            } else if ( p.y < left.y ) {
                left.y = p.y;
            } else {
            }
        }
    }
    double getAmount(Point p) {
        double a = area();
        double b;
        if ( p.x > right.x ) {
            if ( p.y > right.y ) {
                b = area(Rectangle(left,p));
            } else if ( p.y < left.y ) {
                b = area(Rectangle(Point(left.x,p.y),Point(p.x,right.y)));
            } else {
                b = area(Rectangle(left,Point(p.x,right.y)));
            }
        } else if ( p.x < left.x ) {
            if ( p.y > right.y ) {
                b = area(Rectangle(Point(p.x,left.y),Point(right.x,p.y)));
            } else if ( p.y < left.y ) {
                b = area(Rectangle(p,right));
            } else {
                b = area(Rectangle(Point(p.x,left.y),right));
            }
        } else {
            if ( p.y > right.y ) {
                b = area(Rectangle(left,Point(right.x,p.y)));
            } else if ( p.y < left.y ) {
                b = area(Rectangle(Point(left.x,p.y),right));
            } else {
                b = a;
            }
        }
        return b-a;
    }
};

double area(Rectangle r) {
    return (r.right.x - r.left.x)*(r.right.y - r.left.y);
}
/*
area(r);
area(Rectangle(p1,p2));
area(Rectangle(Point(1.1,2.2),Point(2.3,4)));
*/

struct Document {
    Point location;
    map<string,int> keywords;
    Document(){}
    Document(Point _location):
        location (_location) {}
    Document(Point _location,map<string,int>& _keywords):
        location (_location), keywords(_keywords) {}
    void print() {
        printf("(%lf,%lf) ",location.x,location.y);
        for ( map<string,int>::iterator it=keywords.begin();\
                it!=keywords.end();it++ ) 
            printf("[%s:%d] ",it->first.c_str(),it->second);
        puts("");
    }
};
struct Node {
    Node* parent;
    Rectangle mbr;
    list<Node*> child;
    vector<Document> document;
    map<string,int> keywords;
/* constructor */
    Node(): parent(NULL){}
    virtual ~Node(){
//        parent = NULL;
        while ( !child.empty() ) 
            child.pop_back();
    }

    
    void print() {
        printf("[%d,%d] [(%lf,%lf),(%lf,%lf)] ",(int)child.size(),(int)document.size(),mbr.left.x,mbr.left.y,mbr.right.x,mbr.right.y);
        for ( map<string,int>::iterator it=keywords.begin();\
                it!=keywords.end();it++ ) 
            printf("[%s:%d] ",it->first.c_str(),it->second);
        puts("");
    }
    bool isRoot() {
        /*
        if ( parent == NULL ) return true;
        if ( parent->child.empty() && parent->document.empty() ) return true;
        */
        return parent == NULL || (parent->child.empty() && parent->document.empty());
//        return parent == NULL;
    }
    bool isLeaf() {
        return !document.empty();
    }
    int getNumberOfKeywords() {
        return (int)keywords.size();
    }
    int getAllNumberOfKeywords() {
        int ret = 0;
        for ( map<string,int>::iterator it=keywords.begin();it!=keywords.end();it++ )
            ret += it->second;
        return ret;
    }
    void addChild(Node *_child) {
        if ( this->child.empty() ) {
            this->child.push_back(_child);
            _child->parent = this;
            this->keywords = _child->keywords;
            this->mbr = _child->mbr;
        } else {
            this->child.push_back(_child);
            _child->parent = this;
            for ( map<string,int>::iterator it=_child->keywords.begin();\
                    it!=_child->keywords.end();it++ ) {
                if ( this->keywords.count(it->first) == 0 ) {
                    this->keywords[it->first] = it->second;
                } else {
                    this->keywords[it->first] += it->second;
                }
            }
            this->mbr.update(_child->mbr);
        }
    }
    void addDocument(Document d) {
        if ( this->document.empty() ) {
            this->document.push_back(d);
            this->keywords = d.keywords;
            this->mbr = Rectangle(d.location,d.location);
        } else {
            this->document.push_back(d);
            for ( map<string,int>::iterator it=d.keywords.begin();\
                    it!=d.keywords.end();it++ ) {
                if ( this->keywords.count(it->first) == 0 ) {
                    this->keywords[it->first] = it->second;
                } else {
                    this->keywords[it->first] += it->second;
                }
            }
            this->mbr.update(d.location);
        }
    }
    Node* selectSeed(Node* seed1,Node* seed2,Rectangle r) {
        Point right, left;
        right.x = max(seed1->mbr.right.x,r.right.x);
        right.y = max(seed1->mbr.right.y,r.right.y);
        left.x = min(seed1->mbr.left.x,r.left.x);
        left.y = min(seed1->mbr.left.y,r.left.y);
        double area1 = area(Rectangle(left,right)) - area(seed1->mbr);
        right.x = max(seed2->mbr.right.x,r.right.x);
        right.y = max(seed2->mbr.right.y,r.right.y);
        left.x = min(seed2->mbr.left.x,r.left.x);
        left.y = min(seed2->mbr.left.y,r.left.y);
        double area2 = area(Rectangle(left,right)) - area(seed2->mbr);

        if ( (area1 > area2) && MAX_SIZE_OF_MBR-MIN_SIZE_OF_MBR+1>(int)seed1->child.size() ){
            return seed1;
        } else if ( MAX_SIZE_OF_MBR-MIN_SIZE_OF_MBR+1>(int)seed2->child.size() ) {
            return seed2;
        }
        return seed1;
    }
    Node* split() {
        Node* newNode1 = new Node;
        Node* newNode2 = new Node;
        Node* _parent = this->parent;
        if ( _parent->child.empty() && _parent->document.empty() ) {
        } else {
            /*
            int indexForErase = -1;
            for ( int i = 0 ; i < (int)parent->child.size() ; i++ ) 
                if ( parent->child[i] == this ) {
                    indexForErase = i;
                    break;
                }
            for ( map<string,int>::iterator it=this->keywords.begin();\
                    it!=this->keywords.end();it++ ) 
                parent->keywords[it->first] -= it->second;
            parent->child.erase(parent->child.begin()+indexForErase);
            */
            list<Node*>::iterator it;
            for ( it=_parent->child.begin();it!=_parent->child.end();it++ ) 
                if ( *it == this ) break;
            for ( map<string,int>::iterator it2=this->keywords.begin();\
                    it2!=this->keywords.end();it2++ )
                _parent->keywords[it2->first] -= it2->second;
            _parent->child.erase(it);
        }
        newNode1->parent = newNode2->parent = _parent;
        if ( isLeaf() ) {
            pair<double,Document> mxx(this->document[0].location.x,this->document[0]);
            pair<double,Document> mnx(this->document[0].location.x,this->document[0]);
            pair<double,Document> mxy(this->document[0].location.y,this->document[0]);
            pair<double,Document> mny(this->document[0].location.y,this->document[0]);
            int mxxi, mnxi, mxyi, mnyi;
            mxxi = mnxi = mxyi = mnyi = 0;
            for ( int i = 1 ; i < (int)this->document.size() ; i++ ) {
                Document cur = this->document[i];
                if ( mxx.first < cur.location.x ) {
                    mxx = pair<double,Document>(cur.location.x,cur);
                    mxxi = i;
                }
                if ( mnx.first > cur.location.x ) {
                    mnx = pair<double,Document>(cur.location.x,cur);
                    mnxi = i;
                }
                if ( mxy.first < cur.location.y ) {
                    mxy = pair<double,Document>(cur.location.y,cur);
                    mxyi = i;
                }
                if ( mny.first > cur.location.y ) { 
                    mny = pair<double,Document>(cur.location.y,cur);
                    mnyi = i;
                }
            }
            double a = mxx.first - mnx.first;
            double b = mxy.first - mny.first;
            Document seed1, seed2;
            int seed1i, seed2i;
            if ( a > b ) {
                seed1 = mxx.second;
                seed1i = mxxi;
                seed2 = mnx.second;
                seed2i = mnxi;
            } else {
                seed1 = mxy.second;
                seed2 = mny.second;
                seed1i = mxyi;
                seed2i = mnyi;
            }
            newNode1->addDocument(seed1);
            newNode2->addDocument(seed2);
            for ( int i = 0 ; i < (int)this->document.size() ; i++ ) {
                if ( seed1i == i || seed2i == i ) continue;
                Document curDocument = this->document[i];
                Node* resultSeed = selectSeed(newNode1,newNode2,\
                        Rectangle(curDocument.location,curDocument.location));
                resultSeed->addDocument(curDocument);
            }
        } else {
            Node* firstChild = *this->child.begin();
            pair<double,Node*> mxx(firstChild->mbr.left.x,firstChild);
            pair<double,Node*> mnx(firstChild->mbr.right.x,firstChild);
            pair<double,Node*> mxy(firstChild->mbr.left.y,firstChild);
            pair<double,Node*> mny(firstChild->mbr.right.y,firstChild);
            int mxxi, mnxi, mxyi, mnyi;
            mxxi = mnxi = mxyi = mnyi = 0;
            for ( list<Node*>::iterator it=this->child.begin();\
                    it!=this->child.end();it++ ){
//            for ( int i = 1 ; i < (int)this->child.size() ; i++ ) {
//                Node* curChild = this->child[i];
                Node* curChild = *it;
                if ( mxx.first < curChild->mbr.left.x ) {
                    mxx = pair<double,Node*>(curChild->mbr.left.x,curChild);
                }
                if ( mnx.first > curChild->mbr.right.x ) {
                    mnx = pair<double,Node*>(curChild->mbr.right.x,curChild);
                }
                if ( mxy.first < curChild->mbr.left.y ) {
                    mxy = pair<double,Node*>(curChild->mbr.left.y,curChild);
                }
                if ( mny.first > curChild->mbr.right.y ) { 
                    mny = pair<double,Node*>(curChild->mbr.right.y,curChild);
                }
            }
            double a = mxx.first - mnx.first;
            double b = mxy.first - mny.first;
            Node* seed1, *seed2;
            int seed1i, seed2i;
            if ( a > b ) {
                seed1 = mxx.second;
                seed2 = mnx.second;
            } else {
                seed1 = mxy.second;
                seed2 = mny.second;
            }
            newNode1->addChild(seed1);
            seed1->parent = newNode1;
            newNode2->addChild(seed2);
            seed2->parent = newNode2;
            for ( list<Node*>::iterator it=this->child.begin();\
                    it!=this->child.end();it++ ) {
                if ( seed1 == (*it) || seed2 == (*it) ) continue;
                Node *curChild = (*it);
                Node* resultSeed = selectSeed(newNode1,newNode2,curChild->mbr);
                resultSeed->addChild(curChild);
                curChild->parent = resultSeed;
            }
            /*
            for ( int i = 0 ; i < (int)this->child.size() ; i++ ) {
                if ( seed1i == i || seed2i == i ) continue;
                Node *curChild = this->child[i];
                Node* resultSeed = selectSeed(newNode1,newNode2,curChild->mbr);
                resultSeed->addChild(curChild);
                curChild->parent = resultSeed;
            }
            */
        }
        _parent->addChild(newNode1);
        _parent->addChild(newNode2);
        return _parent;
    }
    Node* findChildForInsert(Document d) {
        pair<double,Node*> mn(inf,*this->child.begin());
        for ( list<Node*>::iterator it=this->child.begin();\
                it!=this->child.end();it++ ) {
            Node* curChild = (*it);
            if ( curChild->mbr.isInside(d.location) ) 
                return curChild;
            double curAmount = curChild->mbr.getAmount(d.location);
            if ( mn.first > curAmount ) 
                mn = pair<double,Node*>(curAmount,curChild);
        }
        /*
        pair<double,Node*> mn(inf,this->child[0]);
        for ( int i = 0 ; i < (int)this->child.size() ; i++ ) {
            Node* curChild = this->child[i];
            if ( curChild->mbr.isInside(d.location) ) 
                return curChild;
            double curAmount = curChild->mbr.getAmount(d.location);
            if ( mn.first > curAmount ) 
                mn = pair<double,Node*>(curAmount,curChild);
        }
        */
        return mn.second;
    }
};
struct IRTree {
    Node* root;

    IRTree(): root(NULL){}
    void update(Node *now) {
        if ( now->isLeaf() ) return;
        for ( list<Node*>::iterator it=now->child.begin();it!=now->child.end();it++ ) {
            if ( (*it)->parent != now ) (*it)->parent = now;
            update(*it);
        }
    }
    bool checkLeafOk(Node *now) {
        if ( now->isLeaf() ) {
            return (int)now->document.size() <= MAX_SIZE_OF_MBR;
        }
        bool ret = true;
        for ( list<Node*>::iterator it=now->child.begin();it!=now->child.end();it++ ) {
            if ( (*it)->parent != now ) return false;
            ret &= checkLeafOk(*it);
        }
        return ret;
    }
    void insert(Document d) {
        if ( root == NULL ) {
            Node* newNode = new Node;
            newNode->parent = NULL;
            newNode->mbr = Rectangle(Point(d.location),Point(d.location));
            newNode->addDocument(d);
            root = newNode;
        } else if ( root->isLeaf() ) {
            root->addDocument(d);
            root->mbr.update(d.location);
            if ( (int)root->document.size() > MAX_SIZE_OF_MBR ) {
                Node* newNode = new Node;
                root->parent = newNode;
                Node* resultParent = root->split();
                root = resultParent;
            }
        } else {
            Node* np = root;
            while ( !np->isLeaf() ) {
                np = np->findChildForInsert(d);
            }
            np->addDocument(d);
            if ( (int)np->document.size() > MAX_SIZE_OF_MBR ) {
                while ( !np->isRoot() ) {
                    /*
                    if ( np->isLeaf() && (int)np->document.size() <= MAX_SIZE_OF_MBR )
                        break;
                        */
                    if ( !np->isLeaf() && (int)np->child.size() <= MAX_SIZE_OF_MBR ) 
                        break;
                    Node* resultParent = np->split();
                    np->parent = resultParent;
                    np = resultParent;
                }
                if ( np->isRoot() && np->child.size() > MAX_SIZE_OF_MBR ) {
                    Node* newNode = new Node;
                    root->parent = newNode;
                    Node* resultParent = root->split();
                    root = resultParent;
                }
            }
        }
        update(root);
    }
    void _dfs(Node* now) {
        now->print();
        if ( now->isLeaf() ) {
            for ( int i = 0 ; i < (int)now->document.size() ; i++ ) 
                now->document[i].print();
        } else {
            for ( list<Node*>::iterator it=now->child.begin();\
                    it!=now->child.end();it++ ) 
                _dfs(*it);
            /*
            for ( int i = 0 ; i < (int)now->child.size() ; i++ ) 
                _dfs(now->child[i]);
                */
        }
    }
    void dfs() {
        _dfs(root);
    }
    void bfs() {
        queue<Node*> q;
        q.push(root);
        while ( !q.empty() ) {
            Node* now = q.front();q.pop();
            now->print();
            if ( now->isLeaf() ) {
                for ( int i = 0 ; i < (int)now->document.size() ; i++ ) 
                    now->document[i].print();
            } else {
                for ( list<Node*>::iterator it=now->child.begin();\
                        it!=now->child.end();it++ ) 
                    q.push(*it);
                /*
                for ( int i = 0 ; i < (int)now->child.size() ; i++ ) 
                    q.push(now->child[i]);
                    */
            }
        }
    }
};
