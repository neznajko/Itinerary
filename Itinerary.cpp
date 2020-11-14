/*//////////////////////////////////////////////////////////////////////

PROBLEM 4

You have won a contest sponsored by an airline. The prize is a ticket to
travel around Canada, beginning in the most western point served by this
airline, then traveling only from west  to east until you reach the most
eastern point served, and then  coming back only from east to west until
you reach the starting city.

No city may  be visited  more than once, except  for the  starting city,
which must be visited exactly twice (at the beginning and the end of the
trip).  You are not allowed to use any  other airline or any other means
of transportation. Given a list of  cities served  by the airline, and a
list  of direct flights between pairs of cities, find an itinerary which
visits as  many cities  as possible  and satisfies  the above conditions
beginning with the first city and visiting the last city on the list and
returning to the first city.

Different data sets are written in an ASCII input file, C:\IOI\ITIN.DAT.
Each data set consists of:
> in  the first line: the  number N  of cities served by the airline and
  the  number  V of  direct  flights that  will be  listed.  N will be a
  positive integer not larger than 100. V is any positive integer.
> in each of the next N lines: a name of a city served by the airline.

The names are  ordered from west to east in the input file. That is, the
i-th city  is west of the  j-th city  if and only if i < j (There are no
two cities in the same meridian). The  name of each city is a string of,
at most, 15 digits and/or characters of the Latin alphabet, for example:
AGR34 or BEL4 (There are no spaces in the name of a city):
> in each of the next V lines: two names of cities, taken  from the list
  of cities, separated by a blank space.

If the  pair city1  city2 appears  in a  line, it  indicates  that there
exists a direct flight from city1 to city2 and also a direct flight from
city2 to city1. Different data sets will be separated by an empty record
(that is, a line containing only the end of line character).  There will
be no empty record after the last data set.

The following example is stored in file C:\IOI\ITIN.DAT.

8 9
Vancouver
Yellowknife
Edmonton
Calgary
Winnipeg
Toronto
Montreal
Halifax
Vancouver Edmonton
Vancouver Calgary
Calgary Winnipeg
Winnipeg Toronto
Toronto Halifax
Montreal Halifax
Edmonton Montreal
Edmonton Yellowknife
Edmonton Calgary

5 5
C1
C2
C3
C4
C5
C5 C4
C2 C3
C3 C1
C4 C1
C5 C2

The input may be assumed correct. No checking is necessary.

The solution found for each  data set must be written to an ASCII output
file,  C:\IOI\ITIN.SOL: in the first line, the total number of cities in
the input  data set; in the  next line, the number M of different cities
visited in the itinerary, and  in the next  M+1  lines the names  of the
cities , one per  line, in the order in which they are visited. Note the
first city visited must be  the same as the  last . Only one solution is
required. If  no solution is  found for a data set, only two records for
this data set  must be written  in ITIN.SOL, the  first one  giving  the
total number of cities, and the second one saying: "NO SOLUTION".

A possible solution for the above example, ITIN.SOL:

8
7
Vancouver
Edmonton
Montreal
Halifax
Toronto
Winnipeg
Calgary
Vancouver

5
NO SOLUTION

Put your  program  solution  into an  ASCII  file named  C:\IOI\DDD.xxx.
Extension  .xxx is  .BAS for Qbasic,  .LCN  for LOGO, .C for C, .PAS for
PASCAL.

*///////////////////////////////////////////////////////////////////////
#include <fstream>
#include <string>
#include <map>
#include "MaLib.hpp"
using namespace std;
typedef vector<int> vint;
class Itinerary {
  int nV; // nof vertices
  int nE; // nof edges
  vector<string> cities;
  map<string, int> lookup; // city -> index tab
  vector<vint> g; // graph
  int back; // Halifax
  vint prev; // used both for visited stk and path history
public:
  vector<vint> fwrd_stk; // West-East stack
  vector<vint> path_stk; // Itinerary stack
  void reset() {
    back = nV - 1;
    prev.clear();
    prev.resize(nV, -1);
    fwrd_stk.clear();
    cities.clear();
    cities.resize(nV);
    lookup.clear();
    for (auto &bar: g) {
      bar.clear();
    }
    g.clear();
    g.resize(nV, vint(nV));
    path_stk.clear();
  }
  friend istream& operator>> (istream& is, Itinerary& it) {
    int & nV = it.nV;
    int & nE = it.nE;
    if (!(is >> nV >> nE)) return is;
    it.reset();
    vector<string> & cities = it.cities;
    map<string, int> & lookup = it.lookup;
    vector<vint> & g = it.g;
    for (int i = 0; i < nV; ++i) {
      string & city = cities[i];
      is >> city;
      lookup[city] = i;
    }
    for (int i = 0; i < nE; ++i) {
      string city;
      is >> city;
      int u = lookup[city];
      is >> city;
      int v = lookup[city];
      g[u][v] = g[v][u] = 1;
    }
   return is;
  }
  friend ostream& operator<< (ostream& os, const Itinerary& it) {
    os << it.cities << endl;
    os << "graph\n";
    for (int i = 0; i < it.nV; ++i) {
      cout << it.g[i] << endl;
    }
    return os;
  }
  void fwrd(int u) {
    if (u == back) {
      fwrd_stk.push_back(prev);
    } else {
      for (int v = u + 1; v < nV; ++v) {
        if (g[u][v] && prev[v] == -1) {
          prev[v] = u;
          fwrd(v);
        }
      }
    }
    prev[u] = -1;
  }
  void bwrd(int u, vint& prev) {
    if (u == 0) {
      path_stk.push_back(prev);
    } else {
      for (int v = 0; v < u; ++v) {
        if (g[u][v] && prev[v] == -1) {
          prev[v] = u;
          bwrd(v, prev);
        }
      }
    }
    prev[u] = -1;
  }
  void cycle() {
    fwrd(0); // fill fwrd_stk
    for (auto &prev: fwrd_stk) {
      bwrd(back, prev);
    }
  }
  /* Reversed */
  vector<string> getItinerary(const vint& prev) {
    vector<string> itin;
    int i{ 0 };
    do {
      itin.push_back(cities[i]);
    } while ((i = prev[i]));
    return itin;
  }
};
int test() {
  return 1;
}
int main(int argc, char *argv[]) {
  if (0) { return test(); }
  ifstream ifs("ITIN.DAT");
  Itinerary it;
  while (ifs >> it) {
    cout << it;
    it.cycle(); // get itineraries
    if (it.path_stk.empty()) {
      cout << "NO SOLUTION\n";
    } else {
      int mxlen{ 0 };
      int mxj{ -1 };
      int&& siz = it.path_stk.size();
      for (int j = 0; j < siz; ++j) {
        vint& prev = it.path_stk[j];
        vector<string> path = it.getItinerary(prev);
        int len = path.size();
        if (mxlen < len) {
          mxlen = len;
          mxj = j;
        }
      }
      vint& prev = it.path_stk[mxj];
      vector<string> mxpath = it.getItinerary(prev);
      mxpath.push_back(mxpath[0]);
      reverse(mxpath.begin(), mxpath.end());
      cout << mxpath << endl;
    }
  }
  ifs.close();
}
// log:- Ktge, ktge nak BaHkyBtp gtre!
