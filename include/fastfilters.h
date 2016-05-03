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

#ifndef FASTFILTERS_H
#define FASTFILTERS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

// https://gcc.gnu.org/wiki/Visibility
#if defined _WIN32 || defined __CYGWIN__
#ifdef FASTFILTERS_SHARED_LIBRARY
#ifdef __GNUC__
#define DLL_PUBLIC __attribute__((dllexport))
#else
#define DLL_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
#endif
#else
#ifdef __GNUC__
#define DLL_PUBLIC __attribute__((dllimport))
#else
#define DLL_PUBLIC __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
#endif
#endif
#define DLL_LOCAL
#else
#if __GNUC__ >= 4
#define DLL_PUBLIC __attribute__((visibility("default")))
#define DLL_LOCAL __attribute__((visibility("hidden")))
#else
#define DLL_PUBLIC
#define DLL_LOCAL
#endif
#endif

typedef struct _fastfilters_kernel_fir_t *fastfilters_kernel_fir_t;
typedef struct _fastfilters_kernel_iir_t *fastfilters_kernel_iir_t;
typedef struct _fastfilters_kernel_t *fastfilters_kernel_t;

typedef enum { FASTFILTERS_CPU_AVX, FASTFILTERS_CPU_FMA, FASTFILTERS_CPU_AVX2 } fastfilters_cpu_feature_t;

typedef struct _fastfilters_array2d_t {
    float *ptr;
    size_t n_x;
    size_t n_y;
    size_t stride_x;
    size_t stride_y;
    size_t n_channels;
} fastfilters_array2d_t;

typedef struct _fastfilters_array3d_t {
    float *ptr;
    size_t n_x;
    size_t n_y;
    size_t n_z;
    size_t stride_x;
    size_t stride_y;
    size_t stride_z;
    size_t n_channels;
} fastfilters_array3d_t;

typedef struct _fastfilters_options_t {
    float window_ratio;
} fastfilters_options_t;

typedef void *(*fastfilters_alloc_fn_t)(size_t size);
typedef void (*fastfilters_free_fn_t)(void *);

void DLL_PUBLIC fastfilters_init(void);
void DLL_PUBLIC fastfilters_init_ex(fastfilters_alloc_fn_t alloc_fn, fastfilters_free_fn_t free_fn);

bool DLL_PUBLIC fastfilters_cpu_check(fastfilters_cpu_feature_t feature);
bool DLL_PUBLIC fastfilters_cpu_enable(fastfilters_cpu_feature_t feature, bool enable);

fastfilters_kernel_t DLL_PUBLIC fastfilters_kernel_fir_gaussian(unsigned int order, double sigma, float window_ratio);
fastfilters_kernel_t DLL_PUBLIC fastfilters_kernel_iir_gaussian(unsigned int order, double sigma, float window_ratio);
unsigned int DLL_PUBLIC fastfilters_kernel_fir_get_length(fastfilters_kernel_t kernel);
void DLL_PUBLIC fastfilters_kernel_free(fastfilters_kernel_t kernel);

bool DLL_PUBLIC fastfilters_convolve2d(const fastfilters_array2d_t *inarray, const fastfilters_kernel_t kernelx,
                                       const fastfilters_kernel_t kernely, const fastfilters_array2d_t *outarray,
                                       const fastfilters_options_t *options);
bool DLL_PUBLIC fastfilters_convolve3d(const fastfilters_array3d_t *inarray, const fastfilters_kernel_t kernelx,
                                       const fastfilters_kernel_t kernely, const fastfilters_kernel_t kernelz,
                                       const fastfilters_array3d_t *outarray, const fastfilters_options_t *options);

void DLL_PUBLIC fastfilters_linalg_ev2d(const float *xx, const float *xy, const float *yy, float *ev_small,
                                        float *ev_big, const size_t len);
void DLL_PUBLIC fastfilters_linalg_ev3d(const float *a00, const float *a01, const float *a02, const float *a11,
                                        const float *a12, const float *a22, float *ev0, float *ev1, float *ev2,
                                        const size_t len);

void DLL_PUBLIC fastfilters_combine_add2d(const fastfilters_array2d_t *a, const fastfilters_array2d_t *b,
                                          fastfilters_array2d_t *out);
void DLL_PUBLIC fastfilters_combine_add3d(const fastfilters_array3d_t *a, const fastfilters_array3d_t *b,
                                          const fastfilters_array3d_t *c, fastfilters_array3d_t *out);

