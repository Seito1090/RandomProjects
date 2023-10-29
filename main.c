#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#define PRINT_BINARY(n)   for(int i = sizeof(n)*8 - 1; i >= 0; i--) printf("%d", (n >> i) & 1)

uint8_t MY_HEAP[64000];

/* void print_heap_status()
 *
 * This function prints the status of the heap, it prints the size of each bloc and if it is free or not
 *
 * */
void print_heap_status() {
    printf("\n# -------------------------------------------------- Status -------------------------------------------------- #\n\n");
    uint8_t* current = MY_HEAP + 4; // Start from the first block after the constants

    int allocated_count = 0;
    int free_count = 0;
    size_t allocated_bytes = 0;
    size_t free_bytes = 0;

    uint16_t allocated_blocs = 0;
    uint16_t allocated_bloc_bytes = 0;
    uint16_t free_blocs = 0;
    uint16_t free_bloc_bytes = 0;

    while (current < MY_HEAP + 64000 - 4) {
        // Extract the size and free status from the header
        uint16_t header = (current[0] << 8) | current[1];
        size_t size = header & ~0x1; // Clear the last bit
        int is_allocated = header & 0x1;

        if (is_allocated) {
            if (free_count > 0) {
                printf("(free; %d section%s; %zu bytes), ", free_count, free_count > 1 ? "s" : "", free_bytes);
                free_blocs += free_count;
                free_bloc_bytes += free_bytes;
                free_count = 0;
                free_bytes = 0;
            }
            allocated_count++;
            allocated_bytes += size;
        } else {
            if (allocated_count > 0) {
                printf("(allocated; %d section%s; %zu bytes), ", allocated_count, allocated_count > 1 ? "s" : "", allocated_bytes);
                allocated_blocs += allocated_count;
                allocated_bloc_bytes += allocated_bytes;
                allocated_count = 0;
                allocated_bytes = 0;
            }
            free_count++;
            free_bytes += size;
        }

        current += size; // Skip to the next block (including the 2-byte header)
    }

    // Last block print
    if (allocated_count > 0) {
        printf("(allocated; %d section%s; %zu bytes), ", allocated_count, allocated_count > 1 ? "s" : "", allocated_bytes);
        allocated_blocs += allocated_count;
        allocated_bloc_bytes += allocated_bytes;
    } else if (free_count > 0) {
        printf("(free; %d section%s; %zu bytes), ", free_count, free_count > 1 ? "s" : "", free_bytes);
        free_blocs += free_count;
        free_bloc_bytes += free_bytes;
    }

    printf("\n\n# -------------------------------------------------- Summary ------------------------------------------------- #\n");
    printf("\nTotal: (allocated; %d sections, %hu bytes), (free; %d sections, %hu bytes)\n", allocated_blocs, allocated_bloc_bytes, free_blocs, free_bloc_bytes);
    printf("\n");
}

void print_test(){
    //Get the current header value starting from the 5th byte
    uint8_t *current_header_address= &MY_HEAP[4];

    //Initialize the free and allocated counts for segments and bytes
    int allocated_count = 0;
    int free_count = 0;
    size_t allocated_bytes = 0;
    size_t free_bytes = 0;

    printf("Last address : %p\n", (void*)&MY_HEAP[64000]);
    int count = 0;
    while(current_header_address <= &MY_HEAP[64000] && count < 5){
        count++;
        //Check the last bit to know if the bloc is free or not
        printf("Current header address : %p\n", (void*)current_header_address);
        printf("Current header value : %d\n", *current_header_address << 8 | *(current_header_address+1));
        if (((*current_header_address << 8 | *(current_header_address+1)) & 0x1) == 1){
            //Allocated
            allocated_count++;
            allocated_bytes += (*current_header_address << 8 | *(current_header_address+1)) & ~0x1;
        } else {
            //Free
            free_count++;
            free_bytes += (*current_header_address << 8 | *(current_header_address+1)) & ~0x1;
        }
        printf("Before jump : %p\nJumping %d\n", (void*)current_header_address, (*current_header_address << 8 | *(current_header_address+1)) & ~0x1);
        //Go to the next header
        current_header_address += ((*current_header_address << 8 | *(current_header_address+1)) & ~0x1);
        printf("After jump : %p\n--------------------------\n", (void*)current_header_address);
        if (current_header_address >= &MY_HEAP[64000]){
            return;
        }
    }
}
/* void init()
 *
 * this function initializes the heap, the first 2 bytes are the biggest memory space available at once (in bytes) and the 2 next bytes are the number of mallocs done
 *
 * return : void
 *
 */
