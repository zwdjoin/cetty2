#if !defined(CETTY_SHIRO_SESSION_SESSIONIDGENERATOR_H)
#define CETTY_SHIRO_SESSION_SESSIONIDGENERATOR_H
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
#include <unistd.h>
#include <sys/types.h>

#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

#include <cetty/shiro/session/Session.h>

namespace cetty {
namespace shiro {
namespace session {

using namespace boost::posix_time;

/**
 * Generates session IDs by using a {@link Random} instance to generate random IDs. The default {@code Random}
 * implementation is a {@link java.security.SecureRandom SecureRandom} with the {@code SHA1PRNG} algorithm.
 *
 * @since 1.0
 */
class SessionIdGenerator {
public:
    /**
     * Returns the String value of the session id: current time + id
     *
     * @param session the {@link Session} instance to which the ID will be applied.
     * @return the String value of the session id
     */
    static std::string generateId(SessionPtr session){
        std::stringstream sid;
        std::string host = (session ? session->getHost() : "");
        ptime epoch(boost::gregorian::date(1970,1,1));
        ptime now = second_clock::local_time();
        time_duration::sec_type x = (now - epoch).total_seconds();
        pid_t pid = getpid();

        sid << host << x << pid;
        return sid.str();
    }
};
}
}
}
#endif // #if !defined(CETTY_SHIRO_SESSION_SESSIONIDGENERATOR_H)
