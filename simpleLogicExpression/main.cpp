#include <iostream>
#include <string>
#include "objects.h"
#include "functions.h"

/**
 * ������� ������� ���������.
 *
 * ������������ ��������� ��������� ������, ��������� ������ �������� �����,
 * �������������� ��������� � ������ ���������� � �������� ����.
 * @param argc ���������� ����������
 * @param argv ������ ���������� (argv[1] - ������� ����, argv[2] - �������� ����)
 * @return 0 ��� �������� ����������
 */
int main() {

	return 0;
}


std::string readFile(const std::string& filePath) {
	std::string content = "";

	return content;
}

void writeFile(const std::string& filePath, const std::string& content) {

}

std::vector<Token> tokenize(const std::string& expression, std::set<Error>& errorList) {
	std::vector<Token> tokens;

	return tokens;
}

ExpressionNode* buildExpressionTree(const std::vector<Token>& tokens, std::set<Error>& errorList) {
	ExpressionNode* node = nullptr;

	return node;
}

ExpressionNode* copyNode(ExpressionNode* node) {
	ExpressionNode* newNode = nullptr;

	return newNode;
}

void transformImplicationAndEquivalence(ExpressionNode* node) {

}

bool simplifyExpression(ExpressionNode* node) {
	return false;
}

void removeDoubleNot(ExpressionNode* node) {

}

std::string expressionTreeToInfix(ExpressionNode* node) {
	std::string result = "";

	return result;
}
