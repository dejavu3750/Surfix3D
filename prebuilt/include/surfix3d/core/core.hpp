// core.hpp - PUBLIC, header-only C++ convenience wrapper.
//
// Compiled by the consumer with their own compiler, so no C++ types cross the
// DLL boundary: this wrapper only calls the stable C API in core_c_api.h.
#ifndef SURFIX3D_CORE_HPP
#define SURFIX3D_CORE_HPP

#include "surfix3d/core/core_c_api.h"
#include "surfix3d/core/progress.hpp"
#include "surfix3d/core/units.hpp"

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace surfix3d::core {

// Thrown when a C API call reports a non-OK status.
class Error : public std::runtime_error {
public:
    explicit Error(sfx_status_t s)
        : std::runtime_error(sfx_status_string(s)), status_(s) {}
    sfx_status_t status() const noexcept { return status_; }
private:
    sfx_status_t status_;
};

inline void check(sfx_status_t s) {
    if (s != SFX_OK) throw Error(s);
}

// RAII wrapper around the opaque sfx_document handle (the whole open model).
class DocumentHandle {
public:
    DocumentHandle()
        : handle_(sfx_document_create(), &sfx_document_destroy) {
        if (!handle_) throw std::bad_alloc();
    }

    // Take ownership of a handle produced by another engine module (e.g. io).
    static DocumentHandle adopt(sfx_document_t handle) { return DocumentHandle(handle); }

    uint32_t nodeCount() const {
        uint32_t c = 0; check(sfx_document_node_count(handle_.get(), &c)); return c;
    }
    uint32_t elementCount() const {
        uint32_t c = 0; check(sfx_document_element_count(handle_.get(), &c)); return c;
    }
    uint32_t faceCount() const {
        uint32_t c = 0; check(sfx_document_face_count(handle_.get(), &c)); return c;
    }
    uint32_t edgeCount() const {
        uint32_t c = 0; check(sfx_document_edge_count(handle_.get(), &c)); return c;
    }
    uint32_t vertexCount() const {
		uint32_t c = 0; check(sfx_document_vertex_count(handle_.get(), &c)); return c;
    }
    std::vector<uint32_t> bodyIds() const {
		uint32_t n = 0; check(sfx_document_body_count(handle_.get(), &n));
        std::vector<uint32_t> ids(n);
        if (n) {
            uint32_t w = 0;
			check(sfx_document_body_ids(handle_.get(), ids.data(), n, &w));
            ids.resize(w);
        }
        return ids;
    }        
    sfx_document_t raw() const noexcept {
        return handle_.get();
    }
    std::string bodyName(uint32_t bodyIndex) const {
        char buf[256] = {};
        check(sfx_document_body_name(handle_.get(), bodyIndex, buf, sizeof(buf)));
        return std::string(buf);
    }
    LengthUnit lengthUnit() const {
        sfx_length_unit_t u = SFX_UNIT_MM;
        check(sfx_document_length_unit(handle_.get(), &u));
        return static_cast<LengthUnit>(u);
	}
    // Delete faces by renderer face index. One call = one undo step.
    uint32_t deleteFaces(const std::vector<uint32_t>& faceIds) {
        uint32_t n = 0;
        check(sfx_document_delete_faces(handle_.get(), faceIds.data(), (uint32_t)faceIds.size(), &n));
        return n;
    }
    bool canUndo() const { return sfx_document_can_undo(handle_.get()) != 0; }
    bool canRedo() const { return sfx_document_can_redo(handle_.get()) != 0; }
    void undo() { check(sfx_document_undo(handle_.get())); }
    void redo() { check(sfx_document_redo(handle_.get())); }
    void undo(size_t n) { check(sfx_document_undo_n(handle_.get(), n)); }
    void redo(size_t n) { check(sfx_document_redo_n(handle_.get(), n)); }
    std::vector<std::string> undoHistory() const {
		std::vector<std::string> labels;
        for (size_t i = 0, c = sfx_document_undo_count(handle_.get()); i < c; ++i) {
			const char* s = sfx_document_undo_label(handle_.get(), i);
			labels.emplace_back(s ? s : "Edit");
        }
        return labels;
    }
    std::vector<std::string> redoHistory() const {
		std::vector<std::string> labels;
        for (size_t i = 0, c = sfx_document_redo_count(handle_.get()); i < c; ++i) {
			const char* s = sfx_document_redo_label(handle_.get(), i);
			labels.emplace_back(s ? s : "Edit");
        }
        return labels;
    }
    size_t undoCount() const { return sfx_document_undo_count(handle_.get()); }
    size_t redoCount() const { return sfx_document_redo_count(handle_.get()); }

private:
    explicit DocumentHandle(sfx_document_t handle)
        : handle_(handle, &sfx_document_destroy) {
        if (!handle_) throw std::bad_alloc();
    }
    std::unique_ptr<sfx_document, decltype(&sfx_document_destroy)> handle_;
};

} // namespace surfix3d::core

#endif // SURFIX3D_CORE_HPP
