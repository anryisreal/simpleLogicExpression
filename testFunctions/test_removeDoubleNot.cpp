#include "pch.h"
#include "CppUnitTest.h"
#include "../simpleLogicExpression/functions.h"
#include "../simpleLogicExpression/objects.h"
#include "testFunctions.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace testRemoveDoubleNot
{
    TEST_CLASS(testRemoveDoubleNot)
    {
    public:
        /**
         * @brief ���� 1: ��� ���������
         */
        TEST_METHOD(Test2_NoNegation)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Variable, "a");
            removeDoubleNot(input);

            ExpressionNode* expected = new ExpressionNode(TokenType::Variable, "a");

            Assert::IsTrue(compareExpressionTrees(expected, input));

            delete input;
            delete expected;
        }

        /**
         * @brief ���� 2: ��������� ���������
         * @details ���������, ��� ������� �� �������� ������ � ����� ����������
         */
        TEST_METHOD(Test2_SingleNegation)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Not);
            input->left = new ExpressionNode(TokenType::Variable, "a");
            removeDoubleNot(input);

            ExpressionNode* expected = new ExpressionNode(TokenType::Not);
            expected->left = new ExpressionNode(TokenType::Variable, "a");

            Assert::IsTrue(compareExpressionTrees(expected, input));

            delete input;
            delete expected;
        }

        /**
         * @brief ���� 3: ������� ���������
         * @details ��������� �������� �������� ���������
         */
        TEST_METHOD(Test3_DoubleNegation)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Not);
            input->left = new ExpressionNode(TokenType::Not);
            input->left->left = new ExpressionNode(TokenType::Variable, "a");
            removeDoubleNot(input);

            ExpressionNode* expected = new ExpressionNode(TokenType::Variable, "a");

            Assert::IsTrue(compareExpressionTrees(expected, input));

            delete input;
            delete expected;
        }

        /**
         * @brief ���� 4: ������� ���������
         * @details ��������� �������������� �������� ��������� � ���������
         */
        TEST_METHOD(Test4_TripleNegation)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Not);
            input->left = new ExpressionNode(TokenType::Not);
            input->left->left = new ExpressionNode(TokenType::Not);
            input->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            removeDoubleNot(input);

            ExpressionNode* expected = new ExpressionNode(TokenType::Not);
            expected->left = new ExpressionNode(TokenType::Variable, "a");

            Assert::IsTrue(compareExpressionTrees(expected, input));

            delete input;
            delete expected;
        }

        /**
         * @brief ���� 5: ��������� ��������� ����� ���������� And
         * @details ���������, ��� ��������� ��������� ����� ���������� �� ����������
         */
        TEST_METHOD(Test5_SingleNegationBeforeAnd)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Not);
            input->left = new ExpressionNode(TokenType::And);
            input->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->left->right = new ExpressionNode(TokenType::Variable, "b");
            removeDoubleNot(input);

            ExpressionNode* expected = new ExpressionNode(TokenType::Not);
            expected->left = new ExpressionNode(TokenType::And);
            expected->left->left = new ExpressionNode(TokenType::Variable, "a");
            expected->left->right = new ExpressionNode(TokenType::Variable, "b");

            Assert::IsTrue(compareExpressionTrees(expected, input));

            delete input;
            delete expected;
        }

        /**
         * @brief ���� 6: ������� ��������� ����� ���������� And
         * @details ��������� �������� �������� ��������� ����� ����������
         */
        TEST_METHOD(Test6_DoubleNegationBeforeAnd)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Not);
            input->left = new ExpressionNode(TokenType::Not);
            input->left->left = new ExpressionNode(TokenType::And);
            input->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->left->left->right = new ExpressionNode(TokenType::Variable, "b");
            removeDoubleNot(input);

            ExpressionNode* expected = new ExpressionNode(TokenType::And);
            expected->left = new ExpressionNode(TokenType::Variable, "a");
            expected->right = new ExpressionNode(TokenType::Variable, "b");

            Assert::IsTrue(compareExpressionTrees(expected, input));

            delete input;
            delete expected;
        }

        /**
         * @brief ���� 7: ��������� ������� ���������
         * @details ��������� ��������� ��������� ������� ���������
         */
        TEST_METHOD(Test7_NestedDoubleNegations)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Not);
            input->left = new ExpressionNode(TokenType::Not);
            input->left->left = new ExpressionNode(TokenType::And);
            input->left->left->left = new ExpressionNode(TokenType::Not);
            input->left->left->left->left = new ExpressionNode(TokenType::Not);
            input->left->left->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->left->left->right = new ExpressionNode(TokenType::Variable, "b");
            removeDoubleNot(input);

            ExpressionNode* expected = new ExpressionNode(TokenType::And);
            expected->left = new ExpressionNode(TokenType::Variable, "a");
            expected->right = new ExpressionNode(TokenType::Variable, "b");

            Assert::IsTrue(compareExpressionTrees(expected, input));

            delete input;
            delete expected;
        }

        /**
         * @brief ���� 8: ������� ��������� ������ ��������� Or
         * @details ��������� ��������� �������� ��������� ������ ����������
         */
        TEST_METHOD(Test8_DoubleNegationInsideOr)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Or);
            input->left = new ExpressionNode(TokenType::Not);
            input->left->left = new ExpressionNode(TokenType::Not);
            input->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->right = new ExpressionNode(TokenType::Variable, "b");
            removeDoubleNot(input);

            ExpressionNode* expected = new ExpressionNode(TokenType::Or);
            expected->left = new ExpressionNode(TokenType::Variable, "a");
            expected->right = new ExpressionNode(TokenType::Variable, "b");

            Assert::IsTrue(compareExpressionTrees(expected, input));

            delete input;
            delete expected;
        }

        /**
         * @brief ���� 9: ������� ��������� ������ ��������� And
         * @details ��������� ��������� �������� ��������� ������ ����������
         */
        TEST_METHOD(Test9_DoubleNegationInsideAnd)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::And);
            input->left = new ExpressionNode(TokenType::Not);
            input->left->left = new ExpressionNode(TokenType::Not);
            input->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->right = new ExpressionNode(TokenType::Variable, "b");
            removeDoubleNot(input);

            ExpressionNode* expected = new ExpressionNode(TokenType::And);
            expected->left = new ExpressionNode(TokenType::Variable, "a");
            expected->right = new ExpressionNode(TokenType::Variable, "b");

            Assert::IsTrue(compareExpressionTrees(expected, input));

            delete input;
            delete expected;
        }

        /**
         * @brief ���� 10: ������� ��������� ������ ��������� Implication
         * @details ��������� ��������� �������� ��������� ������ ����������
         */
        TEST_METHOD(Test10_DoubleNegationInsideImplication)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Implication);
            input->left = new ExpressionNode(TokenType::Not);
            input->left->left = new ExpressionNode(TokenType::Not);
            input->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->right = new ExpressionNode(TokenType::Variable, "b");
            removeDoubleNot(input);

            ExpressionNode* expected = new ExpressionNode(TokenType::Implication);
            expected->left = new ExpressionNode(TokenType::Variable, "a");
            expected->right = new ExpressionNode(TokenType::Variable, "b");

            Assert::IsTrue(compareExpressionTrees(expected, input));

            delete input;
            delete expected;
        }

        /**
         * @brief ���� 11: ������� ��������� � ��������� Equivalence
         * @details ��������� ��������� �������� ��������� ������ ���������������
         */
        TEST_METHOD(Test11_DoubleNegationInEquivalence)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Equivalence);
            input->left = new ExpressionNode(TokenType::Not);
            input->left->left = new ExpressionNode(TokenType::Not);
            input->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->right = new ExpressionNode(TokenType::Variable, "b");
            removeDoubleNot(input);

            ExpressionNode* expected = new ExpressionNode(TokenType::Equivalence);
            expected->left = new ExpressionNode(TokenType::Variable, "a");
            expected->right = new ExpressionNode(TokenType::Variable, "b");

            Assert::IsTrue(compareExpressionTrees(expected, input));

            delete input;
            delete expected;
        }

        /**
         * @brief ���� 12: ������� ��������� � ����� ��������� Or
         * @details ��������� ��������� ������� ��������� � ����� ��������� ����������
         */
        TEST_METHOD(Test12_DoubleNegationInBothOperandsOr)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Or);
            input->left = new ExpressionNode(TokenType::Not);
            input->left->left = new ExpressionNode(TokenType::Not);
            input->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->right = new ExpressionNode(TokenType::Not);
            input->right->left = new ExpressionNode(TokenType::Not);
            input->right->left->left = new ExpressionNode(TokenType::Variable, "b");
            removeDoubleNot(input);

            ExpressionNode* expected = new ExpressionNode(TokenType::Or);
            expected->left = new ExpressionNode(TokenType::Variable, "a");
            expected->right = new ExpressionNode(TokenType::Variable, "b");

            Assert::IsTrue(compareExpressionTrees(expected, input));

            delete input;
            delete expected;
        }

        /**
         * @brief ���� 13: ������� ��������� � ����� ��������� And
         * @details ��������� ��������� ������� ��������� � ����� ��������� ����������
         */
        TEST_METHOD(Test13_DoubleNegationInBothOperandsAnd)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::And);
            input->left = new ExpressionNode(TokenType::Not);
            input->left->left = new ExpressionNode(TokenType::Not);
            input->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->right = new ExpressionNode(TokenType::Not);
            input->right->left = new ExpressionNode(TokenType::Not);
            input->right->left->left = new ExpressionNode(TokenType::Variable, "b");
            removeDoubleNot(input);

            ExpressionNode* expected = new ExpressionNode(TokenType::And);
            expected->left = new ExpressionNode(TokenType::Variable, "a");
            expected->right = new ExpressionNode(TokenType::Variable, "b");

            Assert::IsTrue(compareExpressionTrees(expected, input));

            delete input;
            delete expected;
        }

        /**
         * @brief ���� 14: ������� ��������� � ����� ��������� Implication
         * @details ��������� ��������� ������� ��������� � ����� ��������� ����������
         */
        TEST_METHOD(Test14_DoubleNegationInBothOperandsImplication)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Implication);
            input->left = new ExpressionNode(TokenType::Not);
            input->left->left = new ExpressionNode(TokenType::Not);
            input->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->right = new ExpressionNode(TokenType::Not);
            input->right->left = new ExpressionNode(TokenType::Not);
            input->right->left->left = new ExpressionNode(TokenType::Variable, "b");
            removeDoubleNot(input);

            ExpressionNode* expected = new ExpressionNode(TokenType::Implication);
            expected->left = new ExpressionNode(TokenType::Variable, "a");
            expected->right = new ExpressionNode(TokenType::Variable, "b");

            Assert::IsTrue(compareExpressionTrees(expected, input));

            delete input;
            delete expected;
        }

        /**
         * @brief ���� 15: ������� ��������� � ����� ��������� Equivalence
         * @details ��������� ��������� ������� ��������� � ����� ��������� ���������������
         */
        TEST_METHOD(Test15_DoubleNegationInBothOperandsEquivalence)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Equivalence);
            input->left = new ExpressionNode(TokenType::Not);
            input->left->left = new ExpressionNode(TokenType::Not);
            input->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->right = new ExpressionNode(TokenType::Not);
            input->right->left = new ExpressionNode(TokenType::Not);
            input->right->left->left = new ExpressionNode(TokenType::Variable, "b");
            removeDoubleNot(input);

            ExpressionNode* expected = new ExpressionNode(TokenType::Equivalence);
            expected->left = new ExpressionNode(TokenType::Variable, "a");
            expected->right = new ExpressionNode(TokenType::Variable, "b");

            Assert::IsTrue(compareExpressionTrees(expected, input));

            delete input;
            delete expected;
        }

        /**
         * @brief ���� 16: ��������� ������ ������� ���������
         * @details ��������� ��������� ��������� ������ ������� ��������� ��� �������� ���������
         */
        TEST_METHOD(Test16_NegationInsideNegation)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Not);
            input->left = new ExpressionNode(TokenType::Or);
            input->left->left = new ExpressionNode(TokenType::Not);
            input->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->left->right = new ExpressionNode(TokenType::Variable, "b");
            removeDoubleNot(input);

            ExpressionNode* expected = new ExpressionNode(TokenType::Not);
            expected->left = new ExpressionNode(TokenType::Or);
            expected->left->left = new ExpressionNode(TokenType::Not);
            expected->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            expected->left->right = new ExpressionNode(TokenType::Variable, "b");

            Assert::IsTrue(compareExpressionTrees(expected, input));

            delete input;
            delete expected;
        }

        /**
         * @brief ���� 17: ����������� ������ � ���������� �����������
         * @details ��������� ��������� ������������ ��������� � ���������� �����������
         */
        TEST_METHOD(Test17_ComplexCase)
        {
            ExpressionNode* input = new ExpressionNode(TokenType::Not);
            input->left = new ExpressionNode(TokenType::Not);
            input->left->left = new ExpressionNode(TokenType::Implication);
            input->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            input->left->left->right = new ExpressionNode(TokenType::Not);
            input->left->left->right->left = new ExpressionNode(TokenType::Not);
            input->left->left->right->left->left = new ExpressionNode(TokenType::Variable, "b");
            removeDoubleNot(input);

            ExpressionNode* expected = new ExpressionNode(TokenType::Implication);
            expected->left = new ExpressionNode(TokenType::Variable, "a");
            expected->right = new ExpressionNode(TokenType::Variable, "b");

            Assert::IsTrue(compareExpressionTrees(expected, input));

            delete input;
            delete expected;
        }
    };
}