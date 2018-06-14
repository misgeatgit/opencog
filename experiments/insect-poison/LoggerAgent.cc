#include "LoggerAgent.h"

using namespace opencog;
using namespace std::chrono;
using namespace std::placeholders;

#define MAX_SAMPLES 50000
bool LoggerAgent::topic_changed = false;

LoggerAgent::~LoggerAgent(){
}

const ClassInfo& LoggerAgent::classinfo() const
{
    return info();
}

const ClassInfo& LoggerAgent::info()
{
    static const ClassInfo _ci("opencog::LoggerAgent");
    return _ci;
}

LoggerAgent::LoggerAgent(CogServer& cs) : Agent(cs), _start(system_clock::now()){
    _as = &cs.getAtomSpace();
    _bank = &attentionbank(_as);
    _start = system_clock::now();
    _as->atomAddedSignal().connect(std::bind(&LoggerAgent::atomAddedListener, this, _1));
    //_bank->AddAFSignal().connect(std::bind(&LoggerAgent::atomAddedListener, this, _1));
    af_size_stat.reserve(MAX_SAMPLES); //allocate 50K sample holding space.

    try{
        poison_atoms.reserve(3*pow(10,6));
        insect_atoms.reserve(5*pow(10,6));
    } catch(const std::exception& e) {
        std::cout << "Allocation Error: " << e.what() << '\n';
    }

    try{
    insect_poison_percentage.reserve(1000);
    }
    catch (const std::length_error& le) {
     std::cout << "ERROR WHILE TRYING TO RESERVE: " << le.what() << '\n';
    }

}

// Listens to Atoms added to the AS.
// Assumption: if not topic_changed flag is set, atoms are stored in
// poison_atom vec. Else, they would be stored in insect_atoms vec.
void LoggerAgent::atomAddedListener(const Handle& h){
    //std::cout << "INFO: New atom added into the AF.\n";
    if(not topic_changed){
        insect_atoms.push_back(h); 
    }else{
        poison_atoms.push_back(h);
    }
}

