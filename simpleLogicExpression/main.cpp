#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <algorithm>
#include <locale>
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
int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");

    // ��������� ���������� ���������� ��������� ������
    if (argc != 3) {
        std::cerr << L"�������������: " << argv[0] << " <input file> <output file>" << std::endl;
        return 1;
    }

    std::set<Error> errorList;
    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    std::string content;

    // ������ �������� �����
    try {
        content = readFile(inputFile);
    }
    catch (const Error& e) {
        e.message();
        return 1;
    }

    // ����������� ������� ������
    std::vector<Token> tokens = tokenize(content, errorList);

    // �������� �� ������ �����������
    if (!errorList.empty()) {
        for (const auto& error : errorList) {
            error.message();
        }
        return 1;
    }

    // ���������� ������ ���������
    ExpressionNode* exprTree = buildExpressionTree(tokens, errorList);

    // �������� �� ������ ���������� ������
    if (!errorList.empty()) {
        for (const auto& error : errorList) {
            error.message();
        }
        delete exprTree; // ����������� ������
        return 1;
    }

    // �������������� ���������� � ���������������
    transformImplicationAndEquivalence(exprTree);

    // ���������� ������� �� ������� �� ��� ���, ���� ���� ���������
    bool changed;
    do {
        changed = simplifyExpression(exprTree);
    } while (changed);

    // �������� ������� ���������
    removeDoubleNot(exprTree);

    // ������������ �������� ������
    std::string result = expressionTreeToInfix(exprTree);

    // ������ ���������� � �������� ����
    try {
        writeFile(outputFile, result);
    }
    catch (const Error& e) {
        e.message();
        delete exprTree; // ����������� ������
        return 1;
    }

    // ������������ ������
    delete exprTree;

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
        ExpressionNode* newNot = new ExpressionNode(TokenType::Not, nullptr, copyNode(node->left));
        node->type = TokenType::Or;
        node->left = newNot; // node->right �������� ��� ���������
        return;
    }

    // �������������� ��������������� A ~ B � (A & B) | (!A & !B)
    if (node->type == TokenType::Equivalence) {
        // ������� ����� ���� ���������� (A & B)
        ExpressionNode* newLeft = new ExpressionNode(TokenType::And, copyNode(node->left), copyNode(node->right));

        // ������� ������ ���� ���������� (!A & !B)
        ExpressionNode* newRightL = new ExpressionNode(TokenType::Not, nullptr, copyNode(node->left));
        ExpressionNode* newRightR = new ExpressionNode(TokenType::Not, nullptr, copyNode(node->right));
        ExpressionNode* newRight = new ExpressionNode(TokenType::And, newRightL, newRightR);

        // ��������� ������� ����
        node->type = TokenType::Or;
        node->left = newLeft;
        node->right = newRight;
    }
}
bool simplifyExpression(ExpressionNode* node) {
    if (!node) return false;

    bool changed = false;

    // ������� ���������� �������� ����������
    changed |= simplifyExpression(node->left);
    changed |= simplifyExpression(node->right);

    // ��������� ������ ����� �� �������: !(A & B) ? !A | !B
    if (node->type == TokenType::Not && node->right && node->right->type == TokenType::And) {
        // ��������� ������������ ����
        ExpressionNode* originalAnd = node->right;
        ExpressionNode* leftOperand = originalAnd->left;
        ExpressionNode* rightOperand = originalAnd->right;

        // ������� ����� ���� ���������
        ExpressionNode* newNotLeft = new ExpressionNode(TokenType::Not, nullptr, leftOperand);
        ExpressionNode* newNotRight = new ExpressionNode(TokenType::Not, nullptr, rightOperand);

        // ����������� ������� ����
        node->type = TokenType::Or;
        node->left = newNotLeft;
        node->right = newNotRight;

        // ������� ������ ���� And (�� �� ��� �������� - ��� ������ ������������ � ����� �����)
        originalAnd->left = nullptr;
        originalAnd->right = nullptr;
        delete originalAnd;

        changed = true;
    }
    // ��������� ������ ����� �� �������: !(A | B) ? !A & !B
    else if (node->type == TokenType::Not && node->right && node->right->type == TokenType::Or) {
        ExpressionNode* originalOr = node->right;
        ExpressionNode* leftOperand = originalOr->left;
        ExpressionNode* rightOperand = originalOr->right;

        ExpressionNode* newNotLeft = new ExpressionNode(TokenType::Not, nullptr, leftOperand);
        ExpressionNode* newNotRight = new ExpressionNode(TokenType::Not, nullptr, rightOperand);

        node->type = TokenType::And;
        node->left = newNotLeft;
        node->right = newNotRight;

        originalOr->left = nullptr;
        originalOr->right = nullptr;
        delete originalOr;

        changed = true;
    }

    return changed;
}

void removeDoubleNot(ExpressionNode* node) {
    if (!node) return;

    // ���������� ������������ ����������
    removeDoubleNot(node->left);
    removeDoubleNot(node->right);

    // ������� ������� ���������
    if (node->type == TokenType::Not && node->right && node->right->type == TokenType::Not) {
        ExpressionNode* temp = node->right;
        node->type = temp->right->type;
        node->value = temp->right->value;
        node->left = temp->right->left;
        node->right = temp->right->right;

        // �������� ���������, ����� �������� �������� ��������
        temp->right->left = nullptr;
        temp->right->right = nullptr;
        delete temp;
    }
}

std::string expressionTreeToInfix(ExpressionNode* node) {
    if (!node) return "";

    // ����� ����������� ��������
    static const std::map<TokenType, int> priority = {
        {TokenType::Equivalence, 1},
        {TokenType::Implication, 2},
        {TokenType::Or, 3},
        {TokenType::And, 4},
        {TokenType::Not, 5}
    };

    // ��� ����������
    if (node->type == TokenType::Variable) {
        return node->value;
    }

    // ��� ������� �������� (���������)
    if (node->type == TokenType::Not) {
        std::string rightExpr = expressionTreeToInfix(node->right);
        // ��������� ������, ���� ������������ �������
        if (node->right && node->right->type != TokenType::Variable &&
            node->right->type != TokenType::Not) {
            return "!(" + rightExpr + ")";
        }
        return "!" + rightExpr;
    }

    // ��� �������� ��������
    std::string leftExpr = expressionTreeToInfix(node->left);
    std::string rightExpr = expressionTreeToInfix(node->right);

    // ���������� ��������
    std::string op;
    switch (node->type) {
    case TokenType::And: op = " & "; break;
    case TokenType::Or: op = " || "; break;
    case TokenType::Implication: op = " -> "; break;
    case TokenType::Equivalence: op = " ~ "; break;
    default: op = " ? "; break;
    }

    // ��������� ���������� ��� ������ (����� �������)
    if (node->left && priority.count(node->left->type)) {
        if (priority.at(node->left->type) < priority.at(node->type)) {
            leftExpr = "(" + leftExpr + ")";
        }
    }

    // ��������� ���������� ��� ������ (������ �������)
    if (node->right && priority.count(node->right->type)) {
        int rightPrio = priority.at(node->right->type);
        int currPrio = priority.at(node->type);
        if (rightPrio < currPrio ||
            (rightPrio == currPrio && node->type == TokenType::Implication)) {
            rightExpr = "(" + rightExpr + ")";
        }
    }

    return leftExpr + op + rightExpr;
}