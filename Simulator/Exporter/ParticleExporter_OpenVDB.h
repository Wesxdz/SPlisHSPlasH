#ifndef __ParticleExporter_OpenVDB_h__
#define __ParticleExporter_OpenVDB_h__

#include "ExporterBase.h"
#include "SPlisHSPlasH/FluidModel.h"
#include <fstream>

namespace OpenVDB
{
	/** \brief Particle exporter for the OpenVDB format.
	*/
	class ParticleExporter_OpenVDB : public ExporterBase
	{
	protected: 
		std::string m_exportPath;
		std::ofstream *m_outfile;
		std::vector<std::string> m_attributes;

		void createParticleFile(const std::string& fileName, FluidModel* model);
		void writeParticles(const std::string& fileName, FluidModel* model, const unsigned int objId = 0xffffffff);

	public:
		ParticleExporter_OpenVDB(SimulatorBase *base);
		ParticleExporter_OpenVDB(const ParticleExporter_OpenVDB&) = delete;
		ParticleExporter_OpenVDB& operator=(const ParticleExporter_OpenVDB&) = delete;
		virtual ~ParticleExporter_OpenVDB(void);

		virtual void init(const std::string& outputPath);
		virtual void step(const unsigned int frame);
		virtual void reset();
		virtual void setActive(const bool active); 
	};
}

#endif
