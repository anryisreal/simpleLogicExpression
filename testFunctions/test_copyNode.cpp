#include "pch.h"
#include "CppUnitTest.h"
#include "../simpleLogicExpression/functions.h"
#include "../simpleLogicExpression/objects.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace testcopyNode
{
    TEST_CLASS(testcopyNode)
    {
    private:
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

    public:
        /**
         * @brief ���� 1: ����� ������� ����
         * ���������, ��� ������� ���������� nullptr ��� ����������� nullptr
         */
        TEST_METHOD(Test1_CopyNullNode)
        {
            ExpressionNode* node = nullptr;
            ExpressionNode* copiedNode = copyNode(node);
            Assert::IsNull(copiedNode);
        }

        /**
         * @brief ���� 2: ����� ����������
         * ��������� �������� ����� ���� ���� Variable � ��������� "x"
         */
        TEST_METHOD(Test2_CopyVariableNode)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::Variable, "x");
            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief ���� 3: ����� ���������
         * ��������� �������� ����� ���� ���� Not ��� ���������
         */
        TEST_METHOD(Test3_CopyNotNode)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::Not);
            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief ���� 4: ����� ����������
         * ��������� �������� ����� ���� ���� And ��� ���������
         */
        TEST_METHOD(Test4_CopyAndNode)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::And);
            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief ���� 5: ����� ����������
         * ��������� �������� ����� ���� ���� Or ��� ���������
         */
        TEST_METHOD(Test5_CopyOrNode)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::Or);
            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief ���� 6: ����� ����������
         * ��������� �������� ����� ���� ���� Implication ��� ���������
         */
        TEST_METHOD(Test6_CopyImplicationNode)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::Implication);
            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief ���� 7: ����� ������������
         * ��������� �������� ����� ���� ���� Equivalence ��� ���������
         */
        TEST_METHOD(Test7_CopyEquivalenceNode)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::Equivalence);
            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief ���� 8: ����� ��������� � ���������
         * ��������� �������� ����� ���� Not � ����� ��������� "a"
         */
        TEST_METHOD(Test8_CopyNotWithOperand)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::Not);
            node->left = new ExpressionNode(TokenType::Variable, "a");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief ���� 9: ����� ���������� � ����� ���������
         * ��������� �������� ����� ���� And � ����� ��������� "a"
         */
        TEST_METHOD(Test9_CopyAndWithLeftOperand)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::And);
            node->left = new ExpressionNode(TokenType::Variable, "a");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief ���� 10: ����� ���������� � ������ ���������
         * ��������� �������� ����� ���� And � ������ ��������� "a"
         */
        TEST_METHOD(Test10_CopyAndWithRightOperand)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::And);
            node->right = new ExpressionNode(TokenType::Variable, "a");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief ���� 11: ����� ���������� � ����� ����������
         * ��������� �������� ����� ���� And � ���������� "a" � "b"
         */
        TEST_METHOD(Test11_CopyAndWithTwoOperands)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::And);
            node->left = new ExpressionNode(TokenType::Variable, "a");
            node->right = new ExpressionNode(TokenType::Variable, "b");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief ���� 12: ����� ���������� � ����� ���������
         * ��������� �������� ����� ���� Or � ����� ��������� "a"
         */
        TEST_METHOD(Test12_CopyOrWithLeftOperand)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::Or);
            node->left = new ExpressionNode(TokenType::Variable, "a");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief ���� 13: ����� ���������� � ������ ���������
         * ��������� �������� ����� ���� Or � ������ ��������� "a"
         */
        TEST_METHOD(Test13_CopyOrWithRightOperand)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::Or);
            node->right = new ExpressionNode(TokenType::Variable, "a");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief ���� 14: ����� ���������� � ����� ����������
         * ��������� �������� ����� ���� Or � ���������� "a" � "b"
         */
        TEST_METHOD(Test14_CopyOrWithTwoOperands)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::Or);
            node->left = new ExpressionNode(TokenType::Variable, "a");
            node->right = new ExpressionNode(TokenType::Variable, "b");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief ���� 15: ����� ���������� � ����� ���������
         * ��������� �������� ����� ���� Implication � ����� ��������� "a"
         */
        TEST_METHOD(Test15_CopyImplicationWithLeftOperand)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::Implication);
            node->left = new ExpressionNode(TokenType::Variable, "a");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief ���� 16: ����� ���������� � ������ ���������
         * ��������� �������� ����� ���� Implication � ������ ��������� "a"
         */
        TEST_METHOD(Test16_CopyImplicationWithRightOperand)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::Implication);
            node->right = new ExpressionNode(TokenType::Variable, "a");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief ���� 17: ����� ���������� � ����� ����������
         * ��������� �������� ����� ���� Implication � ���������� "a" � "b"
         */
        TEST_METHOD(Test17_CopyImplicationWithTwoOperands)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::Implication);
            node->left = new ExpressionNode(TokenType::Variable, "a");
            node->right = new ExpressionNode(TokenType::Variable, "b");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief ���� 18: ����� ������������ � ����� ���������
         * ��������� �������� ����� ���� Equivalence � ����� ��������� "a"
         */
        TEST_METHOD(Test18_CopyEquivalenceWithLeftOperand)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::Equivalence);
            node->left = new ExpressionNode(TokenType::Variable, "a");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief ���� 19: ����� ������������ � ������ ���������
         * ��������� �������� ����� ���� Equivalence � ������ ��������� "a"
         */
        TEST_METHOD(Test19_CopyEquivalenceWithRightOperand)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::Equivalence);
            node->right = new ExpressionNode(TokenType::Variable, "a");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief ���� 20: ����� ������������ � ����� ����������
         * ��������� �������� ����� ���� Equivalence � ���������� "a" � "b"
         */
        TEST_METHOD(Test20_CopyEquivalenceWithTwoOperands)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::Equivalence);
            node->left = new ExpressionNode(TokenType::Variable, "a");
            node->right = new ExpressionNode(TokenType::Variable, "b");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief ���� 21: ����� ��������� �������� �����
         * ��������� �������� ����� ���� � ��������� ��������� And �����
         */
        TEST_METHOD(Test21_CopyNestedOperationLeft)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::And);
            node->left = new ExpressionNode(TokenType::And);
            node->left->left = new ExpressionNode(TokenType::Variable, "a");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief ���� 22: ����� ��������� �������� ������
         * ��������� �������� ����� ���� � ��������� ��������� And ������
         */
        TEST_METHOD(Test22_CopyNestedOperationRight)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::And);
            node->left = new ExpressionNode(TokenType::Variable, "a");
            node->right = new ExpressionNode(TokenType::And);

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief ���� 23: ����� ��������� �������� ����� � ������
         * ��������� �������� ����� ���� � ���������� ���������� And � ����� ������
         */
        TEST_METHOD(Test23_CopyNestedOperationBothSides)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::And);
            node->left = new ExpressionNode(TokenType::And);
            node->right = new ExpressionNode(TokenType::And);

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief ���� 24: ����� ���������� � ������ ���������
         * ��������� �������� ����� ���� Variable � ������ ������� � �������� ��������
         */
        TEST_METHOD(Test24_CopyVariableWithEmptyValue)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::Variable, "");
            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief ���� 25: ����� ��������� �������� ����� (� ����������)
         * ��������� �������� ����� ���� � ��������� ��������� And �����, ���������� ��������
         */
        TEST_METHOD(Test25_CopyNestedLeftOperationWithOperands)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::And);
            node->left = new ExpressionNode(TokenType::And);
            node->left->left = new ExpressionNode(TokenType::Variable, "a");
            node->left->right = new ExpressionNode(TokenType::Variable, "b");
            node->right = new ExpressionNode(TokenType::Variable, "");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief ���� 26: ����� ��������� �������� ������ (� ����������)
         * ��������� �������� ����� ���� � ��������� ��������� And ������, ���������� ��������
         */
        TEST_METHOD(Test26_CopyNestedRightOperationWithOperands)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::And);
            node->left = new ExpressionNode(TokenType::Variable, "");
            node->right = new ExpressionNode(TokenType::And);
            node->right->left = new ExpressionNode(TokenType::Variable, "a");
            node->right->right = new ExpressionNode(TokenType::Variable, "b");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief ���� 27: ����� ��������� �������� ����� � ������ (� ����������)
         * ��������� �������� ����� ���� � ���������� ���������� And � ����� ������, ����������� ��������
         */
        TEST_METHOD(Test27_CopyNestedOperationsBothSidesWithOperands)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::And);
            node->left = new ExpressionNode(TokenType::And);
            node->left->left = new ExpressionNode(TokenType::Variable, "a");
            node->left->right = new ExpressionNode(TokenType::Variable, "b");
            node->right = new ExpressionNode(TokenType::And);
            node->right->left = new ExpressionNode(TokenType::Variable, "c");
            node->right->right = new ExpressionNode(TokenType::Variable, "d");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief ���� 28: ����� ����������� ��������� ��������
         * ��������� �������� ����� ���� � ������������ ���������� ���������� (Or ����� � Variable ������)
         */
        TEST_METHOD(Test28_CopyHeterogeneousNestedOperation)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::And);
            node->left = new ExpressionNode(TokenType::Or);
            node->left->left = new ExpressionNode(TokenType::Variable, "a");
            node->left->right = new ExpressionNode(TokenType::Variable, "b");
            node->right = new ExpressionNode(TokenType::Variable, "a");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
         * @brief ���� 29: ����� � ����������� ���� ����� ��������
         * ��������� �������� ����� �������� ������ � ����������� ���� ����� ��������
         */
        TEST_METHOD(Test29_CopyWithAllOperationTypesCombination)
        {
            // ������ ������: And(Or(a, Not(b)), Implication(Equivalence(c, d), e))
            ExpressionNode* node = new ExpressionNode(TokenType::And);

            // ����� ���������: Or(a, Not(b))
            node->left = new ExpressionNode(TokenType::Or);
            node->left->left = new ExpressionNode(TokenType::Variable, "a");
            node->left->right = new ExpressionNode(TokenType::Not);
            node->left->right->left = new ExpressionNode(TokenType::Variable, "b");

            // ������ ���������: Implication(Equivalence(c, d), e)
            node->right = new ExpressionNode(TokenType::Implication);
            node->right->left = new ExpressionNode(TokenType::Equivalence);
            node->right->left->left = new ExpressionNode(TokenType::Variable, "c");
            node->right->left->right = new ExpressionNode(TokenType::Variable, "d");
            node->right->right = new ExpressionNode(TokenType::Variable, "e");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }

        /**
        * @brief ���� 30: �������� ������
        */
        TEST_METHOD(Test29_CopyWithAllOperationTypesCombination)
        {
            ExpressionNode* node = new ExpressionNode(TokenType::And);

            node->left = new ExpressionNode(TokenType::Or);
            node->left->left = new ExpressionNode(TokenType::And);
            node->left->left->left = new ExpressionNode(TokenType::Or);
            node->left->left->left->left = new ExpressionNode(TokenType::Variable, "a");
            node->left->left->left->right = new ExpressionNode(TokenType::Variable, "b");

            node->left->left->right = new ExpressionNode(TokenType::Variable, "a");

            node->left->right = new ExpressionNode(TokenType::Or);
            node->left->right->left = new ExpressionNode(TokenType::Variable, "c");
            node->left->right->left = new ExpressionNode(TokenType::Variable, "d");

            node->right = new ExpressionNode(TokenType::Variable, "a");

            ExpressionNode* copiedNode = copyNode(node);

            AssertNodesEqual(node, copiedNode);

            delete node;
            delete copiedNode;
        }


    };
}