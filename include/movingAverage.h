#ifndef MOVINGAVERAGE_HEADER_GUARD
#define MOVINGAVERAGE_HEADER_GUARD

class MovingAverage{
	private:
		float average;
	public:
		MovingAverage(float *, int);
		void setmovingAverage(float *, int);
		float getAverage();
		~MovingAverage();
};

#endif