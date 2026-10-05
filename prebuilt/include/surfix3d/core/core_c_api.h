/*
 * core_c_api.h - PUBLIC, ABI-stable C boundary for the core module.
 *
 * Only C types cross this line: opaque handles + POD. This is what makes the
 * pre-built DLL/SO safe to link against from any compiler/runtime. C++
 * ergonomics live in core.hpp (compiled on the consumer side).
 */
#ifndef SURFIX3D_CORE_C_API_H
#define SURFIX3D_CORE_C_API_H

#include "surfix3d/core/core_export.h"  /* generated: defines CORE_API */

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Status codes returned across the boundary (no exceptions leak out). */
typedef enum sfx_status {
    SFX_OK               = 0,
    SFX_ERR_INVALID_ARG  = 1,
    SFX_ERR_OUT_OF_MEMORY = 2,
    SFX_ERR_INTERNAL     = 3
} sfx_status_t;

/* Human-readable name for a status code (never NULL). */
CORE_API const char* sfx_status_string(sfx_status_t status);

/* ------------------------------------------------------------------------- */
/* Document: the top-level opaque handle for ONE open model (owns the B-Rep + */
/* mesh data and the undo/redo history). io builds one when importing a file. */
typedef struct sfx_document* sfx_document_t;

CORE_API sfx_document_t sfx_document_create(void);
CORE_API void           sfx_document_destroy(sfx_document_t doc);

/* Display name of a body (by body index). Writes a NUL-terminated string; empty
   if the body has no name. Returns SFX_ERR_INVALID_ARG on null. */
CORE_API sfx_status_t sfx_document_body_name(sfx_document_t doc, uint32_t body_index,
    char* out, size_t out_size);

/* Counts of LIVE entities (for verification / UI). */
CORE_API sfx_status_t sfx_document_body_count(sfx_document_t doc, uint32_t* out_count);
CORE_API sfx_status_t sfx_document_body_ids(sfx_document_t doc, uint32_t* out_ids,
    uint32_t max, uint32_t* out_written);
CORE_API sfx_status_t sfx_document_node_count(sfx_document_t doc, uint32_t* out_count);
CORE_API sfx_status_t sfx_document_element_count(sfx_document_t doc, uint32_t* out_count);
CORE_API sfx_status_t sfx_document_face_count(sfx_document_t doc, uint32_t* out_count);
CORE_API sfx_status_t sfx_document_edge_count(sfx_document_t doc, uint32_t* out_count);
CORE_API sfx_status_t sfx_document_vertex_count(sfx_document_t doc, uint32_t* out_count);

/* Model AABB in world space (doubles). out_min/out_max point to 3 double each. 
   Returns SFX_ERR_INVALID_ARG on null; box may be inverted/empty if no geometry. */ 
CORE_API sfx_status_t sfx_document_bounds(sfx_document_t doc, double* out_min, double* out_max);

/* ---- Length unit: the unit the document's coordinates are stored in. ----
   Set by io at import time (read from the file header, or a per-format default).
   Coordinates are NEVER rescaled; UIs convert for display only. */
typedef enum sfx_length_unit {
    SFX_UNIT_MM = 0,
    SFX_UNIT_CM = 1,
    SFX_UNIT_M = 2,
    SFX_UNIT_IN = 3,
    SFX_UNIT_FT = 4,
    SFX_UNIT_COUNT
} sfx_length_unit_t;

CORE_API sfx_status_t sfx_document_length_unit(sfx_document_t doc, sfx_length_unit_t* out_unit);
CORE_API sfx_status_t sfx_document_set_length_unit(sfx_document_t doc, sfx_length_unit_t unit);

/* ---- Render-data snapshot: flat, GPU-friendly arrays extracted from the model ----
   Built on demand, freed by the caller. Returns pointers stay valid until _free. */

typedef struct sfx_render_counts {
    uint32_t faces; 
    uint32_t elements;
	uint32_t brep_edges;
    uint32_t elem_edges;
    uint32_t nodes;
} sfx_render_counts_t;

