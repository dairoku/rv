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
#include <cstring>
#include <gtkmm.h>

class MyDrawArea : public Gtk::Scrollable, public Gtk::Widget   // Order of Scrollable/Widget is very important
//class MyDrawArea : public Gtk::DrawingArea
{
public:

  MyDrawArea()
  : Glib::ObjectBase("my_draw_area"),
    hadjustment_(*this, "hadjustment"),
    vadjustment_(*this, "vadjustment"),
    hscroll_policy_(*this, "hscroll-policy", Gtk::SCROLL_NATURAL),
    vscroll_policy_(*this, "vscroll-policy", Gtk::SCROLL_NATURAL)
  {
    set_has_window(true);
    property_hadjustment().signal_changed().connect(sigc::mem_fun(*this, &MyDrawArea::hadjustment_changed));
    property_vadjustment().signal_changed().connect(sigc::mem_fun(*this, &MyDrawArea::vadjustment_changed));

    set_halign(Gtk::ALIGN_CENTER);
    set_valign(Gtk::ALIGN_CENTER);

    m_org_width = 250;
    m_org_height = 249;
    m_width = m_org_width;
    m_height = m_org_height;
    m_zoom = 100;
    //set_size_request(m_width, m_height);
    add_events(Gdk::SCROLL_MASK);
    //set_hscroll_policy(Gtk::ScrollablePolicy::SCROLL_MINIMUM);
    //set_vscroll_policy(Gtk::ScrollablePolicy::SCROLL_MINIMUM);

    try
    {
      m_pixbuf = Gdk::Pixbuf::create_from_file("./output/test.jpg");
    }
    catch(...)
    {
      Glib::exception_handlers_invoke();
      exit(1);
    }
    //m_layout = create_pango_layout("");
    //m_layout->set_font_description(Pango::FontDescription("Serif 20"));
  }
  Gtk::ScrolledWindow *m_scr_win;
protected:
  Glib::Property<Glib::RefPtr<Gtk::Adjustment>> hadjustment_, vadjustment_;
  Glib::Property<Gtk::ScrollablePolicy> hscroll_policy_, vscroll_policy_;

  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override
  {
    Gdk::Cairo::set_source_color(cr, Gdk::Color("black"));
    Gdk::Cairo::set_source_pixbuf(cr, m_pixbuf->scale_simple(m_width, m_height, Gdk::INTERP_NEAREST), 0, 0);
    cr->paint();

  //get_window()->set_background(Gdk::Color("black");
  //get_window()->clear();
  //m_layout->set_text("Hello, world");
  //get_window()->draw_layout(m_gc, 50, 50, m_layout, Gdk::Color("white");, Gdk::Color("black"));
  //get_window()->draw_pixelbuf(m_gc, m_pixbuf, 0, 0, 50, 100, m_pixbuf->get_width(), m_pixbuf->get_height(), Gdk::RGB_DITHER_NONE, 0, 0);
  //get_window()->draw_pixelbuf(m_gc, m_pixelbuf2, 0, 0, 250, 40, m_pixelbuf2->get_width(), m_pixelbuf2->get_height(), Gdk::RGB_DITHER_NONE, 0, 0);
    
    return true;
  }
  virtual bool on_scroll_event(GdkEventScroll *event)
  {
    std::cout << "wheel event\n"
              << "time = " << event->time << std::endl
              << "x = " << event->x << std::endl
              << "y = " << event->y << std::endl
              << "state = " << event->state << std::endl
              << "direction = " << event->direction << std::endl
              << "delta_x = " << event->delta_x << std::endl
              << "delta_y = " << event->delta_y << std::endl;
    
    if (event->direction == 1)
      m_zoom -= 25;
    else
      m_zoom += 25;
    printf("%d\n", m_zoom);

    double v;
    v = (double )m_org_width * (double )m_zoom / 100.0;
    m_width = (int )v;
    v = (double )m_org_height * (double )m_zoom / 100.0;
    m_height = (int )v;
    printf("%d, %d\n", m_width, m_height);

    //set_size_request(m_width, m_height);

    Glib::RefPtr<Gtk::Adjustment>  adj;
    adj = m_scr_win->get_hadjustment();
    printf("%f, %f, %f\n", adj->get_lower(), adj->get_upper(), adj->get_value());
    adj->set_value((adj->get_upper() - adj->get_lower()) / 2.0);

    adj = m_scr_win->get_vadjustment();
    printf("%f, %f, %f\n", adj->get_lower(), adj->get_upper(), adj->get_value());
    adj->set_value((adj->get_upper() - adj->get_lower()) / 2.0);

    int width = m_scr_win->get_allocated_width();
    int height = m_scr_win->get_allocated_height();
    printf("%d, %d\n", width, height);

    return true;
  };

  void configure_hadjustment()
  {
    const auto v = property_hadjustment().get_value();

    v->freeze_notify();
    v->set_upper(1000);
    v->set_step_increment(10);
    v->set_page_size(100);
    v->thaw_notify();
  }
  void configure_vadjustment()
  {
    const auto v = property_vadjustment().get_value();

    v->freeze_notify();
    v->set_upper(1000);
    v->set_step_increment(10);
    v->set_page_size(100);
    v->thaw_notify();
  }
  void hadjustment_changed()
  {
    const auto v = property_hadjustment().get_value();
    sigc::connection& connection = hadjustment_connection_;
    connection.disconnect();
    connection = v->signal_value_changed().connect(sigc::mem_fun(*this, &MyDrawArea::adjustment_value_changed));
    configure_hadjustment();
  }
  void vadjustment_changed()
  {
    const auto v = property_vadjustment().get_value();
    sigc::connection& connection = vadjustment_connection_;
    connection.disconnect();
    connection = v->signal_value_changed().connect(sigc::mem_fun(*this, &MyDrawArea::adjustment_value_changed));
    configure_vadjustment();
  }
  void adjustment_value_changed()
  {
    queue_draw();
  }
  sigc::connection hadjustment_connection_, vadjustment_connection_;

  //private:
  Gtk::SizeRequestMode get_request_mode_vfunc() const   // Optional
  {
    //Accept the default value supplied by the base class.
    return Gtk::Widget::get_request_mode_vfunc();
  }
  void get_preferred_width_vfunc(int& minimum_width, int& natural_width) const
  {
    minimum_width = 300;
    natural_width = 1000;
  }
  void get_preferred_height_for_width_vfunc(int width, int& minimum_height, int& natural_height) const
  {
    return get_preferred_height_vfunc(minimum_height, natural_height);
  }
  void get_preferred_height_vfunc(int& minimum_height, int& natural_height) const
  {
    minimum_height = 300;
    natural_height = 1000;
  }
  void get_preferred_width_for_height_vfunc(int height, int& minimum_width, int& natural_width) const
  {
    return get_preferred_width_vfunc(minimum_width, natural_width);
  }
  void on_size_allocate(Gtk::Allocation& allocation)
  {
    //Do something with the space that we have actually been given:
    //(We will not be given heights or widths less than we have requested, though
    // we might get more)
    
    //Use the offered allocation for this container:
    set_allocation(allocation);
    
    if(m_window)
      m_window->move_resize(allocation.get_x(), allocation.get_y(), allocation.get_width(), allocation.get_height());
  }

  void on_map()
  {
    //Call base class:
    Gtk::Widget::on_map();
  }

  void on_unmap()
  {
    //Call base class:
    Gtk::Widget::on_unmap();
  }

  void on_realize()
  {
    // Do not call base class Gtk::Widget::on_realize().
    // It's intended only for widgets that set_has_window(false).
    set_realized();
    
    if(!m_window)
    {
      GdkWindowAttr attributes;
      std::memset(&attributes, 0, sizeof(decltype(attributes)));
      const Gtk::Allocation allocation(get_allocation());
      attributes.x = allocation.get_x();
      attributes.y = allocation.get_y();
      attributes.width = allocation.get_width();
      attributes.height = allocation.get_height();
    
      attributes.event_mask = get_events() | Gdk::EXPOSURE_MASK;
      attributes.window_type = GDK_WINDOW_CHILD;
      attributes.wclass = GDK_INPUT_OUTPUT;
    
      m_window = Gdk::Window::create(get_parent_window(), &attributes, Gdk::WA_X | Gdk::WA_Y);
      set_window(m_window);
    
      //override_background_color(Gdk::RGBA("red"));
      //override_color(Gdk::RGBA("blue"));
    
     //make the widget receive expose events
      m_window->set_user_data(Gtk::Widget::gobj());
    }
  }
  void on_unrealize()
  {
    m_window.reset();

    //Call base class:
    Gtk::Widget::on_unrealize();
  }

private:
  int m_width, m_height;
  int m_org_width, m_org_height;
  int m_zoom;
  Glib::RefPtr<Gdk::Pixbuf>  m_pixbuf;
  Glib::RefPtr<Pango::Layout> m_layout;
  Glib::RefPtr<Gdk::Window> m_window;
};



class MainWin : public Gtk::Window
{
public:
  MainWin() :
    m_box(Gtk::Orientation::ORIENTATION_VERTICAL , 0),
    m_status_bar("Statusbar")
  {
    m_scr_win.add(m_drawarea);
    m_box.pack_start(m_scr_win, true, true, 0);
    m_box.pack_end(m_status_bar, false, false, 0);
    add(m_box);
    m_drawarea.m_scr_win = &m_scr_win;
    resize(300, 300);
    show_all_children();
  }
protected:

private:
  Gtk::Box            m_box;
  Gtk::ScrolledWindow m_scr_win;
  Gtk::Label          m_status_bar;
  MyDrawArea          m_drawarea;
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
