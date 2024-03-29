import sys
from pychase import *

logics_file=sys.argv[1]
dsl_file=sys.argv[2]

b= Boolean()

state = Variable(b, Name('test'), generic)
id_now = Identifier(state)
id_next = Identifier(state)
p_now = Proposition(id_now)
p_next = Proposition(id_next)
np_now = UnaryBooleanFormula(op_not, p_now)
np_next = UnaryBooleanFormula(op_not, p_next)

formula = UnaryTemporalFormula(
    op_globally,
    BinaryBooleanFormula(
        op_implies,
        np_now,
        UnaryTemporalFormula(op_next, np_next)
    )
)

print(formula.getString())

graph = Graph(4)
graph.addEdge(Edge(0,1))
graph.addEdge(Edge(0,2))
graph.addEdge(Edge(1,2))
graph.addEdge(Edge(2,3))
graph.addEdge(Edge(3,1))

v0 = Vertex(Name('0'))
v1 = Vertex(Name('1'))
v2 = Vertex(Name('2'))
v3 = Vertex(Name('3'))

graph.associateVertex(0, v0)
graph.associateVertex(1, v1)
graph.associateVertex(2, v2)
graph.associateVertex(3, v3)

print(graph.getString())

vertexes = {v1, v2}

graph1 = getSubGraph(graph, vertexes)
graph1.setName(Name("GenericSubGraph"))
print(graph1.getString())



comp1 = ComponentDefinition("Test")
print(comp1.getString())
print(comp1.views)

c = Contract('Test')
c.addDeclaration(state)
c.addAssumptions(semantic_domain.logic, formula)
print(c.getString())

sys1 = System()
sys1.addDeclaration(state)
sys1.addContract(c)
print(sys1.getString())

parser_logics = LogicsSpecsBuilder()
parser_logics.parseSpecificationFile(logics_file)
logics_problem = parser_logics.getSystem()

console = Console(logics_problem, "outdir")
console.run("help")
console.run("show")

print(logics_problem.getString())

parser_dsl = DSLSpecsBuilder()
parser_dsl.parseSpecificationFile(dsl_file)
dsl_problem = parser_dsl.getContract()
print(dsl_problem.getString())

gr1p = GR1CPrinter()
gr1p.print(dsl_problem, "out.spc")

print
print("Test completed.")
