#include <stdio.h>

#include <boost/bind.hpp>
#include <cetty/service/ClientService.h>
#include <cetty/gearman/builder/GearmanClientBuilder.h>
#include <cetty/gearman/GearmanMessage.h>

using namespace cetty::channel;
using namespace cetty::gearman;
using namespace cetty::bootstrap;
using namespace cetty::service;
using namespace cetty::gearman::builder;

class GearmanEchoClient {
private:
    typedef ::cetty::service::ServiceFuture<GearmanMessagePtr> GearmanServiceFuture;
    typedef boost::intrusive_ptr<GearmanServiceFuture> GearmanServiceFuturePtr;

public:
    GearmanEchoClient(const ClientServicePtr& s): service(s),count(0) {
        future = new GearmanServiceFuture();
        future->addListener(boost::bind(&GearmanEchoClient::replied, this, _1, _2));
    }
    virtual ~GearmanEchoClient() {}

    void sendRequest() {
        ChannelBufferPtr buf = ChannelBuffers::buffer(10);
        buf->writeBytes("hello");

        callMethod(service,GearmanMessage::createsubmitJobMessage("test","1234",buf),future);

        service->write(ChannelMessage(
                           GearmanMessage::createsubmitJobMessage("test","1234",buf)));
    }

    void replied(const GearmanServiceFuturePtr& f, const GearmanMessagePtr& resp) {
        ++count;

        if (count < 10000) {
            sendRequest();
        }
    }

private:
    int count;
    ClientServicePtr service;
    GearmanServiceFuturePtr future;
};

int main(int argc, char* argv[]) {
    GearmanClientBuilder builder;
    builder.addConnection("192.168.1.162", 4730);

    ClientServicePtr service = builder.build();
    GearmanEchoClient client(service);
    client.sendRequest();
    service->getCloseFuture()->awaitUninterruptibly();
}
