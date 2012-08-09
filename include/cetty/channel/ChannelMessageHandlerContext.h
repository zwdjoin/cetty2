#if !defined(CETTY_CHANNEL_CHANNELMESSAGEHANDLERCONTEXT_H)
#define CETTY_CHANNEL_CHANNELMESSAGEHANDLERCONTEXT_H

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

#include <cetty/channel/ChannelInboundMessageHandlerContext.h>
#include <cetty/channel/ChannelOutboundMessageHandlerContext.h>

namespace cetty {
namespace channel {

template<typename InboundInT, typename OutboundInT>
class ChannelMessageHandlerContext
    : public ChannelInboundMessageHandlerContext<InboundInT>,
      public ChannelOutboundMessageHandlerContext<OutboundInT> {
public:
    ChannelMessageHandlerContext(const std::string& name,
                                 ChannelPipeline& pipeline,
                                 const ChannelHandlerPtr& handler,
                                 ChannelHandlerContext* prev,
                                 ChannelHandlerContext* next)
        : ChannelInboundMessageHandlerContext(name, pipeline, handler, prev, next),
          ChannelOutboundMessageHandlerContext(name, pipeline, handler, prev, next),
          ChannelHandlerContext(name, pipeline, handler, prev, next) {
        hasInboundMessageHandler = true;
        hasOutboundMessageHandler = true;
    }

    ChannelMessageHandlerContext(const std::string& name,
                                 const EventLoopPtr& eventLoop,
                                 ChannelPipeline& pipeline,
                                 const ChannelHandlerPtr& handler,
                                 ChannelHandlerContext* prev,
                                 ChannelHandlerContext* next)
        : ChannelInboundMessageHandlerContext(name, eventLoop, pipeline, handler, prev, next),
          ChannelOutboundMessageHandlerContext(name, eventLoop, pipeline, handler, prev, next),
          ChannelHandlerContext(name, eventLoop, pipeline, handler, prev, next) {
        hasInboundMessageHandler = true;
        hasOutboundMessageHandler = true;
    }

    virtual ~ChannelMessageHandlerContext() {}

};

}
}

#endif //#if !defined(CETTY_CHANNEL_CHANNELMESSAGEHANDLERCONTEXT_H)

// Local Variables:
// mode: c++
// End: