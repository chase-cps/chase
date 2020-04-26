from chasecorebnd import *

x1 = Variable(Integer(), Name('x1'), generic)
x2 = Variable(Integer(), Name('x2'), generic)
y2 = Variable(Integer(), Name('y2'), generic)
y1 = Variable(Integer(), Name('y1'), generic)

zero = IntegerValue(0)

A1 = Always(Prop(GE(Id(x1), zero)))
G1 = Always(Prop(GE(Id(y1), zero)))
A2 = Always(Prop(GE(Id(x2), zero)))
G2 = Always(Prop(GE(Id(y2), zero)))

C1 = Contract('C1')
C1.addDeclaration(x1)
C1.addDeclaration(y1)

C2 = Contract('C2')
C2.addDeclaration(x2)
C2.addDeclaration(y2)

C1.addAssumptions(semantic_domain.logic, A1)
C2.addAssumptions(semantic_domain.logic, A2)
C1.addGuarantees(semantic_domain.logic, G1)
C2.addGuarantees(semantic_domain.logic, G2)

Contract.saturate(C1)
print(C1.getString())

print()

Contract.saturate(C2)
print(C2.getString())

print()

# Important: the map should be composed of pairs of variables of the second and first contract that will
# be passed to the operation. In this case: x2 (a variable of C2) will be replaced by y1 (a variable of C1).
map = {'x2': 'y1'}
# To be consistent with the map being defined above, C1 must be the first parameter, C2 is the second.
C = Contract.composition(C1, C2, map)
print()
print("Before applying any simplification:")
print(C.getString())
print()

simplify(C)
print()
print("After applying simplifications:")
print(C.getString())
