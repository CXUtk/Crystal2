#include "Accelerator.h"
#include "Brute.h"

std::unique_ptr<Accelerator> Accelerator::GetAccelerator(const std::string& name) {
    if (name == "Brute") {
        return std::make_unique<Brute>();
    }
    return std::unique_ptr<Accelerator>();
}