//
// Created by Sunny on 24-10-26.
//

#include "filterTools.h"

FilterMovingAverage::FilterMovingAverage(uint32_t len)
{
	len_ = len;
	sum_ = 0;
}

void FilterMovingAverage::push(float value)
{
	if (valueQueue_.size() == len_)
	{
		sum_ -= valueQueue_.front();
		valueQueue_.pop();
	}
	valueQueue_.push(value);
	sum_ += value;
}

float FilterMovingAverage::getResult()
{
	if (valueQueue_.empty())
		return 0;
	return sum_ / (float)valueQueue_.size();
}

FilterRcLinear::FilterRcLinear(float alpha)
{
	alpha_ = alpha;
	output_ = 0;
}
void FilterRcLinear::push(float value)
{
	output_ = alpha_ * value + (1-alpha_) * output_;
}
float FilterRcLinear::getResult()
{
	return output_;
}