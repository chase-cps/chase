/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        10/29/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once

#include "representation.hh"

/// @file Definition of factory functions.

namespace chase
{

    /// @brief Factory building a True boolean constant.
    /// @return A pointer to a new object representing a True boolean constant.
    std::shared_ptr<BooleanConstant> True();
    /// @brief Factory building a False boolean constant.
    /// @return A pointer to a new object representing a False boolean constant.
    std::shared_ptr<BooleanConstant> False();

    /// @brief Factory for the NOT operation.
    /// @param op The operator of the operation.
    /// @return A pointer to a new object representing the NOT operation.
    std::shared_ptr<UnaryBooleanFormula> Not( std::shared_ptr<LogicFormula> op );

    /// @brief Factory for the AND operation.
    /// @param op1 The first operator of the operation.
    /// @param op2 The second operator of the operation.
    /// @return A pointer to a new object representing the AND operation.
    std::shared_ptr<BinaryBooleanFormula> And( std::shared_ptr<LogicFormula> op1, std::shared_ptr<LogicFormula> op2);

    /// @brief Factory for the OR operation.
    /// @param op1 The first operator of the operation.
    /// @param op2 The second operator of the operation.
    /// @return A pointer to a new object representing the OR operation.
    std::shared_ptr<BinaryBooleanFormula> Or( std::shared_ptr<LogicFormula> op1, std::shared_ptr<LogicFormula> op2);

    /// @brief Factory for the Implies operation.
    /// @param op1 The first operator of the operation.
    /// @param op2 The second operator of the operation.
    /// @return A pointer to a new object representing the Implies operation.
    std::shared_ptr<BinaryBooleanFormula> Implies( std::shared_ptr<LogicFormula> op1, std::shared_ptr<LogicFormula> op2);

    /// @brief Factory for the double implication operation.
    /// @param op1 The first operator of the operation.
    /// @param op2 The second operator of the operation.
    /// @return A pointer to a new object representing the Implies operation.
    std::shared_ptr<BinaryBooleanFormula> Iff( std::shared_ptr<LogicFormula> op1, std::shared_ptr<LogicFormula> op2);

    /// @brief Factory for the NAND operation.
    /// @param op1 The first operator of the operation.
    /// @param op2 The second operator of the operation.
    /// @return A pointer to a new object representing the NAND operation.
    std::shared_ptr<BinaryBooleanFormula> Nand( std::shared_ptr<LogicFormula> op1, std::shared_ptr<LogicFormula> op2);

    /// @brief Factory for the Xor operation.
    /// @param op1 The first operator of the operation.
    /// @param op2 The second operator of the operation.
    /// @return A pointer to a new object representing the Xor operation.
    std::shared_ptr<BinaryBooleanFormula> Xor( std::shared_ptr<LogicFormula> op1, std::shared_ptr<LogicFormula> op2);

    /// @brief Factory for the NOR operation.
    /// @param op1 The first operator of the operation.
    /// @param op2 The second operator of the operation.
    /// @return A pointer to a new object representing the NOR operation.
    std::shared_ptr<BinaryBooleanFormula> Nor( std::shared_ptr<LogicFormula> op1, std::shared_ptr<LogicFormula> op2);

    /// @brief Factory for the XNOR operation.
    /// @param op1 The first operator of the operation.
    /// @param op2 The second operator of the operation.
    /// @return A pointer to a new object representing the XNOR operation.
    std::shared_ptr<BinaryBooleanFormula> Xnor( std::shared_ptr<LogicFormula> op1, std::shared_ptr<LogicFormula> op2);

    /// @brief Factory function creating a Large AND operation.
    /// @param formulas Vector of formulas to copy into the operation.
    /// @return A new object representing a Large AND operation.
    std::shared_ptr<LargeBooleanFormula> LargeAnd( std::vector< std::shared_ptr<LogicFormula> >& formulas );

    /// @brief Factory function creating a Large OR operation.
    /// @param formulas Vector of formulas to copy into the operation.
    /// @return A new object representing a Large OR operation.
    std::shared_ptr<LargeBooleanFormula> LargeOr( std::vector< std::shared_ptr<LogicFormula> >& formulas );

    /// @brief Factory for the Always logic formula.
    /// @param op The operator of the formula.
    /// @return A pointer to a new object representing the Always logic formula.
    std::shared_ptr<UnaryTemporalFormula> Always(std::shared_ptr<LogicFormula> op);

    /// @brief Factory for the Eventually logic formula.
    /// @param op The operator of the formula.
    /// @return A pointer to a new object representing the Eventually logic formula.
    std::shared_ptr<UnaryTemporalFormula> Eventually(std::shared_ptr<LogicFormula> op);

    /// @brief Factory for the Next formula.
    /// @param op The operator of the formula.
    /// @return A pointer to a new object representing the next temporal
    /// formula.
    std::shared_ptr<UnaryTemporalFormula> Next(std::shared_ptr<LogicFormula> op);

    /// @brief Factory for the Until formula.
    /// @param op1 The 1st operator of the formula.
    /// @param op2 The 2nd operator of the formula.
    /// @return A pointer to a new object representing the until temporal
    /// formula.
    std::shared_ptr<BinaryTemporalFormula> Until(std::shared_ptr<LogicFormula> op1, std::shared_ptr<LogicFormula> op2);

    /// @brief Factory for the representation of a sum.
    /// @param op1 The first operator of the operation.
    /// @param op2 The second operator of the operation.
    /// @return A pointer to a new object representing a sum.
    std::shared_ptr<Expression> Sum(std::shared_ptr<Value> op1, std::shared_ptr<Value> op2);

