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
            bool valid = std::all_of(tokenStr.begin(), tokenStr.end(), [](char c) {
                return isalnum(c);
                });

            if (!valid) {
                errorList.insert(Error(Error::ErrorType::invalidVariableChar, position));
                continue;
            }

            tokens.emplace_back(TokenType::Variable, tokenStr, position);
            continue;
        }

        // ���� ����� ���������� � ����� ��� ������������� �������
        if (isdigit(tokenStr[0]) || !isalpha(tokenStr[0])) {
            errorList.insert(Error(Error::ErrorType::invalidVariableName, position));
            continue;
        }

        // ��� ��������� ������ - ���������������� ��������
        errorList.insert(Error(Error::ErrorType::unsupportedOperation, position));
    }

    return tokens;
}

ExpressionNode* buildExpressionTree(const std::vector<Token>& tokens, std::set<Error>& errorList) {
    std::vector<ExpressionNode*> stack;

    for (const auto& token : tokens) {
        if (token.type == TokenType::Variable) {
            stack.push_back(new ExpressionNode(token.type, token.value));
            continue;
        }

        if (token.type == TokenType::Not) {
            if (stack.empty()) {
                errorList.insert(Error(Error::ErrorType::insufficientOperands, token.position));
                continue;
            }

            ExpressionNode* operand = stack.back();
            stack.pop_back();
            stack.push_back(new ExpressionNode(token.type, nullptr, operand));
            continue;
        }

        // �������� ��������
        if (stack.size() < 2) {
            errorList.insert(Error(Error::ErrorType::insufficientOperands, token.position));
            continue;
        }

        ExpressionNode* right = stack.back();
        stack.pop_back();
        ExpressionNode* left = stack.back();
        stack.pop_back();
        stack.push_back(new ExpressionNode(token.type, left, right));
    }

    if (stack.size() != 1) {
        errorList.insert(Error(Error::ErrorType::missingOperation, 0));
        return nullptr;
    }

    return stack.back();
}

ExpressionNode* copyNode(ExpressionNode* node) {
    if (!node) return nullptr;

    return new ExpressionNode(
        node->type,
        node->value,
        copyNode(node->left),
        copyNode(node->right)
    );
}

void transformImplicationAndEquivalence(ExpressionNode* node) {
    if (!node) return;

    // ���������� ����������� ����������
    transformImplicationAndEquivalence(node->left);
    transformImplicationAndEquivalence(node->right);

    // �������������� ���������� A > B � !A | B
    if (node->type == TokenType::Implication) {
        ExpressionNode* newNot = new ExpressionNode(TokenType::Not, nullptr, node->left);
        node->type = TokenType::Or;
        node->left = newNot;
    }

    // �������������� ��������������� A ~ B � (A & B) | (!A & !B)
    if (node->type == TokenType::Equivalence) {
        // ����� ���� ���������� (A & B)
        ExpressionNode* newLeft = new ExpressionNode(
            TokenType::And,
            copyNode(node->left),
            copyNode(node->right)
        );

        // ������ ���� ���������� (!A & !B)
        ExpressionNode* newRightL = new ExpressionNode(
            TokenType::Not,
            nullptr,
            copyNode(node->left)
        );

        ExpressionNode* newRightR = new ExpressionNode(
            TokenType::Not,
            nullptr,
            copyNode(node->right)
        );

        ExpressionNode* newRight = new ExpressionNode(
            TokenType::And,
            newRightL,
            newRightR
        );

        // ��������� ������� ����
        node->type = TokenType::Or;
        node->left = newLeft;
        node->right = newRight;
    }
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
