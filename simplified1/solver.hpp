/*
 *  Do "business logic":
 *  1) solve the given quadratic equation;
 *  2) find the vertex of the given parabola.
 *  output into given ostream.
 */

#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <math.h>
#include <iostream>

using namespace std;

class solver {
public:
    solver(ostream& out): out(out) {
        // output numeric format is set here:
        out.precision(4);
        out << fixed;
    }

    void operator()(const long a, const long b, const long c) {
        out << "(" << a << "," << b << "," << c << ")\t=> roots: "; // input values
        equation(a,b,c);
        out << "; vertex: ";  // separator
        extremum(a,b,c);
        out << endl; // end of the record
    }

protected:
    // solve the quadratic equation - write list of roots
    void equation(const long a, const long b, const long c) {
        // specific case:
        if (a==0) { // line, not parabola!
            if (b==0) { // horizontal line
                if (c==0) { // it is the X-axis
                    out << "(ANY)"; // any number is the solution
                } else {
                    out << "()"; // no solution//
                }
            } else { // "normal" line
                const double x = -c/b;
                out << "(" << x << ")";
            }
            return;
        }

        long long d = b*b-4*a*c;
        if (d<0) {
            out << "()";    // no roots (empty list)
        } else if (d==0) {  // a single root
            const double x = -b/2/a;
            out << "(" << x << ")";
        } else { // d>0; a couple of roots
            const double e = sqrt(d);
            const double x1 = (-b-e)/2/a;
            const double x2 = (-b+e)/2/a;
            out << "(" << x1 << "," << x2 << ")";
        }
    }

    // find the vertex of the parabola - write the coordinates
    void extremum(const long a, const long b, const long c) {
        if (a==0) { // line, not parabola
            out << "no vertex!";
            return;
        }
        const double x = -b/2.0/a;
        const double y = a*x*x+b*x+c;
        out << "X=" << x << " Y=" << y;
    }

protected:
    ostream& out;
};

#endif // SOLVER_HPP
