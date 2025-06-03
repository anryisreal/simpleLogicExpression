#include <string>
#include <iostream>
#pragma once


/**
 * ������������ ����� ������� ����������� ���������.
 *
 * �������� ��������� ���� �������: ����������, �������� � ����������� ��������.
 */
enum TokenType {
    Variable,    /// ����������
    Not,         /// ��������� (!)
    And,         /// ���������� (&)
    Or,          /// ���������� (|)
    Implication, /// ���������� (>)
    Equivalence, /// ��������������� (~)
    Any          /// ����������� ��� ��� ������ �� ������������ �������
};

/**
 * ����� ��� ������������� ������ ����������� ���������.
 *
 * �������� ��� ������, ��� ��������� �������� � ������� � �������� ������.
 */
class Token {
public:
    TokenType type;     ///< ��� ������
    std::string value;  ///< ��������� �������� ������
    int position;       ///< ������� ������ � �������� ������

    /**
     * ����������� ������ Token.
     * @param t ��� ������
     * @param v ��������� ��������
     * @param pos ������� � ������
     */
    Token(TokenType t, const std::string& v, int pos) : type(t), value(v), position(pos) {}
};

/**
 * ����� ��� ���� ������ ���������.
 *
 * ������������ ���� � �������� ������ ����������� ���������.
 */
class ExpressionNode {
public:
    TokenType type;             /// ��� ��������/����������
    std::string value;          /// �������� ����������
    ExpressionNode* left;       /// ��������� �� ����� ���������
    ExpressionNode* right;      /// ��������� �� ������ ���������

    /**
     * ����������� ������ ExpressionNode.
     * @param t ��� ����
     * @param v �������� ���������� (�� ��������� ������ ������)
     */
    ExpressionNode(TokenType t, const std::string& v = "") : type(t), value(v), left(nullptr), right(nullptr) {}

    /**
    * ����������� ������ ExpressionNode
    * @param t ��� ����
    * @param l ����� �������
    * @param r ������� �������
    */
    ExpressionNode(TokenType t, ExpressionNode* l, ExpressionNode* r) : type(t), value(""), left(l), right(r) {}

    /**
    * ����������� ������ ExpressionNode
    * @param t ��� ����
    * @param v �������� ����������
    * @param l ����� �������
    * @param r ������� �������
    */
    ExpressionNode(TokenType t, const std::string& v, ExpressionNode* l, ExpressionNode* r) : type(t), value(v), left(l), right(r) {}

    /// ���������� (���������� ������� ����������)
    ~ExpressionNode() {
        delete left;
        delete right;
    }
};

/**
 * ����� ��� ��������� ������ ���������.
 *
 * �������� ��� ������, ������� � ��������.
 */
class Error {
public:
    /**
     * ������������ ����� ������.
     */
    enum ErrorType {
        inputFile,            /// ������ �������� �����
        outputFile,           /// ������ ��������� �����
        invalidSeparator,     /// ������������ �����������
        missingOperation,     /// ��������� ��������
        insufficientOperands, /// ������������ ���������
        invalidVariableName,  /// ������������ ��� ����������
        invalidVariableChar,  /// ������������ ������ � ����� ����������
        unsupportedOperation /// ���������������� ��������
    };

    static std::string getErrorTypeString(ErrorType type) {
        switch (type) {
        case inputFile: return "inputFile";
        case outputFile: return "outputFile";
        case invalidSeparator: return "invalidSeparator";
        case missingOperation: return "missingOperation";
        case insufficientOperands: return "insufficientOperands";
        case invalidVariableName: return "invalidVariableName";
        case invalidVariableChar: return "insufficientOperands";
        case unsupportedOperation: return "invalidVariableName";
        default: return "����������� ������";
        }
    }

    ErrorType type;          /// ��� ������
    int position;            /// ������� ������
    std::string description; /// �������� ������

    /**
     * ����������� ������ Error.
     * @param t ��� ������
     * @param pos ������� ������ (�� ��������� -1)
     */
    Error(ErrorType t, int pos = -1) : type(t), position(pos) {
        switch (type) {
        case inputFile:
            description = "������� ������ ���� � �������� �������. ��������, ���� �� ����������.";
            break;
        case outputFile:
            description = "������� ������ ���� ��� �������� ������. ��������, ���������� ������������ �� ���������� ��� ��� ���� �� ������.";
            break;
        case invalidSeparator:
            description = "������������ ����������� (������� " + std::to_string(position) + ").";
            break;
        case missingOperation:
            description = "�� ������� ������ �������� ���� �������� ����� ���������� (������� " + std::to_string(position) + ").";
            break;
        case insufficientOperands:
            description = "�� ������� ������ �������� ������� ��� �������� (������� " + std::to_string(position) + ").";
            break;
        case invalidVariableName:
            description = "�� ������� ������ ��� ���������� (������� " + std::to_string(position) + ") �����������. ��� ���������� ������ ���������� ������ � ������� ���������� ��������.";
            break;
        case invalidVariableChar:
            description = "�� ������� ������ ��� ���������� (������� " + std::to_string(position) + ") �����������. ��� ��������� ������ ��������� ������ ������� ���������� �������� � �����.";
            break;
        case unsupportedOperation:
            description = "�� ������� ������ ������� ���������������� ���������� �������� (������� " + std::to_string(position) + ").";
            break;
        default:
            description = "����������� ������.";
        }
    }

    /**
     * ������� ��������� �� ������ � ����������� �����.
     */
    void message() const {
        std::cout << description.c_str() << std::endl;
    }

    /**
    * �������� ��������� ��� ������������� � std::set
    */
    bool operator<(const Error& other) const {
        if (type != other.type)
            return type < other.type;
        return position < other.position;
    }

    /**
    * �������� ��������� ��� ��������� ������ � std::set
    */
    bool operator==(const Error& other) const {
        return type == other.type && position == other.position;
    }
};