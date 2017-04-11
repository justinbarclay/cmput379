#include "memory.h"

int addToMemory(int pageNum, int pid, int POLICY, doubleLL* tlb, doubleLL* pageTable, node* frameBuffer[], doubleLL* virtualMemory){
    int frame;
    node* item;
    int isValid;
    // TLB Hit occurs here
// Page fault is if nodeExists & if node is valid
        // Add a valid boolean to nodeExists  so we can verify

    if((frame = nodeExists(pageNum, pid, tlb, &isValid))>0 && POLICY){
        printf("TLB Collision\n");
        item = frameBuffer[frame];

        policyLRU(item, tlb);
        policyLRU(item, virtualMemory);
        return 0;
    } else if((frame = nodeExists(pageNum, pid, pageTable, &isValid)) > 0 && POLICY){
        printf("Page table Collision\n");
        item = frameBuffer[frame];
        policyLRU(item, virtualMemory);
        return 0;
    } else if((frame = nodeExists(pageNum, pid, pageTable, &isValid)) > 0 && !POLICY) {
        return 0;

    } else {
        int frame = addToVirtualMemory(pageNum, pid, frameBuffer, virtualMemory);
        invalidateFrame(frame, tlb);
        invalidateFrame(frame, pageTable);
        // Page eviction occurs here, do number coding scheme for page evicted instead?
        // just need to know what tracefile it belongs to. Will be returning a number from -1 to the file ID
        addNewNode(pageNum, pid, frame, pageTable);
        addNewNode(pageNum, pid, frame, tlb);
        return 1;
    }
}

int addToVirtualMemory(int pageNum,int pid, node* frameBuffer[], doubleLL* virtualMemory){
    // Function makes the assumption that frameBuffer
    int frame = 0;
    if(virtualMemory->currentSize < virtualMemory->maxSize){
        frame = virtualMemory->currentSize; //Grab currentFrame number and increase by one;
        addNewNode(pageNum,pid, frame, virtualMemory);

        // Grab the newest node
        node* currentAddress = virtualMemory->head->next;
        //Store it in our frame buffer
        frameBuffer[frame] = currentAddress;
    } else {
        // If our framebuffer is full
        // grab the frame of the node at the tail
        frame = virtualMemory->tail->previous->frame;
        // and reuse it
        addNewNode(pageNum, pid, frame, virtualMemory);
        // grab current node address and add it to memory
        node* currentAddress = virtualMemory->head->next;
        frameBuffer[frame] = currentAddress;
    }
    return frame;
}

void invalidateFrame(int frame, doubleLL* container){
    node* current = container->head->next;
    int index = 0;
    while(current != NULL){
        index++;
        if(current->frame == frame){
            current->validity = 0;
            return;
        }
        current = current->next;
    }
    return;
}
