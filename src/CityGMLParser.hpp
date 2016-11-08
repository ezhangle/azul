// azul
// Copyright © 2016 Ken Arroyo Ohori
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef CityGMLParser_hpp
#define CityGMLParser_hpp

#include <iostream>
#include <sstream>
#include <list>
#include <vector>
#include <map>
#include <limits>

#include <pugixml.hpp>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/linear_least_squares_fitting_3.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef CGAL::Exact_predicates_tag Tag;
typedef CGAL::Triangulation_vertex_base_2<Kernel> VertexBase;
typedef CGAL::Constrained_triangulation_face_base_2<Kernel> FaceBase;
typedef CGAL::Triangulation_face_base_with_info_2<std::pair<bool, bool>, Kernel, FaceBase> FaceBaseWithInfo;
typedef CGAL::Triangulation_data_structure_2<VertexBase, FaceBaseWithInfo> TriangulationDataStructure;
typedef CGAL::Constrained_Delaunay_triangulation_2<Kernel, TriangulationDataStructure, Tag> Triangulation;

struct CityGMLPoint {
  float coordinates[3];
};

struct CityGMLRing {
  std::list<CityGMLPoint> points;
};

struct CityGMLPolygon {
  CityGMLRing exteriorRing;
  std::list<CityGMLRing> interiorRings;
};

struct CityGMLObject {
  enum Type: unsigned int {Building = 1, Road = 2, WaterBody = 3, ReliefFeature = 4, PlantCover = 5, GenericCityObject = 6, Bridge = 7, LandUse = 8};
  Type type;
  std::string id;
  std::map<int, std::list<CityGMLPolygon>> polygonsByType;
  std::map<int, std::vector<float>> trianglesByType;
  std::vector<float> edges;
};

struct PointsWalker: pugi::xml_tree_walker {
  std::list<CityGMLPoint> points;
  virtual bool for_each(pugi::xml_node &node) {
    if (strcmp(node.name(), "gml:pos") == 0 ||
        strcmp(node.name(), "gml:posList") == 0) {
      //      std::cout << node.name() << " " << node.child_value() << std::endl;
      std::string coordinates(node.child_value());
      std::istringstream iss(coordinates);
      unsigned int currentCoordinate = 0;
      do {
        std::string substring;
        iss >> substring;
        if (substring.length() > 0) {
          if (currentCoordinate == 0) points.push_back(CityGMLPoint());
          points.back().coordinates[currentCoordinate] = std::stof(substring);
          currentCoordinate = (currentCoordinate+1)%3;
        }
      } while (iss);
      if (currentCoordinate != 0) {
        std::cout << "Wrong number of coordinates: not divisible by 3" << std::endl;
        points.pop_back();
      } //std::cout << "Created " << points.size() << " points" << std::endl;
    } return true;
  }
};

struct RingsWalker: pugi::xml_tree_walker {
  pugi::xml_node exteriorRing;
  std::list<pugi::xml_node> interiorRings;
  virtual bool for_each(pugi::xml_node &node) {
    if (strcmp(node.name(), "gml:exterior") == 0) {
      exteriorRing = node;
    } else if (strcmp(node.name(), "gml:interior") == 0) {
      interiorRings.push_back(node);
    } return true;
  }
};

struct PolygonsWalker: pugi::xml_tree_walker {
  std::map<int, std::list<pugi::xml_node>> polygonsByType;
  int inDefinedType = 0;  // 0 = undefined
  unsigned int depthToStop;
  virtual bool for_each(pugi::xml_node &node) {
    if (inDefinedType != 0 && depth() <= depthToStop) {
      inDefinedType = 0;
    } if (strcmp(node.name(), "bldg:RoofSurface") == 0) {
      inDefinedType = 1;
      depthToStop = depth();
    } else if (strcmp(node.name(), "gml:Polygon") == 0 ||
               strcmp(node.name(), "gml:Triangle") == 0) {
      if (polygonsByType.count(inDefinedType) == 0) polygonsByType[inDefinedType] = std::list<pugi::xml_node>();
      polygonsByType[inDefinedType].push_back(node);
    } return true;
  }
};

struct ObjectsWalker: pugi::xml_tree_walker {
  std::list<pugi::xml_node> objects;
  virtual bool for_each(pugi::xml_node &node) {
    if (strcmp(node.name(), "bldg:Building") == 0 ||
        strcmp(node.name(), "tran:Road") == 0 ||
        strcmp(node.name(), "dem:ReliefFeature") == 0 ||
        strcmp(node.name(), "wtr:WaterBody") == 0 ||
        strcmp(node.name(), "veg:PlantCover") == 0 ||
        strcmp(node.name(), "gen:GenericCityObject") == 0 ||
        strcmp(node.name(), "brg:Bridge") == 0 ||
        strcmp(node.name(), "luse:LandUse") == 0) {
      objects.push_back(node);
    } return true;
  }
};

class CityGMLParser {
public:
  std::list<CityGMLObject> objects;
  
  bool firstRing;
  float minCoordinates[3];
  float maxCoordinates[3];
  
  std::list<CityGMLObject>::const_iterator currentObject;
  std::map<int, std::vector<float>>::const_iterator currentTrianglesBuffer;
  
  CityGMLParser();
  void parse(const char *filePath);
  void clear();
  
  void parseObject(pugi::xml_node &node, CityGMLObject &object);
  void parsePolygon(pugi::xml_node &node, CityGMLPolygon &polygon);
  void parseRing(pugi::xml_node &node, CityGMLRing &ring);
  
  void centroidOf(CityGMLRing &ring, CityGMLPoint &centroid);
  void addTrianglesFromTheConstrainedTriangulationOfPolygon(CityGMLPolygon &polygon, std::vector<float> &triangles);
  void regenerateTrianglesFor(CityGMLObject &object);
  void regenerateEdgesFor(CityGMLObject &object);
  void regenerateGeometries();
};

#endif /* CityGMLParser_hpp */
