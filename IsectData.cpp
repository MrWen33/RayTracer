#include "IsectData.h"

const double ClosestHitInfo::inf_t = 1e9;

ClosestHitInfo::ClosestHitInfo()
{
	prim = NULL;
	min_t = inf_t;
}
