/*
 * render_c_api.h - PUBLIC, ABI-stable C boundary for the render module.
 */
#ifndef SURFIX3D_RENDER_C_API_H
#define SURFIX3D_RENDER_C_API_H

#include "surfix3d/render/render_export.h"  /* generated: defines RENDER_API */
#include "surfix3d/core/core_c_api.h"       /* sfx_document_t */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct sfx_renderer* sfx_renderer_t;

typedef enum sfx_render_status {
    SFX_RENDER_OK               = 0,
    SFX_RENDER_ERR_INVALID_ARG  = 1,
    SFX_RENDER_ERR_NO_CONTEXT   = 2  /* no current GL context */
} sfx_render_status_t;

typedef enum sfx_select_filter {
    SFX_SELECT_BODY = 0, SFX_SELECT_FACE = 1, SFX_SELECT_BREP_EDGE = 2,
	SFX_SELECT_ELEMENT = 3, SFX_SELECT_ELEM_EDGE = 4, SFX_SELECT_NODE = 5,
    SFX_SELECT_VERTEX = 6
} sfx_select_filter_t;

/* Pick result. handle payload (h0,h1,h2) meaning depends on `kind`:
   BODY/FACE : (meshPatchIndex, meshPatchGen, -)
   ELEMENT   : (meshPatchIndex, meshPatchGen, localElem)
   BREP_EDGE : (edgeIndex, edgeGen, 0)
   ELEM_EDGE : (nodeLo, nodeHi, 0)
   NODE      : (globalNodeIndex, 0, 0)
   has_hit = 0 means empty pick. */
typedef struct sfx_pick {
    uint32_t kind;
    uint32_t raw_id; /* dense id used for highlight; valid only if has_hit*/
    uint32_t h0, h1, h2; /* payload */
    int has_hit; /* 0 = miss, 1 = hit */
} sfx_pick_t;

/* Tunable shading parameters for the material/textured path. Applied per frame (no re-bake). */
typedef struct sfx_render_params {
    float exposure;       /* overall brightness multiplier              */
    float ambient;        /* ambient term                               */
    float diffuse;        /* diffuse (headlight) term                   */
    float specular;       /* specular highlight strength                */
    float shininess;      /* specular exponent                          */
    float rolloff;        /* highlight roll-off k: lin/(1+k*lin)        */
    float gamma;          /* output gamma (linear -> display)           */
    float glass_opacity;  /* multiplier on Blend-mode (glass) opacity   */
} sfx_render_params_t;


/* Mouse button for the camera controller. */
#define SFX_MOUSE_LEFT   0
#define SFX_MOUSE_MIDDLE 1
#define SFX_MOUSE_RIGHT  2

RENDER_API sfx_renderer_t sfx_renderer_create(void);
RENDER_API void           sfx_renderer_destroy(sfx_renderer_t renderer);

RENDER_API sfx_render_status_t sfx_renderer_set_selection_filter(sfx_renderer_t r, int filter);
RENDER_API sfx_render_status_t sfx_renderer_select_at(sfx_renderer_t r, int x, int y, int additive, sfx_pick_t* out_pick);
/* Select a body directly by its id (e.g. from a UI tree), bypassing pixel picking.
   Behaves like a BODY pick: clears prior selection unless `additive`. */
RENDER_API sfx_render_status_t sfx_renderer_select_body(sfx_renderer_t r, uint32_t body_id, int additive, sfx_pick_t* out_pick);
RENDER_API sfx_render_status_t sfx_renderer_clear_selection(sfx_renderer_t r);

RENDER_API void                sfx_render_params_default(sfx_render_params_t* out);
RENDER_API sfx_render_status_t sfx_renderer_get_render_params(sfx_renderer_t r, sfx_render_params_t* out);
RENDER_API sfx_render_status_t sfx_renderer_set_render_params(sfx_renderer_t r, const sfx_render_params_t* p);

RENDER_API sfx_render_status_t sfx_renderer_set_show_faces(sfx_renderer_t r, int show);
RENDER_API sfx_render_status_t sfx_renderer_set_show_brep_edges(sfx_renderer_t r, int show);
RENDER_API sfx_render_status_t sfx_renderer_set_show_elem_edges(sfx_renderer_t r, int show);
RENDER_API sfx_render_status_t sfx_renderer_set_show_nodes(sfx_renderer_t r, int show);
/* Flat (per-triangle) vs smooth (per-vertex, averaged within each BrepFace)
   shading. mode = sfx_shade_mode_t. Re-bakes normals on the next submit. */
