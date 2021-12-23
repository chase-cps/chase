/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        12/15/2021
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "chase/representation/Matrix.hh"
#include "chase/representation/Identifier.hh"
#include "chase/representation/Integer.hh"
#include "chase/representation/Real.hh"
#include "chase/representation/Boolean.hh"

using namespace chase;

Matrix::Matrix(
        unsigned int rows,
        unsigned int columns,
        std::vector< Value * > M) :
    _rows(rows),
    _columns(columns) {

    _node_type = matrix_node;

    elements.reserve(_rows * columns);
    if(M.size() > 0 && M.size() != (rows * columns))
    {
        messageError("Mismatch in Matrix size and elements");
    }

    for (size_t it = 0; it < M.size(); ++it) {
        elements[it] = M[it];
        _evaluateType(elements[it]);
        elements[it]->setParent(this);
    }
}

Matrix::~Matrix()
{
    elements.clear();
}

unsigned int Matrix::getRows() const {
    return _rows;
}

unsigned int Matrix::getColumns() const {
    return _columns;
}

Value *Matrix::at(unsigned int i, unsigned int j, Value *value) {
    if(i >= 1 && j >= 1 && i <= _rows && j <= _columns) {
        unsigned int _i = i - 1;
        unsigned int _j = j - 1;
        size_t index = _i * _columns + _j;
        if (value != nullptr){
            // Check whether the type of the value is legal.
            Type * t = value->getType();
            switch(t->IsA())
            {
                case integer_node:
                case boolean_node:
                case real_node:
                    break;
                default:
                    messageError(
                            "Invalid type for scalar value:\n\t"
                            + t->getString());
            }
            elements[index] = value;
            _evaluateType(elements[index]);
            elements[index]->setParent(this);
        }
        return elements[index];
    } else return nullptr;
}

int Matrix::accept_visitor(BaseVisitor &v) {
    return v.visitMatrix(*this);
}

std::string Matrix::getString() {
    std::string ret("\t[");
    for(size_t i = 1; i < _rows; ++i)
    {
        for (size_t j = 1; j < _columns; ++j) {
            ret += at(i, j)->getString();
            ret += ", ";
        }
        ret += at(i, _columns)->getString();
        ret += ";\n\t";
    }
    for (size_t j = 1; j < _columns; ++j) {
        ret += at(_rows, j)->getString();
        ret += ", ";
    }
    ret += at(_rows, _columns)->getString();
    ret += "]";
    return ret;
}

Matrix *Matrix::clone()
{
    return new Matrix(_rows, _columns, elements);
}

void Matrix::_evaluateType(Value *v)
{
    Type * t = v->getType();
    if(_type == nullptr) {
        _type = t->clone();
        return;
    }
    if(_type->IsA() == boolean_node && t->IsA() == integer_node) {
        _type = new Integer();
    }
    else if(_type->IsA() == boolean_node && t->IsA() == real_node)
    {
        _type = new Real();
    }
    else if(_type->IsA() == integer_node && t->IsA() == real_node)
    {
        _type = new Real();
    }
}