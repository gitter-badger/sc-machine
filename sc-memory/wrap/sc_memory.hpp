/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

extern "C"
{
#include "sc_memory.h"
#include "sc_helper.h"
}

#include "sc_types.hpp"
#include "sc_addr.hpp"
#include "sc_iterator.hpp"

#include <list>
#include <string>

namespace sc
{

class MemoryContext;
class Stream;

class Memory
{
    friend class MemoryContext;

public:
    //! Returns true, on memory initialized; otherwise returns false
    _SC_EXTERN static bool initialize(sc_memory_params const & params);
    _SC_EXTERN static void shutdown(bool saveState = true);

protected:

    static void registerContext(MemoryContext const * ctx);
    static void unregisterContext(MemoryContext const * ctx);
private:
    static bool hasMemoryContext(MemoryContext const * ctx);

private:
    static sc_memory_context * msGlobalContext;

    typedef std::list<MemoryContext const *> tMemoryContextList;
    static tMemoryContextList msContexts;
};

//! Class used to work with memory. It provides functions to create/erase elements
class MemoryContext
{
public:
    _SC_EXTERN explicit MemoryContext(sc_uint8 accessLevels = 0, std::string const & name = "");
    _SC_EXTERN ~MemoryContext();

    sc_memory_context const * getRealContext() const { return mContext; }

    //! Call this function, when you request to destroy real memory context, before destructor calls for this object
    _SC_EXTERN void destroy();

    std::string const & getName() const { return mName; }

    _SC_EXTERN bool isValid() const;

    //! Check if element exists with specified addr
    _SC_EXTERN bool isElement(Addr const & addr) const;
    //! Erase element from sc-memory and returns true on success; otherwise returns false.
    _SC_EXTERN bool eraseElement(Addr const & addr);

    _SC_EXTERN Addr createNode(sc_type type);
    _SC_EXTERN Addr createLink();
    _SC_EXTERN Addr createArc(sc_type type, Addr const & addrBeg, Addr const & addrEnd);

    //! Returns type of sc-element. If there are any error, then returns 0
    _SC_EXTERN sc_type getElementType(Addr const & addr) const;

    /*! Change subtype of sc-element (subtype & sc_type_element_mask == 0).
     * Return true, if there are no any errors; otherwise return false.
     */
    _SC_EXTERN bool setElementSubtype(Addr const & addr, sc_type subtype);

    _SC_EXTERN Addr getArcBegin(Addr const & arcAddr) const;
    _SC_EXTERN Addr getArcEnd(Addr const & arcAddr) const;

    _SC_EXTERN bool setLinkContent(Addr const & addr, Stream const & stream);
    _SC_EXTERN bool getLinkContent(Addr const & addr, Stream & stream);

    //! Returns true, if any links found
    _SC_EXTERN bool findLinksByContent(Stream const & stream, tAddrList & found);

	//! Finds element by system identifier
	_SC_EXTERN Addr findElementBySysIdtf(std::string const & sysIdtf);

    //! Saves memory state
    _SC_EXTERN bool save();

    template <typename ParamType1, typename ParamType2, typename ParamType3, typename ParamType4, typename ParamType5>
    TIterator5<ParamType1, ParamType2, ParamType3, ParamType4, ParamType5> * iterator5(ParamType1 const & param1,
                                                                                       ParamType2 const & param2,
                                                                                       ParamType3 const & param3,
                                                                                       ParamType4 const & param4,
                                                                                       ParamType5 const & param5)
    {
        return new TIterator5<ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>(*this, param1, param2, param3, param4, param5);
    }

    template <typename ParamType1, typename ParamType2, typename ParamType3>
    TIterator3<ParamType1, ParamType2, ParamType3> * iterator3(ParamType1 const & param1,
                                                               ParamType2 const & param2,
                                                               ParamType3 const & param3)
    {
        return new TIterator3<ParamType1, ParamType2, ParamType3>(*this, param1, param2, param3);
    }

private:
    // Disable object copying
    MemoryContext(MemoryContext const & other) {}
	MemoryContext & operator = (MemoryContext const & other) { return *this; }

private:
    sc_memory_context * mContext;
    std::string mName;
};


}
