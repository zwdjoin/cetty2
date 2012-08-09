#if !defined(CETTY_CHANNEL_CHANNELOUTBOUNDMESSAGEHANDLERCONTEXT_H)
#define CETTY_CHANNEL_CHANNELOUTBOUNDMESSAGEHANDLERCONTEXT_H

/*
 * Copyright (c) 2010-2012 frankee zhou (frankee.zhou at gmail dot com)
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
#include <cetty/channel/ChannelHandlerContext.h>
#include <cetty/channel/ChannelOutboundMessageHandlerFwd.h>

namespace cetty {
namespace channel {

    template<typename OutboundInT>
    class ChannelOutboundMessageHandler;

template<typename OutboundInT>
class ChannelOutboundMessageHandlerContext : public virtual ChannelHandlerContext {
public:
    typedef ChannelOutboundMessageHandler<OutboundInT> OutboundHandler;
    typedef boost::intrusive_ptr<OutboundHandler> OutboundHandlerPtr;

public:
    ChannelOutboundMessageHandlerContext(const std::string& name,
                                         ChannelPipeline& pipeline,
                                         const ChannelHandlerPtr& handler,
                                         ChannelHandlerContext* prev,
                                         ChannelHandlerContext* next)
        : ChannelHandlerContext(name, pipeline, handler, prev, next) {
        hasOutboundMessageHandler = true;

        outboundMsgHandler = boost::dynamic_pointer_cast<OutboundHandler>(handler);
        BOOST_ASSERT(outboundMsgHandler);
    }

    ChannelOutboundMessageHandlerContext(const std::string& name,
                                         const EventLoopPtr& eventLoop,
                                         ChannelPipeline& pipeline,
                                         const ChannelHandlerPtr& handler,
                                         ChannelHandlerContext* prev,
                                         ChannelHandlerContext* next)
        : ChannelHandlerContext(name, eventLoop, pipeline, handler, prev, next) {
        hasOutboundMessageHandler = true;

        outboundMsgHandler = boost::dynamic_pointer_cast<OutboundHandler>(handler);
        BOOST_ASSERT(outboundMsgHandler);
    }

    virtual ~ChannelOutboundMessageHandlerContext() {}

    void addOutboundMessage(const OutboundInT& message) {
        if (eventloop->inLoopThread()) {
            outboundMsgHandler->addOutboundMessage(message);
        }
        else {
            eventloop->post(boost::bind(
                                &ChannelOutboundMessageHandlerContext<OutboundInT>::addOutboundMessage,
                                this,
                                message));
        }
    }

private:
    OutboundHandlerPtr outboundMsgHandler;
};

}
}

#endif //#if !defined(CETTY_CHANNEL_CHANNELOUTBOUNDMESSAGEHANDLERCONTEXT_H)

// Local Variables:
// mode: c++
// End: