// LeakSanitizer suppressions, compiled into the binary so they apply on every run
// without setting LSAN_OPTIONS. The ASan runtime calls this at startup; in builds
// without sanitizers it isn't compiled at all.
//
// The NVIDIA GL driver allocates per-process state when a context is created and
// never frees it (the OS reclaims it at exit). A `leak:<module>` line hides a leak
// only when one of its stack frames is inside that library, so leaks in our own
// code are still reported.
#if defined(__has_feature)
#if __has_feature(address_sanitizer)
extern "C" const char* __lsan_default_suppressions() { // NOLINT(bugprone-reserved-identifier)
    return "leak:libnvidia-glcore.so\n"
           "leak:libnvidia-glsi.so\n";
}
#endif
#endif
