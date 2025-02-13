#ifndef CONTROL_SCAN_EDITION_H
#define CONTROL_SCAN_EDITION_H

#include "controller/controls/AEditionControl.h"
#include "tls_def.h"
#include "utils/safe_ptr.h"

class ScanNode;
class APointCloudNode;

namespace control
{
	namespace scanEdition
	{
		class SetClippable : public ATEditionControl<APointCloudNode, bool> 
		{
		public:
			SetClippable(SafePtr<APointCloudNode> toEditData, bool clippable);
			SetClippable() = delete;
			~SetClippable();
			ControlType getType() const override;
		};

		class RandomScansColors : public AControl
		{
		public:
			RandomScansColors();
			~RandomScansColors();
			void doFunction(Controller& controller) override;
			bool canUndo() const override;
			void undoFunction(Controller& controller) override;
			ControlType getType() const override;
		};
	}
}

#endif // !CONTROLSCANEDITION_H_