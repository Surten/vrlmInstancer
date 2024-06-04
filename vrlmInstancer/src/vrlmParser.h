#pragma once

#include <fstream>
#include <string>
#include "baseNode.h"

/// <summary>
/// Class responsible for importing a VRML file
/// Takes references for arrays of different nodes and geometries 
///	from scene class and fills them up with data inside the loaded file
/// </summary>
class VrmlParser {

private:
	std::ifstream vrlmFile;
	std::string str;
	float n;


private:

	/// <summary>
	/// Skips everything after the comment symbol until next line
	/// </summary>
	void skipComments();

	/// <summary>
	/// Reads next symbol and parses it to number if needed
	/// uses the >> operator to load, so needs spaces between symbols, which VRML supports
	/// </summary>
	void readSymbol();

	/// <summary>
	/// Parse different constructs that use the DEF keyword
	/// </summary>
	/// <param name="parent"> The parent Node, we only use Transform nodes as parents in this implemetation</param>
	void parseDEF(TransformNode* parent);

	/// <summary>
	/// Parse a transform node
	/// </summary>
	/// <param name="transformNode">The node to be filled</param>
	void parseTransformNode(TransformNode* transformNode);

	/// <summary>
	/// Parse children of a Transform node
	/// </summary>
	/// <param name="transformNode">This node, giving reference to itself for future children to use</param>
	void parseChildren(TransformNode* parent);

	/// <summary>
	/// Used to parse Root nodes in a while loop
	/// </summary>
	void parseNextNode();

	/// <summary>
	/// Parse the Shape node and all of its insides
	/// </summary>
	/// <param name="parent"> Parent Transform node</param>
	void parseShape(TransformNode* parent);

	/// <summary>
	/// Parse Apperarance of a Shape node, calls parsing of material and texture url
	/// </summary>
	/// <param name="shapeNode">The Shape node</param>
	void parseApperance(ShapeNode* shapeNode);

	/// <summary>
	/// Goes through and parses the entire geometry, filling the created Geometry class in the process
	/// </summary>
	/// <param name="shapeNode">Shape Node gets reference to the created geometry here</param>
	void parseGeometry(ShapeNode* shapeNode);

	/// <summary>
	/// Part of parseAppearance
	/// </summary>
	/// <param name="shapeNode">The Shape node</param>
	void parseMaterial(ShapeNode* shapeNode);

	/// <summary>
	/// Part of parseAppearance
	/// </summary>
	/// <param name="shapeNode">The Shape node</param>
	void parseTexture(ShapeNode* shapeNode);

	/// <summary>
	/// Part of parseGeometry, parses Color
	/// </summary>
	/// <param name="shapeNode">The Shape node</param>
	void parseColor(ShapeNode* shapeNode);

	/// <summary>
	/// Part of parseGeometry, parses Vertices
	/// </summary>
	/// <param name="shapeNode">The Shape node</param>
	void parseCoords(ShapeNode* shapeNode);

	/// <summary>
	/// Part of parseGeometry, parses texture vertices
	/// </summary>
	/// <param name="shapeNode">The Shape node</param>
	void parseTexCoords(ShapeNode* shapeNode);

	/// <summary>
    /// Part of parseGeometry, parses color indices
    /// </summary>
    /// <param name="shapeNode">The Shape node</param>
	void parseColorIndex(ShapeNode* shapeNode);
	
	/// <summary>
    /// Part of parseGeometry, parses indices
    /// </summary>
    /// <param name="shapeNode">The Shape node</param>
	void parseCoordIndex(ShapeNode* shapeNode);

	/// <summary>
	/// Part of parseGeometry, parses texture indices
	/// </summary>
	/// <param name="shapeNode">The Shape node</param>
	void parseTexCoordIndex(ShapeNode* shapeNode);

	/// <summary>
	/// Parses SpotLight
	/// </summary>
	/// <param name="light">the Node to fill with data</param>
	void parseSpotLight(LightNode* light);

	/// <summary>
	/// Parses ViewPoint
	/// </summary>
	/// <param name="viewPointNode">the Node to fill with data</param>
	void parseViewPoint(ViewPointNode* viewPointNode);

public:
	bool lastWasNumber;

	std::vector<BaseNode*> * AllNodes;
	std::vector<BaseNode*> * RootNodes;
	std::vector<ShapeNode*> * ShapeNodes;
	std::vector<Geometry*> * geometries;
	std::vector<LightNode*> * lights;

public:
	VrmlParser(std::vector<BaseNode*> *AllNodes, std::vector<BaseNode*> *RootNodes,
		std::vector<ShapeNode*> *ShapeNodes, std::vector<Geometry*>* geometries, std::vector<LightNode*>* lights);

	/// <summary>
	/// Loads the given file
	/// </summary>
	/// <param name="vrlmFileName"> file path and name </param>
	/// <returns> true if loaded successfully </returns>
	bool parseFile(const char* vrlmFileName);
};



