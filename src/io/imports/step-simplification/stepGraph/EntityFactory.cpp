/* Copyright (C) 2020 Antoine Garnier <antoine.garnier1@etu.univ-nantes.fr>
 * Copyright (C) 2020 Evan Gisdal <evan.gisdal@etu.univ-nantes.fr>
 * Copyright (C) 2020 Antoine Humbert <antoine.humbert@etu.univ-nantes.fr>
 *
 * This file is part of a transversal project made by students from the computer science department
 * of the Graduate School of Engineering of the University of Nantes. It was realized as an attempt
 * to solve the problem of the simplification of 3D models with the STEP format.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "io/imports/step-simplification/stepGraph/EntityFactory.h"

 #include "io/imports/step-simplification/stepGraph/Entity_Adv_B_Sh_Rep.h"
 #include "io/imports/step-simplification/stepGraph/Entity_Mech_D_Geo_P_Rep.h"
 #include "io/imports/step-simplification/stepGraph/Entity_Styled_Item.h"
 #include "io/imports/step-simplification/stepGraph/Entity_P_L_Ass.h"
 #include "io/imports/step-simplification/stepGraph/Entity_Sh_Rep_Rel.h"

Entity* EntityFactory::createEntity(unsigned int id, const std::string& name, const std::string& args) {
    if (name == "ADVANCED_BREP_SHAPE_REPRESENTATION") return new Entity_Adv_B_Sh_Rep(id, name, args);
    if (name == "MECHANICAL_DESIGN_GEOMETRIC_PRESENTATION_REPRESENTATION") return new Entity_Mech_D_Geo_P_Rep(id, name, args);
    if (name == "STYLED_ITEM") return new Entity_Styled_Item(id, name, args);
    if (name == "PRESENTATION_LAYER_ASSIGNMENT") return new Entity_P_L_Ass(id, name, args);
    if (name == "SHAPE_REPRESENTATION_RELATIONSHIP") return new Entity_Sh_Rep_Rel(id, name, args);

    return new Entity(id, name, args);
}
