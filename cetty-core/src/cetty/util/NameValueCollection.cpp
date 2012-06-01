//
// NameValueCollection.cpp
//
// $Id: //poco/1.4/Net/src/NameValueCollection.cpp#1 $
//
// Library: Net
// Package: Messages
// Module:  NameValueCollection
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//

#include <cetty/util/NameValueCollection.h>
#include <algorithm>
#include <boost/algorithm/string.hpp>

namespace cetty {
namespace util {

static const std::string EMPTY_STR;

NameValueCollection::NameValueCollection() {
}


NameValueCollection::NameValueCollection(const NameValueCollection& nvc):
    _map(nvc._map) {
}


NameValueCollection::~NameValueCollection() {
}


NameValueCollection& NameValueCollection::operator = (const NameValueCollection& nvc) {
    if (&nvc != this) {
        _map = nvc._map;
    }

    return *this;
}


void NameValueCollection::swap(NameValueCollection& nvc) {
    std::swap(_map, nvc._map);
}


const std::string& NameValueCollection::operator [](const std::string& name) const {
    ConstIterator it = _map.find(name);

    if (it != _map.end()) {
        return it->second;
    }
    else {
        return EMPTY_STR;
    }
}

void NameValueCollection::set(const std::string& name, const std::string& value) {
    Iterator it = _map.find(name);

    if (it != _map.end()) {
        it->second = value;
    }
    else {
        _map.insert(HeaderMap::value_type(name, value));
    }
}

void NameValueCollection::add(const std::string& name, const std::string& value) {
    _map.insert(HeaderMap::value_type(name, value));
}

const std::string& NameValueCollection::get(const std::string& name) const {
    ConstIterator it = _map.find(name);

    if (it != _map.end()) {
        return it->second;
    }
    else {
        return EMPTY_STR;
    }
}

const std::string& NameValueCollection::get(const std::string& name, const std::string& defaultValue) const {
    ConstIterator it = _map.find(name);

    if (it != _map.end()) {
        return it->second;
    }
    else {
        return defaultValue;
    }
}

const int NameValueCollection::get(const std::string& name, std::vector<std::string>* values) const {
    int count = 0;
    if (values) {
        ConstIterator itr = _map.find(name);
        for (; itr != _map.end(); ++itr) {
            values->push_back(itr->second);
            ++count;
        }
    }
    return count;
}

const int NameValueCollection::getNames(std::vector<std::string>* names) const {
    int count = 0;
    if (names) {
        for (ConstIterator itr = _map.begin(); itr != _map.end(); ++itr) {
            bool duplicated = false;
            for (std::vector<std::string>::const_iterator i = names->begin(); i != names->end(); ++i) {
                if (boost::iequals(*i, itr->second)) {
                    duplicated = true;
                    break;
                }
            }
            
            if (!duplicated) {
                names->push_back(itr->second);
                ++count;
            }
        }
    }

    return count;
}

bool NameValueCollection::has(const std::string& name) const {
    return _map.find(name) != _map.end();
}

NameValueCollection::ConstIterator NameValueCollection::find(const std::string& name) const {
    return _map.find(name);
}


NameValueCollection::ConstIterator NameValueCollection::begin() const {
    return _map.begin();
}


NameValueCollection::ConstIterator NameValueCollection::end() const {
    return _map.end();
}


bool NameValueCollection::empty() const {
    return _map.empty();
}


int NameValueCollection::size() const {
    return (int) _map.size();
}


void NameValueCollection::erase(const std::string& name) {
    _map.erase(name);
}

void NameValueCollection::erase(const std::string& name, const std::string& value) {
    for (Iterator itr = _map.find(name); itr != _map.end(); ++itr) {
        if (itr->second == value) {
            _map.erase(itr);
        }
    }
}

void NameValueCollection::clear() {
    _map.clear();
}

void NameValueCollection::setValidateNameFunctor(const ValidateNameFunctor& functor) {
    this->validateNameFunctor = functor;
}

}
}