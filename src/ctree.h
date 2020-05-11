#ifndef CTREE_H_INCLUDED
#define CTREE_H_INCLUDED
#include "input.h"
#include "colormod.h"
#include <vector>
using namespace std;

class Ctree{
    public:
        Ctree(Input *input);
    // private:
        Input *in;
};

#endif //CTREE_H_INCLUDED