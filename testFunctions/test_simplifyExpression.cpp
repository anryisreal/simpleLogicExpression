#include "pch.h"
#include "CppUnitTest.h"
#include "../simpleLogicExpression/functions.h"
#include "../simpleLogicExpression/objects.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace testSimplifyExpression
{
    TEST_CLASS(testSimplifyExpression)
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

            return compareExpressionTrees(expected->left, actual->left, path + "-left") &&
                compareExpressionTrees(expected->right, actual->right, path + "-right");
        }

        /**
         * @brief ���� 1: ��������� ���������� (������ ����� �� �������)
         */
        TEST_METHOD(Test1_NegationOfConjunction)
        {
            // ������� ������: !(a & b)
            ExpressionNode* input = new ExpressionNode(Not);
            input->left = new ExpressionNode(And);
            input->left->left = new ExpressionNode(Variable, "a");
            input->left->right = new ExpressionNode(Variable, "b");

            // ��������� ���������: !a | !b
            ExpressionNode* expected = new ExpressionNode(Or);
            expected->left = new ExpressionNode(Not);
            expected->left->left = new ExpressionNode(Variable, "a");
            expected->right = new ExpressionNode(Not);
            expected->right->left = new ExpressionNode(Variable, "b");

            bool changed = simplifyExpression(input);
            Assert::IsTrue(changed, L"��������� ��������� ���������");
            Assert::IsTrue(compareExpressionTrees(expected, input), L"������������ ���������� ������� ������ �� �������");

            delete input;
            delete expected;
        }

        /**
         * @brief ���� 2: ��������� ���������� (������ ����� �� �������)
         */
        TEST_METHOD(Test2_NegationOfDisjunction)
        {
            // ������� ������: !(a | b)
            ExpressionNode* input = new ExpressionNode(Not);
            input->left = new ExpressionNode(Or);
            input->left->left = new ExpressionNode(Variable, "a");
            input->left->right = new ExpressionNode(Variable, "b");

            // ��������� ���������: !a & !b
            ExpressionNode* expected = new ExpressionNode(And);
            expected->left = new ExpressionNode(Not);
            expected->left->left = new ExpressionNode(Variable, "a");
            expected->right = new ExpressionNode(Not);
            expected->right->left = new ExpressionNode(Variable, "b");

            bool changed = simplifyExpression(input);
            Assert::IsTrue(changed, L"��������� ��������� ���������");
            Assert::IsTrue(compareExpressionTrees(expected, input), L"������������ ���������� ������� ������ �� �������");

            delete input;
            delete expected;
        }

        /**
         * @brief ���� 3: ��� �������������� (����������)
         */
        TEST_METHOD(Test3_NoTransformAnd)
        {
            // ������� ������: a & b
            ExpressionNode* input = new ExpressionNode(And);
            input->left = new ExpressionNode(Variable, "a");
            input->right = new ExpressionNode(Variable, "b");

            // ��������� ���������: a & b (��� ���������)
            ExpressionNode* expected = new ExpressionNode(And);
            expected->left = new ExpressionNode(Variable, "a");
            expected->right = new ExpressionNode(Variable, "b");

            bool changed = simplifyExpression(input);
            Assert::IsFalse(changed, L"�� ��������� ��������� ���������");
            Assert::IsTrue(compareExpressionTrees(expected, input), L"���������� �� ������ ���� ����������");

            delete input;
            delete expected;
        }

        /**
         * @brief ���� 4: ��� �������������� (����������)
         */
        TEST_METHOD(Test4_NoTransformOr)
        {
            // ������� ������: a | b
            ExpressionNode* input = new ExpressionNode(Or);
            input->left = new ExpressionNode(Variable, "a");
            input->right = new ExpressionNode(Variable, "b");

            // ��������� ���������: a | b (��� ���������)
            ExpressionNode* expected = new ExpressionNode(Or);
            expected->left = new ExpressionNode(Variable, "a");
            expected->right = new ExpressionNode(Variable, "b");

            bool changed = simplifyExpression(input);
            Assert::IsFalse(changed, L"�� ��������� ��������� ���������");
            Assert::IsTrue(compareExpressionTrees(expected, input), L"���������� �� ������ ���� ����������");

            delete input;
            delete expected;
        }

        /**
         * @brief ���� 5: ��� �������������� (����������)
         */
        TEST_METHOD(Test5_NoTransformImplication)
        {
            // ������� ������: a > b
            ExpressionNode* input = new ExpressionNode(Implication);
            input->left = new ExpressionNode(Variable, "a");
            input->right = new ExpressionNode(Variable, "b");

            // ��������� ���������: a > b (��� ���������)
            ExpressionNode* expected = new ExpressionNode(Implication);
            expected->left = new ExpressionNode(Variable, "a");
            expected->right = new ExpressionNode(Variable, "b");

            bool changed = simplifyExpression(input);
            Assert::IsFalse(changed, L"�� ��������� ��������� ���������");
            Assert::IsTrue(compareExpressionTrees(expected, input), L"���������� �� ������ ���� ����������");

            delete input;
            delete expected;
        }

        /**
         * @brief ���� 6: ��� �������������� (������������)
         */
        TEST_METHOD(Test6_NoTransformEquivalence)
        {
            // ������� ������: a ~ b
            ExpressionNode* input = new ExpressionNode(Equivalence);
            input->left = new ExpressionNode(Variable, "a");
            input->right = new ExpressionNode(Variable, "b");

            // ��������� ���������: a ~ b (��� ���������)
            ExpressionNode* expected = new ExpressionNode(Equivalence);
            expected->left = new ExpressionNode(Variable, "a");
            expected->right = new ExpressionNode(Variable, "b");

            bool changed = simplifyExpression(input);
            Assert::IsFalse(changed, L"�� ��������� ��������� ���������");
            Assert::IsTrue(compareExpressionTrees(expected, input), L"������������ �� ������ ���� ����������");

            delete input;
            delete expected;
        }

        /**
         * @brief ���� 7: ������ ����� ������ ���������
         */
        TEST_METHOD(Test7_FirstLawInsideNegation)
        {
            // ������� ������: !(Not(And("a", "b")))
            ExpressionNode* input = new ExpressionNode(Not);
            input->left = new ExpressionNode(Not);
            input->left->left = new ExpressionNode(And);
            input->left->left->left = new ExpressionNode(Variable, "a");
            input->left->left->right = new ExpressionNode(Variable, "b");

            // ��������� ���������: !(Or(Not("a"), Not("b")))
            ExpressionNode* expected = new ExpressionNode(Not);
            expected->left = new ExpressionNode(Or);
            expected->left->left = new ExpressionNode(Not);
            expected->left->left->left = new ExpressionNode(Variable, "a");
            expected->left->right = new ExpressionNode(Not);
            expected->left->right->left = new ExpressionNode(Variable, "b");

            bool changed = simplifyExpression(input);
            Assert::IsTrue(changed, L"��������� ��������� ���������");
            Assert::IsTrue(compareExpressionTrees(expected, input), L"������������ ��������������");

            delete input;
            delete expected;
        }
        /**
         * @brief ���� 8: ������ ����� ������ ���������
         *
         * ��������� �������������� !(Not(Or("a", "b"))) � !(And(Not("a"), Not("b")))
         */
        TEST_METHOD(Test8_SecondLawInsideNegation)
        {
            // ������� ������: !(Not(Or("a", "b")))
            ExpressionNode* input = new ExpressionNode(Not);
            input->left = new ExpressionNode(Not);
            input->left->left = new ExpressionNode(Or);
            input->left->left->left = new ExpressionNode(Variable, "a");
            input->left->left->right = new ExpressionNode(Variable, "b");

            // ��������� ���������: !(And(Not("a"), Not("b")))
            ExpressionNode* expected = new ExpressionNode(Not);
            expected->left = new ExpressionNode(And);
            expected->left->left = new ExpressionNode(Not);
            expected->left->left->left = new ExpressionNode(Variable, "a");
            expected->left->right = new ExpressionNode(Not);
            expected->left->right->left = new ExpressionNode(Variable, "b");

            bool changed = simplifyExpression(input);
            Assert::IsTrue(changed, L"��������� ��������� ���������");
            Assert::IsTrue(compareExpressionTrees(expected, input),
                L"������������ ���������� ������� ������ �� ������� ������ ���������");

            delete input;
            delete expected;
        }

        /**
         * @brief ���� 9: ������ ����� ������ ������� ������
         */
        TEST_METHOD(Test9_FirstLawInsideSecondLaw)
        {
            // ������� ������: !(Or(And("a", "b"), "c"))
            ExpressionNode* input = new ExpressionNode(Not);
            input->left = new ExpressionNode(Or);
            input->left->left = new ExpressionNode(And);
            input->left->left->left = new ExpressionNode(Variable, "a");
            input->left->left->right = new ExpressionNode(Variable, "b");
            input->left->right = new ExpressionNode(Variable, "c");

            // ��������� ���������: And(Or(Not("a"), Not("b")), Not("c"))
            ExpressionNode* expected = new ExpressionNode(And);
            expected->left = new ExpressionNode(Or);
            expected->left->left = new ExpressionNode(Not);
            expected->left->left->left = new ExpressionNode(Variable, "a");
            expected->left->right = new ExpressionNode(Not);
            expected->left->right->left = new ExpressionNode(Variable, "b");
            expected->right = new ExpressionNode(Not);
            expected->right->left = new ExpressionNode(Variable, "c");

            bool changed = simplifyExpression(input);
            Assert::IsTrue(changed, L"��������� ��������� ���������");
            Assert::IsTrue(compareExpressionTrees(expected, input), L"������������ ��������������");

            delete input;
            delete expected;
        }

        /**
         * @brief ���� 10: ������ ����� ������ ������� ������
         */
        TEST_METHOD(Test10_SecondLawInsideFirstLaw)
        {
            // ������� ������: !(And(Or("a", "b"), "c"))
            ExpressionNode* input = new ExpressionNode(Not);
            input->left = new ExpressionNode(And);
            input->left->left = new ExpressionNode(Or);
            input->left->left->left = new ExpressionNode(Variable, "a");
            input->left->left->right = new ExpressionNode(Variable, "b");
            input->left->right = new ExpressionNode(Variable, "c");

            // ��������� ���������: Or(And(Not("a"), Not("b")), Not("c"))
            ExpressionNode* expected = new ExpressionNode(Or);
            expected->left = new ExpressionNode(And);
            expected->left->left = new ExpressionNode(Not);
            expected->left->left->left = new ExpressionNode(Variable, "a");
            expected->left->right = new ExpressionNode(Not);
            expected->left->right->left = new ExpressionNode(Variable, "b");
            expected->right = new ExpressionNode(Not);
            expected->right->left = new ExpressionNode(Variable, "c");

            bool changed = simplifyExpression(input);
            Assert::IsTrue(changed, L"��������� ��������� ���������");
            Assert::IsTrue(compareExpressionTrees(expected, input), L"������������ ��������������");

            delete input;
            delete expected;
        }

        /**
         * @brief ���� 11: ���������������� ������ ������
         */
        TEST_METHOD(Test11_SequentialFirstLaws)
        {
            // ������� ������: !(And(Not(And("a", "b")), "c"))
            ExpressionNode* input = new ExpressionNode(Not);
            input->left = new ExpressionNode(And);
            input->left->left = new ExpressionNode(Not);
            input->left->left->left = new ExpressionNode(And);
            input->left->left->left->left = new ExpressionNode(Variable, "a");
            input->left->left->left->right = new ExpressionNode(Variable, "b");
            input->left->right = new ExpressionNode(Variable, "c");

            // ��������� ���������: Or(Or(Not("a"), Not("b")), Not("c"))
            ExpressionNode* expected = new ExpressionNode(Or);
            expected->left = new ExpressionNode(Or);
            expected->left->left = new ExpressionNode(Not);
            expected->left->left->left = new ExpressionNode(Variable, "a");
            expected->left->right = new ExpressionNode(Not);
            expected->left->right->left = new ExpressionNode(Variable, "b");
            expected->right = new ExpressionNode(Not);
            expected->right->left = new ExpressionNode(Variable, "c");

            bool changed = simplifyExpression(input);
            Assert::IsTrue(changed, L"��������� ��������� ���������");
            Assert::IsTrue(compareExpressionTrees(expected, input), L"������������ ��������������");

            delete input;
            delete expected;
        }

        /**
         * @brief ���� 12: ���������������� ������ ������
         */
        TEST_METHOD(Test12_SequentialSecondLaws)
        {
            // ������� ������: !(Or(Not(Or("a", "b")), "c"))
            ExpressionNode* input = new ExpressionNode(Not);
            input->left = new ExpressionNode(Or);
            input->left->left = new ExpressionNode(Not);
            input->left->left->left = new ExpressionNode(Or);
            input->left->left->left->left = new ExpressionNode(Variable, "a");
            input->left->left->left->right = new ExpressionNode(Variable, "b");
            input->left->right = new ExpressionNode(Variable, "c");

            // ��������� ���������: And(And(Not("a"), Not("b")), Not("c"))
            ExpressionNode* expected = new ExpressionNode(And);
            expected->left = new ExpressionNode(And);
            expected->left->left = new ExpressionNode(Not);
            expected->left->left->left = new ExpressionNode(Variable, "a");
            expected->left->right = new ExpressionNode(Not);
            expected->left->right->left = new ExpressionNode(Variable, "b");
            expected->right = new ExpressionNode(Not);
            expected->right->left = new ExpressionNode(Variable, "c");

            bool changed = simplifyExpression(input);
            Assert::IsTrue(changed, L"��������� ��������� ���������");
            Assert::IsTrue(compareExpressionTrees(expected, input), L"������������ ��������������");

            delete input;
            delete expected;
        }

        /**
         * @brief ���� 13: ���������������� ������ � ������ ������
         */
        TEST_METHOD(Test13_SequentialFirstAndSecondLaws)
        {
            // ������� ������: !(And(Not(Or("a", "b")), Not(And("c", "d"))))
            ExpressionNode* input = new ExpressionNode(Not);
            input->left = new ExpressionNode(And);
            input->left->left = new ExpressionNode(Not);
            input->left->left->left = new ExpressionNode(Or);
            input->left->left->left->left = new ExpressionNode(Variable, "a");
            input->left->left->left->right = new ExpressionNode(Variable, "b");
            input->left->right = new ExpressionNode(Not);
            input->left->right->left = new ExpressionNode(And);
            input->left->right->left->left = new ExpressionNode(Variable, "c");
            input->left->right->left->right = new ExpressionNode(Variable, "d");

            // ��������� ���������: Or(Or("a", "b"), And("c", "d"))
            ExpressionNode* expected = new ExpressionNode(Or);
            expected->left = new ExpressionNode(Or);
            expected->left->left = new ExpressionNode(Variable, "a");
            expected->left->right = new ExpressionNode(Variable, "b");
            expected->right = new ExpressionNode(And);
            expected->right->left = new ExpressionNode(Variable, "c");
            expected->right->right = new ExpressionNode(Variable, "d");

            bool changed = simplifyExpression(input);
            Assert::IsTrue(changed, L"��������� ��������� ���������");
            Assert::IsTrue(compareExpressionTrees(expected, input), L"������������ ��������������");

            delete input;
            delete expected;
        }

        /**
         * @brief ���� 14: ���������� ��������� �������� ���������
         */
        TEST_METHOD(Test14_PreserveComplexStructure)
        {
            // ������� ������: And("a", Or("b", "c"))
            ExpressionNode* input = new ExpressionNode(And);
            input->left = new ExpressionNode(Variable, "a");
            input->right = new ExpressionNode(Or);
            input->right->left = new ExpressionNode(Variable, "b");
            input->right->right = new ExpressionNode(Variable, "c");

            // ��������� ���������: And("a", Or("b", "c")) (��� ���������)
            ExpressionNode* expected = new ExpressionNode(And);
            expected->left = new ExpressionNode(Variable, "a");
            expected->right = new ExpressionNode(Or);
            expected->right->left = new ExpressionNode(Variable, "b");
            expected->right->right = new ExpressionNode(Variable, "c");

            bool changed = simplifyExpression(input);
            Assert::IsFalse(changed, L"�� ��������� ��������� ���������");
            Assert::IsTrue(compareExpressionTrees(expected, input), L"��������� �� ������ ���� ����������");

            delete input;
            delete expected;
        }
    };
}