#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <mach/mach_init.h>
#include <mach/mach_vm.h>
#include <sys/mman.h>
#include <unistd.h>
#include <dlfcn.h>
#include <mutex>

static kern_return_t patch_memory(void *dst, const void *src, size_t size)
{
	kern_return_t kr = 0;

	kr = mach_vm_protect(mach_task_self(),
        (mach_vm_address_t)dst, (mach_vm_size_t)size,
        FALSE, VM_PROT_READ | VM_PROT_WRITE | VM_PROT_EXECUTE | VM_PROT_COPY);
	if (kr != KERN_SUCCESS) {
		printf("mach_vm_protect() failed with error: 0x%x\n", kr);
		return (kr);
	}

	kr = mach_vm_write(mach_task_self(), (mach_vm_address_t)dst, (vm_offset_t)src, size);
	if (kr != KERN_SUCCESS) {
		printf("mach_vm_write() failed with error: 0x%x\n", kr);
		return (kr);
	}

	kr = mach_vm_protect(mach_task_self(),
        (mach_vm_address_t)dst, (mach_vm_size_t)size,
        FALSE, VM_PROT_READ | VM_PROT_EXECUTE);
	if (kr != KERN_SUCCESS) {
		printf("mach_vm_protect 2() failed with error: 0x%x\n", kr);
	}

    return kr;
}

struct Restorable
{
    void *orig_;
    uint8_t *backup_;
    size_t size_;

    Restorable(void *orig, void *code, size_t size)
        : orig_(orig), size_(size), backup_(new uint8_t[size])
    {
        memcpy(backup_, orig, size);
        patch_memory(orig, code, size);
    }

    ~Restorable()
    {
        patch_memory(orig_, backup_, size_);
        delete[] backup_;
    }

    Restorable swap()
    {
        return Restorable(orig_, backup_, size_);
    }
};

template <typename>
struct Hook;

template <typename R, typename ...Args>
struct Hook<R(*)(Args...)>
{
    using Fn = R(*)(Args...);

    Fn orig_func_ = nullptr;
    Restorable *rest_ = nullptr;
    std::mutex mutex_;

    ~Hook()
    {
        if (rest_ != nullptr) {
            delete rest_;
        }
    }

    bool hook(Fn orig, Fn hook)
    {
        uint8_t opcodes[16];
        memset(opcodes, 0, sizeof(opcodes));

        orig_func_ = orig;

        opcodes[0] = 0xFF;
        opcodes[1] = 0x25;
        *(int *)(&opcodes[2]) = 0;
        *(intptr_t *)(&opcodes[6]) = (intptr_t)hook;

        rest_ = new Restorable((void *)orig, opcodes, sizeof(opcodes));
        return true;
    }

    R operator ()(Args... args)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        {
            Restorable _t = rest_->swap();
            {
                return orig_func_(args...);
            }
        }
    }
};