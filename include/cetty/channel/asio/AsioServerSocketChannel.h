#if !defined(CETTY_CHANNEL_SOCKET_ASIO_ASIOSERVERSOCKETCHANNEL_H)
#define CETTY_CHANNEL_SOCKET_ASIO_ASIOSERVERSOCKETCHANNEL_H

/*
 * Copyright 2009 Red Hat, Inc.
 *
 * Red Hat licenses this file to you under the Apache License, version 2.0
 * (the "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at:
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */
/*
 * Copyright (c) 2010-2011 frankee zhou (frankee.zhou at gmail dot com)
 * Distributed under under the Apache License, version 2.0 (the "License").
 */

#include <boost/asio.hpp>

#include <cetty/channel/IpAddress.h>
#include <cetty/channel/SocketAddress.h>
#include <cetty/channel/EventLoopPoolPtr.h>

#include <cetty/channel/asio/AsioSocketChannel.h>
#include <cetty/channel/asio/AsioServicePoolPtr.h>
#include <cetty/channel/asio/AsioHandlerAllocator.h>
#include <cetty/channel/asio/AsioServerSocketChannelConfig.h>

namespace cetty {
namespace channel {
namespace asio {

class AsioSocketChannel;
typedef boost::shared_ptr<AsioSocketChannel> AsioSocketChannelPtr;

using namespace cetty::channel;

// only response to bind port, open and close.
class AsioServerSocketChannel : public cetty::channel::Channel {
public:
    AsioServerSocketChannel(const EventLoopPtr& eventLoop,
                            const EventLoopPoolPtr& childEventLoopPool);

    virtual ~AsioServerSocketChannel();

    virtual bool isOpen() const;
    virtual bool isActive() const;

protected:
    virtual bool setClosed();

    virtual void doBind(const SocketAddress& localAddress);
    virtual void doDisconnect();
    virtual void doClose();

private:
    void accept();
    void handleAccept(const boost::system::error_code& error,
                      const AsioSocketChannelPtr& channel);

    void handleChildClosed(const ChannelFuture& future);

private:
    typedef std::map<int, ChannelPtr> ChildChannels;

private:
    AsioServicePtr ioService_;

    boost::asio::ip::tcp::acceptor acceptor_;
    AsioHandlerAllocator<int> acceptAllocator_;
    AsioServerSocketChannelConfig socketConfig_;

    AsioServicePoolPtr childServicePool_;
    ChildChannels childChannels_;

    IpAddress::Family ipFamily;
    mutable SocketAddress localAddress;
};

}
}
}

#endif //#if !defined(CETTY_CHANNEL_SOCKET_ASIO_ASIOSERVERSOCKETCHANNEL_H)
