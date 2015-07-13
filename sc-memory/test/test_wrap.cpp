/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "wrap/sc_memory_headers.hpp"
#include <glib.h>

void init_memory()
{
    sc_memory_params params;
    sc_memory_params_clear(&params);

    params.clear = SC_TRUE;
    params.repo_path = "repo";
    params.config_file = "sc-memory.ini";
    params.ext_path = 0;

    sc::Memory::initialize(params);
}

void shutdown_memory(bool save)
{
    sc::Memory::shutdown(save);
}


void test_common_elements()
{
    init_memory();

    {
        sc::MemoryContext ctx;
        sc::Addr addr = ctx.createNode(sc_type_const);
        g_assert(addr.isValid());

        sc::Addr link = ctx.createLink();
        g_assert(link.isValid());

        sc::Addr arc = ctx.createArc(sc_type_arc_pos_const_perm, addr, link);
        g_assert(arc.isValid());

        g_assert(ctx.isElement(addr));
        g_assert(ctx.isElement(link));
        g_assert(ctx.isElement(arc));

        g_assert(ctx.getArcBegin(arc) == addr);
        g_assert(ctx.getArcEnd(arc) == link);

        g_assert(ctx.getElementType(addr) == sc_type_node | sc_type_const);
        g_assert(ctx.getElementType(link) == sc_type_link);
        g_assert(ctx.getElementType(arc) == sc_type_arc_pos_const_perm);

        g_assert(ctx.setElementSubtype(addr, sc_type_var));
        g_assert(ctx.getElementType(addr) == sc_type_node | sc_type_var);

        g_assert(ctx.eraseElement(addr));
        g_assert(!ctx.isElement(addr));
        g_assert(!ctx.isElement(arc));
        g_assert(ctx.isElement(link));
    }

    shutdown_memory(false);
}

void test_common_iterators()
{
    init_memory();

    {
        sc::MemoryContext ctx;
        sc::Addr addr1 = ctx.createNode(sc_type_const);
        sc::Addr addr2 = ctx.createNode(sc_type_var);
        sc::Addr arc1 = ctx.createArc(sc_type_arc_pos_const_perm, addr1, addr2);

        g_assert(addr1.isValid());
        g_assert(addr2.isValid());
        g_assert(arc1.isValid());

        g_assert(ctx.isElement(addr1));
        g_assert(ctx.isElement(addr2));
        g_assert(ctx.isElement(arc1));

        {
            sc::Iterator3_f_a_f iter(ctx, addr1, sc_type_arc_pos_const_perm, addr2);
            g_assert(iter.next());
            g_assert(iter.value(0) == addr1);
            g_assert(iter.value(1) == arc1);
            g_assert(iter.value(2) == addr2);
        }

        {
            sc::Iterator3_f_a_a iter(ctx, addr1, sc_type_arc_pos_const_perm, sc_type_node);
            g_assert(iter.next());
            g_assert(iter.value(0) == addr1);
            g_assert(iter.value(1) == arc1);
            g_assert(iter.value(2) == addr2);
        }

        {
            sc::Iterator3_a_a_f iter(ctx, sc_type_node, sc_type_arc_pos_const_perm, addr2);
            g_assert(iter.next());
            g_assert(iter.value(0) == addr1);
            g_assert(iter.value(1) == arc1);
            g_assert(iter.value(2) == addr2);
        }

        sc::Addr addr3 = ctx.createNode(sc_type_const);
        sc::Addr arc2 = ctx.createArc(sc_type_arc_pos_const_perm, addr3, arc1);

        g_assert(addr3.isValid());
        g_assert(arc2.isValid());

        g_assert(ctx.isElement(addr3));
        g_assert(ctx.isElement(arc2));

        {
            sc::Iterator5_a_a_f_a_a iter(ctx, sc_type_node, sc_type_arc_pos_const_perm, addr2, sc_type_arc_pos_const_perm, sc_type_node);

            g_assert(iter.next());

            g_assert(iter.value(0) == addr1);
            g_assert(iter.value(1) == arc1);
            g_assert(iter.value(2) == addr2);
            g_assert(iter.value(3) == arc2);
            g_assert(iter.value(4) == addr3);
        }

        {
            sc::Iterator5_a_a_f_a_f iter(ctx, sc_type_node, sc_type_arc_pos_const_perm, addr2, sc_type_arc_pos_const_perm, addr3);

            g_assert(iter.next());

            g_assert(iter.value(0) == addr1);
            g_assert(iter.value(1) == arc1);
            g_assert(iter.value(2) == addr2);
            g_assert(iter.value(3) == arc2);
            g_assert(iter.value(4) == addr3);
        }

        {
            sc::Iterator5_f_a_a_a_a iter(ctx, addr1, sc_type_arc_pos_const_perm, sc_type_node, sc_type_arc_pos_const_perm, sc_type_node);

            g_assert(iter.next());

            g_assert(iter.value(0) == addr1);
            g_assert(iter.value(1) == arc1);
            g_assert(iter.value(2) == addr2);
            g_assert(iter.value(3) == arc2);
            g_assert(iter.value(4) == addr3);
        }

        {
            sc::Iterator5_f_a_a_a_f iter(ctx, addr1, sc_type_arc_pos_const_perm, sc_type_node, sc_type_arc_pos_const_perm, addr3);

            g_assert(iter.next());

            g_assert(iter.value(0) == addr1);
            g_assert(iter.value(1) == arc1);
            g_assert(iter.value(2) == addr2);
            g_assert(iter.value(3) == arc2);
            g_assert(iter.value(4) == addr3);
        }

        {
            sc::Iterator5_f_a_f_a_a iter(ctx, addr1, sc_type_arc_pos_const_perm, addr2, sc_type_arc_pos_const_perm, sc_type_node);

            g_assert(iter.next());

            g_assert(iter.value(0) == addr1);
            g_assert(iter.value(1) == arc1);
            g_assert(iter.value(2) == addr2);
            g_assert(iter.value(3) == arc2);
            g_assert(iter.value(4) == addr3);
        }

        {
            sc::Iterator5_f_a_f_a_f iter(ctx, addr1, sc_type_arc_pos_const_perm, addr2, sc_type_arc_pos_const_perm, addr3);

            g_assert(iter.next());

            g_assert(iter.value(0) == addr1);
            g_assert(iter.value(1) == arc1);
            g_assert(iter.value(2) == addr2);
            g_assert(iter.value(3) == arc2);
            g_assert(iter.value(4) == addr3);
        }

        {
            std::string str("test content string");
            sc::Stream stream(str.c_str(), str.size(), SC_STREAM_FLAG_READ);
            sc::Addr link = ctx.createLink();

            g_assert(link.isValid());
            g_assert(ctx.isElement(link));
            g_assert(ctx.setLinkContent(link, stream));

            sc::Stream stream2;
            g_assert(ctx.getLinkContent(link, stream2));

            g_assert(stream.size() == stream2.size());
            g_assert(stream2.seek(SC_STREAM_SEEK_SET, 0));

            std::string str2;
            str2.resize(stream2.size());
            for (int i = 0; i < stream2.size(); ++i)
            {
                sc_char c;
                sc_uint32 readBytes;
                g_assert(stream2.read(&c, sizeof(c), readBytes));
                g_assert(readBytes == sizeof(c));

                str2[i] = c;
            }

            g_assert(str == str2);

            sc::tAddrList result;
            g_assert(ctx.findLinksByContent(stream, result));
            g_assert(result.size() == 1);
            g_assert(result.front() == link);
        }

    }

    shutdown_memory(false);
}

