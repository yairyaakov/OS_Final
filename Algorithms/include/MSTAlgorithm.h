#ifndef MSTALGORITHM_H
#define MSTALGORITHM_H
#include <utility>
#include "../../DataStructures/Include/MST.h"

class MSTAlgorithm {
protected:
    Graph graph;

public:
    virtual ~MSTAlgorithm() = default;

    explicit MSTAlgorithm(const Graph &graph) : graph((graph)) {};

    virtual MST findMST() = 0;
 };


#endif //MSTALGORITHM_H
