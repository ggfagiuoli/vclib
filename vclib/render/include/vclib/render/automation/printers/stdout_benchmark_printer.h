/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

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