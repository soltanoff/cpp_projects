#ifndef ROTATIONHANDLER_H
#define ROTATIONHANDLER_H

class RotationHandler: public osgGA::GUIEventHandler 
{
private:
	bool onlyX;
    osg::ref_ptr<osg::MatrixTransform> m_mat;

public:
    RotationHandler(osg::MatrixTransform *mat):
        m_mat(mat), onlyX(false)
    {
    }

	RotationHandler(osg::MatrixTransform *mat, bool Xrot):
        m_mat(mat), onlyX(Xrot)
    {
    }

    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter &adapter)
    {
        osg::Matrix mat;
        switch (ea.getEventType()) 
		{
        case osgGA::GUIEventAdapter::FRAME:
            mat.makeRotate(ea.getTime(), onlyX ? osg::Vec3(0.000001f, 0.0f, 0.0f): osg::Vec3(1.0f, 1.0f, 1.0f));
            m_mat->setMatrix(mat);
        }
		return true;
    }
};

#endif