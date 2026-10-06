// render.hpp - PUBLIC, header-only C++ wrapper for the render module.
#ifndef SURFIX3D_RENDER_HPP
#define SURFIX3D_RENDER_HPP

#include "surfix3d/render/render_c_api.h"
#include "surfix3d/core/core.hpp"

#include <memory>
#include <stdexcept>

namespace surfix3d::render {

class RenderError : public std::runtime_error {
public:
    explicit RenderError(sfx_render_status_t s)
        : std::runtime_error(sfx_render_status_string(s)), status_(s) {}
    sfx_render_status_t status() const noexcept { return status_; }
private:
    sfx_render_status_t status_;
};

inline void check(sfx_render_status_t s) {
    if (s != SFX_RENDER_OK) throw RenderError(s);
}

class Renderer {
public:
    Renderer() : handle_(sfx_renderer_create(), &sfx_renderer_destroy) {
        if (!handle_) throw std::bad_alloc();
    }
    void submit(const core::DocumentHandle& doc) {
        check(sfx_renderer_submit(handle_.get(), doc.raw()));
    }
    void draw() { check(sfx_renderer_draw(handle_.get())); }

	// --- camera control (editor forwards input here) ---
    void setScreenSize(int w, int h) {
        check(sfx_renderer_set_screen_size(handle_.get(), w, h));
	}
    void mouseButton(int button, bool down, double x, double y) {
        check(sfx_renderer_mouse_button(handle_.get(), button, down ? 1 : 0, x, y));
	}
    void mouseMove(double x, double y) {
        check(sfx_renderer_mouse_move(handle_.get(), x, y));
    }
    void mouseScroll(bool up) {
        check(sfx_renderer_mouse_scroll(handle_.get(), up ? 1 : 0));
	}
    void frameView() {
        check(sfx_renderer_frame_view(handle_.get()));
	}
    // Snap to a top (XY) view: look straight down -Z, +Y up. `dist` is the eye
    // distance above the XY plane; pass <= 0 to keep the current distance.
    void viewTop(float dist = 0.0f) {
        check(sfx_renderer_view_top(handle_.get(), dist));
    }
    void setClearColor(float r, float g, float b, float a) {
        check(sfx_renderer_set_clear_color(handle_.get(), r, g, b, a));
    }
    void setBodyColor(uint32_t bodyId, float r, float g, float b) {
        check(sfx_renderer_set_body_color(handle_.get(), bodyId, r, g, b));
    }
    void focusView() { check(sfx_renderer_focus_view(handle_.get())); }
    void setBodyVisible(uint32_t bodyId, bool visible) {
        check(sfx_renderer_set_body_visible(handle_.get(), bodyId, visible ? 1 : 0));
	}
    void setFaceColor(uint32_t f, float r, float g, float b) { 
        check(sfx_renderer_set_face_color(handle_.get(), f, r, g, b)); 
    }
    void setFaceVisible(uint32_t f, bool v) { 
        check(sfx_renderer_set_face_visible(handle_.get(), f, v ? 1 : 0)); 
    }
    void setElementVisible(uint32_t e, bool v) { 
        check(sfx_renderer_set_element_visible(handle_.get(), e, v ? 1 : 0)); 
    }
    void setBrepEdgeVisible(uint32_t e, bool v) {
        check(sfx_renderer_set_brep_edge_visible(handle_.get(), e, v ? 1 : 0)); 
    }    
    void setSelectionFilter(int f) { 
        check(sfx_renderer_set_selection_filter(handle_.get(), f)); 
    }
    sfx_pick_t selectAt(int x, int y, bool additive) { 
        sfx_pick_t p{};
        check(sfx_renderer_select_at(handle_.get(), x, y, additive ? 1 : 0, &p));
        return p;
    }    
    sfx_pick_t selectBody(uint32_t bodyId, bool additive) {
        sfx_pick_t p{};
        check(sfx_renderer_select_body(handle_.get(), bodyId, additive ? 1 : 0, &p));
        return p;
    }
    void clearSelection() { 
        check(sfx_renderer_clear_selection(handle_.get())); 
    }
    void setShowNodes(bool s) { 
        check(sfx_renderer_set_show_nodes(handle_.get(), s ? 1 : 0)); 
    }
    void setShowFaces(bool s) { 
        check(sfx_renderer_set_show_faces(handle_.get(), s ? 1 : 0)); 
    }
    void setShowBrepEdges(bool s) { 
        check(sfx_renderer_set_show_brep_edges(handle_.get(), s ? 1 : 0)); 
    }
    void setShowElemEdges(bool s) { 
        check(sfx_renderer_set_show_elem_edges(handle_.get(), s ? 1 : 0)); 
    }
    void setShowTextures(bool s) { 
        check(sfx_renderer_set_show_textures(handle_.get(), s ? 1 : 0)); 
	}
    void focusPointAt(int x, int y) { 
        check(sfx_renderer_focus_point_at(handle_.get(), x, y)); 
    }
    void clearMark() { 
        check(sfx_renderer_clear_mark(handle_.get())); 
    }    
    void clearHover() { 
        check(sfx_renderer_clear_hover(handle_.get())); 
    }   
    void hoverFrame(int x, int y) { 
        check(sfx_renderer_hover_frame(handle_.get(), x, y));
    }
    // Fills a column-major 4x4 view matrix (16 floats) for the axis gizmo.
    void getViewMatrix(float out_m[16]) {
        check(sfx_renderer_get_view_matrix(handle_.get(), out_m));
    }
    void setTopInset(int px) {
        check(sfx_renderer_set_top_inset(handle_.get(), px));
    }
    void viewCubeHover(int x, int y) {
        check(sfx_renderer_viewcube_hover(handle_.get(), x, y));
    }
    bool viewCubeClick(int x, int y) {
        int handled = 0;
        check(sfx_renderer_viewcube_click(handle_.get(), x, y, &handled));
        return handled != 0;
    }
    void releaseLeftNoAnim() {
        check(sfx_renderer_release_left_no_anim(handle_.get()));
    }
    void setCubeLabel(int faceIndex, const unsigned char* rgba, int w, int h) {
        check(sfx_renderer_set_cube_label(handle_.get(), faceIndex, rgba, w, h));
    }
    void setOriginPlaneVisible(int plane, bool visible) {
        check(sfx_renderer_set_origin_plane_visible(handle_.get(), plane, visible ? 1 : 0));
    }
    void setShowGrid(bool s) {
        check(sfx_renderer_set_show_grid(handle_.get(), s ? 1 : 0));
    }
    void setShadeMode(int mode) {
        check(sfx_renderer_set_shade_mode(handle_.get(), mode));
    }
    sfx_render_params_t renderParams() const {
        sfx_render_params_t p{};
        check(sfx_renderer_get_render_params(handle_.get(), &p));
        return p;
    }
    void setRenderParams(const sfx_render_params_t& p) {
        check(sfx_renderer_set_render_params(handle_.get(), &p));
    }
    static sfx_render_params_t defaultRenderParams() {
        sfx_render_params_t p{}; sfx_render_params_default(&p); return p;
    }
    void setShowTexture(bool s) {
        check(sfx_renderer_set_show_textures(handle_.get(), s ? 1 : 0));
    }
    // Async readout: request=true queues a sample; always returns the latest result.
    // 0 = nothing, 1 = model surface, 2 = grid plane. out = world xyz (document units).
    int cursorWorld(int x, int y, float out[3], bool request = true) {
        int hit = 0;
        check(sfx_renderer_cursor_world(handle_.get(), x, y, request ? 1 : 0, out, &hit));
        return hit;
    }