void init(){
    //Set everything to 0
    memset(MY_HEAP, 0, sizeof(MY_HEAP)); //Not to forget the pointer to the heap (MY_HEAP) is pointing to the available_space value, NOT THE FIRST BLOC USED FOR MALLOC

    //Get the available space (size of the heap - 4 bytes for the constants)
    uint16_t available_space = sizeof(MY_HEAP) - 4;

    //Split the available_space in 2 bytes
    uint8_t first_byte = available_space >> 8;
    uint8_t second_byte = available_space & 0xFF;

    //Add the bytes to the array
    MY_HEAP[0] = first_byte;
    MY_HEAP[1] = second_byte;

    //MY_HEAP[2] and MY_HEAP[3] are the number of mallocs done, we set them to 0
    //Since memset was used to make everything 0, and we have 0 mallocs done, we don't have to make anything changes, MY_HEAP[2] and MY_HEAP[3] are already 0

    //Creating the first header which contains its size + the size of available space stored on 2 bytes.
    //Blocs size allocated will always be even, so we can use the last bit to know if the bloc is free or not : 0 = free, 1 = used
    uint16_t first_header = available_space & ~0x1; //Available space with last bit forced to 0 to represent a "free" header

    //The first information can be stored from the 5th byte onwards
    MY_HEAP[4] = first_header >> 8;
    MY_HEAP[5] = first_header & 0xFF;
}

/* void my_free(void* pointer)
 *
 * This function frees the memory space pointed by the pointer provided
 *
 * pointer : pointer to the memory space to be freed
 * return : void
 *
 */
void my_free(void* pointer){
    //Preliminary check to return directly if the pointer is NULL
    if (pointer == NULL){
        printf("Pointer is NULL\n");
        return;
    }

    //Get the biggest size available and the number of mallocs done
    uint16_t max = MY_HEAP[0] << 8 | MY_HEAP[1]; //Biggest size available in case we have to update it, can be fetched later if needed tho so might look into moving it
    uint16_t mallocs_done = MY_HEAP[2] << 8 | MY_HEAP[3];

    //Return if the number of mallocs was 0, somthing when obviously wrong
    if (mallocs_done <= 0){
        printf("No mallocs done, WHAT ARE YOU FREEING ?\n");
        return;
    }

    //Get the header value from the pointer provided. Be aware that the header is always 2 bytes before the pointer !
    //Set the pointer to uint8_t
    uint8_t *data_ptr = (uint8_t *) (pointer);

    //Go back 2 bytes and read the bit values
    data_ptr -= 2;
    uint8_t *header_ptr0 = data_ptr;
    uint8_t *header_ptr1 = data_ptr + 1;

    //Bit manipulation to get the whole header
    uint16_t header = *header_ptr0 << 8 | *header_ptr1;
    uint8_t *header_ptr = (uint8_t *) (pointer - 2);

    printf("Header value : %d\n", header);
    printf("Header start address : %p\n", (void*)header_ptr);
    printf("Pointer value : %p\n", (void*)pointer);

    //Check if the header is valid or not
    if (header_ptr < &MY_HEAP[4]){ //Check if the user is not trying to free our precious data needed to manage the heap :(
        printf("You are not allowed to free this space !\n");
        return;
    } else if ((header & 0x1) == 0){ //Check if the bloc is indeed allocated to begin with
        printf("This space is already free !\n");
        return;
    } else { //Pointer is correct
        //Set the last bit to 0 to indicate that the bloc is free
        printf("Header value before : %d\n", header);
        header = header & ~0x1;
        printf("Header value after : %d\n", header);

        //Split the header into 2 uint8_t
        uint8_t first_part = header >> 8;
        uint8_t second_part = header & 0xFF;

        //Store it
        MY_HEAP[header_ptr - &MY_HEAP[0]] = first_part;
        MY_HEAP[header_ptr - &MY_HEAP[0] + 1] = second_part;

        //check if the correct value was stored
        printf("Value stored : %d\n", MY_HEAP[header_ptr - &MY_HEAP[0]] << 8 | MY_HEAP[header_ptr - &MY_HEAP[0] + 1]);
        //Update the number of mallocs done
        mallocs_done--;

        //Store the new value in the heap
        MY_HEAP[2] = mallocs_done >> 8;
        MY_HEAP[3] = mallocs_done & 0xFF;

        //Go to the next header and check if it is also free
        header_ptr += header;

        //Get the value of the next header
        uint16_t next_header = MY_HEAP[header_ptr - &MY_HEAP[0]] << 8 | MY_HEAP[header_ptr - &MY_HEAP[0] + 1];
        printf("Next header value : %d\n", next_header);
        //TODO finish this part to fuse free blocs

        //Check if next header is free too, if so fuse them, continue until the next header is not free
        /*while ((next_header & 0x1) != 1){
            //Get the size of the next header added to the current one
            header += next_header;
            printf("Header value : %d\n", header);

            next_header
        }*/
        printf("Next header address : %p\n", (void*)header_ptr);
        return;
    }
}

