#include <iostream>
#include "WampMBED.h"
#include "logger.h"
#include "MpackPrinter.h"
#include "WampTransportRaw.h"
#include "MsgUnpack.h"

WampTransportRaw *wt;
WampMBED *wamp;

static void blinky(void) {

}


int main() {

    std::cout << "Hello world!\n";

    wt = new WampTransportRaw {};
    wamp = new WampMBED (*wt);

    wamp->connect([&]() {
        LOG("Session joined :" << wamp->sessionID);

        wamp->publish("test", MsgPackArr {"hello"}, MsgPackMap {});

        wamp->subscribe("com.example.oncounter", [](mpack_node_t &args, mpack_node_t &kwargs) {
            LOG("Received event: " << MpackPrinter(args).toJSON());
            blinky();
        });
    });

    int n=0;
    while (n>=0) {
        n= wt->process();
    }

}