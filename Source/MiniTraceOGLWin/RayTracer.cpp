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

#ifdef WIN32
#include <Windows.h>
#include <gl/GL.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#endif

#include "RayTracer.h"
#include "Ray.h"
#include "Scene.h"
#include "Camera.h"

RayTracer::RayTracer()
{
	m_buffHeight = m_buffWidth = 0.0;
	m_renderCount = 0;
	SetTraceLevel(5);
	m_traceflag = (TraceFlag)(TRACE_AMBIENT | TRACE_DIFFUSE_AND_SPEC |
		TRACE_SHADOW | TRACE_REFLECTION | TRACE_REFRACTION | !TRACE_ORTHO);
}

RayTracer::RayTracer(int Width, int Height)
{
	m_buffWidth = Width;
	m_buffHeight = Height;
	m_renderCount = 0;
	SetTraceLevel(5);
	
	m_traceflag = (TraceFlag)(TRACE_AMBIENT | TRACE_DIFFUSE_AND_SPEC |
		TRACE_SHADOW | TRACE_REFLECTION | TRACE_REFRACTION);
}

RayTracer::~RayTracer()
{

}

void RayTracer::DoRayTrace( Scene* pScene )
{
	Camera* cam = pScene->GetSceneCamera();
	
	Vector3 camRightVector = cam->GetRightVector();
	Vector3 camUpVector = cam->GetUpVector();
	Vector3 camViewVector = cam->GetViewVector();
	Vector3 centre = cam->GetViewCentre();
	Vector3 camPosition = cam->GetPosition();

	double sceneWidth;
	double sceneHeight;

	if (m_traceflag & TRACE_ORTHO)
	{
		sceneWidth = pScene->GetSceneWidth() * 20;
		sceneHeight = pScene->GetSceneHeight() *20;
	}
	else
	{
		sceneWidth = pScene->GetSceneWidth();
		sceneHeight = pScene->GetSceneHeight();
	}

	double pixelDX = sceneWidth / m_buffWidth;
	double pixelDY = sceneHeight / m_buffHeight;
	
	int total = m_buffHeight*m_buffWidth;
	int done_count = 0;
	
	Vector3 start;

	start[0] = centre[0] - ((sceneWidth * camRightVector[0])
		+ (sceneHeight * camUpVector[0])) / 2.0;
	start[1] = centre[1] - ((sceneWidth * camRightVector[1])
		+ (sceneHeight * camUpVector[1])) / 2.0;
	start[2] = centre[2] - ((sceneWidth * camRightVector[2])
		+ (sceneHeight * camUpVector[2])) / 2.0;
	
	Colour scenebg = pScene->GetBackgroundColour();

	if (m_renderCount == 0)
	{
		fprintf(stdout, "Trace start.\n");

		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);

		for (int i = 0; i < m_buffHeight; i++) {
			for (int j = 0; j < m_buffWidth; j++) {

				//calculate the metric size of a pixel in the view plane (e.g. framebuffer)
				Vector3 pixel;
				
				pixel[0] = start[0] + (i + 0.5) * camUpVector[0] * pixelDY
					+ (j + 0.5) * camRightVector[0] * pixelDX;
				pixel[1] = start[1] + (i + 0.5) * camUpVector[1] * pixelDY
					+ (j + 0.5) * camRightVector[1] * pixelDX;
				pixel[2] = start[2] + (i + 0.5) * camUpVector[2] * pixelDY
					+ (j + 0.5) * camRightVector[2] * pixelDX;

				/*
				* setup view ray
				* In perspective projection, each view ray originates from the eye (camera) position 
				* and pierces through a pixel in the view plane
				*
				* TODO: For a little extra credit, set up the view rays to produce orthographic projection
				*/
				Ray viewray;
				
				if (m_traceflag & TRACE_ORTHO)
				{
					//orthographic
					viewray.SetRay(pixel, camViewVector.Normalise());
				}
				else
				{
					//perspective
					viewray.SetRay(camPosition, (pixel - camPosition).Normalise());
				}

				//trace the scene using the view ray
				//the default colour is the background colour, unless something is hit along the way
				Colour colour = this->TraceScene(pScene, viewray, scenebg, m_traceLevel);

				/*
				* The only OpenGL code we need
				* Draw the pixel as a coloured rectangle
				*/
				glColor3f(colour.red, colour.green, colour.blue);
				glRecti(j, i, j + 1, i + 1);
			}
			glFlush();
		}

		fprintf(stdout, "Done!!!\n");
		m_renderCount++;
	}
	glFlush();
}

