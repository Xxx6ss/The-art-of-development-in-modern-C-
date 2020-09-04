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
    int Get_value = 0;
};

using ExpressionPtr = std::unique_ptr<Expression>;




#endif /* Common_h */

