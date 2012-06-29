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

#include <cetty/redis/RedisReplyMessage.h>

namespace cetty {
namespace redis {

const RedisReplyMessageType RedisReplyMessageType::STRING  = 0;
const RedisReplyMessageType RedisReplyMessageType::ARRAY   = 1;
const RedisReplyMessageType RedisReplyMessageType::INTEGER = 2;
const RedisReplyMessageType RedisReplyMessageType::NIL     = 3;
const RedisReplyMessageType RedisReplyMessageType::STATUS  = 4;
const RedisReplyMessageType RedisReplyMessageType::ERROR   = 5;

}
}