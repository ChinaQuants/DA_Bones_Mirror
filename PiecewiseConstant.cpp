
#include "Platform.h"
#include "PiecewiseConstant.h"
#include "Strict.h"

#include "Algorithms.h"
#include "Archive.h"

namespace
{
#include "MG_PiecewiseConstant_v1_Write.inc"
#include "MG_PiecewiseConstant_v1_Read.inc"
}

void PiecewiseConstant_::Write(Archive::Store_& dst) const
{
   PiecewiseConstant_v1::XWrite(dst, knotDates_, fRight_, name_);
}

Vector_<> PiecewiseConstant_::Sofar() const
{
	Vector_<> retval(1, 0.0);
	for (int ii = 1; ii < knotDates_.size(); ++ii)
	{
		const double dt = knotDates_[ii] - knotDates_[ii - 1];
		retval.push_back(retval.back() + dt * fRight_[ii - 1]);
	}
	return retval;
}

double PiecewiseConstant_::IntegralTo(const DateTime_& dt) const
{
	const auto iGE = LowerBound(knotDates_, dt) - knotDates_.begin();
	if (iGE <= 0)
		return 0.0;
	if (iGE < knotDates_.size() && knotDates_[iGE] == dt)
		return sofar_[iGE];
	const auto iLT = iGE - 1;
	const double elapsed = dt - knotDates_[iLT];
	return sofar_[iLT] + elapsed * fRight_[iLT];
}

double PWC::F
	(const PiecewiseConstant_& pwc,
	 const DateTime_& dt,
	 bool* is_knot)
{
	const auto pGE = LowerBound(pwc.knotDates_, dt);
	const auto iGE = pGE - pwc.knotDates_.begin();
	if (pGE != pwc.knotDates_.end() && *pGE == dt)
	{
		ASSIGN(is_knot, true);
		return pwc.fRight_[iGE];
	}
	ASSIGN(is_knot, false);
	return pGE == pwc.knotDates_.begin()
			? 0.0
			: pwc.fRight_[iGE - 1];
}

