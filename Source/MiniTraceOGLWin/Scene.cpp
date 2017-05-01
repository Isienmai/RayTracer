/*---------------------------------------------------------------------
*
* Copyright © 2015  Minsi Chen
* E-mail: m.chen@derby.ac.uk
*
* The source is written for the Graphics I and II modules. You are free
* to use and extend the functionality. The code provided here is functional
* however the author does not guarantee its performance.
---------------------------------------------------------------------*/
#include <math.h>

#include "Scene.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include "Cylinder.h"
#include "Point_Light.h"
#include "Directional_Light.h"
#include "Spot_Light.h"

Scene::Scene()
{
	InitDefaultScene();
}


Scene::~Scene()
{
	CleanupScene();
}

void Scene::InitDefaultScene()
{	
	//Create a box and its material
	Primitive* newobj = new Cylinder(Vector3(-2.0, 5.1, -8.0),10, 3.0);
	Material* newmat = new Material();
	//mat for the box
	newmat->SetAmbientColour(0.0, 0.0, 0.0);
	newmat->SetDiffuseColour(1.0, 0.0, 0.0);
	newmat->SetSpecularColour(1.0, 1.0, 1.0);
	newmat->SetSpecPower(20);
	newmat->SetTransparency(0);
	newmat->SetReflectivity(0);
	newobj->SetMaterial(newmat);
	m_sceneObjects.push_back(newobj);
	m_objectMaterials.push_back(newmat);
	
	//Create sphere 1 and its material
	newobj = new Sphere(3.0, 5, -3.5, 2.0); //sphere 1
	newmat = new Material();
	newmat->SetAmbientColour(0.0, 0.0, 0.0);
	newmat->SetDiffuseColour(0.0, 0.8, 0.0);
	newmat->SetSpecularColour(1.0, 1.0, 1.0);
	newmat->SetSpecPower(5);
	newmat->SetRefractInd(1.01);
	newmat->SetTransparency(0.9);
	newmat->SetReflectivity(0);
	newobj->SetMaterial(newmat);
	m_sceneObjects.push_back(newobj);
	m_objectMaterials.push_back(newmat);
	
	//Create sphere 2 and its material
	newobj = new Sphere(-4.0, 7, 3.5, 2.0); //sphere 2
	newmat = new Material();
	newmat->SetAmbientColour(0.0, 0.0, 0.0);
	newmat->SetDiffuseColour(0.0, 0.0, 0.9);
	newmat->SetSpecularColour(1.0, 1.0, 1.0);
	newmat->SetSpecPower(2);
	newmat->SetRefractInd(1.7);
	newmat->SetTransparency(0);
	newmat->SetReflectivity(0.9);
	newobj->SetMaterial(newmat);
	m_sceneObjects.push_back(newobj);
	m_objectMaterials.push_back(newmat);


	newobj = new Plane(); //an xz plane at the origin, floor
	static_cast<Plane*>(newobj)->SetPlane(Vector3(0.0, 1.0, 0.0), 0.0);
	newmat = new Material();
	newmat->SetAmbientColour(0.0, 0.0, 0.0);
	newmat->SetDiffuseColour(1.0, 0.0, 0.0);
	newmat->SetSpecularColour(0.0, 0.0, 0.0);
	newmat->SetSpecPower(10);
	newmat->SetCastShadow(false);
	newmat->SetReflectivity(0);
	newobj->SetMaterial(newmat);
	m_sceneObjects.push_back(newobj);
	m_objectMaterials.push_back(newmat);
	
	newobj = new Plane(); //an xz plane 40 units above, ceiling
	static_cast<Plane*>(newobj)->SetPlane(Vector3(0.0, -1.0, 0.0), -40.0);
	newobj->SetMaterial(newmat);
	m_sceneObjects.push_back(newobj);
	

	newobj = new Plane(); //an xy plane 40 units along -z axis, 
	static_cast<Plane*>(newobj)->SetPlane(Vector3(0.0, 0.0, 1.0), -40.0);
	newmat = new Material();
	newmat->SetAmbientColour(0.0, 0.0, 0.0);
	newmat->SetDiffuseColour(0.0, 1.0, 0.0);
	newmat->SetSpecularColour(0.0, 0.0, 0.0);
	newmat->SetSpecPower(10);
	newmat->SetCastShadow(false);
	newmat->SetReflectivity(0);
	newobj->SetMaterial(newmat);
	m_sceneObjects.push_back(newobj);
	m_objectMaterials.push_back(newmat);
	
	newobj = new Plane(); //an xy plane 40 units along the z axis
	static_cast<Plane*>(newobj)->SetPlane(Vector3(0.0, 0.0, -1.0), -40.0);
	newobj->SetMaterial(newmat);
	m_sceneObjects.push_back(newobj);
	
	newobj = new Plane(); //an yz plane 20 units along -x axis
	static_cast<Plane*>(newobj)->SetPlane(Vector3(1.0, 0.0, 0.0), -20.0);
	newmat = new Material();
	newmat->SetAmbientColour(0.0, 0.0, 0.0);
	newmat->SetDiffuseColour(0.0, 0.0, 1.0);
	newmat->SetSpecularColour(0.0, 0.0, 0.0);
	newmat->SetSpecPower(10);
	newmat->SetCastShadow(false);
	newmat->SetReflectivity(0);
	newobj->SetMaterial(newmat);
	m_sceneObjects.push_back(newobj);
	m_objectMaterials.push_back(newmat);

	newobj = new Plane(); //an yz plane 20 units along +x axis
	static_cast<Plane*>(newobj)->SetPlane(Vector3(-1.0, 0.0, 0.0), -20.0);
	newobj->SetMaterial(newmat);
	m_sceneObjects.push_back(newobj);

	//Create Point Light
	Light *newlight = new Point_Light();
	newlight->SetLightPosition(0, 10.0, 10);
	m_lights.push_back(newlight);

	//Create spot light
	Light *newlight2 = new Spot_Light();
	newlight2->SetLightPosition(-9.0, 15.0, 12.0);
	newlight2->SetLightDirection(3.0, -5.0, -4);
	//m_lights.push_back(newlight2);

	//Create directional light
	Light *newlight3 = new Directional_Light();
	//newlight3->SetLightPosition(0.0, 13.0, 0.0);
	newlight3->SetLightDirection(3.0, -5.0, -3);
	//m_lights.push_back(newlight3);

	//A blue background
	m_background.red = 0.25;
	m_background.green = 0.6;
	m_background.blue = 1.0;

	//default scene width and height;
	m_sceneWidth = 1.33333333;
	m_sceneHeight = 1.0;

	//default camera position and look at
	m_activeCamera.SetPositionAndLookAt(Vector3(3.0, 10.0, 13.0), Vector3(0.0, 9.0, 0.0));
}

