#include "ast.h"
#include <string>
#include <vector>

using namespace std;

struct Node
{
  string val;
  NodeType type;
  vector<Node*> childs;
};

Node* createNode(NodeType type,const char* val,Node* l,Node* r)
{
    Node* n = new Node;
    n->val = val;
    if(l)
        n->childs.push_back(l);
    if(r)
        n->childs.push_back(r);
    return n;
}
int spaces = 0;
void prettyPrint(Node * n)
{
    if(!n)
      return;
    for(int i=1;i<=spaces;i++)
      fputc(' ',stdout);
    fputc('|',stdout);
    puts(n->val.c_str());
    spaces += 2;
    for(auto child: n->childs)
    {
        prettyPrint(child);
    }
    spaces-=2;
}