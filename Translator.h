//
// Created by dana on 07.06.21.
//

#ifndef OMTSCHED_TRANSLATOR_H
#define OMTSCHED_TRANSLATOR_H

#include "Problem.h"
#include "Model.h"
#include <string>

namespace omtsched {

    template<class ComponentID, class TagID, class GroupID>
    class Translator {

    public:

        virtual void solve() = 0;

        virtual Model getSolution() = 0;
        virtual std::vector<Model> getAllSolutions() = 0;

        virtual void printProblem(std::string path) const = 0;
        virtual void printSolution(std::string path) const = 0;
        virtual void printAllSolutions(std::string path) const = 0;
        virtual void printExplanation(std::string path) const = 0;

        Problem <ComponentID, TagID, GroupID> &getProblem() const;

        void setProblem(Problem <ComponentID, TagID, GroupID> &problem);

        bool isGenerateAllSolution() const;

        void setGenerateAllSolution(bool generateAllSolution);

        bool isGenerateExplanations() const;

        void setGenerateExplanations(bool generateExplanations);

    protected:
        Problem<ComponentID, TagID, GroupID> &problem;
        bool generateAllSolution;
        bool generateExplanations;

    };

    template<class ComponentID, class TagID, class GroupID>
    Problem <ComponentID, TagID, GroupID> &Translator<ComponentID, TagID, GroupID>::getProblem() const {
        return problem;
    }

    template<class ComponentID, class TagID, class GroupID>
    void Translator<ComponentID, TagID, GroupID>::setProblem(Problem <ComponentID, TagID, GroupID> &problem) {
        Translator::problem = problem;
    }

    template<class ComponentID, class TagID, class GroupID>
    bool Translator<ComponentID, TagID, GroupID>::isGenerateAllSolution() const {
        return generateAllSolution;
    }

    template<class ComponentID, class TagID, class GroupID>
    void Translator<ComponentID, TagID, GroupID>::setGenerateAllSolution(bool generateAllSolution) {
        Translator::generateAllSolution = generateAllSolution;
    }

    template<class ComponentID, class TagID, class GroupID>
    bool Translator<ComponentID, TagID, GroupID>::isGenerateExplanations() const {
        return generateExplanations;
    }

    template<class ComponentID, class TagID, class GroupID>
    void Translator<ComponentID, TagID, GroupID>::setGenerateExplanations(bool generateExplanations) {
        Translator::generateExplanations = generateExplanations;
    }

}