RENDER_API sfx_render_status_t sfx_renderer_set_shade_mode(sfx_renderer_t r, int mode);

RENDER_API sfx_render_status_t sfx_renderer_get_view_matrix(sfx_renderer_t r, float out_m[16]);
/* Reserve N pixels at the top of the viewport for UI (top bar + ribbon) so the
   ViewCube is placed just below it. */
RENDER_API sfx_render_status_t sfx_renderer_set_top_inset(sfx_renderer_t renderer, int px);
/* ViewCube interaction. hover updates highlight; click returns handled=1 if the
   cube consumed the click (editor should then skip scene selection). Coords are
   window pixels, top-left origin. */
RENDER_API sfx_render_status_t sfx_renderer_viewcube_hover(sfx_renderer_t renderer, int x, int y);
RENDER_API sfx_render_status_t sfx_renderer_viewcube_click(sfx_renderer_t renderer, int x, int y, int* out_handled);
RENDER_API sfx_render_status_t sfx_renderer_release_left_no_anim(sfx_renderer_t renderer);

/* Upload a ViewCube face label texture (straight RGBA, 4 bytes/px, top-left origin).
   faceIndex: 0=FRONT 1=BACK 2=RIGHT 3=LEFT 4=TOP 5=BOTTOM. */
RENDER_API sfx_render_status_t sfx_renderer_set_cube_label(sfx_renderer_t renderer,
    int faceIndex, const unsigned char* rgba, int w, int h);

/* Show/hide an origin reference plane. plane: 0=XY, 1=YZ, 2=ZX. */
RENDER_API sfx_render_status_t sfx_renderer_set_origin_plane_visible(sfx_renderer_t renderer,
    int plane, int visible);
RENDER_API sfx_render_status_t sfx_renderer_set_show_grid(sfx_renderer_t renderer, int show);

/* Upload/refresh GPU buffers for a document's mesh patches. */
RENDER_API sfx_render_status_t sfx_renderer_submit(sfx_renderer_t renderer,
                                                   sfx_document_t doc);

/* Issue draw calls for the current frame. */
RENDER_API sfx_render_status_t sfx_renderer_draw(sfx_renderer_t renderer);

/* Camera control: editor forwards raw input; render owns the camera. */
RENDER_API sfx_render_status_t sfx_renderer_set_screen_size(sfx_renderer_t renderer, int w, int h);
RENDER_API sfx_render_status_t sfx_renderer_mouse_button(sfx_renderer_t renderer, 
	int button, int down, double x, double y);
RENDER_API sfx_render_status_t sfx_renderer_mouse_move(sfx_renderer_t renderer, double x, double y);
RENDER_API sfx_render_status_t sfx_renderer_mouse_scroll(sfx_renderer_t renderer, int up);
RENDER_API sfx_render_status_t sfx_renderer_frame_view(sfx_renderer_t renderer);
/* Snap the camera to a top (XY) view: look straight down the -Z axis, +Y up.
   `dist` is the eye distance above the XY plane; pass <= 0 to keep the current
   distance. Orientation + distance only; does not refit to a model. Works with
   or without a document. */
RENDER_API sfx_render_status_t sfx_renderer_view_top(sfx_renderer_t renderer, float dist);
RENDER_API sfx_render_status_t sfx_renderer_set_clear_color(sfx_renderer_t renderer,
    float r, float g, float b, float a);

/* Override a body's color (rgb 0..1). Faces of that body follow it. Re-bakes on next draw. */
RENDER_API sfx_render_status_t sfx_renderer_set_body_color(sfx_renderer_t renderer,
    uint32_t body_id,
    float r, float g, float b);

/* Body ops cascade to that body's faces. Face/element/edge ops are per-entity. */
RENDER_API sfx_render_status_t sfx_renderer_set_face_color(sfx_renderer_t r, uint32_t face_id, float rr, float gg, float bb);
RENDER_API sfx_render_status_t sfx_renderer_set_face_visible(sfx_renderer_t r, uint32_t face_id, int visible);
RENDER_API sfx_render_status_t sfx_renderer_set_element_visible(sfx_renderer_t r, uint32_t elem_id, int visible);
RENDER_API sfx_render_status_t sfx_renderer_set_brep_edge_visible(sfx_renderer_t r, uint32_t edge_id, int visible);

