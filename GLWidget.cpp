

#include "stdafx.h"
#include "GLWidget.h"
//#include "KCentersClustering.h"


GLWidget::GLWidget(QGLFormat format, QWidget *parent) :
    QGLWidget(format, parent),
    _isMouseDown(false),
    _zoomFactor(1.0),
    _canvas_width(1024),
    _canvas_height(1024)
{
}


GLWidget::~GLWidget()
{
}


void GLWidget::SetCanvas(int canvas_witdh, int canvas_height)
{
    this->_canvas_width = canvas_witdh;
    this->_canvas_height = canvas_height;
}

void GLWidget::initializeGL()
{
    glClearColor(0.5, 0.5, 0.5, 1);
    //this->repaint();
}

/**
 * SET
 */
void GLWidget::SetWidthHeight(int canvas_width, int canvas_height)
{
    this->_canvas_width = canvas_width;
    this->_canvas_height = canvas_height;
}


bool GLWidget::event( QEvent * event )
{
    return QGLWidget::event(event);
}


// This is an override function from Qt but I can't find its purpose
void GLWidget::resizeGL(int width, int height)
{
    QGLWidget::resizeGL(width, height);
}


void GLWidget::paintGL()
{
    glViewport(0, 0, this->width(),  this->height());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // scrollOffset has the coordinates of horizontal and vertical scrollbars
    gluOrtho2D(0 + _scrollOffset.x(),
               this->width() + _scrollOffset.x() ,
               this->height() + _scrollOffset.y(),
               0 + _scrollOffset.y() );	// flip the y axis
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(_zoomFactor, _zoomFactor, _zoomFactor);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 0.0f);


    if(points.size() > 0)
    {
        for(size_t a = 0; a < points.size(); a++)
        {
            for(size_t b = 0; b < points[a].size() - 1; b++)
            {
                glVertex2f(points[a][b].x, points[a][b].y);
                glVertex2f(points[a][b+1].x, points[a][b+1].y);
            }
        }

    }

    if(tempPoints.size() > 2)
    {
        for(size_t a = 0; a < tempPoints.size() -1 ; a++)
        {
            glVertex2f(tempPoints[a].x, tempPoints[a].y);
            glVertex2f(tempPoints[a+1].x, tempPoints[a+1].y);
        }
    }

    glEnd();

    glFlush();
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);

}


// Mouse is pressed
void GLWidget::mousePressEvent(int x, int y)
{
    _isMouseDown = true;

    double dx = x + _scrollOffset.x();
    dx /= _zoomFactor;

    double dy = y + _scrollOffset.y();
    dy /= _zoomFactor;


    tempPoints.push_back(MyPoint(dx, dy));

    this->repaint();
}


// Mouse is moved
void GLWidget::mouseMoveEvent(int x, int y)
{
    double dx = x + _scrollOffset.x();
    dx /= _zoomFactor;

    double dy = y + _scrollOffset.y();
    dy /= _zoomFactor;

    if(_isMouseDown)
    {
        tempPoints.push_back(MyPoint(dx, dy));
        this->repaint();
    }
}


// Mouse is released
void GLWidget::mouseReleaseEvent(int x, int y)
{
    _isMouseDown = false;
    double dx = x + _scrollOffset.x();
    dx /= _zoomFactor;

    double dy = y + _scrollOffset.y();
    dy /= _zoomFactor;

    if(tempPoints.size() >= 2)
    {
        tempPoints.push_back(MyPoint(dx, dy));
        std::vector<MyPoint> tempPoints2;
        for(size_t a = 0; a < tempPoints.size(); a++)
        {
            tempPoints2.push_back(tempPoints[a]);
        }
        points.push_back(tempPoints2);
        tempPoints.clear();
    }
    this->repaint();
}

void GLWidget::mouseDoubleClick(int x, int y)
{
    double dx = x + _scrollOffset.x();
    dx /= _zoomFactor;

    double dy = y + _scrollOffset.y();
    dy /= _zoomFactor;
}

// Save image to file
void GLWidget::SaveImage(QString filename)
{
    // Save onscreen
    paintGL();
    glFlush();
    QImage image = this->grabFrameBuffer();
    if( !image.save( filename) ) std::cout << "Error saving image\n";
}


void GLWidget::HorizontalScroll(int val) { _scrollOffset.setX(val); }
void GLWidget::VerticalScroll(int val) { _scrollOffset.setY(val); }
void GLWidget::ZoomIn() { this->_zoomFactor += 0.05f; }
void GLWidget::ZoomOut() { this->_zoomFactor -= 0.05f; if(this->_zoomFactor < 0.1f) _zoomFactor = 0.1f; }

void GLWidget::Reset()
{
}


/*
void GLWidget::DrawCircle(GLfloat x, GLfloat y, GLfloat radius, QColor col)
{
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    int i;
    int triangleAmount = 10;
    GLfloat twicePi = 2.0f * M_PI;

    glBegin(GL_TRIANGLE_FAN);
    glColor4f(col.red() / 255.0, col.green() / 255.0, col.blue() / 255.0, 1.0);
    glVertex2f(x, y); // center of circle
    for(i = 0; i <= triangleAmount;i++)
    {
        glVertex2f(
            x + (radius * cos(i *  twicePi / triangleAmount)),
            y + (radius * sin(i * twicePi / triangleAmount))
            );
    }
    glEnd();

    glFlush();
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
}
*/

/*
void GLWidget::DoClustering(int k)
{
    this-> labels = KCentersClustering::DoClustering(this->points, k);

    colors.clear();
    for(size_t a = 0; a < k; a++)
    {
        colors.push_back(qRgb(rand() % 255, rand() % 255, rand() % 255));
    }
    //std::vector<QRgb> colors;
    //std::vector<int> labels;
    this->repaint();

}
*/
