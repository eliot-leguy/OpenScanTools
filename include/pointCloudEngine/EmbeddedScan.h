#ifndef EMBEDDED_SCAN_H
#define EMBEDDED_SCAN_H

#include "models/pointCloud/TLS.h"
#include "models/graph/TransformationModule.h"
#include "pointCloudEngine/PCE_graphics.h"
#include "pointCloudEngine/PCE_stream.h"
#include "pointCloudEngine/OctreeBase.h"
#include "pointCloudEngine/SmartBuffer.h"
#include "pointCloudEngine/SmartBufferWorkload.h"
#include "models/data/clipping/ClippingGeometry.h"
#include "io/imports/TlsReader.h"
#include "pointCloudEngine/OctreeRayTracing.h"

#include <vector>
#include <atomic>
#include <filesystem>
#include <mutex>

class IScanFileWriter;

struct TestInside
{
    bool inside[6];
    bool allInside;
    bool oneOutside;
};

// **** Cube vertices in homogeneous coordinates ****
// \brief Vertices order are { xyz, xyZ, xYz, xYZ, Xyz, XyZ, XYz, XYZ }
// Because we are in homogeneous coordinates we can quickly compute a middle point by summing 2 points.
struct HCube
{
    HCube() noexcept {};

    // The anchor corner is the cube's corner with minimum
    HCube(const glm::dmat4& boxSpace, float anchorCorner[3], float size) noexcept
    {
        halfSize = size / 2.0f;
        center = boxSpace * glm::dvec4(anchorCorner[0] + size / 2.0, anchorCorner[1] + size / 2.0, anchorCorner[2] + size / 2.0, 1.0);
        for (int x = 0; x < 2; ++x)
        {
            for (int y = 0; y < 2; ++y)
            {
                for (int z = 0; z < 2; ++z)
                {
                    glm::dvec4 localCorner(anchorCorner[0] + x * size, anchorCorner[1] + y * size, anchorCorner[2] + z * size, 1.0);

                    // The corner must be indexed in the same order than the children in the octree
                    corner[x * 4 + y * 2 + z] = boxSpace * localCorner;
                }
            }
        }
    }

    HCube(HCube const& hcube, uint32_t j) noexcept
        : corner{ hcube.corner[j] + hcube.corner[0],
            hcube.corner[j] + hcube.corner[1],
            hcube.corner[j] + hcube.corner[2],
            hcube.corner[j] + hcube.corner[3],
            hcube.corner[j] + hcube.corner[4],
            hcube.corner[j] + hcube.corner[5],
            hcube.corner[j] + hcube.corner[6],
            hcube.corner[j] + hcube.corner[7] }
    {}

    glm::dvec4 corner[8];
    glm::dvec4 center;
    float halfSize;
};

struct TlFrustumTest
{
    HCube cube;
    TestInside test;
};

struct TlClippingTest
{
    ClippingShape clippingForm;
    HCube cube;
    TestInside testInside;
    uint32_t clipId;
};

/*
- Le TlScan contient les informations logiques sur un nuage de point : position, nombre de points, structure en octree.
- Le TlScan fait le lien entre les informations logiques et les emplacements physiques des données.
- Les emplacements physiques peuvent être sur disque secondaire (HDD), sur la mémoire primaire(CPU), ou sur la mémoire vidéo (GPU)
*/
class EmbeddedScan : public OctreeBase
{
TLS_READER_DECLARE(EmbeddedScan);

public:
    EmbeddedScan(std::filesystem::path const& filepath);
    ~EmbeddedScan();

    tls::ScanGuid getGuid() const;
    tls::FileGuid getFileGuid() const;
    void getInfo(tls::ScanHeader &info) const;
    std::filesystem::path getPath() const;
    void setPath(const std::filesystem::path& newPath);

    // For rendering
    bool getGlobalDrawInfo(TlScanDrawInfo& ScanrawInfo);
    bool viewOctree(std::vector<TlCellDrawInfo>& cellDrawInfo, const TlProjectionInfo& projInfo);
    bool viewOctreeCB(std::vector<TlCellDrawInfo>& cellDrawInfo, std::vector<TlCellDrawInfo_multiCB>& cellDrawInfoCB, const TlProjectionInfo& projInfo, const ClippingAssembly& clippingAssembly);

#ifndef PORTABLE
    // For File Clipping
    bool clipAndWrite(const TransformationModule& modelMat, const ClippingAssembly& clippingAssembly, IScanFileWriter* writer);
#endif

