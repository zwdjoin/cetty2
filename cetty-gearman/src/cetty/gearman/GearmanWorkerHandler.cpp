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
#include <string>
#include <iostream>

#include <cetty/gearman/GearmanMessage.h>
#include <cetty/gearman/GearmanWorkerHandler.h>
#include <cetty/channel/ChannelHandlerContext.h>
#include <cetty/channel/Channels.h>
#include <cetty/channel/MessageEvent.h>
#include <cetty/channel/ChannelStateEvent.h>
#include <cetty/channel/ChannelMessage.h>


namespace cetty {
namespace gearman {

using namespace cetty::channel;
using namespace cetty::buffer;

GearmanWorkerHandler::GearmanWorkerHandler():channel(0),grabCount(0) {
}

GearmanWorkerHandler::~GearmanWorkerHandler() {
}

void GearmanWorkerHandler::start() {
    grabJob();
}

void GearmanWorkerHandler::channelConnected(ChannelHandlerContext& ctx, const ChannelStateEvent& e)
{
    channel = e.getChannel();
}

void GearmanWorkerHandler::registerWorker(const std::string& functionName,const GrabJobCallback& worker)
{
    GearmanMessagePtr msg(GearmanMessage::createCandoMessage(functionName));
    channel->write(ChannelMessage(msg));
    //add the worker to the funcMap
    funcMap.insert(std::pair<const std::string,GrabJobCallback&>(functionName,worker));
}


void GearmanWorkerHandler::handleJob(GearmanMessagePtr gearmanMessage)
{
    FuncMapIter  iter;
    if ((iter = funcMap.find(gearmanMessage->getParameters()[1])) != funcMap.end())
    {
        //to call the function rely on the functionName
        iter->second(gearmanMessage);
    }
}

void GearmanWorkerHandler::grabJob() {
    if(grabCount>2)
    {
        preSleep();
    }
    GearmanMessagePtr msg(GearmanMessage::createGrabJobMessage());
    channel->write(ChannelMessage(msg));
}

void GearmanWorkerHandler::grabJobUnique() {
    if(grabCount>2)
    {
        preSleep();
    }
    GearmanMessagePtr msg(GearmanMessage::createGrabJobUniqMessage());
    channel->write(ChannelMessage(msg));
}

void GearmanWorkerHandler::preSleep() {
    GearmanMessagePtr msg(GearmanMessage::createPreSleepMessage());
    channel->write(ChannelMessage(msg));
}

void GearmanWorkerHandler::messageReceived(ChannelHandlerContext& ctx, const MessageEvent& e) {
    GearmanMessagePtr message = e.getMessage().smartPointer<GearmanMessage>();
    std::string  data;
    std::vector<std::string> params;

    if (!message) {
        ctx.sendUpstream(e);
    }

    switch(message->getType()) {
        //worker rsp
    case GearmanMessage::NOOP:
        std::cout<<"the NOOP is recivered,wake up to do job"<< std::endl;
        grabJob();
        break;

    case GearmanMessage::NO_JOB:
        std::cout<<"the GRAB_JOB  RSP is NO_JOB "<< std::endl;
        grabCount++;
        grabJob();
        break;

    case GearmanMessage::JOB_ASSIGN:
        std::cout<<"the GRAB_JOB  RSP  is JOB_ASSIGN,do the job now "<< std::endl;
        params = message->getParameters();
        std::cout<<"the job-handler is "<<params[0]<<std::endl;
        std::cout<<"the function name is "<<params[1]<<std::endl;
        data = ChannelBuffers::hexDump(message->getData());
        std::cout<<"the arg data is "<<data<<std::endl;
        std::string unique;

        handlerJob(params[1],)
        //grabCount reset
        grabCount = 0;
        //如果需要分割job
        /*if(message->Split())
        {
            GearmanWorker::splitJob();
        }
        GearmanWorker::handleJob(params[1],ChannelBuffers::hexDump(message->getData()),unique);*/
        break;

    case GearmanMessage::JOB_ASSIGN_UNIQ:
        std::cout<<"the GRAB_JOB_UNIQ  RSP  is JOB_ASSIGN_UNIQ which have uniqId assined from client "<< std::endl;
        params = message->getParameters();
        std::cout<<"the job-handler is "<<params[0]<<std::endl;
        std::cout<<"the function name is "<<params[1]<<std::endl;
        std::cout<<"the unique ID is "<<params[2]<<std::endl;
        data = ChannelBuffers::hexDump(message->getData());
        std::cout<<"the arg data is "<<data<<std::endl;

        //grabCount reset
        grabCount = 0;
        //call message handler of uniq
        //GearmanWorker::handleJob(params[1],ChannelBuffers::hexDump(message->getData()),params[2]);
        break;

    case GearmanMessage::ECHO_RES:
        data = ChannelBuffers::hexDump(message->getData());
        std::cout<<"the ECHO_RES is ok "<< std::endl;
        std::cout<<"the data is  "<<data<<std::endl;
        break;

    default:
        ctx.sendUpstream(e);
    }
}

void GearmanWorkerHandler::writeRequested(ChannelHandlerContext& ctx, const MessageEvent& e) {
    GearmanMessagePtr msg = e.getMessage().smartPointer<GearmanMessage>();

    if (msg->getType() == GearmanMessage::PRE_SLEEP)
        if (msg) {
            msgs.push_back(msg);
            Channels::write(ctx, e.getFuture(), ChannelMessage(msg));
        }
        else {
            ctx.sendDownstream(e);
        }
}

ChannelHandlerPtr GearmanWorkerHandler::clone() {
    return ChannelHandlerPtr(new GearmanWorkerHandler);
}

std::string GearmanWorkerHandler::toString() const {
    return "GearmanWorkerHandler";
}

}
}