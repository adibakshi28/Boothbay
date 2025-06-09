#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

// trim whitespace
string trim(const string &s) {
    int start = 0;
    while (start < s.size() && isspace(s[start])) {
        start++;
    }
    if (start == s.size()) {
        return "";
    }
    int end = s.size() - 1;
    while (end > start && isspace(s[end])) {
        end--;
    }
    return s.substr(start, end - start + 1);
}

// check if only contain digits
bool isNumber(const string &s) {
    if (s.empty()) {
        return false;
    }
    for (int i = 0; i < s.size(); i++) {
        if (!isdigit(s[i])) {
            return false;
        }
    }
    return true;
}

struct Node {
    string name;
    bool isScale;
    long mass;      // only if !isScale
    Node *left, *right;
    long addLeft, addRight, total;

    Node(const string& n, bool scale) {
        name = n;
        isScale = scale;
        if (scale) {
            mass = 0;
        } else {
            mass = stol(n);
        }
        left = nullptr;
        right = nullptr;
        addLeft = 0;
        addRight = 0;
        total = 0;
    }
};

// post order DFS
long dfs(Node* cur) {
    if (!cur->isScale) {
        cur->total = cur->mass;
        return cur->total;
    }
    long leftWt = dfs(cur->left);
    long rightWt = dfs(cur->right);
    if (leftWt > rightWt) {
        cur->addRight = leftWt - rightWt;
    } else {
        cur->addLeft = rightWt - leftWt;
    }
    cur->total = 1 + (leftWt + cur->addLeft) + (rightWt + cur->addRight);
    return cur->total;
}

// comparator() for sorting by name
struct ByName {
    bool operator()(Node* a, Node* b) const {
        return a->name < b->name;
    }
};

// attach left or right child
bool attachChild(const string &tok, map<string,Node*> &nodes, set<string> &scales, set<string> &children, Node*& ptr, int lineNo) {
    if (isalpha(tok[0])) { // Scale
        for (int i = 0; i < tok.size(); i++) {
            if (!isalnum(tok[i])) {
                cerr << "Error on line " << lineNo << ": invalid character in scale name: '" << tok << endl;
                return false;
            }
        }
        if (nodes.count(tok) == 0) {
            nodes[tok] = new Node(tok, true);
        }
        ptr = nodes[tok];
        scales.insert(tok);
        children.insert(tok);
    } else { // Mass
        if (!isNumber(tok)) {
            cerr << "Error on line " << lineNo << ": invalid mass (not a non negative integer): '" << tok << endl;
            return false;
        }
        ptr = new Node(tok, false);
    }
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    map<string,Node*> nodes;
    set<string> scales, children;
    string rawLine;
    int lineNo = 0;

    // parse input & build tree
    while (getline(cin, rawLine)) {
        lineNo++;
        // skip empty or comment
        if (rawLine.empty() || rawLine[0] == '#') {
            continue;
        }

        // remove tabs and CR
        string line;
        for (int i = 0; i < rawLine.size(); i++) {
            if (rawLine[i] != '\r' && rawLine[i] != '\t'){
                line.push_back(rawLine[i]);
            }
        }

        // locate two commas
        int p1 = line.find(',');
        int p2 = (p1 == string::npos ? string::npos : line.find(',', p1+1));
        if (p1 == string::npos || p2 == string::npos) {
            cerr << "Error on line " << lineNo << ": must contain exactly two commas."<<endl;
            return 1;
        }

        // split and trim
        string rawN = line.substr(0, p1);
        string rawL = line.substr(p1+1, p2-p1-1);
        string rawR = line.substr(p2+1);

        string name = trim(rawN);
        string L = trim(rawL);
        string R = trim(rawR);

        if (name.empty() || L.empty() || R.empty()) {
            cerr << "Error on line " << lineNo << ": empty field detected."<<endl;
            return 1;
        }
        if (!isalpha(name[0])) {
            cerr << "Error on line " << lineNo << ": scale name must start with a letter: '" << name << endl;
            return 1;
        }

        // ensure scale node exists
        if (nodes.count(name) == 0) {
            nodes[name] = new Node(name, true);
        }
        scales.insert(name);
        Node* cur = nodes[name];

        // attach its two children
        if (!attachChild(L, nodes, scales, children, cur->left, lineNo)) {
            return 1;
        }
        if (!attachChild(R, nodes, scales, children, cur->right, lineNo)) {
            return 1;
        }
    }

    // find unique root
    if (scales.empty()) {
        cerr << "Error: no scales defined."<<endl;
        return 1;
    }
    string rootName;
    for (set<string>::iterator it = scales.begin(); it != scales.end(); ++it) {
        if (children.count(*it) == 0) {
            if (!rootName.empty()) {
                cerr << "Error: multiple roots detected ('" << rootName << "' and '" << *it << "')."<<endl;
                return 1;
            }
            rootName = *it;
        }
    }
    if (rootName.empty()) {
        cerr << "Error: no root found (cycle?)."<<endl;
        return 1;
    }

    // balance bottom up
    try {
        dfs(nodes[rootName]);
    } catch (const exception &e) {
        cerr << "Internal error during balancing: " << e.what() << endl;
        return 1;
    }

    // collect, sort, print
    vector<Node*> out;
    for (auto it = nodes.begin(); it != nodes.end(); ++it) {
        if (it->second->isScale) {
            out.push_back(it->second);
        }
    }
    sort(out.begin(), out.end(), ByName());

    for (int i = 0; i < out.size(); i++) {
        Node* s = out[i];
        cout << s->name << ',' << s->addLeft << ',' << s->addRight << endl;
    }
    return 0;
}