void Scene::CleanupScene()
{
	//Cleanup object list
	std::vector<Primitive*>::iterator prim_iter = m_sceneObjects.begin();

	while(prim_iter != m_sceneObjects.end())
	{
		delete *prim_iter;
		prim_iter++;
	}

	m_sceneObjects.clear();

	//Cleanup material list
	std::vector<Material*>::iterator mat_iter = m_objectMaterials.begin();

	while (mat_iter != m_objectMaterials.end())
	{
		delete *mat_iter;
		mat_iter++;
	}
	m_objectMaterials.clear();

	//cleanup light list
	std::vector<Light*>::iterator lit_iter = m_lights.begin();

	while (lit_iter != m_lights.end())
	{
		delete *lit_iter;
		lit_iter++;
	}

	m_lights.clear();
}

RayHitResult Scene::IntersectByRay(Ray& ray)
{
	RayHitResult result = Ray::s_defaultHitResult;
	
	std::vector<Primitive*>::iterator prim_iter = m_sceneObjects.begin();
	
	while (prim_iter != m_sceneObjects.end())
	{
		RayHitResult current;

		current = (*prim_iter)->IntersectByRay(ray);

		if (current.t > 0.0 && current.t < result.t)
		{
			result = current;
		}

		prim_iter++;
	}	

	return result;
}

Colour Scene::IntersectByShadowRay(Ray& ray)
{
	Colour toReturn = Colour();

	std::vector<Primitive*>::iterator prim_iter = m_sceneObjects.begin();
	
	while (prim_iter != m_sceneObjects.end())
	{
		RayHitResult current = Ray::s_defaultHitResult;
		bool castShadow = (*prim_iter)->GetMaterial()->CastShadow();

		current = (*prim_iter)->IntersectByRay(ray);

		//if the object casts a shadow, the t is valid, and the intersection point is not behind the light source
		if (castShadow && current.t > 0.0  && current.t < Ray::s_defaultHitResult.t && (ray.GetRay().Norm() >= (current.point - ray.GetRayStart()).Norm()))
		{
			toReturn += 1 - ((Primitive*)current.data)->GetMaterial()->GetTransparency();

			//The following adds colour to the shadow according to the colour and transparency of the material casting it
			Colour reverseMatCol;
			reverseMatCol.red = 1 - ((Primitive*)current.data)->GetMaterial()->GetDiffuseColour().red;
			reverseMatCol.blue = 1 - ((Primitive*)current.data)->GetMaterial()->GetDiffuseColour().blue;
			reverseMatCol.green = 1 - ((Primitive*)current.data)->GetMaterial()->GetDiffuseColour().green;
			reverseMatCol = reverseMatCol * (1 - ((Primitive*)current.data)->GetMaterial()->GetTransparency());			
			toReturn += reverseMatCol;
		}

		prim_iter++;
	}

	//clamp the shadow's darkness
	toReturn.red = fmin(toReturn.red, 1);
	toReturn.green = fmin(toReturn.green, 1);
	toReturn.blue = fmin(toReturn.blue, 1);

	return toReturn;
}
