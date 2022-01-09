#include "Accelerator.h"
#include "Brute.h"
#include "BVH.h"
#include "KDTree.h"

std::unique_ptr<Accelerator> Accelerator::GetAccelerator(const std::string& name)
{
	if (name == "Brute")
	{
		return std::make_unique<Brute>();
	}
	else if (name == "BVH")
	{
		return std::make_unique<BVH>();
	}
	else if (name == "KDTree")
	{
		return std::make_unique<KDTree>();
	}
	else
	{
		throw std::exception("No such accelerator!");
	}
	return nullptr;
}
