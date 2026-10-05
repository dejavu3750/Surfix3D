// io.hpp - PUBLIC, header-only C++ wrapper for the io module.
#ifndef SURFIX3D_IO_HPP
#define SURFIX3D_IO_HPP

#include "surfix3d/io/io_c_api.h"
#include "surfix3d/core/core.hpp"

#include <stdexcept>
#include <string>

namespace surfix3d::io {

class IoError : public std::runtime_error {
public:
    explicit IoError(sfx_io_status_t s)
        : std::runtime_error(sfx_io_status_string(s)), status_(s) {}
    sfx_io_status_t status() const noexcept { return status_; }
private:
    sfx_io_status_t status_;
};

// ---- progress-aware overloads --------------------------------------------
namespace detail {
    // Forward core::ProgressToken through the C callback boundary.
    inline int progressTrampoline(float f, const char* s, void* user) {
        auto* t = static_cast<core::ProgressToken*>(user);
        if (!t) return 0;
        if (f >= 0.0f) t->report(f, s);   // negative fraction = poll only
        return t->cancelled() ? 1 : 0;
    }
}

// Load a file into a managed Document, reporting to(and cancellable via) `token`.
inline core::DocumentHandle import(const std::string& path, core::ProgressToken* token) {
    sfx_document_t raw = nullptr;
    sfx_io_status_t st = sfx_io_import_progress(
        path.c_str(), &raw, token ? &detail::progressTrampoline : nullptr, token);
    if (st != SFX_IO_OK) {
        if (raw) sfx_document_destroy(raw);
        throw IoError(st);
    }
    return core::DocumentHandle::adopt(raw);
}

// Append a file into an existing Document, reporting to / cancellable via `token`.
inline void importInto(const std::string& path, const core::DocumentHandle& doc,
    core::ProgressToken* token) {
    sfx_io_status_t st = sfx_io_import_into_progress(
        path.c_str(), doc.raw(), token ? &detail::progressTrampoline : nullptr, token);
    if (st != SFX_IO_OK) throw IoError(st);
}

inline void exportTo(const std::string& path, const core::DocumentHandle& doc) {
    sfx_io_status_t st = sfx_io_export(path.c_str(), doc.raw());
    if (st != SFX_IO_OK) throw IoError(st);
}

} // namespace surfix3d::io

#endif // SURFIX3D_IO_HPP