void test_common_streams()
{
    init_memory();

    {
        static int const length = 1024;
        unsigned char buff[length];

        for (int i = 0; i < length; ++i)
            buff[i] = rand() % 256;

        sc::Stream stream((sc_char*)buff, length, SC_STREAM_FLAG_READ);

        g_assert(stream.isValid());
        g_assert(stream.hasFlag(SC_STREAM_FLAG_READ));
        g_assert(stream.hasFlag(SC_STREAM_FLAG_SEEK));
        g_assert(stream.hasFlag(SC_STREAM_FLAG_TELL));
        g_assert(!stream.eof());
        g_assert(stream.pos() == 0);
        g_assert(stream.size() == length);

        for (int i = 0; i < length; ++i)
        {
            unsigned char c;
            sc_uint32 readBytes;
            g_assert(stream.read((sc_char*)&c, sizeof(c), readBytes));
            g_assert(c == buff[i]);
        }

        g_assert(stream.eof());
        g_assert(stream.pos() == length);

        // random seek
        static int const seekOpCount = 1000000;
        for (int i = 0; i < seekOpCount; ++i)
        {
            sc_uint32 pos = rand() % length;
            g_assert(stream.seek(SC_STREAM_SEEK_SET, pos));


            unsigned char c;
            sc_uint32 readBytes;
            g_assert(stream.read((sc_char*)&c, sizeof(c), readBytes));
            g_assert(c == buff[pos]);
        }
    }

    shutdown_memory(false);
}

void test_common_helper()
{
    init_memory();

    {
        std::string const idtf = "test_idtf";
        sc::MemoryContext ctx;
        sc::Addr addr = ctx.createNode(sc_type_const);

        g_assert(addr.isValid());
        g_assert(ctx.isElement(addr));

        g_assert(ctx.setSystemIdentifier(addr, idtf));
        sc::Addr addr2 = ctx.findElementBySystemIdentifier(idtf);
        g_assert(addr2.isValid());
        g_assert(addr2 == addr);
    }

    shutdown_memory(false);
}

int main(int argc, char *argv[])
{
    g_test_init(&argc, &argv, NULL);

    g_test_add_func("/common/elements", test_common_elements);
    g_test_add_func("/common/iterators", test_common_iterators);
    g_test_add_func("/common/streams", test_common_streams);
    g_test_add_func("/common/helper", test_common_helper);

    g_test_run();

    return 0;
}
