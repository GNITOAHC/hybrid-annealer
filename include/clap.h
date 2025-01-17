#ifndef _CLAP_H_
#define _CLAP_H_

#include <algorithm>
#include <iostream>
#include <regex>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

inline std::string trim (std::string str) {
    str.erase(str.find_last_not_of(' ') + 1); // suffixing spaces
    str.erase(0, str.find_first_not_of(' ')); // prefixing spaces
    return str;
}

inline std::string lstripDash (std::string str) {
    str.erase(0, str.find_first_not_of('-')); // prefixing spaces
    return str;
}

namespace clap {

typedef std::string ArgKey;
typedef std::variant<bool, int, float, double, std::string> ArgVariant;
typedef struct ArgType {
    std::string name;
    enum { FLAG, ARG_INT, ARG_FLOAT, ARG_DOUBLE, ARG_STRING } type;
    enum { NO, REQUIRED, OPTIONAL } required;
} ArgType;
const std::string argTypeStr[] = { "FLAG", "ARG_INT", "ARG_FLOAT", "ARG_DOUBLE", "ARG_STRING" };
const std::string argTypeReq[] = { "NO", "REQUIRED", "OPTIONAL" };

typedef struct ArgVal {
    ArgVariant val;
    ArgType type;
} ArgVal;

typedef struct Arg {
    std::string shortKey;
    std::string longKey;
    std::string help;
    std::vector<ArgType> type;
    int group;
} Arg;

typedef struct GroupInfo {
    int group;
    std::string help;
    bool mutex;
} GroupInfo;
inline bool operator<(const GroupInfo& lhs, const GroupInfo& rhs) {
    return lhs.group < rhs.group;
}

typedef struct Visitor {
    void operator()(const int& i) const {
        std::cout << i << std::endl;
    }
    void operator()(const bool& b) {
        std::cout << b << std::endl;
    }
    void operator()(const double& d) {
        std::cout << d << std::endl;
    }
    void operator()(const std::string& s) {
        std::cout << s << std::endl;
    }
    void operator()(const std::vector<int>& vi) {
        for (const auto& i : vi)
            std::cout << i << std::endl;
    }
    void operator()(const std::vector<double>& vd) {
        for (const auto& d : vd)
            std::cout << d << std::endl;
    }
    void operator()(const std::vector<std::string>& vs) {
        for (const auto& s : vs)
            std::cout << s << std::endl;
    }
} Visotor;

class Clap {
  private:
    std::unordered_map<ArgKey, std::vector<ArgVal> > args; // <long opt, value>
    std::vector<Arg> argList;
    std::vector<std::pair<std::string, std::string> > positional; // <name, help>
    struct ProgramInfo {
        std::string program;
        std::string version;
        std::string description;
        std::set<GroupInfo> groupinfo;
    } programInfo;