/* Refit distance + recenter on the model, KEEPING the current orientation (unlike frame_view). */
RENDER_API sfx_render_status_t sfx_renderer_focus_view(sfx_renderer_t renderer);
RENDER_API sfx_render_status_t sfx_renderer_set_body_visible(sfx_renderer_t r, uint32_t body_id, int visible);

/* Focus/recenter the camera on the surface point under (x,y) (depth unproject).
   Keeps orientation and zoom; also becomes the new orbit pivot. No-op if no surface. */
RENDER_API sfx_render_status_t sfx_renderer_focus_point_at(sfx_renderer_t renderer, int x, int y);

/* Asynchronous coordinate readout (no extra draw pass, no GPU stall).
   request=1: sample depth at (x,y) during the next draw(). The result is available ~1 frame later.
   Always returns the LATEST available result:
   out_xyz = 3 floats (world); *out_hit: 0 = nothing, 1 = model surface, 2 = Z=0 grid plane. */
RENDER_API sfx_render_status_t sfx_renderer_cursor_world(sfx_renderer_t r, int x, int y,
    int request, float* out_xyz, int* out_hit);

RENDER_API sfx_render_status_t sfx_renderer_clear_mark(sfx_renderer_t renderer);
RENDER_API sfx_render_status_t sfx_renderer_clear_hover(sfx_renderer_t renderer);
RENDER_API sfx_render_status_t sfx_renderer_hover_frame(sfx_renderer_t renderer, int x, int y);

RENDER_API sfx_render_status_t sfx_renderer_set_show_textures(sfx_renderer_t r, int show);


RENDER_API const char* sfx_render_status_string(sfx_render_status_t status);

/* ---- Section view ---- */
/* Sync surface pick: world xyz of the nearest visible face under (x,y). out_hit  = 0 on miss. */
RENDER_API sfx_render_status_t sfx_renderer_pick_surface(sfx_renderer_t r, int x, int y, float out_xyz[3], int* out_hit);
/* Section plane: dot(normal, p) = d. Normal need not be unit (normalized internally). */
RENDER_API sfx_render_status_t sfx_renderer_set_section_plane(sfx_renderer_t r, const float normal[3], float d);
RENDER_API sfx_render_status_t sfx_renderer_clear_section_plane(sfx_renderer_t r);
/* Number of plane/mesh intersection segments of the current section. */
RENDER_API sfx_render_status_t sfx_renderer_section_segment_count(sfx_renderer_t r, uint32_t* out_count);
/* One-sided section: hide geometry on the +normal side of the section plane (GPU clip). */
RENDER_API sfx_render_status_t sfx_renderer_set_section_one_sided(sfx_renderer_t r, int one_sided);
/* Section drag handle (flat arrow at the plane center, along the plane normal). */
RENDER_API sfx_render_status_t sfx_renderer_section_arrow(sfx_renderer_t r, float base[3], float tip[3], int* out_valid);
RENDER_API sfx_render_status_t sfx_renderer_set_section_arrow_hot(sfx_renderer_t r, int hot);
/* World -> window pixel (top-left origin). out_visible = 0 if behind / invalid */
RENDER_API sfx_render_status_t sfx_renderer_world_to_screen(sfx_renderer_t r, const float xyz[3], float out_xy[2], int* out_visible);
/* Fill the cut with a solid cap (one-sided section only; needs closed meshes + stencil buffer).
   rgb may be NULL to keep the current color. */
RENDER_API sfx_render_status_t sfx_renderer_set_section_cap(sfx_renderer_t r, int on, const float rgb[3]);

/* Isolate: hide every face, element that is not selected
	out_done = 1 if anything was isolated. Undo with sfx_renderer_unisolate(). */
RENDER_API sfx_render_status_t sfx_renderer_isolate_selection(sfx_renderer_t r, int* out_done);
/* Re-show only what the last isolate(s) hid.*/
RENDER_API sfx_render_status_t sfx_renderer_unisolate(sfx_renderer_t r);


#ifdef __cplusplus
}
#endif

#endif /* SURFIX3D_RENDER_C_API_H */
