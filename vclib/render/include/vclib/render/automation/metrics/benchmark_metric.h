#ifndef VCL_BENCHMARK_METRIC_H
#define VCL_BENCHMARK_METRIC_H

#include <string>

namespace vcl{

/*
    Absttract class that represents a way of measuring the performance of an Automation.
*/
class BenchmarkMetric
{
    public:
    
    virtual void start() = 0;
    virtual void measure() = 0;
    virtual std::string getMeasureString() = 0;
    virtual std::string getUnitOfMeasure() = 0;
    virtual void end() = 0;

    virtual std::shared_ptr<BenchmarkMetric> clone() const & = 0;
    virtual std::shared_ptr<BenchmarkMetric> clone()  && = 0;
};

}

#endif