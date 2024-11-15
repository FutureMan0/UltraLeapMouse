//
// Created by Luis Blüml on 14.11.2024.
//

#include <stdio.h>
#include <stdlib.h>
#include "LeapC.h"

static LEAP_CONNECTION connectionHandle;

/** Callback for when the connection opens. */
static void OnConnect(void) {
    printf("Connected.\n");
}

/** Callback for when a device is found. */
static void OnDevice(const LEAP_DEVICE_EVENT *device_event) {
    LEAP_DEVICE device;
    if (LeapOpenDevice(device_event->device, &device) == eLeapRS_Success) {
        LEAP_DEVICE_INFO device_info = { sizeof(device_info) };
        device_info.serial = (char*)malloc(device_info.serial_length);
        LeapGetDeviceInfo(device, &device_info);

        printf("Found device %s.\n", device_info.serial);

        free(device_info.serial);
        LeapCloseDevice(device);
    } else {
        printf("Failed to open device.\n");
    }
}

/** Callback for when a frame of tracking data is available. */
static void OnFrame(const LEAP_TRACKING_EVENT *frame) {
    if (frame->info.frame_id % 60 == 0)
        printf("Frame %lli with %i hands.\n", (long long int)frame->info.frame_id, frame->nHands);

    for (uint32_t h = 0; h < frame->nHands; h++) {
        LEAP_HAND* hand = &frame->pHands[h];
        printf("    Hand id %i is a %s hand with position (%f, %f, %f).\n",
            hand->id,
            (hand->type == eLeapHandType_Left ? "left" : "right"),
            hand->palm.position.x,
            hand->palm.position.y,
            hand->palm.position.z);

        // Loop through the fingers

    }
}




static void* allocate(uint32_t size, eLeapAllocatorType typeHint, void* state) {
    return malloc(size);
}

static void deallocate(void* ptr, void* state) {
    if (ptr) free(ptr);
}

int main(int argc, char** argv) {
    LEAP_CONNECTION_CONFIG config = { sizeof(config), 0 };
    LeapCreateConnection(&config, &connectionHandle);
    LeapOpenConnection(connectionHandle);

    // Set up allocator
    LEAP_ALLOCATOR allocator = { allocate, deallocate, NULL };
    LeapSetAllocator(connectionHandle, &allocator);

    // Set policies
    LeapSetPolicyFlags(connectionHandle, eLeapPolicyFlag_Images | eLeapPolicyFlag_MapPoints, 0);

    // Event loop for polling connection
    LEAP_CONNECTION_MESSAGE msg;
    eLeapRS result;
    printf("Running... Press Ctrl+C to exit.\n");

    while (1) {
        // Poll for events with a 100 ms timeout
        result = LeapPollConnection(connectionHandle, 100, &msg);

        if (result != eLeapRS_Success) {
            fprintf(stderr, "LeapPollConnection error: %d\n", result);
            continue;
        }

        // Process the event
        switch (msg.type) {
            case eLeapEventType_Connection:
                OnConnect();
                break;
            case eLeapEventType_Device:
                OnDevice(msg.device_event);
                break;
            case eLeapEventType_Tracking:
                OnFrame(msg.tracking_event);
                break;
            default:
                break;
        }
    }

    // Close connection (will only be reached if you add exit logic)
    LeapCloseConnection(connectionHandle);
    LeapDestroyConnection(connectionHandle);

    return 0;
}