    void decodePointXYZIRGB(uint32_t cellId, std::vector<PointXYZIRGB>& dstPoints);
    void decodePointCoord(uint32_t cellId, std::vector<glm::dvec3>& dstPoints, uint32_t layerDepth, bool transformToGlobal);

    // Allow to transfer the point buffers possesion from one scan to another.
    //   - The corresponding map of ids allow the use 2 scans with different octree layout.
    //   - WARNING: there must be external guaranties that the 2 scans are related.
    static bool movePointBuffers(EmbeddedScan& dstScan, EmbeddedScan& srcScan, const std::vector<uint32_t>& correspCellId);

    // For changing the local referential (in case of multiple scene instance)
    void setComputeTransfo(const glm::dvec3& t, const glm::dquat& q);

    BoundingBox getLocalBoundingBox() const;

    // For streaming
    void assumeWorkload();
    bool startStreamingAll(char* _stageBuf, uint64_t _stageSize, uint64_t& _stageOffset, std::vector<TlStagedTransferInfo>& gpuTransfers);
    void checkDataState();

    bool canBeDeleted();
    void deleteFileWhenDestroyed(bool deletePhysicalFile);

protected:
    bool copyData(std::ifstream& _istream, void* const _dest, uint64_t _filePos, uint64_t _dataSize) const;

    bool getVisibleTree_impl(uint32_t _cellId, std::vector<TlCellDrawInfo>& _result, const TlProjectionInfo& _projInfo, const TlFrustumTest& _frustumTest, std::vector<uint32_t>& _missingCells);
    bool getVisibleTreeMultiClip_impl(uint32_t _cellId, std::vector<TlCellDrawInfo>& _result, std::vector<TlCellDrawInfo_multiCB>& _resultCB, const TlProjectionInfo& _projInfo, const TlFrustumTest& _frustumTest, const ClippingAssembly& clippingAssembly, std::vector<uint32_t>& _missingCells);
    
    uint64_t getMinimumPointsResolution(const HCube& _cube, uint64_t width, uint64_t height, float pointSize);

    // Clipping Export
    void getClippedCells_impl(uint32_t cellId, const ClippingAssembly& clippingAssembly, std::vector<std::pair<uint32_t, bool>>& _result);

    // Proceed to:
    //  * allocate the cell buffers not already loaded
    //  * guard_lock the buffers
    //  * if some buffers are not loaded, add this object to the streamer queue
    // Return:
    //  * 'true' if all the buffers are availables
    //  * 'false' if one buffer or more buffers are missing
    bool treatWorkload(SmartBufferWorkload& sbw);

    void getDecodedPoints(const std::vector<uint32_t>& leavesId, std::vector<glm::dvec3>& retPoints, bool transformToGlobal);

    // Temporary functions, for dev evaluation
    void startDecodingStats();
    void timerStart();
    void timerEnd(size_t pointCount);
    void endDecodingStats(std::string label);

    struct ConcatCellStates
    {
        uint32_t countByState[(size_t)TlDataState::MAX_ENUM];
        VkDeviceSize sizeByState[(size_t)TlDataState::MAX_ENUM];
        void reset();
    };

