#ifndef MSTFACTORY_H
#define MSTFACTORY_H

#include "../../Algorithms/include/MSTAlgorithm.h"
#include "../../Algorithms/include/PrimAlgorithm.h"
#include "../../Algorithms/include/KruskalAlgorithm.h"
#include <memory>
#include <string>

class MSTFactory {
public:
    static std::unique_ptr<MSTAlgorithm> createAlgorithm(const std::string& type, const Graph& graph) {
        if (type == "prim") {
            return std::make_unique<PrimAlgorithm>(graph);
        }
        if (type == "kruskal") {
            return std::make_unique<KruskalAlgorithm>(graph);
        }
        throw std::invalid_argument("Unknown MST algorithm type.");
    }
};

#endif // MSTFACTORY_H