    /// @brief Factory for the representation of a subtraction.
    /// @param op1 The first operator of the operation.
    /// @param op2 The second operator of the operation.
    /// @return A pointer to a new object representing a subtraction.
    std::shared_ptr<Expression> Sub(std::shared_ptr<Value> op1, std::shared_ptr<Value> op2);

    /// @brief Factory for the representation of a multiplication.
    /// @param op1 The first operator of the operation.
    /// @param op2 The second operator of the operation.
    /// @return A pointer to a new object representing a multiplication.
    std::shared_ptr<Expression> Mult(std::shared_ptr<Value> op1, std::shared_ptr<Value> op2);

    /// @brief Factory for the representation of a division.
    /// @param op1 The first operator of the operation.
    /// @param op2 The second operator of the operation.
    /// @return A pointer to a new object representing a division.
    std::shared_ptr<Expression> Div(std::shared_ptr<Value> op1, std::shared_ptr<Value> op2);

    /// @brief Factory for the representation of a equality expression.
    /// @param op1 The first operator of the operation.
    /// @param op2 The second operator of the operation.
    /// @return A pointer to a new object representing a equality (op1 = op2).
    std::shared_ptr<Expression> Eq(std::shared_ptr<Value> op1, std::shared_ptr<Value> op2);

    /// @brief Factory for the representation of a less-equal expression.
    /// @param op1 The first operator of the operation.
    /// @param op2 The second operator of the operation.
    /// @return A pointer to a new object representing a less-equal (op1<=op2).
    std::shared_ptr<Expression> NEq(std::shared_ptr<Value> op1, std::shared_ptr<Value> op2);

    /// @brief Factory for the representation of a less-equal expression.
    /// @param op1 The first operator of the operation.
    /// @param op2 The second operator of the operation.
    /// @return A pointer to a new object representing a less-equal (op1<=op2).
    std::shared_ptr<Expression> LE(std::shared_ptr<Value> op1, std::shared_ptr<Value> op2);

    /// @brief Factory for the representation of a less-than expression.
    /// @param op1 The first operator of the operation.
    /// @param op2 The second operator of the operation.
    /// @return A pointer to a new object representing a less-than (op1<op2).
    std::shared_ptr<Expression> LT(std::shared_ptr<Value> op1, std::shared_ptr<Value> op2);

    /// @brief Factory for the representation of a greater-equal expression.
    /// @param op1 The first operator of the operation.
    /// @param op2 The second operator of the operation.
    /// @return A pointer to a new object representing a greater-equal
    /// (op1>=op2).
    std::shared_ptr<Expression> GE(std::shared_ptr<Value> op1, std::shared_ptr<Value> op2);

    /// @brief Factory for the representation of a greater-than expression.
    /// @param op1 The first operator of the operation.
    /// @param op2 The second operator of the operation.
    /// @return A pointer to a new object representing a greater-than (op1>op2).
    std::shared_ptr<Expression> GT(std::shared_ptr<Value> op1, std::shared_ptr<Value> op2);

    /// @brief Factory to create a proposition from a variable.
    /// @param var The variable to be used.
    /// @return The proposition being created.
    std::shared_ptr<Proposition> Prop(std::shared_ptr<Variable> var);

    /// @brief Factory to create a proposition from an expression.
    /// @param exp The expression to be used.
    /// @return The proposition being created.
    std::shared_ptr<Proposition> Prop(std::shared_ptr<Expression> exp);

    /// @brief Factory to create an Identifier from a declaration.
    /// @param declaration The declaration of the Identifier.
    /// @return Pointer to the created identifier.
    std::shared_ptr<Identifier> Id(std::shared_ptr<DataDeclaration> declaration);

    /// @brief Factory to create an Integer Value.
    /// @param n The integer to be represented.
    /// @return Pointer to the created integer object.
    std::shared_ptr<IntegerValue> IntVal( int n );

    /// @brief Factory to create an Real Value.
    /// @param r The double number to be represented.
    /// @return Pointer to the created real value object.
    std::shared_ptr<RealValue> RealVal( double r );

    /// @brief Factory to create an Boolean Value.
    /// @param b The boolean value to be represented.
    /// @return Pointer to the created boolean value object.
    std::shared_ptr<BooleanValue> BoolVal( bool b );

    /// @brief Factory to create a Boolean type.
    /// @return Pointer to the boolean type object.
    std::shared_ptr< Boolean > Bool();

    /// @brief Factory to create a Integer type.
    /// @param l
    /// @param r
    /// @return Pointer to the integer type object.
    std::shared_ptr< Integer > Int( int l = -2147483647, int r = 2147483647);

    /// @brief Factory to create a Variable declaration.
    /// @param type Type of the variable.
    /// @param name Name of the variable.
    /// @param causality Causality of the variable.
    /// @return Pointer to the variable declaration object.
    std::shared_ptr< Variable > Var(std::shared_ptr< Type > type,
                                    std::shared_ptr< Name > name,
                                    causality_t causality = generic );

    /// @brief Factory to create a constant declaration.
    /// @param type Type of the constant.
    /// @param name Name of the constant.
    /// @param value Value of the constant.
    /// @return Pointer to the constant declaration object.
    std::shared_ptr< Constant > Const(std::shared_ptr< Type > type,
                                    std::shared_ptr< Name > name,
                                    std::shared_ptr< Value > value );

    /// @brief Factory to create a Name.
    /// @param n The string of the name to create.
    /// @return Pointer to the created name object.
    std::shared_ptr<Name> Nam(std::string n);

    /// @brief Factory to create a Name.
    /// @param n The string of the name to create.
    /// @return Pointer to the created name object.
    std::shared_ptr<Name> Nam( const char * n);


}
