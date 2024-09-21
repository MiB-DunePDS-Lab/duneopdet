// ========================================================================================
// SPETemplateUtils.h
// This library is an auxiliary library for the OpDetDigitizerDUNE and Deconvolution modules.
// It contains utility functions that are used to generate and deconvolve wvfs.
//
// @authors     : Viktor Pec, Sergio Manthey Corchado
// @created     : Jul, 2024
//=========================================================================================

#ifndef SPETemplateTool_h
#define SPETemplateTool_h

#include <map>
#include <cmath>
#include <limits>
#include <memory>
#include <string>
#include <vector>
#include <fcntl.h>
#include <iomanip>
#include <stdio.h>
#include <iostream>

#include "messagefacility/MessageLogger/MessageLogger.h"
#include "fhiclcpp/ParameterSet.h"

namespace opdet
{
    class SPETemplateUtils
    {
    public:
        explicit SPETemplateUtils(fhicl::ParameterSet const &p);
        static int GetColor(std::string ColorName);
        static void PrintInColor(std::string MyString, int Color, std::string Type = "Info");
        int CountFileColumns(const char *file_path);
        void CreateSinglePEWaveform();
        void SourceSPEDigiDataFiles();
        void SourceNoiseTemplateFiles();

    private:
        // From fhicl configuration
        const int fSamples;
        const int fDigiDataColumn;
        const std::string fDigiDataFiles;
        const std::string fNoiseTemplateFiles;
    };
}
#endif