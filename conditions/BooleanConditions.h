//
// Created by Betrieb-PC on 06.10.2021.
//

#ifndef OMTSCHED_BOOLEANCONDITIONS_H
#define OMTSCHED_BOOLEANCONDITIONS_H

#include "../Condition.h"

namespace omtsched {


template<typename ID>
class Not : public Condition<ID> {

public:
    Not(std::shared_ptr<Condition < ID>>

    subcondition) : subcondition{ std::move(subcondition) } {}
    static const CONDITION_TYPE type = CONDITION_TYPE::NOT;
    const std::shared_ptr<Condition < ID>> subcondition;

    void print(std::ostream &ostr, const std::vector<Assignment < ID> *

    > &asgns)
    const override;
};

template<typename ID>
void Not<ID>::print(std::ostream &ostr, const std::vector<Assignment < ID> *

> &asgn) const {
ostr << "(not ";
subcondition->
print(ostr, asgn
);
ostr << ") ";
}

template<typename ID>
std::shared_ptr<Condition<ID>> notC(std::shared_ptr<Condition<ID>> subcondition) {
    return std::make_shared<Not<ID>>(subcondition);
}

template<typename ID>
class And : public Condition<ID> {

public:
    And(std::vector<std::shared_ptr<Condition < ID>>

    > subconditions) : subconditions{ std::move(subconditions) } {}
    static const CONDITION_TYPE type = CONDITION_TYPE::AND;
    const std::vector<std::shared_ptr<Condition < ID>>>
    subconditions;

    void print(std::ostream &ostr, const std::vector<Assignment < ID> *

    > &asgns)
    const override;
};

template<typename ID>
void And<ID>::print(std::ostream &ostr, const std::vector<Assignment < ID> *

> &asgn) const {

ostr << "(and ";
for(
const auto &subcondition
: subconditions)
subcondition->
print(ostr, asgn
);
ostr << ") ";
}

template<typename ID>
std::shared_ptr<Condition<ID>> andC(std::vector<std::shared_ptr<Condition<ID>>> subconditions) {
    return std::make_shared<And<ID>>(subconditions);
}

template<typename ID>
class Or : public Condition<ID> {

public:
    Or(std::vector<std::shared_ptr<Condition < ID>>

    > subconditions) : subconditions{ std::move(subconditions) } {}
    static const CONDITION_TYPE type = CONDITION_TYPE::OR;
    const std::vector<std::shared_ptr<Condition < ID>>>
    subconditions;

    void print(std::ostream &ostr, const std::vector<Assignment < ID> *

    > &asgns)
    const override;
};

template<typename ID>
void Or<ID>::print(std::ostream &ostr, const std::vector<Assignment < ID> *

> &asgn) const {

ostr << "(or ";
for(
const auto &subcondition
: subconditions)
subcondition->
print(ostr, asgn
);
ostr << ") ";
}

template<typename ID>
std::shared_ptr<Condition<ID>> orC(std::vector<std::shared_ptr<Condition<ID>>> subconditions) {
    return std::make_shared<Or<ID>>(subconditions);
}

template<typename ID>
class Implies : public Condition<ID> {
public:
    Implies(std::shared_ptr<Condition < ID>>

    antecedent, std::shared_ptr<Condition < ID>> consequent) : antecedent{
        std::move(antecedent)
    }, consequent{ std::move(consequent) } {}
    static const CONDITION_TYPE type = CONDITION_TYPE::IMPLIES;
    const std::shared_ptr<Condition < ID>> antecedent;
    const std::shared_ptr<Condition < ID>> consequent;

    void print(std::ostream &ostr, const std::vector<Assignment < ID> *

    > &asgns)
    const override;
};

template<typename ID>
void Implies<ID>::print(std::ostream &ostr, const std::vector<Assignment<ID>*> &asgn) const {

    ostr << "(=> ";
    antecedent->print(ostr, asgn);
    consequent->print(ostr, asgn);
    ostr << ") ";
}

template<typename ID>
std::shared_ptr<Condition<ID>> implies(const std::shared_ptr<Condition < ID>> antecedent, const std::shared_ptr<Condition < ID>> consequent) {
    return std::make_shared<Implies<ID>>(antecedent, consequent);
}

template<typename ID>
class Xor : public Condition<ID> {
public:
    Xor(std::shared_ptr<Condition < ID>>

    first, std::shared_ptr<Condition < ID>> second) : first{ std::move(first) }, second{ std::move(second) } {}
    static const CONDITION_TYPE type = CONDITION_TYPE::XOR;
    const Condition <ID> first;
    const Condition <ID> second;

    void print(std::ostream &ostr, const std::vector<Assignment<ID>*> &asgns) const override;
};

template<typename ID>
void Xor<ID>::print(std::ostream &ostr, const std::vector<Assignment<ID>*> &asgn) const {

ostr << "(=> ";
first->print(ostr, asgn);
second->print(ostr, asgn);
ostr << ") ";
}

template<typename ID>
std::shared_ptr<Condition<ID>> xorC(const std::shared_ptr<Condition < ID>> first, const std::shared_ptr<Condition < ID>> second) {
    return std::make_shared<Xor<ID>>(first, second);
}

template<typename ID>
class Iff : public Condition<ID> {
public:
    Iff(std::shared_ptr<Condition < ID>>

    first, std::shared_ptr<Condition < ID>> second) : first{ std::move(first) }, second{ std::move(second) } {}
    static const CONDITION_TYPE type = CONDITION_TYPE::IFF;
    const std::shared_ptr<Condition < ID>> first;
    const std::shared_ptr<Condition < ID>> second;

    void print(std::ostream &ostr, const std::vector<Assignment < ID> *

    > &asgns)
    const override;
};

template<typename ID>
void Iff<ID>::print(std::ostream &ostr, const std::vector<Assignment<ID>*> &asgn) const {

ostr << "(and (=> ";
first->print(ostr, asgn);
second->print(ostr, asgn);
ostr << ") (=> ";
second->print(ostr, asgn);
first->print(ostr, asgn);
ostr << ")) ";
}

template<typename ID>
std::shared_ptr<Condition<ID>> iff(const std::shared_ptr<Condition < ID>> first, const std::shared_ptr<Condition < ID>> second) {
    return std::make_shared<Iff<ID>>(first, second);
}

}
#endif //OMTSCHED_BOOLEANCONDITIONS_H