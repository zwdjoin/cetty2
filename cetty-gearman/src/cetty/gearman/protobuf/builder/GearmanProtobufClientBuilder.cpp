
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

#include <cetty/gearman/protobuf/builder/GearmanProtobufClientBuilder.h>

#include <cetty/channel/Channels.h>
#include <cetty/channel/ChannelPipeline.h>
#include <cetty/handler/codec/frame/LengthFieldBasedFrameDecoder.h>
#include <cetty/gearman/GearmanDecoder.h>
#include <cetty/gearman/GearmanEncoder.h>
#include <cetty/gearman/GearmanClientHandler.h>
#include <cetty/gearman/protobuf/GearmanProtobufMessageFilter.h>

namespace cetty {
namespace gearman {
namespace protobuf {
namespace builder {

using namespace cetty::channel;
using namespace cetty::handler::codec::frame;
using namespace cetty::gearman;
using namespace cetty::gearman::protobuf;

void GearmanProtobufClientBuilder::init() {
    pipeline = Channels::pipeline();

    pipeline->addLast("frameDecoder", new LengthFieldBasedFrameDecoder(16 * 1024 * 1024, 0, 4, 0, 4));
    pipeline->addLast("gearmanDecoder", new GearmanDecoder());
    pipeline->addLast("gearmanEncoder", new GearmanEncoder());
    pipeline->addLast("gearmanClient", new GearmanClientHandler());
    pipeline->addLast("gearmanFilter", new GearmanProtobufClientFilter());

    ClientBuilderType::setPipeline(pipeline);
}

}
}
}
}