typedef struct sfx_face_batch {
    uint32_t body_id;
    uint32_t first_vertex;
    uint32_t vertex_count;
} sfx_face_batch_t;
typedef struct sfx_render_data* sfx_render_data_t;

/* Phong material snapshot (POD). has_texture=1 -> query the path with
    sfx_render_data_material_texture().*/
typedef struct sfx_material {
    float kd[3]; // diffuse color
    float ka[3]; // ambient color
    float ks[3]; // specular color
    float ns;    // specular exponent
    float opacity; // transparency (1.0 = opaque)
    int has_texture; // 1 if diffuseTexture is valid, 0 otherwise
    int   alpha_mode;   // 0 = opaque, 1 = mask (cutout), 2 = blend
    float alpha_cutoff; // mask threshold (alpha < cutoff -> discarded)
} sfx_material_t;

/* A run of face vertices that share one material (contiguous in the faces array).
   Draw with the material's texture/color; skip if body_id is hidden. */
typedef struct sfx_face_material_batch {
    uint32_t material_id; // index into sfx_render_data_materials, 0xffffffff if no material

    uint32_t body_id; // render body index (faceBodyOf) 0xffffffff if no body
    uint32_t first_vertex; // index into sfx_render_data_faces()
    uint32_t vertex_count; // number of vertices in this run
} sfx_face_material_batch_t;

/* Shading mode for tessellated faces.
   FLAT   = one normal per triangle (faceted look).
   SMOOTH = per-vertex normal, averaged within each BrepFace/MeshPatch so that
            surface boundaries (real feature edges) stay sharp. */
typedef enum sfx_shade_mode {
    SFX_SHADE_FLAT = 0,
    SFX_SHADE_SMOOTH = 1
} sfx_shade_mode_t;

CORE_API sfx_render_data_t sfx_render_data_build(sfx_document_t doc);
/* Same as sfx_render_data_build, but selects the face-normal shading mode. */
CORE_API sfx_render_data_t sfx_render_data_build_ex(sfx_document_t doc, int shade_mode);
CORE_API void sfx_render_data_free(sfx_render_data_t rd);
CORE_API sfx_render_counts_t sfx_render_data_counts(sfx_render_data_t rd);

/* Resolve a pick handle (from sfx_renderer_select_at) to a human-readable entity
   string. kind matches sfx_select_filter_t. Writes NUL-terminated into out. */
CORE_API sfx_status_t sfx_document_describe_pick(sfx_document_t doc, uint32_t kind,
    uint32_t h0, uint32_t h1, uint32_t h2, char* out, size_t out_size);

/* Describe a body by its render body index (the value in face_body_of / faceToBody). */
CORE_API sfx_status_t sfx_document_describe_body(sfx_document_t doc, uint32_t body_index, char* out, size_t out_size);

/* AABB of a picked entity, by handle. kind matches sfx_select_filter_t, (h0,h1,h2)
   is the pick handle payload. out_min/out_max: 3 doubles each. Returns
   SFX_ERR_INVALID_ARG if the entity is empty/stale. */
CORE_API sfx_status_t sfx_document_entity_bounds(sfx_document_t doc, uint32_t kind,
    uint32_t h0, uint32_t h1, uint32_t h2, double* out_min, double* out_max);

/* Faces: pos + nrm (6f/vertex). face_ids = 2 uints/vertex: (faceId, elemId). */
CORE_API const float* sfx_render_data_faces(sfx_render_data_t rd, uint32_t* out_vertex_count);
CORE_API const uint32_t* sfx_render_data_face_ids(sfx_render_data_t rd, uint32_t* out_vertex_count);
CORE_API const uint32_t* sfx_render_data_face_body_of(sfx_render_data_t rd, uint32_t* out_face_count);
CORE_API const sfx_face_batch_t* sfx_render_data_face_batches(sfx_render_data_t rd, uint32_t* out_count);

