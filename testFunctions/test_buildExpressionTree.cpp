#include "pch.h"
#include "CppUnitTest.h"
#include "../simpleLogicExpression/functions.h"
#include "../simpleLogicExpression/objects.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace testbuildExpressionTree
{
    TEST_CLASS(testbuildExpressionTree)
    {
    public:

        /**
         * @brief ���� 1: �������� ��������� ������� ������ �������
         *
         * ������� ������ �������� ������ missingOperation ��� ������ ������ �������
         */
        TEST_METHOD(EmptyTokenList)
        {
            std::vector<Token> tokens;
            std::set<Error> errorList;

            // �������� ������� � ������ ������� �������
            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            // ���������, ��� ������� ������� nullptr
            Assert::IsNull(result);

            // ���������, ��� � ������ ������ ���� ������ missingOperation
            bool hasError = false;
            for (const auto& error : errorList) {
                if (error.type == Error::missingOperation) {
                    hasError = true;
                    break;
                }
            }
            Assert::IsTrue(hasError, L"������ missingOperation �� ��������� � ������ ������");
        }

        /**
         * @brief ���� 2: �������� ��������� ������ ������ (����������)
         *
         * ������� ������ ��������� ������� ������ � ����� ����������
         */
        TEST_METHOD(SingleVariableToken)
        {
            std::vector<Token> tokens = { Token(TokenType::Variable, "a", 0) };
            std::set<Error> errorList;

            // �������� ������� � ����� �������-����������
            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            // ���������, ��� ������� ������� �� nullptr
            Assert::IsNotNull(result);

            // ���������, ��� ��� ���� - ����������
            Assert::AreEqual(static_cast<int>(TokenType::Variable), static_cast<int>(result->type));

            // ���������, ��� �������� ���������� ����������
            Assert::AreEqual(std::string("a"), result->value);

            // ���������, ��� ������ ������ ����
            Assert::IsTrue(errorList.empty());

            // ����������� ������
            delete result;
        }

        /**
         * @brief ���� 3: �������� ��������� ������ ������ (��������)
         *
         * ������� ������ �������� ������ insufficientOperands ��� ����� ������-���������
         */
        TEST_METHOD(SingleOperatorToken)
        {
            std::vector<Token> tokens = { Token(TokenType::Not, "!", 0) };
            std::set<Error> errorList;

            // �������� ������� � ����� �������-����������
            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            // ���������, ��� ������� ������� nullptr
            Assert::IsNull(result);

            // ���������, ��� � ������ ������ ���� ������ insufficientOperands
            bool hasError = false;
            for (const auto& error : errorList) {
                if (error.type == Error::insufficientOperands) {
                    hasError = true;
                    break;
                }
            }
            Assert::IsTrue(hasError, L"������ insufficientOperands �� ��������� � ������ ������");
        }

        /**
         * @brief ���� 4: �������� ��������� �������� ���������
         *
         * ������� ������ ��������� ������� ������ � ���������� ����������
         */
        TEST_METHOD(SimpleNegation)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "a", 0),
                Token(TokenType::Not, "!", 1)
            };
            std::set<Error> errorList;

            // �������� ������� � �������� ��� �������� ���������
            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            // ���������, ��� ������� ������� �� nullptr
            Assert::IsNotNull(result);

            // ���������, ��� ��� ���� - ���������
            Assert::AreEqual(static_cast<int>(TokenType::Not), static_cast<int>(result->type));

            // ���������, ��� � ��������� ���� �������
            Assert::IsNotNull(result->left);

            // ���������, ��� ������� - ���������� "a"
            Assert::AreEqual(static_cast<int>(TokenType::Variable), static_cast<int>(result->left->type));
            Assert::AreEqual(std::string("a"), result->left->value);

            // ���������, ��� ������ ������ ����
            Assert::IsTrue(errorList.empty());

            // ����������� ������
            delete result;
        }

        /**
         * @brief ���� 5: �������� ��������� ������� ����������
         *
         * ������� ������ ��������� ������� ������ � ����������� ���� ����������
         */
        TEST_METHOD(SimpleConjunction)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "a", 0),
                Token(TokenType::Variable, "b", 1),
                Token(TokenType::And, "&", 2)
            };
            std::set<Error> errorList;

            // �������� ������� � �������� ��� ������� ����������
            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            // ���������, ��� ������� ������� �� nullptr
            Assert::IsNotNull(result);

            // ���������, ��� ��� ���� - ����������
            Assert::AreEqual(static_cast<int>(TokenType::And), static_cast<int>(result->type));

            // ���������, ��� � ���������� ���� ����� � ������ ��������
            Assert::IsNotNull(result->left);
            Assert::IsNotNull(result->right);

            // ���������, ��� ����� ������� - ���������� "a"
            Assert::AreEqual(static_cast<int>(TokenType::Variable), static_cast<int>(result->left->type));
            Assert::AreEqual(std::string("a"), result->left->value);

            // ���������, ��� ������ ������� - ���������� "b"
            Assert::AreEqual(static_cast<int>(TokenType::Variable), static_cast<int>(result->right->type));
            Assert::AreEqual(std::string("b"), result->right->value);

            // ���������, ��� ������ ������ ����
            Assert::IsTrue(errorList.empty());

            // ����������� ������
            delete result;
        }

        /**
         * @brief ���� 6: �������� ��������� ��������� ����� �������
         *
         * ������� ������ ��������� ������� ������ � ���������� ��������� � �������
         */
        TEST_METHOD(NegationBeforeBracket)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "a", 0),
                Token(TokenType::Variable, "b", 1),
                Token(TokenType::And, "&", 2),
                Token(TokenType::Not, "!", 3)
            };
            std::set<Error> errorList;

            // �������� ������� � �������� ��� ��������� ����� �������
            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            // ���������, ��� ������� ������� �� nullptr
            Assert::IsNotNull(result);

            // ���������, ��� ��� ���� - ���������
            Assert::AreEqual(static_cast<int>(TokenType::Not), static_cast<int>(result->type));

            // ���������, ��� � ��������� ���� �������
            Assert::IsNotNull(result->left);

            // ���������, ��� ������� - ����������
            Assert::AreEqual(static_cast<int>(TokenType::And), static_cast<int>(result->left->type));

            // ���������, ��� � ���������� ���� ����� � ������ ��������
            Assert::IsNotNull(result->left->left);
            Assert::IsNotNull(result->left->right);

            // ���������, ��� ����� ������� ���������� - ���������� "a"
            Assert::AreEqual(static_cast<int>(TokenType::Variable), static_cast<int>(result->left->left->type));
            Assert::AreEqual(std::string("a"), result->left->left->value);

            // ���������, ��� ������ ������� ���������� - ���������� "b"
            Assert::AreEqual(static_cast<int>(TokenType::Variable), static_cast<int>(result->left->right->type));
            Assert::AreEqual(std::string("b"), result->left->right->value);

            // ���������, ��� ������ ������ ����
            Assert::IsTrue(errorList.empty());

            // ����������� ������
            delete result;
        }

        /**
         * @brief ���� 7: �������� ��������� ������������ ���������
         *
         * ������� ������ ��������� ������� ������ ��� ������������ ���������
         */
        TEST_METHOD(ComplexExpression)
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

            // �������� ������� � �������� ��� ������������ ���������
            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            // ���������, ��� ������� ������� �� nullptr
            Assert::IsNotNull(result);

            // ���������, ��� ��� ���� - ����������
            Assert::AreEqual(static_cast<int>(TokenType::Or), static_cast<int>(result->type));

            // ���������, ��� � ���������� ���� ����� � ������ ��������
            Assert::IsNotNull(result->left);
            Assert::IsNotNull(result->right);

            // ���������, ��� ����� ������� - ����������
            Assert::AreEqual(static_cast<int>(TokenType::And), static_cast<int>(result->left->type));

            // ���������, ��� ������ ������� - ����������
            Assert::AreEqual(static_cast<int>(TokenType::And), static_cast<int>(result->right->type));

            // ���������, ��� ������ ������ ����
            Assert::IsTrue(errorList.empty());

            // ����������� ������
            delete result;
        }

        /**
         * @brief ���� 8: �������� ��������� ������������� ������
         *
         * ������� ������ �������� ������ invalidVariableName ��� ������������ ����� ����������
         */
        TEST_METHOD(InvalidToken)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "1a", 0),
                Token(TokenType::Variable, "b", 1),
                Token(TokenType::And, "&", 2)
            };
            std::set<Error> errorList;

            // �������� ������� � ��������, ����������� ������������ ��� ����������
            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            // ���������, ��� ������� ������� nullptr
            Assert::IsNull(result);

            // ���������, ��� � ������ ������ ���� ������ invalidVariableName
            bool hasError = false;
            for (const auto& error : errorList) {
                if (error.type == Error::invalidVariableName && error.position == 0) {
                    hasError = true;
                    break;
                }
            }
            Assert::IsTrue(hasError, L"������ invalidVariableName �� ��������� � ������ ������");
        }

        /**
         * @brief ���� 9: �������� ��������� ����������
         *
         * ������� ������ ��������� ������� ������ � ����������� ���� ����������
         */
        TEST_METHOD(Implication)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "a", 0),
                Token(TokenType::Variable, "b", 1),
                Token(TokenType::Implication, ">", 2)
            };
            std::set<Error> errorList;

            // �������� ������� � �������� ��� ����������
            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            // ���������, ��� ������� ������� �� nullptr
            Assert::IsNotNull(result);

            // ���������, ��� ��� ���� - ����������
            Assert::AreEqual(static_cast<int>(TokenType::Implication), static_cast<int>(result->type));

            // ���������, ��� � ���������� ���� ����� � ������ ��������
            Assert::IsNotNull(result->left);
            Assert::IsNotNull(result->right);

            // ���������, ��� ����� ������� - ���������� "a"
            Assert::AreEqual(static_cast<int>(TokenType::Variable), static_cast<int>(result->left->type));
            Assert::AreEqual(std::string("a"), result->left->value);

            // ���������, ��� ������ ������� - ���������� "b"
            Assert::AreEqual(static_cast<int>(TokenType::Variable), static_cast<int>(result->right->type));
            Assert::AreEqual(std::string("b"), result->right->value);

            // ���������, ��� ������ ������ ����
            Assert::IsTrue(errorList.empty());

            // ����������� ������
            delete result;
        }

        /**
         * @brief ���� 10: �������� ��������� ���������������
         *
         * ������� ������ ��������� ������� ������ � ���������������� ���� ����������
         */
        TEST_METHOD(Equivalence)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "a", 0),
                Token(TokenType::Variable, "b", 1),
                Token(TokenType::Equivalence, "~", 2)
            };
            std::set<Error> errorList;

            // �������� ������� � �������� ��� ���������������
            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            // ���������, ��� ������� ������� �� nullptr
            Assert::IsNotNull(result);

            // ���������, ��� ��� ���� - ���������������
            Assert::AreEqual(static_cast<int>(TokenType::Equivalence), static_cast<int>(result->type));

            // ���������, ��� � ��������������� ���� ����� � ������ ��������
            Assert::IsNotNull(result->left);
            Assert::IsNotNull(result->right);

            // ���������, ��� ����� ������� - ���������� "a"
            Assert::AreEqual(static_cast<int>(TokenType::Variable), static_cast<int>(result->left->type));
            Assert::AreEqual(std::string("a"), result->left->value);

            // ���������, ��� ������ ������� - ���������� "b"
            Assert::AreEqual(static_cast<int>(TokenType::Variable), static_cast<int>(result->right->type));
            Assert::AreEqual(std::string("b"), result->right->value);

            // ���������, ��� ������ ������ ����
            Assert::IsTrue(errorList.empty());

            // ����������� ������
            delete result;
        }

        /**
         * @brief ���� 11: �������� ��������� ������������� ��������
         *
         * ������� ������ ��������� ������� ������ ��� ��������� � ����������� ����������
         */
        TEST_METHOD(MultipleOperations)
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

            // �������� ������� � �������� ��� ������������� ��������
            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            // ���������, ��� ������� ������� �� nullptr
            Assert::IsNotNull(result);

            // ���������, ��� ��� ���� - ����������
            Assert::AreEqual(static_cast<int>(TokenType::Or), static_cast<int>(result->type));

            // ���������, ��� � ���������� ���� ����� � ������ ��������
            Assert::IsNotNull(result->left);
            Assert::IsNotNull(result->right);

            // ���������, ��� ����� ������� - ����������
            Assert::AreEqual(static_cast<int>(TokenType::And), static_cast<int>(result->left->type));

            // ���������, ��� ������ ������� - ���������� "d"
            Assert::AreEqual(static_cast<int>(TokenType::Variable), static_cast<int>(result->right->type));
            Assert::AreEqual(std::string("d"), result->right->value);

            // ���������, ��� ������ ������ ����
            Assert::IsTrue(errorList.empty());

            // ����������� ������
            delete result;
        }

        /**
         * @brief ���� 12: �������� ��������� ������ � �������� ���������
         *
         * ������� ������ �������� ������ unsupportedOperation ��� ���������������� ��������
         */
        TEST_METHOD(ErrorInMiddleOfExpression)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "a", 0),
                Token(TokenType::Variable, "b", 1),
                Token(TokenType::Any, "+", 2),
                Token(TokenType::Variable, "c", 3),
                Token(TokenType::And, "&", 4)
            };
            std::set<Error> errorList;

            // �������� ������� � ��������, ����������� ���������������� ��������
            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            // ���������, ��� ������� ������� nullptr
            Assert::IsNull(result);

            // ���������, ��� � ������ ������ ���� ������ unsupportedOperation
            bool hasError = false;
            for (const auto& error : errorList) {
                if (error.type == Error::unsupportedOperation && error.position == 2) {
                    hasError = true;
                    break;
                }
            }
            Assert::IsTrue(hasError, L"������ unsupportedOperation �� ��������� � ������ ������");
        }

        /**
         * @brief ���� 13: �������� ��������� ��������� ��� ������� ����������
         *
         * ������� ������ ��������� ������� ������ � ���������� �������� ���������
         */
        TEST_METHOD(NegationOverComplexExpression)
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

            // �������� ������� � �������� ��� ��������� ��� ������� ����������
            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            // ���������, ��� ������� ������� �� nullptr
            Assert::IsNotNull(result);

            // ���������, ��� ��� ���� - ���������
            Assert::AreEqual(static_cast<int>(TokenType::Not), static_cast<int>(result->type));

            // ���������, ��� � ��������� ���� �������
            Assert::IsNotNull(result->left);

            // ���������, ��� ������� - ����������
            Assert::AreEqual(static_cast<int>(TokenType::Or), static_cast<int>(result->left->type));

            // ���������, ��� ������ ������ ����
            Assert::IsTrue(errorList.empty());

            // ����������� ������
            delete result;
        }

        /**
         * @brief ���� 14: �������� ��������� �������� ���������
         *
         * ������� ������ ��������� ������� ������ � ������� ���������� ����������
         */
        TEST_METHOD(DoubleNegation)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "a", 0),
                Token(TokenType::Not, "!", 1),
                Token(TokenType::Not, "!", 2)
            };
            std::set<Error> errorList;

            // �������� ������� � �������� ��� �������� ���������
            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            // ���������, ��� ������� ������� �� nullptr
            Assert::IsNotNull(result);

            // ���������, ��� ��� ���� - ���������
            Assert::AreEqual(static_cast<int>(TokenType::Not), static_cast<int>(result->type));

            // ���������, ��� � ��������� ���� �������
            Assert::IsNotNull(result->left);

            // ���������, ��� ������� - ���������
            Assert::AreEqual(static_cast<int>(TokenType::Not), static_cast<int>(result->left->type));

            // ���������, ��� � ���������� ��������� ���� �������
            Assert::IsNotNull(result->left->left);

            // ���������, ��� ������� ���������� ��������� - ���������� "a"
            Assert::AreEqual(static_cast<int>(TokenType::Variable), static_cast<int>(result->left->left->type));
            Assert::AreEqual(std::string("a"), result->left->left->value);

            // ���������, ��� ������ ������ ����
            Assert::IsTrue(errorList.empty());

            // ����������� ������
            delete result;
        }

        /**
         * @brief ���� 15: �������� ��������� ��������� ���������
         *
         * ������� ������ ��������� ������� ������ � ���������� �����������
         */
        TEST_METHOD(NestedNegations)
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

            // �������� ������� � �������� ��� ��������� ���������
            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            // ���������, ��� ������� ������� �� nullptr
            Assert::IsNotNull(result);

            // ���������, ��� ��� ���� - ���������
            Assert::AreEqual(static_cast<int>(TokenType::Not), static_cast<int>(result->type));

            // ���������, ��� ������ ������ ����
            Assert::IsTrue(errorList.empty());

            // ����������� ������
            delete result;
        }

        /**
         * @brief ���� 16: �������� ��������� ���������� � ������������ ���������
         *
         * ������� ������ �������� ������ invalidVariableChar ��� ���������� � ������������ ���������
         */
        TEST_METHOD(VariableWithForbiddenChars)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "a_!", 0),
                Token(TokenType::Variable, "b", 1),
                Token(TokenType::And, "&", 2)
            };
            std::set<Error> errorList;

            // �������� ������� � ��������, ����������� ���������� � ������������ ���������
            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            // ���������, ��� ������� ������� nullptr
            Assert::IsNull(result);

            // ���������, ��� � ������ ������ ���� ������ invalidVariableChar
            bool hasError = false;
            for (const auto& error : errorList) {
                if (error.type == Error::invalidVariableChar && error.position == 0) {
                    hasError = true;
                    break;
                }
            }
            Assert::IsTrue(hasError, L"������ invalidVariableChar �� ��������� � ������ ������");
        }

        /**
         * @brief ���� 17: �������� ��������� ������ �������� ��������
         *
         * ������� ������ �������� ������ insufficientOperands ��� ������ �������� ��������
         */
        TEST_METHOD(OnlyBinaryOperations)
        {
            std::vector<Token> tokens = {
                Token(TokenType::And, "&", 0),
                Token(TokenType::Implication, ">", 1),
                Token(TokenType::Equivalence, "~", 2)
            };
            std::set<Error> errorList;

            // �������� ������� � ��������, ����������� ������ �������� ��������
            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            // ���������, ��� ������� ������� nullptr
            Assert::IsNull(result);

            // ���������, ��� � ������ ������ ���� ������ insufficientOperands ��� ������ ��������
            int errorCount = 0;
            for (const auto& error : errorList) {
                if (error.type == Error::insufficientOperands) {
                    errorCount++;
                }
            }
            Assert::AreEqual(3, errorCount, L"�������� ���������� ������ insufficientOperands");
        }

        /**
         * @brief ���� 18: �������� ��������� ���������� ����� ������
         *
         * ������� ������ �������� ������ ������ ����� ��� ������� ���������� �������
         */
        TEST_METHOD(MultipleErrorTypes)
        {
            std::vector<Token> tokens = {
                Token(TokenType::Variable, "a_!", 0),
                Token(TokenType::And, "&", 1)
            };
            std::set<Error> errorList;

            // �������� ������� � ��������, ����������� ��������� ����� ������
            ExpressionNode* result = buildExpressionTree(tokens, errorList);

            // ���������, ��� ������� ������� nullptr
            Assert::IsNull(result);

            // ���������, ��� � ������ ������ ���� ������ invalidVariableChar
            bool hasInvalidCharError = false;
            for (const auto& error : errorList) {
                if (error.type == Error::invalidVariableChar && error.position == 0) {
                    hasInvalidCharError = true;
                    break;
                }
            }
            Assert::IsTrue(hasInvalidCharError, L"������ invalidVariableChar �� ��������� � ������ ������");

            // ���������, ��� � ������ ������ ���� ������ insufficientOperands
            bool hasInsufficientOperandsError = false;
            for (const auto& error : errorList) {
                if (error.type == Error::insufficientOperands && error.position == 1) {
                    hasInsufficientOperandsError = true;
                    break;
                }
            }
            Assert::IsTrue(hasInsufficientOperandsError, L"������ insufficientOperands �� ��������� � ������ ������");
        }
    };
}