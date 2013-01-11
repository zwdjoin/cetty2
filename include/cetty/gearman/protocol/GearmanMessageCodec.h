#if !defined(CETTY_GEARMAN_PROTOCOL_GEARMANCODEC_H)
#define CETTY_GEARMAN_PROTOCOL_GEARMANCODEC_H

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

#include <cetty/handler/codec/MessageToMessageCodec.h>
#include <cetty/gearman/protocol/GearmanMessage.h>

namespace cetty {
namespace gearman {
namespace protocol {

using namespace cetty::channel;
using namespace cetty::handler::codec;

class GearmanMessageCodec : private boost::noncopyable {
public:
    typedef MessageToMessageCodec<GearmanMessageCodec,
            ChannelBufferPtr,
            GearmanMessagePtr,
            GearmanMessagePtr,
            ChannelBufferPtr> Codec;

    typedef Codec::Context Context;

    typedef Codec::Handler Handler;
    typedef Codec::HandlerPtr HandlerPtr;

public:
    GearmanMessageCodec();
    ~GearmanMessageCodec();

    void registerTo(Context& ctx) {
        codec_.registerTo(ctx);
    }

private:
    GearmanMessagePtr decode(ChannelHandlerContext& ctx,
                             const ChannelBufferPtr& msg);

    ChannelBufferPtr encode(ChannelHandlerContext& ctx,
                            const GearmanMessagePtr& msg);

    int caculateParametersLength(const GearmanMessagePtr& msg);

    void writeHeader(const ChannelBufferPtr& buffer, int type, int length);
    void writeHeaderAhead(const ChannelBufferPtr& buffer, int type, int length);
    void writeParameters(const ChannelBufferPtr& buffer,
                         const std::vector<std::string>& parameters,
                         bool withZeroPad);
    void writeParametersAhead(const ChannelBufferPtr& buffer,
                              const std::vector<std::string>& parameters,
                              bool withZeroPad);

private:
    Codec codec_;
};

}
}
}

#endif //#if !defined(CETTY_GEARMAN_PROTOCOL_GEARMANCODEC_H)

// Local Variables:
// mode: c++
// End: