#include "ParticleExporter_OpenVDB.h"
#include <Utilities/Logger.h>
#include <Utilities/FileSystem.h>
#include "SPlisHSPlasH/Simulation.h"

using namespace SPH;
using namespace Utilities;

ParticleExporter_OpenVDB::ParticleExporter_OpenVDB(SimulatorBase *base) :
	ExporterBase(base)
{
	m_outfile = nullptr;
	openvdb::initialize();
}

ParticleExporter_OpenVDB::~ParticleExporter_OpenVDB(void)
{
}

void ParticleExporter_OpenVDB::init(const std::string& outputPath)
{
	m_exportPath = FileSystem::normalizePath(outputPath + "/openvdb");
}

void ParticleExporter_OpenVDB::step(const unsigned int frame)
{
	Simulation* sim = Simulation::getCurrent();
	for (unsigned int i = 0; i < sim->numberOfFluidModels(); i++)
	{
		FluidModel* model = sim->getFluidModel(i);
		std::string fileName = "ParticleData";
		if (!m_base->getValue<bool>(SimulatorBase::EXPORT_OBJECT_SPLITTING))
		{
			fileName = fileName + "_" + model->getId() + "_" + std::to_string(frame);
			std::string exportFileName = FileSystem::normalizePath(m_exportPath + "/" + fileName);
			writeParticles(exportFileName + ".vdb", model);
		}
		else
		{
			// object splitting
			for (auto j = 0u; j < m_base->getLastObjectId(); j++)
			{
				std::string fileName2 = fileName + "_" + model->getId() + "_" + std::to_string(j) + "_" + std::to_string(frame);
				std::string exportFileName = FileSystem::normalizePath(m_exportPath + "/" + fileName2);
				writeParticles(exportFileName + ".vdb", model, j);
			}
		}
	}
}

void ParticleExporter_OpenVDB::reset()
{
}

void ParticleExporter_OpenVDB::setActive(const bool active)
{
	ExporterBase::setActive(active);
	if (m_active)
	{
		FileSystem::makeDirs(m_exportPath);
	}
}

void ParticleExporter_OpenVDB::writeParticles(const std::string& fileName, FluidModel* model, const unsigned int objId)
{
// #ifdef USE_DOUBLE
// 	std::unordered_map<std::string, openvdb::DoubleGrid::Ptr> scalarGrids;
// #else
// 	std::unordered_map<std::string, openvdb::FloatGrid::Ptr> scalarGrids;
// #endif
// 	std::unordered_map<std::string, openvdb::Int32Grid::Ptr> intGrids;
// 	std::unordered_map<std::string, openvdb::VectorGrid::Ptr> vectorGrids;

// 	for (unsigned int j = 0; j < model->numberOfFields(); j++)
// 	{
// 		const FieldDescription& field = model->getField(j);
// 		if (field.type == Scalar) // density
// 		{
// #ifdef USE_DOUBLE
// 			openvdb::DoubleGrid::Ptr grid = openvdb::DoubleGrid::create();
// #else
// 			openvdb::FloatGrid::Ptr grid = openvdb::FloatGrid::create();
// #endif
// 			grid->setName(field.name);
// 			scalarGrids[field.name] = grid;
// 		} 
// 		else if (field.type == UInt) // id
// 		{
// 			openvdb::Int32Grid::Ptr grid = openvdb::Int32Grid::create();
// 			grid->setName(field.name);
// 			intGrids[field.name] = grid;
// 		}
// 		else if (field.type == Vector3) // velocity -> Blender color/temperature
// 		{
// 			openvdb::VectorGrid::Ptr grid = openvdb::VectorGrid::create();
// 			grid->setName(field.name);
// 			vectorGrids[field.name] = grid;
// 		}
// 	}

	openvdb::FloatGrid::Ptr testGrid = openvdb::FloatGrid::create(0.0f);
	// testGrid->setTransform(openvdb::Transform::UniformScale)
	testGrid->setName("density");
	openvdb::math::Transform::Ptr linearTransform = openvdb::math::Transform::createLinearTransform(0.01f);
	// TODO: Also rotate to Blender's space
	openvdb::FloatGrid::Accessor accessor = testGrid->getAccessor();

	const unsigned int numParticles = model->numActiveParticles();

	unsigned int counter = 0;
	for (unsigned int i = 0; i < numParticles; i++)
	{
		if ((objId != 0xffffffff) && (model->getObjectId(i) != objId))
			continue;

		const Vector3r& simPos = model->getPosition(i);
		openvdb::Vec3f worldSpacePoint(simPos[0], simPos[1], simPos[2]);
		openvdb::Vec3f indexSpacePoint = linearTransform->worldToIndex(worldSpacePoint);
		openvdb::Coord xyz((int32_t)indexSpacePoint.x(), (int32_t)indexSpacePoint.y(), (int32_t)indexSpacePoint.z());
		int id = model->getParticleId(i);

		// TODO: Set field grid values
		// for (auto& field : model->getFields())
		// {
		// 	if (field.type == FieldType::Scalar)
		// 	{

		// 	} 
		// 	else if (field.type == FieldType::UInt)
		// 	{

		// 	} else if (field.type == FieldType::Vector3)
		// 	{

		// 	}
		// }

		accessor.setValue(xyz, 1.0);
	}

	openvdb::io::File(fileName).write({testGrid}); // TODO: Write velocity and id grids here
}