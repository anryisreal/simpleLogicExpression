#include "pch.h"
#include "CppUnitTest.h"
#include "../simpleLogicExpression/functions.h"
#include "../simpleLogicExpression/objects.h"
#include "testFunctions.cpp"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace testExpressionTreeToInfix
{
    TEST_CLASS(testExpressionTreeToInfix)
    {
    public:
        /**
         * @brief ���� 1: ������� ����������
         * @details ��������� �������������� ������ � ����� ����������
         */
        TEST_METHOD(Test1_SimpleVariable)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Variable, "a");
            std::string result = expressionTreeToInfix(input);
            std::string expected = "a";

            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            expected.erase(std::remove(expected.begin(), expected.end(), ' '), expected.end());

            Assert::AreEqual(expected, result);

            delete input;
        }

        /**
         * @brief ���� 2: ���������
         * @details ��������� �������������� ������ � ����������
         */
        TEST_METHOD(Test2_Negation)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Not);
            input->left = new ExpressionNode(TokenType::Variable, "a");
            std::string result = expressionTreeToInfix(input);
            std::string expected = "!a";

            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            expected.erase(std::remove(expected.begin(), expected.end(), ' '), expected.end());

            Assert::AreEqual(expected, result);

            delete input;
        }

        /**
         * @brief ���� 3: ����������
         * @details ��������� �������������� ������ � �����������
         */
        TEST_METHOD(Test3_Conjunction)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::And);
            input->left = new ExpressionNode(TokenType::Variable, "a");
            input->right = new ExpressionNode(TokenType::Variable, "b");
            std::string result = expressionTreeToInfix(input);
            std::string expected = "a & b";

            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            expected.erase(std::remove(expected.begin(), expected.end(), ' '), expected.end());

            Assert::AreEqual(expected, result);

            delete input;
        }

        /**
         * @brief ���� 4: ����������
         * @details ��������� �������������� ������ � �����������
         */
        TEST_METHOD(Test4_Disjunction)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Or);
            input->left = new ExpressionNode(TokenType::Variable, "a");
            input->right = new ExpressionNode(TokenType::Variable, "b");
            std::string result = expressionTreeToInfix(input);
            std::string expected = "a || b";

            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            expected.erase(std::remove(expected.begin(), expected.end(), ' '), expected.end());

            Assert::AreEqual(expected, result);

            delete input;
        }

        /**
         * @brief ���� 5: ����������
         * @details ��������� �������������� ������ � �����������
         */
        TEST_METHOD(Test5_Implication)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Implication);
            input->left = new ExpressionNode(TokenType::Variable, "a");
            input->right = new ExpressionNode(TokenType::Variable, "b");
            std::string result = expressionTreeToInfix(input);
            std::string expected = "a -> b";

            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            expected.erase(std::remove(expected.begin(), expected.end(), ' '), expected.end());

            Assert::AreEqual(expected, result);

            delete input;
        }

        /**
         * @brief ���� 6: ���������������
         * @details ��������� �������������� ������ � ����������������
         */
        TEST_METHOD(Test6_Equivalence)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Equivalence);
            input->left = new ExpressionNode(TokenType::Variable, "a");
            input->right = new ExpressionNode(TokenType::Variable, "b");
            std::string result = expressionTreeToInfix(input);
            std::string expected = "a ~ b";

            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            expected.erase(std::remove(expected.begin(), expected.end(), ' '), expected.end());

            Assert::AreEqual(expected, result);

            delete input;
        }

        /**
         * @brief ���� 7: ��������� ��������
         * @details ��������� ������������ ����������� ������ �������� ���������� ��������
         */
        TEST_METHOD(Test7_OperationPriority)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::And);
            input->left = new ExpressionNode(TokenType::Or);
            input->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->left->right = new ExpressionNode(TokenType::Variable, "b");
            input->right = new ExpressionNode(TokenType::Or);
            input->right->left = new ExpressionNode(TokenType::Variable, "c");
            input->right->right = new ExpressionNode(TokenType::Variable, "d");
            std::string result = expressionTreeToInfix(input);
            std::string expected = "(a || b) & (c || d)";

            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            expected.erase(std::remove(expected.begin(), expected.end(), ' '), expected.end());

            Assert::AreEqual(expected, result);

            delete input;
        }

        /**
         * @brief ���� 8: ��������� ������
         * @details ��������� ������������ ��������� ��������� ������
         */
        TEST_METHOD(Test8_NestedParentheses)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Not);
            input->left = new ExpressionNode(TokenType::Or);
            input->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->left->right = new ExpressionNode(TokenType::And);
            input->left->right->left = new ExpressionNode(TokenType::Variable, "b");
            input->left->right->right = new ExpressionNode(TokenType::Variable, "c");
            std::string result = expressionTreeToInfix(input);
            std::string expected = "!(a || (b & c))";

            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            expected.erase(std::remove(expected.begin(), expected.end(), ' '), expected.end());

            Assert::AreEqual(expected, result);

            delete input;
        }

        /**
         * @brief ���� 9: ��������� ������������ � ����������
         * @details ��������� ������������ ����������� ������ ��� �������� � ������ �����������
         */
        TEST_METHOD(Test9_PriorityOfEquivalenceAndImplication)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::And);
            input->left = new ExpressionNode(TokenType::Implication);
            input->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->left->right = new ExpressionNode(TokenType::Variable, "b");
            input->right = new ExpressionNode(TokenType::Equivalence);
            input->right->left = new ExpressionNode(TokenType::Variable, "c");
            input->right->right = new ExpressionNode(TokenType::Variable, "d");
            std::string result = expressionTreeToInfix(input);
            std::string expected = "(a -> b) & (c ~ d)";

            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            expected.erase(std::remove(expected.begin(), expected.end(), ' '), expected.end());

            Assert::AreEqual(expected, result);

            delete input;
        }

        /**
         * @brief ���� 10: ������� ��������� � ������ ��������
         * @details ��������� ������, ����� ����� ������� ����� ������� ���������
         */
        TEST_METHOD(Test10_HigherPriorityInLeftOperand)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Or);
            input->left = new ExpressionNode(TokenType::And);
            input->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->left->right = new ExpressionNode(TokenType::Variable, "b");
            input->right = new ExpressionNode(TokenType::Equivalence);
            input->right->left = new ExpressionNode(TokenType::Variable, "c");
            input->right->right = new ExpressionNode(TokenType::Variable, "d");
            std::string result = expressionTreeToInfix(input);
            std::string expected = "a & b || (c ~ d)";

            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            expected.erase(std::remove(expected.begin(), expected.end(), ' '), expected.end());

            Assert::AreEqual(expected, result);

            delete input;
        }

        /**
         * @brief ���� 11: ������� ��������� � ������� ��������
         * @details ��������� ������, ����� ������ ������� ����� ������� ���������
         */
        TEST_METHOD(Test11_HigherPriorityInRightOperand)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Or);
            input->left = new ExpressionNode(TokenType::Equivalence);
            input->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->left->right = new ExpressionNode(TokenType::Variable, "b");
            input->right = new ExpressionNode(TokenType::And);
            input->right->left = new ExpressionNode(TokenType::Variable, "c");
            input->right->right = new ExpressionNode(TokenType::Variable, "d");
            std::string result = expressionTreeToInfix(input);
            std::string expected = "(a ~ b) || c & d";

            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            expected.erase(std::remove(expected.begin(), expected.end(), ' '), expected.end());

            Assert::AreEqual(expected, result);

            delete input;
        }

        /**
         * @brief ���� 12: ���������� ���������
         * @details ��������� ������, ����� ��� �������� ����� ���������� ���������
         */
        TEST_METHOD(Test12_SamePriority)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::And);
            input->left = new ExpressionNode(TokenType::And);
            input->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->left->right = new ExpressionNode(TokenType::Variable, "b");
            input->right = new ExpressionNode(TokenType::And);
            input->right->left = new ExpressionNode(TokenType::Variable, "c");
            input->right->right = new ExpressionNode(TokenType::Variable, "d");
            std::string result = expressionTreeToInfix(input);
            std::string expected = "a & b & c & d";

            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            expected.erase(std::remove(expected.begin(), expected.end(), ' '), expected.end());

            Assert::AreEqual(expected, result);

            delete input;
        }

        /**
         * @brief ���� 13: ��������� ���������
         * @details ��������� ��������� ��������� ��������� � ������� ���������
         */
        TEST_METHOD(Test13_NestedNegation)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Not);
            input->left = new ExpressionNode(TokenType::Or);
            input->left->left = new ExpressionNode(TokenType::Not);
            input->left->left->left = new ExpressionNode(TokenType::And);
            input->left->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->left->left->left->right = new ExpressionNode(TokenType::Variable, "b");
            input->left->right = new ExpressionNode(TokenType::And);
            input->left->right->left = new ExpressionNode(TokenType::Variable, "c");
            input->left->right->right = new ExpressionNode(TokenType::Variable, "d");
            std::string result = expressionTreeToInfix(input);
            std::string expected = "!(!(a & b) || c & d)";

            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            expected.erase(std::remove(expected.begin(), expected.end(), ' '), expected.end());

            Assert::AreEqual(expected, result);

            delete input;
        }

        /**
         * @brief ���� 14: ��������� ��������� ����� ����������
         * @details ��������� ��������� ���������� ��������� ����� ����������
         */
        TEST_METHOD(Test14_MultipleNegationsBeforeVariable)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Not);
            input->left = new ExpressionNode(TokenType::Not);
            input->left->left = new ExpressionNode(TokenType::Variable, "a");
            std::string result = expressionTreeToInfix(input);
            std::string expected = "!!a";

            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            expected.erase(std::remove(expected.begin(), expected.end(), ' '), expected.end());

            Assert::AreEqual(expected, result);

            delete input;
        }

        /**
         * @brief ���� 15: ��������� ��������� ����� ���������� Or
         * @details ��������� ��������� ���������� ��������� ����� ���������� ����������
         */
        TEST_METHOD(Test15_MultipleNegationsBeforeOr)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Not);
            input->left = new ExpressionNode(TokenType::Not);
            input->left->left = new ExpressionNode(TokenType::Or);
            input->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->left->left->right = new ExpressionNode(TokenType::Variable, "b");
            std::string result = expressionTreeToInfix(input);
            std::string expected = "!!(a || b)";

            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            expected.erase(std::remove(expected.begin(), expected.end(), ' '), expected.end());

            Assert::AreEqual(expected, result);

            delete input;
        }

        /**
         * @brief ���� 16: ��������� ��������� ����� ���������� And
         * @details ��������� ��������� ���������� ��������� ����� ���������� ����������
         */
        TEST_METHOD(Test16_MultipleNegationsBeforeAnd)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Not);
            input->left = new ExpressionNode(TokenType::Not);
            input->left->left = new ExpressionNode(TokenType::And);
            input->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->left->left->right = new ExpressionNode(TokenType::Variable, "b");
            std::string result = expressionTreeToInfix(input);
            std::string expected = "!!(a & b)";

            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            expected.erase(std::remove(expected.begin(), expected.end(), ' '), expected.end());

            Assert::AreEqual(expected, result);

            delete input;
        }

        /**
         * @brief ���� 17: ��������� ��������� ����� ���������� Implication
         * @details ��������� ��������� ���������� ��������� ����� ���������� ����������
         */
        TEST_METHOD(Test17_MultipleNegationsBeforeImplication)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Not);
            input->left = new ExpressionNode(TokenType::Not);
            input->left->left = new ExpressionNode(TokenType::Implication);
            input->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->left->left->right = new ExpressionNode(TokenType::Variable, "b");
            std::string result = expressionTreeToInfix(input);
            std::string expected = "!!(a -> b)";

            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            expected.erase(std::remove(expected.begin(), expected.end(), ' '), expected.end());

            Assert::AreEqual(expected, result);

            delete input;
        }

        /**
         * @brief ���� 18: ��������� ��������� ����� ���������� Equivalence
         * @details ��������� ��������� ���������� ��������� ����� ���������� ���������������
         */
        TEST_METHOD(Test18_MultipleNegationsBeforeEquivalence)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Not);
            input->left = new ExpressionNode(TokenType::Not);
            input->left->left = new ExpressionNode(TokenType::Equivalence);
            input->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->left->left->right = new ExpressionNode(TokenType::Variable, "b");
            std::string result = expressionTreeToInfix(input);
            std::string expected = "!!(a ~ b)";

            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            expected.erase(std::remove(expected.begin(), expected.end(), ' '), expected.end());

            Assert::AreEqual(expected, result);

            delete input;
        }

        /**
         * @brief ���� 19: ��������� ������ � ������� ���������
         * @details ��������� ��������� ��������� ��� ������, ��� � ������� ���������
         */
        TEST_METHOD(Test19_NegationInsideAndOutsideOperator)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Not);
            input->left = new ExpressionNode(TokenType::And);
            input->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->left->right = new ExpressionNode(TokenType::Not);
            input->left->right->left = new ExpressionNode(TokenType::Variable, "b");
            std::string result = expressionTreeToInfix(input);
            std::string expected = "!(a & !b)";

            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            expected.erase(std::remove(expected.begin(), expected.end(), ' '), expected.end());

            Assert::AreEqual(expected, result);

            delete input;
        }
    };
}