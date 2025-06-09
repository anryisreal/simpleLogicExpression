/**
 * @file testFunctions.cpp
 * @brief Реализация дополнительных функций для тестирования.
 */

#include "pch.h"
#include <algorithm>
#include "CppUnitTest.h"
#include "../simpleLogicExpression/functions.h"
#include "../simpleLogicExpression/objects.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

/**
* @brief Рекурсивно сравнивает два дерева выражений
* @param [in] expected Ожидаемое дерево
* @param [in] actual Фактическое дерево
* @param [in, out] path Текущий путь в дереве (для диагностики)
* @return true если деревья идентичны, false в противном случае
*/
bool compareExpressionTrees(const ExpressionNode* expected, const ExpressionNode* actual, const std::wstring& path = L"root")
{
    if (expected == nullptr && actual == nullptr) return true;
    if (expected == nullptr) {
        Logger::WriteMessage((L"Ошибка в узле: " + path + L" - ожидался nullptr").c_str());
        return false;
    }
    if (actual == nullptr) {
        Logger::WriteMessage((L"Ошибка в узле: " + path + L" - неожиданный nullptr").c_str());
        return false;
    }

    if (expected->type != actual->type) {
        Logger::WriteMessage((L"Ошибка в узле: " + path + L" - тип узла не совпадает").c_str());
        return false;
    }

    if (expected->value != actual->value) {
        Logger::WriteMessage((L"Ошибка в узле: " + path + L" - значение не совпадает").c_str());
        return false;
    }

    return compareExpressionTrees(expected->left, actual->left, path + L"-left") &&
        compareExpressionTrees(expected->right, actual->right, path + L"-right");
}

/**
 * @brief Сравнивает два множества ошибок и выводит различия
 * @param [in] expected Ожидаемое множество ошибок
 * @param [in] actual Фактическое множество ошибок
 * @return true если множества идентичны, false в противном случае
 */
bool compareErrorSets(const std::set<Error>& expected, const std::set<Error>& actual)
{
    if (expected == actual) {
        return true;
    }

    std::wstring message = L"Ошибки не совпадают:\n";

    // Находим ошибки, которые ожидались, но отсутствуют в actual
    std::vector<Error> missingErrors;
    std::set_difference(expected.begin(), expected.end(), actual.begin(), actual.end(), std::back_inserter(missingErrors));

    if (!missingErrors.empty()) {
        message += L"Отсутствующие ошибки:\n";
        for (const auto& error : missingErrors) {
            message += L"  - " + std::wstring(Error::getErrorTypeString(error.type).begin(), Error::getErrorTypeString(error.type).end()) +
                L" на позиции " + std::to_wstring(error.position) + L"\n";
        }
    }

    // Находим ошибки, которые не ожидались, но присутствуют в actual
    std::vector<Error> unexpectedErrors;
    std::set_difference(actual.begin(), actual.end(), expected.begin(), expected.end(), std::back_inserter(unexpectedErrors));

    if (!unexpectedErrors.empty()) {
        message += L"Неожиданные ошибки:\n";
        for (const auto& error : unexpectedErrors) {
            message += L"  + " + std::wstring(Error::getErrorTypeString(error.type).begin(), Error::getErrorTypeString(error.type).end()) +
                L" на позиции " + std::to_wstring(error.position) + L"\n";
        }
    }

    Logger::WriteMessage(message.c_str());
    return false;
}

/**
 * @brief Рекурсивно проверяет соответствие двух узлов и выводит путь при несоответствии
 * @param [in] original Оригинальный узел
 * @param [in] copied Скопированный узел
 * @param [in, out] path Текущий путь в дереве (для сообщений об ошибках)
 * @return true если узлы идентичны, false в противном случае
 */
bool compareNodesRecursive(const ExpressionNode* original, const ExpressionNode* copied, const std::wstring& path = L"node")
{
    // Проверка на nullptr
    if (original == nullptr && copied == nullptr) {
        return true;
    }

    // Проверка на равенство указателей
    if (original == copied) {
        if (original != nullptr) {
            Logger::WriteMessage((L"Ошибка в узле: " + path + L" (указатели на оригинал и копию совпадают)").c_str());
        }
        return false;
    }

    if (original == nullptr) {
        Logger::WriteMessage((L"Ошибка в узле: " + path + L" (оригинал null, но копия не null)").c_str());
        return false;
    }

    if (copied == nullptr) {
        Logger::WriteMessage((L"Ошибка в узле: " + path + L" (копия null, но оригинал не null)").c_str());
        return false;
    }

    // Проверка содержимого узлов
    if (original->type != copied->type) {
        Logger::WriteMessage((L"Ошибка в узле: " + path + L" (тип не совпадает: оригинал " + std::to_wstring(static_cast<int>(original->type)) +
            L", копия " + std::to_wstring(static_cast<int>(copied->type)) + L")").c_str());
        return false;
    }

    if (original->value != copied->value) {
        Logger::WriteMessage((L"Ошибка в узле: " + path + L" (значение не совпадает: оригинал '" + std::wstring(original->value.begin(), original->value.end()) +
            L"', копия '" + std::wstring(copied->value.begin(), copied->value.end()) + L"')").c_str());
        return false;
    }

    // Проверка поддеревьев
    bool leftValid = compareNodesRecursive(original->left, copied->left, path + L"-left");
    bool rightValid = compareNodesRecursive(original->right, copied->right, path + L"-right");

    return leftValid && rightValid;
}

/**
 * @brief Проверяет, что два узла идентичны, включая все поддеревья
 * @param [in] original Оригинальный узел
 * @param [in] copied Скопированный узел
 */
void AssertNodesEqual(const ExpressionNode* original, const ExpressionNode* copied)
{
    bool result = compareNodesRecursive(original, copied);
    Assert::IsTrue(result, L"Узлы не идентичны. Проверьте вывод для деталей.");
}