/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/20/2022
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "LogisticsProblem.hh"

Equipment::Equipment(std::string name) :
        name(std::move(name)), contract(nullptr), type(generic) {}
const std::string &Equipment::getName() const {return name;}
void Equipment::setName(const std::string &n) {name = n;}
Equipment::~Equipment() {delete contract;}
Bin::Bin(const std::string &name) : Equipment(name) {type = bin;}
Sink::Sink(const std::string &name) : Equipment(name) {type = sink;}
Machine::Machine(const std::string &name) : Equipment(name) {type = machine;}
Road::Road(const std::string &name) : Equipment(name), len(0) {type = road;}
Crossroad::Crossroad(
        const std::string &name) : Equipment(name) {type = crossroad;}
Forum::Forum(const std::string &name) : Equipment(name), capacity(0) {type = forum;}
Bay::Bay(const std::string &name) : Equipment(name) {type = bay;}

Warehouse::Warehouse() {}


