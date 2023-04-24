#include <stdio.h>
#include <nvml.h>

int main(void)
{
    nvmlReturn_t result;
    unsigned int device_count, i;
    int cudaDriverVersion;
    char version[NVML_SYSTEM_DRIVER_VERSION_BUFFER_SIZE];

    // First initialize NVML library
    result = nvmlInit();
    if (NVML_SUCCESS != result)
    { 
        printf("Failed to initialize NVML: %s\n", nvmlErrorString(result));

        printf("Press ENTER to continue...\n");
        getchar();
        return 1;
    }

    // get driver version
    result = nvmlSystemGetDriverVersion(version, NVML_SYSTEM_DRIVER_VERSION_BUFFER_SIZE);
    if (NVML_SUCCESS != result)
    {
        printf("Failed to query system driver version: %s\n", nvmlErrorString(result));
        goto Error;
    }
    printf("nvmlSystemGetDriverVersion: %s\n", version);

    // get nvml version
    result = nvmlSystemGetNVMLVersion(version, NVML_SYSTEM_DRIVER_VERSION_BUFFER_SIZE);
    if (NVML_SUCCESS != result)
    {
        printf("Failed to query system nvml version: %s\n", nvmlErrorString(result));
        goto Error;
    }
    printf("nvmlSystemGetNVMLVersion: %s\n", version);

    // cuda driver version
    result = nvmlSystemGetCudaDriverVersion(&cudaDriverVersion);
    if (NVML_SUCCESS != result)
    {
        printf("Failed to query system cuda driver version: %s\n", nvmlErrorString(result));
        goto Error;
    }
    printf("nvmlSystemGetCudaDriverVersion: %d.%d\n",
        NVML_CUDA_DRIVER_VERSION_MAJOR(cudaDriverVersion),
        NVML_CUDA_DRIVER_VERSION_MINOR(cudaDriverVersion));

    result = nvmlDeviceGetCount(&device_count);
    if (NVML_SUCCESS != result)
    { 
        printf("Failed to query device count: %s\n", nvmlErrorString(result));
        goto Error;
    }
    printf("Found %u device%s\n\n", device_count, device_count != 1 ? "s" : "");

    printf("Listing devices:\n");    
    for (i = 0; i < device_count; i++)
    {
        nvmlDevice_t device;
        char name[NVML_DEVICE_NAME_BUFFER_SIZE];
        nvmlPciInfo_t pci;
        nvmlUtilization_t utilization;
        nvmlMemory_t memory;
        unsigned int power, limit;
        unsigned int temperature;

        // Query for device handle to perform operations on a device
        // You can also query device handle by other features like:
        // nvmlDeviceGetHandleBySerial
        // nvmlDeviceGetHandleByPciBusId
        result = nvmlDeviceGetHandleByIndex(i, &device);
        if (NVML_SUCCESS != result)
        { 
            printf("Failed to get handle for device %u: %s\n", i, nvmlErrorString(result));
            goto Error;
        }

        result = nvmlDeviceGetName(device, name, NVML_DEVICE_NAME_BUFFER_SIZE);
        if (NVML_SUCCESS != result)
        { 
            printf("Failed to get name of device %u: %s\n", i, nvmlErrorString(result));
            goto Error;
        }
        
        // pci.busId is very useful to know which device physically you're talking to
        // Using PCI identifier you can also match nvmlDevice handle to CUDA device.
        result = nvmlDeviceGetPciInfo(device, &pci);
        if (NVML_SUCCESS != result)
        { 
            printf("Failed to get pci info for device %u: %s\n", i, nvmlErrorString(result));
            goto Error;
        }

        printf("%u. %s [%s]\n", i, name, pci.busId);

        result = nvmlDeviceGetMemoryInfo(device, &memory);
        if (NVML_SUCCESS != result)
        {
            printf("Failed to get memory info of device %u: %s\n", i, nvmlErrorString(result));
            goto Error;
        }
        printf("memory total %llu, free %llu, used %llu\n", memory.total, memory.free, memory.used);

        result = nvmlDeviceGetUtilizationRates(device, &utilization);
        if (NVML_SUCCESS != result)
        {
            printf("Failed to get utilization rates of device %u: %s\n", i, nvmlErrorString(result));
            goto Error;
        }
        printf("UtilizationRates gpu %u, memory %u\n", utilization.gpu, utilization.memory);

        result = nvmlDeviceGetPowerUsage(device, &power);
        if (NVML_SUCCESS != result)
        {
            printf("Failed to get power usage of device %u: %s\n", i, nvmlErrorString(result));
            goto Error;
        }
        
        result = nvmlDeviceGetPowerManagementLimit(device, &limit);
        if (NVML_SUCCESS != result)
        {
            printf("Failed to get power management limit of device %u: %s\n", i, nvmlErrorString(result));
            goto Error;
        }
        printf("Power usage %u, limit %u\n", power, limit);

        result = nvmlDeviceGetTemperature(device, NVML_TEMPERATURE_GPU, &temperature);
        if (NVML_SUCCESS != result)
        {
            printf("Failed to get power temperature of device %u: %s\n", i, nvmlErrorString(result));
            goto Error;
        }
        printf("Temperature %u C\n", temperature);
    }

    result = nvmlShutdown();
    if (NVML_SUCCESS != result)
        printf("Failed to shutdown NVML: %s\n", nvmlErrorString(result));

    printf("All done.\n");

    printf("Press ENTER to continue...\n");
    getchar();
    return 0;

Error:
    result = nvmlShutdown();
    if (NVML_SUCCESS != result)
        printf("Failed to shutdown NVML: %s\n", nvmlErrorString(result));

    printf("Press ENTER to continue...\n");
    getchar();
    return 1;
}