    void parseArgs (std::string& params) {
        // std::cout << "Parsing arguments..." << std::endl;
        std::regex argRegex(R"((\[\d+\])?(-\w, )?(--[\w\-]+)((?: <[\w\-]+:\w+>\.?\.?\.?)+)? (.+))");
        std::regex typeRegex(R"(<([\w\-]+):(\w+)>(\.\.\.)?)");
        std::smatch match, typeMatch;

        auto extractGroup = [&] (std::string regexGroup) -> int {
            std::string g = regexGroup.substr(1, regexGroup.size() - 2);
            return std::stoi(g);
        };
        auto extractArgType = [&] (std::string s) {
            if (s == "int") return ArgType::ARG_INT;
            if (s == "double") return ArgType::ARG_DOUBLE;
            if (s == "string") return ArgType::ARG_STRING;
            return ArgType::FLAG;
        };

        std::string::const_iterator searchStart(params.cbegin());
        while (std::regex_search(searchStart, params.cend(), match, argRegex)) {
            // for (int i = 0; i < match.size(); i++) {
            //     std::cout << i << ": " << match[i] << std::endl;
            // }

            Arg arg = {
                // e.g. [1]-f, --file <filename:string> The input file to read
                .shortKey = match[2].matched ? match[2].str().substr(1, 1) : "", // -f
                .longKey  = match[3].str().substr(2),                            // --file
                .help     = trim(match[5].str()), // The input file to read
                .type     = std::vector<ArgType>(),
                .group    = match[1].matched ? extractGroup(match[1].str()) : 0, // [1]
            };

            if (match[4].matched) { // <filename:string>
                std::string typeStr = match[4].str();
                std::string::const_iterator typeSearchStart(typeStr.cbegin());
                while (std::regex_search(typeSearchStart, typeStr.cend(), typeMatch, typeRegex)) {
                    // for (int i = 0; i < typeMatch.size(); i++) {
                    //     std::cout << i << ": " << typeMatch[i] << std::endl;
                    // }
                    // std::cout << std::endl;
                    std::string argName = typeMatch[1];                                 // filename
                    ArgType argType     = { typeMatch[1], extractArgType(typeMatch[2]), // string
                                        typeMatch[3].matched ? ArgType::OPTIONAL    // ...
                                                                 : ArgType::REQUIRED };
                    arg.type.push_back(argType);
                    typeSearchStart = typeMatch.suffix().first;
                }
            } else {
                arg.type.push_back({ "", ArgType::FLAG, ArgType::NO });
            }

            this->argList.push_back(arg);

            searchStart = match.suffix().first;
        }

        // std::cout << std::endl;
        // for (const auto& arg : this->argList) {
        //     std::cout << arg.shortKey << "|" << arg.longKey << "|" << arg.help << std::endl;
        //     for (const auto& type : arg.type) {
        //         std::cout << argTypeStr[type.type] << "|" << argTypeReq[type.required] <<
        //         std::endl;
        //     }
        //     std::cout << "Group: " << arg.group << std::endl;
        //     std::cout << std::endl;
        // }

        std::regex positionalRegex(R"(<([\w\-]+)> (.+)\n)");
        std::smatch matchPositional;
        std::string::const_iterator searchStartPositional(params.cbegin());
        while (std::regex_search(searchStartPositional, params.cend(), matchPositional,
                                 positionalRegex)) {
            // std::cout << "Positional: " << matchPositional[1] << std::endl;
            std::string posHelp = "";
            if (matchPositional[2].matched) { posHelp = matchPositional[2].str(); }
            this->positional.push_back({ matchPositional[1].str(), posHelp });

            searchStartPositional = matchPositional.suffix().first;
        }
    };

    void parseProgram (std::string& params) {
        // std::cout << "Parsing program..." << std::endl;
        std::regex programRegex(R"(Program: (\w+)\n)");
        std::regex versionRegex(R"(Version: (\d+\.\d+\.\d+)\n)");
        std::regex descRegex(R"(Description: (.+)\n)");
        std::regex groupRegex(R"(\[G(\d+)\](\[M\])?(.+))");
        std::smatch match;
        std::string::const_iterator searchStart(params.cbegin());

        if (std::regex_search(searchStart, params.cend(), match, programRegex)) {
            // std::cout << "Program: " << match[1] << std::endl;
            this->programInfo.program = match[1].str();
        }
        if (std::regex_search(searchStart, params.cend(), match, versionRegex)) {
            // std::cout << "Version: " << match[1] << std::endl;
            this->programInfo.version = match[1].str();
        }
        if (std::regex_search(searchStart, params.cend(), match, descRegex)) {
            // std::cout << "Description: " << match[1] << std::endl;
            this->programInfo.description = match[1].str();
        }

        while (std::regex_search(searchStart, params.cend(), match, groupRegex)) {
            // std::cout << "Group: " << match[1] << std::endl;
            // std::cout << "Mutex: " << match[2] << std::endl;
            // std::cout << "Help: " << match[3] << std::endl;
            this->programInfo.groupinfo.insert({
                .group = std::stoi(match[1].str()),
                .help  = trim(match[3].str()),
                .mutex = match[2].matched,
            });

            searchStart = match.suffix().first;
        }
    };

  public:
    Clap (std::string params) {
        this->parseArgs(params);
        this->parseProgram(params);
        std::sort(this->argList.begin(), this->argList.end(),
                  [] (const Arg& a, const Arg& b) { return a.group < b.group; });
    }
    Clap() {};

    void setParams (std::string params) {
        this->parseArgs(params);
        this->parseProgram(params);
        std::sort(this->argList.begin(), this->argList.end(),
                  [] (const Arg& a, const Arg& b) { return a.group < b.group; });
    }

