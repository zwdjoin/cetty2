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

#include <boost/assert.hpp>
#include <cetty/protobuf/service/handler/ProtobufServiceMessage.h>
#include <cetty/protobuf/service/proto/service.pb.h>

namespace cetty {
namespace protobuf {
namespace service {
namespace handler {

using namespace cetty::protobuf::service;
using namespace cetty::protobuf::service::proto;

ProtobufServiceMessage::ProtobufServiceMessage()
    : message(new ServiceMessage) {

}

ProtobufServiceMessage::ProtobufServiceMessage(int type)
    : message(new ServiceMessage) {
    message->set_type(MessageType(type));
}

ProtobufServiceMessage::ProtobufServiceMessage(int type, boost::int64_t id)
    : message(new ServiceMessage) {
    message->set_type(MessageType(type));
    message->set_id(id);
}

ProtobufServiceMessage::ProtobufServiceMessage(int type,
        boost::int64_t id,
        const MessagePtr& payload)
    : message(new ServiceMessage), payload(payload) {
    message->set_type(MessageType(type));
    message->set_id(id);
}

ProtobufServiceMessage::ProtobufServiceMessage(int type,
        const std::string& service,
        const std::string& method)
    : message(new ServiceMessage) {
    message->set_type(MessageType(type));
    message->set_service(service);
    message->set_method(method);
}

ProtobufServiceMessage::ProtobufServiceMessage(int type,
        const std::string& service,
        const std::string& method,
        const MessagePtr& payload)
    : message(new ServiceMessage), payload(payload) {
    message->set_type(MessageType(type));
    message->set_service(service);
    message->set_method(method);
}

ProtobufServiceMessage::~ProtobufServiceMessage() {
    if (message) {
        delete message;
    }
}

bool ProtobufServiceMessage::isRequest() const {
    BOOST_ASSERT(message && "message should not be NULL.");
    return message->type() == REQUEST;
}

bool ProtobufServiceMessage::isResponse() const {
    BOOST_ASSERT(message && "message should not be NULL.");
    return message->type() == RESPONSE;
}

bool ProtobufServiceMessage::isError() const {
    BOOST_ASSERT(message && "message should not be NULL.");
    return message->type() == ERROR;
}

void ProtobufServiceMessage::setType(int type) {
    BOOST_ASSERT(message && "message should not be NULL.");
    message->set_type(MessageType(type));
}

boost::int64_t ProtobufServiceMessage::getId() const {
    BOOST_ASSERT(message && "message should not be NULL.");
    return message->id();
}

void ProtobufServiceMessage::setId(boost::int64_t id) {
    BOOST_ASSERT(message && "message should not be NULL.");
    return message->set_id(id);
}

const std::string& ProtobufServiceMessage::getService() const {
    BOOST_ASSERT(message && "message should not be NULL.");
    return message->service();
}

std::string* ProtobufServiceMessage::mutableService() {
    BOOST_ASSERT(message && "message should not be NULL.");
    return message->mutable_service();
}

const std::string& ProtobufServiceMessage::getMethod() const {
    BOOST_ASSERT(message && "message should not be NULL.");
    return message->method();
}

std::string* ProtobufServiceMessage::mutableMethod() {
    BOOST_ASSERT(message && "message should not be NULL.");
    return message->mutable_method();
}

const ServiceMessage& ProtobufServiceMessage::getServiceMessage() const {
    BOOST_ASSERT(message && "message should not be NULL.");
    return *message;
}

static MessagePtr EMPTY_MESSAGE;

const MessagePtr& ProtobufServiceMessage::getResponse() const {
    if (isResponse()) {
        return payload;
    }

    return EMPTY_MESSAGE;
}

const MessagePtr& ProtobufServiceMessage::getRequest() const {
    if (isRequest()) {
        return payload;
    }

    return EMPTY_MESSAGE;
}

bool ProtobufServiceMessage::checkType(int type) {
    return MessageType_IsValid(type);
}

int ProtobufServiceMessage::getMessageSize() const {
    return message->ByteSize() + (payload ? payload->ByteSize() + 8 : 0);
}


}
}
}
}