    void checkCellDataState(uint32_t _cellId, ConcatCellStates& _concatStates, uint32_t _depth);

public:
    // Lucas functions
    bool beginRayTracing(const glm::dvec3& globalRay, const glm::dvec3& globalRayOrigin, glm::dvec3& bestPoint, const double& cosAngleThreshold, const ClippingAssembly& clippingAssembly, const bool& isOrtho);
	bool findNeighborsBucketsDirected(const glm::dvec3& globalSeedPoint, const glm::dvec3& directedPoint, const double& radius, std::vector<std::vector<glm::dvec3>>& neighborList, const int& numberOfBuckets, const ClippingAssembly& globalClippingAssembly);
	bool findNeighborsBuckets(const glm::dvec3& globalSeedPoint, const double& radius, std::vector<std::vector<glm::dvec3>>& neighborList, const int& numberOfBuckets, const ClippingAssembly& globalClippingAssembly);
    bool findNeighbors(const glm::dvec3& globalSeedPoint, const double& radius, std::vector<glm::dvec3>& neighborList, const ClippingAssembly& clippingAssembly);
    bool findNeighborsTowardsPoint(const glm::dvec3& globalSeedPoint, const glm::dvec3& targetPoint, const double& radius, std::vector<glm::dvec3>& neighborList, const ClippingAssembly& clippingAssembly);
    void createBoxFromLeaves(const std::vector<uint32_t>& leafList, std::vector<glm::dquat>& rotations, std::vector<glm::dvec3>& positions, std::vector<double>& scales);
	double distancePointFromCell(const glm::dvec3& localPoint, const uint32_t& cellId);
	bool findNeighborsBucketsTest(const glm::dvec3& globalSeedPoint, const double& radius, std::vector<std::vector<glm::dvec3>>& neighborList, const int& numberOfBuckets, const ClippingAssembly& globalClippingAssembly, std::vector<glm::dquat>& rotations, std::vector<glm::dvec3>& positions, std::vector<double>& scales);

    bool nearestNeighbor(const glm::dvec3& globalPoint, glm::dvec3& result);
	bool nearestNeighbor2(const glm::dvec3& globalPoint, glm::dvec3& result, const ClippingAssembly& clippingBoxes);
    bool nearestBendNeighbor(const glm::dvec3& globalPoint, glm::dvec3& result, const glm::dvec3& normalVector);

    void getPointsInLeaf(const glm::dvec3& globalPoint, std::vector<glm::dvec3>& retPoints);
	void getPointsInLeafList(const std::vector<uint32_t>& leavesId, std::vector<glm::dvec3>& retPoints);
    std::vector<glm::dvec3> getPointsInBox(const glm::dvec3& seedPoint, const glm::dvec3& beamDir, const glm::dvec3& orthoDir, const glm::dvec3& normalDir, const std::vector<std::vector<double>>& xyRange, const double& heightMax);
    std::vector<glm::dvec3> getPointsInGeometricBox(const GeometricBox& box, const ClippingAssembly& clippingAssembly);
    GeometricBox createGeometricBoxFromLeaf(const uint32_t& leafId, const ClippingAssembly& clippingAssembly);
    bool updateVoxelGrid(const ClippingAssembly& clippingAssembly, VoxelGrid& voxelGrid, const int& scanNumber);
    bool updateVoxelGridWithLeaf(VoxelGrid& voxelGrid, const uint32_t& leafId, const int& scanNumber, int& numberOfambiguousVoxels, const ClippingAssembly& clippingAssembly);
    bool updateVoxelGridWithPoint(VoxelGrid& voxelGrid, const glm::dvec3& point, const int& scanNumber, const ClippingAssembly& clippingAssembly);
    bool classifyVoxelsByScan(VoxelGrid& voxelGrid, const ClippingAssembly& clippingAssembly, std::vector<std::vector<std::vector<bool>>>& dynamicVoxels, const int& scanNumber);
    int classifyVoxelsWithLeaf(const uint32_t& leafId, const VoxelGrid& voxelGrid, std::vector<std::vector<std::vector<bool>>>& dynamicVoxels, std::vector<std::vector<std::vector<bool>>>& overrideDynamic, const int& scanNumber, const glm::dvec3& scanOrigin, const ClippingAssembly& clippingAssembly);
    void rayTraversal(const glm::dvec3& targetPoint, const VoxelGrid& voxelGrid, std::vector<std::vector<std::vector<bool>>>& dynamicVoxels, std::vector<std::vector<std::vector<bool>>>& overrideDynamic, const int& scanNumber, const ClippingAssembly& clippingAssembly);
    int countPointsInBox(const GeometricBox& box, const ClippingAssembly& clippingAssembly);
    int countPointsInBoxByLeaf(const uint32_t& leafId, const GeometricBox& box, const ClippingAssembly& clippingAssembly);

