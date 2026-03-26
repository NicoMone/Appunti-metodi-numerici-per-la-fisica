#include <math.h>
float gamdev(int ia, long *idum){
\\ Returns a deviate distributed as a gamma distribution of integer order ia, 
\\ ie, a waiting time to the i-th event in a Poisson process of unit mean, 
\\ using ran1(idum) as the source of uniform deviates.

    float ran1(long *idum);
    void nrerror(char error_text[]);
    int j;
    float am,e,s,v1,v2,x,y;
    if (ia < 1) nrerror("Error in routine gamdev");
    if (ia < 6) {
    \\ Use direct method, adding waiting
    x = 1.0;
    times.
    for (j=1;j<=ia;j++) x *= ran1(idum);
    x = -log(x);
    } 
    else {      \\ Use rejection method.
        do {
            do {
                \\ These four lines generate the tangent of a random angle, 
                \\ ie, they are equivalent to y = tan{π * ran1(idum)}.
                do {
                    v1 = ran1(idum);
                    v2 = 2.0*ran1(idum) - 1.0;
                } while (v1*v1+v2*v2 > 1.0);
                y= v2 / v1;
                am = ia - 1;
                s = sqrt( 2.0*am+1.0 );
                x = s*y + am;
            } while (x <= 0.0);
            e=(1.0+y*y)*exp(am*log(x/am)-s*y);
        } while (ran1(idum) > e);
    }
return x;

\\ We decide whether to reject x: Reject in region of zero probability.
\\ Ratio of prob. fn. to comparison fn. Reject on basis of a second uniform deviate.
}