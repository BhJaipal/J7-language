#pragma once
#include "Lexer.hpp"
#include <iostream>
#include <vector>

namespace J7 {

enum class NodeType {
	// Statements
	Program,
	IfStatement,
	WhileStatement,
	ForStatement,
	VariableDeclaration,
	ImportStatement,
	FuncDeclaration,
	// Expressions
	AssignmentExpr,
	UrinaryExpr,
	BinaryExpr,
	StringLiteral,
	ArrayLiteral,
	IntLiteral,
	FloatLiteral,
	Identifier,
	NullLiteral,
	FuncCall,
	UndefinedLiteral,
};
enum class ValueType {
	Integer,
	Floating,
	Boolean,
	Null,
	Undefined,
	String,
	FuncValue
};
struct Statement {
	NodeType node_type;
	virtual ~Statement() {}
};
struct Program : public Statement {
	std::vector<Statement *> statements;
	Program();
};

struct Expression : public Statement {};

struct VariableDeclaration : public Statement {
	std::string VariableName;
	bool isConst;
	ValueType valueType;
	Expression *value;
	VariableDeclaration();
};

struct FuncDeclaration : public Statement {
	std::string FuncName;
	std::vector<std::string> argumentNames;
	std::vector<Statement *> body;
	FuncDeclaration();
};
struct ImportStatement : public Statement {
	std::string path;
	std::string name;
	ImportStatement();
};

struct IntLiteral : public Expression {
	int vale;
	IntLiteral();
};

struct FloatLiteral : public Expression {
	double value;
	FloatLiteral();
};

struct NullLiteral : public Expression {
	NullLiteral();
};

struct UndefinedLiteral : public Expression {
	UndefinedLiteral();
};

struct IfStatement : public Statement {
	Expression *condition;
	std::vector<Statement *> ifBody;
	std::vector<Statement *> elifBody;
	std::vector<Statement *> elseBody;
	IfStatement();
};

struct WhileStatement : public Statement {
	Expression *condition;
	std::vector<Statement *> body;
	WhileStatement();
};
struct ForStatement : public Statement {
	Expression *init;
	Expression *condition;
	Expression *step;
	std::vector<Statement *> body;
	ForStatement();
};

struct StringLiteral : public Expression {
	std::string value;
	StringLiteral();
};

struct ArrayLiteral : public Expression {
	std::vector<Expression *> elements;
	ArrayLiteral();
};

struct Identifier : public Expression {
	std::string name;
	Identifier();
};

struct AssignmentExpr : public Expression {
	Expression *assigne;
	Expression *value;
	AssignmentExpr();
};
struct FuncCall : public Expression {
	std::string caller;
	std::vector<Expression *> arguments;
	FuncCall();
};

std::ostream &operator<<(std::ostream &out, Statement *statement);
} // namespace J7
