#ifndef I_PANEL_H
#define I_PANEL_H

class IGuiData;

class IPanel
{
public:
	/*! S'ex�cute � chaque fois qu'un message IGuiData d'un type �cout� par le IPanel (via la m�thode DataDispatcher::registerObserverOnKey)
		est sortie de la file de message IGuiData du DataDispatcher.
		
		Il permet donc de faire une op�ration en fonction du IGuiData _keyValue_ re�u*/
    virtual void informData(IGuiData *keyValue) = 0;
};

#endif