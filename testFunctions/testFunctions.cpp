#include "pch.h"
#include <algorithm>
#include "CppUnitTest.h"
#include "../simpleLogicExpression/functions.h"
#include "../simpleLogicExpression/objects.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

/**
* @brief ���������� ���������� ��� ������ ���������
* @param expected ��������� ������
* @param actual ����������� ������
* @param path ������� ���� � ������ (��� �����������)
* @return true ���� ������� ���������, false � ��������� ������
*/
bool compareExpressionTrees(const ExpressionNode* expected, const ExpressionNode* actual, const std::string& path = "root")
{
    if (expected == nullptr && actual == nullptr) return true;
    if (expected == nullptr) {
        Logger::WriteMessage(("������ � ����: " + path + " - �������� nullptr").c_str());
        return false;
    }
    if (actual == nullptr) {
        Logger::WriteMessage(("������ � ����: " + path + " - ����������� nullptr").c_str());
        return false;
    }

    if (expected->type != actual->type) {
        Logger::WriteMessage(("������ � ����: " + path + " - ��� ���� �� ���������").c_str());
        return false;
    }

    if (expected->value != actual->value) {
        Logger::WriteMessage(("������ � ����: " + path + " - �������� �� ���������").c_str());
        return false;
    }

    return compareExpressionTrees(expected->left, actual->left, path + "-left") && compareExpressionTrees(expected->right, actual->right, path + "-right");
}

/**
 * @brief ���������� ��� ��������� ������ � ������� ��������
 * @param expected ��������� ��������� ������
 * @param actual ����������� ��������� ������
 * @return true ���� ��������� ���������, false � ��������� ������
 */
bool compareErrorSets(const std::set<Error>& expected, const std::set<Error>& actual)
{
    if (expected == actual) {
        return true;
    }

    std::string message = "������ �� ���������:\n";

    // ������� ������, ������� ���������, �� ����������� � actual
    std::vector<Error> missingErrors;
    std::set_difference(expected.begin(), expected.end(), actual.begin(), actual.end(), std::back_inserter(missingErrors));

    if (!missingErrors.empty()) {
        message += "������������� ������:\n";
        for (const auto& error : missingErrors) {
            message += "  - " + Error::getErrorTypeString(error.type) + " �� ������� " + std::to_string(error.position) + "\n";
        }
    }

    // ������� ������, ������� �� ���������, �� ������������ � actual
    std::vector<Error> unexpectedErrors;
    std::set_difference(actual.begin(), actual.end(), expected.begin(), expected.end(), std::back_inserter(unexpectedErrors));

    if (!unexpectedErrors.empty()) {
        message += "����������� ������:\n";
        for (const auto& error : unexpectedErrors) {
            message += "  + " + Error::getErrorTypeString(error.type) + " �� ������� " + std::to_string(error.position) + "\n";
        }
    }

    Logger::WriteMessage(message.c_str());
    return false;
}

/**
 * @brief ���������� ��������� ������������ ���� ����� � ������� ���� ��� ��������������
 * @param original ������������ ����
 * @param copied ������������� ����
 * @param path ������� ���� � ������ (��� ��������� �� �������)
 * @return true ���� ���� ���������, false � ��������� ������
 */
bool compareNodesRecursive(const ExpressionNode* original, const ExpressionNode* copied, const std::string& path = "node")
{
    // �������� �� ��������� ����������
    if (original == copied) {
        if (original != nullptr) {
            Logger::WriteMessage(("������ � ����: " + path + " (��������� �� �������� � ����� ���������)").c_str());
        }
        return false;
    }

    // �������� �� nullptr
    if (original == nullptr && copied == nullptr) {
        return true;
    }

    if (original == nullptr) {
        Logger::WriteMessage(("������ � ����: " + path + " (�������� null, �� ����� �� null)").c_str());
        return false;
    }

    if (copied == nullptr) {
        Logger::WriteMessage(("������ � ����: " + path + " (����� null, �� �������� �� null)").c_str());
        return false;
    }

    // �������� ����������� �����
    if (original->type != copied->type) {
        Logger::WriteMessage(("������ � ����: " + path + " (��� �� ���������: �������� " + std::to_string(static_cast<int>(original->type)) + ", ����� " + std::to_string(static_cast<int>(copied->type)) + ")").c_str());
        return false;
    }

    if (original->value != copied->value) {
        Logger::WriteMessage(("������ � ����: " + path + " (�������� �� ���������: �������� '" + original->value + "', ����� '" + copied->value + "')").c_str());
        return false;
    }

    // �������� �����������
    bool leftValid = compareNodesRecursive(original->left, copied->left, path + "-left");
    bool rightValid = compareNodesRecursive(original->right, copied->right, path + "-right");

    return leftValid && rightValid;
}

/**
 * @brief ���������, ��� ��� ���� ���������, ������� ��� ����������
 * @param original ������������ ����
 * @param copied ������������� ����
 */
void AssertNodesEqual(const ExpressionNode* original, const ExpressionNode* copied)
{
    bool result = compareNodesRecursive(original, copied);
    Assert::IsTrue(result, L"���� �� ���������. ��������� ����� ��� �������.");
}