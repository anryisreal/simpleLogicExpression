#include "pch.h"
#include "CppUnitTest.h"
#include <algorithm>
#include "../simpleLogicExpression/functions.h"
#include "../simpleLogicExpression/objects.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace testbuildExpressionTree
{
    TEST_CLASS(testbuildExpressionTree)
    {
    public:
        /**
         * @brief ���������� ���������� ��� ������ ���������
         * @param expected ��������� ������
         * @param actual ����������� ������
         * @param path ������� ���� � ������ (��� �����������)
         * @return true ���� ������� ���������, false � ��������� ������
         */
        bool compareExpressionTrees(const ExpressionNode* expected, const ExpressionNode* actual, const std::string& path = "root") const
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
        bool compareErrorSets(const std::set<Error>& expected, const std::set<Error>& actual) const
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
         * @brief ���� 1: �������� ��������� ������� ������ �������
         */
        TEST_METHOD(Test1_EmptyTokenList)
        {
            std::vector<Token> tokens;
            std::set<Error> errorList;

            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            std::set<Error> expectedErrors = {
                {Error::missingOperation, -1}
            };

            Assert::IsNull(result);
            Assert::IsTrue(compareErrorSets(expectedErrors, errorList));
        }

        /**
         * @brief ���� 2: �������� ��������� ������ ������ (����������)
         */
        TEST_METHOD(Test2_SingleVariableToken)
        {
            std::vector<Token> tokens = { Token(TokenType::Variable, "a", 0) };
            std::set<Error> errorList;
            std::set<Error> expectedErrors;

            ExpressionNode* result = buildExpressionTree(tokens, errorList);
            ExpressionNode* expected = new ExpressionNode(TokenType::Variable, "a");

            Assert::IsNotNull(result);
            Assert::IsTrue(compareExpressionTrees(expected, result));
            Assert::IsTrue(compareErrorSets(expectedErrors, errorList));

            delete result;
            delete expected;
        }

        /**
         * @brief ���� 3: �������� ��������� ������ ������ (��������)
         */
        TEST_METHOD(Test3_SingleOperatorToken)
        {
            std::vector<Token> tokens = { Token(TokenType::Not, "!", 0) };
            std::set<Error> errorList;

            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            std::set<Error> expectedErrors = {
                {Error::insufficientOperands, 0}
            };

            Assert::IsNull(result);
            Assert::IsTrue(compareErrorSets(expectedErrors, errorList));
        }

        /**
         * @brief ���� 4: �������� ��������� �������� ���������
         */
        TEST_METHOD(Test4_SimpleNegation)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "a", 0),
                Token(TokenType::Not, "!", 1)
            };
            std::set<Error> errorList;
            std::set<Error> expectedErrors;

            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            ExpressionNode* expected = new ExpressionNode(TokenType::Not);
            expected->left = new ExpressionNode(TokenType::Variable, "a");

            Assert::IsNotNull(result);
            Assert::IsTrue(compareExpressionTrees(expected, result));
            Assert::IsTrue(compareErrorSets(expectedErrors, errorList));

            delete result;
            delete expected;
        }

        /**
         * @brief ���� 5: �������� ��������� ������� ����������
         */
        TEST_METHOD(Test5_SimpleConjunction)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "a", 0),
                Token(TokenType::Variable, "b", 1),
                Token(TokenType::And, "&", 2)
            };
            std::set<Error> errorList;
            std::set<Error> expectedErrors;

            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            ExpressionNode* expected = new ExpressionNode(TokenType::And);
            expected->left = new ExpressionNode(TokenType::Variable, "a");
            expected->right = new ExpressionNode(TokenType::Variable, "b");

            Assert::IsNotNull(result);
            Assert::IsTrue(compareExpressionTrees(expected, result));
            Assert::IsTrue(compareErrorSets(expectedErrors, errorList));

            delete result;
            delete expected;
        }

        /**
         * @brief ���� 6: �������� ��������� ��������� ����� �������
         */
        TEST_METHOD(Test6_NegationBeforeBracket)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "a", 0),
                Token(TokenType::Variable, "b", 1),
                Token(TokenType::And, "&", 2),
                Token(TokenType::Not, "!", 3)
            };
            std::set<Error> errorList;
            std::set<Error> expectedErrors;

            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            ExpressionNode* expected = new ExpressionNode(TokenType::Not);
            expected->left = new ExpressionNode(TokenType::And);
            expected->left->left = new ExpressionNode(TokenType::Variable, "a");
            expected->left->right = new ExpressionNode(TokenType::Variable, "b");

            Assert::IsNotNull(result);
            Assert::IsTrue(compareExpressionTrees(expected, result));
            Assert::IsTrue(compareErrorSets(expectedErrors, errorList));

            delete result;
            delete expected;
        }

        /**
         * @brief ���� 7: �������� ��������� ������������ ���������
         */
        TEST_METHOD(Test7_ComplexExpression)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "a", 0),
                Token(TokenType::Variable, "b", 1),
                Token(TokenType::And, "&", 2),
                Token(TokenType::Variable, "c", 3),
                Token(TokenType::Variable, "d", 4),
                Token(TokenType::And, "&", 5),
                Token(TokenType::Or, "|", 6)
            };
            std::set<Error> errorList;
            std::set<Error> expectedErrors;

            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            ExpressionNode* expected = new ExpressionNode(TokenType::Or);
            expected->left = new ExpressionNode(TokenType::And);
            expected->left->left = new ExpressionNode(TokenType::Variable, "a");
            expected->left->right = new ExpressionNode(TokenType::Variable, "b");
            expected->right = new ExpressionNode(TokenType::And);
            expected->right->left = new ExpressionNode(TokenType::Variable, "c");
            expected->right->right = new ExpressionNode(TokenType::Variable, "d");

            Assert::IsNotNull(result);
            Assert::IsTrue(compareExpressionTrees(expected, result));
            Assert::IsTrue(compareErrorSets(expectedErrors, errorList));

            delete result;
            delete expected;
        }

        /**
         * @brief ���� 8: �������� ��������� ������������� ������
         */
        TEST_METHOD(Test8_InvalidToken)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "1a", 0),
                Token(TokenType::Variable, "b", 1),
                Token(TokenType::And, "&", 2)
            };
            std::set<Error> errorList;

            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            std::set<Error> expectedErrors = {
                {Error::invalidVariableName, 0}
            };

            Assert::IsNull(result);
            Assert::IsTrue(compareErrorSets(expectedErrors, errorList));
        }

        /**
         * @brief ���� 9: �������� ��������� ����������
         */
        TEST_METHOD(Test9_Implication)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "a", 0),
                Token(TokenType::Variable, "b", 1),
                Token(TokenType::Implication, ">", 2)
            };
            std::set<Error> errorList;
            std::set<Error> expectedErrors;

            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            ExpressionNode* expected = new ExpressionNode(TokenType::Implication);
            expected->left = new ExpressionNode(TokenType::Variable, "a");
            expected->right = new ExpressionNode(TokenType::Variable, "b");

            Assert::IsNotNull(result);
            Assert::IsTrue(compareExpressionTrees(expected, result));
            Assert::IsTrue(compareErrorSets(expectedErrors, errorList));

            delete result;
            delete expected;
        }

        /**
         * @brief ���� 10: �������� ��������� ���������������
         */
        TEST_METHOD(Test10_Equivalence)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "a", 0),
                Token(TokenType::Variable, "b", 1),
                Token(TokenType::Equivalence, "~", 2)
            };
            std::set<Error> errorList;
            std::set<Error> expectedErrors;

            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            ExpressionNode* expected = new ExpressionNode(TokenType::Equivalence);
            expected->left = new ExpressionNode(TokenType::Variable, "a");
            expected->right = new ExpressionNode(TokenType::Variable, "b");

            Assert::IsNotNull(result);
            Assert::IsTrue(compareExpressionTrees(expected, result));
            Assert::IsTrue(compareErrorSets(expectedErrors, errorList));

            delete result;
            delete expected;
        }

        /**
         * @brief ���� 11: �������� ��������� ������������� ��������
         */
        TEST_METHOD(Test11_MultipleOperations)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "b", 0),
                Token(TokenType::Not, "!", 1),
                Token(TokenType::Variable, "c", 2),
                Token(TokenType::And, "&", 3),
                Token(TokenType::Variable, "d", 4),
                Token(TokenType::Or, "|", 5)
            };
            std::set<Error> errorList;
            std::set<Error> expectedErrors;

            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            ExpressionNode* expected = new ExpressionNode(TokenType::Or);
            expected->left = new ExpressionNode(TokenType::And);
            expected->left->left = new ExpressionNode(TokenType::Not);
            expected->left->left->left = new ExpressionNode(TokenType::Variable, "b");
            expected->left->right = new ExpressionNode(TokenType::Variable, "c");
            expected->right = new ExpressionNode(TokenType::Variable, "d");

            Assert::IsNotNull(result);
            Assert::IsTrue(compareExpressionTrees(expected, result));
            Assert::IsTrue(compareErrorSets(expectedErrors, errorList));

            delete result;
            delete expected;
        }

        /**
         * @brief ���� 12: �������� ��������� ������ � �������� ���������
         */
        TEST_METHOD(Test12_ErrorInMiddleOfExpression)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "a", 0),
                Token(TokenType::Variable, "b", 1),
                Token(TokenType::Any, "+", 2),
                Token(TokenType::Variable, "c", 3),
                Token(TokenType::And, "&", 4)
            };
            std::set<Error> errorList;

            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            std::set<Error> expectedErrors = {
                {Error::unsupportedOperation, 2}
            };

            Assert::IsNull(result);
            Assert::IsTrue(compareErrorSets(expectedErrors, errorList));
        }

        /**
         * @brief ���� 13: �������� ��������� ��������� ��� ������� ����������
         */
        TEST_METHOD(Test13_NegationOverComplexExpression)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "b", 0),
                Token(TokenType::Not, "!", 1),
                Token(TokenType::Variable, "c", 2),
                Token(TokenType::And, "&", 3),
                Token(TokenType::Variable, "d", 4),
                Token(TokenType::Or, "|", 5),
                Token(TokenType::Not, "!", 6)
            };
            std::set<Error> errorList;
            std::set<Error> expectedErrors;

            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            ExpressionNode* expected = new ExpressionNode(TokenType::Not);
            expected->left = new ExpressionNode(TokenType::Or);
            expected->left->left = new ExpressionNode(TokenType::And);
            expected->left->left->left = new ExpressionNode(TokenType::Not);
            expected->left->left->left->left = new ExpressionNode(TokenType::Variable, "b");
            expected->left->left->right = new ExpressionNode(TokenType::Variable, "c");
            expected->left->right = new ExpressionNode(TokenType::Variable, "d");

            Assert::IsNotNull(result);
            Assert::IsTrue(compareExpressionTrees(expected, result));
            Assert::IsTrue(compareErrorSets(expectedErrors, errorList));

            delete result;
            delete expected;
        }

        /**
         * @brief ���� 14: �������� ��������� �������� ���������
         */
        TEST_METHOD(Test14_DoubleNegation)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "a", 0),
                Token(TokenType::Not, "!", 1),
                Token(TokenType::Not, "!", 2)
            };
            std::set<Error> errorList;
            std::set<Error> expectedErrors;

            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            ExpressionNode* expected = new ExpressionNode(TokenType::Not);
            expected->left = new ExpressionNode(TokenType::Not);
            expected->left->left = new ExpressionNode(TokenType::Variable, "a");

            Assert::IsNotNull(result);
            Assert::IsTrue(compareExpressionTrees(expected, result));
            Assert::IsTrue(compareErrorSets(expectedErrors, errorList));

            delete result;
            delete expected;
        }

        /**
         * @brief ���� 15: �������� ��������� ��������� ���������
         */
        TEST_METHOD(Test15_NestedNegations)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "a", 0),
                Token(TokenType::Variable, "b", 1),
                Token(TokenType::Implication, ">", 2),
                Token(TokenType::Not, "!", 3),
                Token(TokenType::Variable, "c", 4),
                Token(TokenType::Variable, "d", 5),
                Token(TokenType::And, "&", 6),
                Token(TokenType::Not, "!", 7),
                Token(TokenType::Or, "|", 8),
                Token(TokenType::Not, "!", 9)
            };
            std::set<Error> errorList;
            std::set<Error> expectedErrors;

            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            // ������� ��������� ������
            ExpressionNode* expected = new ExpressionNode(TokenType::Not);
            expected->left = new ExpressionNode(TokenType::Or);
            expected->left->left = new ExpressionNode(TokenType::Not);
            expected->left->left->left = new ExpressionNode(TokenType::Implication);
            expected->left->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            expected->left->left->left->right = new ExpressionNode(TokenType::Variable, "b");
            expected->left->right = new ExpressionNode(TokenType::Not);
            expected->left->right->left = new ExpressionNode(TokenType::And);
            expected->left->right->left->left = new ExpressionNode(TokenType::Variable, "c");
            expected->left->right->left->right = new ExpressionNode(TokenType::Variable, "d");

            Assert::IsNotNull(result);
            Assert::IsTrue(compareExpressionTrees(expected, result));
            Assert::IsTrue(compareErrorSets(expectedErrors, errorList));

            delete result;
            delete expected;
        }

        /**
         * @brief ���� 16: �������� ��������� ���������� � ������������ ���������
         */
        TEST_METHOD(Test16_VariableWithForbiddenChars)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "a_!", 0),
                Token(TokenType::Variable, "b", 1),
                Token(TokenType::And, "&", 2)
            };
            std::set<Error> errorList;

            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            std::set<Error> expectedErrors = {
                {Error::invalidVariableChar, 0}
            };

            Assert::IsNull(result);
            Assert::IsTrue(compareErrorSets(expectedErrors, errorList));
        }

        /**
         * @brief ���� 17: �������� ��������� ������ �������� ��������
         */
        TEST_METHOD(Test17_OnlyBinaryOperations)
        {
            std::vector<Token> tokens = {
                Token(TokenType::And, "&", 0),
                Token(TokenType::Implication, ">", 1),
                Token(TokenType::Equivalence, "~", 2)
            };
            std::set<Error> errorList;

            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            std::set<Error> expectedErrors = {
                {Error::insufficientOperands, 0},
                {Error::insufficientOperands, 1},
                {Error::insufficientOperands, 2}
            };

            Assert::IsNull(result);
            Assert::IsTrue(compareErrorSets(expectedErrors, errorList));
        }

        /**
         * @brief ���� 18: �������� ��������� ���������� ����� ������
         */
        TEST_METHOD(Test18_MultipleErrorTypes)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "a_!", 0),
                Token(TokenType::And, "&", 1)
            };
            std::set<Error> errorList;

            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            std::set<Error> expectedErrors = {
                {Error::invalidVariableChar, 0},
                {Error::insufficientOperands, 1}
            };

            Assert::IsNull(result);
            Assert::IsTrue(compareErrorSets(expectedErrors, errorList));
        }
    };
}