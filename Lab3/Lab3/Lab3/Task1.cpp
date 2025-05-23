#include <iostream>
#include <string>
#include <unordered_set>
#include <random>
#include <iterator>

using Set = std::unordered_set<std::string>;
struct Clause {
public: 
    Set p;
    Set n;

    bool operator==(const Clause& other) const {
        return p == other.p && n == other.n;
    }
    bool empty() const {
        return p.empty() && n.empty();
    }

    bool subsetOf(const Clause& other) const {
        for (const std::string& x : p) {
            if (other.p.find(x) == other.p.end()) return false;
        }

        for (const std::string& x : n) {
            if (other.n.find(x) == other.n.end()) return false;
        }
        
        return true;
    }

    void toString() {
        if (empty()) {
            std::cout << "False";
        }
        else {
            bool first = true;

            for (const auto& lit : p) {
                if (!first) std::cout << " v ";
                std::cout << lit;
                first = false;
            }

            for (const auto& lit : n) {
                if (!first) std::cout << " v ";
                std::cout << "-" << lit;
                first = false;
            }
        }
    }
};

namespace std {
    template<>
    struct hash<Clause> {
        size_t operator()(const Clause& c) const {
            size_t hashVal = 0;
            hash<string> hasher;
            for (const auto& lit : c.p) {
                hashVal ^= hasher(lit);
            }
            for (const auto& lit : c.n) {
                hashVal ^= hasher(lit) << 1;
            }
            return hashVal;
        }
    };
}
using ClauseSet = std::unordered_set<Clause>;

Clause Resolution(const Clause& A, const Clause& B);
ClauseSet Solver(ClauseSet& KB);
ClauseSet Incorporate(const ClauseSet& S, ClauseSet& KB);
ClauseSet Incorporate_clause(const Clause& A, ClauseSet& KB);

Set Intersection(const Set& a, const Set& b);
Set Union (const Set & a, const Set& b);
ClauseSet readClausesFromInput();
std::string getRandomElement(const Set& s);

/*************MAIN***********/
int main() {
    ClauseSet KB1 = readClausesFromInput();

    ClauseSet solved = Solver(KB1);

    std::cout << "\nSolved:\n";
    for (Clause c : solved) {
        if (c.p.size() + c.n.size() == 1) {
            c.toString();
            std::cout << std::endl;
        }
    }
}

Clause Resolution(const Clause& Aorig, const Clause& Borig) {
    Clause A = Aorig; Clause B = Borig;

    Set C1 = Intersection(A.p, B.n);
    Set C2 = Intersection(A.n, B.p);

    if (C1.empty() && C2.empty()) return Clause{};

    if (!C1.empty()) {
        std::string randel = getRandomElement(C1);
        A.p.erase(randel);
        B.n.erase(randel);
    } else {
        std::string randel = getRandomElement(C2);
        A.n.erase(randel);
        B.p.erase(randel);
    }

    Clause C;
    C.p = Union(A.p, B.p);
    C.n = Union(A.n, B.n);

    if (!Intersection(C.p, C.n).empty()) return Clause{}; //tautology
    return C; // No duplicates in unordered_set
}

ClauseSet Solver(ClauseSet& KB) {
    ClauseSet KBprim;
    ClauseSet empty;
    Incorporate(KB, empty);

    do {
        ClauseSet S;
        KBprim = KB; 

        for (auto it1 = KB.begin(); it1 != KB.end(); ++it1) { //resolve pairs of clauses
            auto it2 = it1;
            ++it2;
            for (; it2 != KB.end(); ++it2) {
                Clause C = Resolution(*it1, *it2);
                if (!C.empty()) S.insert(C); //insert new clauses
            }
        }

        if (S.empty()) return KB; //no new clauses
        KB = Incorporate(S, KB);

    } while (KBprim != KB);
    return KB;
}

//remove unneccesary clauses inc & inc_clause
ClauseSet Incorporate(const ClauseSet& S, ClauseSet& KB) { 
    for (const Clause& A : S) {
        KB = Incorporate_clause(A,KB);
    }
    return KB;
}

ClauseSet Incorporate_clause(const Clause& A, ClauseSet& KB) {
    for (const Clause& B : KB) {
        if (B.subsetOf(A)) return KB; //ignore subsets of A
    }

    for (auto it = KB.begin(); it != KB.end(); ) {
        if (A.subsetOf(*it)) {
            it = KB.erase(it); //removes clauses that subsume A, then add A
        }
        else {
            ++it;
        }
    }

    KB.insert(A);
    return KB;
}

Set Intersection(const Set& a, const Set& b) {

    Set ret;
    for (const auto& el : a) {
        if (b.find(el) != b.end()) {
            ret.insert(el);
        }
    }
    return ret;
}

Set Union(const Set& a, const Set& b) {
    Set ret = a;
    ret.insert(b.begin(), b.end()); 
    return ret;
}

ClauseSet readClausesFromInput() {
    ClauseSet clauses;
    int numClauses;

    std::cout << "How many clauses? ";
    std::cin >> numClauses;
    std::cin.ignore(); 

    std::cout << "\n";

    for (int i = 1; i <= numClauses; ++i) {
        Clause clause;
        std::string line;

        std::cout << "Enter positive literals for clause " << i << ": ";
        std::getline(std::cin, line);
        size_t start = 0, end;
        while ((end = line.find(',', start)) != std::string::npos) {
            std::string literal = line.substr(start, end - start);
            if (!literal.empty()) clause.p.insert(literal);
            start = end + 1;
        }

        std::string lastPos = line.substr(start);
        if (!lastPos.empty()) clause.p.insert(lastPos);

        std::cout << "Enter negative literals for clause " << i << ": ";
        std::getline(std::cin, line);
        start = 0;
        while ((end = line.find(',', start)) != std::string::npos) {
            std::string literal = line.substr(start, end - start);
            if (!literal.empty()) clause.n.insert(literal);
            start = end + 1;
        }
        std::string lastNeg = line.substr(start);
        if (!lastNeg.empty()) clause.n.insert(lastNeg);

        clauses.insert(clause);
        std::cout << "\n";
    }

    return clauses;
}

std::string getRandomElement(const Set& s) {
    if (s.empty()) throw std::runtime_error("Set is empty");

    // Generate random index
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, s.size() - 1);

    size_t index = dist(gen);

    // Advance iterator to that index
    auto it = s.begin();
    std::advance(it, index);
    return *it;
}