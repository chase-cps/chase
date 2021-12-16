/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        12/15/2021
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once
#include "Value.hh"
#include <vector>

namespace chase {

    /// @brief Container class for Matrixes of Values. The matrix must contain
    /// only valid scalar values, i.e., integer, real, or boolean values.
    /// \todo Binding in the Python library.
    class Matrix : public Value {
    public:
        /// @brief Constructor.
        /// @param rows Number of rows of the matrix to create.
        /// @param columns Number of columns of the matrix to create.
        /// @params M elements of the matrix (if not specified, the matrix
        /// is empty).
        Matrix(
                unsigned int rows,
                unsigned int columns,
                std::vector< Value * > M = {});

        Matrix(unsigned int rows);

        /// @brief Destructor.
        ~Matrix();

        /// @brief Getter of the number of rows.
        /// @return The number of rows.
        unsigned int getRows() const;
        /// @brief Getter of the number of columns.
        /// @return The number of columns.
        unsigned int getColumns() const;

        /// @brief Simplified access to the matrix M's elements. The function
        /// retrieve the value in M(i,j). It a value is specified, then it sets
        /// the element to the specified value.
        /// @param i row coordinate of the element.
        /// @param j column coordinate of the element.
        /// @param value The value to set. If the value is not specified, then
        /// the function act as an accessor to the element in M(i,j).
        /// The value must be a valid scalar value, i.e., integer, real or
        /// boolean. Identifiers which declarations are of a valid scalar type
        /// are also accepted as a legar value for the matrix.
        /// @return Pointer to the element in M(i,j), after the execution of
        /// the function. I.e., if value it is specified, the returned pointer
        /// points to a copy of the value.
        /// \todo Creation of the type.
        Value * at(unsigned i, unsigned j, Value * value = nullptr);

        /// @brief Main visit function.
        /// @return the standard value of the function.
        /// @param v The visitor visiting the Matrix.
        int accept_visitor(BaseVisitor &v) override;

        /// @brief Function to print the matrix.
        /// @return String representing the matrix.
        std::string getString() override;

        /// @brief Clone function.
        /// @return Copy of the Matrix.
        Matrix *clone() override;

    protected:
        /// @brief elements of the Matrix.
        std::vector< Value * > elements;
        /// @brief Number of rows in the Matrix.
        unsigned int _rows;
        /// @brief Number of columns in the Matrix.
        unsigned int _columns;

        /// @brief Method to re-evaluate type of the Matrix when a new element is
        /// introduced.
        /// @param v New value being introduced in the Matrix.
        /// \todo Increase the precision of the type. In particular,
        /// it will be necessary to specify the domain of the variable.
        /// It would be ideal to implement the "minimum common multiplier" of types.
        void _evaluateType(Value * v);

    };

}
