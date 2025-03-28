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

#ifndef VCL_CSV_BENCHMARK_PRINTER_H
#define VCL_CSV_BENCHMARK_PRINTER_H

#include <vclib/render/automation/printers/benchmark_printer.h>

#include <fstream>
#include <sstream>
#include <utility>

namespace vcl {

/*
    Class which writes the metric's results to a csv file
*/
class CsvBenchmarkPrinter : public BenchmarkPrinter
{
    uint32_t                                    loopCounter        = 0;
    uint32_t                                    automationCounter  = 0;
    uint32_t                                    maxMeasurementSize = 0;
    std::string                                 fileName;
    std::ofstream                               stream;
    std::vector<std::pair<std::string, size_t>> measurementStrings;

public:
    CsvBenchmarkPrinter(const std::string& fileName) : fileName {fileName}
    {
        stream.open(fileName);
        if (stream.fail()) {
            throw "CsvBenchmarkPrinter : invalid file name\n";
        }
    };

    CsvBenchmarkPrinter(const CsvBenchmarkPrinter& other) :
            fileName {other.fileName}, stream()
    {
        stream.open(fileName);
    };

    void onBenchmarkLoop() override
    {
        loopCounter++;
        automationCounter = 0;
    };

    void print(BenchmarkMetric& metric) override
    {
        std::ostringstream temp;
        temp << loopCounter << ";" << automationCounter;

        std::vector<std::string> measureStrings = metric.getMeasureStrings();

        maxMeasurementSize =
            std::max(maxMeasurementSize, (uint32_t) measureStrings.size());

        for (size_t i = 0; i < measureStrings.size(); i++) {
            measureStrings[i] = measureStrings[i] + metric.getUnitOfMeasure();
        }

        if (metric.getMeasureStrings().size() > 0) {
            temp << ";";
            std::copy(
                measureStrings.begin(),
                measureStrings.end() - 1,
                std::ostream_iterator<std::string>(temp, ";"));
            temp << measureStrings.back();
        }

        measurementStrings.push_back(
            std::make_pair(temp.str(), measureStrings.size()));

        automationCounter++;
    };

    void finish(BenchmarkMetric& metric) override
    {
        stream << "Loop;Automation";
        for (uint32_t i = 0; i < maxMeasurementSize; i++) {
            stream << ";Measurement " << i;
        }
        for (auto meas : measurementStrings) {
            stream << "\n"
                   << meas.first
                   << std::string(maxMeasurementSize - meas.second, ';');
        }
        stream.close();
    };

    std::shared_ptr<BenchmarkPrinter> clone() const& override
    {
        return std::make_shared<CsvBenchmarkPrinter>(*this);
    };

    std::shared_ptr<BenchmarkPrinter> clone() && override
    {
        return std::make_shared<CsvBenchmarkPrinter>(std::move(*this));
    };

    ~CsvBenchmarkPrinter()
    {
        if (stream.is_open()) {
            stream.close();
        }
    };
};

} // namespace vcl

#endif
