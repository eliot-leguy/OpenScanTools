#ifndef BEAM_BENDING_MEASURE_NODE_H
#define BEAM_BENDING_MEASURE_NODE_H

#include "models/graph/AObjectNode.h"
#include "models/data/BeamBendingMeasure/BeamBendingMeasureData.h"

class BeamBendingMeasureNode : public AObjectNode, public BeamBendingMeasureData
{
public:
    BeamBendingMeasureNode(const BeamBendingMeasureNode& node);
    BeamBendingMeasureNode();
    ~BeamBendingMeasureNode();

    void setColor(const Color32& color) override;
    ElementType getType() const override;
    TreeType getDefaultTreeType() const override;

    std::unordered_set<Selection> getAcceptableSelections(const ManipulationMode& mode) const override;
    std::unordered_set<ManipulationMode> getAcceptableManipulationModes() const override;


    // TODO - future marker interface


    virtual void setMaxBendingPos(const Pos3D& pos) override;
};

#endif
