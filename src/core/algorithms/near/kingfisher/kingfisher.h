#pragma once

#include "algorithms/near/near_discovery.h"

namespace algos {

class Kingfisher : public NeARDiscovery {
private:
    void RegisterOptions();
    void ResetState() override;
    unsigned long long ExecuteInternal() override;

public:
    Kingfisher();
};

} // namespace algos
