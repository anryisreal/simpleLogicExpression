#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <algorithm>
#include "objects.h"
#include "functions.h"

/**
* ������������ ����� ������� � ����� ��������
*/
const std::map<std::string, TokenType> stringToTokenType = {
    {"!", TokenType::Not},
    {"&", TokenType::And},
    {"|", TokenType::Or},
    {">", TokenType::Implication},
    {"~", TokenType::Equivalence}
};

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

/**
* ������ ��� ������ �� �����
*/
std::string readFile(const std::string& filePath) {

    std::ifstream file(filePath);	// ������� ����� ��� ������ �� �����

	// ������ ����������, ���� ���� �� ��������
    if (!file.is_open()) {
        throw Error(Error::inputFile);
    }
    
    std::string content;			// ����������� ������ �� �����
    std::getline(file, content);	// ���������� ������
    file.close();					// �������� �����

	return content;
}

/**
* ������� ��� ������ � ����
*/
void writeFile(const std::string& filePath, const std::string& content) {

    std::ofstream file(filePath);  // ������� ����� ��� ������ � ����

    // ������ ����������, ���� ���� �� ��������
    if (!file.is_open()) {
        throw Error(Error::outputFile);
    }

    file << content;               // ������ � ����
    file.close();                  // �������� �����
}

std::vector<Token> tokenize(const std::string& expression, std::set<Error>& errorList) {
    std::vector<Token> tokens;
    std::istringstream iss(expression);
    std::string tokenStr;
    int position = 0;

    while (iss >> tokenStr) {
        position++;

        // �������� �� ��������
        auto it = stringToTokenType.find(tokenStr);
        if (it != stringToTokenType.end()) {
            tokens.emplace_back(it->second, tokenStr, position);
            continue;
        }

        // �������� �� ����������
        if (isalpha(tokenStr[0])) {
            bool valid = all_of(tokenStr.begin(), tokenStr.end(), [](char c) {
                return isalnum(c);
                });

            if (!valid) {
                errorList.insert(Error(Error::ErrorType::invalidVariableChar, position));
                continue;
            }

            tokens.emplace_back(TokenType::Variable, tokenStr, position);
            continue;
        }

        // ���� ����� �� ���������
        errorList.insert(Error(Error::ErrorType::invalidVariableName, position));
    }

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