    ///////////////////////////////
    // --- section view ---     ///
    ///////////////////////////////
    bool pickSurface(int x, int y, float out[3]) {
        int hit = 0;
        check(sfx_renderer_pick_surface(handle_.get(), x, y, out, &hit));
		return hit != 0;
    }
    void setSectionPlane(const float n[3], float d) {
        check(sfx_renderer_set_section_plane(handle_.get(), n, d));
	}
    void clearSectionPlane() {
        check(sfx_renderer_clear_section_plane(handle_.get()));
	}
    uint32_t sectionSegmentCount() {
        uint32_t n = 0;
		check(sfx_renderer_section_segment_count(handle_.get(), &n));
        return n;
    }
    void setSectionOneSided(bool on) {
        check(sfx_renderer_set_section_one_sided(handle_.get(), on ? 1 : 0));
    }
    bool sectionArrow(float base[3], float tip[3]) {
        int ok = 0;
        check(sfx_renderer_section_arrow(handle_.get(), base, tip, &ok));
        return ok != 0;
    }
    void setSectionArrowHot(bool hot) {
        check(sfx_renderer_set_section_arrow_hot(handle_.get(), hot ? 1 : 0));
    }
    bool worldToScreen(const float xyz[3], float out_xy[2]) {
        int vis = 0;
        check(sfx_renderer_world_to_screen(handle_.get(), xyz, out_xy, &vis));
        return vis != 0;
    }
    void setSectionCap(bool on, const float* rgb = nullptr) {
        check(sfx_renderer_set_section_cap(handle_.get(), on ? 1 : 0, rgb));
    }
    bool isolateSelection() {
        int done = 0;
		check(sfx_renderer_isolate_selection(handle_.get(), &done));
		return done != 0;
    }
    void unisolate() {
        check(sfx_renderer_unisolate(handle_.get()));
	}


private:
    std::unique_ptr<sfx_renderer, decltype(&sfx_renderer_destroy)> handle_;
};

} // namespace surfix3d::render

#endif // SURFIX3D_RENDER_HPP
