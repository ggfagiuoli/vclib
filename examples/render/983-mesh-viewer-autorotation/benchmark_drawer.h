#ifndef BENCHMARK_DRAWER_H
#define BENCHMARK_DRAWER_H

#include "automation_action.h"
#include <bgfx/bgfx.h>
#include <vector>
#include <vclib/space/core/vector/polymorphic_object_vector.h>
#include <chrono>
#include <iostream>
#include <vclib/misc/timer.h>
#include <string>
#include <fstream>

#define BENCHMARK_DRAWER_REPEAT_FOREVER 0

template<typename DerivedDrawer>
class BenchmarkDrawer : public vcl::PlainDrawer<DerivedDrawer>
{
    using Parent = vcl::PlainDrawer<DerivedDrawer>;
    vcl::PolymorphicObjectVector<AutomationAction> automations;
    std::vector<bool> relevancies;
    std::string outStr = "";
    std::ofstream outStream;
    size_t currentAutomationIndex = 0;
    uint32_t repeatTimes = 1;
    uint32_t repeatCount = 0;
    vcl::Timer currentAutomationTimer;
    uint32_t currentAutomationFrames = 0;
    bool toStdOut = true;
    bool firstCall = true;
    bool allDone = false;

    void benchmarkLoop()
    {
        currentAutomationIndex = 0;
        allDone = false;
    };

    bool isLastLoop()
    {
        return repeatTimes != BENCHMARK_DRAWER_REPEAT_FOREVER && repeatCount >= repeatTimes; 
    }

    void onAutomationEnd()
    {
        currentAutomationTimer.stop();
        double currentAutomationSeconds = currentAutomationTimer.delay();
        if(relevancies[currentAutomationIndex]){
            double framerate = (double)currentAutomationFrames / currentAutomationSeconds;
            if(toStdOut){
                printf("Loop %u, automation %zu : %.4f fps average\n", repeatCount, currentAutomationIndex, framerate);
            }else{
                outStr += std::format("{:.3f}", framerate);
                if(currentAutomationIndex == automations.size()-1){
                    outStr += "\n";
                }else{
                    outStr += ",";
                }
                printf("outStr: %s\n", outStr.c_str());
            }
        }
        currentAutomationFrames = 0;
        currentAutomationIndex++;
    }

public:

    using Parent::Parent;
    using Parent::onDraw;
    using Parent::onInit;
    using Parent::onResize;
    using Parent::onPostDraw;

    void setRepeatTimes(float repeatTimes)
    {
        this->repeatTimes = repeatTimes;
    }

    void onDrawContent(uint viewId) override
    {
        if(automations.size() == 0){
            allDone = true;
        }
        if(allDone){
            return;
        }
        if(firstCall){
            automations[0]->start();
            firstCall = false;
            currentAutomationTimer.start();
        }
        if(!automations[currentAutomationIndex]->isActive()){

            onAutomationEnd();
            allDone = currentAutomationIndex >= automations.size();
            if(allDone){
                repeatCount++;
                if(isLastLoop()){
                    printf("All benchmarks done.\n");
                    if(!toStdOut){
                        outStream << outStr;
                        outStream.close();
                    }
                    return;
                }
                benchmarkLoop();
            }
            currentAutomationTimer.start();
            automations[currentAutomationIndex]->start();
        }
        currentAutomationFrames++;
        if(automations[currentAutomationIndex]->isActive()){
            automations[currentAutomationIndex]->update();
        }
    };

    size_t addAutomation(const AutomationAction &action, bool relevancy = true)
    {
        automations.pushBack(action);
        relevancies.push_back(relevancy);
        return automations.size()-1;
    }

    void restartBenchmark()
    {
        currentAutomationIndex = 0;
        firstCall = true;
        allDone = false;
        repeatCount = 0;
    };

    void useStdOut()
    {
        toStdOut = true;
    }

    void setOutputFile(const std::string str){
        outStream.open(str);
        if(outStream.fail()){
            printf("Error: couldn't open file %s. Writing to stdout instead\n", str.c_str());
            return;
        }
        toStdOut = false;
    }
};

#endif