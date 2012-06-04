/*
 * Copyright (c) 2008, Willow Garage, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Willow Garage, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */


#include <wx/wx.h>

#include "../ogre_tools/wx_ogre_render_window.h"
#include "../ogre_tools/grid.h"
#include "../ogre_tools/fps_camera.h"
#include "../ogre_tools/orbit_camera.h"
#include "../ogre_tools/axes.h"
#include "../ogre_tools/shape.h"
#include "../ogre_tools/arrow.h"
#include "../ogre_tools/point_cloud.h"
#include "../ogre_tools/billboard_line.h"
#include "../ogre_tools/initialization.h"

#include <OGRE/OgreRoot.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreViewport.h>
#include <OGRE/OgreLight.h>

#include <ros/time.h>

#ifdef __WXMAC__
#include <ApplicationServices/ApplicationServices.h>
#endif

using namespace ogre_tools;

class MyFrame : public wxFrame
{
public:
  MyFrame(wxWindow* parent)
  : wxFrame(parent, -1, _("Grid Test App"), wxDefaultPosition, wxSize(800,600), wxDEFAULT_FRAME_STYLE)
  , left_mouse_down_( false )
  , middle_mouse_down_( false )
  , right_mouse_down_( false )
  , mouse_x_( 0 )
  , mouse_y_( 0 )
  {
    ogre_tools::initializeOgre();

    root_ = Ogre::Root::getSingletonPtr();

    try
    {
        scene_manager_ = root_->createSceneManager( Ogre::ST_GENERIC, "TestSceneManager" );

        render_panel_ = new ogre_tools::wxOgreRenderWindow( root_, this );
        render_panel_->SetSize( this->GetSize() );
        render_panel_->setAutoRender(false);

        ogre_tools::initializeResources( ogre_tools::V_string() );

        camera_ = new ogre_tools::OrbitCamera( scene_manager_ );
        camera_->setPosition( 0, 0, 15 );
        camera_->getOgreCamera()->setNearClipDistance( 0.1 );

        render_panel_->getViewport()->setCamera( camera_->getOgreCamera() );

        Ogre::Light* directional_light = scene_manager_->createLight( "MainDirectional" );
        directional_light->setType( Ogre::Light::LT_DIRECTIONAL );
        directional_light->setDirection( Ogre::Vector3( 0, -1, 1 ) );
        directional_light->setDiffuseColour( Ogre::ColourValue( 1.0f, 1.0f, 1.0f ) );

#if 0
        ogre_tools::Grid* grid = new ogre_tools::Grid( scene_manager_, NULL, ogre_tools::Grid::Lines, 10, 1.0f, 0.02, Ogre::ColourValue(1.0f, 1.0f, 1.0f, 0.5f));
        grid->setHeight(4);


        ogre_tools::BillboardLine* line = new ogre_tools::BillboardLine( scene_manager_, NULL );
        line->setMaxPointsPerLine(105);
        for ( int i = -50; i < 50; ++i )
        {
          line->addPoint( Ogre::Vector3( i*2, 0.0f, -1.0f ) );
        }

        for ( int i = 0; i < 5; ++i )
        {
          line->addPoint( Ogre::Vector3( 4.0f, 0.0f, i ) );
        }

        line->setLineWidth( 0.05 );
        line->setColor( 0.0f, 1.0f, 0.0f, 0.5f );

        Shape* sphere = new Shape(Shape::Sphere, scene_manager_);
        sphere->setPosition(Ogre::Vector3(0.0f, 0.0f, 2.0f));
        sphere->setColor(0.0f, 1.0f, 2.0f, 1.0f);
        Shape* cube = new Shape(Shape::Cube, scene_manager_);
        cube->setPosition(Ogre::Vector3(0.0f, 1.0f, 2.0f));
        cube->setColor(1.0f, 0.0f, 0.0f, 1.0f);
        Shape* cylinder = new Shape(Shape::Cylinder, scene_manager_);
        cylinder->setPosition(Ogre::Vector3(0.0f, 2.0f, 2.0f));
        cylinder->setColor(1.0f, 1.0f, 0.0f, 1.0f);
        Shape* cone = new Shape(Shape::Cone, scene_manager_);
        cone->setPosition(Ogre::Vector3(0.0f, 3.0f, 2.0f));
        cone->setColor(0.0f, 0.0f, 1.0f, 1.0f);

        ogre_tools::Axes* axes = new ogre_tools::Axes( scene_manager_ );
        //axes->setScale( Ogre::Vector3( 2.0f, 2.0f, 2.0f ) );

        /*ogre_tools::Cone* cone = new ogre_tools::Cone( scene_manager_, NULL );
        cone->setScale( Ogre::Vector3( 0.3f, 2.0f, 0.3f ) );*/

        ogre_tools::Arrow* arrow = new ogre_tools::Arrow( scene_manager_ );
        arrow->setHeadColor( 1.0f, 0.0f, 0.0f );
        arrow->setShaftColor( 0.0f, 0.0f, 1.0f );
        arrow->setOrientation( Ogre::Quaternion::IDENTITY );
        //arrow->setOrientation( Ogre::Quaternion( Ogre::Degree( 45 ), Ogre::Vector3::UNIT_X ) );
        //arrow->setScale( Ogre::Vector3( 1.0f, 1.0f, 3.0f ) );
#endif

#if 01
        Ogre::SceneNode* scene_node = scene_manager_->getRootSceneNode()->createChildSceneNode();
        ogre_tools::PointCloud* pointCloud = new ogre_tools::PointCloud();
        pointCloud->setDimensions(0.05f, 0.05f, 0.05f);
        //pointCloud->setColorByIndex(true);
        pointCloud->setRenderMode(PointCloud::RM_BILLBOARDS);
        pointCloud->setCommonDirection(Ogre::Vector3(0.0, 1.0, 0.0));
        pointCloud->setCommonUpVector(Ogre::Vector3(0.0, 0.0, -1.0));
        pointCloud->setAlpha(1.0);
        std::vector<ogre_tools::PointCloud::Point> points;
        int32_t xcount = 200;
        int32_t ycount = 100;
        int32_t zcount = 100;
	//        points.resize(xcount * ycount * zcount);
        float factor = 0.1f;
        for (int32_t x = 0; x < xcount; ++x)
        {
          for (int32_t y = 0; y < ycount; ++y)
          {
            for (int32_t z = 0; z < zcount; ++z)
            {
	      //    int32_t index = (ycount*zcount*x) + zcount*y + z;
              ogre_tools::PointCloud::Point point;// = points[index];
              point.x = x * factor;
              point.y = y * factor;
              point.z = z * factor;

              point.setColor(x * 0.1, y * 0.1, z * 0.1);
	      points.push_back(point);
            }
          }
        }

	printf("size: %d\n", points.size());
        pointCloud->addPoints( &points.front(), points.size() );
        scene_node->attachObject(pointCloud);
#endif
    }
    catch ( Ogre::Exception& e )
    {
        printf( "Fatal error: %s\n", e.what() );
        exit(1);
    }

    render_panel_->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( MyFrame::OnMouseEvents ), NULL, this );
    render_panel_->Connect( wxEVT_MIDDLE_DOWN, wxMouseEventHandler( MyFrame::OnMouseEvents ), NULL, this );
    render_panel_->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( MyFrame::OnMouseEvents ), NULL, this );
    render_panel_->Connect( wxEVT_MOTION, wxMouseEventHandler( MyFrame::OnMouseEvents ), NULL, this );
    render_panel_->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( MyFrame::OnMouseEvents ), NULL, this );
    render_panel_->Connect( wxEVT_MIDDLE_UP, wxMouseEventHandler( MyFrame::OnMouseEvents ), NULL, this );
    render_panel_->Connect( wxEVT_RIGHT_UP, wxMouseEventHandler( MyFrame::OnMouseEvents ), NULL, this );
    render_panel_->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( MyFrame::OnMouseEvents ), NULL, this );

     wxTheApp->Connect(wxEVT_IDLE, wxIdleEventHandler( MyFrame::onRenderIdle ), NULL, this );
  }

  ~MyFrame()
  {
    render_panel_->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( MyFrame::OnMouseEvents ), NULL, this );
    render_panel_->Disconnect( wxEVT_MIDDLE_DOWN, wxMouseEventHandler( MyFrame::OnMouseEvents ), NULL, this );
    render_panel_->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( MyFrame::OnMouseEvents ), NULL, this );
    render_panel_->Disconnect( wxEVT_MOTION, wxMouseEventHandler( MyFrame::OnMouseEvents ), NULL, this );
    render_panel_->Disconnect( wxEVT_LEFT_UP, wxMouseEventHandler( MyFrame::OnMouseEvents ), NULL, this );
    render_panel_->Disconnect( wxEVT_MIDDLE_UP, wxMouseEventHandler( MyFrame::OnMouseEvents ), NULL, this );
    render_panel_->Disconnect( wxEVT_RIGHT_UP, wxMouseEventHandler( MyFrame::OnMouseEvents ), NULL, this );
    render_panel_->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( MyFrame::OnMouseEvents ), NULL, this );

    render_panel_->Destroy();
  }

  void onRenderIdle(wxIdleEvent& e)
  {
    ros::WallTime start = ros::WallTime::now();
    root_->renderOneFrame();
    ros::WallTime end = ros::WallTime::now();
    printf("Render took [%f] msec\n", (end - start).toSec() * 1000.0f);

    e.Skip();
    e.RequestMore();
  }

