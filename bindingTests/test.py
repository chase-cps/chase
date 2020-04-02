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