// =============================================================================
//  rv.h
//
//  MIT License
//
//  Copyright (c) 2019 Dairoku Sekiguchi
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
  \file     rv.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/02/03
  \brief    
*/
#ifndef MAIN_WIN_H_
#define MAIN_WIN_H_

// Includes --------------------------------------------------------------------
#include <gtkmm.h>
#include <boost/program_options.hpp>
#include "ibc/gtkmm/image_view.h"
#include "ibc/gtkmm/image_data.h"

//#define DEBUG_TRACE(...)    printf(__VA_ARGS__)
//#define VERBOSE_INFO(...)   printf(__VA_ARGS__)
#define VERBOSE_INFO(...)
#define DEBUG_TRACE(...)

// -----------------------------------------------------------------------------
// rvWindow class
// -----------------------------------------------------------------------------
class rvWindow : public Gtk::Window
{
public:
  // ---------------------------------------------------------------------------
  // rvWindow
  // ---------------------------------------------------------------------------
  rvWindow() :
    m_box(Gtk::Orientation::ORIENTATION_VERTICAL , 0),
    m_status_bar("Statusbar")
  {
    m_scr_win.add(mImageView);
    m_box.pack_start(m_scr_win, true, true, 0);
    m_box.pack_end(m_status_bar, false, false, 0);
    add(m_box);
    resize(300, 300);
    show_all_children();
  }

  // Member functions ----------------------------------------------------------
  // ---------------------------------------------------------------------------
  // setImageDataptr
  // ---------------------------------------------------------------------------
  void setImageDataptr(ibc::gtkmm::ImageData *inImageDataPtr)
  {
    mImageDataPtr = inImageDataPtr;
    mImageView.setImageDataPtr(inImageDataPtr);
  }

private:
  Gtk::Box            m_box;
  Gtk::ScrolledWindow m_scr_win;
  Gtk::Label          m_status_bar;
  ibc::gtkmm::ImageView   mImageView;
  ibc::gtkmm::ImageData   *mImageDataPtr;
};

// -----------------------------------------------------------------------------
// rvApplication class
// -----------------------------------------------------------------------------
class rvApplication: public Gtk::Application
{
public:
  static Glib::RefPtr<rvApplication> create()
  {
    return Glib::RefPtr<rvApplication>(new rvApplication());
  }

protected:
  boost::program_options::variables_map       mVarMap;
  ibc::gtkmm::ImageData   mImageData;
  unsigned char *mData;
  double mGain, mOffset;

  // ---------------------------------------------------------------------------
  // rvApplication
  // ---------------------------------------------------------------------------
  rvApplication() :
    Gtk::Application("org.gtkmm.examples.application",
                     Gio::APPLICATION_HANDLES_COMMAND_LINE | Gio::APPLICATION_NON_UNIQUE)
    //Gtk::Application("org.gtkmm.examples.application", Gio::APPLICATION_HANDLES_OPEN)
  {
    mGain = 1.0;
    mOffset = 0.0;
    mData = NULL;
  }

  // Member functions ----------------------------------------------------------
  virtual int on_command_line(const Glib::RefPtr<Gio::ApplicationCommandLine > &command_line);
  void on_activate() override;
  void on_open(const Gio::Application::type_vec_files& files, const Glib::ustring& hint) override;

  // Debug functions -----------------------------------------------------------
  void createTestPattern(ibc::gtkmm::ImageData *inImageData,
      int inPattern,
      ibc::image::ImageType::PixelType inPixelType,
      ibc::image::ImageType::DataType inDataType,
      int inWidth, int inHeight,
      ibc::image::ColorMap::ColorMapIndex inColorMapIndex,
      int inColorMapMultiNum,
      double inGain, double inOffsset);
  void fillTestPattern(ibc::gtkmm::ImageData *inImageData, int inPattern);

  // ---------------------------------------------------------------------------
  // create_appwindow
  // ---------------------------------------------------------------------------
  rvWindow  *create_appwindow()
  {
    auto appwindow = new rvWindow();
  
    // Make sure that the application runs for as long this window is still open.
    add_window(*appwindow);
  
    // Gtk::Application::add_window() connects a signal handler to the window's
    // signal_hide(). That handler removes the window from the application.
    // If it's the last window to be removed, the application stops running.
    // Gtk::Window::set_application() does not connect a signal handler, but is
    // otherwise equivalent to Gtk::Application::add_window().
  
    // Delete the window when it is hidden.
    appwindow->signal_hide().connect(sigc::bind<Gtk::Window*>(sigc::mem_fun(*this,
      &rvApplication::on_hide_window), appwindow));
  
    return appwindow;
  }
  // ---------------------------------------------------------------------------
  // on_hide_window
  // ---------------------------------------------------------------------------
  void on_hide_window(Gtk::Window *window)
  {
    delete window;
  }
};

#endif  // #ifdef MAIN_WIN_H_
