#include <catalyst/logging.h>
#include <catalyst/memory.h>

int main(int argc, char** argv) {
    catalyst_initialize_logging(); 
    catalyst_initialize_memory_subsystem();

    void* linear_memory = catalyst_allocate(sizeof (char) * 4096, alignof (char));

    catalyst_allocator_t* linear_allocator = catalyst_create_allocator(
                CATALYST_ALLOCATOR_TYPE_LINEAR,
                linear_memory, sizeof(char) * 64, sizeof(char)
            );
    
    if(linear_allocator)
        CATALYST_TRACE("Linear allocator %p has been successfully created", linear_allocator);
    CATALYST_INFO("Catalyst Editor has been successfully initialized");

    for(size_t i = 0; i < 64 / sizeof(int); i++) {
        int* leaking_allocation = catalyst_allocator_allocate(linear_allocator, sizeof(int), alignof(int));
        if(leaking_allocation)
            CATALYST_DEBUG("Leaking allocation %p is successful", leaking_allocation);
        else
            CATALYST_DEBUG("Leaking allocation was unsuccessful");
    }

    if(linear_allocator) {
        catalyst_deallocate(linear_memory);
        catalyst_delete_allocator(linear_allocator);
        CATALYST_TRACE("Linear allocator %p has been successfully deleted", linear_allocator);
    }

    catalyst_terminate_memory_subsystem();
    catalyst_terminate_logging(); 
    
    return 0;
}
