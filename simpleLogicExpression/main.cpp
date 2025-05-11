#include <iostream>
#include <fstream>
#include <string>
#include "objects.h"
#include "functions.h"

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
