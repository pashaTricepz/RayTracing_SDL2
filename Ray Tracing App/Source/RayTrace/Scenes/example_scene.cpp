#include "example_scene.h"

RT::ExampleScene::ExampleScene()
{
	SetupSceneObjects();
}

RT::ExampleScene::~ExampleScene()
{

}

void RT::ExampleScene::SetupSceneObjects()
{
	// **************************************************************************************
	// Configure the camera.
	// **************************************************************************************	
	m_camera.SetPosition({ 6.0, -10.0, -4.0 });
	m_camera.SetLookAt({ 0.0, 0.0, 0.5 });
	m_camera.SetUp({ 0.0, 0.0, 1.0 });
	m_camera.SetHorzSize(1.0);
	m_camera.SetLength(3.0);
	m_camera.SetAspect(16.0 / 9.0);
	m_camera.UpdateCameraGeometry();

	// **************************************************************************************
	// Setup ambient lightling.
	// **************************************************************************************		
	RT::MaterialBase::m_ambientColor = { 1.0, 1.0, 1.0 };
	RT::MaterialBase::m_ambientIntensity = 0.25;

	// **************************************************************************************
	// Create the color maps.
	// **************************************************************************************	
	auto woodMap = std::make_shared<RT::Texture::ColorMap>(RT::Texture::ColorMap());
	woodMap->SetStop(0.0, { 200.0 / 255.0, 150.0 / 255.0, 120.0 / 255.0, 1.0 });
	woodMap->SetStop(0.5, { 100.0 / 255.0, 50.0 / 255.0, 30.0 / 255.0, 1.0 });
	woodMap->SetStop(1.0, { 200.0 / 255.0, 150.0 / 255.0, 120.0 / 255.0, 1.0 });

	auto noiseMap = std::make_shared<RT::Texture::ColorMap>(RT::Texture::ColorMap());
	noiseMap->SetStop(0.0, { 1.0, 0.75, 0.0, 0.5 });
	noiseMap->SetStop(0.5, { 0.25, 0.25, 0.25, 0.25 });
	noiseMap->SetStop(1.0, { 0.0, 0.0, 0.0, 0.0 });

	auto fineMap = std::make_shared<RT::Texture::ColorMap>(RT::Texture::ColorMap());
	fineMap->SetStop(0.0, { 100.0 / 255.0, 50.0 / 255.0, 30.0 / 255.0, 0.25 });
	fineMap->SetStop(0.2, { 200.0 / 255.0, 150.0 / 255.0, 120.0 / 255.0, 0.25 });
	fineMap->SetStop(0.4, { 0.0, 0.0, 0.0, 0.25 });
	fineMap->SetStop(1.0, { 0.0, 0.0, 0.0, 0.0 });

	// **************************************************************************************
	// Create the textures.
	// **************************************************************************************	
	// The spay image texture.
	auto sprayTexture = std::make_shared<RT::Texture::Image>(RT::Texture::Image());
	sprayTexture->LoadImage("Resources/qbSpray1.bmp");
	sprayTexture->SetTransform({ 0.0, 0.0 }, 0.0, { 1.0, 1.0 });

	// The box image texture.										
	auto boxTexture = std::make_shared<RT::Texture::Image>(RT::Texture::Image());
	boxTexture->LoadImage("Resources/WoodBoxTexture.bmp");
	boxTexture->SetTransform({ 0.0, 0.0 }, 0.0, { 1.0, 1.0 });

	// The floor texture.
	auto floorTexture = std::make_shared<RT::Texture::Checker>(RT::Texture::Checker());
	floorTexture->SetTransform({ 0.0, 0.0 }, 0.0, { 8.0, 8.0 });
	floorTexture->SetColor({ 0.2, 0.2, 0.2, 1.0 }, { 0.4, 0.4, 0.4, 1.0 });

	// Textures for the wooden torus.
	auto qbWood = std::make_shared<RT::Texture::Marble>(RT::Texture::Marble());
	qbWood->SetColorMap(woodMap);
	qbWood->SetAmplitude(8.0, 1.0);
	qbWood->SetScale(2, 10);
	qbWood->SetSine(1.0, 8.0);
	qbWood->SetMinMax(-1.0, 1.0);
	qbWood->SetTransform({ 0.0, 0.0 }, 0.0, { 1.0, 1.0 });

	auto dirtTexture1 = std::make_shared<RT::Texture::BasicValNoise>(RT::Texture::BasicValNoise());
	dirtTexture1->SetColorMap(noiseMap);
	dirtTexture1->SetAmplitude(2.0);
	dirtTexture1->SetScale(16);

	auto vertGrad1 = std::make_shared<RT::Texture::Gradient>(RT::Texture::Gradient());
	vertGrad1->SetStop(0.0, { 1.0, 0.75, 0.0, 1.0 });
	vertGrad1->SetStop(0.5, { 0.5, 0.5, 0.5, 0.25 });
	vertGrad1->SetStop(1.0, { 0.5, 0.5, 0.5, 0.0 });
	vertGrad1->SetTransform({ 0.0, 0.0 }, 0.0, { 1.0, 1.0 });

	auto fineTexture = std::make_shared<RT::Texture::Marble>(RT::Texture::Marble());
	fineTexture->SetColorMap(fineMap);
	fineTexture->SetAmplitude(2.0, 1.0);
	fineTexture->SetScale(20, 40);
	fineTexture->SetSine(64.0, 128.0);
	fineTexture->SetMinMax(-1.0, 1.0);
	fineTexture->SetTransform({ 0.0, 0.0 }, 0.0, { 1.0, 1.0 });

	// **************************************************************************************
	// Create and setup normal maps.
	// **************************************************************************************
	auto woodBoxNormal = std::make_shared<RT::Normal::Image>(RT::Normal::Image());
	woodBoxNormal->LoadImage("Resources/WoodBoxNormal.bmp");
	woodBoxNormal->SetTransform({ 0.0, 0.0 }, 0.0, { 1.0, 1.0 });

	auto woodNorm1 = std::make_shared<RT::Normal::TextureNormal>(RT::Normal::TextureNormal());
	woodNorm1->AssignBaseTexture(qbWood);
	woodNorm1->m_scale = 0.0001;

	auto fineNorm1 = std::make_shared<RT::Normal::TextureNormal>(RT::Normal::TextureNormal());
	fineNorm1->AssignBaseTexture(fineTexture);
	fineNorm1->m_scale = 0.001;
	fineNorm1->m_reverse = false;

	auto mirrorNorm = std::make_shared<RT::Normal::SimpleRough>(RT::Normal::SimpleRough());
	mirrorNorm->m_amplitude = 0.01;

	// **************************************************************************************
	// Create some materials.
	// **************************************************************************************
	auto floorMaterial = std::make_shared<RT::SimpleMaterial>(RT::SimpleMaterial());
	floorMaterial->m_baseColor = { 1.0, 1.0, 1.0 };
	floorMaterial->m_reflectivity = 0.75;
	floorMaterial->m_shininess = 0.0;
	floorMaterial->AssignTexture(floorTexture);

	auto sprayBodyMat = std::make_shared<RT::SimpleMaterial>(RT::SimpleMaterial());
	sprayBodyMat->m_baseColor = { 1.0, 1.0, 1.0 };
	sprayBodyMat->m_reflectivity = 0.1;
	sprayBodyMat->m_shininess = 16.0;;
	sprayBodyMat->AssignTexture(sprayTexture);

	auto metalMat = std::make_shared<RT::SimpleMaterial>(RT::SimpleMaterial());
	metalMat->m_baseColor = { 0.5, 0.5, 0.5 };
	metalMat->m_reflectivity = 0.5;
	metalMat->m_shininess = 64.0;

	auto whitePlasticMat = std::make_shared<RT::SimpleMaterial>(RT::SimpleMaterial());
	whitePlasticMat->m_baseColor = { 1.0, 1.0, 1.0 };
	whitePlasticMat->m_reflectivity = 0.0;
	whitePlasticMat->m_shininess = 0.0;

	auto mirrorMat = std::make_shared<RT::SimpleMaterial>(RT::SimpleMaterial());
	mirrorMat->m_baseColor = { 0.0, 0.0, 1.0 };
	mirrorMat->m_reflectivity = 0.5;
	mirrorMat->m_shininess = 64.0;
	mirrorMat->AssignNormalMap(mirrorNorm);

	auto mirrorMat2 = std::make_shared<RT::SimpleMaterial>(*mirrorMat);
	mirrorMat2->m_baseColor = { 1.0, 1.0, 0.0 };

	auto boxMat = std::make_shared<RT::SimpleMaterial>(RT::SimpleMaterial());
	boxMat->m_baseColor = { 1.0, 1.0, 0.0 };
	boxMat->m_reflectivity = 0.0;
	boxMat->m_shininess = 32.0;
	boxMat->AssignTexture(boxTexture);
	boxMat->AssignNormalMap(woodBoxNormal);

	auto woodMat = std::make_shared<RT::SimpleMaterial>(RT::SimpleMaterial());
	woodMat->m_baseColor = { 1.0, 1.0, 1.0 };
	woodMat->m_reflectivity = 0.0;
	woodMat->m_shininess = 32.0;
	woodMat->AssignTexture(qbWood);
	woodMat->AssignTexture(fineTexture);
	woodMat->AssignTexture(dirtTexture1);
	woodMat->AssignTexture(vertGrad1);
	woodMat->AssignNormalMap(woodNorm1);
	woodMat->AssignNormalMap(fineNorm1);

	// **************************************************************************************	
	// Create and setup objects.
	// **************************************************************************************
	auto floor = std::make_shared<RT::Plane>(RT::Plane());
	floor->m_tag = "floor";
	floor->m_isVisible = true;
	floor->SetTransformMatrix(RT::GTform {{0.0, 0.0, 0.5}, { 0.0, 0.0, 0.0 }, { 6.0, 6.0, 1.0 }});
	floor->AssignMaterial(floorMaterial);
	floor->m_uvMapType = RT::uvPLANE;

	auto backWall = std::make_shared<RT::Plane>(RT::Plane());
	backWall->m_tag = "backWall";
	backWall->m_isVisible = true;
	backWall->SetTransformMatrix(RT::GTform {{0.0, 2.0, 0.0}, { -M_PI / 2.0, 0.0, 0.0 }, { 4.0, 4.0, 1.0 }});
	backWall->AssignMaterial(mirrorMat);

	auto sideWall = std::make_shared<RT::Plane>(*backWall);
	sideWall->m_tag = "sideWall";
	sideWall->SetTransformMatrix(RT::GTform {{-2.0, 0.0, 0.0}, { -M_PI / 2.0, -M_PI / 2.0, 0.0 }, { 4.0, 4.0, 1.0 }});
	sideWall->AssignMaterial(mirrorMat2);

	/*double sprayX = 1.0;
	double sprayY = -1.75;*/
	double sprayX = 0.0;
	double sprayY = 0.0;
	auto sprayBody = std::make_shared<RT::Cylinder>(RT::Cylinder());
	sprayBody->m_tag = "sprayBody";
	sprayBody->m_isVisible = true;
	sprayBody->SetTransformMatrix(RT::GTform {{sprayX, sprayY, -0.5}, { 0.0, 0.0, M_PI / 5.0 }, { 0.4, 0.4, 1.0 }});
	sprayBody->AssignMaterial(sprayBodyMat);
	sprayBody->m_uvMapType = RT::uvCYLINDER;

	auto sprayTopCone = std::make_shared<RT::Cone>(RT::Cone());
	sprayTopCone->m_tag = "sprayTopCone";
	sprayTopCone->m_isVisible = true;
	sprayTopCone->SetTransformMatrix(RT::GTform {{sprayX, sprayY, -2.0}, { 0.0, 0.0, 0.0 }, { 0.4, 0.4, 0.5 }});
	sprayTopCone->AssignMaterial(metalMat);

	auto sprayTop = std::make_shared<RT::Cylinder>(RT::Cylinder());
	sprayTop->m_tag = "sprayTop";
	sprayTop->m_isVisible = true;
	sprayTop->SetTransformMatrix(RT::GTform {{sprayX, sprayY, -1.5}, { 0.0, 0.0, 0.0 }, { 0.2, 0.2, 0.5 }});
	sprayTop->AssignMaterial(whitePlasticMat);


	auto sprayCan = std::make_shared<RT::SHAPES::CompositeBase>(RT::SHAPES::CompositeBase());
	sprayCan->m_tag = "sprayCan";
	sprayCan->m_isVisible = true;
	sprayCan->AddSubShape(sprayBody);
	sprayCan->AddSubShape(sprayTopCone);
	sprayCan->AddSubShape(sprayTop);
	sprayCan->SetTransformMatrix(RT::GTform{{1.0, -1.75, 0.0}, { 0.0, -M_PI / 4, 0.0 }, { 1.0, 1.0, 1.0 }});


	auto box = std::make_shared<RT::Box>(RT::Box());
	box->m_tag = "box";
	box->m_isVisible = true;
	box->SetTransformMatrix(RT::GTform {{-1.0, -2.0, 0.0}, { 0.0, 0.0, -M_PI / 6.0 }, { 0.5, 0.5, 0.5 }});
	box->AssignMaterial(boxMat);
	box->m_uvMapType = RT::uvBOX;

	auto box2 = std::make_shared<RT::Box>(*box);
	box2->SetTransformMatrix(RT::GTform {{-1.0, -0.75, -0.25}, { M_PI / 4.0, 0.0, M_PI / 2.0 }, { 0.5, 0.5, 0.5 }});

	auto torus = std::make_shared<RT::RM::Torus>(RT::RM::Torus());
	torus->m_tag = "torus";
	torus->m_isVisible = true;
	torus->SetRadii(0.7, 0.3);
	torus->SetTransformMatrix(RT::GTform {{2.5, -2.0, 0.2}, { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 }});
	torus->AssignMaterial(woodMat);
	torus->m_uvMapType = RT::uvSPHERE;

	// **************************************************************************************
	// Put the objects into the scene.	
	// **************************************************************************************
	/*m_objectList.push_back(sprayBody);
	m_objectList.push_back(sprayTopCone);
	m_objectList.push_back(sprayTop);*/
	m_objectList.push_back(sprayCan);
	m_objectList.push_back(box);
	m_objectList.push_back(box2);
	m_objectList.push_back(floor);
	m_objectList.push_back(sideWall);
	m_objectList.push_back(backWall);
	m_objectList.push_back(torus);

	// **************************************************************************************	
	// Construct and setup the lights.
	// **************************************************************************************	
	auto leftLight = std::make_shared<RT::PointLight>(RT::PointLight());
	leftLight->m_location = { 0.0, -20.0, -20.0 };
	leftLight->m_color = { 1.0, 1.0, 1.0 };
	leftLight->m_intensity = 0.33;

	auto rightLight = std::make_shared<RT::PointLight>(RT::PointLight());
	rightLight->m_location = { 8.0, -20.0, -20.0 };
	rightLight->m_color = { 1.0, 1.0, 1.0 };
	rightLight->m_intensity = 0.33;

	auto topLight = std::make_shared<RT::PointLight>(RT::PointLight());
	topLight->m_location = { 0.0, 3.0, -20.0 };
	topLight->m_color = { 1.0, 1.0, 1.0 };
	topLight->m_intensity = 0.33;

	m_lightList.push_back(leftLight);
	m_lightList.push_back(rightLight);
	m_lightList.push_back(topLight);
}
