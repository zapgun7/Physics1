#ifndef _cPhysics_HG_
#define _cPhysics_HG_

// This manages all the physics stuff
#include "sPhsyicsProperties.h"
#include "../cVAOManager/cVAOManager.h"		// For the mesh indirect
#include <vector>
#include "../Other Graphics Stuff/cGraphicsMain.h"

class cPhysics
{
public:
	cPhysics();
	~cPhysics();

	void setVAOManager(cVAOManager* pTheMeshManager);

	// Once you pass this in, the cPhysics handles the lifetime
	// (i.e. IT will call delete)
	// When added, if a mesh is present, the drawPosition, etc. are set
	void AddShape(sPhsyicsProperties* pNewShape);
	void DeleteShape(int shapeID); // Deletes the physics shape
	std::vector< sPhsyicsProperties* > getPhysicsVec(void);
	void setGraphics(void); // Lazy

	void Update(double deltaTime);


	/// Updating values for the editor  //////  !!! These will only be called when the physics simulation is paused, and only from the editor!
	void setShapePos(glm::vec3 newPos, unsigned int ID);
	void setShapeOri(glm::vec3 newOri, unsigned int ID);
	void setPhysicsRunningState(bool isRunning); // Update whether the physics simulation is paused
	void deleteAllObjects(void); // Used in loading new scenes

	////////////////////////////////////////

	// returns NULL if not found
	sPhsyicsProperties* findShapeByUniqueID(unsigned int uniqueIDtoFind);
	sPhsyicsProperties* findShapeByFriendlyName(std::string friendlyNameToFind);

	// Vector is empty if none found
	std::vector<sPhsyicsProperties*> findShapesByType(sPhsyicsProperties::eShape shapeType);
	// Returns false if none found
	bool findShapesByType(sPhsyicsProperties::eShape shapeType, std::vector<sPhsyicsProperties*> &vecShapes);

	// There's a ray cast code in chapter 5 of Ericson's book.
	// What does this return? 
	// It could return all the things that the ray hit.
	// You could add some methods that only ray cast through specific objects (like only meshes)
	// You could also do something where it returns the "closest" object (maybe form the startXYZ)
	bool rayCast(glm::vec3 startXYZ, glm::vec3 endXYZ, std::vector<sPhsyicsProperties*>& vecObjectsHit);

private:
	std::vector< sPhsyicsProperties* > m_vec_pPhysicalProps;
	
	bool m_Sphere_Sphere_IntersectionTest(sPhsyicsProperties* pSphereA, sPhsyicsProperties* pShpereB);
	bool m_Sphere_Plane_IntersectionTest(sPhsyicsProperties* pSphere, sPhsyicsProperties* pPlane);
	bool m_Sphere_Triangle_IntersectionTest(sPhsyicsProperties* pSphere, sPhsyicsProperties* pTriangle);
	bool m_Sphere_AABB_IntersectionTest(sPhsyicsProperties* pSphere, sPhsyicsProperties* pAABB);
	bool m_Sphere_Capsule_IntersectionTest(sPhsyicsProperties* pSphere, sPhsyicsProperties* pCapsule);
	bool m_Sphere_TriMeshIndirect_IntersectionTest(sPhsyicsProperties* pSphere, sPhsyicsProperties* pTriMesh);
	bool m_Sphere_TriMeshLocal_IntersectionTest(sPhsyicsProperties* pSphere, sPhsyicsProperties* pTriMesh);


	// The basic tests
	bool m_TestSphereTriangle(float sphereRadius, glm::vec3 vert0, glm::vec3 vert1, glm::vec3 vert2, glm::vec3 sphereCentre);
	glm::vec3 m_ClosestPtPointTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c);

	cVAOManager* m_pMeshManager = NULL;
	cGraphicsMain* m_pGraphicsMain;

	bool m_IsRunning = false; // If physics updates should be calculated. Pausing good for moving stuff around in editor then resuming physics

	

	// this is a list of all the objects that collided this past frame
	struct sCollisionEvent
	{
		sPhsyicsProperties* pObjectA;		// Sphere
		sPhsyicsProperties* pObjectB;		// Triangle
		// .. And other information we might need
		glm::vec3 contactPoint;
		glm::vec3 velocityAtCollision;
		glm::vec3 reflectionNormal;

	};
	std::vector< sCollisionEvent > m_vecCollisionsThisFrame;

	bool m_CheckExistingCollision(sPhsyicsProperties* objA, sPhsyicsProperties* objB); // Returns sCollisionEvent object if the objects have collided before
};

#endif // _cPhysics_HG_