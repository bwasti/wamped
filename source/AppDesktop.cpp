#include <iostream>
#include "Wamp.h"
#include "WampTransportRaw.h"
#include "WampTransportWS.h"
#include "MsgUnpack.h"

#ifdef __linux__
#include <unistd.h>
#endif

#include "logger.h"



//WampTransportRaw *wt;
WampTransportWS *wt;
Wamp *wamp;

void onConnect() {

}

void empty() {
    LOG("Called empty");
}

uint16_t getCounter() {
    return 42;
}

int sum(int x, int y) {
    return x+y;
}


int main() {

    std::cout << "Hello world!\n";

    //wt = new WampTransportRaw {"localhost"};
    wt = new WampTransportWS {"ws://demo.crossbar.io:8080"};
    wamp = new Wamp (*wt);

    wamp->onClose = ([&]() {
        LOG("Reconnecting");
        sleep(1);
        wamp->connect();
    });

    wamp->connect([&]() {
        LOG("Session joined :" << wamp->sessionID);

        wamp->pub("test", "hello");

        wamp->subscribe("com.example.oncounter", [](MPNode args, MPNode kwargs) {
            (void) kwargs;
            LOG("Received event: " << args);
        });

        wamp->registerProcedure("com.mydevice.sum", sum, [](URI err) {
            if (!err.empty()) {
                LOG("REGISTRATION ERROR:" << err);
            }
            LOG("Registration OK");
        });

        wamp->registerProcedure("com.freedom.getCounter", empty);

        wamp->call("com.example.add", MsgPackArr {20,3}, MsgPackMap{},
                   [](URI err, MPNode args, MPNode kwargs) {
                       (void) kwargs;
                   if (!err.empty()) {
                LOG("Received result:" << args);
            }

        });

    });



    while (true) {
        wt->process();
    }

}