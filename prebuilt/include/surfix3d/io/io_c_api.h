/*
 * io_c_api.h - PUBLIC, ABI-stable C boundary for the io module.
 * Imports files into a core Document; exports a Document back to disk.
 */
#ifndef SURFIX3D_IO_C_API_H
#define SURFIX3D_IO_C_API_H

#include "surfix3d/io/io_export.h"       /* generated: defines IO_API */
#include "surfix3d/core/core_c_api.h"    /* sfx_document_t (a C handle) */

#ifdef __cplusplus
extern "C" {
#endif

typedef enum sfx_io_status {
    SFX_IO_OK           = 0,
    SFX_IO_ERR_OPEN     = 1,  /* file could not be opened */
    SFX_IO_ERR_PARSE    = 2,  /* malformed content */
    SFX_IO_ERR_UNSUPPORTED = 3,  /* extension/format not handled */
    SFX_IO_ERR_INVALID_ARG = 4,
    SFX_IO_ERR_CANCELLED = 5   /* aborted via the progress callback */
} sfx_io_status_t;

/* Detected/selected file format. Only Nastran BDF is supported for now. */
typedef enum sfx_io_format {
    SFX_FMT_UNKNOWN = 0,
    SFX_FMT_BDF,          /* Nastran Bulk Data (.bdf/.nas/.dat) - DCAD */
    SFX_FMT_OBJ,
	SFX_FMT_GLTF,          /* glTF 2.0 (.glb binary / .gltf JSON) */
} sfx_io_format_t;

/* Guess the format from a path's extension. */
IO_API sfx_io_format_t sfx_io_detect_format(const char* path);

/* Import `path` into a freshly created document (*out_doc). Caller owns it and
 * must free it with sfx_document_destroy(). */
IO_API sfx_io_status_t sfx_io_import(const char* path,
                                     sfx_document_t* out_doc);

/* Import `path` into an existing document, appending its geometry (each file
 * becomes its own body via connected-component detection). The caller retains
 * ownership of `doc`. */
IO_API sfx_io_status_t sfx_io_import_into(const char* path, sfx_document_t doc);

/* Progress + cooperative cancel. `cb` is invoked with fraction in [0,1] and an
 * optional `stage` label; a negative fraction is a "poll only" call (do not update
 * the UI). Returning non-zero requests cancellation. `cb` may be NULL. */
typedef int (*sfx_progress_cb)(float fraction, const char* stage, void* user);

IO_API sfx_io_status_t sfx_io_import_progress(const char* path,
    sfx_document_t* out_doc,
    sfx_progress_cb cb, void* user);

IO_API sfx_io_status_t sfx_io_import_into_progress(const char* path,
    sfx_document_t doc,
    sfx_progress_cb cb, void* user);

/* Export `doc` to `path`, choosing the writer from the extension. */
IO_API sfx_io_status_t sfx_io_export(const char* path,
                                     sfx_document_t doc);

IO_API const char* sfx_io_status_string(sfx_io_status_t status);

#ifdef __cplusplus
}
#endif

#endif /* SURFIX3D_IO_C_API_H */
