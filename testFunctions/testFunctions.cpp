#include "pch.h"
#include <algorithm>
#include "CppUnitTest.h"
#include "../simpleLogicExpression/functions.h"
#include "../simpleLogicExpression/objects.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

/**
* @brief Рекурсивно сравнивает два дерева выражений
* @param expected Ожидаемое дерево
* @param actual Фактическое дерево
* @param path Текущий путь в дереве (для диагностики)
* @return true если деревья идентичны, false в противном случае
*/
bool compareExpressionTrees(const ExpressionNode* expected, const ExpressionNode* actual, const std::string& path = "root")
{
    if (expected == nullptr && actual == nullptr) return true;
    if (expected == nullptr) {
        Logger::WriteMessage(("Ошибка в узле: " + path + " - ожидался nullptr").c_str());
        return false;
    }
    if (actual == nullptr) {
        Logger::WriteMessage(("Ошибка в узле: " + path + " - неожиданный nullptr").c_str());
        return false;
    }

    if (expected->type != actual->type) {
        Logger::WriteMessage(("Ошибка в узле: " + path + " - тип узла не совпадает").c_str());
        return false;
    }

    if (expected->value != actual->value) {
        Logger::WriteMessage(("Ошибка в узле: " + path + " - значение не совпадает").c_str());
        return false;
    }

    return compareExpressionTrees(expected->left, actual->left, path + "-left") && compareExpressionTrees(expected->right, actual->right, path + "-right");
}

/**
 * @brief Сравнивает два множества ошибок и выводит различия
 * @param expected Ожидаемое множество ошибок
 * @param actual Фактическое множество ошибок
 * @return true если множества идентичны, false в противном случае
 */
bool compareErrorSets(const std::set<Error>& expected, const std::set<Error>& actual)
{
    if (expected == actual) {
        return true;
    }

    std::string message = "Ошибки не совпадают:\n";

    // Находим ошибки, которые ожидались, но отсутствуют в actual
    std::vector<Error> missingErrors;
    std::set_difference(expected.begin(), expected.end(), actual.begin(), actual.end(), std::back_inserter(missingErrors));

    if (!missingErrors.empty()) {
        message += "Отсутствующие ошибки:\n";
        for (const auto& error : missingErrors) {
            message += "  - " + Error::getErrorTypeString(error.type) + " на позиции " + std::to_string(error.position) + "\n";
        }
    }

    // Находим ошибки, которые не ожидались, но присутствуют в actual
    std::vector<Error> unexpectedErrors;
    std::set_difference(actual.begin(), actual.end(), expected.begin(), expected.end(), std::back_inserter(unexpectedErrors));

    if (!unexpectedErrors.empty()) {
        message += "Неожиданные ошибки:\n";
        for (const auto& error : unexpectedErrors) {
            message += "  + " + Error::getErrorTypeString(error.type) + " на позиции " + std::to_string(error.position) + "\n";
        }
    }

    Logger::WriteMessage(message.c_str());
    return false;
}

/**
 * @brief Рекурсивно проверяет соответствие двух узлов и выводит путь при несоответствии
 * @param original Оригинальный узел
 * @param copied Скопированный узел
 * @param path Текущий путь в дереве (для сообщений об ошибках)
 * @return true если узлы идентичны, false в противном случае
 */
bool compareNodesRecursive(const ExpressionNode* original, const ExpressionNode* copied, const std::string& path = "node")
{
    // Проверка на равенство указателей
    if (original == copied) {
        if (original != nullptr) {
            Logger::WriteMessage(("Ошибка в узле: " + path + " (указатели на оригинал и копию совпадают)").c_str());
        }
        return false;
    }

    // Проверка на nullptr
    if (original == nullptr && copied == nullptr) {
        return true;
    }

    if (original == nullptr) {
        Logger::WriteMessage(("Ошибка в узле: " + path + " (оригинал null, но копия не null)").c_str());
        return false;
    }

    if (copied == nullptr) {
        Logger::WriteMessage(("Ошибка в узле: " + path + " (копия null, но оригинал не null)").c_str());
        return false;
    }

    // Проверка содержимого узлов
    if (original->type != copied->type) {
        Logger::WriteMessage(("Ошибка в узле: " + path + " (тип не совпадает: оригинал " + std::to_string(static_cast<int>(original->type)) + ", копия " + std::to_string(static_cast<int>(copied->type)) + ")").c_str());
        return false;
    }

    if (original->value != copied->value) {
        Logger::WriteMessage(("Ошибка в узле: " + path + " (значение не совпадает: оригинал '" + original->value + "', копия '" + copied->value + "')").c_str());
        return false;
    }

    // Проверка поддеревьев
    bool leftValid = compareNodesRecursive(original->left, copied->left, path + "-left");
    bool rightValid = compareNodesRecursive(original->right, copied->right, path + "-right");

    return leftValid && rightValid;
}

/**
 * @brief Проверяет, что два узла идентичны, включая все поддеревья
 * @param original Оригинальный узел
 * @param copied Скопированный узел
 */
void AssertNodesEqual(const ExpressionNode* original, const ExpressionNode* copied)
{
    bool result = compareNodesRecursive(original, copied);
    Assert::IsTrue(result, L"Узлы не идентичны. Проверьте вывод для деталей.");
}