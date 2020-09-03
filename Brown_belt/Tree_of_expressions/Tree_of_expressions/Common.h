//
//  Common.h
//  Tree_of_expressions
//
//  Created by Andrew Kireev on 03.09.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//

#ifndef Common_h
#define Common_h

#include <memory>
#include <string>

// Базовый класс арифметического выражения
class Expression {
public:
  virtual ~Expression() = default;

    // Вычисляет значение выражения
  virtual int Evaluate() const = 0;

    // Форматирует выражение как строку
    // Каждый узел берётся в скобки, независимо от приоритета
  virtual std::string ToString() const = 0;
};

using ExpressionPtr = std::unique_ptr<Expression>;


class ValueExression : public Expression {
public:
    int Get_value() const {
        return value_;
    }
    
private:
    int value_;
};


class OperationExression : public Expression {
public:
    
protected:
    std::unique_ptr<ValueExression> left_son_;
    std::unique_ptr<ValueExression> right_son_;
};


class MinusExression : public OperationExression {
public:
    int Evaluate() const {
        return left_son_->Get_value() -
        right_son_->Get_value();
    }

};

class PlusExression : public OperationExression {
public:
    int Evaluate() const {
        return left_son_->Get_value() +
        right_son_->Get_value();
    }

};

class DivExression : public OperationExression {
public:
    int Evaluate() const {
        return left_son_->Get_value() /
        right_son_->Get_value();
    }

};

class MultExression : public OperationExression {
public:
    int Evaluate() const {
        return left_son_->Get_value() *
        right_son_->Get_value();
    }

};

// Функции для формирования выражения
ExpressionPtr Value(int value) {
    return
}
ExpressionPtr Sum(ExpressionPtr left, ExpressionPtr right);
ExpressionPtr Product(ExpressionPtr left, ExpressionPtr right);

#endif /* Common_h */
