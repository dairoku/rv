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

// Includes -------------------------------------------------------------------
#include <math.h>
#include <iostream>
#include <string>
#include <cstring>
#include "rv.h"

// -----------------------------------------------------------------------------
// main
// -----------------------------------------------------------------------------
//
int main(int argc, char *argv[])
{
  auto app = RVApplication::create();
  return app->run(argc, argv);
}

// =============================================================================
// RVApplication class
// =============================================================================
// -----------------------------------------------------------------------------
// on_command_line
// -----------------------------------------------------------------------------
//  The following handler will be called after a standard gtk+ command line
//  option parse process is finished
//  Note that: the standard command line options are always removed before
//  calling this handler from the command_line object by the caller
//
int RVApplication::on_command_line(const Glib::RefPtr<Gio::ApplicationCommandLine > &command_line)
{
  boost::program_options::options_description desc("rv");
  boost::program_options::positional_options_description  positional;
  int width = 0, height = 0;

  // Get argc and argv
  int     argc = 0;
  char  **argv = command_line->get_arguments(argc);

  // Define command line options
  desc.add_options()
    ("input",     boost::program_options::value<std::vector <std::string>>()->required(), "Specify input files or directories")
    ("width,x",   boost::program_options::value<int>(&width),   "Specify a width of the input image")
    ("height,y",  boost::program_options::value<int>(&height),  "Specify a height of the input image")
    ("type,t",    boost::program_options::value<std::string>(), "Specify a image type (default: RAW8)")
    ("debug",     boost::program_options::value<int>(), "Specify a debug option (0: xor, 1: H gradation)")
    ("help,h",    "Shows this help")
    ;
  positional.add("input", -1);

  try
  {
    boost::program_options::store(
      boost::program_options::command_line_parser(argc, argv).options(desc).positional(positional).run(),
      mVarMap);
    boost::program_options::notify(mVarMap);
  }
  catch (std::exception &ex)
  {
    std::cout << ex.what() << std::endl;
    std::cout << std::endl;
    std::cout << desc << std::endl;
    return EXIT_FAILURE;
  }

  if (mVarMap.count("help"))
  {
    std::cout << desc << std::endl;
    return EXIT_FAILURE;
  }

  std::vector<std::string> inputs = mVarMap["input"].as<std::vector<std::string>>();
  std::cout << inputs[0]  << std::endl;

  activate(); // NOTE: Without activate() the window won't be shown.
  return EXIT_SUCCESS;
}

// -----------------------------------------------------------------------------
// on_activate
// -----------------------------------------------------------------------------
void RVApplication::on_activate()
{
  // allocate image buffer
  // if (debug mode)
  createTestPattern(&mImageData, 0);

  mImageData.markAsImageModified();

  // The application has been started, so let's show a window.
  auto appwindow = create_appwindow();
  appwindow->setImageDataptr(&mImageData);
  appwindow->present();
}

// -----------------------------------------------------------------------------
// createTestPattern
// -----------------------------------------------------------------------------
void RVApplication::createTestPattern(ibc::gtkmm::ImageData *inImageData, int inPattern)
{
  // Create ImageData here
#if 0
  ibc::image::ImageType   imageType(ibc::image::ImageType::PIXEL_TYPE_RGB,
                                    ibc::image::ImageType::BUFFER_TYPE_PIXEL_ALIGNED,
                                    ibc::image::ImageType::DATA_TYPE_8BIT);
#endif
#if 1
  ibc::image::ImageType   imageType(ibc::image::ImageType::PIXEL_TYPE_MONO,
                                    ibc::image::ImageType::BUFFER_TYPE_PIXEL_ALIGNED,
                                    ibc::image::ImageType::DATA_TYPE_8BIT);
#endif
#if 0
  ibc::image::ImageType   imageType(ibc::image::ImageType::PIXEL_TYPE_MONO,
                                    ibc::image::ImageType::BUFFER_TYPE_PIXEL_ALIGNED,
                                    ibc::image::ImageType::DATA_TYPE_16BIT);
#endif
  ibc::image::ImageFormat imageFormat(imageType, 640, 480);
  inImageData->allocateImageBuffer(imageFormat);
  //inImageData->mActiveConverter->setColorMapIndex(ibc::image::ColorMap::CMIndex_GrayScale);
  //inImageData->mActiveConverter->setColorMapIndex(ibc::image::ColorMap::CMIndex_Jet);
  inImageData->mActiveConverter->setColorMapIndex(ibc::image::ColorMap::CMIndex_CoolWarm);
  //inImageData->mActiveConverter->setColorMapIndex(ibc::image::ColorMap::CMIndex_Jet, 3);
  //inImageData->mActiveConverter->setColorMapIndex(ibc::image::ColorMap::CMIndex_CoolWarm, 4);
  //inImageData->mActiveConverter->setGain(2.0);
  //inImageData->mActiveConverter->setOffset(10000);
  //inImageData->mActiveConverter->setOffset(-128);
  //inImageData->mActiveConverter->setGamma(2.8);

  fillTestPattern(inImageData, inPattern);
}

// -----------------------------------------------------------------------------
// fillTestPattern
// -----------------------------------------------------------------------------
void RVApplication::fillTestPattern(ibc::gtkmm::ImageData *inImageData, int inPattern)
{
  if (inImageData->getImageType().mPixelType == ibc::image::ImageType::PIXEL_TYPE_MONO &&
      inImageData->getImageType().mDataType  == ibc::image::ImageType::DATA_TYPE_8BIT)
  {
    unsigned char *bufPtr = (unsigned char *)inImageData->getImageBufferPtr();
    for (int y = 0; y < inImageData->getHeight(); y++)
      for (int x = 0; x < inImageData->getWidth(); x++)
      {
        switch (inPattern)
        {
          case 0:
            *bufPtr = (unsigned char)(x ^ y);
            break;
          case 1:
            *bufPtr = (unsigned char)(x & 0xFF);
            break;
          case 2:
            *bufPtr = (unsigned char)(y & 0xFF);
            break;
          case 3:
            *bufPtr = (unsigned char)((x+y) & 0xFF);
            break;
        }
        bufPtr++;
      }
    return; 
  }
  if (inImageData->getImageType().mPixelType == ibc::image::ImageType::PIXEL_TYPE_MONO &&
      inImageData->getImageType().mDataType  == ibc::image::ImageType::DATA_TYPE_16BIT)
  {
    unsigned short *bufPtr = (unsigned short *)inImageData->getImageBufferPtr();
    for (int y = 0; y < inImageData->getHeight(); y++)
      for (int x = 0; x < inImageData->getWidth(); x++)
      {
        switch (inPattern)
        {
          case 0:
            *bufPtr = (unsigned short)(x ^ y);
            break;
          case 1:
            *bufPtr = (unsigned short)(x & 0xFFFF);
            break;
          case 2:
            *bufPtr = (unsigned short)(y & 0xFFFF);
            break;
          case 3:
            *bufPtr = (unsigned short)((x+y) & 0xFFFF);
            break;
        }
        bufPtr++;
      }
    return; 
  }
}

void RVApplication::on_open(const Gio::Application::type_vec_files& files,
  const Glib::ustring& /* hint */)
{
  // The application has been asked to open some files,
  // so let's open a new view for each one.
  /*RVWindow* appwindow = nullptr;
  auto windows = get_windows();
  if (windows.size() > 0)
    appwindow = dynamic_cast<RVWindow*>(windows[0]);

  if (!appwindow)
    appwindow = create_appwindow();

  for (const auto& file : files)
    appwindow->open_file_view(file);

  appwindow->present();*/
}
