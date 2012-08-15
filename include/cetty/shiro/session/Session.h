#if !defined(CETTY_SHIRO_SESSION_SESSION_H)
#define CETTY_SHIRO_SESSION_SESSION_H
/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <vector>

#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/any.hpp>
#include <cetty/shiro/session/ReferenceCounter.h>
#include <cetty/shiro/util/EmptyObj.h>
//#include "SessionManager.h"

namespace cetty {
namespace shiro {
namespace session {

using namespace boost::posix_time;
using namespace cetty::util;
/**
 * A {@code Session} is a stateful data context associated with a single Subject (user, daemon process,
 * etc) who interacts with a software system over a period of time.
 * <p/>
 * A {@code Session} is intended to be managed by the business tier and accessible via other
 * tiers without being tied to any given client technology.  This is a <em>great</em> benefit to Java
 * systems, since until now, the only viable session mechanisms were the
 * {@code javax.servlet.http.HttpSession} or Stateful Session EJB's, which many times
 * unnecessarily coupled applications to web or ejb technologies.
 *
 * @since 0.1
 */
class Session : public ReferenceCounter<Session, int>{
public:
    enum SessionState {
        START,
        ACTIVE,
        STOPPED,
        EXPIRED
    };

public:
    Session() { init(); }
    Session(const std::string& host) : host(host) { init(); }

    const std::string& getId() const { return this->id; }
    void setId(const std::string& id) { this->id = id; }

    const ptime& getStartTimestamp() const { return startTimestamp; }
    void setStartTimestamp(const ptime& startTimestamp) { this->startTimestamp = startTimestamp; }

    /**
     * Returns the time the session was stopped, or <tt>null</tt> if the session is still active.
     * <p/>
     * A session may become stopped under a number of conditions:
     * <ul>
     * <li>If the user logs out of the system, their current session is terminated (released).</li>
     * <li>If the session expires</li>
     * <li>The application explicitly calls {@link #stop()}</li>
     * <li>If there is an internal system error and the session state can no longer accurately
     * reflect the user's behavior, such in the case of a system crash</li>
     * </ul>
     * <p/>
     * Once stopped, a session may no longer be used.  It is locked from all further activity.
     *
     * @return The time the session was stopped, or <tt>null</tt> if the session is still
     *         active.
     */
    const ptime& getStopTimestamp() const { return stopTimestamp; }
    void setStopTimestamp(const ptime& stopTimestamp) {
        this->stopTimestamp = stopTimestamp;
    }

    const ptime& getLastAccessTime() const { return lastAccessTime; }
    void setLastAccessTime(ptime& lastAccessTime) {
        this->lastAccessTime = lastAccessTime;
    }

    /**
     * Returns true if this session has expired, false otherwise.  If the session has
     * expired, no further user interaction with the system may be done under this session.
     *
     * @return true if this session has expired, false otherwise.
     */
    bool isExpired() { return expired; }
    void setExpired(bool expired) { this->expired = expired; }

    int getTimeout() const{ return timeout; }
    void setTimeout(int timeout) { this->timeout = timeout; }

    const std::string& getHost() const { return host; }
    void setHost(const std::string& host) { this->host = host; }

    const std::map<std::string, boost::any>& getAttributes() const {
        return attributes;
    }
    void setAttributes(const std::map<std::string, boost::any>& attributes) {
        this->attributes.insert(attributes.begin(), attributes.end());
    }

    void touch() {
        this->lastAccessTime =  ptime(second_clock::local_time());
    }

    void stop() {
        if (this->stopTimestamp.is_not_a_date_time()) {
            this->stopTimestamp =  ptime( second_clock::local_time() );
        }
    }

    void getAttributeKeys(std::vector<std::string>* keys){}

    const boost::any &getAttribute(const std::string& key) {
        std::map<std::string, boost::any>::iterator it = attributes.find(key);
        if (it == attributes.end()) { return cetty::shiro::util::emptyAny; }
        return it->second;
    }

    void setAttribute(const std::string& key, const boost::any& value){
        if(key.empty()) return;
        attributes.insert(std::pair<std::string, boost::any>(key, value));
    }

    void removeAttribute(const std::string& key){
        if(key.empty()) return;
        attributes.erase(key);
    }

    SessionState validate();

protected:
    static const int MILLIS_PER_SECOND;
    static const int MILLIS_PER_MINUTE;
    static const int MILLIS_PER_HOUR;

    bool isStopped() { return !getStopTimestamp().is_not_a_date_time(); }
    void expire() {
        stop();
        this->expired = true;
    }
    bool isValid() { return !isStopped() && !isExpired(); }

    /**
     * Determines if this session is expired.
     *
     * @return true if the specified session has expired, false otherwise.
     */
    bool isTimedOut();

private:
    void init();

    const std::map<std::string, boost::any>& getAttributesLazy(){ return getAttributes(); };

private:
    std::string id;
    ptime startTimestamp;
    ptime stopTimestamp;
    ptime lastAccessTime;
    int timeout;
    bool expired;
    std::string host;

    std::map<std::string, boost::any> attributes;
};

}
}
}
#endif // #if !defined(CETTY_SHIRO_SESSION_SESSION_H)
