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

#include <cetty/channel/ChannelHandlerContext.h>

namespace cetty {
namespace channel {

template<typename OutT>
class ChannelOutboundMessageHandlerContext : public ChannelHandlerContext {
public:
    typedef std::deque<OutT> MessageQueue;

public:
    ChannelOutboundMessageHandlerContext();
    virtual ~ChannelOutboundMessageHandlerContext() {}

    virtual bool hasMessageQueue() const { return true; }

    MessageQueue& getMessageQueue() { return queue; }
    void addMessage(const OutT& message) { queue.push_back(message); }

private:
    MessageQueue queue;
};

}
}

#endif //#if !defined(CETTY_CHANNEL_CHANNELOUTBOUNDMESSAGEHANDLERCONTEXT_H)

// Local Variables:
// mode: c++
// End:
