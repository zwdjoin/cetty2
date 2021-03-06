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

#include <cetty/buffer/ChannelBuffer.h>
#include <cetty/handler/codec/http/HttpResponse.h>
#include <cetty/util/StringUtil.h>

namespace cetty {
namespace handler {
namespace codec {
namespace http {

using namespace cetty::buffer;
using namespace cetty::util;

HttpResponse::HttpResponse()
    : version_(HttpVersion::HTTP_1_1),
    status_(HttpResponseStatus::NOT_FOUND) {
}

HttpResponse::HttpResponse(const HttpVersion& version, const HttpResponseStatus& status)
    : version_(version),
    status_(status) {
}

void HttpResponse::clear() {
    headers_.clear();
    status_ = HttpResponseStatus::NOT_FOUND;
}

std::string HttpResponse::toString() const {
    std::string buf;
    buf.reserve(2048);

//     StringUtil::printf(&buf,
//                           "HttpResponse (TransferEncoding: %s)\r\n%s %s",
//                           transferEncoding().toString().c_str(),
//                           getProtocolVersion().getText().c_str(),
//                           status().toString().c_str());
// 
//     ConstHeaderIterator end = getLastHeader();
// 
//     for (ConstHeaderIterator itr = getFirstHeader(); itr != end; ++itr) {
//         StringUtil::printf(&buf, "\r\n%s: %s", itr->first.c_str(), itr->second.c_str());
//     }

    return buf;
}

}
}
}
}
