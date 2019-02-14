#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Time
{
private:
    friend ostream& operator<<(ostream& os, const Time& t);
    int hour;
    int minute;
    int sec;
    int msec;

    long int total_ms = 0;

public:
    Time(int h, int m, int s, int ms ) : hour(h), minute(m), sec(s), msec(ms)
    {
        total_ms = (hour * 60 * 60 * 1000) + minute * 60 * 1000 + sec * 1000 + ms;
    }

    inline bool operator==(const Time& t) const
    {
        return total_ms == t.total_ms;
    }

    inline bool operator!=(const Time& t) const
    {
        return total_ms != t.total_ms;
    }

    inline bool operator<(const Time& t) const
    {
        return total_ms < t.total_ms;
    }

    string to_str(void)
    {
        return to_string(hour) + ":" + to_string(minute) + ":" +
               to_string(sec) + to_string(msec);
    }
};

ostream& operator<<(ostream& os, const Time& t)
{
    return os << to_string(t.hour) + ":" + to_string(t.minute) + ":" + to_string(
               t.sec)
           + ":" + to_string(t.msec);
}

Time get_time(const string& str)
{
    //HH:MM:S:MS
    //13:44:49:096
    int h, m, s, ms;
    int cnt = 0; // ':' position marker
    int begin = 0; // next substring starting position marker
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == ':') {
            if (0 == cnt) {
                string st = str.substr(begin, i - begin + 1);
                h = stoi(st);
            } else if ( 1 == cnt ) {
                string st = str.substr(begin, i - begin + 1);
                m = stoi(st);
            } else if ( 2 == cnt ) {
                string st = str.substr(begin, i - begin);
                s = stoi( st);
                st = str.substr(i + 1, (str.length() - 1 - i));
                ms = stoi(st);
                break;
            }

            cnt++;
            begin = i + 1;
        }
    }

    return Time(h, m, s, ms);
}

// {time:{{uuid,sti},....}....};
map<Time, vector<pair<string, double>>> sti_series;
void parse_csv(const std::string& file_name)
{
    ifstream ifs;
    ifs.open(file_name, ifstream::in);

    set<string> uuids;
    string line;
    getline(ifs, line); // skip the header
    getline(ifs, line); // skip the header
    getline(ifs, line); // skip the header
    //std::cout << "DEBUG: parse_csv LINE:" << line << std::endl;
    size_t line_no = 3;
    // TODO handle empty lines.
    while (getline(ifs, line)) {
        if(line.empty()) continue;
        //std::cout << "DEBUG: parse_csv LINE:(LINE_NO: "<< line_no++ << ")" << line << std::endl;
        istringstream stream(line);
        string value;
        vector<string> values;

        while (getline(stream, value, ',')) {
            values.push_back(value);
        }

        Time t1 = get_time(values[2]);
        double sti1 = stod(values[4]);
        Time t2 = get_time(values[3]);
        double sti2 = stod(values[5]);
        string uuid = values[0];
        //printf("uuid:%s, t1:%s, sti1:%f, t2:%s, sti2:%f\n",uuid.c_str(),
        //        t1.to_str().c_str(),sti1,t2.to_str().c_str(),sti2); 
        uuids.insert(uuid);

        auto add = [](Time t,pair<string,double> val){
            if (sti_series.find(t) == sti_series.end()) 
                sti_series[t] = {val};
            else 
                sti_series[t].push_back(val);
        };

        // t1 and t2 might be the same in the log we need to log sti1 in that
        // case.
        if ( t1 != t2) {
            add(t1, make_pair(uuid, sti1));
            add(t2, make_pair(uuid, sti2));
        } else if (t1 == t2) {
            add(t1, make_pair(uuid, sti1));
        }
    }
    // Reshape the matrix to have identical column size
    for(auto& p : sti_series) {
     const Time& t = p.first;
     vector<pair<string, double>>& vec = p.second;
     set<string> row_uuid;
     for(auto column : vec)
         row_uuid.insert(column.first);

     //do set_difference to get missing uuids.
     set<string> remaining;
     auto it = set_difference(uuids.begin(), uuids.end(), row_uuid.begin(),
                          row_uuid.end(), inserter(remaining, remaining.end()));
     //std::cout << "Total UUID cnt: " << uuids.size() << " Row UUID cnt: " <<
     //row_uuid.size() << " Difference: " << remaining.size() << "\n";  
     // append remaining
     for(auto uuid : remaining)
         sti_series[t].push_back(make_pair(uuid,0.0)); //XXX wtf vec.push_back didn't affect the sti_series vec.
    }
}

void dump_matrix(const std::string& file_name)
{
    // sort by UUID to make each columns unique to a uuid
    for (auto p : sti_series) {
        sort((p.second).begin(), (p.second).end(),
        [](pair<string, double>& uuid1, pair<string, double>& uuid2) {
            return uuid1.first < uuid2.first;
        });
    }

    ofstream of;
    of.open(file_name, ofstream::trunc);

    for (auto p : sti_series) {
        for (auto e : p.second)
            of << e.second << " ";

        of << "\n";
    }

    of.flush();
    of.close();
}


int main(int argc, char** argv )
{
    if (argc < 2) {
        std::cout << "Usage: program <input_file> <output_file> \n";
        return 0;
    }

    string testime = " 14:45:45:756 ";
    std::cout << get_time(testime) << std::endl;
    
    auto t1 = chrono::system_clock::now();
    parse_csv(argv[1]);
    auto t2 = chrono::system_clock::now();
    std::cout << "Parsed CSV in " << chrono::duration<double>(t2 - t1).count()
              << " sec\n";

    dump_matrix(argv[2]);
    std::cout << "Dumped matrix in " << chrono::duration<double>
              (chrono::system_clock::now() - t2).count() << " sec\n";

    return 0;
}
