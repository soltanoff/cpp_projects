#pragma comment(lib, "osg.lib")
#pragma comment(lib, "osgViewer.lib")
#pragma comment(lib, "osgDB.lib")
#pragma comment(lib, "osgGA.lib")
#pragma comment(lib, "osgFX.lib")

#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>
#include "RotationHandler.h"

osg::Geode *set_geode_obj(osg::ShapeDrawable *drawable, osg::Material *material)
{
	osg::Geode *obj = new osg::Geode();
	obj->addDrawable(drawable);
	obj->getOrCreateStateSet()->setAttribute(material);
	return obj;
}

osg::PositionAttitudeTransform *set_light(osg::Group *group)
{
	osg::StateSet *lightStateSet = group->getOrCreateStateSet();

	osg::Material *material = new osg::Material();
	material->setDiffuse(osg::Material::FRONT,  osg::Vec4(0.0, 0.0, 0.0, 1.0));
	material->setEmission(osg::Material::FRONT, osg::Vec4(1.0f, 1.0f, 1.0f, 0.0f));

	osg::Geode *lightMarker = new osg::Geode();
	lightMarker->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(), 1)));
	lightMarker->getOrCreateStateSet()->setAttribute(material);

	osg::Light *light = new osg::Light();
	light->setLightNum(1);
	light->setPosition(osg::Vec4(0.0, 0.0, 0.0, 1.0));
	light->setDiffuse(osg::Vec4(1.0, 1.0, 0.0, 1.0));
	light->setSpecular(osg::Vec4(1.0, 1.0, 1.0, 1.0));
	light->setAmbient(osg::Vec4(0.0, 0.0, 0.0, 1.0));

	osg::LightSource *lightSource = new osg::LightSource();

	lightSource->setLight(light);
	lightSource->setLocalStateSetModes(osg::StateAttribute::ON);
	lightSource->setStateSetModes(*lightStateSet, osg::StateAttribute::ON);

	osg::PositionAttitudeTransform *lightTransform = new osg::PositionAttitudeTransform();
	lightTransform->addChild(lightSource);
	lightTransform->addChild(lightMarker);
	lightTransform->setPosition(osg::Vec3(0, 0, 5));
	lightTransform->setScale(osg::Vec3(0.1,0.1,0.1));
	lightTransform->setPosition(osg::Vec3(12.0, -4.0, 1.0));
	return lightTransform;
}

osg::MatrixTransform *set_matrix_transform(osg::Group *group, osg::ShapeDrawable *drawable, osg::Material *material)
{
	osg::MatrixTransform *mat_obj = new osg::MatrixTransform;
    mat_obj->addChild(set_geode_obj(drawable, material));
    group->addChild(mat_obj);
	return mat_obj;
}

int main()
{
    osgViewer::Viewer viewer;
	osg::Group *group = new osg::Group(); // Корень дерева
	// ==========================================================================================
	// Создание материалов
	osg::Material *material_cone = new osg::Material();
	material_cone->setDiffuse(osg::Material::FRONT,  osg::Vec4(1.0, 1.0, 1.0, 1.0));
	material_cone->setSpecular(osg::Material::FRONT, osg::Vec4(0.0, 0.0, 0.0, 1.0));
	material_cone->setAmbient(osg::Material::FRONT,  osg::Vec4(0.1, 0.1, 0.1, 1.0));
	material_cone->setEmission(osg::Material::FRONT, osg::Vec4(0.0, 0.0, 0.0, 1.0));
	material_cone->setShininess(osg::Material::FRONT, 25.0);

	osg::Material *material_shape = new osg::Material();
	material_shape->setDiffuse(osg::Material::FRONT,  osg::Vec4(0.4, 0.0, 0.1, 1.0));
	material_shape->setSpecular(osg::Material::FRONT, osg::Vec4(0.0, 0.1, 0.0, 1.0));
	material_shape->setAmbient(osg::Material::FRONT,  osg::Vec4(0.1, 0.0, 0.1, 1.0));
	material_shape->setEmission(osg::Material::FRONT, osg::Vec4(0.0, 0.0, 0.0, 1.0));
	material_shape->setShininess(osg::Material::FRONT, 25.0);

	osg::Material *material_cylinder = new osg::Material();
	material_cylinder->setDiffuse(osg::Material::FRONT,  osg::Vec4(0.0, 0.0, 1.0, 0.2));
	material_cylinder->setSpecular(osg::Material::FRONT, osg::Vec4(1.0, 1.0, 1.0, 0.2));
	material_cylinder->setAmbient(osg::Material::FRONT,  osg::Vec4(0.1, 0.0, 0.1, 1.0));
	material_cylinder->setEmission(osg::Material::FRONT, osg::Vec4(0.0, 0.0, 0.0, 1.0));
	material_cylinder->setShininess(osg::Material::FRONT, 10.0);
	// ==========================================================================================
	// Создание объектов
    osg::Sphere *shape = new osg::Sphere(osg::Vec3(2.0f, 0.0f, 0.0f), 2.0f);
    osg::ShapeDrawable *drawable_shape = new osg::ShapeDrawable(shape);
	drawable_shape->setColor(osg::Vec4(0.22f, 0.5f, 0.5f, 1.0f));

	osg::Cone *cone = new osg::Cone(osg::Vec3(2.0f, -3.0f, -3.0f), 1.5f, 2.0f);
	osg::ShapeDrawable *drawable_cone = new osg::ShapeDrawable(cone);
	drawable_cone->setColor(osg::Vec4(1.0f, 1.0f, 0.0f, 0.0f));

	osg::Cylinder *cylinder = new osg::Cylinder(osg::Vec3(8.0f, -8.0f, 0.0f), 1.0f, 1.0f);
	osg::ShapeDrawable *drawable_cylinder = new osg::ShapeDrawable(cylinder);
	drawable_cone->setColor(osg::Vec4(0.4f, 1.0f, 0.8f, 0.0f));
	// ==========================================================================================
	// Создание дерева объектов сцены
	group->addChild(set_light(group)); // Задаем свет
    viewer.addEventHandler(new RotationHandler(set_matrix_transform(group, drawable_cone, material_cone)));
    viewer.addEventHandler(new RotationHandler(set_matrix_transform(group, drawable_cylinder, material_cylinder), true));
	group->addChild(set_geode_obj(drawable_shape, material_shape));

	viewer.setSceneData(group);
    return viewer.run();
}