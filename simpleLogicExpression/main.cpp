#include <iostream>
#include <fstream>
#include <string>
#include <map>
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
    std::string item;
    int position = 0;

    while (iss >> item) {
        auto it = stringToTokenType.find(item);
        if (it != stringToTokenType.end()) {
            // ��� ��������
            tokens.emplace_back(it->second, item, position);
        }
        else if (!item.empty() && isLatinLetter(item[0])) {
            // ��� ����������, ��������� ��� �������
            for (size_t i = 1; i < item.length(); ++i) {
                if (!isLatinLetterOrDigit(item[i])) {
                    throw Error(Error::invalidVariableChar, position);
                }
            }
            tokens.emplace_back(TokenType::Variable, item, position);
        }
        else {
            // ������������ �����
            if (!item.empty() && !isLatinLetter(item[0])) {
                throw Error(Error::invalidVariableName, position);
            }
            else {
                throw Error(Error::unsupportedOperation, position);
            }
        }
        position += item.length() + 1; // +1 ��� �������
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
