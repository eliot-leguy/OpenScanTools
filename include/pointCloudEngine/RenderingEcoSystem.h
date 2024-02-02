#ifndef RENDERING_ECO_SYSTEM_H
#define RENDERING_ECO_SYSTEM_H

// Robin Kervadec - 16/06/2023
// 
// +++++ Probl�matique +++++
//  * L'affichage du nuage de point par le GPU est tr�s couteux en temps et en �nergie.
//  * On souhaite r�duire au maximum les op�rations qui aboutirait au m�me rendu que celui pr�c�dent.
//

// +++++ Analyse +++++
// Voici la liste d�taill�e des param�tres ayant une influence sur le rendu du nuage de point
//  * Viewport : taille, 
//  * Mode & Param�tres de rendu : taille de point, transparence, BCLS, normals, couleur pure, rampe
//  * Camera : position, fov, near / far
//  * Scan / PCO : position, couleur,
//  * Clipping{active} : position, scale / size, user scale
//  * Polyline{clipping::active} : nombre de mesure
//  * Objet{ ramp } : position,
//  * Suppression d'un objet : Scan, PCO, Clipping {active}, Objet {ramp}
//
// +++++ Syst�me centralis� +++++
//  * Avant la refonte du mod�le de donn�e, tout changement aux donn�es �tait centralis� par le
//    graph de scene. Ainsi, on pouvait d�terminer si le renomage d'un tag, le d�placement d'un
//    cylindre ou le changement de couleur d'un scan �tait susceptible de modifier le rendu final.
//  * En outre, les param�tres d'affichage de li� � la vue (mode de rendu, taille de point,
//    transparence, r�solution, position de camera, etc) �taient stock�s par le viewport, le
//    rendering engine et la cam�ra. Leur changement �tait traqu� par leur interm�diaire.
//
// +++++ Syst�me individuel +++++
//  * Traquer la modification de tous ces param�tres individuellement est tr�s difficile et
//    potentiellement tr�s couteuse. Il faut noter que nous n'avons plus comme avant d'observateur
//    central pour filtrer les changement.
//  * Un syst�me ou chaque objet tient compte de ses propres changement est complexe � d�velopper, 
//    prompt aux erreurs et tr�s difficile � maintenir.
//  * Un tel syst�me n'est pas adapt� en particulier pour �valuer l'effet d'une suppression d'objet 
//    puisque l'objet n'est plus l� pour attester de sa modification.

#include "models/data/clipping/ClippingGeometry.h"
#include "models/3d/PointCloudDrawData.h"
#include "models/3d/DisplayParameters.h"
#include "vulkan/vulkan.h"

class HashFrame
{
public:
    static uint64_t hashRenderingData(VkExtent2D viewportExtent, const glm::dmat4& VP, const ClippingAssembly& clipAssembly, const std::vector<PointCloudDrawData>& m_pcDrawData, const DisplayParameters& displayParams);

    static uint64_t hashRenderingData_v2(const glm::dmat4& VP, const ClippingAssembly& clipAssembly, const std::vector<PointCloudDrawData>& m_pcDrawData, const DisplayParameters& displayParams);
};


#endif