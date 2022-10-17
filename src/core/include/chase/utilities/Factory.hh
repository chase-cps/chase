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
    BooleanConstant * True();
    /// @brief Factory building a False boolean constant.
    /// @return A pointer to a new object representing a False boolean constant.
    BooleanConstant * False();

    /// @brief Factory for the NOT operation.
    /// @param op The operator of the operation.
    /// @return A pointer to a new object representing the NOT operation.
    UnaryBooleanFormula * Not( LogicFormula * op );

    /// @brief Factory for the AND operation.
    /// @param op1 The first operator of the operation.
    /// @param op2 The second operator of the operation.
    /// @return A pointer to a new object representing the AND operation.
    BinaryBooleanFormula * And( LogicFormula * op1, LogicFormula * op2);

    /// @brief Factory for the OR operation.
    /// @param op1 The first operator of the operation.
    /// @param op2 The second operator of the operation.
    /// @return A pointer to a new object representing the OR operation.
    BinaryBooleanFormula * Or( LogicFormula * op1, LogicFormula * op2);

    /// @brief Factory for the Implies operation.
    /// @param op1 The first operator of the operation.
    /// @param op2 The second operator of the operation.
    /// @return A pointer to a new object representing the Implies operation.
    BinaryBooleanFormula * Implies( LogicFormula * op1, LogicFormula * op2);

    /// @brief Factory for the double implication operation.
    /// @param op1 The first operator of the operation.
    /// @param op2 The second operator of the operation.
    /// @return A pointer to a new object representing the Implies operation.
    BinaryBooleanFormula * Iff( LogicFormula * op1, LogicFormula * op2);

    /// @brief Factory for the NAND operation.
    /// @param op1 The first operator of the operation.
    /// @param op2 The second operator of the operation.
    /// @return A pointer to a new object representing the NAND operation.
    BinaryBooleanFormula * Nand( LogicFormula * op1, LogicFormula * op2);

    /// @brief Factory for the Xor operation.
    /// @param op1 The first operator of the operation.
    /// @param op2 The second operator of the operation.
    /// @return A pointer to a new object representing the Xor operation.
    BinaryBooleanFormula * Xor( LogicFormula * op1, LogicFormula * op2);

    /// @brief Factory for the NOR operation.
    /// @param op1 The first operator of the operation.
    /// @param op2 The second operator of the operation.
    /// @return A pointer to a new object representing the NOR operation.
    BinaryBooleanFormula * Nor( LogicFormula * op1, LogicFormula * op2);

    /// @brief Factory for the XNOR operation.
    /// @param op1 The first operator of the operation.
    /// @param op2 The second operator of the operation.
    /// @return A pointer to a new object representing the XNOR operation.
    BinaryBooleanFormula * Xnor( LogicFormula * op1, LogicFormula * op2);

    /// @brief Factory function creating a Large AND operation.
    /// @param formulas Vector of formulas to copy into the operation.
    /// @return A new object representing a Large AND operation.
    LargeBooleanFormula * LargeAnd( std::vector< LogicFormula * >& formulas );

    /// @brief Factory function creating a Large OR operation.
    /// @param formulas Vector of formulas to copy into the operation.
    /// @return A new object representing a Large OR operation.
    LargeBooleanFormula * LargeOr( std::vector< LogicFormula * >& formulas );

    /// @brief Factory for the Always logic formula.
    /// @param op The operator of the formula.
    /// @return A pointer to a new object representing the Always logic formula.
    UnaryTemporalFormula * Always(LogicFormula * op);

    /// @brief Factory for the Eventually logic formula.
    /// @param op The operator of the formula.
    /// @return A pointer to a new object representing the Eventually logic formula.
    UnaryTemporalFormula * Eventually(LogicFormula * op);

    /// @brief Factory for the Next formula.
    /// @param op The operator of the formula.
    /// @return A pointer to a new object representing the next temporal
    /// formula.
    UnaryTemporalFormula * Next(LogicFormula * op);

    /// @brief Factory for the Until formula.
    /// @param op1 The 1st operator of the formula.
    /// @param op2 The 2nd operator of the formula.
    /// @return A pointer to a new object representing the until temporal
    /// formula.
    BinaryTemporalFormula * Until(LogicFormula * op1, LogicFormula * op2);

    /// @brief Factory for the representation of a sum.
    /// @param op1 The first operator of the operation.
    /// @param op2 The second operator of the operation.
    /// @return A pointer to a new object representing a sum.
    Expression * Sum(Value * op1, Value * op2);

    /// @brief Factory for the representation of a subtraction.
    /// @param op1 The first operator of the operation.
    /// @param op2 The second operator of the operation.
    /// @return A pointer to a new object representing a subtraction.
    Expression * Sub(Value * op1, Value * op2);

    /// @brief Factory for the representation of a multiplication.
    /// @param op1 The first operator of the operation.
    /// @param op2 The second operator of the operation.
    /// @return A pointer to a new object representing a multiplication.
    Expression * Mult(Value * op1, Value * op2);

    /// @brief Factory for the representation of a division.
    /// @param op1 The first operator of the operation.
    /// @param op2 The second operator of the operation.
    /// @return A pointer to a new object representing a division.
    Expression * Div(Value * op1, Value * op2);

    /// @brief Factory for the representation of a equality expression.
    /// @param op1 The first operator of the operation.
    /// @param op2 The second operator of the operation.
    /// @return A pointer to a new object representing a equality (op1 = op2).
    Expression * Eq(Value * op1, Value * op2);

    /// @brief Factory for the representation of a less-equal expression.
    /// @param op1 The first operator of the operation.
    /// @param op2 The second operator of the operation.
    /// @return A pointer to a new object representing a less-equal (op1<=op2).
    Expression * NEq(Value * op1, Value * op2);

    /// @brief Factory for the representation of a less-equal expression.
    /// @param op1 The first operator of the operation.
    /// @param op2 The second operator of the operation.
    /// @return A pointer to a new object representing a less-equal (op1<=op2).
    Expression * LE(Value * op1, Value * op2);

    /// @brief Factory for the representation of a less-than expression.
    /// @param op1 The first operator of the operation.
    /// @param op2 The second operator of the operation.
    /// @return A pointer to a new object representing a less-than (op1<op2).
    Expression * LT(Value * op1, Value * op2);

    /// @brief Factory for the representation of a greater-equal expression.
    /// @param op1 The first operator of the operation.
    /// @param op2 The second operator of the operation.
    /// @return A pointer to a new object representing a greater-equal
    /// (op1>=op2).
    Expression * GE(Value * op1, Value * op2);

    /// @brief Factory for the representation of a greater-than expression.
    /// @param op1 The first operator of the operation.
    /// @param op2 The second operator of the operation.
    /// @return A pointer to a new object representing a greater-than (op1>op2).
    Expression * GT(Value * op1, Value * op2);

    /// @brief Factory to create a proposition from a variable.
    /// @param var The variable to be used.
    /// @return The proposition being created.
    Proposition * Prop(Variable * var);

    /// @brief Factory to create a proposition from an expression.
    /// @param exp The expression to be used.
    /// @return The proposition being created.
    Proposition * Prop(Expression * exp);

    /// @brief Factory to create an Identifier from a declaration.
    /// @param declaration The declaration of the Identifier.
    /// @return Pointer to the created identifier.
    Identifier * Id(DataDeclaration * declaration);

    /// @brief Factory to create an Integer Value.
    /// @param n The integer to be represented.
    /// @return Pointer to the created integer object.
    IntegerValue * IntVal( int n );

    /// @brief Factory to create an Real Value.
    /// @param r The double number to be represented.
    /// @return Pointer to the created real value object.
    RealValue * RealVal( double r );

    /// @brief Factory to create an Boolean Value.
    /// @param b The boolean value to be represented.
    /// @return Pointer to the created boolean value object.
    BooleanValue * BoolVal( bool b );

    /// @brief Factory to create an Arithmetic Constraint.
    /// @param op The operator of the constraint.
    /// @param op1 The first operator.
    /// @param op2 The second operator.
    /// @return A pointer to the arithmetic constraint.
    Constraint * arithmeticConstraint(Operator op,
                                      Value * op1, Value * op2);

}
