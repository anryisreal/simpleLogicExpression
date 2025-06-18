/**
 * @file objects.h
 * @brief Обьекты и структуры данных для программы.
 */

// -*- coding: utf-8 -*-
#include <string>
#include <iostream>
#pragma once

/**
 * @brief Перечисление типов токенов логического выражения.
 *
 * Определяет возможные типы токенов, используемых в логических выражениях.
 */
enum TokenType {
    Variable,    ///< Переменная (например, A, B, x).
    Not,         ///< Операция отрицания (!).
    And,         ///< Операция конъюнкции (&).
    Or,          ///< Операция дизъюнкции (|).
    Implication, ///< Операция импликации (>).
    Equivalence, ///< Операция эквивалентности (~).
    Any          ///< Специальный тип для неиспользуемых токенов.
};

/**
 * @brief Класс для представления токена логического выражения.
 *
 * Хранит информацию о типе токена, его строковом представлении и позиции в исходной строке.
 */
class Token {
public:
    TokenType type;     ///< Тип токена (переменная или операция).
    std::string value;  ///< Строковое значение токена.
    int position;       ///< Позиция токена в исходной строке.

    /**
     * @brief Конструктор класса Token.
     *
     * Инициализирует токен с указанным типом, значением и позицией.
     * @param t Тип токена.
     * @param v Строковое значение токена.
     * @param pos Позиция токена в строке.
     */
    Token(TokenType t, const std::string& v, int pos) : type(t), value(v), position(pos) {}
};

/**
 * @brief Класс для представления узла бинарного дерева логического выражения.
 *
 * Хранит тип узла, значение (для переменных) и указатели на левое и правое поддеревья.
 */
class ExpressionNode {
public:
    TokenType type;             ///< Тип узла (переменная или операция).
    std::string value;          ///< Значение переменной (для узлов типа Variable).
    ExpressionNode* left;       ///< Указатель на левое поддерево.
    ExpressionNode* right;      ///< Указатель на правое поддерево.

    /**
     * @brief Конструктор для узла с типом и значением.
     *
     * Создает узел с указанным типом и значением, поддеревья инициализируются как nullptr.
     * @param t Тип узла.
     * @param v Значение переменной (по умолчанию пустая строка).
     */
    ExpressionNode(TokenType t, const std::string& v = "") : type(t), value(v), left(nullptr), right(nullptr) {}

    /**
     * @brief Конструктор для узла с типом и поддеревьями.
     *
     * Создает узел с указанным типом и указателями на левое и правое поддеревья.
     * @param t Тип узла.
     * @param l Указатель на левое поддерево.
     * @param r Указатель на правое поддерево.
     */
    ExpressionNode(TokenType t, ExpressionNode* l, ExpressionNode* r) : type(t), value(""), left(l), right(r) {}

    /**
     * @brief Конструктор для узла с типом, значением и поддеревьями.
     *
     * Создает узел с указанным типом, значением и указателями на поддеревья.
     * @param t Тип узла.
     * @param v Значение переменной.
     * @param l Указатель на левое поддерево.
     * @param r Указатель на правое поддерево.
     */
    ExpressionNode(TokenType t, const std::string& v, ExpressionNode* l, ExpressionNode* r) : type(t), value(v), left(l), right(r) {}

    /**
     * @brief Деструктор класса ExpressionNode.
     *
     * Рекурсивно освобождает память, занятую поддеревьями.
     */
    ~ExpressionNode() {
        delete left;
        delete right;
    }
};

/**
 * @brief Класс для обработки ошибок программы.
 *
 * Хранит информацию об ошибке: ее тип, позицию в строке и описание.
 */
