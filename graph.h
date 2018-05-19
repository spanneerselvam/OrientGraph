#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <cassert>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <stack>
#include <utility>
#include "digraph.h"
class graph
{
public:
    typedef std::string T;
    typedef std::unordered_set<T> AdjacentType;
    typedef std::pair<T, T> Edge;



    graph()
    {
    }

    // pre: none
    // post: returns number of vertices in this graph
    std::size_t n() const
    {
        return _t.size();
    }

    // pre: none
    // post: returns number of edges in this graph
    std::size_t m() const
    {
        std::size_t ans(0);

        for (auto e: _t)
            ans += e.second.size();

        return ans/2;

    }

    // pre: v is a vertex of this graph
    // post: returns the number of vertices adjacent to v in this graph
    std::size_t deg(const T & v) const
    {
        assert(_t.find(v) != _t.end());
        return _t.at(v).size();
        // t[v].size()
    }

    // pre:  v is NOT a vertex of this graph
    // post: new vertex v has been added to this graph
    void add_vertex(const T & v)
    {
        assert(_t.find(v) == _t.end());
        _t[v] = AdjacentType();
    }

    // pre: s, d are vertices of this graph and {s, d} is not an edge
    // post: {s, d} has been added to this graph
    void add_edge(const T & s, const T & d)
    {
        assert(_t.find(s) != _t.end() && _t.find(d) != _t.end());
        _t[s].insert(d);
        _t[d].insert(s);
    }

    // pre: s and d are vertices and {s, d} is an edge
    // post: {s, d} has been removed from this graph
    void remove_edge(const T & s, const T & d)
    {
        assert(_t.find(s) != _t.end() && _t.find(d) != _t.end());
        _t[s].erase(d);
        _t[d].erase(s);
    }

    // pre: v is a vertex
    // post: v and edges with v as an endpoint have been removed from this graph
    void remove_vertex(const T & v)
    {
        assert(_t.find(v) != _t.end());
        for (auto e: _t)
            e.second.erase(v);
        _t.erase(v);
    }

    // pre: none
    // post: returns a vector containing the vertices of this graph
    std::vector<T> V() const
    {
        std::vector<T> ans;

        for (auto e: _t)
            ans.push_back(e.first);

        return ans;
    }

    // pre: v is a vertex of this graph
    // post: the vertices adjacent to v are returned
    AdjacentType Adj(const T & v) const
    {
        assert(_t.find(v) != _t.end());

        return _t.at(v);
    }

   // pre: a is a vertex, visited contains vertices visited in previous calls to bfs
   // post: visited updated to include vertices reachable from a
   graph bfs(const T & a, std::unordered_set<T> & visited) const
    {
        assert(_t.find(a) != _t.end());
        std::queue<T> q;
        graph ans;

        // initialization
        visited.insert(a);
        q.push(a);
        ans.add_vertex(a);

        while (!q.empty())
        {
            T front = q.front();
            q.pop();

            std::size_t count(0);
            for (auto w: Adj(front))
            {
                if (visited.insert(w).second) // w has not been visited
                {
                    q.push(w);
                    ans.add_vertex(w);
                    ans.add_edge(front, w);
                }
            }
        }
        return ans;
    }

   // pre: none
   // post: returns the number of connected components of this graph
    std::size_t nc() const
    {
        std::size_t ans(0);
        std::unordered_set<T> visited;

        for (auto e: V())
        {
            if (visited.find(e) == visited.end())
            {
                bfs(e, visited);
                ++ans;
            }
        }

        return ans;
    }

   // pre: a and b are vertices
   // post: true iff there is a path between a and b
   bool is_reachable (const T & a, const T & b) const
   {
       assert(_t.find(a) != _t.end() && _t.find(b) != _t.end());

       std::unordered_set<T> visited;

       bfs(a, visited);

       return (visited.find(b) != visited.end());

   }

   // pre: none
   // post: true iff this graph is connected
   bool is_connected() const
   {
       std::unordered_set<T> visited;
       bfs(_t.begin()->first, visited);

       return visited.size() == n();

   }

   void dfs(const T & v)
   {
       std::stack<T> s;
       std::unordered_map<T, T> parent;
       std::unordered_map<T, std::size_t> pre, post;

       std::size_t time(1);

       s.push(v);

       while (!s.empty())
       {
           T top = s.top();
           if (pre.find(top) == pre.end()) // has not been discovered
           {
               pre[top] = time++;
               AdjacentType neighbors = Adj(top);
               for (auto n: neighbors)
                   if (pre.find(n) == pre.end())
                   {
                       s.push(n);
                       parent[n] = top;
                   }
           }
           else
           {
               if (post.find(top) == post.end())
                   post[top] = time++;
               s.pop();
           }
       }


       std::cout << "Parent:" << std::endl;
       for (auto e: parent)
           std::cout << "(" << e.first << ", " << e.second << " ) ";
       std::cout << std::endl;

       std::cout << "Pre:" << std::endl;
       for (auto e: pre)
           std::cout << "(" << e.first << ", " << e.second << " ) ";
       std::cout << std::endl;

       std::cout << "Post: " << std::endl;
       for (auto e: post)
           std::cout << "(" << e.first << ", " << e.second << " ) ";
       std::cout << std::endl;

   }

