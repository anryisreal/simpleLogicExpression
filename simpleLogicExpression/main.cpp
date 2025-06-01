#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <algorithm>
#include "objects.h"
#include "functions.h"

/**
* Соответствие между строкой и типом операции
*/
const std::map<std::string, TokenType> stringToTokenType = {
    {"!", TokenType::Not},
    {"&", TokenType::And},
    {"|", TokenType::Or},
    {">", TokenType::Implication},
    {"~", TokenType::Equivalence}
};

/**
 * Главная функция программы.
 *
 * Обрабатывает аргументы командной строки, выполняет чтение входного файла,
 * преобразование выражения и запись результата в выходной файл.
 * @param argc Количество аргументов
 * @param argv Массив аргументов (argv[1] - входной файл, argv[2] - выходной файл)
 * @return 0 при успешном завершении
 */
int main() {

	return 0;
}

/**
* Функци для чтения из файла
*/
std::string readFile(const std::string& filePath) {

    std::ifstream file(filePath);	// Создаем поток для чтения из файла

	// Выброс исключения, если файл не открылся
    if (!file.is_open()) {
        throw Error(Error::inputFile);
    }
    
    std::string content;			// Считываемая строка из файла
    std::getline(file, content);	// Считывание строки
    file.close();					// Закрытие файла

	return content;
}

/**
* Функция для записи в файл
*/
void writeFile(const std::string& filePath, const std::string& content) {

    std::ofstream file(filePath);  // Создаем поток для записи в файл

    // Выброс исключения, если файл не открылся
    if (!file.is_open()) {
        throw Error(Error::outputFile);
    }

    file << content;               // Запись в файл
    file.close();                  // Закрытие файла
}

std::vector<Token> tokenize(const std::string& expression, std::set<Error>& errorList) {
    std::vector<Token> tokens;
    std::istringstream iss(expression);
    std::string tokenStr;
    int position = 0;

    while (iss >> tokenStr) {
        position++;

        // Проверка на операцию
        auto it = stringToTokenType.find(tokenStr);
        if (it != stringToTokenType.end()) {
            tokens.emplace_back(it->second, tokenStr, position);
            continue;
        }

        // Проверка на переменную
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

        // Если токен не распознан
        errorList.insert(Error(Error::ErrorType::invalidVariableName, position));
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

        // Бинарные операции
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
