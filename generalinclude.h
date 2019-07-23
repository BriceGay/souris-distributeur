#ifndef GENERALINCLUDE_H
#define GENERALINCLUDE_H

#include <qdebug.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <vector>
#include <QString>

using namespace std;

struct Souris {
   QString name;
   double taille;
   bool operator < (Souris & s) {//ça va faire des bugs ça non ?
       return taille < s.taille;
   }
};

#endif // GENERALINCLUDE_H
