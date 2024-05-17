#ifndef AST_H_
#define AST_H_

//Forward declare cpp struct
typedef struct Node Node;

typedef enum NodeType
{
  PROGRAM_NODE,
  EXPR_NODE,
  ADD_NODE,
  SUB_NODE,
  DIV_NODE,
  MUL_NODE,
  LT_NODE,
  LTE_NODE,
  GT_NODE,
  GTE_NODE,
  EQ_NODE,
  NOTEQ_NODE,
  VAR_NODE,
  SET_NODE,
  PRINT_NODE,
  PRINTSTR_NODE,
  STR_NODE,
  FLOAT_NODE,
  ID_NODE,
  IF_NODE,
  IFELSE_NODE,
  IFELIFELSE_NODE,
  IFELIF_NODE,
  WHILE_NODE,
  DOWHILE_NODE,
  STMTS_NODE,
  STMT_NODE,
  MOD_NODE
}NodeType;

#ifdef __cplusplus
extern "C"{
#endif
    Node* createNode(NodeType type,const char* val,Node* l,Node* r);
    void addChild(Node* node,Node* child);
    void prettyPrint(Node*,int);
    void deleteAST(Node*);
#ifdef __cplusplus
}

#include <string>
#include <vector>

struct Node {
  std::string val;
  NodeType type;
  std::vector<Node*> childs;
};

#endif

#endif