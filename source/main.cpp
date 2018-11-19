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
#include <gtkmm.h>

class MyDrawArea : public Gtk::DrawingArea
{
 public:
  MyDrawArea()
  {
    set_size_request(500, 500);

    try
    {
      m_pixbuf = Gdk::Pixbuf::create_from_file("./test.jpg");
    }
    catch(...)
    {
      Glib::exception_handlers_invoke();
      exit(1);
    }
    //m_layout = create_pango_layout("");
    //m_layout->set_font_description(Pango::FontDescription("Serif 20"));
  }
protected:
  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override
  {
    Gdk::Cairo::set_source_color(cr, Gdk::Color("black"));
    Gdk::Cairo::set_source_pixbuf(cr, m_pixbuf->scale_simple(200, 200, Gdk::INTERP_NEAREST), 0, 0);
    cr->paint();

  //get_window()->set_background(Gdk::Color("black");
  //get_window()->clear();
  //m_layout->set_text("Hello, world");
  //get_window()->draw_layout(m_gc, 50, 50, m_layout, Gdk::Color("white");, Gdk::Color("black"));
  //get_window()->draw_pixelbuf(m_gc, m_pixbuf, 0, 0, 50, 100, m_pixbuf->get_width(), m_pixbuf->get_height(), Gdk::RGB_DITHER_NONE, 0, 0);
  //get_window()->draw_pixelbuf(m_gc, m_pixelbuf2, 0, 0, 250, 40, m_pixelbuf2->get_width(), m_pixelbuf2->get_height(), Gdk::RGB_DITHER_NONE, 0, 0);
    
    return true;
  }

private:
  Glib::RefPtr<Gdk::Pixbuf>  m_pixbuf;
  Glib::RefPtr<Pango::Layout> m_layout;
};

class MainWin : public Gtk::Window
{
public:
  MainWin()
  {
    m_scr.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    m_scr.add(m_drawarea);
    add(m_scr);
    resize(300, 300);
    //add(m_drawarea);
    show_all_children();
  }
private:
  MyDrawArea m_drawarea;
  Gtk::ScrolledWindow m_scr;
};

int main(int argc, char *argv[])
{
  //Gtk::Main kit(argc, argv);
  //MainWin mainWin;
  //Gtk::Main::run(mainWin);
  auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example.base");
  MainWin window;
  return app->run(window);
}
