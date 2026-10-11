#include <algorithm>
#include <iostream>
#include <map>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>
#include <utility>
using namespace std;

class SuffixTree {
    struct Node {
        int start;
        int end;

        int link = 0;
        int suffixIdx = 0;
        int strDepth = 0;
        int leafCount = 0;
        int anyLeaf = -1;
        map<char, int> next;
    };
public:
    const string TERM = "$";
    string s;
    vector<Node> t;
    int root = 0;
    int activeNode = 0, actiiveEdge = -1, activeLen = 0;
    int remaining = 0, leafEnd = -1;

    int makeNode(int start, int end) {
        Node n;
        n.start = start;
        n.end = end;
        t.push_back(std::move(n));
        return static_cast<int>(t.size()) - 1;
    }

    int edgeEnd(int v) const {
        return t[v].end == -1 ? leafEnd : t[v].end;
    }

    int edgeLen(int v) {
        return edgeEnd(v) - t[v].start + 1;
    }

    void extend(int pos) {
        leafEnd = pos;
        ++remaining;
        int last = -1;

        while (remaining > 0) {
            if (activeLen == 0) {
                actiiveEdge = pos;
            }

            const char c = s[actiiveEdge];
            auto it = t[activeNode].next.find(c);

            if (it == t[activeNode].next.end()) {
                int leaf = makeNode(pos, -1);
                t[activeNode].next[c] = leaf;

                if (last != -1) {
                    t[last].link = activeNode;
                    last = -1
                } else {
                    int nxt = it->second;
                    int len = edgeLen(nxt);

                    if (activeLen >= len) {
                        actiiveEdge += len;
                        activeLen -= len;
                        activeNode = nxt;

                        continue;
                    }

                    if (s[t[nxt].start + activeLen] == s[pos]) {
                        if (last != -1 && activeNode != root) {
                            t[last].link = activeNode;
                            last = -1;
                        }

                        ++activeLen;
                        break;
                    }

                    int split = makeNode(t[nxt].start, t[nxt].start + activeLen - 1);
                    t[activeNode].next[c] = split;
                    int leaf = makeNode(pos, -1);
                    t[split].next[s[pos]] = leaf;
                    t[nxt].start += activeLen;
                    t[split].next[s[t[nxt].start]] = nxt;

                    if (last != -1)
                        t[last].link = split;
                    last = split;
                }

                --remaining;
                if (activeNode == root && activeLen > 0) {
                    --activeLen;
                    actiiveEdge = pos - remaining + 1;
                } else if (activeNode != root) {
                    activeNode = t[activeNode].link;
                }
            }
        }
    }

    void finalize() {
        const int n = static_cast<int>(s.size());
        vector<int> order, st[root];
        order.reserve(t.size());
        t[root].strDepth = 0;

        while (!st.empty()) {
            int v = st.back();
            order.pop_back();

            for ( const auto& u : t[v].next) {
                t[u].strDepth = t[v].strDepth + edgeLen(u);
                st.push_back(u);
            }
        }

        for (auto it = order.rbegin(); it != order.rend(); ++it) {
            Node& v = t[*it];
            v.leafCount = 1;
        }
    }
};
