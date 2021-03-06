#if !defined(CETTY_HANDLER_CODEC_FIXEDLENGTHFRAMEDECODER_H)
#define CETTY_HANDLER_CODEC_FIXEDLENGTHFRAMEDECODER_H

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

#include <cetty/buffer/Unpooled.h>
#include <cetty/channel/ChannelHandler.h>
#include <cetty/handler/codec/BufferToMessageDecoder.h>
#include <cetty/util/StringUtil.h>
#include <cetty/util/Exception.h>

namespace cetty {
namespace handler {
namespace codec {

using namespace cetty::channel;
using namespace cetty::buffer;
using namespace cetty::util;

/**
 * A decoder that splits the received {@link ChannelBuffer}s by the fixed number
 * of bytes. For example, if you received the following four fragmented packets:
 * <pre>
 * +---+----+------+----+
 * | A | BC | DEFG | HI |
 * +---+----+------+----+
 * </pre>
 * A {@link FixedLengthFrameDecoder}<tt>(3)</tt> will decode them into the
 * following three packets with the fixed length:
 * <pre>
 * +-----+-----+-----+
 * | ABC | DEF | GHI |
 * +-----+-----+-----+
 * </pre>
 *
 *
 * @author <a href="http://gleamynode.net/">Trustin Lee</a>
 *
 * @author <a href="mailto:frankee.zhou@gmail.com">Frankee Zhou</a>
 */

class FixedLengthFrameDecoder : public  {
public:
    /**
     * Creates a new instance.
     *
     * @param frameLength  the length of the frame
     */
    FixedLengthFrameDecoder(int frameLength)
        : frameLength_(frameLength) {
        if (frameLength <= 0) {
            throw InvalidArgumentException(
                StringUtil::printf(
                    "frameLength must be a positive integer: %d",
                    frameLength));
        }

        decoder_.setDecoder(boost::bind(&FixedLengthFrameDecoder::decode,
                                        this,
                                        _1,
                                        _2));
    }

protected:
    ChannelBufferPtr decode(ChannelHandlerContext& ctx,
                            const ChannelBufferPtr& in) {
        if (!in && in->readableBytes() < frameLength_) {
            return Unpooled::EMPTY_BUFFER;
        }
        else {
            return in->readSlice(frameLength_);
        }
    }

private:
    int frameLength_;
    BufferToMessageDecoder<FixedLengthFrameDecoder, ChannelBufferPtr> decoder_;
};

}
}
}

#endif //#if !defined(CETTY_HANDLER_CODEC_FIXEDLENGTHFRAMEDECODER_H)

// Local Variables:
// mode: c++
// End:
