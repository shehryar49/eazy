#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/raw_ostream.h"

#include "ast.h"

#include <map>
#include <iostream>

extern "C" int yylex();
extern "C" int yyparse();

Node *ast;

//initialization of llvm stuff
llvm::LLVMContext context;
llvm::Module *module = new llvm::Module( "my module", context );
llvm::IRBuilder<> builder( context );
std::map<std::string, llvm::Value*> NamedValues;

llvm::Value* logErrorV( const char* str ) {
    fprintf( stderr, "Error: %s\n", str );
    return nullptr;
}

llvm::Value *generateIRC( Node *node ) {
	if( !node ) {
		return nullptr;
	}

	std::cout << node->type << "\t/\t" << node->val << std::endl;

	switch( node->type ) {
	case PROGRAM_NODE: {
		return generateIRC( node->childs[0] );
	}
	case STMTS_NODE: {
		for( Node *child : node->childs ) {
			generateIRC( child );
		}
		return nullptr;
	}
	case STMT_NODE: {
		return generateIRC( node->childs[0] );
	}
	case VAR_NODE: {
		llvm::Value *right = generateIRC( node->childs[1] );
		NamedValues[node->childs[0]->val] = right;
		return right;
	}
	case ADD_NODE: {
		llvm::Value *left = generateIRC( node->childs[0] );
		llvm::Value *right = generateIRC( node->childs[1] );
		return builder.CreateFAdd( left, right, "addtmp" );
	}
	case SUB_NODE: {
		llvm::Value *left = generateIRC( node->childs[0] );
		llvm::Value *right = generateIRC( node->childs[1] );
		return builder.CreateFSub( left, right, "subtmp" );
	}
	case MUL_NODE: {
		llvm::Value *left = generateIRC( node->childs[0] );
		llvm::Value *right = generateIRC( node->childs[1] );
		return builder.CreateFMul( left, right, "multmp" );
	}
	case DIV_NODE: {
		llvm::Value *left = generateIRC( node->childs[0] );
		llvm::Value *right = generateIRC( node->childs[1] );
		return builder.CreateFDiv( left, right, "divtmp" );
	}
	case FLOAT_NODE: {
		return llvm::ConstantFP::get( context, llvm::APFloat( strtod( node->val.c_str(), nullptr ) ) );
	}
	case ID_NODE: {
		llvm::Value *V = NamedValues[node->val];
		if (!V) return logErrorV("Unknown variable name");
		return V;
	}
	case SET_NODE: {
		std::string varName = node->childs[0]->val;
		llvm::Value *V = generateIRC( node->childs[1] );
		//if (!V) return nullptr;
		NamedValues[varName] = V;
		return V;
	}
	case PRINT_NODE: {
		llvm::Value *value = generateIRC( node->childs[0] );
		std::cout << "here\n";
      	builder.CreateCall( module->getFunction("printf"), {
        	builder.CreateGlobalStringPtr( "%f\n", "fmt" ),
        	value
      	}, "printf" );
		std::cout << "made call\n";
      	return nullptr;
	}
	default:
		return logErrorV("Invalid node type");
	}
}

int main() {
	yyparse();

	prettyPrint( ast, 0 );

	llvm::FunctionType *printfType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(context), // printf returns int
        llvm::PointerType::get(llvm::Type::getInt8Ty(context), 0), // first argument is char*
        true
    );
    llvm::Function::Create( printfType, llvm::Function::ExternalLinkage, "printf", module );

	llvm::FunctionType *funcType = llvm::FunctionType::get( llvm::Type::getFloatTy( context ), false );
	llvm::Function *mainFunc = llvm::Function::Create( funcType, llvm::Function::ExternalLinkage, "main", module );

	llvm::BasicBlock *basicBlock = llvm::BasicBlock::Create( context, "first block", mainFunc );
	builder.SetInsertPoint( basicBlock );

	llvm::Value *irc = generateIRC( ast );

	module->print( llvm::outs(), nullptr );

	deleteAST( ast );
	return 0;
}