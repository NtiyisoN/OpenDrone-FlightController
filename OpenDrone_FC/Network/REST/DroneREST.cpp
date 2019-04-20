#include "DroneREST.h"
#include "StatsEndpoint.h"
#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>

using namespace std;
using namespace Pistache;

Port port(6969);
Address addr(Ipv4::any(), port);
StatsEndpoint stats(addr);

void DroneREST::startUp() {
    int thr = 2;

    stats.init(thr);
    stats.start();

    stats.shutdown();
}

void DroneREST::stop() {
    stats.shutdown();
}

DroneREST::~DroneREST()
{
}

DroneREST::DroneREST() {

}