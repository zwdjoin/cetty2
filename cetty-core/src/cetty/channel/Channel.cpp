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
#include <cetty/channel/Channel.h>

#include <boost/bind.hpp>

#include <cetty/channel/InetAddress.h>
#include <cetty/channel/ChannelFuture.h>
#include <cetty/channel/VoidChannelFuture.h>
#include <cetty/channel/FailedChannelFuture.h>
#include <cetty/channel/DefaultChannelFuture.h>
#include <cetty/channel/ChannelFutureListener.h>
#include <cetty/channel/SucceededChannelFuture.h>

#include <cetty/util/Adler32.h>
#include <cetty/util/Exception.h>
#include <cetty/util/StringUtil.h>

namespace cetty {
namespace channel {

Channel::Channel(const ChannelPtr& parent,
                 const EventLoopPtr& eventLoop)
    : id_(),
      state_(CHANNEL_INIT),
      parent_(parent),
      eventLoop_(eventLoop),
      pipeline_() {
    allocateId();
}

Channel::Channel(int id,
                 const ChannelPtr& parent,
                 const EventLoopPtr& eventLoop)
    : id_(id),
      state_(CHANNEL_INIT),
      parent_(parent),
      eventLoop_(eventLoop),
      pipeline_() {
    allocateId();
}

Channel::~Channel() {
    LOG_DEBUG << "Channel " << toString() << " dctr";

    if (pipeline_) {
        delete pipeline_;
    }
}

void Channel::open() {
    if (isOpen()) {
        LOG_WARN << "channel " << toString() << " is already open";
        return;
    }

    ChannelPtr self = shared_from_this();

    if (!succeededFuture_) { // Once have been opened
        pipeline_ = new ChannelPipeline(self);
        succeededFuture_ = new SucceededChannelFuture(self);
        closeFuture_ = new DefaultChannelFuture(self, false);

        if (initializer_) {
            initializer_(self);
        }
    }
    else {
        closeFuture_.reset(new DefaultChannelFuture(self, false));
    }

    doPreOpen();

    strVal_.clear();
    state_ = CHANNEL_OPENED;
    pipeline_->fireChannelOpen();
}

ChannelFuturePtr Channel::close() {
    if (pipeline_) {
        return pipeline_->close();
    }
    else {
        LOG_INFO << "close the channel, but the pipeline has detached.";
        return closeFuture_;
    }
}

const ChannelFuturePtr& Channel::close(const ChannelFuturePtr& future) {
    return pipeline_->close(future);
}

ChannelFuturePtr Channel::newFuture() {
    return new DefaultChannelFuture(shared_from_this(), false);
}

ChannelFuturePtr Channel::newFailedFuture(const Exception& e) {
    return new FailedChannelFuture(shared_from_this(), e);
}

ChannelFuturePtr Channel::newVoidFuture() {
    return new VoidChannelFuture(shared_from_this());
}

void Channel::allocateId() {
    if (!id_) {
        id_ = Adler32::adler32(reinterpret_cast<char const*>(this),
                               sizeof(this));
    }
}

int Channel::compareTo(const ChannelPtr& c) const {
    if (c) {
        return (id() - c->id());
    }

    return 1;
}

std::string Channel::toString() const {
    if (!strVal_.empty()) {
        return strVal_;
    }

    const InetAddress& local = localAddress();
    const InetAddress& remote = remoteAddress();

    if (remote) {
        if (!parent()) { // server channel or client channel
            StringUtil::printf(&strVal_, "[id: 0x%08x, %s => %s]", id(),
                               local.toString().c_str(),
                               remote.toString().c_str());
        }
        else { // connection channel
            StringUtil::printf(&strVal_, "[id: 0x%08x, %s => %s]", id(),
                               remote.toString().c_str(),
                               local.toString().c_str());
        }
    }
    else if (local) {
        StringUtil::printf(&strVal_, "[id: 0x%08x, %s]", id(),
                           local.toString().c_str());
    }
    else {
        StringUtil::printf(&strVal_, "[id: 0x%08x]", id());
    }

    return strVal_;
}

void Channel::doBind(ChannelHandlerContext& ctx,
                     const InetAddress& localAddress,
                     const ChannelFuturePtr& future) {
    if (!isOpen()) {
        std::string msg;
        StringUtil::printf(&msg,
                           "should not binding to %s when this channel has not open yet.",
                           localAddress.toString().c_str());

        LOG_ERROR << toString() << msg;
        future->setFailure(ChannelException(msg));
        return;
    }

    try {
        bool wasActive = isActive();

        if (doBind(localAddress)) {
            setActived();
            future->setSuccess();
        }
        else {
            processFailure(StringUtil::printf(
                               "unable bind to %s",
                               localAddress.toString().c_str()),
                           future);
        }
    }
    catch (const std::exception& e) {
        processFailure(StringUtil::printf(
                           "exception happened: %s,  when binding to %s",
                           e.what(),
                           localAddress.toString().c_str()),
                       future);
    }
}

void Channel::doDisconnect(ChannelHandlerContext& ctx,
                           const ChannelFuturePtr& future) {
    if (isActive()) {
        try {
            if (doDisconnect()) {
                pipeline_->fireChannelInactive();
            }
            else {
                processFailure(StringUtil::printf(
                                   "disconnect failed from %s",
                                   remoteAddress().toString().c_str()),
                               future);
            }
        }
        catch (const std::exception& e) {
            processFailure(StringUtil::printf(
                               "exception happened: %s,  when disconnect from %s",
                               e.what(),
                               remoteAddress().toString().c_str()),
                           future);
        }
    }
    else {
        future->setSuccess();
    }

    state_ = CHANNEL_INACTIVED;
}

void Channel::doClose(ChannelHandlerContext& ctx, const ChannelFuturePtr& future) {
    bool wasActive = isActive();

    if (isOpen()) {
        try {
            doPreClose();

            if (doClose()) {
                future->setSuccess();

                if (wasActive) {
                    LOG_INFO << toString() << " closed successfully.";
                    pipeline_->fireChannelInactive();
                }
            }
            else {
                processFailure("close failed", future);
            }
        }
        catch (const std::exception& e) {
            processFailure(StringUtil::printf(
                               "exception happened: %s,  when closing.",
                               e.what()),
                           future);
        }
    }
    else {
        // Closed already.
        future->setSuccess();
    }

    state_ = CHANNEL_INACTIVED;
}

void Channel::closeIfClosed() {
    if (isOpen()) {
        return;
    }

    close(newVoidFuture());
}

void Channel::setActived() {
    state_ = CHANNEL_ACTIVED;
    pipeline_->fireChannelActive();
}

void Channel::processFailure(const std::string& message,
                             const ChannelFuturePtr& future) {
    LOG_ERROR << toString() << message;

    ChannelException exception(message);
    future->setFailure(exception);
    pipeline_->fireExceptionCaught(exception);
    closeIfClosed();
}

}
}