Colour RayTracer::TraceScene(Scene* pScene, Ray& ray, Colour incolour, int tracelevel)
{
	RayHitResult result;
	Colour outcolour = incolour;
	std::vector<Light*>* light_list = pScene->GetLightList();

	if (tracelevel <= 0) // reach the MAX depth of the recursion.
	{
		return outcolour;
	}

	result = pScene->IntersectByRay(ray);

	if (result.data) //the ray has hit something
	{

		Vector3 start = ray.GetRayStart();
		
		Colour reflectedColour = Colour(), refractedColour = Colour(), shadowColour = Colour();
		Material* collidedMaterial = ((Primitive*)result.data)->GetMaterial();

		outcolour = CalculateLighting(light_list,
			&start,
			&result);
		
		if(m_traceflag & TRACE_REFLECTION)
		{
			//Only consider reflection for spheres and boxes
			if (((Primitive*)result.data)->m_primtype == Primitive::PRIMTYPE_Sphere ||
				((Primitive*)result.data)->m_primtype == Primitive::PRIMTYPE_Box ||
				((Primitive*)result.data)->m_primtype == Primitive::PRIMTYPE_Cylinder)
			{
				reflectedColour.red = 1;
				reflectedColour.green = 1;
				reflectedColour.blue = 1;

				Ray reflectionRay;
				reflectionRay.SetRay(result.point, ray.GetRay().Reflect(result.normal));

				//Recursively call TraceScene with the reflection ray
				reflectedColour *= TraceScene(pScene, reflectionRay, incolour, tracelevel - 1) * collidedMaterial->GetReflectivity();
			}
		}

		if (m_traceflag & TRACE_REFRACTION)
		{
			//Only consider refraction for spheres, boxes and cylinders
			if (((Primitive*)result.data)->m_primtype == Primitive::PRIMTYPE_Sphere ||
				((Primitive*)result.data)->m_primtype == Primitive::PRIMTYPE_Box||
				((Primitive*)result.data)->m_primtype == Primitive::PRIMTYPE_Cylinder)
			{
				refractedColour.red = 1;
				refractedColour.green = 1;
				refractedColour.blue = 1;

				Ray refractionRay;
				Vector3 refVec = ray.GetRay().Refract(result.normal, collidedMaterial->GetRefractInd());
				refractionRay.SetRay(result.point + refVec * 0.00001, refVec);

				//Recursively call TraceScene with the reflection ray
				refractedColour *= TraceScene(pScene, refractionRay, incolour, tracelevel - 1) * collidedMaterial->GetTransparency();
			}
		}
		
		//////Check if this is in shadow
		if ((tracelevel != 1) && (m_traceflag & TRACE_SHADOW))
		{
			std::vector<Light*>::iterator lit_iter = light_list->begin();

			//TODO: Calculate the shadow ray using the current intersection result and the light position
			Ray shadowRay;
			Vector3 lightVector;
			double shadowMultiplier = 0;

			while (lit_iter != light_list->end())
			{
				lightVector = (*lit_iter)->GetPointToLightVector(result.point);
				shadowRay.SetRay(result.point + (lightVector.Normalise() * 0.000000001), lightVector);
				
				Colour totalTransparency = pScene->IntersectByShadowRay(shadowRay);
				if (totalTransparency != Colour())
				{
					if (m_traceflag & TRACE_REFRACTION)
					{
						shadowColour += outcolour * totalTransparency;
					}
					else
					{
						shadowColour += 1;
					}
				}
				lit_iter++;
			}
			shadowColour = shadowColour / max(1, lit_iter - light_list->begin());
		}

		//Combine the returned colours with the current surface colour 
		outcolour += reflectedColour;
		outcolour += refractedColour;
		outcolour += shadowColour*-1;
	}
		
	return outcolour;
}

Colour RayTracer::CalculateLighting(std::vector<Light*>* lights, Vector3* campos, RayHitResult* hitresult)
{
	Colour outcolour = Colour();
	std::vector<Light*>::iterator lit_iter = lights->begin();

	//Retrive the material for the intersected primitive
	Primitive* prim = (Primitive*)hitresult->data;
	Material* mat = prim->GetMaterial();

	Colour tempDiffuse = mat->GetDiffuseColour();
		
	//This is a hack to set a checker pattern on the planes
	if (((Primitive*)hitresult->data)->m_primtype == Primitive::PRIMTYPE_Plane)
	{
		Vector3 normal = hitresult->normal;

		int dx = 0;
		int dy = 0;
		int dz = 0;

		//only react to an axis if the plane travels along it
		if (abs(normal[0]) < 0.001)
		{
			dx = (hitresult->point[0] / 2.0);
		}
		if (abs(normal[1]) < 0.001)
		{
			dy = (hitresult->point[1] / 2.0);
		}
		if (abs(normal[2]) < 0.001)
		{
			dz = (hitresult->point[2] / 2.0);
		}

		if (dx % 2 || dy % 2 || dz % 2)
		{
			tempDiffuse.red = 1.0;
			tempDiffuse.green = 1.0;
			tempDiffuse.blue = 1.0;
		}
	}

	////Go through all the light sources in the scene
	//and calculate the lighting at the intersection point
	if (m_traceflag & TRACE_DIFFUSE_AND_SPEC)
	{
		while (lit_iter != lights->end())
		{
			Colour ambient = Colour(), diffuse = Colour(), specular = Colour();
			Vector3 light_pos = (*lit_iter)->GetLightPosition();  //position of the light source
			Vector3 normal = hitresult->normal; //surface normal at intersection
			Vector3 surface_point = hitresult->point; //location of the intersection on the surface
			Colour lightColour = (*lit_iter)->GetLightColour(surface_point);//Get the colour/intensity of the light
			
			//calculate the ambient
			ambient = mat->GetAmbientColour() * lightColour;

			//get the light and view unit vectors
			Vector3 lightVector = (*lit_iter)->GetPointToLightVector(surface_point).Normalise();
			Vector3 e = (*campos - surface_point).Normalise();

			float angle = normal.DotProduct(lightVector);
			if (angle < 0)
			{
				angle = 0;
			}

			diffuse = tempDiffuse * lightColour * angle;
			

			//get the reflection vector
			Vector3 r = lightVector.Reflect(normal);
			r = (lightVector - r) * 0.5;

			//calculate the angle
			angle = e.DotProduct(r);				
			if (angle < 0)
			{
				angle = 0;
			}

			//calculate the specular component
			specular = mat->GetSpecularColour() * lightColour * pow(angle, mat->GetSpecPower());
			

			lit_iter++;

			outcolour += ambient;
			outcolour += diffuse;
			outcolour += specular;
		}	

		// 3. store the result in outcolour
		//outcolour /= lit_iter - lights->begin();
	}
	return outcolour * (1 - mat->GetTransparency());
}
