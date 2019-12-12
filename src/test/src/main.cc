#include "Chase.hh"
#include <cassert>

using namespace chase;

int main( int argc, char * argv[] )
{
    if(argc > 0) assert(argv);
    chase::messageInfo("This is an Information.");
    chase::messageWarning("This is a Warning.");

    chase::messageInfo("Testing the creation of the concrete classes.");

    Integer * i = new Integer();
    Real * r = new Real();
    Boolean * b = new Boolean();

    assert(i);
    assert(r);
    assert(b);

    std::cout << "Test IsA codes: " << i->IsA() 
        << "\t" << r->IsA() << "\t" << b->IsA()
        << std::endl;

    IntegerValue * iv = new IntegerValue(7);
    RealValue * rv = new RealValue(2.0);
    BooleanValue * bv = new BooleanValue(true);

    assert(iv);
    assert(rv);
    assert(bv);

    Range * ri = new Range(new IntegerValue(0), new IntegerValue(8));
    Range * rr = new Range(new RealValue(0.0), new RealValue(8.0));

    assert(ri);
    assert(rr);

    Variable * variable = new Variable( new Integer(), new Name("Var") );
    Constant * constant = new Constant( new Integer(), new Name("Cons") );

    Identifier * id = new Identifier( variable );

    chase::messageInfo("All class have been correctly created.");

    delete(i);
    delete(r);
    delete(b);

    delete(iv);
    delete(rv);
    delete(bv);

    delete(ri);
    delete(rr);

    delete(id);

    delete(variable);
    delete(constant);

    chase::messageInfo("All class have been corretly distructed.");

    chase::messageError("I print an Error and I exit.");

}
