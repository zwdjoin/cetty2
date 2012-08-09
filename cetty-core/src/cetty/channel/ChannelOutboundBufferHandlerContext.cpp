
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

#include <cetty/channel/ChannelOutboundBufferHandlerContext.h>
#include <boost/bind.hpp>

#include <cetty/channel/Channel.h>
#include <cetty/channel/ChannelPipeline.h>
#include <cetty/channel/ChannelOutboundBufferHandler.h>

namespace cetty {
namespace channel {

void ChannelOutboundBufferHandlerContext::setOutboundChannelBuffer(
    const ChannelBufferPtr& buffer) {
    if (eventloop->inLoopThread()) {
        outboundHandler->setOutboundChannelBuffer(buffer);
    }
    else {
        eventloop->post(boost::bind(
                            &ChannelOutboundBufferHandlerContext::setOutboundChannelBuffer,
                            this,
                            buffer));
    }
}

ChannelOutboundBufferHandlerContext::ChannelOutboundBufferHandlerContext(
    const std::string& name,
    ChannelPipeline& pipeline,
    const ChannelHandlerPtr& handler,
    ChannelHandlerContext* prev,
    ChannelHandlerContext* next)
    : ChannelHandlerContext(name, pipeline, handler, prev, next) {
    hasOutboundBufferHandler = true;
    outboundHandler = boost::dynamic_pointer_cast<ChannelOutboundBufferHandler>(handler);
    BOOST_ASSERT(outboundHandler);
}

ChannelOutboundBufferHandlerContext::ChannelOutboundBufferHandlerContext(
    const std::string& name,
    const EventLoopPtr& eventLoop,
    ChannelPipeline& pipeline,
    const ChannelHandlerPtr& handler,
    ChannelHandlerContext* prev,
    ChannelHandlerContext* next)
    : ChannelHandlerContext(name, eventLoop, pipeline, handler, prev, next) {
    hasOutboundBufferHandler = true;
    outboundHandler = boost::dynamic_pointer_cast<ChannelOutboundBufferHandler>(handler);
    BOOST_ASSERT(outboundHandler);
}

ChannelOutboundBufferHandlerContext::~ChannelOutboundBufferHandlerContext() {
}

}
}