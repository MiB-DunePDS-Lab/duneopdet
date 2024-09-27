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

// C++ Includes
#include <map>
#include <memory>
#include <string>
#include <stdio.h>
#include <iostream>
#include <vector>

// Framework includes
#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "art_root_io/TFileService.h"
#include "art_root_io/TFileDirectory.h"
#include "canvas/Persistency/Common/Ptr.h"
#include "art/Persistency/Common/PtrMaker.h"
#include "cetlib_except/exception.h"
#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/Sequence.h"
#include "canvas/Utilities/InputTag.h"
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

using std::string;
using std::vector;

namespace opdet
{
    class SPETemplateUtils
    {
    public:
        explicit SPETemplateUtils(fhicl::ParameterSet const &pset);
        int CountFileColumns(const char *file_path);
        void SourceSPEDigiDataFiles();
        void SourceNoiseTemplateFiles();

    private:
        // From fhicl configuration
        int fSamples;
        size_t fDigiDataColumn;
        std::string fSPEDataFile;
        std::vector<std::string> fDigiDataFiles;
        std::vector<std::string> fNoiseTemplateFiles;
        std::vector<std::vector<double>> fNoiseTemplates;
        std::vector<std::vector<double>> fSinglePEWaveforms;
        std::vector<double> fSinglePEAmplitudes;
    };
}
#endif