private:

  void OnMouseEvents( wxMouseEvent& event )
  {
    int lastX = mouse_x_;
    int lastY = mouse_y_;

    mouse_x_ = event.GetX();
    mouse_y_ = event.GetY();

    if ( event.LeftDown() )
    {
      left_mouse_down_ = true;
      middle_mouse_down_ = false;
      right_mouse_down_ = false;
    }
    else if ( event.MiddleDown() )
    {
      left_mouse_down_ = false;
      middle_mouse_down_ = true;
      right_mouse_down_ = false;
    }
    else if ( event.RightDown() )
    {
      left_mouse_down_ = false;
      middle_mouse_down_ = false;
      right_mouse_down_ = true;
    }
    else if ( event.LeftUp() )
    {
      left_mouse_down_ = false;
    }
    else if ( event.MiddleUp() )
    {
      middle_mouse_down_ = false;
    }
    else if ( event.RightUp() )
    {
      right_mouse_down_ = false;
    }
    else if ( event.Dragging() )
    {
      int32_t diff_x = mouse_x_ - lastX;
      int32_t diff_y = mouse_y_ - lastY;

      bool handled = false;
      if ( left_mouse_down_ )
      {
        camera_->mouseLeftDrag( diff_x, diff_y, event.CmdDown(), event.AltDown(), event.ShiftDown());

        handled = true;
      }
      else if ( middle_mouse_down_ )
      {
        camera_->mouseMiddleDrag( diff_x, diff_y, event.CmdDown(), event.AltDown(), event.ShiftDown() );

        handled = true;
      }
      else if ( right_mouse_down_ )
      {
        camera_->mouseRightDrag( diff_x, diff_y, event.CmdDown(), event.AltDown(), event.ShiftDown() );

        handled = true;
      }
    }

    if ( event.GetWheelRotation() != 0 )
    {
      camera_->scrollWheel( event.GetWheelRotation(), event.CmdDown(), event.AltDown(), event.ShiftDown() );
    }
  }

  Ogre::Root* root_;
  Ogre::SceneManager* scene_manager_;

  ogre_tools::wxOgreRenderWindow* render_panel_;

  ogre_tools::Grid* grid_;
  ogre_tools::CameraBase* camera_;

  // Mouse handling
  bool left_mouse_down_;
  bool middle_mouse_down_;
  bool right_mouse_down_;
  int mouse_x_;
  int mouse_y_;
};

// our normal wxApp-derived class, as usual
class MyApp : public wxApp
{
public:

  bool OnInit()
  {
#ifdef __WXMAC__
    ProcessSerialNumber PSN;
    GetCurrentProcess(&PSN);
    TransformProcessType(&PSN,kProcessTransformToForegroundApplication);
    SetFrontProcess(&PSN);
#endif

    wxFrame* frame = new MyFrame(NULL);
    SetTopWindow(frame);
    frame->Show();
    return true;
  }

  int OnExit()
  {
    cleanupOgre();
      return 0;
  }
};

DECLARE_APP(MyApp);
IMPLEMENT_APP(MyApp);
