#include "sc_fs_storage.h"
#include "sc_segment.h"
#include <memory.h>

gchar *repo_path = 0;

const gchar *seg_dir = "segments";
const gchar *content_dir = "contents";
gchar segments_path[MAX_PATH_LENGTH + 1];
gchar contents_path[MAX_PATH_LENGTH + 1];
#define dir_permissions -1

sc_bool sc_fs_storage_initialize(const gchar *path)
{
    g_message("Initialize sc-storage from path: %s\n", path);

    g_snprintf(segments_path, MAX_PATH_LENGTH, "%s/%s", path, seg_dir);
    g_snprintf(contents_path, MAX_PATH_LENGTH, "%s/%s", path, content_dir);

    if (!g_file_test(contents_path, G_FILE_TEST_IS_DIR))
    {
        if (g_mkdir_with_parents(contents_path, dir_permissions) < 0)
            g_error("Can't create '%s' directory.", contents_path);
        return SC_FALSE;
    }

    repo_path = g_malloc0(sizeof(gchar) * (strlen(path) + 1));
    g_stpcpy(repo_path, path);

    return SC_TRUE;
}

sc_bool sc_fs_storage_shutdown(GPtrArray *segments)
{
    g_message("Shutdown sc-storage\n");
    g_message("Write storage into %s", repo_path);
    sc_fs_storage_write_to_path(segments);

    g_free( repo_path );

    return SC_TRUE;
}

void _get_segment_path(const gchar *path, 
                       sc_uint id,
                       sc_uint max_path_len,
                       gchar *res)
{
    g_snprintf(res, max_path_len, "%s/%.10d", path, id);
}

sc_segment* sc_fs_storage_load_segment(sc_uint id)
{
    sc_segment *segment = sc_segment_new(id);
    gchar file_name[MAX_PATH_LENGTH + 1];
    sc_uint length;

    _get_segment_path(segments_path, id, MAX_PATH_LENGTH, file_name);
    g_assert( g_file_get_contents(file_name, (gchar**)(&segment), &length, 0) );
    g_assert( length == sizeof(sc_segment) );

    return segment;
}

sc_bool sc_fs_storage_read_from_path(GPtrArray *segments)
{
    const gchar *fname = 0;
    gchar file_name[MAX_PATH_LENGTH + 1];
    gsize length = 0;
    sc_uint files_count = 0, idx;
    sc_segment *segment = 0;
    GDir *dir = 0;

    if (!g_file_test(repo_path, G_FILE_TEST_IS_DIR))
    {
        g_error("%s isn't a directory.", repo_path);
        return SC_FALSE;
    }

    if (!g_file_test(segments_path, G_FILE_TEST_IS_DIR))
    {
        g_message("There are no segments directory in %s", repo_path);
        return SC_FALSE;
    }

    dir = g_dir_open(segments_path, 0, 0);
    g_assert( dir != (GDir*)0 );

    // calculate files
    fname = g_dir_read_name(dir);
    while (fname != 0)
    {
        if (!g_file_test(fname, G_FILE_TEST_IS_DIR))
            files_count++;
        fname = g_dir_read_name(dir);
    }

    g_message("Segments founded: %u", files_count);

    // load segments
    for (idx = 0; idx < files_count; idx++)
        g_ptr_array_add( segments, (gpointer)sc_fs_storage_load_segment(idx) );

    g_message("Segments loaded: %u", segments->len);

    g_dir_close(dir);
    return SC_TRUE;
}

sc_bool sc_fs_storage_write_to_path(GPtrArray *segments)
{
    sc_uint idx = 0;
    sc_segment *segment = 0;
    GMappedFile *file = 0;
    gchar file_name[MAX_PATH_LENGTH + 1];
    gchar segments_path[MAX_PATH_LENGTH + 1];

    if (!g_file_test(repo_path, G_FILE_TEST_IS_DIR))
    {
        g_error("%s isn't a directory.", repo_path);
        return SC_FALSE;
    }

    // check if segments directory exists, if it doesn't, then create one
    g_snprintf(segments_path, MAX_PATH_LENGTH, "%s/%s", repo_path, seg_dir);
    if (!g_file_test(segments_path, G_FILE_TEST_IS_DIR))
    {
        if (g_mkdir_with_parents(segments_path, dir_permissions) < 0)
            return SC_FALSE;
    }

    for (idx = 0; idx < segments->len; idx++)
    {
        segment = (sc_segment*)g_ptr_array_index(segments, idx);
        _get_segment_path(segments_path, idx, MAX_PATH_LENGTH, file_name);
        g_file_set_contents(file_name, (gchar*)segment, sizeof(sc_segment), 0);
    }

    _get_segment_path(segments_path, idx, MAX_PATH_LENGTH, file_name);
    while (g_file_test(file_name, G_FILE_TEST_EXISTS))
    {
        g_remove(file_name);
        _get_segment_path(segments_path, ++idx, MAX_PATH_LENGTH, file_name);
    }


    return SC_TRUE;
}