    void parse (const int argc, char **argv, int& iter, int terminatePos = 1) {
        // std::cout << "argc = " << argc << std::endl;
        if (argc == 1) return;
        iter = iter == 0 ? 1 : iter;

        auto search = [this] (std::string s) -> std::vector<ArgType> {
            // std::cout << "Finding " << s << std::endl;
            for (auto const& l : this->argList) {
                if (l.shortKey == lstripDash(s) || l.longKey == lstripDash(s)) return l.type;
            }
            return std::vector<ArgType>();
        };
        auto getLong = [this] (std::string s) -> std::string {
            for (auto const& l : this->argList) {
                if (l.shortKey == lstripDash(s) || l.longKey == lstripDash(s))
                    return lstripDash(l.longKey);
            }
            return "";
        };

        std::vector<ArgVal> argVal(0);
        int positionalCount = 0;
        for (; iter < argc; ++iter) {
            if (positionalCount == terminatePos) break;

            std::string cur = argv[iter];
            std::vector<ArgType> argType;

            if ((argType = search(cur)).size() == 0) {
                // std::cout << "Positional argument: " << cur << std::endl;
                if (positionalCount < this->positional.size())
                    this->positional[positionalCount++].second = cur;

                continue;
            }

            // std::cout << "This arg is found: " << cur << std::endl;
            int peek = iter + 1; // Consume the argument indicator itself
            for (auto const& a : argType) {
                if (a.required == ArgType::NO) break;
                if (a.required == ArgType::OPTIONAL && search(argv[peek]).size() != 0) break;
                // std::cout << argTypeStr[a.type] << "|" << argTypeReq[a.required] << "|";
                // std::cout << "Peeking: " << argv[peek] << "\n";
                switch (a.type) {
                    case ArgType::ARG_INT: argVal.push_back({ std::stoi(argv[peek]), a }); break;
                    case ArgType::ARG_FLOAT: argVal.push_back({ std::stof(argv[peek]), a }); break;
                    case ArgType::ARG_DOUBLE: argVal.push_back({ std::stod(argv[peek]), a }); break;
                    case ArgType::ARG_STRING: argVal.push_back({ argv[peek], a }); break;
                    default: std::cerr << "Parse error: " << std::endl; break;
                }
                ++peek;
                if (peek == argc) break;
            }

            this->args.insert({ getLong(cur), argVal });
            argVal.clear();

            iter = peek - 1;
        }

        // for (auto const& a : this->args) {
        //     std::cout << a.first << std::endl;
        // }
    };

    void help () const {
        std::cout << "Program: " << this->programInfo.program << std::endl;
        std::cout << "Version: " << this->programInfo.version << std::endl;
        std::cout << "Description: " << this->programInfo.description << std::endl;
        std::cout << "\n\tHELP:" << std::endl;

        std::set<GroupInfo>::iterator itr = this->programInfo.groupinfo.begin();

        for (int i = 0; i < this->argList.size(); i++) {
            if (itr != this->programInfo.groupinfo.end() &&
                (*itr).group == this->argList[i].group) {
                std::cout << std::endl << "\t" << (*itr).help << std::endl;
                itr++;
            }
            if (this->argList[i].shortKey.empty()) {
                std::cout << "\t\t    --" << this->argList[i].longKey;
            } else {
                std::cout << "\t\t-" << this->argList[i].shortKey << ", --"
                          << this->argList[i].longKey;
            }
            for (const auto& type : this->argList[i].type) {
                if (type.type == ArgType::FLAG) { continue; }
                std::cout << " <" << type.name << ":" << argTypeStr[type.type] << ">";
                if (type.required == ArgType::OPTIONAL) { std::cout << "..."; }
            }
            std::cout << std::endl;
            std::cout << "\t\t\t" << this->argList[i].help << std::endl;
        }

        if (this->positional.size() > 0) { std::cout << "\n\tPOSITIONAL ARGUMENTS:\n"; }
        for (auto const& p : this->positional) {
            std::cout << "\t\t<" << p.first << ">" << "  " << p.second << std::endl;
        }
    };

    bool hasArg (const std::string& s) const {
        return this->args.find(s) != this->args.end();
    };

    std::vector<ArgVariant> getArg (const std::string& s) const {
        if (this->args.find(s) == this->args.end()) { return std::vector<ArgVariant>(); }
        std::vector<ArgVariant> args;
        for (const auto& a : this->args.at(s)) {
            args.push_back(a.val);
        }
        return args;
    };

    std::string getPosArg (const int i) const {
        if (i >= this->positional.size()) return "";
        return this->positional[i].second;
    }
};

} // namespace clap

#endif
