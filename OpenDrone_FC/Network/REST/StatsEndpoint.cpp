#include "StatsEndpoint.h"
#include "json.hpp"

PID *pidInstance = PID::getInstanceCreated();
//Calibration *c = new Calibration(pidInstance->getOrientatin());

StatsEndpoint::StatsEndpoint()
{
}

StatsEndpoint::~StatsEndpoint()
{
}

StatsEndpoint::StatsEndpoint(Address addr) : httpEndpoint(std::make_shared<Http::Endpoint>(addr))
{ }

void StatsEndpoint::init(size_t thr) {
    thr = 2;
    auto opts = Http::Endpoint::options()
        .threads(thr)
        .flags(Tcp::Options::InstallSignalHandler);
    httpEndpoint->init(opts);
    setupRoutes();
}

void StatsEndpoint::start() {
    //c = new Calibration(pidInstance->getOrientatin());
    httpEndpoint->setHandler(router.handler());
    httpEndpoint->serve();
}

void StatsEndpoint::shutdown() {
    cout << "Shutdown REST";
    httpEndpoint->shutdown();
}

//---------------------- Space for REST-handling methods------------------------
static void handleSteering(const Rest::Request& request, Http::ResponseWriter response) {
    json::JSON input;
    input = input.Load(request.body());

    json::JSON obj;
    pidInstance->setThrottle(input["throttle"].ToInt());
    pidInstance->setYawSetpoint(input["Yaw"].ToInt());
    pidInstance->setPitchSetpoint(input["Pitch"].ToInt());
    pidInstance->setRollSetpoint(input["Roll"].ToInt());

    response.send(Http::Code::Ok, "ok");
}

static void handleArmPOST(const Rest::Request& request, Http::ResponseWriter response) {
    json::JSON obj;
    obj["armed"] = true;

    pidInstance->armMotor();
    pidInstance->setRun(true);

    stringstream ss;
    ss << obj << std::endl;

    response.send(Http::Code::Ok, ss.str());
}

static void handleArmGET(const Rest::Request& request, Http::ResponseWriter response) {
    json::JSON obj;
    obj["armed"] = false;

    //TODO

    stringstream ss;
    ss << obj << std::endl;

    response.send(Http::Code::Ok, ss.str());
}

static void handleCalibrationPOST(const Rest::Request& request, Http::ResponseWriter response) {
    json::JSON obj;
    obj["calibration"] = "set";

    /*
    c->calibrate();*/

    stringstream ss;
    ss << obj << std::endl;

    response.send(Http::Code::Ok, ss.str());
}

static void handleCalibrationGET(const Rest::Request& request, Http::ResponseWriter response) {
    json::JSON obj;

    //TODO:

    stringstream ss;
    ss << obj << std::endl;

    response.send(Http::Code::Ok, ss.str());
}

static void handleCrash(const Rest::Request& request, Http::ResponseWriter response) {
    json::JSON obj;
    obj["crash"] = true;

    //TODO:

    stringstream ss;
    ss << obj << std::endl;

    response.send(Http::Code::Ok, ss.str());
}

static void handleInterrupt(const Rest::Request& request, Http::ResponseWriter response) {
    json::JSON obj;
    obj["interrupt"] = true;
    
    pidInstance->interruptPid();

    stringstream ss;
    ss << obj << std::endl;

    response.send(Http::Code::Ok, ss.str());
}

static void handleShutdown(const Rest::Request& request, Http::ResponseWriter response) {
    json::JSON obj;
    obj["shutdown"] = true;

    stringstream ss;
    ss << obj << std::endl;
    
    response.send(Http::Code::Ok, ss.str());
    exit(0);
}

static void handlePIDGET(const Rest::Request& request, Http::ResponseWriter response) {
    json::JSON obj;

    float *ar = pidInstance->getPIDCurrentValues();
    obj["P"] = ar[0];
    obj["I"] = ar[1];
    obj["D"] = ar[2];

    stringstream ss;
    ss << obj << std::endl;

    response.send(Http::Code::Ok, ss.str());
}

static void handlePIDPOST(const Rest::Request& request, Http::ResponseWriter response) {
    json::JSON input;
    input = input.Load(request.body());

    pidInstance->setP(input["P"].ToInt());
    pidInstance->setI(input["I"].ToInt());
    pidInstance->setD(input["D"].ToInt());


    json::JSON obj;

    float *ar = pidInstance->getPIDCurrentValues();
    obj["P"] = ar[0];
    obj["I"] = ar[1];
    obj["D"] = ar[2];

    stringstream ss;
    ss << obj << std::endl;

    response.send(Http::Code::Ok, ss.str());
}

//------------------------------------------------------------------------------

void StatsEndpoint::setupRoutes() {
    using namespace Rest;
    Routes::Post(router, "/api/steering", Routes::bind(handleSteering));
    Routes::Post(router, "/api/arm", Routes::bind(handleArmPOST));
    Routes::Get(router, "/api/arm", Routes::bind(handleArmGET));
    Routes::Post(router, "/api/calibration", Routes::bind(handleCalibrationPOST));
    Routes::Get(router, "/api/calibration", Routes::bind(handleCalibrationGET));
    Routes::Post(router, "/api/crash", Routes::bind(handleCrash));
    Routes::Post(router, "/api/interrupt", Routes::bind(handleInterrupt));
    Routes::Post(router, "/api/shutdown", Routes::bind(handleShutdown));
    Routes::Get(router, "/api/pid", Routes::bind(handlePIDGET));
    Routes::Post(router, "/api/pid", Routes::bind(handlePIDPOST));

}


