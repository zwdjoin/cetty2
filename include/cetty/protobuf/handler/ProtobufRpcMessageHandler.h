#if !defined(CETTY_HANDLER_RPC_PROTOBUF_PROTOBUFRPCMESSAGEHANDLER_H)
#define CETTY_HANDLER_RPC_PROTOBUF_PROTOBUFRPCMESSAGEHANDLER_H

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

#include <deque>
#include "boost/cstdint.hpp"
#include "boost/thread/mutex.hpp"
#include "google/protobuf/service.h"
#include "cetty/channel/SimpleChannelHandler.h"
#include "cetty/handler/rpc/protobuf/ProtobufServiceRegister.h"

namespace google {
namespace protobuf {
class MethodDescriptor;
class RpcController;
class Message;
}
}

namespace cetty {
namespace channel {
class Channel;
}
}

namespace cetty {
namespace protobuf {
namespace proto {
class RpcMessage;
}
}
}

namespace cetty {
namespace protobuf {
namespace handler {
using namespace cetty::channel;

class ProtobufRpcMessageHandler;
typedef boost::intrusive_ptr<ProtobufRpcMessageHandler> ProtobufRpcMessageHandlerPtr;

class ProtobufRpcMessageHandler : public cetty::channel::SimpleChannelHandler {
public:
    struct ProtobufRpcMessage {
        cetty::protobuf::proto::RpcMessage* rpc;
        google::protobuf::Message* payload;
    };

    struct OutstandingCall {
        cetty::protobuf::proto::RpcMessage* rpc;
        google::protobuf::Message* response;
        google::protobuf::Closure* done;
        google::protobuf::Message* message;
    };

public:
    ProtobufRpcMessageHandler();
    virtual ~ProtobufRpcMessageHandler();

    virtual void channelConnected(ChannelHandlerContext& ctx, const ChannelStateEvent& e);
    virtual void channelDisconnected(ChannelHandlerContext& ctx, const ChannelStateEvent& e);
    virtual void messageReceived(ChannelHandlerContext& ctx, const MessageEvent& e);
    virtual void exceptionCaught(ChannelHandlerContext& ctx, const ExceptionEvent& e);

    virtual void writeRequested(ChannelHandlerContext& ctx, const MessageEvent& e);

    virtual ChannelHandlerPtr clone();
    virtual std::string toString() const { return "ProtobufRpcMessageHandler"; }

private:
    void doneCallback(google::protobuf::Message* response, boost::int64_t id);

private:
    Channel* channel;

    std::map<boost::int64_t, OutstandingCall> outstandings;
    boost::int64_t id;
};

}
}
}

#endif //#if !defined(CETTY_HANDLER_RPC_PROTOBUF_PROTOBUFRPCMESSAGEHANDLER_H)