class Error {
public:
    /**
     * @brief Перечисление типов ошибок.
     *
     * Определяет возможные типы ошибок, возникающих при обработке логических выражений.
     */
    enum ErrorType {
        inputFile,            ///< Ошибка при открытии входного файла.
        outputFile,           ///< Ошибка при открытии выходного файла.
        invalidSeparator,     ///< Некорректные разделители в выражении.
        missingOperation,     ///< Пропущена операция между операндами.
        insufficientOperands, ///< Недостаточно операндов для операции.
        invalidVariableName,  ///< Некорректное имя переменной.
        invalidVariableChar,  ///< Некорректный символ в имени переменной.
        unsupportedOperation, ///< Неподдерживаемая логическая операция.
        emptyFile             ///< Отсутствует выражение во входном файле.
    };

    /**
     * @brief Возвращает строковое представление типа ошибки.
     *
     * Преобразует тип ошибки в читаемую строку для вывода в сообщениях.
     * @param type Тип ошибки.
     * @return Строковое представление типа ошибки.
     */
    static std::string getErrorTypeString(ErrorType type) {
        switch (type) {
        case inputFile: return "inputFile";
        case outputFile: return "outputFile";
        case invalidSeparator: return "invalidSeparator";
        case missingOperation: return "missingOperation";
        case insufficientOperands: return "insufficientOperands";
        case invalidVariableName: return "invalidVariableName";
        case invalidVariableChar: return "invalidVariableChar";
        case unsupportedOperation: return "unsupportedOperation";
        case emptyFile:            return "emptyFile";
        default: return "Неизвестная ошибка";
        }
    }

    ErrorType type;          ///< Тип ошибки.
    int position;            ///< Позиция ошибки в строке (-1, если позиция не применима).
    std::string description; ///< Описание ошибки.

    /**
     * @brief Конструктор класса Error.
     *
     * Инициализирует ошибку с указанным типом и позицией, формируя соответствующее описание.
     * @param t Тип ошибки.
     * @param pos Позиция ошибки в строке (по умолчанию -1).
     */
    Error(ErrorType t, int pos = -1) : type(t), position(pos) {
        switch (type) {
        case inputFile:
            description = "Неверно указан файл с входными данными. Возможно, файл не существует.";
            break;
        case outputFile:
            description = "Неверно указан файл для выходных данных. Возможно, указанного расположения не существует или нет прав на запись.";
            break;
        case invalidSeparator:
            description = "Некорректные разделители (позиция " + std::to_string(position) + ").";
            break;
        case missingOperation:
            description = "Во входной строке пропущен знак операции между операндами (позиция " + std::to_string(position) + ").";
            break;
        case insufficientOperands:
            description = "Во входной строке пропущен операнд для операции (позиция " + std::to_string(position) + ").";
            break;
        case invalidVariableName:
            description = "Во входной строке имя переменной (позиция " + std::to_string(position) + ") некорректно. Имя переменной должно начинаться только с символа латинского алфавита.";
            break;
        case invalidVariableChar:
            description = "Во входной строке имя переменной (позиция " + std::to_string(position) + ") некорректно. Имя переменной должно содержать только символы латинского алфавита и цифры.";
            break;
        case unsupportedOperation:
            description = "Во входной строке указана неподдерживаемая логическая операция (позиция " + std::to_string(position) + ").";
            break;
        case emptyFile:
            description = "Отсутствует выражение во входном файле.";
            break;
        default:
            description = "Неизвестная ошибка.";
        }
    }

    /**
     * @brief Выводит сообщение об ошибке.
     *
     * Выводит описание ошибки в стандартный поток вывода.
     */
    void message() const {
        std::wcout << description.c_str() << std::endl;
    }

    /**
     * @brief Оператор сравнения для использования в std::set.
     *
     * Сравнивает ошибки по типу и позиции для упорядочивания в множестве.
     * @param other Другая ошибка для сравнения.
     * @return true, если текущая ошибка меньше другой.
     */
    bool operator<(const Error& other) const {
        if (type != other.type)
            return type < other.type;
        return position < other.position;
    }

    /**
     * @brief Оператор равенства для сравнения ошибок.
     *
     * Проверяет, равны ли две ошибки по типу и позиции.
     * @param other Другая ошибка для сравнения.
     * @return true, если ошибки идентичны.
     */
    bool operator==(const Error& other) const {
        return type == other.type && position == other.position;
    }
};