// =============================================================================
//  main.cpp
//
//  MIT License
//
//  Copyright (c) 2018 Dairoku Sekiguchi
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
// =============================================================================
/*!
  \file     main.cpp
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2018/11/19
  \brief    
*/

// Includes --------------------------------------------------------------------
#include <iostream>
#include <string>
#include <cstring>
#include <math.h>
#include <gtkmm.h>
#include "ibc/gtkmm/image_view.h"
#include "ibc/gtkmm/image_data.h"

//#define DEBUG_TRACE(...)    printf(__VA_ARGS__)
//#define VERBOSE_INFO(...)   printf(__VA_ARGS__)
#define VERBOSE_INFO(...)
#define DEBUG_TRACE(...)

class MainWin : public Gtk::Window
{
public:
  MainWin(ibc::gtkmm::ImageData *inImageDataPtr) :
    m_box(Gtk::Orientation::ORIENTATION_VERTICAL , 0),
    m_status_bar("Statusbar")
  {
    mImageDataPtr = inImageDataPtr;
    mImageView.setImageDataPtr(inImageDataPtr);
    m_scr_win.add(mImageView);
    m_box.pack_start(m_scr_win, true, true, 0);
    m_box.pack_end(m_status_bar, false, false, 0);
    add(m_box);
    resize(300, 300);
    show_all_children();
  }
protected:

private:
  Gtk::Box            m_box;
  Gtk::ScrolledWindow m_scr_win;
  Gtk::Label          m_status_bar;
  ibc::gtkmm::ImageView   mImageView;
  ibc::gtkmm::ImageData   *mImageDataPtr;
};

//  The following handler will be called after a standard gtk+ command line
//  option parse process is finished
//  Note that: the standard command line options are always removed before
//  calling this handler from the command_line object by the caller
int on_command_line(const Glib::RefPtr<Gio::ApplicationCommandLine>& command_line,
                    Glib::RefPtr<Gtk::Application>& app)
{
  int argc = 0;
  char** argv = command_line->get_arguments(argc);

  for (int i = 0; i < argc; ++i)
  std::cout << "argv[" << i << "] = " << argv[i] << std::endl;

  //if (argc != 1)
  //  g_file_name.assign(argv[1]);

  app->activate(); // NOTE: Without activate() the window won't be shown.
  return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
  // The command line arguments must be checked before Gtk::Application::run()
  // is called. The Gio::APPLICATION_HANDLES_COMMAND_LINE flag and the
  // on_command_line() signal handler are not necessary. This program is simpler
  // without them, and with argc = 1 in the call to Gtk::Application::create().
  // They are included to show a program with Gio::APPLICATION_HANDLES_COMMAND_LINE.
  // Gio::APPLICATION_NON_UNIQUE makes it possible to run several instances of
  // this application simultaneously.
  auto app = Gtk::Application::create(argc, argv,
      "org.gtkmm.example", Gio::APPLICATION_HANDLES_COMMAND_LINE | Gio::APPLICATION_NON_UNIQUE);

  // Note after = false.
  // Only one signal handler is invoked. This signal handler must run before
  // the default signal handler, or else it won't run at all.
  app->signal_command_line().connect(sigc::bind(sigc::ptr_fun(&on_command_line), app), false);

  // Create ImageData here
#if 0
  ibc::gtkmm::ImageData   imageData;
  ibc::image::ImageType   imageType(ibc::image::ImageType::PIXEL_TYPE_RGB,
                                    ibc::image::ImageType::BUFFER_TYPE_PIXEL_ALIGNED,
                                    ibc::image::ImageType::DATA_TYPE_8BIT);
  ibc::image::ImageFormat imageFormat(imageType, 640, 480);
  imageData.allocateImageBuffer(imageFormat);
  unsigned char *bufPtr = (unsigned char *)imageData.getImageBufferPtr();
  for (int y = 0; y < 480; y++)
    for (int x = 0; x < 640; x++)
    {
      *bufPtr = (unsigned char)(x ^ y);
      bufPtr++;
      *bufPtr = (unsigned char)(x ^ y);
      bufPtr++;
      *bufPtr = (unsigned char)(x ^ y);
      bufPtr++;
    }
#endif
#if 1
  ibc::gtkmm::ImageData   imageData;
  ibc::image::ImageType   imageType(ibc::image::ImageType::PIXEL_TYPE_MONO,
                                    ibc::image::ImageType::BUFFER_TYPE_PIXEL_ALIGNED,
                                    ibc::image::ImageType::DATA_TYPE_8BIT);
  ibc::image::ImageFormat imageFormat(imageType, 640, 480);
  imageData.allocateImageBuffer(imageFormat);
  unsigned char *bufPtr = (unsigned char *)imageData.getImageBufferPtr();
  for (int y = 0; y < 480; y++)
    for (int x = 0; x < 640; x++)
    {
      //*bufPtr = (unsigned char)(x ^ y);
      *bufPtr = (unsigned char)(x & 0xFF);
      bufPtr++;
    }
//imageData.mActiveConverter->setColorMapIndex(ibc::image::ColorMap::CMIndex_GrayScale);
//imageData.mActiveConverter->setColorMapIndex(ibc::image::ColorMap::CMIndex_Jet);
//imageData.mActiveConverter->setColorMapIndex(ibc::image::ColorMap::CMIndex_CoolWarm);
  imageData.mActiveConverter->setColorMapIndex(ibc::image::ColorMap::CMIndex_Jet, 3);
#endif

  imageData.markAsImageModified();

  MainWin window(&imageData);
  return app->run(window);
}
