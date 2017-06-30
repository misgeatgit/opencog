
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char** args) {
    std::ifstream is(args[1]);
    std::string line; //Atom(uuid),EnteredAt,LastSeenAt,DurationInAF,IsNLPParseOutput
    std::vector<float> durations;
    float max = -1.0; 
    try{
        durations.reserve(500);
    } catch(std::exception& e){
        std::cout << "Allocator error: " << e.what() << std::endl;
    }
    
    int line_no = 0;
    while(std::getline(is, line)){
        //std::cout << line << std::endl;
        std::istringstream iss(line, std::istringstream::in);
        if(line_no > 2 ){ // skip headers 
            std::string val;
            for(int i = 0; i <= 3; i++) iss >> val;
            // DurationInAF
            float v = std::stof(val);
            durations.push_back(v);
            if ( max <= v ) max = v;
        }
        line_no++;
    }


    std::ostringstream output ;
    for(int i = 0 ; i < durations.size(); i++){
      output << "Atom" << i << "," << durations[i] << "\n";
    }
    std::cout << output.str() ;
}

