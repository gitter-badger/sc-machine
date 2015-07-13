/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

extern "C"
{
#include "../sc_memory.h"
}

#include <vector>
#include <list>

namespace sc
{

class Addr
{
    friend class MemoryContext;

    template <typename ParamType1, typename ParamType2, typename ParamType3> friend class Iterator3;
    template <typename ParamType1, typename ParamType2, typename ParamType3, typename ParamType4, typename ParamType5> friend class Iterator5;

public:
    explicit Addr();
    explicit Addr(sc_addr const & addr);

    bool isValid() const;
    void reset();

    bool operator == (Addr const & other) const;
    bool operator != (Addr const & other) const;

protected:
    sc_addr mRealAddr;
};

typedef std::vector<Addr> tAddrVector;
typedef std::list<Addr> tAddrList;
}
