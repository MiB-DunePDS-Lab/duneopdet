#include "SPETemplateUtils.h"

namespace opdet
{
    SPETemplateUtils::SPETemplateUtils(fhicl::ParameterSet const &pset)
        : fSamples(pset.get<int>("Samples")),
          fDigiDataColumn(pset.get<size_t>("DigiDataColumn")),
          fSPEDataFile(pset.get<std::string>("SPEDataFile")),
          fDigiDataFiles(pset.get<std::vector<std::string>>("DigiDataFiles")),
          fNoiseTemplateFiles(pset.get<std::vector<std::string>>("NoiseTemplateFiles"))
    {
    }

    /**
     * @brief Count the nr of column in a txt file
     *
     * @param file_path
     *
     * @return nr of columns
     */
    int SPETemplateUtils::CountFileColumns(const char *file_path)
    {
        std::ifstream file_;
        file_.open(file_path);

        if (!file_.is_open())
        {
            printf("Deconvolution::CountFileColumns(%s) ERROR:\n",
                   file_path);
            printf("Unable to open file.");
            throw art::Exception(art::errors::FileOpenError);
        }

        int N_COLUMNS = 0;
        std::string line;
        int iline = 0;
        while (std::getline(file_, line))
        {
            std::stringstream sstream;
            sstream << line;
            std::string sub;
            int n_columns = 0;

            while (sstream)
            {
                sstream >> sub;
                if (sub.length())
                    ++n_columns;
            }

            if (iline == 1)
            {
                N_COLUMNS = n_columns;
            }
            else if (iline > 1)
            {
                if (n_columns != N_COLUMNS)
                {
                    printf("Deconvolution::CountFileColumns(%s): WARNING ",
                           file_path);
                    printf("Nr of columns change along the file!\n");
                    N_COLUMNS = n_columns;
                }
            }
            iline++;
        }
        file_.close();
        return N_COLUMNS;
    }

    /**
     * @brief Source the single p.e. response from file
     *
     * Source the single p.e. response template from the dat file set by
     * `fDigiDataFile` and set the variable `fSinglePEAmplitude` with the
     * amplitude of the single p.e. response. In case of a multi-column
     * template file, the relevant column can be selected by setting the
     * variable `fDigiDataColumn`.
     */
    void SPETemplateUtils::SourceSPEDigiDataFiles()
    {
        cet::search_path sp("FW_SEARCH_PATH");
        for (auto fname : fDigiDataFiles)
        {
            fSinglePEWaveforms.push_back(std::vector<double>()); // add a new empty waform
            auto &spewfrm = fSinglePEWaveforms.back();           // get the reference to the waveform vector
            std::string datafile;
            // taking the file name as the first argument,
            // the second argument is the local variable where to store the full path - both are std::string objects
            sp.find_file(fname, datafile);
            std::ifstream SPEData;
            SPEData.open(datafile);
            size_t n_columns = CountFileColumns(datafile.c_str());
            std::cout << "ncols= " << n_columns << std::endl;
            if (fDigiDataColumn >= n_columns)
            {
                printf("Deconvolution::SourceSPETemplate ERROR: ");
                printf("The module is supposed to select column %lu, but only %lu columns are present.\n",
                       fDigiDataColumn, n_columns);
                throw art::Exception(art::errors::InvalidNumber);
            }

            Double_t buff[100] = {0};

            std::string temp_str;
            if (SPEData.is_open())
            {
                while (std::getline(SPEData, temp_str))
                {
                    std::stringstream ss;
                    ss << temp_str;
                    int icol = 0;
                    while (ss)
                    {
                        ss >> buff[icol];
                        ++icol;
                    }

                    spewfrm.push_back(buff[fDigiDataColumn]);
                }
            }
            else
            {
                printf("Deconvolution::produce ERROR ");
                printf("Cannot open SPE template file.\n");

                throw art::Exception(art::errors::FileOpenError);
            }

            spewfrm.resize(fSamples, 0);

            SPEData.close();

            // Set single p.e. maximum value
            fSinglePEAmplitudes.push_back(TMath::Max(1.0,
                                                     *(std::max_element(spewfrm.begin(), spewfrm.end()))));
            std::cout << "SPE Amplitude for template " << fSinglePEWaveforms.size() << ": " << fSinglePEAmplitudes.back() << std::endl;
        }
        return;
    }

    /**
     * @brief Source template noise from files
     *
     * Source the noise templates from the dat files set by
     * `fNoiseTemplateFiles`.
     */
    void SPETemplateUtils::SourceNoiseTemplateFiles()
    {
        cet::search_path sp("FW_SEARCH_PATH");
        for (auto fname : fNoiseTemplateFiles)
        {
            fNoiseTemplates.push_back(std::vector<double>()); // add a new empty waform
            auto &noisewfrm = fNoiseTemplates.back();         // get the reference to the waveform vector
            std::string datafile;
            // taking the file name as the first argument,
            // the second argument is the local variable where to store the full path - both are std::string objects
            sp.find_file(fname, datafile);
            std::ifstream noiseData;
            noiseData.open(datafile);

            std::string temp_str;
            double temp = 0.;
            if (noiseData.is_open())
            {
                while (std::getline(noiseData, temp_str))
                {
                    std::stringstream ss;
                    ss << temp_str;
                    ss >> temp;
                    noisewfrm.push_back(temp);
                }
            }
            else
            {
                printf("SPETemplateUtils::SourceNoiseTemplateFiles ERROR ");
                printf("Cannot open noise template file.\n");

                throw art::Exception(art::errors::FileOpenError);
            }

            noisewfrm.resize(fSamples, 0.);

            noiseData.close();
        }
        return;
    }
} // namespace solar