/* void *my_malloc(size_t size)
 *
 * This function allocates a memory space of the size provided
 *
 * size : size of the memory space to be allocated
 * return : pointer to the memory space allocated if everything went well, NULL if not
 *
 * */
void *my_malloc(size_t size){
    //TODO : fusing free cells that might have been missed by the free function + add a way to check if the bloc is the big boi or not, it causes issues when allocating in a small one
    //Change size to fit our design, like adding place for header (2 bytes) and adding a byte if the size wanted is odd.
    if (size % 2 != 0){size++;}
    size += 2;
    uint16_t allocated_size = (uint16_t) size;
    printf("Value to be allocated : %d\n", allocated_size);
    //Create a "pointer" that will hold our position in the heap, it will store how much we have gone from the root (MY_HEAP[0]), set to 4 since MY_HEAP[4] is the first usable space
    uint16_t displacement = 4;

    //Check if the biggest block can manage to hold it if no other space is found
    uint16_t max_box = MY_HEAP[0] << 8 | MY_HEAP[1];
    if (size > max_box){return NULL;}

    //Get the number of mallocs done for later
    uint16_t mallocs_done = MY_HEAP[2] << 8 | MY_HEAP[3];

    //Search for the place to put it
    //TODO : add the logic that allows to get the best out of 3 fits
    //1st take the first available header
    uint16_t current_header_value;

    //2nd loop on the boxes available and check if we can put it in
    //Need to loop on the memory spaces, from header to header (header + size of the bloc), if the last bit is 1, continue, else check if the size is enough, if it not continues
    //Max size loop = sizeof(MY_HEAP)

    while (displacement != (64000-4)){ //Max usable size of the heap
        //Get the value at the current header address
        current_header_value =  MY_HEAP[displacement] << 8 | MY_HEAP[displacement+1];

        //Check if the block is free or not
        printf("Currennt header address : %p and value : %d\n", (void*)&MY_HEAP[displacement], current_header_value);
        printf("displacement : %d\n", displacement);
        if ((current_header_value & 0x1) == 1){ //Not free, already used
            printf("Not free\n");
            printf("Which takes us from : %p to %p\n", (void*)&MY_HEAP[displacement], (void*)&MY_HEAP[displacement + (current_header_value & ~0x1)]);
            displacement += (current_header_value & ~0x1);
            current_header_value = MY_HEAP[displacement] << 8 | MY_HEAP[displacement+1];

        } else if ((current_header_value & 0x1) == 0){ //Free, potentially usable
            printf("Free to use\n");
            if (current_header_value >= allocated_size){
                printf("entered here\n");
                //Allocating the space //TODO : found the issue, incorrect change of header value when allocating a bloc freed before
                //Check if we are in the biggest bloc or not
                if (current_header_value != max_box){ //It is free so the value stored should be comparable without issues
                    max_box = current_header_value;
                }

                //Check if the size wanted won't leave some unusable space if done normally
                if ((current_header_value - 2) == allocated_size){
                    allocated_size = current_header_value; //Set the value to the size wanted if it does leave some unusable space
                }

                //We now need to change the header
                current_header_value = allocated_size;
                printf("current header value : %d\n", current_header_value);
                //Force the last bit to 1, to make it clear it is allocated
                current_header_value = current_header_value | 0x1;
                printf("current header value : %d\n", current_header_value);

                //Split the header into 2 separate bytes
                uint8_t first_part = current_header_value >> 8;
                uint8_t second_part = current_header_value & 0xFF;

                //Store the values at the correct place in the Heap
                MY_HEAP[displacement] = first_part;
                MY_HEAP[displacement+1] = second_part;
                printf("Value %d set at : %p and %p\n", current_header_value, (void*)&MY_HEAP[displacement], (void*)&MY_HEAP[displacement+1]);

                /*
                 * Test to check if the value stored is indeed correct
                 * */
                printf("Value stored : %d\n", MY_HEAP[displacement] << 8 | MY_HEAP[displacement+1]);

                //Update the number of mallocs done
                mallocs_done++;

                //Update the number of mallocs done in the heap
                MY_HEAP[2] = mallocs_done >> 8;
                MY_HEAP[3] = mallocs_done & 0xFF;

                //move to the next header
                //Update displacement / pointer of our position in the heap to the part where the data can be written to, aka 2 bytes further

                //Get the pointer to the place where data can be written to
                void *pointer = (void *) (&MY_HEAP[0]) + displacement + 2;
                printf("Pointer returned : %p\n", pointer);
                //--------------------------------------------------------------Down to here it's ok-------------------------------------------------------
                //Get to the next header address
                displacement += allocated_size;

                //printf("Available space : %d\n", max_box);
                //Check if we have to change the next header value
                uint16_t next_header_value = MY_HEAP[displacement] << 8 | MY_HEAP[displacement+1];
                printf("Next header value : %d\n", next_header_value);
                printf("Next header address : %p & %p\n", (void*)&MY_HEAP[displacement], (void*)&MY_HEAP[displacement+1]);
                if ((next_header_value & 0x1) == 1){
                    //if the next section is allocated, we can't change its value
                    return pointer;
                }
                //Get the number of space left //TODO : the issue comes from here probably, need to check if the bloc that has just been allocated was a part of the big one or not

                max_box -= allocated_size;

                //Set the value in the new header
                MY_HEAP[displacement] = max_box >> 8;
                MY_HEAP[displacement+1] = max_box & 0xFF;

                //Update the largest bloc available
                MY_HEAP[0] = max_box >> 8;
                MY_HEAP[1] = max_box & 0xFF;

                printf("Number of mallocs done : %d\n", mallocs_done);
                printf("Value stored : %d\n", MY_HEAP[displacement] << 8 | MY_HEAP[displacement+1]);
                printf("Available space : %d\n", max_box);
                printf("Next header address : %p\n", (void*)&MY_HEAP[displacement - 2]);
                printf("--------------------------------\n");
                return pointer;
            }
        } else {
            displacement += (current_header_value & ~0x1);
            current_header_value = MY_HEAP[displacement] << 8 | MY_HEAP[displacement+1];
        }
    }
    return NULL;
}

