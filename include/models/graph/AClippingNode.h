#ifndef A_CLIPPING_NODE_H
#define A_CLIPPING_NODE_H

#include "models/graph/AObjectNode.h"
#include "models/data/Clipping/ClippingData.h"
#include "models/data/Clipping/ClippingGeometry.h"

class AClippingNode : public AObjectNode, public ClippingData
{
public:
	AClippingNode(const AClippingNode& node);
	AClippingNode();

	virtual void setDefaultData(const Controller& controller) override;

	virtual void pushClippingGeometries(ClippingAssembly& clipAssembly, const TransformationModule& transfo) const;
	virtual void pushRampGeometries(std::vector<std::shared_ptr<IClippingGeometry>>& retGeom, const TransformationModule& transfo) const;

	virtual void setSelectedPhase(const std::wstring& phase);
	virtual std::wstring getSelectedPhase() const;
private:
	std::wstring m_phaseSelected=L"";
};
#endif