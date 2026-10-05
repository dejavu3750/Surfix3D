// progress.hpp - generic progress + cooperative cancellation for long-running ops. 
// Cross-cutting primitive shared by every heavy operation: import, smooth, fille-hole, remesh
// ... The caller (usually the UI) owns a ProgressToken, the worker receives a ProgressSink
// view and periodically. 
// - calls report (fraction, stage) to push progress upward, and
// - checks cancelled()  to abort early (cooperative)
// Header-only, no engine dependices
#ifndef SURFIX3D_CORE_PROGRESS_HPP
#define SURFIX3D_CORE_PROGRESS_HPP

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <string>

namespace surfix3d::core {

	// Thrown by ProgressSink::throwIfCancelled() so an operation can unwind through
	// RAII (e.g. a Document::Transaction) and roll back cleanly on cancel.
	struct OperationCancelled {};

	// Lightweight, copyable view passed INTO a worker. A default-constructed sink
	// means "no progress requested" (every call is a no-op). Operations should take
	// `const ProgressSink&` as an optional trailing argument. Slicing lets a nested
	// step report 0..1 while contributing only its slice of the overall bar.
	class ProgressSink {
	public:
		using ReportFn = std::function<void(float/*fraction*/, const char* /*stage*/)>;
		using CancelFn = std::function<bool()>;

		ProgressSink() = default;
		ProgressSink(ReportFn report, CancelFn cancel)
			: report_(std::move(report)), cancel_(std::move(cancel)) {
		}

		void report(float fraction, const char* stage = nullptr) const {
			if (!report_) return;
			if (fraction < 0.0f) fraction = 0.0f;
			if (fraction > 1.0f) fraction = 1.0f;
			report_(lo_ + (hi_ - lo_) * fraction, stage); // remap into this slice
		}

		bool cancelled() const {
			return cancel_ && cancel_();
		}
		void thrownIfCancelled() const {
			if (cancelled()) throw OperationCancelled();
		}

		// Map sub-range [begin, end] (of this sink) onto a child sink.
		ProgressSink slice(float begin, float end) const {
			ProgressSink s;
			s.report_ = report_;
			s.cancel_ = cancel_;
			s.lo_ = lo_ + (hi_ - lo_) * begin;
			s.hi_ = lo_ + (hi_ - lo_) * end;
			return s;
		}

	private:

		ReportFn report_;
		CancelFn cancel_;
		float lo_ = 0.0f, hi_ = 1.0f;
	};

	// Thread-safe shared state owned by the caller. The worker may run on another 
	// thread while the UI thread reads progress and requests cancellation. The UI thread may also call report() to push progress
	class ProgressToken {
	public:
		// --- worker side ---
		void report(float fraction, const char* stage) {
			if (fraction < 0.0f) fraction = 0.0f;
			if (fraction > 1.0f) fraction = 1.0f;
			fraction_.store(fraction, std::memory_order_relaxed);
			if (stage) { std::lock_guard<std::mutex> lk(stageMx_); stage_ = stage; }
		}

		// --- caller side ---
		float       fraction()  const { return fraction_.load(std::memory_order_relaxed); }
		std::string stage()     const { std::lock_guard<std::mutex> lk(stageMx_); return stage_; }
		void        cancel() { cancelled_.store(true, std::memory_order_relaxed); }
		bool        cancelled() const { return cancelled_.load(std::memory_order_relaxed); }

		// Reuse the same token for a new run.
		void reset() {
			fraction_.store(0.0f, std::memory_order_relaxed);
			cancelled_.store(false, std::memory_order_relaxed);
			std::lock_guard<std::mutex> lk(stageMx_); stage_.clear();
		}

		// A sink wired to this token (for in-process callers that link core directly).
		ProgressSink sink() {
			return ProgressSink(
				[this](float f, const char* s) { report(f, s); },
				[this]() { return cancelled(); });
		}

	private:
		std::atomic<float> fraction_{ 0.0f };
		std::atomic<bool> cancelled_{ false };
		mutable std::mutex stageMx_;
		std::string stage_;
	};

}	// namespace surfix3d::core

#endif // SURFIX3D_CORE_PROGRESS_HPP