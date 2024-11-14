#ifndef PROJECT_TREE_PANEL_H
#define PROJECT_TREE_PANEL_H

#include "gui/IPanel.h"
#include "gui/IDataDispatcher.h"
#include "gui/widgets/TreeNodeSystem/TreeNodeFactory.h"
#include "gui/GuiData/GuiDataTree.h"
#include "gui/widgets/TreeNodeSystem/TreeModel.h"
#include "gui/widgets/TreeNodeSystem/TreeNode.h"

#include <QtWidgets/qtreeview.h>

#include <vector>

class ProjectTreePanel;

typedef void (ProjectTreePanel::* treeMethod)(IGuiData*);

/*! G�re l'UI de l'arborescence */
class ProjectTreePanel : public QTreeView, public IPanel
{
	Q_OBJECT

	friend class TreeNodeFactory;
public:

	ProjectTreePanel(IDataDispatcher& dataDispatcher, GraphManager& graphManager, float guiScale);
	~ProjectTreePanel();

	void informData(IGuiData* keyValue) override;

private:
	void blockAllSignals(bool block);

	/*! Mise � jour de l'UI de l'arborescence  */
	void actualizeNodes(IGuiData* data);
	void cleanTree(IGuiData* data);
	void selectItems(IGuiData* data);

    //void actualizeObjectNodes(std::unordered_set<SafePtr<AGraphNode>> datasToAdd, bool completeActualize);

	void updateSelection(const std::unordered_set<SafePtr<AGraphNode>>& datas);
	//void updateFilterNodes(std::unordered_set<SafePtr<AGraphNode>> datas);
	//void recRefreshCheckStateQtItem_ascendant(QStandardItem* node);
	void generateTreeModel();
	TreeNode* buildTreeModelBranch(const QString& name, TreeType type);
	
	/*! Rafra�chit l'affichage de l'objet dans l'arbre*/
    void applyWaitingRefresh();

    /*! Remove from treeId map
        Remove from xg::Guid  multimap
        Romove from DataNode parent
    */
    void removeTreeNode(TreeNode* node);

	void treeDragEvent(QDragEnterEvent* dEvent);
	void treeMoveEvent(QDragMoveEvent* mEvent);
	void treeDropEvent(QDropEvent* dEvent);

	/*! V�rifie si le noeud _origin_ peut �tre avoir comme parent _dest_ */
	bool checkDropAreaIsValid(TreeNode* dest, const SafePtr<AGraphNode>& dropData);

	void onExpand(const QModelIndex& ind);
	void onCollapse(const QModelIndex& ind);

public slots:
	void treeViewMoveProgress(const QModelIndex& doubleClickedInd);
    void treeSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
	void collapseChildren(const QModelIndex& collapsInd);
	/*! G�n�re le menu contextuel appara�sant selon le point QPoint p cliqu� */
	void showTreeMenu(QPoint p); // to nodes

	void createCluster();
	void exportScan();
	/*! Supprime les objets s�lectionn�s dans l'arbre */
	void deleteTreeElement();
	void multiChangeAttributes();
	void removeElemFromHierarchy(); 
	void disconnectPiping();
	void moveToItem();
	void enableClipping();
	void disableClipping();
	void disableAllClippings();
	void disableAllRamps();
	void externCliping();
	void internClipping();
	void pickItems();
	void dropManualItems();
	void dropElem(TreeNode* dest);
	void selectClusterChildrens();

	void onTreeDataChanged(QStandardItem* item);

private:

	/*! Red�finition de la m�thode Qt qui traite l'�venement de 'Drag' (correspond au d�but du Drag&Drop) */
    void dragEnterEvent(QDragEnterEvent* qevent) override;
	/*! Red�finition de la m�thode Qt qui traite l'�venement de 'DragMove' (se r�p�te entre un �v�nement de 'Drag' et 'Drop') */
    void dragMoveEvent(QDragMoveEvent* mevent) override;
	/*! Red�finition de la m�thode Qt qui traite l'�venement de 'Drop' (correspond � la fin du Drag&Drop) */
    void dropEvent(QDropEvent* qevent) override;

private:

	IDataDispatcher& m_dataDispatcher;
	GraphManager& m_graphManager;

	QStandardItem* m_highlightNode;
	QBrush m_lastHighLightBrush;

	TreeNode* m_lastTreeNodeSelected = nullptr;
	TreeNode* m_itemsNode = nullptr;

	std::list<SafePtr<AGraphNode>> m_dropList;
	std::unordered_set<SafePtr<AGraphNode>> m_pickedItems;
	QPoint m_lastPoint;

	/*! Arbre c�t� UI */
	TreeModel* m_model = nullptr;
	TreeNodeFactory* m_nodeFactory = nullptr;

    typedef void (ProjectTreePanel::*GuiDataFunction)(IGuiData*);
    inline void registerGuiDataFunction(guiDType type, GuiDataFunction fct)
    {
        m_dataDispatcher.registerObserverOnKey(this, type);
        m_methods.insert({ type, fct });
    };
	std::unordered_map<guiDType, treeMethod> m_methods;

	std::unordered_set<SafePtr<AGraphNode>> m_selectedNodes;

	std::unordered_map<TreeType, std::vector<TreeNode*>> m_rootNodes;
};

#endif
