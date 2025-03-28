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

#ifndef VCL_NULL_BENCHMARK_METRIC_H
#define VCL_NULL_BENCHMARK_METRIC_H

#include <string>
#include <format>
#include <vclib/render/automation/metrics/benchmark_metric.h>

namespace vcl{

/*
    Measures nothing
*/
class NullBenchmarkMetric : public BenchmarkMetric
{
    public:

    void start() override {};

    void measure() override {};

    std::vector<std::string> getMeasureStrings() override { return std::vector<std::string>{""}; };

    std::string getUnitOfMeasure() override { return ""; };

    std::string getFullLengthUnitOfMeasure() override { return ""; };

    void end() override {};

    std::shared_ptr<BenchmarkMetric> clone() const & override
    {
        return std::make_shared<NullBenchmarkMetric>(*this);
    };

    std::shared_ptr<BenchmarkMetric> clone()  && override
    {
        return std::make_shared<NullBenchmarkMetric>(std::move(*this));
    };
};

}

#endif