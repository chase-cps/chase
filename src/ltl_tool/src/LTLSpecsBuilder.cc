/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/23/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#include "LTLSpecsBuilder.hh"

using namespace chase;
using namespace ltl_tool;
using namespace antlr4;

LTLSpecsBuilder::LTLSpecsBuilder() :
    _system(nullptr),
    _currContract(nullptr)
{
}

LTLSpecsBuilder::~LTLSpecsBuilder() = default;



System *LTLSpecsBuilder::getSystem() const {
    return _system;
}

System *LTLSpecsBuilder::parseSpecificationFile(std::string infile)
{
    ANTLRFileStream input( infile );
    LTLContractsLexer lexer( &input );
    CommonTokenStream tokens( &lexer );

    LTLContractsParser parser( &tokens );

    parser.setBuildParseTree(true);
    LTLContractsParser::SystemSpecContext * tree = parser.systemSpec();

    visitSystemSpec(tree);

    return _system;
}



antlrcpp::Any
LTLSpecsBuilder::visitSystemSpec(LTLContractsParser::SystemSpecContext *ctx) {
    LTLContractsParser::NameContext * n_ctx = ctx->name();
    if( n_ctx != nullptr )
    {
        std::string name = n_ctx->ID()->getText();
        messageInfo("Parsing the system: " + name);
        _system = new System(name);
    } else{
        messageInfo("Parsing the system: SYSTEM (default)");
        _system = new System();
    }

    return LTLContractsBaseVisitor::visitSystemSpec(ctx);
}

antlrcpp::Any
LTLSpecsBuilder::visitDeclaration(LTLContractsParser::DeclarationContext *ctx) {
    std::string name = ctx->ID()->getText();
    DataDeclaration * dec = nullptr;
    if (ctx->variableKW() != nullptr)
        dec = new Variable(new Integer(), new Name(name));
    else if (ctx->constantKW() != nullptr)
    {
        int num = std::stoi(ctx->NUMBER()->getText().c_str());
        dec = new Constant(
                new Integer(),
                new Name(name),
                new IntegerValue(num));
    }
    else if (ctx->propositionKw() != nullptr)
    {
        dec = new Variable(
                new Boolean(), new Name(name));

        if( ctx->relation() != nullptr ) {
            Expression *exp = buildRelation(ctx->relation());

            _map_props_values.insert(
                    std::pair<Variable *, Expression *>(
                            reinterpret_cast<Variable *>(dec), exp));
        }
    }
    else
        messageError("Unknown entry:\t" + ctx->getText());

    if( _currContract != nullptr )
        _currContract->addDeclaration(dec);
    else
        _system->addDeclaration(dec);

    return antlrcpp::Any();
}

Expression *
LTLSpecsBuilder::buildRelation(LTLContractsParser::RelationContext *ctx)
{
    std::string rel_op = ctx->relation_op()->getText();
    Operator op = op_neq;
    if( rel_op == "!=" ) op = op_neq;
    else if( rel_op == "=" ) op = op_eq;
    else if( rel_op == ">" ) op = op_gt;
    else if( rel_op == "<" ) op = op_lt;
    else if( rel_op == ">=" ) op = op_ge;
    else if( rel_op == "<=" ) op = op_le;
    else messageError("Unsupported relation operator: " + rel_op);

    return new Expression(op,
            buildValue(ctx->lvalue()->value()),
            buildValue(ctx->rvalue()->value()));



}

Value *LTLSpecsBuilder::buildValue(LTLContractsParser::ValueContext *ctx)
{
    // The value is terminal.
    if(ctx->children.size() == 1)
    {
        if(ctx->ID(0) != nullptr)
            return createIdentifier(ctx->ID(0)->getText());
        if(ctx->value(0))
            return buildValue(ctx->value(0));
        if(ctx->NUMBER())
            return new IntegerValue(std::stoi(ctx->getText()));
        if(ctx->minus_number())
            return new IntegerValue(std::stoi(ctx->getText()));
    }

    if(ctx->children.size() == 3)
    {
        if(ctx->children[0]->getText() == "(" &&
        ctx->children[2]->getText() == ")" )
        {
            return buildValue(ctx->value(0));
        }

        std::string math_op = ctx->bin_math_op()->getText();
        Operator op = op_plus;
        if(math_op == "+") op = op_plus;
        if(math_op == "-") op = op_minus;
        if(math_op == "*") op = op_multiply;
        if(math_op == "/") op = op_divide;

        // Brute force with combinations.
        if(ctx->children[0] == ctx->ID(0) &&
            ctx->children[2] == ctx->ID(1))
        {
            return new Expression(op,
                    createIdentifier(ctx->ID(0)->getText()),
                    createIdentifier(ctx->ID(1)->getText()));
        }
        // Brute force with combinations.
        if(ctx->children[0] == ctx->value(0) &&
           ctx->children[2] == ctx->value(1))
        {
            return new Expression(op,
                    buildValue(ctx->value(0)),
                    buildValue(ctx->value(1)));

        }
        if(ctx->children[0] == ctx->ID(0) &&
           ctx->children[2] == ctx->value(0))
        {
            return new Expression(op,
                    createIdentifier(ctx->ID(0)->getText()),
                    buildValue(ctx->value(0)));
        }
        if(ctx->children[0] == ctx->value(0) &&
           ctx->children[2] == ctx->ID(0))
        {
            return new Expression(op,
                    buildValue(ctx->value(0)),
                    createIdentifier(ctx->ID(0)->getText()));
        }
    }
    return nullptr;
}

Identifier *LTLSpecsBuilder::createIdentifier(std::string name)
{
    if( _currContract != nullptr )
    {
        // search declaration inside contract.
        for(auto i = _currContract->declarations.begin();
                i != _currContract->declarations.end(); ++i )
        {
            DataDeclaration * currDec = dynamic_cast<DataDeclaration *>(*i);
            if( currDec == nullptr ) continue;
            if(currDec->getName()->getString() == name )
                return new Identifier(currDec);
        }
    }

    // if it is not in the current contract, search in the global declarations.
    for(auto i = _system->getDeclarationsSet().begin();
        i != _system->getDeclarationsSet().end(); ++i )
    {
        DataDeclaration * currDec = dynamic_cast<DataDeclaration *>(*i);
        if( currDec == nullptr ) continue;
        if(currDec->getName()->getString() == name )
            return new Identifier(currDec);
    }

    return nullptr;
}







