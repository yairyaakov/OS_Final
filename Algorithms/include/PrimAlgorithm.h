
#ifndef PRIMALGORITHM_H
#define PRIMALGORITHM_H
#include "MSTAlgorithm.h"


class PrimAlgorithm : public MSTAlgorithm {
public:
    explicit PrimAlgorithm(const Graph &graph): MSTAlgorithm(graph) {} ;
    ~PrimAlgorithm() override = default ;
     MST findMST() override;


};


#endif //PRIMALGORITHM_H
