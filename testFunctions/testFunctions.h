#pragma once
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
bool compareExpressionTrees(const ExpressionNode* expected, const ExpressionNode* actual, const std::string& path = "root");

/**
 * @brief ���������� ��� ��������� ������ � ������� ��������
 * @param expected ��������� ��������� ������
 * @param actual ����������� ��������� ������
 * @return true ���� ��������� ���������, false � ��������� ������
 */
bool compareErrorSets(const std::set<Error>& expected, const std::set<Error>& actual);

/**
 * @brief ���������� ��������� ������������ ���� ����� � ������� ���� ��� ��������������
 * @param original ������������ ����
 * @param copied ������������� ����
 * @param path ������� ���� � ������ (��� ��������� �� �������)
 * @return true ���� ���� ���������, false � ��������� ������
 */
bool compareNodesRecursive(const ExpressionNode* original, const ExpressionNode* copied, const std::string& path = "node");

/**
 * @brief ���������, ��� ��� ���� ���������, ������� ��� ����������
 * @param original ������������ ����
 * @param copied ������������� ����
 */
void AssertNodesEqual(const ExpressionNode* original, const ExpressionNode* copied);