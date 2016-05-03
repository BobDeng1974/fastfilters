// fastfilters
// Copyright (c) 2016 Sven Peter
// sven.peter@iwr.uni-heidelberg.de or mail@svenpeter.me
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
// Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#ifndef FASTFILTERS_COMMON_H
#define FASTFILTERS_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>

#include "fastfilters.h"
#include "config.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifdef HAVE_BUILTIN_EXPECT
#define likely(x) __builtin_expect(x, true)
#define unlikely(x) __builtin_expect(x, false)
#else
#define likely(x) (x)
#define unlikely(x) (x)
#endif

#define border_0 mirror
#define border_enum_0 FASTFILTERS_BORDER_MIRROR

#define border_1 optimistic
#define border_enum_1 FASTFILTERS_BORDER_OPTIMISTIC

#define border_2 ptr
#define border_enum_2 FASTFILTERS_BORDER_PTR

#define ARRAY_LENGTH(x) (sizeof((x)) / sizeof((x)[0]))

typedef enum {
    FASTFILTERS_BORDER_MIRROR,
    FASTFILTERS_BORDER_OPTIMISTIC,
    FASTFILTERS_BORDER_PTR
} fastfilters_border_treatment_t;

typedef bool (*impl_fn_t)(const float *, const float *, const float *, size_t, size_t, size_t, size_t, float *, size_t,
                          size_t, const fastfilters_kernel_fir_t kernel);

typedef bool (*convolve_fn_t)(const float *, size_t, size_t, size_t, size_t, float *, size_t, fastfilters_kernel_t,
                              fastfilters_border_treatment_t, fastfilters_border_treatment_t, const float *,
                              const float *, size_t);

struct _fastfilters_kernel_fir_t {
    size_t len;
    bool is_symmetric;
    float *coefs;

    impl_fn_t fn_inner_mirror;
    impl_fn_t fn_inner_ptr;
    impl_fn_t fn_inner_optimistic;

    impl_fn_t fn_outer_mirror;
    impl_fn_t fn_outer_ptr;
    impl_fn_t fn_outer_optimistic;
};

struct _fastfilters_kernel_t {
    fastfilters_kernel_fir_t fir;
    fastfilters_kernel_iir_t iir;

    convolve_fn_t convolve_outer;
    convolve_fn_t convolve_inner;
};

void DLL_LOCAL fastfilters_cpu_init(void);
void DLL_LOCAL fastfilters_linalg_init(void);

void DLL_LOCAL fastfilters_memory_init(fastfilters_alloc_fn_t alloc_fn, fastfilters_free_fn_t free_fn);

void DLL_LOCAL *fastfilters_memory_alloc(size_t size);
void DLL_LOCAL fastfilters_memory_free(void *ptr);

void DLL_LOCAL *fastfilters_memory_align(size_t alignment, size_t size);
void DLL_LOCAL fastfilters_memory_align_free(void *ptr);

void DLL_LOCAL fastfilters_fir_init(void);

bool DLL_LOCAL fastfilters_fir_convolve_fir_inner(const float *inptr, size_t n_pixels, size_t pixel_stride,
                                                  size_t n_outer, size_t outer_stride, float *outptr,
                                                  size_t outptr_stride, fastfilters_kernel_t kernel,
                                                  fastfilters_border_treatment_t left_border,
                                                  fastfilters_border_treatment_t right_border,
                                                  const float *borderptr_left, const float *borderptr_right,
                                                  size_t border_outer_stride);
bool DLL_LOCAL fastfilters_fir_convolve_fir_outer(const float *inptr, size_t n_pixels, size_t pixel_stride,
                                                  size_t n_outer, size_t outer_stride, float *outptr,
                                                  size_t outptr_stride, fastfilters_kernel_t kernel,
                                                  fastfilters_border_treatment_t left_border,
                                                  fastfilters_border_treatment_t right_border,
                                                  const float *borderptr_left, const float *borderptr_right,
                                                  size_t border_outer_stride);

bool DLL_LOCAL fastfilters_fir_convolve_fir_inner_avx(const float *inptr, size_t n_pixels, size_t pixel_stride,
                                                      size_t n_outer, size_t outer_stride, float *outptr,
                                                      size_t outptr_stride, fastfilters_kernel_t kernel,
                                                      fastfilters_border_treatment_t left_border,
                                                      fastfilters_border_treatment_t right_border,
                                                      const float *borderptr_left, const float *borderptr_right,
                                                      size_t border_outer_stride);
bool DLL_LOCAL fastfilters_fir_convolve_fir_outer_avx(const float *inptr, size_t n_pixels, size_t pixel_stride,
                                                      size_t n_outer, size_t outer_stride, float *outptr,
                                                      size_t outptr_stride, fastfilters_kernel_t kernel,
                                                      fastfilters_border_treatment_t left_border,
                                                      fastfilters_border_treatment_t right_border,
                                                      const float *borderptr_left, const float *borderptr_right,
                                                      size_t border_outer_stride);
bool DLL_LOCAL fastfilters_fir_convolve_fir_inner_avxfma(const float *inptr, size_t n_pixels, size_t pixel_stride,
                                                         size_t n_outer, size_t outer_stride, float *outptr,
                                                         size_t outptr_stride, fastfilters_kernel_t kernel,
                                                         fastfilters_border_treatment_t left_border,
                                                         fastfilters_border_treatment_t right_border,
                                                         const float *borderptr_left, const float *borderptr_right,
                                                         size_t border_outer_stride);
bool DLL_LOCAL fastfilters_fir_convolve_fir_outer_avxfma(const float *inptr, size_t n_pixels, size_t pixel_stride,
                                                         size_t n_outer, size_t outer_stride, float *outptr,
                                                         size_t outptr_stride, fastfilters_kernel_t kernel,
                                                         fastfilters_border_treatment_t left_border,
                                                         fastfilters_border_treatment_t right_border,
                                                         const float *borderptr_left, const float *borderptr_right,
                                                         size_t border_outer_stride);

static inline double opt_window_ratio(const fastfilters_options_t *options)
{
    if (!options)
        return 0.0;
    return options->window_ratio;
}

#ifdef __cplusplus
}
#endif

#endif
