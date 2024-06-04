#include "codegen.h"
#include "ast.h"
#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/CFG.h>
#include <llvm/IR/Constant.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilderFolder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>
#include <map>
#include <string>

using namespace llvm;

static LLVMContext TheContext;
static IRBuilder<> Builder(TheContext);
static std::unique_ptr<Module> TheModule;
static std::map<std::string, AllocaInst *> NamedValues;
static llvm::FunctionType* funcType;
static llvm::Function* mainFunc;
static llvm::BasicBlock* bb; //curr active basic block
Value* fmt;
Value* fmtstr;
int total_bb = 0;

void init_codegen()
{
    // Open a new context and module.
	llvm::FunctionType *printfType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(TheContext), // printf returns int
        llvm::PointerType::get(llvm::Type::getInt8Ty(TheContext), 0), // first argument is char*
        true
    );
    TheModule = std::make_unique<Module>("eazy program", TheContext);
	llvm::Function::Create(printfType,llvm::Function::ExternalLinkage,"printf",*TheModule);
	funcType = llvm::FunctionType::get( llvm::Type::getInt32Ty(TheContext), false );
	mainFunc = llvm::Function::Create( funcType, llvm::Function::ExternalLinkage, "main",*TheModule );
	bb = llvm::BasicBlock::Create(TheContext,std::to_string(++total_bb),mainFunc);
	Builder.SetInsertPoint( bb );
	fmt = Builder.CreateGlobalStringPtr("%f\n","fmt");
	fmtstr = Builder.CreateGlobalStringPtr("%s\n","fmtstr");
	
}
Value* expr_codegen(Node* ast)
{
	if(ast->type == FLOAT_NODE)
	{
		double Val = atof(ast->val.c_str());
  		return ConstantFP::get(TheContext, APFloat(Val));
	}
	else if(ast->type == ID_NODE)
	{
		Value *V = NamedValues[ast->val];
  		if (!V)
		{
    		printf("Unknown variable name %s",ast->val.c_str());
			exit(0);
		}
		return Builder.CreateLoad(llvm::Type::getDoubleTy(TheContext),V);
	}
	else if(ast->type == ADD_NODE)
	{
		Value *L = expr_codegen(ast->childs[0]);
		Value *R = expr_codegen(ast->childs[1]);
  		if (!L || !R)
    		return nullptr;
		return Builder.CreateFAdd(L, R, "addtmp");
	}
	else if(ast->type == SUB_NODE)
	{
		Value *L = expr_codegen(ast->childs[0]);
		Value *R = expr_codegen(ast->childs[1]);
  		if (!L || !R)
    		return nullptr;
		return Builder.CreateFSub(L, R, "subtmp");
	}
	else if(ast->type == DIV_NODE)
	{
		Value *L = expr_codegen(ast->childs[0]);
		Value *R = expr_codegen(ast->childs[1]);
  		if (!L || !R)
    		return nullptr;
		return Builder.CreateFDiv(L, R, "subdiv");
	}
	else if(ast->type == MUL_NODE)
	{
		Value *L = expr_codegen(ast->childs[0]);
		Value *R = expr_codegen(ast->childs[1]);
  		if (!L || !R)
    		return nullptr;
		return Builder.CreateFMul(L, R, "multmp");
	}
	else if(ast->type == AND_NODE)
	{
		Value *L = expr_codegen(ast->childs[0]);
		Value *R = expr_codegen(ast->childs[1]);
  		if (!L || !R)
    		return nullptr;
		return Builder.CreateFMul(L, R, "multmp");
	}
	else if(ast->type == OR_NODE)
	{
		Value *L = expr_codegen(ast->childs[0]);
		Value *R = expr_codegen(ast->childs[1]);
  		if (!L || !R)
    		return nullptr;
		return Builder.CreateFAdd(L, R, "addtmp");
	}
	else if(ast->type == LT_NODE)
	{
		Value *L = expr_codegen(ast->childs[0]);
		Value *R = expr_codegen(ast->childs[1]);
  		if (!L || !R)
    		return nullptr;
		Value* tmp = Builder.CreateFCmpOLT(L,R);
		return Builder.CreateUIToFP(tmp,llvm::Type::getDoubleTy(TheContext));
	}
	else if(ast->type == LTE_NODE)
	{
		Value *L = expr_codegen(ast->childs[0]);
		Value *R = expr_codegen(ast->childs[1]);
  		if (!L || !R)
    		return nullptr;
		Value* tmp = Builder.CreateFCmpOLE(L,R);
		return Builder.CreateUIToFP(tmp,llvm::Type::getDoubleTy(TheContext));
	}
	else if(ast->type == GT_NODE)
	{
		Value *L = expr_codegen(ast->childs[0]);
		Value *R = expr_codegen(ast->childs[1]);
  		if (!L || !R)
    		return nullptr;
		Value* tmp = Builder.CreateFCmpOGT(L,R);
		return Builder.CreateUIToFP(tmp,llvm::Type::getDoubleTy(TheContext));
	}
	else if(ast->type == GTE_NODE)
	{
		Value *L = expr_codegen(ast->childs[0]);
		Value *R = expr_codegen(ast->childs[1]);
  		if (!L || !R)
    		return nullptr;
		Value* tmp = Builder.CreateFCmpOGE(L,R);
		return Builder.CreateUIToFP(tmp,llvm::Type::getDoubleTy(TheContext));
	}
	else if(ast->type == EQ_NODE)
	{
		Value *L = expr_codegen(ast->childs[0]);
		Value *R = expr_codegen(ast->childs[1]);
  		if (!L || !R)
    		return nullptr;
		Value* tmp = Builder.CreateFCmpOEQ(L,R);
		return Builder.CreateUIToFP(tmp,llvm::Type::getDoubleTy(TheContext));
	}
	else if(ast->type == NOTEQ_NODE)
	{
		Value *L = expr_codegen(ast->childs[0]);
		Value *R = expr_codegen(ast->childs[1]);
  		if (!L || !R)
    		return nullptr;
		Value* tmp = Builder.CreateFCmpOEQ(L,R);
		return Builder.CreateUIToFP(tmp,llvm::Type::getDoubleTy(TheContext));
	}
	else if(ast->type == MOD_NODE)
	{
		Value *L = expr_codegen(ast->childs[0]);
		Value *R = expr_codegen(ast->childs[1]);
		if(!L || !R)
			return nullptr;
		L = Builder.CreateFPToUI(L, llvm::Type::getInt64Ty(TheContext));
		R = Builder.CreateFPToUI(R, llvm::Type::getInt64Ty(TheContext));
		//thanks to weird docs of LLVM
		Value* d = Builder.CreateUDiv(L,R);
		Value* e = Builder.CreateMul(d,R);
		Value* f = Builder.CreateSub(L,e);
		return Builder.CreateUIToFP(f,llvm::Type::getDoubleTy(TheContext));
	}
	return nullptr;
}
Value* stmts_codegen(Node* ast)
{
	if(!ast)
    {
		printf("Invalid stmt!\n");
		exit(0);
	}
   	//child 0 is simple statement
   	Node* stmt = ast->childs[0]; //current statement
   	if(stmt->type == VAR_NODE) // create variable
   	{
		std::string& name = stmt->childs[0]->val;
		if(NamedValues.find(name)!= NamedValues.end())
		{
			printf("Error redeclaration of variable %s\n",name.c_str());
			exit(0);
		}
		if(bb->size() > 0)
		{
			auto newbb =  bb->splitBasicBlock(bb->end(),std::to_string(++total_bb));
			Builder.SetInsertPoint(newbb);
			bb = newbb;
		}
		Value* val = expr_codegen(stmt->childs[1]);
		AllocaInst* v = Builder.CreateAlloca(llvm::Type::getDoubleTy(TheContext));
		Builder.CreateStore(val,v);
		NamedValues[stmt->childs[0]->val] = v;
		return val;
   	}
	else if(stmt->type == PRINT_NODE)
	{
		if(bb->size() > 0)
		{
			auto newbb =  bb->splitBasicBlock(bb->end(),std::to_string(++total_bb));
			Builder.SetInsertPoint(newbb);
			bb = newbb;
		}
		Value* val = expr_codegen(stmt->childs[0]);
		return Builder.CreateCall(TheModule->getFunction("printf"),{fmt,val});
	}
	else if(stmt->type == PRINTSTR_NODE)
	{
		if(bb->size() > 0)
		{
			auto newbb =  bb->splitBasicBlock(bb->end(),std::to_string(++total_bb));
			Builder.SetInsertPoint(newbb);
			bb = newbb;
		}
		std::string val = stmt->childs[0]->val;

		return Builder.CreateCall(TheModule->getFunction("printf"),{fmtstr,Builder.CreateGlobalStringPtr(val.substr(1,val.length()-2))});
	}
   	else if(stmt->type == SET_NODE)
   	{
		if(bb->size() > 0)
		{
			auto newbb =  bb->splitBasicBlock(bb->end(),std::to_string(++total_bb));
			Builder.SetInsertPoint(newbb);
			bb = newbb;
		}
		std::string& name = stmt->childs[0]->val;
		Value* expr = expr_codegen(stmt->childs[1]);
		if(NamedValues.find(name)  == NamedValues.end())
		{
			printf("Unknown variable %s\n",name.c_str());
			exit(0);
		}
		Value* v = NamedValues[name];
		return Builder.CreateStore(expr,v);
   	}
	else if(stmt->type == WHILE_NODE)
	{
		auto condbb = bb->splitBasicBlock(bb->end(),std::to_string(++total_bb));	
		auto loopbb = condbb->splitBasicBlock(condbb->end(),std::to_string(++total_bb));
		Builder.SetInsertPoint(loopbb);
		bb = loopbb;
		auto startloopbb = bb;
		block_codegen(stmt->childs[1]);
		loopbb = bb;
		auto afterloop = loopbb->splitBasicBlock(loopbb->end(),std::to_string(++total_bb));
		loopbb->back().eraseFromParent();
		condbb->back().eraseFromParent();
		Builder.SetInsertPoint(condbb);
		Value* expr = expr_codegen(stmt->childs[0]);
		Value* expr_typedcasted = Builder.CreateFPToUI(expr,llvm::Type::getInt1Ty(TheContext));
		BranchInst* b1 = Builder.CreateCondBr(expr_typedcasted,startloopbb,afterloop);
		Builder.SetInsertPoint(loopbb);
		BranchInst* b2 = Builder.CreateBr(condbb);
		Builder.SetInsertPoint(afterloop);
		bb = afterloop;
	}
	else if(stmt->type == DOWHILE_NODE)
	{
		auto condskip = bb->splitBasicBlock(bb->end(),std::to_string(++total_bb));
		auto condbb = condskip->splitBasicBlock(condskip->end(),std::to_string(++total_bb));	
		auto loopbb = condbb->splitBasicBlock(condbb->end(),std::to_string(++total_bb));
		condskip->back().eraseFromParent();
		Builder.SetInsertPoint(condskip);
		BranchInst* skipcond_bi = Builder.CreateBr(loopbb);

		Builder.SetInsertPoint(loopbb);
		bb = loopbb;
		auto startloopbb = bb;
		block_codegen(stmt->childs[1]);
		loopbb = bb;
		auto afterloop = loopbb->splitBasicBlock(loopbb->end(),std::to_string(++total_bb));
		loopbb->back().eraseFromParent();
		condbb->back().eraseFromParent();
		Builder.SetInsertPoint(condbb);
		Value* expr = expr_codegen(stmt->childs[0]);
		Value* expr_typedcasted = Builder.CreateFPToUI(expr,llvm::Type::getInt1Ty(TheContext));
		BranchInst* b1 = Builder.CreateCondBr(expr_typedcasted,startloopbb,afterloop);
		Builder.SetInsertPoint(loopbb);
		BranchInst* b2 = Builder.CreateBr(condbb);
		Builder.SetInsertPoint(afterloop);
		bb = afterloop;
	}
	else if(stmt->type == IF_NODE)
	{
		auto ifcond = bb->splitBasicBlock(bb->end(),std::to_string(++total_bb));
		auto ifbody = ifcond->splitBasicBlock(ifcond->end(),std::to_string(++total_bb));
		auto afterIf = ifbody->splitBasicBlock(ifbody->end(),std::to_string(++total_bb));
		ifcond->back().eraseFromParent();
		bb = ifcond;
		Builder.SetInsertPoint(ifcond);
		Value* expr = expr_codegen(stmt->childs[0]);
		Value* expr_typedcasted = Builder.CreateFPToUI(expr,llvm::Type::getInt1Ty(TheContext));
		BranchInst* bi = Builder.CreateCondBr(expr_typedcasted,ifbody,afterIf);
		Builder.SetInsertPoint(ifbody);
		bb = ifbody;
		ifbody->back().eraseFromParent();
		block_codegen(stmt->childs[1]);
		BranchInst* bi2 = Builder.CreateBr(afterIf);
		bb = afterIf;
		Builder.SetInsertPoint(bb);
	}
	else if(stmt->type == IFELSE_NODE)
	{
		auto ifcond = bb->splitBasicBlock(bb->end(),std::to_string(++total_bb));
		auto ifbody = ifcond->splitBasicBlock(ifcond->end(),std::to_string(++total_bb));
		auto elsebody = ifbody->splitBasicBlock(ifbody->end(),std::to_string(++total_bb));
		auto afterIfElse = elsebody->splitBasicBlock(elsebody->end(),std::to_string(++total_bb));

		ifcond->back().eraseFromParent();
		bb = ifcond;
		Builder.SetInsertPoint(ifcond);
		Value* expr = expr_codegen(stmt->childs[0]);
		Value* expr_typedcasted = Builder.CreateFPToUI(expr,llvm::Type::getInt1Ty(TheContext));
		BranchInst* bi = Builder.CreateCondBr(expr_typedcasted,ifbody,elsebody);
		Builder.SetInsertPoint(ifbody);
		bb = ifbody;
		ifbody->back().eraseFromParent();
		block_codegen(stmt->childs[1]);
		BranchInst* bi2 = Builder.CreateBr(afterIfElse);
		
		bb = elsebody;
		Builder.SetInsertPoint(elsebody);
		elsebody->back().eraseFromParent();
		block_codegen(stmt->childs[2]);
		BranchInst* bi3 = Builder.CreateBr(afterIfElse);

		bb = afterIfElse;
		Builder.SetInsertPoint(bb);
	}
	else if(stmt->type == IFELIFELSE_NODE)
	{
		auto ifcond = bb->splitBasicBlock(bb->end(),std::to_string(++total_bb));
		auto ifbody = ifcond->splitBasicBlock(ifcond->end(),std::to_string(++total_bb));
		auto elifcond = ifbody->splitBasicBlock(ifbody->end(),std::to_string(++total_bb));
		auto elifbody = elifcond->splitBasicBlock(elifcond->end(),std::to_string(++total_bb));
		auto elsebody = elifbody->splitBasicBlock(elifbody->end(),std::to_string(++total_bb));
		auto afterIfElifElse = elsebody->splitBasicBlock(elsebody->end(),std::to_string(++total_bb));

		ifcond->back().eraseFromParent();
		bb = ifcond;
		Builder.SetInsertPoint(ifcond);
		Value* expr = expr_codegen(stmt->childs[0]);
		Value* expr_typedcasted = Builder.CreateFPToUI(expr,llvm::Type::getInt1Ty(TheContext));
		BranchInst* bi = Builder.CreateCondBr(expr_typedcasted,ifbody,elifcond);
		Builder.SetInsertPoint(ifbody);
		bb = ifbody;
		ifbody->back().eraseFromParent();
		block_codegen(stmt->childs[1]);
		BranchInst* bi2 = Builder.CreateBr(afterIfElifElse);
		////
		elifcond->back().eraseFromParent();
		bb = elifcond;
		Builder.SetInsertPoint(elifcond);
		Value* elifexpr = expr_codegen(stmt->childs[2]);
		Value* elifexpr_typedcasted = Builder.CreateFPToUI(elifexpr,llvm::Type::getInt1Ty(TheContext));
		BranchInst* elifbi = Builder.CreateCondBr(elifexpr_typedcasted,elifbody,elsebody);
		Builder.SetInsertPoint(elifbody);
		bb = elifbody;
		elifbody->back().eraseFromParent();
		block_codegen(stmt->childs[3]);
		BranchInst* elif_bi = Builder.CreateBr(afterIfElifElse);
		////
		bb = elsebody;
		Builder.SetInsertPoint(elsebody);
		elsebody->back().eraseFromParent();
		block_codegen(stmt->childs[4]);
		BranchInst* bi3 = Builder.CreateBr(afterIfElifElse);

		bb = afterIfElifElse;
		Builder.SetInsertPoint(bb);
	}
	else if(stmt->type == IFELIF_NODE)
	{
		auto ifcond = bb->splitBasicBlock(bb->end(),std::to_string(++total_bb));
		auto ifbody = ifcond->splitBasicBlock(ifcond->end(),std::to_string(++total_bb));
		auto elifcond = ifbody->splitBasicBlock(ifbody->end(),std::to_string(++total_bb));
		auto elifbody = elifcond->splitBasicBlock(elifcond->end(),std::to_string(++total_bb));
		auto afterIfElif = elifbody->splitBasicBlock(elifbody->end(),std::to_string(++total_bb));

		ifcond->back().eraseFromParent();
		bb = ifcond;
		Builder.SetInsertPoint(ifcond);
		Value* expr = expr_codegen(stmt->childs[0]);
		Value* expr_typedcasted = Builder.CreateFPToUI(expr,llvm::Type::getInt1Ty(TheContext));
		BranchInst* bi = Builder.CreateCondBr(expr_typedcasted,ifbody,elifcond);
		Builder.SetInsertPoint(ifbody);
		bb = ifbody;
		ifbody->back().eraseFromParent();
		block_codegen(stmt->childs[1]);
		BranchInst* bi2 = Builder.CreateBr(afterIfElif);
		////
		elifcond->back().eraseFromParent();
		bb = elifcond;
		Builder.SetInsertPoint(elifcond);
		Value* elifexpr = expr_codegen(stmt->childs[2]);
		Value* elifexpr_typedcasted = Builder.CreateFPToUI(elifexpr,llvm::Type::getInt1Ty(TheContext));
		BranchInst* elifbi = Builder.CreateCondBr(elifexpr_typedcasted,elifbody,afterIfElif);
		Builder.SetInsertPoint(elifbody);
		bb = elifbody;
		elifbody->back().eraseFromParent();
		block_codegen(stmt->childs[3]);
		BranchInst* elif_bi = Builder.CreateBr(afterIfElif);
		////


		bb = afterIfElif;
		Builder.SetInsertPoint(bb);
	}
   	else 
   	{
    	fprintf(stderr,"Code generation not implemented for this statement %s.",stmt->val.c_str());
      	exit(0);
   	}
	return nullptr;
}
void block_codegen(Node* ast)
{
	if(!ast)
		return;
	Node* stmt = ast->childs[0];
	stmts_codegen(stmt);
	if(ast->childs.size() == 2)
	  block_codegen(ast->childs[1]);
	return;
}
void program_codegen(Node* ast)
{
    block_codegen(ast->childs[0]);
	Value* v = llvm::ConstantInt::get(llvm::Type::getInt32Ty(TheContext), 0);
	Builder.CreateRet(v);
	std::error_code EC;
	llvm::raw_fd_ostream OS("tmp.ll", EC);
	TheModule->print( OS, nullptr );
}