/* Brep edges: pos (3f/vertex), ids = 2 uints/vertex: (brepEdgeId, faceId). */
CORE_API const float* sfx_render_data_brep_edges(sfx_render_data_t rd, uint32_t* out_vertex_count);
CORE_API const uint32_t* sfx_render_data_brep_edge_ids(sfx_render_data_t rd, uint32_t* out_vertex_count);

/* Elem edges: pos (3f/vertex). ids = 2 uints/vertex: (elemId, faceId). */
CORE_API const float* sfx_render_data_elem_edges(sfx_render_data_t rd, uint32_t* out_vertex_count);
CORE_API const uint32_t* sfx_render_data_elem_edge_ids(sfx_render_data_t rd, uint32_t* out_vertex_count);
CORE_API const float* sfx_render_data_nodes(sfx_render_data_t rd, uint32_t* out_count);

/* Body index per TRIANGLE (length = *out_tri_count = vertex_count/3). 0xffffffff = none. */
CORE_API const uint32_t* sfx_render_data_face_bodies(sfx_render_data_t rd, uint32_t* out_tri_count);

/* 
    Pick handles: core identity per vertex, parallel to the *_ids arrays above. 
    face_handles: 3 units/vertex = (meshPatchId, meshPatchGen, localElem)
    brep_edge_handles: 2 units/vertex = (edgeIndex, edgeGen).
    elem_edge_handles: 2 units/vertex = (nodeLo, nodeHi) [global node indices]
    node_globals: 1 unit/vertex = global node index
 */
CORE_API const uint32_t* sfx_render_data_face_handles(sfx_render_data_t rd, uint32_t* out_vertex_count);
CORE_API const uint32_t* sfx_render_data_brep_edge_handles(sfx_render_data_t rd, uint32_t* out_vertex_count);
CORE_API const uint32_t* sfx_render_data_elem_edge_handles(sfx_render_data_t rd, uint32_t* out_vertex_count);
CORE_API const uint32_t* sfx_render_data_node_globals(sfx_render_data_t rd, uint32_t* out_vertex_count);

/* Per-face-vertex UVs: 2 floats/vertex, parallel to sfx_render_data_faces().*/
CORE_API const float* sfx_render_data_face_uvs(sfx_render_data_t rd, uint32_t* out_vertex_count);
/* Material table + per-material diffuse texture path (absolute; "" if none).*/
CORE_API const sfx_material_t* sfx_render_data_materials(sfx_render_data_t rd, uint32_t* out_count);
CORE_API const char* sfx_render_data_material_texture(sfx_render_data_t rd, uint32_t material_id);

/* Material-grouped draw runs (parallel semantics to sfx_render_data_face_batches)*/
CORE_API const sfx_face_material_batch_t* sfx_render_data_face_material_batches(sfx_render_data_t rd, uint32_t* out_count);


// Commands
CORE_API sfx_status_t sfx_document_delete_faces(sfx_document_t doc, 
                                                const uint32_t* face_ids, 
                                                uint32_t count,
                                                uint32_t* out_deleted);

// History 
CORE_API int sfx_document_can_undo(sfx_document_t doc);
CORE_API int sfx_document_can_redo(sfx_document_t doc);
CORE_API sfx_status_t sfx_document_undo(sfx_document_t doc);
CORE_API sfx_status_t sfx_document_redo(sfx_document_t doc);
CORE_API sfx_status_t sfx_document_undo_n(sfx_document_t doc, size_t n);
CORE_API sfx_status_t sfx_document_redo_n(sfx_document_t doc, size_t n);
CORE_API size_t sfx_document_undo_count(const sfx_document_t doc);
CORE_API size_t sfx_document_redo_count(const sfx_document_t doc);
CORE_API const char* sfx_document_undo_label(const sfx_document_t doc, size_t index);
CORE_API const char* sfx_document_redo_label(const sfx_document_t doc, size_t index);



#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SURFIX3D_CORE_C_API_H */
