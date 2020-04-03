import chasecorebnd

#         Variable * state = _stateVariables.find(*cit)->second;
#         auto id_now = new Identifier(state);
#         auto id_next = new Identifier(state);
#         auto p_now = new Proposition(id_now);
#         auto p_next = new Proposition(id_next);
#         auto np_now = new UnaryBooleanFormula(op_not, p_now);
#         auto np_next = new UnaryBooleanFormula(op_not, p_next);

#         auto formula = new UnaryTemporalFormula(
#                 op_globally,
#                 new BinaryBooleanFormula(
#                         op_implies,
#                         np_now,
#                         new UnaryTemporalFormula(op_next, np_next)));


bv = chasecorebnd.BooleanValue()
print(bv.getString())
bv.setValue(True)
print(bv.getString())

iv= chasecorebnd.IntegerValue()
print(iv.getString())
iv.setValue(4)
print(iv.getString())

rv= chasecorebnd.RealValue()
print(rv.getString())
rv.setValue(4)
print(rv.getString())

r = chasecorebnd.Range(1, 3)
i = chasecorebnd.Integer(r)

print(r.getString())
print(i.getRange().getString())


n = chasecorebnd.Name('test')
n1 = chasecorebnd.Name(n)
b = chasecorebnd.Boolean()

state = chasecorebnd.Constant(b, n1, bv)
print(state.getString())
iden = chasecorebnd.Identifier(state)
print(iden.getString())

state1 = chasecorebnd.Variable(i, chasecorebnd.Name('i'), chasecorebnd.generic)
print(state1.getString())

exp = chasecorebnd.Expression(chasecorebnd.op_eq, iden, iv)
print(exp.getString())

