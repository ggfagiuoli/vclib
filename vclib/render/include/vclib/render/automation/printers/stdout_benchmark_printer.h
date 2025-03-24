#ifndef VCL_STDOUT_BENCHMARK_PRINTER_H
#define VCL_STDOUT_BENCHMARK_PRINTER_H

#include <vclib/render/automation/printers/benchmark_printer.h>

namespace vcl{

/*
    Class which writes the metric's results to stdout
*/
class StdoutBenchmarkPrinter : public BenchmarkPrinter
{
    uint32_t automationIndex = 0;
    uint32_t loopCounter = 0;

    public:

    void print(BenchmarkMetric &metric) override
    {
        printf("Loop %u, automation %u: %s\n",
            loopCounter,
            automationIndex,
            (metric.getMeasureString() + metric.getUnitOfMeasure()).c_str());

        automationIndex++;
    };

    void onBenchmarkLoop() override
    {
        loopCounter++;
    }

    void finish(BenchmarkMetric &metric) override {};

    std::shared_ptr<BenchmarkPrinter> clone() const & override
    {
        return std::make_shared<StdoutBenchmarkPrinter>(*this);
    };

    std::shared_ptr<BenchmarkPrinter> clone() && override
    {
        return std::make_shared<StdoutBenchmarkPrinter>(std::move(*this));
    };
};

}

#endif