void DLL_PUBLIC fastfilters_combine_addsqrt2d(const fastfilters_array2d_t *a, const fastfilters_array2d_t *b,
                                              fastfilters_array2d_t *out);
void DLL_PUBLIC fastfilters_combine_addsqrt3d(const fastfilters_array3d_t *a, const fastfilters_array3d_t *b,
                                              const fastfilters_array3d_t *c, fastfilters_array3d_t *out);

void DLL_PUBLIC fastfilters_combine_mul2d(const fastfilters_array2d_t *a, const fastfilters_array2d_t *b,
                                          fastfilters_array2d_t *out);
void DLL_PUBLIC fastfilters_combine_mul3d(const fastfilters_array3d_t *a, const fastfilters_array3d_t *b,
                                          fastfilters_array3d_t *out);

DLL_PUBLIC fastfilters_array2d_t *fastfilters_array2d_alloc(size_t n_x, size_t n_y, size_t channels);
DLL_PUBLIC void fastfilters_array2d_free(fastfilters_array2d_t *v);

DLL_PUBLIC fastfilters_array3d_t *fastfilters_array3d_alloc(size_t n_x, size_t n_y, size_t n_z, size_t channels);
DLL_PUBLIC void fastfilters_array3d_free(fastfilters_array3d_t *v);

bool DLL_PUBLIC fastfilters_fir_gaussian2d(const fastfilters_array2d_t *inarray, unsigned order, double sigma,
                                           fastfilters_array2d_t *outarray, const fastfilters_options_t *options);
bool DLL_PUBLIC fastfilters_fir_gaussian3d(const fastfilters_array3d_t *inarray, unsigned order, double sigma,
                                           fastfilters_array3d_t *outarray, const fastfilters_options_t *options);

bool DLL_PUBLIC fastfilters_fir_hog2d(const fastfilters_array2d_t *inarray, double sigma, fastfilters_array2d_t *out_xx,
                                      fastfilters_array2d_t *out_xy, fastfilters_array2d_t *out_yy,
                                      const fastfilters_options_t *options);
DLL_PUBLIC bool fastfilters_fir_hog3d(const fastfilters_array3d_t *inarray, double sigma, fastfilters_array3d_t *out_xx,
                                      fastfilters_array3d_t *out_yy, fastfilters_array3d_t *out_zz,
                                      fastfilters_array3d_t *out_xy, fastfilters_array3d_t *out_xz,
                                      fastfilters_array3d_t *out_yz, const fastfilters_options_t *options);

bool DLL_PUBLIC fastfilters_fir_gradmag2d(const fastfilters_array2d_t *inarray, double sigma,
                                          fastfilters_array2d_t *outarray, const fastfilters_options_t *options);
bool DLL_PUBLIC fastfilters_fir_gradmag3d(const fastfilters_array3d_t *inarray, double sigma,
                                          fastfilters_array3d_t *outarray, const fastfilters_options_t *options);

bool DLL_PUBLIC fastfilters_fir_laplacian2d(const fastfilters_array2d_t *inarray, double sigma,
                                            fastfilters_array2d_t *outarray, const fastfilters_options_t *options);
bool DLL_PUBLIC fastfilters_fir_laplacian3d(const fastfilters_array3d_t *inarray, double sigma,
                                            fastfilters_array3d_t *outarray, const fastfilters_options_t *options);

bool DLL_PUBLIC fastfilters_fir_structure_tensor2d(const fastfilters_array2d_t *inarray, double sigma_outer,
                                                   double sigma_inner, fastfilters_array2d_t *out_xx,
                                                   fastfilters_array2d_t *out_xy, fastfilters_array2d_t *out_yy,
                                                   const fastfilters_options_t *options);
bool DLL_PUBLIC fastfilters_fir_structure_tensor3d(const fastfilters_array3d_t *inarray, double sigma_outer,
                                                   double sigma_inner, fastfilters_array3d_t *out_xx,
                                                   fastfilters_array3d_t *out_yy, fastfilters_array3d_t *out_zz,
                                                   fastfilters_array3d_t *out_xy, fastfilters_array3d_t *out_xz,
                                                   fastfilters_array3d_t *out_yz, const fastfilters_options_t *options);
#ifdef __cplusplus
}
#endif

#endif