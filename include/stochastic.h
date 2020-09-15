#include <cstddef>

#define MAX_ARRAY_SIZE 200

class Stochastic
{
    private:
    float stochasticResult;
    float lowPriceDaily;
    float highPriceDaily;
    float kcurve;
    struct stochasticData {
        float priceCloseDaily[MAX_ARRAY_SIZE];
        float priceHighDaily[MAX_ARRAY_SIZE];
        float priceLowDaily[MAX_ARRAY_SIZE];
        float Kcurve[MAX_ARRAY_SIZE];
        float KcurveAverage[MAX_ARRAY_SIZE]; 
        float price[MAX_ARRAY_SIZE];
    } stochasticData;

    public:
        Stochastic();
        ~Stochastic();
        void setLowDailyPrice(void);
        void setHighDailyPrice(void);
        void setPrice(float *, size_t);
        void getKcurve(void);
        void averageKcurve(void);
        float getResult(void);
        float getLowDailyPrice(void);
        float getHighDailyPrice(void);
        size_t valoresMinimos(float * output, float * inputArray, size_t inputSize);
        size_t valoresMaximos(float * output, float * inputArray, size_t inputSize);
        size_t kValues(float * output, float * arrayMin, float* arrayMax, float* arrayClose, size_t sizeClose);
};