void LoggerAgent::run(void){
    HandleSeq afset;
    _bank->get_handle_set_in_attentional_focus(std::back_inserter(afset));
    if(afset.size() <= 0 )
        return;

    HandleSeq poison_atoms_AF; 
    HandleSeq insect_atoms_AF; 
    HandleSeq insecticide_atoms_AF; 
    HandleSeq nonNLP_atoms_AF;
    
    // Get insecticide related atoms from the non-NLP atoms in AF.
    auto find_insecticides = [this](HandleSeq& nonNLP_atoms_AF, HandleSeq& insecticide_atoms_AF){
        for(const auto& name : insecticides){
            auto it =std::find_if(nonNLP_atoms_AF.begin(), nonNLP_atoms_AF.end(), [name](const Handle& h){
                    HandleSeq seq;
                    if (h->is_link()) {
                    HandleSeq outg = h->getOutgoingSet();
                    seq = outg;
                    } else{
                    seq.push_back(h);
                    }    
                    bool r = false;
                    for(const auto& h : seq){
                    //TODO make sure there capitalization doesn't matter
                    if(h->is_node() and h->get_name() == name){
                        r = true;
                        break;
                      }
                    }
                    return r; });

            if( it != nonNLP_atoms_AF.end()){
                insecticide_atoms_AF.push_back(*it);
            }
        }
    };

    // TODO We don't have any straight forward mechanism to watch for poison
    // related atoms while insect related article is being parsed. i.e ahead of
    // topic_changed flag.
    if(not topic_changed){
        std::sort(insect_atoms.begin(), insect_atoms.end());
        std::sort(afset.begin(), afset.end());
        // Get insect atoms in AF.
        std::set_intersection(insect_atoms.begin(), insect_atoms.end(),
                afset.begin(), afset.end(),
                std::back_inserter(insect_atoms_AF));

        // Get non-NLP atoms in AF. i.e AF - poison_atoms_AF
        std::sort(insect_atoms_AF.begin(), insect_atoms_AF.end());
        std::set_difference(
                afset.begin(), afset.end(),
                insect_atoms_AF.begin(), insect_atoms_AF.end(),
                std::back_inserter(nonNLP_atoms_AF));

        // Populate insecticde_atoms_AF
        find_insecticides(nonNLP_atoms_AF, insecticide_atoms_AF);
    }

    else {
        std::sort(poison_atoms.begin(), poison_atoms.end());
        std::sort(afset.begin(), afset.end());
        // Get poison atoms in AF.
        std::set_intersection(poison_atoms.begin(), poison_atoms.end(),
                afset.begin(), afset.end(),
                std::back_inserter(poison_atoms_AF));

        // Get non-NLP atoms in AF. i.e AF - poison_atoms_AF
        std::sort(poison_atoms_AF.begin(), poison_atoms_AF.end());
        std::set_difference(
                afset.begin(), afset.end(),
                poison_atoms_AF.begin(), poison_atoms_AF.end(),
                std::back_inserter(nonNLP_atoms_AF));

        // Get insect atoms from prev parse.
        std::sort(nonNLP_atoms_AF.begin(), nonNLP_atoms_AF.end());
        std::sort(insect_atoms.begin(), insect_atoms.end());
        std::set_intersection(
                nonNLP_atoms_AF.begin(), nonNLP_atoms_AF.end(),
                insect_atoms.begin(), insect_atoms.end(),
                std::back_inserter(insect_atoms_AF));

        // Populate insecticde_atoms_AF
        find_insecticides(nonNLP_atoms_AF, insecticide_atoms_AF);
    } 

    // task-1 afstat
    for(const Handle h : afset){
        bool is_nlp_parse_output = false;
        auto it = std::find(nonNLP_atoms_AF.begin(), nonNLP_atoms_AF.end(), h);
        if(it != nonNLP_atoms_AF.end()){
            is_nlp_parse_output = true;
        }
        //task 2 - query af and fill atomstat
        auto iter = handle_atomstat_map.find(h);
        auto time = system_clock::now();
        if (iter == handle_atomstat_map.end()){
            AtomStat ast;
            ast.h = h;
            ast.last_active = time;
            ast.entered_at = time;

            ast.is_nlp_parse_output = is_nlp_parse_output;
            ast.dr = duration<double>(0);
            // ast.percentage_sti_gain_from_spreading =
            handle_atomstat_map[h] = ast;
        }
        else{
            (iter->second).last_active = time;
            (iter->second).dr = system_clock::now() - (iter->second).entered_at;
        }
    }

    static bool first_time = true;
    static auto last_run = system_clock::now();

    // task-4, task-3 and task-1 calculate percentage of AF content from prev topic
    float af_size = static_cast<float>(afset.size());
    float insect_af_percentage = static_cast<float>(insect_atoms_AF.size()) / af_size;
    float poison_af_percentage = static_cast<float>(poison_atoms_AF.size()) / af_size;
    float nonNLP_af_percentage = static_cast<float>(nonNLP_atoms_AF.size()) / af_size;
    float insecticide_af_percentage = static_cast<float>(insecticide_atoms_AF.size()) / af_size;
    if(first_time){
        insect_poison_percentage.push_back(std::vector<float>{0.0 , nonNLP_af_percentage,
                insect_af_percentage, poison_af_percentage, insecticide_af_percentage});
        first_time = false;
    } else{
        duration<double> dr = system_clock::now() - last_run;
        insect_poison_percentage.push_back(std::vector<float>{float(dr.count()) , nonNLP_af_percentage,
                insect_af_percentage, poison_af_percentage, insecticide_af_percentage});

        //last_probing_time = system_clock::now(); //reference time to deduce whether atom is still in AF or not.
    }

    //last_run = system_clock::now();
}

