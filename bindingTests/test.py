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

b= chasecorebnd.Boolean()

state = chasecorebnd.Variable(b, chasecorebnd.Name('test'), chasecorebnd.generic)
id_now = chasecorebnd.Identifier(state)
id_next = chasecorebnd.Identifier(state)
p_now = chasecorebnd.Proposition(id_now)
p_next = chasecorebnd.Proposition(id_next)
np_now = chasecorebnd.UnaryBooleanFormula(chasecorebnd.op_not, p_now)
np_next = chasecorebnd.UnaryBooleanFormula(chasecorebnd.op_not, p_next)

formula = chasecorebnd.UnaryTemporalFormula(
    chasecorebnd.op_globally,
    chasecorebnd.BinaryBooleanFormula(
        chasecorebnd.op_implies,
        np_now,
        chasecorebnd.UnaryTemporalFormula(chasecorebnd.op_next, np_next)
    )
)

print(formula.getString())

graph = chasecorebnd.Graph(4)
graph.addEdge(chasecorebnd.Edge(0,1))
graph.addEdge(chasecorebnd.Edge(0,2))
graph.addEdge(chasecorebnd.Edge(1,2))
graph.addEdge(chasecorebnd.Edge(2,3))
graph.addEdge(chasecorebnd.Edge(3,1))

v0 = chasecorebnd.Vertex(chasecorebnd.Name('0'))
v1 = chasecorebnd.Vertex(chasecorebnd.Name('1'))
v2 = chasecorebnd.Vertex(chasecorebnd.Name('2'))
v3 = chasecorebnd.Vertex(chasecorebnd.Name('3'))

graph.associateVertex(0, v0)
graph.associateVertex(1, v1)
graph.associateVertex(2, v2)
graph.associateVertex(3, v3)

print(graph.getString())

vertexes = {v1, v2}

print(chasecorebnd.getSubGraph(graph, vertexes).getString())


