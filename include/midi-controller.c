#include <CoreMIDI/CoreMIDI.h>
#include <Security/Security.h>

//should be callable in a loop constantly calling listAndConnectMIDIDevices

MIDIReceiveBlock receiveBlock = ^(const MIDIEventList *evtlist, void *srcConnRefCon) {
    const MIDIEventPacket *packet = &evtlist->packet[0];
    for (unsigned int i = 0; i < evtlist->numPackets; ++i) {
        for (unsigned int j = 0; j < packet->wordCount; ++j) {
            uint32_t word = packet->words[j];
            
            // The MIDI message is packed into the lower 24 bits of the word
            uint8_t status = (word >> 16) & 0xFF;
            uint8_t data1 = (word >> 8) & 0xFF;
            uint8_t data2 = word & 0xFF;
            
            // Interpret MIDI message
            uint8_t messageType = status & 0xF0;
            uint8_t channel = status & 0x0F;
            
            switch (messageType) {
                case 0x90: // Note On
                    if (data2 == 0) { // Note On with velocity 0 is treated as Note Off
                        printf("Note Off - Channel: %d, Note: %d\n", 
                               channel + 1, data1);
                    } else {
                        printf("Note On - Channel: %d, Note: %d, Velocity: %d\n", 
                               channel + 1, data1, data2);

                        double freq = 27.5 * pow(2, ((double)data1/12));
                        // double freq = 293.664;
                    }
                    break;
                case 0x80: // Note Off
                    printf("Note Off - Channel: %d, Note: %d, Velocity: %d\n", 
                           channel + 1, data1, data2);

                    break;
                case 0xB0: // Control Change
                    printf("Control Change - Channel: %d, Controller: %d, Value: %d\n", 
                           channel + 1, data1, data2);
                    break;
                case 0xE0: // Pitch Bend
                    printf("Pitch Bend - Channel: %d, Value: %d\n", 
                           channel + 1, (data2 << 7) | data1);
                    break;
                case 0xD0: // Channel Pressure
                    printf("Channel Pressure - Channel: %d, Value: %d\n", 
                           channel + 1, data1);
                    break;
                case 0xA0: // Polyphonic Key Pressure
                    printf("Poly Pressure - Channel: %d, Note: %d, Value: %d\n", 
                           channel + 1, data1, data2);
                    break;
                case 0xC0: // Program Change
                    printf("Program Change - Channel: %d, Program: %d\n", 
                           channel + 1, data1);
                    break;
                default:
                    printf("Unknown Message: Status: 0x%02X, Data1: 0x%02X, Data2: 0x%02X\n", 
                           status, data1, data2);
            }
        }
        packet = MIDIEventPacketNext(packet);
    }
};

char * CFStringCopyUTF8String(CFStringRef aString) {
    if (aString == NULL) {
        return NULL;
    }
    CFIndex length = CFStringGetLength(aString);
    CFIndex maxSize = CFStringGetMaximumSizeForEncoding(length, kCFStringEncodingUTF8) + 1;
    char *buffer = (char *)malloc(maxSize);
    if (CFStringGetCString(aString, buffer, maxSize, kCFStringEncodingUTF8)) {
        return buffer;
    }
    free(buffer);
    return NULL;
}

void listAndConnectMIDIDevices() {
    MIDIClientRef client;
    OSStatus result = MIDIClientCreate(CFSTR("MIDI Device Lister"), NULL, NULL, &client);
    if (result != noErr) {
        printf("Error creating MIDI client\n");
        return;
    }

    ItemCount numSources = MIDIGetNumberOfSources();
    printf("Found %d MIDI Input sources:\n", (int)numSources);

    if (numSources == 0) {
        printf("No MIDI devices found!\n");
        return;
    }

    // Create input port first
    MIDIPortRef inPort;
    result = MIDIInputPortCreateWithProtocol(client, 
                                           CFSTR("Input Port"), 
                                           kMIDIProtocol_1_0, 
                                           &inPort, 
                                           receiveBlock);
    if (result != noErr) {
        printf("Error creating input port\n");
        CFStringRef errMessage = SecCopyErrorMessageString(result, NULL);
        char *errBuffer = CFStringCopyUTF8String(errMessage);
        printf("%s\n", errBuffer);
        free(errBuffer);
        CFRelease(errMessage);
        return;
    }

    // Connect to all available sources
    for (ItemCount i = 0; i < numSources; i++) {
        MIDIEndpointRef source = MIDIGetSource(i);
        CFStringRef name = NULL;
        result = MIDIObjectGetStringProperty(source, kMIDIPropertyName, &name);
        
        if (result == noErr && name != NULL) {
            char *nameBuffer = CFStringCopyUTF8String(name);
            printf("Connecting to device: %s\n", nameBuffer);
            
            result = MIDIPortConnectSource(inPort, source, NULL);
            if (result != noErr) {
                printf("Error connecting to source:\n");
                CFStringRef err = SecCopyErrorMessageString(result, NULL);
                char *errChr = CFStringCopyUTF8String(err);
                printf("%s\n", errChr);
                free(errChr);
                CFRelease(err);
            } else {
                printf("Successfully connected to %s\n", nameBuffer);
            }
            
            free(nameBuffer);
            CFRelease(name);
        }
    }
}