   void rdfs1(const T & v, std::unordered_map<T, T> & parent,
             std::unordered_map<T, std::size_t> & pre,
             std::unordered_map<T, std::size_t> & post, std::size_t & time)
   {
       pre[v] = time++;
       for (auto n: Adj(v))
       {
           if (pre.find(n) == pre.end())
           {
               parent[n] = v;
               rdfs1(n, parent, pre, post, time);
           }
       }
       post[v] = time++;
   }

   void rdfs(const T & v)
   {
       std::unordered_map<T, T> parent;
       std::unordered_map<T, std::size_t> pre, post;
       std::size_t time(1);


       rdfs1(v, parent, pre, post, time);
       std::cout << "Parent:" << std::endl;
       for (auto e: parent)
           std::cout << "(" << e.first << ", " << e.second << " ) ";
       std::cout << std::endl;

       std::cout << "Pre:" << std::endl;
       for (auto e: pre)
           std::cout << "(" << e.first << ", " << e.second << " ) ";
       std::cout << std::endl;

       std::cout << "Post: " << std::endl;
       for (auto e: post)
           std::cout << "(" << e.first << ", " << e.second << " ) ";
       std::cout << std::endl;

   }

   bool has_eulerian_cycle() const
   {
       if (!is_connected())
           return false;

       for (auto e: V())
       {
           if (deg(e) % 2 != 0)
               return false;
       }
       return true;
   }

   std::vector<T> eulerian_cyle() const
   {
       std::vector<T> ans;
       std::stack<T> S;

       std::unordered_map<T, AdjacentType> A;

       for (auto v: V())
           A[v] = Adj(v);

       // the first vertex in the graph
       T v = _t.begin()->first;

       bool done(false);

       while (!done)
       {
           while (true)
           {
               if (A[v].size() == 0)  // no more free edges to take
                   break;

               S.push(v);

               T w = *(A[v].begin());
               A[v].erase(w);
               A[w].erase(v);
               v = w;
           }
           if (S.empty())
               done = true;

           else
           {
               v = S.top();
               S.pop();
               ans.push_back(v);

           }
       }
       return ans;
   }

 digraph orient() const
{	
	digraph G;
	bool bridges(false);
	bridges = has_eulerian_cycle();

	if(bridges == false)
	{
		return G;
	}
	else
	{
		std::vector<T> vertices = V();
		std::vector<T> ans = eulerian_cyle();
		std::vector<T>::iterator itr;
		for(auto e: vertices)
			G.add_vertex(e);
		
		for(itr = ans.begin()+1; itr < ans.end(); ++itr)
		{
			G.add_edge(*(itr-1), *(itr));
			//std::cout << *(itr-1) << ", " <<*itr << std::endl;
		}
		T front = *(ans.end()-1);

		//std::vector<T>::iterator it;
		//it = _t.at(front);
		AdjacentType y = Adj(front);
		std::vector<T> x;
		for(auto e: y)
			x.push_back(e);
		
		G.add_edge(front, x[1]);
		return G;
	}
}
   void fb(const T & a, const T & b, std::unordered_map<T, std::size_t> & pre,
           std::unordered_map<T, std::size_t> &low, std::size_t & time,
           std::vector<Edge> & bridges,
           std::vector<T> & ap) const
   {
       pre[b] = low[b] = time++;
       bool found(false);
       std::size_t nc(0);
       for (auto n: Adj(b))
       {
           if (pre.find(n) == pre.end())
           {
               ++nc;
               fb(b, n, pre, low, time, bridges, ap);
               low[b] = std::min(low[b], low[n]);
               if (low[n] == pre[n])
                   bridges.push_back(Edge(b, n));
               if (low[n] >= pre[b])
                   found = true;
           }
           else if (n != a)
               low[b] = std::min(low[b], pre[n]);

       }
       if (a != b && found || a == b && nc > 1)
           ap.push_back(b);
   }

   std::vector<Edge> find_bridges() const
   {
       std::unordered_map<T, std::size_t> pre, low;
       std::size_t time(1);
       std::vector<Edge> bridges;
       std::vector<T> ap;

       for (auto v: V())
       {
           if (pre.find(v) == pre.end())
               fb(v, v, pre, low, time, bridges, ap);
       }

       return bridges;
   }

   std::vector<T> find_ap() const
   {
       std::unordered_map<T, std::size_t> pre, low;
       std::size_t time(1);
       std::vector<Edge> bridges;
       std::vector<T> ap;

       for (auto v: V())
       {
           if (pre.find(v) == pre.end())
               fb(v, v, pre, low, time, bridges, ap);
       }

       return ap;
   }


private:
    std::unordered_map<T, AdjacentType> _t;


};

std::ostream & operator << (std::ostream & os, const graph & G)
{
    os << G.n() << " " << G.m() << std::endl;
    for (auto v: G.V())
        os << v << " ";
    os << std::endl;
    for (auto v: G.V())
    {
        os << v << ": ";
        for (auto e: G.Adj(v))
            os << e << " ";
        os << std::endl;
    }
    return os;
}
std::istream & operator >> (std::istream & is, graph & G)
{
    std::size_t n, m;
    is >> n >> m;
    std::string v, w;

    for (std::size_t i = 1; i <= n; ++i)
    {
        is >> v;
        G.add_vertex(v);
    }
    for (std::size_t i = 1; i <= m; ++i)
    {
        is >> v >> w;
        G.add_edge(v, w);
    }
    return is;
}

#endif // GRAPH_H

