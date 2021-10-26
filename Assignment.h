//
// Created by admin on 25.08.2021.
//

#ifndef OMTSCHED_ASSIGNMENT_H
#define OMTSCHED_ASSIGNMENT_H

#include "Component.h"
#include "ComponentType.h"
#include <utility>
#include <vector>

namespace omtsched {

    // special values
    enum Number {
        ONE_OR_MORE = -1,
        ANY = -2
    };

    template<typename ID>
    struct ComponentSlot {

        ComponentSlot() = default;

        /*
         * Constructor for a variable component slot
         */
        ComponentSlot(ID componentType, int number, bool optional) : type{componentType},
        number{number}, optional{optional}, fixed{false} {};

        /*
         * Constructor for a fixed component slot
         */
        ComponentSlot(std::vector<Component<ID>> &&comps) : type{comps.front().getType()},
        number{comps.size()}, optional{false}, fixed{true}, components{comps} {}

        ComponentSlot<ID> &operator=(ComponentSlot<ID>&&);

        std::string describe() const;

        void addComponent(const Component<ID> &);

        const ID type;
        int number;
        bool optional;

        bool fixed;
        std::vector<ID> components;
    };

    template<typename ID>
    ComponentSlot<ID> &ComponentSlot<ID>::operator=(ComponentSlot<ID> &&cs) {

        if (&cs == this)
            return *this;

        type = cs.type;
        number = cs.number;
        optional = cs.optional;
        fixed = cs.fixed;
        components = std::move(cs.components);

        return *this;
    }

    template<typename ID>
    std::string ComponentSlot<ID>::describe() const {

        std::string s = "";

        std::string sFixed = fixed ? "fixed " : "variable ";
        std::string sOptional = optional ? "optional " : "required ";

        s = sFixed + " component slot of type " + type + ": ";

        if(fixed)
            s += std::to_string(number) + " components.";
            //for(const auto &comp : components)
                //s += " " + comp; // TODO
        else
            s += std::to_string(number) + " components wanted.";

        return s;
    }

    template<typename ID>
    void ComponentSlot<ID>::addComponent(const Component<ID> &c) {
        components.push_back(c.getID());
    }

    template<typename ID>
    class Assignment {

    public:
        std::vector<Component<ID>> &getDomain(const int &id);

        void setFixed(const ID &name, const Component<ID>&);
        void setFixed(const ID &name, std::vector<Component<ID>>&);

        void setVariable(const ID &name, ID componentType, bool optional);

        const std::map<std::string, ComponentSlot<ID>> & getComponentSlots() const;

        void setOptional(bool optional);

        void setWeight(int weight);

    private:
        std::map<std::string, ComponentSlot<ID>> componentSlots;
        bool optional;
        int weight;
    
    };

    template<typename ID>
    void Assignment<ID>::setFixed(const ID &name, const Component<ID> &comps) {

        componentSlots[name].addComponent(comps);
    }

    template<typename ID>
    void Assignment<ID>::setFixed(const ID &name, std::vector<Component<ID>> &comps) {

        for(const auto &c : comps)
            componentSlots[name].addComponent(comps);
    }

    template<typename ID>
    void Assignment<ID>::setVariable(const ID &name, ID componentType, int number, bool optional) {

        componentSlots[name] = {componentType, number, optional};
    }

    template<typename ID>
    const std::map<std::string, ComponentSlot<ID>> & Assignment<ID>::getComponentSlots() const {
        return componentSlots;
    }

    template<typename ID>
    void Assignment<ID>::setOptional(bool optional) {
        Assignment::optional = optional;
    }

    template<typename ID>
    void Assignment<ID>::setWeight(int weight) {
        Assignment::weight = weight;
    }

}

#endif //OMTSCHED_ASSIGNMENT_H
