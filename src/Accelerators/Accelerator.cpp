#include "Accelerator.h"
#include "Brute.h"
#include "BVH.h"

std::unique_ptr<Accelerator> Accelerator::GetAccelerator(const std::string& name) {
    if (name == "Brute") {
        return std::make_unique<Brute>();
    }
    else if (name == "BVH") {
        return std::make_unique<BVH>();
    }
    else {
        throw std::exception("No such accelerator!");
    }
    return nullptr;
}
