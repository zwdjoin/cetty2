/*
 * Copyright (c) 2010-2011 frankee zhou (frankee.zhou at gmail dot com)
 *
 * Distributed under under the Apache License, version 2.0 (the "License").
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */

#include <cetty/protobuf/service/handler/ProtobufServiceMessageHandler.h>

#include <boost/thread/locks.hpp>
#include <boost/bind.hpp>
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>

#include <cetty/channel/Channel.h>
#include <cetty/channel/ChannelFuture.h>
#include <cetty/channel/ChannelPipeline.h>
#include <cetty/channel/ChannelHandlerContext.h>

#include <cetty/protobuf/service/ProtobufService.h>
#include <cetty/protobuf/service/ProtobufServiceFuture.h>
#include <cetty/protobuf/service/ProtobufServiceRegister.h>
#include <cetty/protobuf/service/ProtobufServiceMessage.h>
#include <cetty/protobuf/service/proto/service.pb.h>

namespace cetty {
namespace protobuf {
namespace service {
namespace handler {

using namespace cetty::channel;
using namespace cetty::service;
using namespace cetty::protobuf::service;
using namespace cetty::protobuf::service::proto;
using namespace google::protobuf;

ProtobufServiceMessageHandler::ProtobufServiceMessageHandler() {
}

ProtobufServiceMessageHandler::~ProtobufServiceMessageHandler() {
}

void ProtobufServiceMessageHandler::messageReceived(ChannelHandlerContext& ctx,
        const ProtobufServiceMessagePtr& msg) {
    const ServiceMessage& rpc = msg->getServiceMessage();

    if (rpc.type() == cetty::protobuf::service::proto::REQUEST) {
        const ProtobufServicePtr& service =
            ProtobufServiceRegister::instance().getService(rpc.service());

        const google::protobuf::MethodDescriptor* method =
            ProtobufServiceRegister::instance().getMethodDescriptor(service,
                    rpc.method());

        boost::int64_t id = rpc.id();

        if (method) {
            service->CallMethod(method,
                                msg->getPayload(),
                                MessagePtr(),
                                boost::bind(&ProtobufServiceMessageHandler::doneCallback,
                                            this,
                                            _1,
                                            boost::ref(ctx),
                                            msg,
                                            id));
        }
        else {
            // return error message
            printf("has no such service or method.\n");
        }
    }
    else if (rpc.type() == cetty::protobuf::service::proto::ERROR) {
    }
    else {
        //ctx.sendUpstream(e);
    }
}

void ProtobufServiceMessageHandler::doneCallback(const MessagePtr& response,
        ChannelHandlerContext& ctx,
        ProtobufServiceMessagePtr req,
        boost::int64_t id) {

    ProtobufServiceMessagePtr message(
        new ProtobufServiceMessage(RESPONSE,
                                      id,
                                      req->getService(),
                                      req->getMethod(),
                                      response));

    outboundTransfer.write(ctx, message, ctx.getChannel()->newSucceededFuture());
}

cetty::channel::ChannelHandlerPtr ProtobufServiceMessageHandler::clone() {
    return ChannelHandlerPtr(new ProtobufServiceMessageHandler());
}

std::string ProtobufServiceMessageHandler::toString() const {
    return "ProtobufServiceMessageHandler";
}

}
}
}
}