    bool updateOctreeVoxelGrid(OctreeVoxelGrid& octreeVoxelGrid, const int& scanNumber);
    bool updateOctreeVoxelGridWithLeaf(OctreeVoxelGrid& octreeVoxelGrid, const uint32_t& leafId, const int& scanNumber, int& numberOfambiguousVoxels);
    bool updateOctreeVoxelGridWithPoint(OctreeVoxelGrid& octreeVoxelGrid, const glm::dvec3& point, const int& scanNumber);
    bool classifyOctreeVoxelsByScan(OctreeVoxelGrid& octreeVoxelGrid, std::vector<std::vector<std::vector<bool>>>& dynamicVoxels, const int& scanNumber);
    void classifyOctreeVoxelsWithLeaf(const uint32_t& leafId, const OctreeVoxelGrid& octreeVoxelGrid, std::vector<std::vector<std::vector<bool>>>& dynamicVoxels, std::vector<std::vector<std::vector<bool>>>& overrideDynamic, const int& scanNumber, const glm::dvec3& scanOrigin);
    void octreeRayTraversal(const glm::dvec3& targetPoint, const OctreeVoxelGrid& octreeVoxelGrid, std::vector<std::vector<std::vector<bool>>>& dynamicVoxels, std::vector<std::vector<std::vector<bool>>>& overrideDynamic, const int& scanNumber);


    // Sampling functions
    void samplePointsByStep(float samplingStep, const std::vector<uint32_t>& leavesId, std::vector<glm::dvec3>& sampledPoints);
    void samplePointsByQuota(size_t quotaMax, const std::vector<uint32_t>& leavesId, std::vector<glm::dvec3>& sampledPoints);

	bool pointToCell(const glm::dvec3& localSeedPoint, uint32_t& resultCellId) const;
    glm::dvec3 getLocalCoord(const glm::dvec3& globalCoord) const;

protected:
    int updateRay(glm::dvec3& localRay, glm::dvec3& localRayOrigin, const double& rootSize); //returns a=4x+2y+z, where s=0 iff ray.s>0 and 1 otherwise
	void traceRay(const double& tx0, const double& ty0, const double& tz0, const double& tx1, const double& ty1, const double& tz1, const uint32_t& cellId, const int& rayModifier, std::vector<uint32_t>& leafList, const ClippingAssembly& clippingAssembly, const glm::dvec3& localRayOrigin);	
	glm::dvec3 findBestPointIterative(const std::vector<uint32_t>& leafList, const glm::dvec3& rayDirection, const glm::dvec3& rayOrigin, const double& cosAngleThreshold, const double& rayRadius, const ClippingAssembly& localClippingAssembly, const bool& isOrtho, bool& success);

    glm::dvec3 pointInNeighborCell(const glm::dvec3& point);

    glm::dvec3 getGlobalCoord(const glm::dvec3& localCoord) const;
    // TODO - Return a reference
    glm::dmat4 getMatrixToLocal() const;

protected:
    std::filesystem::path m_path;
    tls::FileHeader m_fileHeader;
    tls::ScanHeader m_scanHeader;

    uint64_t m_fileSize;
    uint64_t m_pointDataOffset;
    uint64_t m_instanceDataOffset;
    bool m_deleteFileWhenDestroyed;

    glm::dmat3 m_rotationToGlobal;
    glm::dvec3 m_translationToGlobal;
    glm::dmat4 m_matrixToGlobal;
    glm::dmat4 m_matrixToLocal;

    std::atomic<uint32_t> m_lastFrameUse;

    // Init in constructor
    SimpleBuffer m_instanceBuffer;
    SmartBuffer* m_pCellBuffers;  // size = OctreeBase::m_cellCount

    // For Streaming
    std::mutex m_streamMutex;
    std::vector<uint32_t> m_missingCells;

    std::mutex m_workloadMutex;
    std::vector<SmartBufferWorkload*> m_waitingWorkload;

    // Decoding stats
    size_t t_ptsDecodedCount = 0;
    std::chrono::steady_clock::time_point t_timer;
    double t_time = 0.0;
};

#endif // !EMBEDDED_SCAN_H_