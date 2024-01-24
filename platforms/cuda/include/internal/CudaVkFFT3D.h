#ifndef __OPENMM_CUDAVKFFT3D_H__
#define __OPENMM_CUDAVKFFT3D_H__

/* -------------------------------------------------------------------------- *
 *                          OpenMM Custom CPP Forces                          *
 *                          ========================                          *
 *                                                                            *
 *  A plugin for distributing OpenMM CustomCPPForce instances                 *
 *                                                                            *
 *  Copyright (c) 2024 Charlles Abreu                                         *
 *  https://github.com/craabreu/customcppforces                               *
 * -------------------------------------------------------------------------- */

#include "internal/CudaFFT3D.h"
#include "openmm/cuda/CudaArray.h"
#define VKFFT_BACKEND 1 // CUDA
#include "vkFFT.h"

using namespace OpenMM;

namespace CustomCPPForces {

/**
 * This class performs three dimensional Fast Fourier Transforms using VkFFT by
 * Dmitrii Tolmachev (https://github.com/DTolm/VkFFT).
 *
 * Note that this class performs an unnormalized transform.  That means that if you perform
 * a forward transform followed immediately by an inverse transform, the effect is to
 * multiply every value of the original data set by the total number of data points.
 */

class CudaVkFFT3D : public CudaFFT3D {
public:
    /**
     * Create an CudaVkFFT3D object for performing transforms of a particular size.
     *
     * The transform cannot be done in-place: the input and output
     * arrays must be different.  Also, the input array is used as workspace, so its contents
     * are destroyed.  This also means that both arrays must be large enough to hold complex values,
     * even when performing a real-to-complex transform.
     *
     * When performing a real-to-complex transform, the output data is of size xsize*ysize*(zsize/2+1)
     * and contains only the non-redundant elements.
     *
     * @param context the context in which to perform calculations
     * @param stream  the CUDA stream doing the calculations
     * @param xsize   the first dimension of the data sets on which FFTs will be performed
     * @param ysize   the second dimension of the data sets on which FFTs will be performed
     * @param zsize   the third dimension of the data sets on which FFTs will be performed
     * @param batch   the number of FFTs
     * @param realToComplex  if true, a real-to-complex transform will be done.  Otherwise, it is complex-to-complex.
     * @param in      the data to transform, ordered such that in[x*ysize*zsize + y*zsize + z] contains element (x, y, z)
     * @param out     on exit, this contains the transformed data
     */
    CudaVkFFT3D(CudaContext& context, CUstream& stream, int xsize, int ysize, int zsize, int batch, bool realToComplex, CudaArray& in, CudaArray& out);
    ~CudaVkFFT3D();
    /**
     * Perform a Fourier transform.
     *
     * @param forward  true to perform a forward transform, false to perform an inverse transform
     */
    void execFFT(bool forward);
private:
    int device;
    uint64_t inputBufferSize;
    uint64_t outputBufferSize;
    VkFFTApplication* app;
};

} // namespace CustomCPPForces

#endif // __OPENMM_CUDAVKFFT3D_H__