int main() {
    init();
    //printf("Available space : %d\n", MY_HEAP[0] << 8 | MY_HEAP[1]);
    //printf("Number of mallocs : %d\n", MY_HEAP[2] << 8 | MY_HEAP[3]);
    size_t sizeAllocated = 5;
    uint16_t *mem0 = (uint16_t*) my_malloc(sizeAllocated);
    uint16_t *mem1 = (uint16_t*) my_malloc(sizeAllocated +2);
    uint16_t *mem2 = (uint16_t*) my_malloc(sizeAllocated + 256);
    //uint16_t *mem4 = (uint16_t*) my_malloc(20000);

    printf("Free 0 ---------------------------------------\n");
    my_free(mem0);
    printf("Free 1 ---------------------------------------\n");
    my_free(mem1);
    printf("Free 2 ---------------------------------------\n");
    printf("Free 3 ---------------------------------------\n");
    //my_free(mem3);
    printf("Free 4 ---------------------------------------\n");
    //my_free(mem4);
    printf("Malloc ---------------------------------------\n");
    //uint16_t *mem3 = (uint16_t*) my_malloc(sizeAllocated+2); //TODO : issue when trying to reallocate a memory space that was freed before ! aka allocate memory not from the big boi bloc

    //printf("address returned : %p\n", (void*)mem4);
    //printf("address returned : %p\n", (void*)mem3);
    // print convert the first header to decimal and print it
    /*printf("First header : 0b");
    PRINT_BINARY(MY_HEAP[4] << 8 | MY_HEAP[5]);
    printf("\n");*/
    printf("Malloc ---------------------------------------\n");
    uint16_t *mem5 = (uint16_t*) my_malloc(sizeAllocated -2);
    //printf("address returned : %p\n", (void*)mem5);
    //Show mallocs done
    printf("---------------------------------------\nNumber of mallocs done : %d\n", MY_HEAP[2] << 8 | MY_HEAP[3]);
    //print_heap_status();
    print_test();
    printf("address returned : %p\n", (void*)mem0);
    //printf("address returned : %p\n", (void*)mem1);
    //printf("address returned : %p\n", (void*)mem2);
    //printf("address returned : %p\n", (void*)mem5);
    return 0;
}
