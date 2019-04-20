#pragma once
#include "../../Controller/PID.h"
#include "../../Controller/Calibration.h"

#include <algorithm>
#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>

using namespace std;
using namespace Pistache;

class StatsEndpoint
{
public:
    StatsEndpoint();
    StatsEndpoint(Address addr);
    ~StatsEndpoint();
    void shutdown();
    void start();
    void init(size_t thr);
    void setupRoutes();
    
private:
    std::shared_ptr<Http::Endpoint> httpEndpoint;
    Rest::Router router;
};

