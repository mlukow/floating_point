#include <stdio.h>
#include <stdlib.h>

#include <OpenCL/opencl.h>

const char *KernelSource =
  "__kernel void iter(__global long double *input, const size_t count) {                     \n" \
  "  for (size_t i = 2; i < count; i++) {                                                    \n" \
  "    input[i] = 111.0L - 1130.0L / input[i - 1] + 3000.0L / (input[i - 2] * input[i - 1]); \n" \
  "  }                                                                                       \n" \
  "}";

int main(void) {
  int err;
  cl_device_id device_id;
  clGetDeviceIDs(NULL, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);

  cl_context context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
  if (err != CL_SUCCESS) {
    printf("failed to create context\n");
    return err;
  }

  cl_command_queue commands = clCreateCommandQueue(context, device_id, 0, &err);
  if (err != CL_SUCCESS) {
    printf("failed to create command queue\n");
    return err;
  }
  
  cl_program program = clCreateProgramWithSource(context, 1, &KernelSource, NULL, &err);
  if (err != CL_SUCCESS) {
    printf("failed to create program\n");
    return err;
  }

  err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
  if (err != CL_SUCCESS) {
    printf("failed to build program\n");
    return err;
  }

  cl_kernel kernel = clCreateKernel(program, "iter", &err);
  if (err != CL_SUCCESS) {
    printf("failed to create kernel\n");
    return err;
  }

  size_t count = 32;

  cl_mem input = clCreateBuffer(context,  CL_MEM_READ_ONLY,  count * sizeof(long double), NULL, NULL);
  long double data[count];

  data[0] = 2.0L;
  data[1] = -4.0L;
  err = clEnqueueWriteBuffer(commands, input, CL_TRUE, 0, count * sizeof(long double), data, 0, NULL, NULL);
  if (err != CL_SUCCESS) {
    printf("failed to enqueue write buffer\n");
    return err;
  }

  clSetKernelArg(kernel, 0, sizeof(cl_mem), &input);
  clSetKernelArg(kernel, 1, sizeof(size_t), &count);

  err = clEnqueueTask(commands, kernel, 0, NULL, NULL);
  if (err != CL_SUCCESS) {
    printf("failed to enqueue task\n");
    return err;
  }

  clFinish(commands);

  long double results[count];
  clEnqueueReadBuffer(commands, input, CL_TRUE, 0, count * sizeof(long double), results, 0, NULL, NULL);

  for (size_t i = 0; i < count; i++) {
    printf("u[%2zu] = %-16.12Le\n", i, results[i]);
  }
  
  clReleaseMemObject(input);
  clReleaseProgram(program);
  clReleaseKernel(kernel);
  clReleaseCommandQueue(commands);
  clReleaseContext(context);

  return EXIT_SUCCESS;
}