/*
template <typename TaskID, typename TimeslotID, typename GroupID, typename TagID>
class Translator {

public:

    enum class Solver {
        Z3
    };


    void saveEncoding(const Problem<TaskID, TimeslotID, GroupID, TagID> &problem);
    virtual bool solve(const Problem<TaskID, TimeslotID, GroupID, TagID> &problem) = 0;

protected:

    const char* makeID(const std::string &type, const size_t &number, const std::string &attribute) const;
    const char* makeID(const std::string &type, const size_t &num1, const std::string &attribute, const size_t &num2) const;

    int timeInUnit(const Unit &unit, const boost::posix_time::time_duration &duration) const;
    int timeBetween(const Unit &unit, const boost::posix_time::ptime &first, const boost::posix_time::ptime &second) const;

    // ----------------------------------
    std::map<TaskID, size_t> task_id;
    std::map<TimeslotID, size_t> ts_id;
    std::map<TagID, size_t> tag_id;
    std::map<GroupID, size_t> group_id;

private:

    const std::string assigned(const size_t &tid, const size_t &aid) const;

    };



template <typename TaskID, typename TimeslotID, typename GroupID, typename TagID>
int Translator<TaskID, TimeslotID, GroupID, TagID>::timeInUnit(const Unit &unit, const boost::posix_time::time_duration &duration) const {

    switch (unit) {

        case Unit::HOURS:
            return duration.hours();

        case Unit::MINUTES:
            return duration.hours() *60 + duration.minutes();

        case Unit::SECONDS:
            return duration.hours()*60*60 + duration.minutes()*60 + duration.seconds();


    }

}

template <typename TaskID, typename TimeslotID, typename GroupID, typename TagID>
int Translator<TaskID, TimeslotID, GroupID, TagID>::timeBetween(const Unit &unit, const boost::posix_time::ptime &first, const boost::posix_time::ptime &second) const {

    boost::posix_time::time_duration between = second - first;

    return timeInUnit(unit, between);

}



template <typename TaskID, typename TimeslotID, typename GroupID, typename TagID>
void Translator<TaskID, TimeslotID, GroupID, TagID>::saveEncoding(const Problem<TaskID, TimeslotID, GroupID, TagID> &problem) {

    std::ofstream ostream(problem.problemName + ".smt2");

    ostream << "(set-logic QF_LIA)" << std::endl;
    //ostream << "(set-option :produce-models true)" << std::endl;
    //ostream << "(set-option :produce-unsat-cores true)" << std::endl;

    //---------------------------------------------------------------------------------------
    // ---------------------- Declare all used variables/constants --------------------------
    //---------------------------------------------------------------------------------------


    // Assign internal numerical IDs to all tags and groups
    size_t tag_count = 0;
    for(const auto & tag : problem.getAllTags())
        tag_id[tag] = tag_count++;

    size_t group_count = 0;
    for(const auto & group : problem.getAllGroups())
        group_id[group] = group_count++;


    // Declare all task-related constants
    size_t acount = 0;
    for(const auto & [id, task] : problem.getAllTasks()) {

        // Assign an internal numerical ID
        task_id[id] = acount;

        ostream << "(declare-fun a" << acount  << "s () Int)      ; new task" << std::endl; // Start Point
        ostream << "(declare-fun a" << acount << "du () Int)" << std::endl;                 // Duration
        ostream << "(declare-fun a" << acount << "dl () Int)" << std::endl;                 // Deadline
        ostream << "(declare-fun a" << acount << "o () Bool)" << std::endl;                 // Optional

        // Tags
        for(const auto & [tag, tagid] : tag_id)
            ostream << "(declare-fun a" << acount << "t" << tagid << " () Int)" << std::endl;

        // Groups
        for(const auto & [group, gid] : group_id)
            ostream << "(declare-fun a" << acount << "g" << gid << " () Bool)" << std::endl;

        acount++;
    }


    // Declare all timeslot-related constants
    size_t tcount = 0;
    for(const auto & [id, ts] : problem.getAllTimeslots()) {

        // Assign an internal numerical ID
        ts_id[id] = tcount;

        ostream << "(declare-fun t" << tcount  << "s () Int)      ; new task" << std::endl; // Start Point
        ostream << "(declare-fun t" << tcount << "du () Int)" << std::endl;                 // Duration

        // Tags
        for(const auto & [tag, tagid] : tag_id)
            ostream << "(declare-fun t" << tcount << "t" << tagid << " () Int)" << std::endl;

        // Groups
        for(const auto & [group, gid] : group_id)
            ostream << "(declare-fun t" << tcount << "g" << gid << " () Bool)" << std::endl;

        tcount++;
    }


    // Declare all assignment-variables
    for(const auto &[tid, tzid] : ts_id)
        for(const auto &[aid, azid] : task_id)
            ostream << "(declare-fun " << assigned(tzid, azid) << " () Bool)" << std::endl;



    //---------------------------------------------------------------------------------------
    // ---------------------- Assert given values of all constants --------------------------
    //---------------------------------------------------------------------------------------

    auto startPoint = problem.getStartPoint();
    auto unit = problem.getUnit();

    // Assert task-related values
    for(const auto & [id, task] : problem.getAllTasks()) {

        const auto & internal_id = task_id.at(id);

        ostream << "(assert (= a" << internal_id << "s " << timeBetween(unit, startPoint, task.getStartPoint()) << "))" << std::endl; // Start Point
        ostream << "(assert (= a" << internal_id << "du " << timeInUnit(unit, task.getDuration()) << "))" << std::endl;             // Duration
        ostream << "(assert (= a" << internal_id << "dl " << timeBetween(unit, startPoint, task.getDeadline()) << "))" << std::endl;  // Deadline

        // Optionality
        if(task.isOptional())
            ostream << "(assert a" << internal_id << "o)" << std::endl;
        else
            ostream << "(assert (not a" << internal_id << "o))" << std::endl;


        // Tags
        for(const auto & [tag, tagid] : tag_id)
            ostream << "(assert (= a" << internal_id << "t" << tagid << " " << task.getTagPriority(tag) << "))" << std::endl;


        // Groups
        for(const auto & [group, groupid] : group_id)

            if(task.inGroup(group))
                ostream << "(assert a" << internal_id << "g" << groupid << ")" << std::endl;
            else
                ostream << "(assert (not a" << internal_id << "g" << groupid << "))" << std::endl;
    }


    // Assert timeslot-related values
    for(const auto & [id, timeslot] : problem.getAllTimeslots()) {

        const auto & internal_id = ts_id.at(id);

        ostream << "(assert (= t" << internal_id << "s " << timeBetween(unit, startPoint, timeslot.getStartPoint()) << "))" << std::endl; // Start Point
        ostream << "(assert (= t" << internal_id << "du " << timeInUnit(unit, timeslot.getDuration()) << "))" << std::endl;             // Duration


        // Tags
        for(const auto & [tag, tagid] : tag_id)
            ostream << "(assert (= t" << internal_id << "t" << tagid << " " << timeslot.getTagPriority(tag) << "))" << std::endl;

        // Groups
        for(const auto & [group, groupid] : group_id)

            if(timeslot.inGroup(group))
                ostream << "(assert t" << internal_id << "g" << groupid << ")" << std::endl;
            else
                ostream << "(assert (not t" << internal_id << "g" << groupid << "))" << std::endl;
    }


    //---------------------------------------------------------------------------------------
    // ---------------------- Assert constraints for assignments ----------------------------
    //---------------------------------------------------------------------------------------


    // 0. a timeslot cannot be double-booked
    for(const auto &[tid, tiid] : ts_id)
        for(const auto &[aid, aiid] : task_id)
            for(const auto &[aid2, aiid2] : task_id)
                if(aiid != aiid2)
                    ostream << "(assert (or (not " << assigned(tiid, aiid) << ") (not " << assigned(tiid, aiid2) << ")))" << std::endl;


    // 1. task cannot be scheduled before its start point
    for(const auto &[tsid, ts] : problem.getAllTimeslots())
        for(const auto &[aid, task] : problem.getAllTasks())
            ostream << "(assert (=> " << assigned(ts_id.at(tsid), task_id.at(aid))
                    << " (>= " << timeBetween(unit, startPoint, task.getStartPoint())
                    << " " << timeBetween(unit, startPoint, ts.getStartPoint()) << ")))" << std::endl;


    // 2. Task execution cannot exceed its deadline
    for(const auto &[tsid, ts] : problem.getAllTimeslots())
        for(const auto &[aid, task] : problem.getAllTasks())
            ostream << "(assert (=> " << assigned(ts_id.at(tsid), task_id.at(aid))
                    << " (<= " << timeBetween(unit, startPoint, ts.getStartPoint()) + timeInUnit(unit, ts.getDuration())
                    << " " << timeBetween(unit, startPoint, task.getDeadline()) << ")))" << std::endl;


    // 3. Task duration cannot exceed timeslot duration
    for(const auto &[tsid, ts] : problem.getAllTimeslots())
        for(const auto &[aid, task] : problem.getAllTasks())
            ostream << "(assert (=> " << assigned(ts_id.at(tsid), task_id.at(aid))
                    << "(<= " << timeInUnit(unit, task.getDuration())
                    << " " << timeInUnit(unit, ts.getDuration()) << ")))" << std::endl;


    // 4. If a tag is set on a task, it must be set on a timeslot
    for(const auto &[tsid, ts] : problem.getAllTimeslots())
        for(const auto &[aid, task] : problem.getAllTasks()) {
            ostream << "(assert (! (=> " << assigned(ts_id.at(tsid), task_id.at(aid))
                    << " (and ";
            for(const auto &tag : problem.getAllTags())
                ostream << "(=> (< 0 " << task.getTagPriority(tag) << ") (< 0 " << ts.getTagPriority(tag) << ")) ";

            ostream << ")) :named tags-" << tsid << "-" << aid << "))" << std::endl;
        }


    // 5. All non-optional tasks need to be assigned

    for(const auto &[aid, task] : problem.getAllTasks())
        if(!task.isOptional()) {

            ostream << "(assert (! (or ";

            for(const auto &[tsid, ts] : problem.getAllTimeslots())
                ostream << assigned(ts_id.at(tsid), task_id.at(aid)) << " ";

            ostream << ") :named assign-" << aid << "))" << std::endl;
        }


    //---------------------------------------------------------------------------------------
    // -------------------------------- Final Settings --------------------------------------
    //---------------------------------------------------------------------------------------


    ostream << "(check-sat)" << std::endl;
    /*ostream << "(get-value (";

    for(const auto &[t, tid] : ts_id)
        for(const auto &[a, aid] : task_id)
            ostream << assigned(tid, aid) << " ";
*/
    //ostream << "(get-unsat-core)" << std::endl;

    //ostream << "))" << std::endl;
/*
    ostream.close();

}

template<typename TaskID, typename TimeslotID, typename GroupID, typename TagID>
const char* Translator<TaskID, TimeslotID, GroupID, TagID>::makeID(const std::string &type, const size_t &number, const std::string &attribute) const {

    return (type + std::to_string(number) + attribute).c_str();

}

template<typename TaskID, typename TimeslotID, typename GroupID, typename TagID>
const char* Translator<TaskID, TimeslotID, GroupID, TagID>::makeID(const std::string &type, const size_t &num1, const std::string &attribute, const size_t &num2) const {

    return (type + std::to_string(num1) + attribute + std::to_string(num2)).c_str();

}

template <typename TaskID, typename TimeslotID, typename GroupID, typename TagID>
const std::string Translator<TaskID, TimeslotID, GroupID, TagID>::assigned(const size_t &tid, const size_t &aid) const {

    return "c" + std::to_string(tid) + "_" + std::to_string(aid);

}
*/
#endif //OMTSCHED_TRANSLATOR_H
